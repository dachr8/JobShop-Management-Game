//author dachr
#include <stdio.h>
#include "struct.h"

extern JOBPTR *getJobByKeyboard();

extern void getOverhaulByKeyboard();

extern JOBPTR *getByFile();

extern MACHINEPTR *schedule(JOBPTR *job);

extern MACHINEPTR *reSchedule(MACHINEPTR *machine, JOBPTR *job);

extern void outputOnScreen(MACHINEPTR *machine, JOBPTR *job);

extern void outputByFile(MACHINEPTR *machine, JOBPTR *job);

extern void freeAll(JOBPTR *job, MACHINEPTR *machine, OVERHAULPTR overhaul);

int main(void) {
    JOBPTR *job = NULL;
    MACHINEPTR *machine = NULL;

    int inputMode;
    printf("Input Mode: 1.Keyboard 2.File\n");
    printf("Please input your input mode: ");
    scanf("%d", &inputMode);
    switch (inputMode) {
        case 1:
            job = getJobByKeyboard();
            printf("\nPlease wait for the output result...\n");
            machine = schedule(job);
            outputOnScreen(machine, job);
            outputByFile(machine, job);

            printf("\nPlease enter overhaul information:\n");
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
        default:
            printf("Please input your input mode: ");
            scanf("%d", &inputMode);
    }
    freeAll(job, machine, overhaul);

    return 0;
}
