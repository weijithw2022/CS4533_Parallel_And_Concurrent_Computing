#include "definitions.h"

void PrintList(linked_list_t* list) {
    node_t* current = list->head;
    printf("List: ");
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int get_random_value() {
    return rand() % 65536;
}

task_t* get_all_tasks(task_t* tasks, linked_list_t* list, int n_member, int n_insert, int n_delete) {
    int idx = 0;
    for (int i = 0; i < n_member; i++) {
        tasks[idx].type = OP_MEMBER;
        tasks[idx].value = get_random_value();
        idx++;
    }
    for (int i = 0; i < n_insert; i++) {
        tasks[idx].type = OP_INSERT;
        int val;
        do {
            val = get_random_value();
        } while (Member(list, val));
        tasks[idx].value = val;
        idx++;
    }
    for (int i = 0; i < n_delete; i++) {
        tasks[idx].type = OP_DELETE;
        tasks[idx].value = get_random_value();
        idx++;
    }

    for (int i = n_member + n_insert + n_delete - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        task_t temp = tasks[i];
        tasks[i] = tasks[j];
        tasks[j] = temp;
    }
    return tasks;
}