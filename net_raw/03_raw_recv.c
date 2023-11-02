#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ether.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define ERR_LOG(str) \
    do               \
    {                \
        perror(str); \
        exit(1);     \
    } while (0)

int main(int argc, char const *argv[])
{
    int sockfd;

    if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        ERR_LOG("fail to socket");
    }

    while (1)
    {
        unsigned char msg[1024] = "";
        if (recv(sockfd, msg, sizeof(msg), 0) < 0)
        {
            ERR_LOG("fail to recv");
        }

        unsigned char dst_mac[18] = "";
        unsigned char src_mac[18] = "";
        unsigned short type;
        unsigned char dst_ip[17] = "";
        unsigned char src_ip[16] = "";
        unsigned char pro_type[10] = "";

        sprintf(dst_mac, "%x:%x:%x:%x:%x:%x", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5]);
        sprintf(src_mac, "%x:%x:%x:%x:%x:%x", msg[6], msg[7], msg[8], msg[9], msg[10], msg[11]);
        type = ntohs(*(unsigned short *)(msg + 12));

        sprintf(src_ip, "%d.%d.%d.%d", msg[26], msg[27], msg[28], msg[29]);
        sprintf(dst_ip, "%d.%d.%d.%d", msg[30], msg[31], msg[32], msg[33]);

        sprintf(pro_type, "%d", msg[23]);

        printf("源mac: %s --> 目的mac: %s\n", src_mac, dst_mac);
        // printf("type = %#x\n", type);
        printf("源ip: %s --> 目的ip: %s\n", src_ip, dst_ip);

        if (type == 0x0800)
        {
            printf("IP数据报\n");
            switch (atoi(pro_type))
            {
            case 1:
                printf("ICMP格式\n");
                break;
            case 2:
                printf("IGMP格式\n");
                break;
            case 6:
                printf("TCP格式\n");
                printf("源端口:%d --> 目的端口：%d\n", ntohs(*(unsigned short *)msg + 38), ntohs(*(unsigned short *)msg + 40));
                break;
            case 17:
                printf("UDP格式\n");
                break;
            default:
                exit(1);
                break;
            }
        }
        else if (type == 0x0806)
        {
            printf("ARP数据报\n");
        }
        else if (type == 0x8035)
        {
            printf("RARP数据报\n");
        }
    }

    return 0;
}
