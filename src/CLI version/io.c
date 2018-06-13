//author dachr
#include <malloc.h>
#include "jobshop.h"

void *getJob() {
    scanf("%d%d", &jobNum, &machineNum);

    job = malloc(jobNum * sizeof(JOBPTR));

    for (int i = 0; i < jobNum; ++i) {
        JOBPTR node = job[i] = malloc(sizeof(struct job));
        for (int j = 0; j < machineNum - 1; ++j) {
            scanf("%d %d", &node->machine, &node->time);
            node = node->nextMachine = malloc(sizeof(struct job));
        }
        scanf("%d %d", &node->machine, &node->time);
        node->nextMachine = NULL;
    }
}

void output(int makespan) {
    FILE *fp = fopen("9~12班36output.txt", "a");
    for (int i = 0; i < machineNum; ++i) {
        printf("\nM%d", i);
        fprintf(fp, "\r\nM%d", i);
        MACHINEPTR tmp = machine[i];

        while (tmp) {
            printf(" (%d,%d-%d,%d)", tmp->startTime, tmp->job, tmp->order, tmp->endTime);
            fprintf(fp, " (%d,%d-%d,%d)", tmp->startTime, tmp->job, tmp->order, tmp->endTime);
            tmp = tmp->nextJob;
        }
    }
    fprintf(fp, "\nTime Used: %.3lfs", (double) clock() / CLOCKS_PER_SEC);
    printf("\nTime Used: %.3lfs", (double) clock() / CLOCKS_PER_SEC);
    printf("\nEnd Time: %d\n\n", makespan);
    fprintf(fp, "\r\nEnd Time: %d\r\n", makespan);

    fclose(fp);
}

void freeAll() {
    for (int i = 0; i < jobNum; ++i) {
        while (job[i]) {
            JOBPTR tmp = job[i];
            job[i] = job[i]->nextMachine;
            free(tmp);
        }
    }
    free(job);
    for (int i = 0; i < machineNum; ++i) {
        while (machine[i]) {
            MACHINEPTR tmp1 = machine[i];
            machine[i] = machine[i]->nextJob;
            free(tmp1);
        }
    }
    free(machine);
}
