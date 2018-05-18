//author dachr
#include <stdio.h>
#include "struct.h"

extern JOBPTR getJobByKeyboard();

extern void getOverhaulByKeyboard();

extern JOBPTR getByFile();

extern JOBPTR getJobByGraph();

extern void getOverhaulByGraph();

extern MACHINEPTR schedule(JOBPTR job);

extern MACHINEPTR reSchedule(MACHINEPTR machine);

extern void outputOnScreen(MACHINEPTR machine);

extern void outputByFile(MACHINEPTR machine);

extern void outputByGraph(MACHINEPTR machine);

int main(void) {
    JOBPTR job;
    MACHINEPTR machine;

    int inputMode;
    printf("Input Mode: 1.Keyboard 2.File 3.graph\n");
    printf("Please input your input mode: ");
    scanf("%d", &inputMode);
    switch (inputMode) {
        case 1:
            job = getJobByKeyboard();
            machine = schedule(job);
            outputOnScreen(machine);
            outputByFile(machine);

            getOverhaulByKeyboard();

            machine = reSchedule(machine);
            outputOnScreen(machine);
            outputByFile(machine);
            break;
        case 2:
            job = getByFile();
            machine = schedule(job);
            outputOnScreen(machine);
            outputByFile(machine);

            machine = reSchedule(machine);
            outputOnScreen(machine);
            outputByFile(machine);
            break;
        case 3:
            job = getJobByGraph();
            machine = schedule(job);
            outputByGraph(machine);
            outputByFile(machine);

            getOverhaulByGraph();

            machine = reSchedule(machine);
            outputByGraph(machine);
            outputByFile(machine);
            break;
        default:
            printf("Please input your input mode: ");
            scanf("%d", &inputMode);
    }

    return 0;
}
