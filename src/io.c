#include <stdio.h>
#include <malloc.h>
#include "struct.h"

JOBPTR inputByKeyboard() {
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

JOBPTR inputByFile() {
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

    fclose(fp);

    return *job;
}

void outputOnScreen(JOBPTR machine) {
    printf("End ");
}

void outputByFile(JOBPTR machine) {
    FILE *fp = fopen("output.txt", "w");

    fclose(fp);

}
