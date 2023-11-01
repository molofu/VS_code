#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

#define N 1024
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

void *pthread_fun(void *arg)
{
    MSG msg = *(MSG *)arg;
    char buf[N] = "";
    char head[] = "HTTP/1.1 200 OK\r\n"
                  "Content-Type: text/html\r\n"
                  "\r\n";
    char err[] = "HTTP/1.1 404 Not Found\r\n"
                 "Conten-Type: text/html\r\n"
                 "\r\n"
                 "<HTML><BODY>File not found</BODY></HTML>";
    if (recv(msg.acceptfd, buf, N, 0) < 0)
    {
        ERR_LOG("fail to recv");
    }
    printf("****************************\n\n");
    printf("%s\n", buf);
    printf("\n****************************\n");
    // printf("%s\n", buf);
    // printf("这里\n");
    // printf("%s\n", buf);
    char filename[128] = "";
    char cmp[128] = "";
    sscanf(buf, "GET /%s %s", filename, cmp);

    // printf("%s\n", filename);
    printf("%s\n", cmp);

    // if (strncmp(cmp, "HTTP/1.1", 8) == 0)
    // {
    //     //ERR_LOG("fail to strncmp");

    // }

    // printf("这里\n");
    char path[] = "./";
    strcat(path, filename);
    printf("%s\n", path);
    int fd;
    if ((fd = open(path, O_RDONLY)) < 0) // 文件打开失败的情况
    {
        if (errno == ENOENT)
        {
            if (send(msg.acceptfd, err, strlen(err), 0) < 0)
            {
                ERR_LOG("fail to send");
            }

            close(msg.acceptfd);
            pthread_exit(0);
        }
        else
        {
            ERR_LOG("fail to open");
        }
    }

    if (send(msg.acceptfd, head, strlen(head), 0) < 0)
    {
        ERR_LOG("fail to send");
    }
    ssize_t bytes;
    char text[N] = "";
    while ((bytes = read(fd, text, 1024)) > 0)
    {
        if (send(msg.acceptfd, text, bytes, 0) < 0)
        {
            ERR_LOG("fail to send");
        }
    }

    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s ip port\n", argv[0]);
        exit(1);
    }

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERR_LOG("fail to socket");
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));
    socklen_t addrlen = sizeof(serveraddr);

    if (bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
    {
        ERR_LOG("fail to bind");
    }

    if (listen(sockfd, 5) < 0)
    {
        ERR_LOG("fail to listen");
    }

    int acceptfd;
    struct sockaddr_in clientaddr;

    while (1)
    {
        if ((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
        {
            ERR_LOG("fail to accept");
        }
        // printf("%s-%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        MSG msg;
        msg.addr = clientaddr;
        msg.acceptfd = acceptfd;
        pthread_t thread;
        // int i = 0;
        // printf("第%d次\n", i++);
        if (pthread_create(&thread, NULL, pthread_fun, &msg) != 0)
        {
            ERR_LOG("fail to pthread_create");
        }
        pthread_detach(thread);
    }

    return 0;
}
