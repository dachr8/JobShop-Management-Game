#include <stdio.h>
#include <malloc.h>
#include "jobshop.h"

JOBPTR inputByKeyboard();

JOBPTR inputByFile();

void schedule();

void outputOnScreen();

void outputByFile();

int jobNum, machineNum;

int main(void) {


    int makeSpan;


    int inputMode;
    printf("Input Mode: 1.Keyboard 2.File 3.graph\n");
    printf("Please input your input mode: ");
    scanf("%d", &inputMode);
    switch (inputMode) {
        case 1:
            scanf("%d %d", &jobNum, &machineNum);
            //int time[jobNum] = {0}, order[jobNum] = {0}, machine[machineNum] = {0};
            //inputByKeyboard(time, order, machineNum);
            break;
        case 2:
            inputByFile();
            break;
        case 3:
            printf("3.graph");
            break;
        default:
            printf("Please input your input mode: ");
            scanf("%d", &inputMode);
    }

    schedule();


    outputOnScreen();
    outputOnTxt();

    return 0;
}

JOBPTR inputByKeyboard() {

}

/*
void inputByKeyboard(int time[], int order[], int machineNum) {
    for (int i = 0; i <= machineNum; ++i)
        while (scanf("(%d，%d)", &time[i], &order[i]) != 2);
}
*/
JOBPTR inputByFile() {
    char fileName[99];

    printf("Please enter the file name: ");
    gets(fileName);

    FILE *fp = fopen("filename", "r");
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

void schedule() {

}

void outputOnScreen() {
    printf("End ");
}

void outputByFile() {

}