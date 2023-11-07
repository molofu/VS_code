#include "01_seqList.h"

void showMenu()
{
    printf("********************************************\n");
    printf("****    1.push_back     2.push_front    ****\n");
    printf("****    3.show_list     4.pop_back      ****\n");
    printf("****    5.pop_front     6.insert_pos    ****\n");
    printf("****    7.find          8.length        ****\n");
    printf("****    9.delete_pos    10.delete_val   ****\n");
    printf("****    11.sort         12.reverse      ****\n");
    printf("****    13.clear        0.quit_system   ****\n");
    printf("********************************************\n");
}

void InitSeqList(SeqList *list)
{
    list->base = (ElemType *)malloc(sizeof(ElemType) * INITSIZE);
    assert(list->base != NULL);
    list->capacity = INITSIZE;
    list->size = 0;
}

void push_back(SeqList *list, ElemType val)
{
    if (list->size >= list->capacity)
    {
        printf("顺序表已满,无法插入\n");
        return;
    }
    else
    {
        list->base[list->size] = val;
        list->size++;
    }
}

void push_front(SeqList *list, ElemType val)
{
    if (list->size >= list->capacity)
    {
        printf("顺序表已满,无法插入\n");
        return;
    }
    for (int i = list->size; i > 0; i--)
    {
        list->base[i] = list->base[i - 1];
    }
    list->base[0] = val;
    list->size++;
}

void show_list(const SeqList *list)
{
    for (int i = 0; i < list->size; i++)
    {
        printf("%d ", list->base[i]);
    }
    printf("\n");
}

void pop_back(SeqList *list)
{
    if (list->size == 0)
    {
        printf("表空\n");
        return;
    }
    list->size--;
}

void pop_front(SeqList *list)
{
    if (0 == list->size)
    {
        printf("表空\n");
        return;
    }
    for (int i = 0; i < list->size; i++)
    {
        list->base[i + 1] = list->base[i];
    }
    list->size--;
}

void insert_pos(SeqList *list, int pos, ElemType val)
{
    if (pos < 0 || pos > list->size)
    {
        printf("插入位置有误\n");
        return;
    }

    for (int i = list->size; i > pos; i--)
    {
        list->base[i] = list->base[i - 10];
    }
    list->base[pos] = val;
    list->size++;
}

int find(const SeqList *list, ElemType val)
{
    for (int i = 0; i < list->size; i++)
    {
        if (list->base[i] == val)
        {
            return i;
        }
    }
    return -1;
}

int length(const SeqList *list)
{
    return list->size;
}

void delete_pos(SeqList *list, int pos)
{
    pos--;
    if (pos < 0 || pos >= list->size)
    {
        printf("删除数据的位置非法\n");
        return;
    }
    for (int i = pos; i < list->size - 1; i++)
    {
        list->base[i] = list->base[i + 1];
    }
    list->size--;
}

void delete_val(SeqList *list, ElemType val)
{
    int pos = find(list, val);
    if (pos == -1)
    {
        printf("要删除的数据不存在\n");
    }
    else
    {
        delete_pos(list, pos);
    }
}

void sort(SeqList *list)
{
    for (int i = 0; i < list->size - 1; i++)
    {
        for (int j = 0; j < list->size - i - 1; j++)
        {
            if (list->base[j] > list->base[j + 1])
            {
                ElemType tmp = list->base[j];
                list->base[j] = list->base[j + 1];
                list->base[j + 1] = tmp;
            }
        }
    }
}

void reverse(SeqList *list)
{
    if (list->size <= 1)
    {
        return;
    }
    int low = 0;
    int high = list->size - 1;
    ElemType tmp;
    while (low < high)
    {
        tmp = list->base[low];
        list->base[low] = list->base[high];
        list->base[high] = tmp;
        low++;
        high--;
    }
}

void clear(SeqList *list)
{
    list->size = 0;
}

void destory(SeqList *list)
{
    if (list->base != NULL)
    {
        free(list->base);
        list->base = NULL;
        list->size = 0;
        list->capacity = 0;
    }
}