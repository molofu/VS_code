#pragma once
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define ElemType int

typedef struct Node
{
    ElemType data;
    struct Node *next;
} Node, *pNode;

typedef struct list
{
    pNode first;
    pNode last;
    size_t size;
} List;

void showMenu();

void InitList(List *list);

void push_back(List *list, ElemType x);

void show_list(const List *list);

void push_front(List *list, ElemType x);

void pop_back(List *list);

void pop_front(List *list);