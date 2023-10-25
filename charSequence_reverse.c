#include<stdio.h>
#include<arpa/inet.h>

int main()
{
    int a = 0x12345678;
    short b = 0x1234;

    printf("%#x\n",htonl(a));               //%x 16进制
    printf("%#x\n",htons(b));               //%#x 16进制 前加0x


    return 0;
}