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
            OVERHAULPTR tmp;
            if (overhaul[machine]) {
                tmp = overhaul[machine];
                while (tmp->nextOverhaul)
                    tmp = tmp->nextOverhaul;
                tmp = tmp->nextOverhaul = malloc(sizeof(struct overhaul));
            } else
                tmp = overhaul[machine] = malloc(sizeof(struct overhaul));
            tmp->startTime = startTime;
            tmp->endTime = startTime + time;
            tmp->nextOverhaul = NULL;
        }
        if (scanf("%d", &num) != 1)
            return;
    }
}

void output(int makespan) {
    FILE *fp = fopen("output.txt", "a");
    for (int i = 0; i < machineNum; ++i) {
        printf("\nM%d", i);
        fprintf(fp, "\r\nM%d", i);
        MACHINEPTR tmp = machine[i];
        OVERHAULPTR tmp2 = NULL;
        if (overhaul) {
            tmp2 = overhaul[i];
            int flag = 1;
            while (tmp2 && flag)
                if (tmp2->endTime <= tmp->startTime) {
                    printf(" (%d,\"Overhaul\",%d)", tmp2->startTime, tmp2->endTime);
                    fprintf(fp, " (%d,\"Overhaul\",%d)", tmp2->startTime, tmp2->endTime);
                    tmp2 = tmp2->nextOverhaul;
                } else
                    flag = 0;
        }


        while (tmp) {
            printf(" (%d,%d-%d,%d)", tmp->startTime, tmp->job, tmp->order, tmp->endTime);
            fprintf(fp, " (%d,%d-%d,%d)", tmp->startTime, tmp->job, tmp->order, tmp->endTime);
            tmp = tmp->nextJob;
        }
    }
    printf("\nEnd %d\n\n", makespan);
    fprintf(fp, "\r\nEnd %d\r\n", makespan);
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
        if (overhaul)
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
