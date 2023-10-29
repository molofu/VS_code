#include <stdio.h>
#include <stdlib.h>     //exit
#include <sys/socket.h> //sock
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h>  //hton inet_addr
#include <fcntl.h>      //open 函数
#include <unistd.h>     //write函数

void do_download(int sockfd, struct sockaddr_in serveraddr)
{
    // 输入下载文件名
    char filename[128] = "";
    printf("请输入要下载的文件名：");
    scanf("%s", filename);

    unsigned char text[1024] = ""; // 接受sprintf的数据
    int text_len;
    socklen_t addrlen = sizeof(serveraddr);
    ssize_t bytes;
    int flags = 0;
    int fd;
    int num = 0;

    // 构建给服务器发送的TFTP指令并发送指令
    text_len = sprintf(text, "%c%c%s%c%s%c", 0, 1, filename, 0, "octet", 0);

    if (sendto(sockfd, text, text_len, 0, (struct sockaddr *)&serveraddr, addrlen) < 0) // 发送text 到 server
    {
        perror("fail to sendto");
        exit(1);
    }

    while (1)
    {
        // 接受服务器传来的数据
        if ((bytes = recvfrom(sockfd, text, sizeof(text), 0, (struct sockaddr *)&serveraddr, &addrlen)) < 0)
        {
            perror("fail to recvfrom");
            exit(1);
        }
        // printf("已下载字节数：%d\n", bytes);

        // 判断块编号和操作码
        if (text[1] == 5) // 判断操作码 等于5代表错误
        {
            printf("error: %s\n", text + 4); // 4字节后是差错信息
            return;
        }
        else if (text[1] == 3) // 操作码3 代表发送的请求正确
        {
            // printf("%d %d\n", text[1],num);

            if (flags == 0)
            {
                // 创建文件
                if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0) // open 成功返回文件描述符（正整数），失败返回-1
                {
                    perror("fail to open");
                    exit(1);
                }
                flags = 1;
            }
            // 对比块编号和接受的数据大小 将内容写入文件
            if ((num + 1 == ntohs(*(unsigned short *)(text + 2))) && (bytes == 516))
            {
                num = ntohs(*(unsigned short *)(text + 2));
                // printf("==%d\n", num);
                if (write(fd, text + 4, bytes - 4) < 0)
                {
                    perror("fail to write1");
                    exit(1);
                }
                // 文件写入完毕后，向服务器发送ACK
                text[1] = 4;                                                                 // 操作码第一次回复时改为4 ,块编号接受的多少就返回多少 不需要改变
                if (sendto(sockfd, text, 4, 0, (struct sockaddr *)&serveraddr, addrlen) < 0) // ACK长度4字节serveraddr网络信息代表要发往的地址
                {
                    perror("fail to sendto");
                    exit(1);
                }
            }
            // 当bytes小于516时，也即数据内容小于512时，代表最后一次传输文件完成
            if ((num+1==ntohs(*(unsigned short *)(text + 2))) && (bytes < 516))
            {
                // printf("<%d\n", num);
                if (write(fd, text + 4, bytes - 4) < 0)
                {
                    perror("fail to write2");
                    exit(1);
                }
                // 写入成功
                text[1] = 4;
                if (sendto(sockfd, text, 4, 0, (struct sockaddr *)&serveraddr, addrlen) < 0)
                {
                    perror("fail to sendto");
                    exit(1);
                }
                printf("文件下载完毕\n");
                // 关闭文件
                close(fd);
                return;
            }
            // printf("%d\n", num);
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s ip\n", argv[0]);
        exit(1);
    }

    // 创建套接字
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("fail to socket");
        exit(1);
    }

    // 创建网络信息结构体
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]); // 输入 TFTP服务器的IP地址
    serveraddr.sin_port = htons(69);                 // TFTP服务器的默认端口号是69；

    // 下载操作
    do_download(sockfd, serveraddr);

    return 0;
}
