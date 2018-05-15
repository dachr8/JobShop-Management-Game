#include <stdio.h>
#include "struct.h"

extern JOBPTR inputByKeyboard();

extern JOBPTR inputByFile();

extern JOBPTR inputByGraph();

extern JOBPTR schedule(JOBPTR job);

extern JOBPTR reSchedule(JOBPTR overhaul, JOBPTR machine);

extern void outputOnScreen(JOBPTR machine);

extern void outputByFile(JOBPTR machine);

extern void outputByGraph(JOBPTR machine);

int main(void) {
    int makeSpan;
    JOBPTR job, machine, overhaul;

    int inputMode;
    printf("Input Mode: 1.Keyboard 2.File 3.graph\n");
    printf("Please input your input mode: ");
    scanf("%d", &inputMode);
    switch (inputMode) {
        case 1:
            job = inputByKeyboard();
            machine = schedule(job);
            outputOnScreen(machine);
            outputByFile(machine);

            overhaul = inputByKeyboard();
            machine = reSchedule(overhaul, machine);
            outputOnScreen(machine);
            outputByFile(machine);
            break;
        case 2:
            job = inputByFile();
            machine = schedule(job);
            outputOnScreen(machine);
            outputByFile(machine);

            overhaul = inputByFile();
            machine = reSchedule(overhaul, machine);
            outputOnScreen(machine);
            outputByFile(machine);
            break;
        case 3:
            job = inputByGraph();
            machine = schedule(job);
            outputByGraph(machine);
            outputByFile(machine);

            overhaul = inputByGraph();
            machine = reSchedule(overhaul, machine);
            outputByGraph(machine);
            outputByFile(machine);
            break;
        default:
            printf("Please input your input mode: ");
            scanf("%d", &inputMode);
    }

    return 0;
}
