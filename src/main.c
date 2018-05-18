//author dachr
#include <stdio.h>
#include "struct.h"

extern JOBPTR *getJobByKeyboard();

extern void getOverhaulByKeyboard();

extern JOBPTR *getByFile();

extern JOBPTR *getJobByGraph();

extern void getOverhaulByGraph();

extern MACHINEPTR *schedule(JOBPTR *job);

extern MACHINEPTR *reSchedule(MACHINEPTR *machine, JOBPTR *job);

extern void outputOnScreen(MACHINEPTR *machine, JOBPTR *job);

extern void outputByFile(MACHINEPTR *machine, JOBPTR *job);

extern void outputByGraph(MACHINEPTR *machine, JOBPTR *job);

int main(void) {
    JOBPTR *job;
    MACHINEPTR *machine;

    int inputMode;
    printf("Input Mode: 1.Keyboard 2.File 3.graph\n");
    printf("Please input your input mode: ");
    scanf("%d", &inputMode);
    switch (inputMode) {
        case 1:
            job = getJobByKeyboard();
            machine = schedule(job);
            outputOnScreen(machine, job);
            outputByFile(machine, job);

            getOverhaulByKeyboard();

            machine = reSchedule(machine, job);
            outputOnScreen(machine, job);
            outputByFile(machine, job);
            break;
        case 2:
            job = getByFile();
            machine = schedule(job);
            outputOnScreen(machine, job);
            outputByFile(machine, job);

            machine = reSchedule(machine, job);
            outputOnScreen(machine, job);
            outputByFile(machine, job);
            break;
        case 3:
            job = getJobByGraph();
            machine = schedule(job);
            outputByGraph(machine, job);
            outputByFile(machine, job);

            getOverhaulByGraph();

            machine = reSchedule(machine, job);
            outputByGraph(machine, job);
            outputByFile(machine, job);
            break;
        default:
            printf("Please input your input mode: ");
            scanf("%d", &inputMode);
    }

    return 0;
}
