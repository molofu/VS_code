#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define N 128

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
        perror("fail to socket");
        exit(1);
    }
    // printf("sockfd = %d\n", sockfd);

    // 2.发送客户端连接请求
    // 创建网络信息结构体
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));
    socklen_t addrlen = sizeof(serveraddr);

    if (connect(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
    {
        perror("fail to connect");
        exit(1);
    }

    // 3.通信
    // 发送数据
    char buf[N] = "";
    while (1)
    {
        fgets(buf, N, stdin);
        buf[strlen(buf) - 1] = '\0';

        if (send(sockfd, buf, N, 0) < 0)
        {
            perror("fail to send");
            exit(1);
        }

        // 接受数据
        char text[N] = "";
        if (recv(sockfd, text, N, 0) < 0)
        {
            perror("fail to recv");
            exit(1);
        }

        printf("from server: %s\n", text);
    }

    // 4.关闭套接字
    close(sockfd);

    return 0;
}
