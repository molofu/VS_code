#include "01_list_Node.h"

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

void InitList(List *list)
{
    list->first = list->last = (Node *)malloc(sizeof(Node));
    assert(list->first != NULL);
    list->size = 0;
}

void push_back(List *list, ElemType x)
{
    Node *s = (Node *)malloc(sizeof(Node));
    assert(s != NULL);
    s->data = x;
    s->next = NULL;

    list->last->next = s;
    list->last = s;
    list->size++;
}

void show_list(const List *list)
{
    Node *p = list->first->next;
    while (p != NULL)
    {
        printf("%d--", p->data);
        p = p->next;
    }
    printf("nul.\n");
}

void push_front(List *list, ElemType x)
{
    Node *s = (Node *)malloc(sizeof(Node));
    assert(s != NULL);
    s->data = x;
    s->next = list->first->next;
    list->first->next = s;
    if (list->size == 0)
    {
        list->last = s;
    }
    list->size++;
}

void pop_back(List *list)
{
    if (list->size == 0)
    {
        return;
    }
    Node *p = list->first;
    while (p->next != list->last)
    {
        p = p->next;
    }
    free(list->last);
    list->last = p;
    p->next = NULL;
    list->size--;
}

void pop_front(List *list)
{
    if (list->size == 0)
    {
        return;
    }
    Node *p = list->first->next;
    list->first->next = p->next;
    free(p);
    if(list->size==1)
    {
        list->last = list->first;
    }
    list->size--;
}