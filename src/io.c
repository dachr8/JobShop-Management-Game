//author dachr
#include <malloc.h>
#include "jobshop.h"

int *getJob() {
    scanf("%d%d", &jobNum, &machineNum);

    int *times = malloc(jobNum * sizeof(int));
    job = malloc(jobNum * sizeof(JOBPTR));

    int num;
    scanf("%d", &num);
    while (num != -1) {

        JOBPTR node = job[num] = malloc(sizeof(struct job)), tmp;
        times[num] = 0;
        while (times[num] < machineNum && scanf(" (%d,%d)", &node->time, &node->machine) == 2) {
            tmp = node;
            node = node->nextMachine = malloc(sizeof(struct job));
            times[num]++;
        }
        tmp->nextMachine = NULL;
        free(node);
        scanf("%d", &num);
    }

    return times;
}

void getOverhaul() {
    int num = 0;
    OVERHAULPTR tmp = overhaul = malloc(sizeof(struct overhaul));
    while (num != -1) {
        while (scanf("%d%d%d", &tmp->startTime, &overhaul->machine, &overhaul->time) == 3)
            tmp = tmp->nextOverhaul = malloc(sizeof(struct overhaul));
        scanf("%d", &num);
    }
}

void output(int makespan) {
    FILE *fp = fopen("output.txt", "a");
    for (int i = 0; i < machineNum; ++i) {
        printf("\nM%d", i);
        fprintf(fp, "\nM%d", i);
        MACHINEPTR tmp = machine[i];
        while (tmp != NULL) {
            printf(" (%d,", tmp->startTime);
            fprintf(fp, " (%d,", tmp->startTime);
            if (tmp->job == -1) {
                printf("\"Overhaul\"");
                fprintf(fp, "\"Overhaul\"");
            } else {
                printf("%d-", tmp->job);
                fprintf(fp, "%d-", tmp->job);

                JOBPTR tmp2 = job[tmp->job];
                for (int j = 1, flag = 1; flag; ++j, tmp2 = tmp2->nextMachine)
                    if (tmp2->machine == i) {
                        printf("%d", j);
                        fprintf(fp, "%d", j);
                        flag = 0;
                    }
            }
            printf(",%d)", tmp->endTime);
            fprintf(fp, ",%d)", tmp->endTime);
            tmp = tmp->nextJob;
        }
    }
    printf("\nEnd %d\n", makespan);
    fprintf(fp, "\nEnd %d\n", makespan);
    fclose(fp);
}

void freeAll(JOBPTR *job, MACHINEPTR *machine, OVERHAULPTR overhaul, int *times, int **population) {
    for (int i = 0; i < jobNum; ++i) {
        while (job[i]) {
            JOBPTR tmp = job[i];
            job[i] = job[i]->nextMachine;
            free(tmp);
        }
    }
    free(job);
    for (int i = 0; i < machineNum; ++i)
        while (machine[i]) {
            MACHINEPTR tmp = machine[i];
            machine[i] = machine[i]->nextJob;
            free(tmp);
        }
    free(machine);
    while (overhaul) {
        OVERHAULPTR tmp = overhaul;
        overhaul = overhaul->nextOverhaul;
        free(tmp);
    }
    free(times);
    for (int i = 0; i < SIZE; ++i)
        free(population[i]);
    free(population);
}
