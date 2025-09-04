#include "linkedlist.h"

struct list_node_s* head = NULL;

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

    op_entry_t *ops = malloc(m * sizeof(op_entry_t));

    int idx = 0;
    for (int i = 0; i < totalmemberfns; i++) {
        ops[idx].op = 'M';
        ops[idx].value = rand() % 65536;  // random value to check
        idx++;
    }

    for (int i = 0; i < totalinsertfns; i++) {
        ops[idx].op = 'I';
        ops[idx].value = insertNewValues[i];
        idx++;
    }

    for (int i = 0; i < totaldeletefns; i++) {
        ops[idx].op = 'D';
        ops[idx].value = deleteNewValues[i];
        idx++;
    }

    for (int i = m - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        op_entry_t temp = ops[i];
        ops[i] = ops[j];
        ops[j] = temp;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Execute operations
    for (int i = 0; i < m; i++) {
        int val = ops[i].value;
        if (ops[i].op == 'M') {
            Member(val, head);
        } else if (ops[i].op == 'I') {
            Insert(val, &head);
        } else if (ops[i].op == 'D') {
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