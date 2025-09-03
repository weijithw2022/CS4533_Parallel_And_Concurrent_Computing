#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int main(int argc, char* argv[])
{
    if(argc != 6){
        fprintf(stderr, "Usage: %s <n> <m> <mMember> <mInsert> <mDelete>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    double mMember = atof(argv[3]);
    double mInsert = atof(argv[4]);
    double mDelete = atof(argv[5]);

    if(n <= 0 || n > 65536){
        fprintf(stderr, "n must be between 1 and 2^16\n");
        return 1;
    }

    if(m<=0){
        fprintf(stderr, "m must be greater than 0\n");
        return 1;
    }

    if(mMember + mInsert + mDelete > 1.0001 || mMember + mInsert + mDelete < 0.9999){
        fprintf(stderr, "Fraction sum must be equal to 1\n");
        return 1;
    }

    
    struct list_node_s* head = NULL;

    srand(time(NULL));
    
    int count = 0;
    while(count < n){
        int value = rand() % 65536;
        if(Insert(value, &head)) /*Fills with n unique values*/
            count++;
    }

    // printf("List after filling with unique values: \n");
    // PrintList(head);

    int memberfns = 0, insertfns = 0, deletefns = 0;
    int totalmemberfns = m * mMember;
    int totalinsertfns = m * mInsert;
    int totaldeletefns = m * mDelete;   

    int insertNewCount = 0;
    int *insertNewValues  = malloc(totalinsertfns * sizeof(int));
    while(insertNewCount < totalinsertfns){
        int value = rand() % 65536;
        if(!Member(value, head)){
            insertNewValues[insertNewCount++] = value;
        }
    }

    int deleteNewCount = 0;
    int *deleteNewValues = malloc(totaldeletefns * sizeof(int));
    while(deleteNewCount < totaldeletefns){
        int value = rand() % 65536;
        if(Member(value, head)){
            deleteNewValues[deleteNewCount++] = value;
        }
    }

    char *ops = malloc(m * sizeof(char));
    for (int i =0; i< totalmemberfns; i++) ops[i] = 'M';
    for (int i =0; i< totalinsertfns; i++) ops[totalmemberfns + i] = 'I';
    for (int i =0; i< totaldeletefns; i++) ops[totalmemberfns + totalinsertfns + i] = 'D';

    for(int i=m-1; i>0; i--){
        int j = rand() % (i+1);
        char temp = ops[i];
        ops[i] = ops[j];
        ops[j] = temp;
    }

    for(int i=0;i<m;i++){
        if(ops[i]=='M'){
            int val = rand() % 65536;
            Member(val, head);
            memberfns++;
        } else if(ops[i]=='I'){
            int val = insertNewValues[insertfns++];
            Insert(val, &head);
        } else {
            int val = deleteNewValues[deletefns++];
            Delete(val, &head);
        }
    }

    // while(succesCount < m){
    //     double prob = (double) rand() / RAND_MAX;
    //     int value = rand() % 65536;
    //     if(prob < mMember){
    //         Member(value, head);
    //         memberfns++;
    //         succesCount++;
    //     }
    //     else if(prob < mMember + mInsert){
    //         if(Insert(value, &head)){
    //             insertfns++;
    //             succesCount++;
    //         }
    //     }
    //     else{
    //         if(Delete(value, &head)){
    //             deletefns++;
    //             succesCount++;
    //         }
    //         // Delete(value, &head);
    //         // deletefns++;
    //     }
    // }
    
    /* for(int i =0; i<m; i++){
        double prob = (double) rand() / RAND_MAX;
        int value = rand() % 65536;
        if(prob < mMember){
            Member(value, head);
            memberfns++;
        }
        else if(prob < mMember + mInsert){
            if(Insert(value, &head))
                insertfns++;
        }
        else{
            if(Delete(value, &head))
                deletefns++;
            // Delete(value, &head);
            // deletefns++;
        }
    } */

    printf("Number of member functions: %d\n", memberfns);
    printf("Number of insert functions: %d\n", insertfns);
    printf("Number of delete functions: %d\n", deletefns);

    printf("List after all operations: \n");
    PrintList(head);

    // Insert(5, &head);
    // Insert(3, &head);
    // Insert(7, &head);

    // printf("List after inserts: \n");
    // PrintList(head);


    // Insert(4, &head);
    // Insert(6, &head);
    // Insert(2, &head);
    // printf("List after more inserts: \n");
    // PrintList(head);
    // Insert(5, &head);
    // printf("List after inserting 5: \n");
    // PrintList(head);

    // int ans = Member(5, head);
    // printf("Is 5 a member of the list? %s\n", ans ? "Yes" : "No");

    // Delete(5, &head);
    // Delete(7, &head);
    // Delete(4, &head);
    // printf("List after deletes: \n");
    // PrintList(head);
    // int ans2 = Member(4, head);
    // printf("Is 4 a member of the list? %s\n", ans2 ? "Yes" : "No");
    // Delete(6, &head);
    // Delete(2, &head);
    // printf("List after deleting: \n");
    // PrintList(head);
    // printf("Trying to delete from the empty list: \n");
    // Delete(2, &head);

    FreeList(head);
    free(insertNewValues);
    free(deleteNewValues);

    return 0;
}