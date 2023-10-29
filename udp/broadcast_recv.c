#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    //printf("hh\n");
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s ip port\n", argv[0]);
        exit(1);
    }

    // 1.创建套接字
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("fail to socket");
        exit(1);
    }

    // 2.创建网络结构体
    struct sockaddr_in broadcastaddr;
    broadcastaddr.sin_family = AF_INET;
    broadcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
    broadcastaddr.sin_port = htons(atoi(argv[2]));

    socklen_t addrlen = sizeof(broadcastaddr);

    // 3.绑定套接字和网络信息结构体
    if (bind(sockfd, (struct sockaddr *)&broadcastaddr, addrlen) < 0)
    {
        perror("fail to bind");
        exit(1);
    }

    // 4.通信
    char buf[1024] = "";

    // 创建接受发来的信息的网络结构体
    struct sockaddr_in sendaddr;
    //printf("hh\n");
    while (1)
    {
        //printf("hh\n");
        if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&sendaddr, &addrlen) < 0)
        {
            perror("fail to recvfrom");
            exit(1);
        }
        //printf("hh\n");
        printf("[%s:%d]: %s\n", inet_ntoa(sendaddr.sin_addr), ntohs(sendaddr.sin_port), buf);
    }

    close(sockfd);

    return 0;
}
