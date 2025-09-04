#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

typedef struct node_t {
    int data;
    struct node_t* next;
} node_t;

typedef struct {
    node_t* head;
} linked_list_t;

typedef enum { OP_MEMBER, OP_INSERT, OP_DELETE } op_type;

typedef struct {
    op_type type;
    int value;
} task_t;

typedef struct {
    task_t* tasks;
    int start;
    int end;
} thread_data_t;

int Member(linked_list_t* list, int value);
int Insert(linked_list_t* list, int value);
int Delete(linked_list_t* list, int value);
linked_list_t* init_linked_list(linked_list_t* list,int n);
int get_random_value();
task_t* get_all_tasks(task_t* tasks, linked_list_t* list, int n_member, int n_insert, int n_delete);
void PrintList(linked_list_t* list);

#endif // DEFINITIONS_H