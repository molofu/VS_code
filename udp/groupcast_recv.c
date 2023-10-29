#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>

// struct ip_mreq
// {
//     struct in_addr imr_multiaddr;
//     struct in_addr imr_interface;
// };

// Linux内核已定义 struct ip_mreq 不需要重定义

int main(int argc, char const *argv[])
{
    // 1.创建套接字
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("fail to socket");
        exit(1);
    }

    // 2.设置加入多播组

    // Linux内核已定义 struct ip_mreq 不需要重定义
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(argv[1]);
    mreq.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        perror("fail to setsockopt");
        exit(1);
    }

    // 3.填充组播网络信息结构体
    struct sockaddr_in groupcastaddr;
    groupcastaddr.sin_family = AF_INET;
    groupcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
    groupcastaddr.sin_port = htons(atoi(argv[2]));
    socklen_t addrlen = sizeof(groupcastaddr);

    // 4.套接字绑定网络信息结构体
    if (bind(sockfd, (struct sockaddr *)&groupcastaddr, addrlen) < 0)
    {
        perror("fail to bind");
        exit(1);
    }

    // 5.通信
    char buf[1024] = "";
    struct sockaddr_in sendaddr;

    while (1)
    {
        if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&sendaddr, &addrlen) < 0)
        {
            perror("fail to recvfrom");
            exit(1);
        }
        printf("[%s:%d]: %s\n", inet_ntoa(sendaddr.sin_addr), ntohs(sendaddr.sin_port), buf);
    }

    return 0;
}