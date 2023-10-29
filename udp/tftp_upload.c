#include <stdio.h>
#include <stdlib.h>     //exit
#include <sys/socket.h> //sock
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h>  //hton inet_addr
#include <fcntl.h>      //open 函数
#include <unistd.h>     //write函数

void do_upload(int sockfd, struct sockaddr_in serveraddr)
{
    // 输入上传文件名
    char filename[128] = "";
    printf("请输入要上传的文件名：");
    scanf("%s", filename);

    // 判断文件是否存在

    unsigned char text[1024] = ""; // 接受sprintf的数据
    int text_len;
    socklen_t addrlen = sizeof(serveraddr);
    ssize_t bytes;
    int fd, read_len;
    int num = 0;

    if ((fd = open(filename, O_RDONLY)) < 0)
    {
        perror("fail to open");
        close(sockfd);
        exit(1);
    }

    // 构建给服务器发送的TFTP指令并发送指令
    text_len = sprintf(text, "%c%c%s%c%s%c", 0, 2, filename, 0, "octet", 0);

    if (sendto(sockfd, text, text_len, 0, (struct sockaddr *)&serveraddr, addrlen) < 0) // 发送text 到 server
    {
        perror("fail to sendto");
        exit(1);
    }

    do
    {
        // 接受服务器传来的数据
        if ((bytes = recvfrom(sockfd, text, sizeof(text), 0, (struct sockaddr *)&serveraddr, &addrlen)) < 0)
        {
            perror("fail to recvfrom");
            exit(1);
        }

        // 判断块编号和操作码
        if (text[1] == 5) // 判断操作码 等于5代表错误
        {
            printf("error: %s\n", text + 4); // 4字节后是差错信息
            return;
        }
        else if (text[1] == 4) // 操作码4
        {
            // 取出块编号
            num = ntohs(*(unsigned short *)(text + 2));
            // 读本地文件
            read_len = read(fd, text + 4, 512);
            // 上传文件,操作码3 数据包
            text[1] = 3;
            // 设置数据包的块编号
            (*(unsigned short *)(text + 2)) = htons(num + 1);
            // 发送数据包
            if (sendto(sockfd, text, read_len + 4, 0, (struct sockaddr *)&serveraddr, addrlen) < 0)
            {
                perror("fail to send");
                exit(1);
            }
        }
    } while (read_len == 512);
    bytes = recvfrom(sockfd, text, sizeof(text), 0, (struct sockaddr *)&serveraddr, &addrlen);
    close(fd);
    printf("文件上传完毕\n");
    return;
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
    do_upload(sockfd, serveraddr);

    return 0;
}
