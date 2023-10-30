#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

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

    // 2.绑定套接字与网络信息结构体
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));
    socklen_t addrlen = sizeof(serveraddr);

    if (bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
    {
        perror("fail to bind");
        exit(1);
    }

    // 3.将套接字设置为被动监听状态
    if (listen(sockfd, 10) < 0) // 参数2，为服务器设置了一个大小为 10 的等待连接队列
    {
        perror("fail to listen");
        exit(1);
    }

    // 4.阻塞等待客户端的链接请求
    int acceptfd;
    struct sockaddr_in clientaddr;
    if ((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
    {
        perror("fail to accept");
        exit(1);
    }

    // 可以打印链接的客户端的信息
    // printf("huhu\n");
    printf("ip:%s, port:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    // 5.通信
    char buf[128] = "";

    // tcp服务器与客户端通信时，使用的是accept函数返回的文件描述符
    while(1)
    {
        if (recv(acceptfd, buf, 128, 0) < 0)
        {
            perror("fail to recv");
            exit(1);
        }

        printf("from client: %s\n", buf);

        // 回复信息
        strcat(buf, " *_*");

        if (send(acceptfd, buf, 128, 0) < 0)
        {
            perror("fail to send");
            exit(1);
        }
    }


    // 关闭套接字文件描述符
    close(sockfd);
    close(acceptfd);

    return 0;
}
