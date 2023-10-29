#include<stdio.h>
#include<arpa/inet.h>

int main()
{
    unsigned char ip_int[]={192,168,88,128};
    char ip_str[16]="";


    //将整形转换为字符串（IP地址）
    inet_ntop(AF_INET,&ip_int,ip_str,16);
    //printf("ip_str = %d\n",*ip_str); 
    printf("ip_str = %s\n",ip_str); 



    return 0;
}