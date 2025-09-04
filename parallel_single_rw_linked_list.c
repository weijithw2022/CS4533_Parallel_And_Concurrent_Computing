#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "definitions.h"

linked_list_t list;
pthread_rwlock_t list_rwlock;   // Read-Write lock

void* thread_work(void* arg) {
    thread_data_t td = *(thread_data_t*)arg;

    for (int i = td.start; i < td.end; i++) {
        int val = td.tasks[i].value;

        switch (td.tasks[i].type) {
            case OP_MEMBER:
                pthread_rwlock_rdlock(&list_rwlock);   // Acquire read lock
                Member(&list, val);
                pthread_rwlock_unlock(&list_rwlock);   // Release lock
                break;

            case OP_INSERT:
                pthread_rwlock_wrlock(&list_rwlock);   // Acquire write lock
                Insert(&list, val);
                pthread_rwlock_unlock(&list_rwlock);   // Release lock
                break;

            case OP_DELETE:
                pthread_rwlock_wrlock(&list_rwlock);   // Acquire write lock
                Delete(&list, val);
                pthread_rwlock_unlock(&list_rwlock);   // Release lock
                break;
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {

    pthread_rwlock_init(&list_rwlock, NULL);
    struct timespec start, end;
    srand(time(NULL));
    list.head = NULL;

    if (argc != 7) {
        fprintf(stderr, "Usage: %s <n> <m> <num_threads> <mMember> <mInsert> <mDelete>\n", argv[0]);
        return -1;
    }

    int n = atoi(argv[1]);          // initial elements
    int m = atoi(argv[2]);          // total operations
    int num_threads = atoi(argv[3]);
    float mm = atof(argv[4]);       // fraction Member
    float mi = atof(argv[5]);       // fraction Insert
    float md = atof(argv[6]);       // fraction Delete

    int n_member = m * mm;
    int n_insert = m * mi;
    int n_delete = m * md;

    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    task_t* tasks = malloc(m * sizeof(task_t));
    if (!tasks) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    init_linked_list(&list, n);
    get_all_tasks(tasks, &list, n_member, n_insert, n_delete);

    int chunk = m / num_threads;
    int remainder = m % num_threads;

    for (int t = 0, start_idx = 0; t < num_threads; t++) {
        int end_idx = start_idx + chunk + (t < remainder ? 1 : 0);
        thread_data[t].tasks = tasks;
        thread_data[t].start = start_idx;
        thread_data[t].end = end_idx;
        start_idx = end_idx;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, thread_work, &thread_data[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec) / 1e6;

    FILE* fp = fopen("execution_time_parallel_rwlock.csv", "a");
    if (fp != NULL) {
        fprintf(fp, "%d,%d,%d,%.2f,%.2f,%.2f,%.6f\n", n, m,num_threads, mm, mi, md, elapsed_time);
        fclose(fp);
    } else {
        fprintf(stderr, "Error opening file for writing\n");
    }

    pthread_rwlock_destroy(&list_rwlock);
    free(tasks);
    return 0;
}
