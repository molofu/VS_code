#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define N 128

int main(int argc, char const *argv[])
{

    if (argc < 3)
    {
        fprintf(stderr, "Usage:%s ip port\n", argv[0]); // argv[0] 填到%s上
        exit(1);
    }

    // 创建套接字
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("fail to socket");
        exit(1);
    }

    // 将服务器的网络信息结构体进行填充
    struct sockaddr_in serveraddr;

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]); // 将点分十进制的IP地址 字符串转化为 无符号整形
    serveraddr.sin_port = htons(atoi(argv[2]));

    // 将网络信息结构体于套接字绑定
    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("fail to bind");
        exit(1);
    }

    // 接受数据
    char buf[N] = "";
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    while (1)
    {
        if (recvfrom(sockfd, buf, 128, 0, (struct sockaddr *)&clientaddr, &addrlen) == -1) // 函数只需要网络信息的通用结构体
        {
            perror("fail to recvfrom");
            exit(1);
        }

        // 打印数据
        // 打印客户端的IP地址和端口号
        // printf("hello world\n");
        printf("ip:%s,port:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        // inet_ntoa 将无符号整形转化为点分十进制字符串，但参数只需要sin_addr的结构体，不需要其中的成员sin_addr.s_addr

        // 打印接受到数据
        printf("from client: %s\n", buf);
        

        //向客户端回复数据 ，此处直接追加一个字符回复
        strcat(buf, " *_*");

        if(sendto(sockfd,buf,128,0,(struct sockaddr*)&clientaddr,addrlen)==-1)
        {
            perror("fail to sendto");
            exit(1);
        }
        
        memset(buf, 0, 128); // buf内容置为0,ASCII 0 代表\0;
    }

    return 0;
}