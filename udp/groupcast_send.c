#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    // 1.创建套接字
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("fail to socket");
        exit(1);
    }

    // 2.填充组播网络信息结构体
    struct sockaddr_in groupcastaddr;
    groupcastaddr.sin_family = AF_INET;
    groupcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
    groupcastaddr.sin_port = htons(atoi(argv[2]));
    socklen_t addrlen = sizeof(groupcastaddr);

    // 3.通信
    char buf[1024] = "";
    while (1)
    {
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = '\0';

        if (sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&groupcastaddr, addrlen) < 0)
        {
            perror("fail to sendto");
            exit(1);
        }
    }

    close(sockfd);

    return 0;
}
