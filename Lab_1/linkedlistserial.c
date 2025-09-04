#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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

double run_experiments(int n, int m, double mMember, double mInsert, double mDelete){
    struct list_node_s* head = NULL;
    
    int count = 0;
    while(count < n){
        int value = rand() % 65536;
        if(Insert(value, &head)) /*Fills with n unique values*/
            count++;
    }

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

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

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

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    // printf("Wall time used: %f seconds\n", elapsed);

    // printf("List after all operations: \n");
    // PrintList(head);


    FreeList(head);
    free(insertNewValues);
    free(deleteNewValues);
    free(ops);
    return elapsed;
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

    int sample_runs = 30;
    double sample_times[sample_runs];
    
    for(int i= 0; i< sample_runs; i++){
        srand(time(NULL) + i);
        sample_times[i] = run_experiments(n, m, mMember, mInsert, mDelete);
    }

    double sample_sum = 0.0;
    for(int i=0; i<sample_runs; i++){
        sample_sum += sample_times[i];
    }
    double sample_mean = sample_sum / sample_runs;

    double variance = 0.0;
    for(int i=0; i<sample_runs; i++){
        variance += (sample_times[i] - sample_mean) * (sample_times[i] - sample_mean);
    }
    variance /= (sample_runs-1);
    double sample_stddev = sqrt(variance);

    double r = 5.0; // ±5% accuracy
    double z = 1.96; // 95% confidence level

    double required_runs = pow((100 * z * sample_stddev) / (r * sample_mean), 2);
    int required_samples = (int)ceil(required_runs);
    printf("Required runs: %f\n", required_runs);
    printf("Required samples: %d\n", required_samples);

    double *all_times = malloc(required_samples * sizeof(double));

    for(int i=0; i<required_samples; i++){
        srand(time(NULL) + i);
        all_times[i] = run_experiments(n, m, mMember, mInsert, mDelete);
    }

    double all_sum = 0.0;
    for(int i=0; i<required_samples; i++){
        all_sum += all_times[i];
    }
    double average = all_sum / required_samples;

    double all_variance = 0.0;
    for(int i=0; i<required_samples; i++){
        all_variance += (all_times[i] - average) * (all_times[i] - average);
    }
    all_variance /= (required_samples);
    double all_stddev = sqrt(all_variance);

    printf("All runs - Mean: %f, StdDev: %f\n", average, all_stddev);

    free(all_times);
    return 0;
}