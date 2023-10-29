#include<stdio.h>

union 
{
    int a;
    char b;
}un;

int main()
{

    //这是一个注释
    un.a=0x12345678;

    if(un.b == 0x78)
    {
        printf("小端存储\n");
    }
    else
    {
        printf("大端存储\n");
    }


    return 0;
}