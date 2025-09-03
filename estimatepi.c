// pi = 4(1 - 1/3 + 1/5 - 1/7 + 1/9 - 1/11 + ...+(-1)^n/(2n+1) + ..)

/*
Serial Code for Estimating Pi
double factor = 0.0;
double sum = 0.0;
for (int i = 0; i < n; i++, factor = -factor) {
    sum += factor / (2 * i + 1);
}
double pi = 4.0 * sum;
*/

/*
You get different values when the number of threads is changed for the same iterations.
This is due to the way the threads are accessing and modifying the shared variable 'sum'.

Solutions; 
1. Use a flag to wait until the previous thread has finished updating the sum.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_ITER 1000000000
#define NUM_THREADS 1

int thread_count = NUM_THREADS;
double sum = 0.0;
int flag = 0;

void *estimate_pi(void* rank);

int main(int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    for(thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, estimate_pi, (void*) thread);
    
    for(thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    
    free(thread_handles);

    double pi = 4.0 * sum;
    printf("%f\n", pi);

    return 0;
}

void *estimate_pi(void* rank){
    long my_rank = (long) rank;
    double factor;
    long long i;
    long long my_n = NUM_ITER / thread_count;
    long long my_first_i = my_rank * my_n;
    long long my_last_i = (my_rank + 1) * my_n - 1;
    if (my_first_i % 2 == 0)
        factor = 1.0;
    else
        factor = -1.0;

    for( i = my_first_i; i <= my_last_i; i++, factor = -factor){
        while(flag != my_rank); // busy wait
        sum += factor / (2 * i + 1);
        flag = (flag + 1) % thread_count;
    }

    return NULL;

}