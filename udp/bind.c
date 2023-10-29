#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    if(argc<3)
    {
        fprintf(stderr, "Usage:%s ip port\n", argv[0]); // argv[0] 填到%s上
        exit(1);
    }

    //创建套接字
    int sockfd;
    if ((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("fail to socket");
        exit(1);
    }
    
    //将服务器的网络信息结构体进行填充
    struct sockaddr_in serveraddr;

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);    //将点分十进制的IP地址 字符串转化为 无符号整形
    serveraddr.sin_port = htons(atoi(argv[2]));


    //将网络信息结构体于套接字绑定
    if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))==-1)
    {
        perror("fail to bind");
        exit(1);
    }


    return 0;
}
