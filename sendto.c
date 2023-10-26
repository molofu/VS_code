#include <stdio.h>  //printf
#include <stdlib.h> //exit
#include <sys/types.h>
#include <sys/socket.h> //socket
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h>  //htons inet_addr
#include <unistd.h>     //close
#include <string.h>

int main(int argc, char const *argv[])
{

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s ip port\n", argv[0]); // argv[0] 程序运行时的全路径名
        exit(1);    //stderr 标准错误流（屏幕）
    }

    // 1.创建套接字
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {

        perror("fail to socket");
        exit(1);
    }

    printf("sockfd = %d\n", sockfd);

    // 2.填充 服务器网络信息结构体 sockaddr_in
    struct sockaddr_in serveraddr; // netinet 中已定义好的结构体
    socklen_t addrlen = sizeof(serveraddr);

    serveraddr.sin_family = AF_INET; // 协议族，ipv4网络协议
    // serveraddr.sin_addr.s_addr=inet_addr("192.168.171.1");   //服务器的IP地址
    // serveraddr.sin_port=htons(8080);

    // 通用 IP地址 由命令行输入IP地址
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]); // argv是char类型的，atoi 将一个 数字型的字符串类型转化为整形
    serveraddr.sin_port = htons(atoi(argv[2]));      // hton将主机字节序转化为网络字节序  网络字节序为大端，主机在该计算机上为小端存储

    // 3.发送数据
    char buf[128] = "";
    while (1)
    {
        fgets(buf, 128, stdin);
        buf[strlen(buf) - 1] = '\0'; // 把buf 接受到的最后一个回车符转化为\0

        if (sendto(sockfd, buf, 128, 0, (struct sockaddr *)&serveraddr, addrlen) == -1)
        {
            perror("fail to sendto");
            exit(1);
        }
    }

    // 4.关闭套接字文件描述符
    close(sockfd);

    return 0;
}
