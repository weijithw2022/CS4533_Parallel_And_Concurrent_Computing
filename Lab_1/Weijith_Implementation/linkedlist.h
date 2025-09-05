#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

typedef struct list_node_s
{
    int data;
    struct list_node_s *next;
} list_node_t;

typedef struct
{
    char op; // 'M', 'I', 'D'
    int value;
} op_entry_t;

typedef struct
{
    long rank;
    op_entry_t *ops;
    int no_thread_operations;
} thread_data_t;

extern list_node_t *head;

int Member(int value, list_node_t *head_p);
int Insert(int value, list_node_t **head_pp);
int Delete(int value, list_node_t **head_pp);
void PrintList(list_node_t *head_p);
void FreeList(list_node_t *head_p);
void *thread_work(void *rank);

#endif
