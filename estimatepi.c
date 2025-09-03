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
2. Use mutex locks to ensure that only one thread can update the sum at a time.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_ITER 1000000000
#define NUM_THREADS 3

int thread_count = NUM_THREADS;
double sum = 0.0;
int flag = 0;
pthread_mutex_t mutex;

void *estimate_pi(void* rank);

int main(int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;
    pthread_mutex_init(&mutex, NULL);

    thread_handles = malloc(thread_count * sizeof(pthread_t));
    for(thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, estimate_pi, (void*) thread);

    for(thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    
    free(thread_handles);

    double pi = 4.0 * sum;
    printf("%f\n", pi);

    pthread_mutex_destroy(&mutex);
    return 0;
}

void *estimate_pi(void* rank){
    long my_rank = (long) rank;
    double factor;
    long long i;
    long long my_n = NUM_ITER / thread_count;
    long long my_first_i = my_rank * my_n;
    long long my_last_i = (my_rank + 1) * my_n - 1;
    double my_sum = 0.0;

    if (my_first_i % 2 == 0)
        factor = 1.0;
    else
        factor = -1.0;

    for( i = my_first_i; i <= my_last_i; i++, factor = -factor){
        my_sum += factor / (2 * i + 1);
    }
    pthread_mutex_lock(&mutex);
    sum += my_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;

}