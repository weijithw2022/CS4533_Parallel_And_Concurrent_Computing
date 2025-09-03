
/* 
Serial Code for Matrix Multiplication
A(mxn) *x(nx1) = y(mx1)
for (int i = 0; i < m; i++){
    y[i] = 0;
    for(int j = 0; j < n; j++){
        y[i] += A[i][j] * x[j];
    }
} */

// Suppose 6x6 matrix multiplication
// Assinging 3 threads for each component of the output vector y
// y[0], y[1] -> 0
// y[2], y[3] -> 1
// y[4], y[5] -> 2

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_COUNT 3
int m = 6;
int n = 5;
int A[6][5];
int x[5];
int y[5];

int A[6][5] = {
    {1, 0, 1, 1, 0},
    {0, 1, 0, 0, 1},
    {1, 0, 1, 0, 3},
    {2, 0, 0, 1, 0},
    {1, 2, 0, 1, 1},
    {1, 1, 0, 1, 1}
};

int x[5] = {1, 
            2, 
            3, 
            4, 
            5};

int thread_count = THREAD_COUNT;

void *Thread_Mult(void* rank);

int main(int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    for(thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Thread_Mult, (void*) thread);
    
    for(thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    
    free(thread_handles);

    for(int i = 0; i < m; i++)
        printf("%d ", y[i]);
    printf("\n");

    return 0;
}

void *Thread_Mult(void* rank){
    long my_rank = (long) rank;
    int i, j;
    int local_m = m/ thread_count;
    int my_first_row = my_rank * local_m;
    int my_last_row = (my_rank + 1) * local_m - 1;
    for ( i = my_first_row; i<= my_last_row; i++){
        y[i] = 0;
        for(j = 0; j< n; j++){
            y[i] += A[i][j] * x[j];
        }
    }
    return NULL;
}