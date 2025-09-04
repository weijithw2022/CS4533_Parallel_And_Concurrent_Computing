#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "definitions.h"



int main(int argc, char* argv[]) {

    struct timespec start, end;
    linked_list_t list;
    srand(time(NULL));

    if (argc != 6) {
        fprintf(stderr, "Usage: %s <n> <m> <mMember> <mInsert> <mDelete>\n", argv[0]);
        return -1;
    }

    int n = atoi(argv[1]);       // initial elements
    int m = atoi(argv[2]);       // total operations
    float mm = atof(argv[3]);    // fraction Member
    float mi = atof(argv[4]);    // fraction Insert
    float md = atof(argv[5]);    // fraction Delete

    int n_member = m * mm;
    int n_insert = m * mi;
    int n_delete = m * md;


    task_t* tasks = malloc(m * sizeof(task_t));
    if (!tasks) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    init_linked_list(&list,n);
    get_all_tasks(tasks, &list, n_member, n_insert, n_delete);

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i=0; i<m; i++) {
        switch (tasks[i].type) {
            case OP_MEMBER:
                Member(&list, tasks[i].value);
                break;
            case OP_INSERT:
                Insert(&list, tasks[i].value);
                break;
            case OP_DELETE:
                Delete(&list, tasks[i].value);
                break;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec) / 1e6;


    FILE* fp = fopen("serial_execution_time.csv", "a");
    if (fp != NULL) {
        fprintf(fp, "%d,%d,%.2f,%.2f,%.2f,%.6f\n", n, m, mm, mi, md, elapsed_time);
        fclose(fp);
    } else {
        fprintf(stderr, "Error opening file for writing\n");
    }

    return 0;
}
