#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s ip port\n", argv[0]);
        exit(1);
    }

    // 文件描述符
    int sockfd;
    // 1.创建套接字，正确返回文件描述符
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("fail to socket");
        exit(1);
    }

    // 2.设置允许发送广播信息
    int on = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
    {
        perror("fail to setsockopt");
        exit(1);
    }

    // 3.创建网络信息结构体
    struct sockaddr_in broadcastaddr;
    broadcastaddr.sin_family = AF_INET;
    broadcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
    broadcastaddr.sin_port = htons(atoi(argv[2])); // atoi 将数字字符串转化为数字整形，头文件stdlib 此处argv是字符串类型需转换
    socklen_t addrlen = sizeof(broadcastaddr);
    // printf("%u\n", broadcastaddr.sin_addr.s_addr);
    // printf("%hd\n", broadcastaddr.sin_port);

    // 关于字节序转换
    // 网络信息结构体中的 port和addr 协议规定必须转换
    // family 处于本机内核 ，没有上传网络 ，可以不转化
    // 单字节数据 可以不转化  例如 char[10] ch ;每一个数据都是char 是单字节可以不转换
    // 对于数据大于1个字节的 如 double ,long long等
    // 当通信两端的字节序存储方式不同时需要转化，若提前知道两端的存储方式相同 也可以不转化
    // 时下 大多数PC端字节序为小端存储，所以 多数据一般也可以不转换 （对于这一点此处还不太确定）

    // 4.通信
    char buf[1024] = "";
    while (1)
    {
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = '\0'; // 将fgets 后面的换行符转化为\0

        if (sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&broadcastaddr, addrlen) < 0)
        {
            perror("fail to sendto");
            exit(1);
        }
    }
    // 关闭套接字
    close(sockfd);

    return 0;
}
