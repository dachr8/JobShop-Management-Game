//author dachr
#include <stdio.h>
#include <malloc.h>
#include "struct.h"

JOBPTR getJobByKeyboard() {
    scanf("%d %d", &jobNum, &machineNum);

    JOBPTR job[jobNum] = {NULL};
    int num = 0;
    while (scanf("%d", &num) && (num != -1)) {
        JOBPTR tmp = job[num] = malloc(sizeof(struct job));
        while (scanf("(%d，%d)", &tmp->nextMachine->time, &tmp->nextMachine->machine) == 2)
            tmp = tmp->nextMachine;
        tmp->nextMachine = NULL;
    }

    return *job;
}

void getOverhaulByKeyboard() {
    int num = 0;
    while (num != -1) {
        scanf("%d%d%d", &overhaul.timeline, &overhaul.job, &overhaul.time);
        scanf("%d", &num);
    }
}

JOBPTR getByFile() {
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

    return *job;
}

void outputOnScreen(MACHINEPTR machine) {
    printf("End %d\n", makeSpan);
}

void outputByFile(MACHINEPTR machine) {
    FILE *fp = fopen("output.txt", "w");

    fclose(fp);
}
