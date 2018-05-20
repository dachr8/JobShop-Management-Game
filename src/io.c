//author dachr
#include <stdio.h>
#include <malloc.h>
#include "struct.h"

JOBPTR *getJobByKeyboard() {
    scanf("%d %d", &jobNum, &machineNum);

    JOBPTR job[jobNum] = {NULL};
    int num = 0;
    while (scanf("%d", &num) && (num != -1)) {
        JOBPTR tmp = job[num] = malloc(sizeof(struct job));
        while (scanf("(%d，%d)", &tmp->time, &tmp->machine) == 2)
            tmp = tmp->nextMachine = malloc(sizeof(struct job));
        tmp->nextMachine = NULL;
    }

    return *(&job);
}

void getOverhaulByKeyboard() {
    int num = 0;
    OVERHAULPTR tmp = overhaul = malloc(sizeof(struct overhaul));
    while (num != -1) {
        while (scanf("%d%d%d", &tmp->timeline, &overhaul->machine, &overhaul->time) == 3)
            tmp = tmp->nextOverhaul = malloc(sizeof(struct overhaul));
        scanf("%d", &num);
    }
}

JOBPTR *getByFile() {
    FILE *fp = fopen("input.txt", "r");
    fscanf(fp, "%d %d", &jobNum, &machineNum);

    JOBPTR job[jobNum] = {NULL};
    int num = 0;
    while (fscanf(fp, "%d", &num) && (num != -1)) {
        JOBPTR tmp = job[num] = malloc(sizeof(struct job));
        while (fscanf(fp, "(%d，%d)", &tmp->time, &tmp->machine) == 2)
            tmp = tmp->nextMachine = malloc(sizeof(struct machine));
        tmp->nextMachine = NULL;
    }

    num = 0;
    OVERHAULPTR tmp = overhaul = malloc(sizeof(struct overhaul));
    while (num != -1) {
        while (fscanf(fp, "%d%d%d", &tmp->timeline, &overhaul->machine, &overhaul->time) == 3)
            tmp = tmp->nextOverhaul = malloc(sizeof(struct overhaul));
        fscanf(fp, "%d", &num);
    }

    fclose(fp);

    return *(&job);
}

void outputOnScreen(MACHINEPTR *machine, JOBPTR *job) {
    for (int i = 0; i < machineNum; ++i) {
        printf("\nM%d", i);
        MACHINEPTR tmp = machine[i];
        while (tmp != NULL) {
            printf(" (%d,", tmp->timeline);
            if (tmp->job == -1)
                printf("\"Overhaul\"");
            else {
                printf("%d-", tmp->job);
                JOBPTR tmp2 = job[i];
                for (int j = 0;; ++j) {
                    if (tmp2->machine == i) {
                        printf("%d", i);
                        break;
                    }
                    tmp2 = tmp2->nextMachine;
                }
            }
            printf(",%d)", tmp->time);
            tmp = tmp->nextJob;
        }
    }
    printf("\nEnd %d\n", makeSpan);
}

void outputByFile(MACHINEPTR *machine, JOBPTR *job) {
    FILE *fp = fopen("output.txt", "a");

    for (int i = 0; i < machineNum; ++i) {
        fprintf(fp, "\nM%d", i);
        MACHINEPTR tmp = machine[i];
        while (tmp != NULL) {
            fprintf(fp, " (%d,", tmp->timeline);
            if (tmp->job == -1)
                fprintf(fp, "\"Overhaul\"");
            else {
                fprintf(fp, "%d-", tmp->job);
                JOBPTR tmp2 = job[i];
                for (int j = 0;; ++j) {
                    if (tmp2->machine == i) {
                        printf("%d", i);
                        break;
                    }
                    tmp2 = tmp2->nextMachine;
                }
            }
            fprintf(fp, ",%d)", tmp->time);
            tmp = tmp->nextJob;
        }
    }
    fprintf(fp, "\nEnd %d\n", makeSpan);

    fclose(fp);
}

extern void freeAll(JOBPTR *job, MACHINEPTR *machine, OVERHAULPTR overhaul) {
    for (int i = 0; i < jobNum; ++i) {
        JOBPTR tmp = job[i];
        while (job[i] != NULL) {
            tmp = job[i];
            job[i] = job[i]->nextMachine;
            free(tmp);
        }
    }
    for (int i = 0; i < machineNum; ++i) {
        MACHINEPTR tmp = machine[i];
        while (machine[i] != NULL) {
            tmp = machine[i];
            machine[i] = machine[i]->nextJob;
            free(tmp);
        }
    }
    while (overhaul != NULL) {
        OVERHAULPTR tmp = overhaul;
        overhaul = overhaul->nextOverhaul;
        free(tmp);
    }
}
