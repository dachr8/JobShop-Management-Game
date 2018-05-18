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
        while (scanf("(%d，%d)", &tmp->nextMachine->time, &tmp->nextMachine->machine) == 2)
            tmp = tmp->nextMachine;
        tmp->nextMachine = NULL;
    }

    return *(&job);
}

void getOverhaulByKeyboard() {
    int num = 0;
    while (num != -1) {
        scanf("%d%d%d", &overhaul.timeline, &overhaul.job, &overhaul.time);
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
        while (fscanf(fp, "(%d，%d)", &tmp->nextMachine->time, &tmp->nextMachine->machine) == 2)
            tmp = tmp->nextMachine;
        tmp->nextMachine = NULL;
    }

    num = 0;
    while (num != -1) {
        scanf("%d%d%d", &overhaul.timeline, &overhaul.job, &overhaul.time);
        scanf("%d", &num);
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
