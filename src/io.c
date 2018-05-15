#include <stdio.h>
#include <malloc.h>
#include "struct.h"

JOBPTR inputByKeyboard() {

}

/*
void inputByKeyboard(int time[], int order[], int machineNum) {
    for (int i = 0; i <= machineNum; ++i)
        while (scanf("(%d，%d)", &time[i], &order[i]) != 2);
}
*/
JOBPTR inputByFile() {
    FILE *fp = fopen("input.txt", "r");
    fscanf(fp, "%d %d", &jobNum, &machineNum);

    JOBPTR job[jobNum] = {NULL};
    int num = 0;
    fscanf(fp, "%d", &num);
    while (num != -1) {
        JOBPTR tmp = job[num] = malloc(sizeof(struct job));
        while (fscanf(fp, "(%d，%d)", &tmp->nextMachine->time, &tmp->nextMachine->machine) == 2)
            tmp = tmp->nextMachine;
        tmp->nextMachine = NULL;
        fscanf(fp, "%d", &num);
    }

    fclose(fp);

    return job[];
}

void outputOnScreen() {
    printf("End ");
}

void outputByFile() {

}
