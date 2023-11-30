#include "01_list_Node.h"

void func(int sig)
{
}
void press_key_clear()
{
    printf("press ctrl+C to continue\n");
    pause();
    system("clear");
}
void signal_fun()
{
    signal(SIGINT, func);
}

int main()
{
    List myList;
    InitList(&myList);
    int val;

    while (1)
    {
        showMenu();
        printf("请输入你的选择：\n");
        int select = 0;
        scanf("%d", &select);
        switch (select)
        {
        case 1:
        {
            signal_fun();
            printf("请输入你要尾插的数据(-1表示结束)\n");
            while (scanf("%d", &val), val != -1)
            {
                push_back(&myList, val);
            };
            press_key_clear();
            break;
        }
        case 2:
        {
            signal_fun();
            printf("请输入你要头插的数据(-1表示结束)\n");
            while (scanf("%d", &val), val != -1)
            {
                push_front(&myList, val);
            };
            press_key_clear();
            break;
        }
        case 3:
        {
            signal_fun();
            show_list(&myList);
            press_key_clear();
            break;
        }
        case 4:
        {
            signal_fun();
            pop_back(&myList);
            press_key_clear();
            break;
        }
        case 5:
        {
            signal_fun();
            pop_front(&myList);
            press_key_clear();
            break;
        }
        // case 6:
        // {
        //     signal_fun();
        //     printf("请输入要插入的位置:\n");
        //     scanf("%d", &pos);
        //     printf("请输入要插入的值:\n");
        //     scanf("%d", &val);
        //     insert_pos(&myList, pos, val);
        //     press_key_clear();
        //     break;
        // }
        // case 7:
        // {
        //     signal_fun();
        //     printf("请输入要查找的数据:\n", val);
        //     scanf("%d", &val);
        //     pos = find(&myList, val);
        //     if (pos == -1)
        //     {
        //         printf("要查找的数据%d未找到\n", val);
        //     }
        //     else
        //     {
        //         printf("要查找的数据%d在表中的下标%d位置\n", val, pos);
        //     }
        //     press_key_clear();
        //     break;
        // }
        // case 8:
        // {
        //     signal_fun();
        //     printf("顺序表的长度为%d\n", length(&myList));
        //     press_key_clear();
        //     break;
        // }
        // case 9:
        // {
        //     signal_fun();
        //     printf("请输入要删除的位置:\n");
        //     scanf("%d", &pos);
        //     delete_pos(&myList, pos);
        //     press_key_clear();
        //     break;
        // }
        // case 10:
        // {
        //     signal_fun();
        //     printf("请输入要删除的数据:\n");
        //     scanf("%d", &val);
        //     delete_val(&myList, val);
        //     press_key_clear();
        //     break;
        // }
        // case 11:
        // {
        //     signal_fun();
        //     sort(&myList);
        //     press_key_clear();
        //     break;
        // }
        // case 12:
        // {
        //     signal_fun();
        //     reverse(&myList);
        //     press_key_clear();
        //     break;
        // }
        // case 13:
        // {
        //     signal_fun();
        //     clear(&myList);
        //     press_key_clear();
        //     break;
        // }
        // case 0:
        // {
        //     destory(&myList);
        //     exit(0);
        //     break;
        // }
        default:
            printf("输入有误,请重新输入\n");
            break;
        }
    }
    return 0;
}