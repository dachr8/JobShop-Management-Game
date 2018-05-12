#include <stdio.h>
#include "struct.h"

extern JOBPTR inputByKeyboard();

extern JOBPTR inputByFile();

extern void schedule();

extern void outputOnScreen();

extern void outputByFile();

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
    outputByFile();

    return 0;
}
