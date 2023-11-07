#pragma once
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define INITSIZE 10
typedef int ElemType;

typedef struct SeqList
{
    ElemType *base;
    int capacity;
    int size;
} SeqList;

void showMenu();

void InitSeqList(SeqList *list);

void push_back(SeqList *list, ElemType val);

void push_front(SeqList *list, ElemType val);

void show_list(const SeqList *list);

void pop_back(SeqList *list);

void pop_front(SeqList *list);

void insert_pos(SeqList *list, int pos, ElemType val);

int find(const SeqList *list, ElemType val);

int length(const SeqList *list);

void delete_pos(SeqList *list, int pos);

void delete_val(SeqList *list, ElemType val);

void sort(SeqList *list);

void reverse(SeqList *list);

void clear(SeqList *list);

void destory(SeqList *list);