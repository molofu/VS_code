#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#define N 128

void handler(int sig)
{
    wait(NULL);
}

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

    signal(SIGCHLD, handler);

    // 4.阻塞等待客户端的链接请求
    int acceptfd;
    struct sockaddr_in clientaddr;
    while (1)
    {
        if ((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
        {
            perror("fail to accept");
            exit(1);
        }
        printf("ip:%s, port:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // 5.创建子进程
        pid_t pid;
        if ((pid = fork()) < 0)
        {
            perror("fail to fork");
            exit(1);
        }
        else if (pid > 0) // 父进程 执行accept ，if语句结束后阻塞在accept函数处
        {
        }
        else // 子进程 跟指定的客户端通信
        {
            char buf[N] = "";
            ssize_t bytes;
            while (1)
            {
                if ((bytes = recv(acceptfd, buf, N, 0)) < 0)
                {
                    perror("fail to recv");
                    exit(1);
                }
                else if (bytes == 0)
                {
                    printf("The client quited\n");
                    exit(1);
                }

                if (strncmp(buf, "quit", 4) == 0)
                {
                    exit(0);
                }
                printf("from client: %s\n", buf);

                strcat(buf, " *_*");
                if (send(acceptfd, buf, N, 0) < 0)
                {
                    perror("fail to send");
                    exit(1);
                }
            }
        }
    }

    return 0;
}