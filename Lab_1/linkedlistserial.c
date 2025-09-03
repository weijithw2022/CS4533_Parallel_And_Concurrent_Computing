#include <stdio.h>
#include <stdlib.h>

struct list_node_s {
   int data;
   struct list_node_s* next;
};  

/* Check if a value is a member of the sorted singly linked list */
int Member(int value, struct list_node_s* head_p){
    struct list_node_s* curr_p = head_p;
    
    while(curr_p!= NULL && curr_p->data<value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value)
        return 0;
    else
        return 1;
}

int Insert(int value, struct list_node_s** head_pp){
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while(curr_p != NULL && curr_p->data < value){
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
    if(curr_p == NULL || curr_p->data > value){
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if(pred_p == NULL)
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    }
    else /*Value already is in the list*/
        return 0;
}

int Delete(int value, struct list_node_s** head_pp){
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while(curr_p != NULL && curr_p->data < value){
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
    if (curr_p != NULL && curr_p->data == value){
        if(pred_p == NULL)
            *head_pp = curr_p->next;  /* Deleting the head*/
        else
            pred_p->next = curr_p->next;
        free(curr_p);
        return 1;
    }
    else /* Value not in the list */
        return 0;
}

void PrintList(struct list_node_s* head_p){
    struct list_node_s* curr_p = head_p;
    while(curr_p != NULL){
        printf("%d\n", curr_p->data);
        curr_p = curr_p->next;
    }
}

void FreeList(struct list_node_s* head_p){
    struct list_node_s* temp;
    while(head_p != NULL){
        temp = head_p;
        head_p = head_p->next;
        free(temp); 
    }
}

int main()
{
    struct list_node_s* head = NULL;

    Insert(5, &head);
    Insert(3, &head);
    Insert(7, &head);

    printf("List after inserts: \n");
    PrintList(head);

    Delete(3, &head);
    printf("List after deleting 3: \n");
    PrintList(head);

    Insert(4, &head);
    Insert(6, &head);
    Insert(2, &head);
    printf("List after more inserts: \n");
    PrintList(head);
    Insert(5, &head);
    printf("List after inserting 5: \n");
    PrintList(head);


    Delete(5, &head);
    printf("List after deleting 5: \n");
    PrintList(head);
    Delete(10, &head);
    printf("List after deleting 10(Not in the list): \n");
    PrintList(head);
    Delete(7, &head);
    Delete(4, &head);
    Delete(6, &head);
    printf("List after deleting 7, 4, 6: \n");
    PrintList(head);
    Delete(2, &head);
    printf("List after deleting 2: \n");
    PrintList(head);
    Delete(2, &head);
    printf("Trying to delete from the empty list: \n");
    Delete(2, &head);

    FreeList(head);

    return 0;
}