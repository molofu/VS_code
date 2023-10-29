#include<stdio.h>
#include<arpa/inet.h>

int main()
{
    char ip_str[]="192.168.3.103";
    unsigned int ip_int = 0;
    unsigned char *ip_p = NULL;


    //将点分十进制IP地址转换为32为无符号整形
    inet_pton(AF_INET,ip_str,&ip_int);
    printf("ip_int = %d\n",ip_int);


    //强转为char类型
    //以整形格式打印char
    ip_p=(char*)&ip_int;
    printf("ip_int = %d,%d,%d,%d\n",*ip_p,*(ip_p+1),*(ip_p+2),*(ip_p+3));


    return 0;
}