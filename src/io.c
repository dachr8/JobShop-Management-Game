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
    overhaul = malloc(machineNum * sizeof(OVERHAULPTR));
    for (int i = 0; i < machineNum; ++i)
        overhaul[i] = NULL;
    int num = 0, startTime, machine, time;
    while (num != -1) {
        while (scanf("%d%d%d", &startTime, &machine, &time) == 3) {
            overhaul[machine] = malloc(sizeof(struct overhaul));
            overhaul[machine]->startTime = startTime;
            overhaul[machine]->time = time;
            overhaul[machine]->nextOverhaul = NULL;
        }
        if (!scanf("%d", &num))
            return;
    }
}

void output(int makespan) {
    FILE *fp = fopen("output.txt", "a");
    for (int i = 0; i < machineNum; ++i) {
        printf("\nM%d", i);
        fprintf(fp, "\r\nM%d", i);
        MACHINEPTR tmp = machine[i];
        while (tmp != NULL) {
            printf(" (%d,", tmp->startTime);
            fprintf(fp, " (%d,", tmp->startTime);
            if (tmp->job == -1) {
                printf("\"Overhaul\"");
                fprintf(fp, "\"Overhaul\"");
            } else {
                printf("%d-%d", tmp->job, tmp->order);
                fprintf(fp, "%d-%d", tmp->job, tmp->order);
            }
            printf(",%d)", tmp->endTime);
            fprintf(fp, ",%d)", tmp->endTime);
            tmp = tmp->nextJob;
        }
    }
    printf("\nEnd %d\n", makespan);
    fprintf(fp, "\r\nEnd %d\n", makespan);
    fclose(fp);
}

void freeAll(int *times, int **population) {
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
        while (overhaul[i]) {
            OVERHAULPTR tmp2 = overhaul[i];
            overhaul[i] = overhaul[i]->nextOverhaul;
            free(tmp2);
        }
    }
    free(machine);
    free(times);
    for (int i = 0; i < SIZE; ++i)
        free(population[i]);
    free(population);
}
