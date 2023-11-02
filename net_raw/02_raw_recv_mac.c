#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ether.h>
#include <stdlib.h>
#include<arpa/inet.h>


#define ERR_LOG(str) \
    do               \
    {                \
        perror(str); \
        exit(1);     \
    } while (0)

int main(int argc, char const *argv[])
{
    int sockfd;
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        ERR_LOG("fail to socket");
    }

    unsigned char msg[1600] = "";
    while (1)
    {
        if (recv(sockfd, msg, 1600, 0) < 0)
        {
            ERR_LOG("fail to recv");
        }

        unsigned char dst_mac[18] = "";
        unsigned char src_mac[18] = "";
        unsigned short type;

        sprintf(dst_mac, "%x:%x:%x:%x:%x:%x", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5]);
        sprintf(src_mac, "%x:%x:%x:%x:%x:%x", msg[6], msg[7], msg[8], msg[9], msg[10], msg[11]);
        type = ntohs(*(unsigned short *)(msg + 12));

        printf("源mac: %s --> 目的mac: %s\n", src_mac, dst_mac);
        printf("type = %#x\n", type);
    }

    return 0;
}
