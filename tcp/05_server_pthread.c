#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define N 128

#define ERR_LOG(str) \
    do               \
    {                \
        perror(str); \
        exit(1);     \
    } while (0)

typedef struct
{
    struct sockaddr_in addr;
    int acceptfd;
} MSG;

void *pthread_func(void *arg)
{
    char buf[N] = "";
    ssize_t bytes;
    MSG msg = *(MSG *)arg;
    while (1)
    {
        if ((bytes = recv(msg.acceptfd, buf, N, 0)) < 0)
        {
            ERR_LOG("fail to recv");
        }
        else if (bytes == 0)
        {
            printf("The client quited\n");
            pthread_exit(0);
        }

        if ((strncmp(buf, "quit", 4)) == 0)
        {
            printf("The client quited\n");
            pthread_exit(0);
        }

        printf("[%s - %d]: %s\n", inet_ntoa(msg.addr.sin_addr), ntohs(msg.addr.sin_port), buf);

        strcat(buf, " *_*");
        // 回复信息
        if (send(msg.acceptfd, buf, N, 0) < 0)
        {
            ERR_LOG("fail to send");
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s ip port\n", argv[0]);
        exit(1);
    }

    // 1.创建套接字
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERR_LOG("fail to socket");
    }

    // 2.绑定
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));
    socklen_t addrlen = sizeof(serveraddr);

    if (bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
    {
        ERR_LOG("fail to bind");
    }

    // 3.监听链接
    if (listen(sockfd, 5) < 0)
    {
        ERR_LOG("fail to listen");
    }

    // 4.等待客户端连接
    int acceptfd;
    struct sockaddr_in clientaddr;
    while (1)
    {
        if ((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
        {
            ERR_LOG("fail to accept");
        }
        // 创建子线程
        MSG msg;
        msg.addr = clientaddr;
        msg.acceptfd = acceptfd;
        pthread_t thread;
        if (pthread_create(&thread, NULL, pthread_func, &msg) != 0)
        {
            ERR_LOG("failt to pthread_create");
        }
        pthread_detach(thread);
    }
    return 0;
}
