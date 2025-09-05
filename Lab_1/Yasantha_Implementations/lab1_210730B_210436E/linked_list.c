#include "definitions.h"

int Member(linked_list_t* list, int value)
{

    node_t* current = list->head;
    while (current != NULL && current->data < value)
    {
        current = current->next;
    }
    if (current == NULL || current->data > value)
    {
        return 0; 
    }
    return 1;
}


int Insert(linked_list_t* list, int value)
{
    node_t* current = list->head;
    node_t* previous = NULL;

    while (current != NULL && current->data < value)
    {
        previous = current;
        current = current->next;
    }

    if (current != NULL && current->data == value)
    {
        return 0; 
    }


    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if(!new_node)
    {
        return -1;
    }
    new_node->data = value;
    new_node->next = current;

    if (previous == NULL)
    {
        list->head = new_node;
    }
    else
    {
        previous->next = new_node;
    }
    
    return 1; 
}

int Delete(linked_list_t* list, int value)
{
    node_t* current = list->head;
    node_t* previous = NULL;

    while (current != NULL && current->data < value)
    {
        previous = current;
        current = current->next;
    }

    if (current != NULL && current->data == value)
    {
        if (previous == NULL)
        {
            list->head = current->next;
        }
        else
        {
            previous->next = current->next;
        }
        free(current);
        return 1; 
    }
    return 0; 
}

linked_list_t* init_linked_list(linked_list_t* list,int n)
{
    list->head = NULL;
    for (int i = 0; i < n; i++) {
        int value;
        do {
            value = get_random_value();
        } while (Member(list, value));
        Insert(list, value);
    }
    return list;
}
