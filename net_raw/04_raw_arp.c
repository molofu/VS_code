#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ether.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <string.h>
#include <sys/ioctl.h>

#define ERR_LOG(str) \
    do               \
    {                \
        perror(str); \
        exit(1);     \
    } while (0)

int main(int argc, char const *argv[])
{
    // 创建原始套接字
    int sockfd;
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        ERR_LOG("fail to socket");
    }

    // 组ARP数据包
    unsigned char msg[1600] = {
        // 以太网首部
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 目的mac地址
        0x00, 0x0c, 0x29, 0x4d, 0xd6, 0x49, // 源mac
        0x08, 0x06,                         // 类型
        0x00, 0x01,                         // 硬件类型
        0x08, 0x00,                         // 协议类型
        0x06,                               // 硬件地址长度
        0x04,                               // 协议地址长度
        0x00, 0x01,                         // op选项
        0x00, 0x0c, 0x29, 0x4d, 0xd6, 0x49, // 发送端MAC:
        192, 168, 88, 128,                  // 发送端ip
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 目的MAC
        192, 168, 88, 1                     // 目的ip
    };

    // 获取网络接口信息
    struct ifreq ethreq;
    strncpy(ethreq.ifr_name, "ens33", IFNAMSIZ);
    if (ioctl(sockfd, SIOGIFINDEX, &ethreq) < 0)
    {
        ERR_LOG("fail to ioctl");
    }

    // 设置本机网络接口
    struct sockaddr_ll sll;
    memset(&sll, 0, sizeof(sll));
    sll.sll_ifindex = ethreq.ifr_ifindex;

    // 发送数据
    if (sendto(sockfd, msg, 14 + 28, 0, (struct sockaddr *)&sll, sizeof(sll)) < 0)
    {
        ERR_LOG("fail to sendto");
    }

    // 接受数据
    unsigned char recv_msg[1600] = "";
    unsigned char rsc_mac[18] = "";
    while (1)
    {
        // printf("到这里\n");
        if (recv(sockfd, recv_msg, sizeof(recv_msg), 0) < 0)
        {
            ERR_LOG("fail to recv");
        }
        // printf("到这里\n");
        if (ntohs(*(unsigned short *)(recv_msg + 12)) == 0x0806) // 0x0806 判断是否是ARP数据包
        {

            if (ntohs(*(unsigned short *)(recv_msg + 20)) == 2) // op 类型是2 则为ARP应答
            {

                sprintf(rsc_mac, "%x:%x:%x:%x:%x:%x", recv_msg[6], recv_msg[7], recv_msg[8], recv_msg[9], recv_msg[10], recv_msg[11]);
                printf("源IP: 192.168.88.%d --> 目的MAC: %s\n", recv_msg[41], rsc_mac);
                break;
            }
        }
    }
    // printf("到这里\n");
    close(sockfd);
    return 0;
}
