//author dachr
#include <stdio.h>
#include <malloc.h>
#include "struct.h"

int *getJob() {
    scanf("%d%d", &jobNum, &machineNum);

    int *times = malloc(jobNum * sizeof(int));
    job = malloc(jobNum * sizeof(JOBPTR));

    int num;
    scanf("%d", &num);
    while (num != -1) {
        JOBPTR node = job[num] = malloc(sizeof(struct job));
        JOBPTR tmp;
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
        while (scanf("%d%d%d", &tmp->timeline, &overhaul->machine, &overhaul->time) == 3)
            tmp = tmp->nextOverhaul = malloc(sizeof(struct overhaul));
        scanf("%d", &num);
    }
}

void output(MACHINEPTR *machine) {
    FILE *fp = fopen("output.txt", "a");
    for (int i = 0; i < machineNum; ++i) {
        printf("\nM%d", i);
        fprintf(fp, "\nM%d", i);
        MACHINEPTR tmp = machine[i];
        while (tmp != NULL) {
            printf(" (%d,", tmp->timeline);
            fprintf(fp, " (%d,", tmp->timeline);
            if (tmp->job == -1) {
                printf("\"Overhaul\"");
                fprintf(fp, "\"Overhaul\"");
            } else {
                printf("%d-", tmp->job);
                fprintf(fp, "%d-", tmp->job);
                JOBPTR tmp2 = job[i];
                for (int j = 0;; ++j) {
                    if (tmp2->machine == i) {
                        printf("%d", i);
                        fprintf(fp, "%d", i);
                        break;
                    }
                    tmp2 = tmp2->nextMachine;
                }
            }
            printf(",%d)", tmp->time);
            fprintf(fp, ",%d)", tmp->time);
            tmp = tmp->nextJob;
        }
    }
    printf("\nEnd %d\n", minMakespan);
    fprintf(fp, "\nEnd %d\n", minMakespan);
    fclose(fp);
}

void freeAll(MACHINEPTR *machine, OVERHAULPTR overhaul, int *times) {
    for (int i = 0; i < jobNum; ++i) {
        free(&times[i]);
        while (job[i]) {
            JOBPTR tmp = job[i];
            job[i] = job[i]->nextMachine;
            free(tmp);
        }
    }
    for (int i = 0; i < machineNum; ++i) {
        while (machine[i]) {
            MACHINEPTR tmp = machine[i];
            machine[i] = machine[i]->nextJob;
            free(tmp);
        }
    }
    while (overhaul) {
        OVERHAULPTR tmp = overhaul;
        overhaul = overhaul->nextOverhaul;
        free(tmp);
    }
}
