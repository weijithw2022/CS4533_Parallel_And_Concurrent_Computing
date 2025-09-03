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
3. Using of thread sum 
*/

/*
Time  Comparison of Busy Wait and Mutex Lock
Threads    Time with Busy Wait (seconds)    Time with Mutex Lock (seconds)
1          3.570267                         3.617244       
2          1.872185                         1.869786
4          0.976358                         0.975118                                     
8          0.613545                         0.662650
16         0.624775                         0.584278                                       
32         0.608149                         0.576296
64         0.633920                         0.574159
*/

/*
Time without any Synchronization from BusyWait or Mutex Lock
Threads    Time(seconds)
1          3.602716
2          1.850353
4          0.965202
8          0.609571
16         0.597984
32         0.573637
64         0.591330


*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_ITER 1000000000
// #define NUM_THREADS 3

// int thread_count = NUM_THREADS;
int thread_count;
double sum = 0.0;
// int flag = 0;
// pthread_mutex_t mutex;

typedef struct{
    long rank;
    int thread_count;
    double *thread_sums; 
} threads_args_t;

void *estimate_pi(void* rank);

int main(int argc, char* argv[]){
    // clock_t start, end;
    //double cpu_time_used;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    long thread;
    pthread_t* thread_handles;
    // pthread_mutex_init(&mutex, NULL);

    thread_count = strtol(argv[1], NULL, 10);
    double *thread_sums = malloc(thread_count * sizeof(double));

    //start = clock();

    thread_handles = malloc(thread_count * sizeof(pthread_t));
    threads_args_t *args = malloc(thread_count * sizeof(threads_args_t));

    for(thread = 0; thread < thread_count; thread++){
        args[thread].rank = thread;
        args[thread].thread_count = thread_count;
        args[thread].thread_sums = thread_sums;
        pthread_create(&thread_handles[thread], NULL, estimate_pi, (void*) &args[thread]);
    }

    for(thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    for(int t = 0; t < thread_count; t++)
        sum += thread_sums[t];

    double pi = 4.0 * sum;
    printf("%f\n", pi);

    // end = clock();
    // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("CPU time used: %f seconds\n", cpu_time_used);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Wall time used: %f seconds\n", elapsed);
    free(thread_handles);
    free(thread_sums);
    free(args);
    // pthread_mutex_destroy(&mutex);
    return 0;
}

void *estimate_pi(void* rank){
    threads_args_t* my_arg = (threads_args_t*) rank;
    long my_rank = my_arg->rank;
    int thread_count = my_arg->thread_count;
    double *thread_sums = my_arg->thread_sums;
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
        // while(flag != my_rank); // Busy Wait
        my_sum += factor / (2 * i + 1);
        // flag = (flag + 1) % thread_count;
        // my_sum += factor / (2 * i + 1);
    }
    // while(flag != my_rank); // Busy Wait
    // sum += my_sum;
    // flag = (flag + 1) % thread_count;
    // pthread_mutex_lock(&mutex);
    //sum += my_sum;
    //pthread_mutex_unlock(&mutex);
    thread_sums[my_rank] = my_sum;  

    return NULL;

}