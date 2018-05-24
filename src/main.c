//author dachr
#include <stdio.h>
#include "struct.h"

extern JOBPTR *getJob();

extern void getOverhaul();

extern MACHINEPTR *schedule(JOBPTR *job);

extern MACHINEPTR *reSchedule(MACHINEPTR *machine, JOBPTR *job);

extern void output(MACHINEPTR *machine, JOBPTR *job);

extern void freeAll(JOBPTR *job, MACHINEPTR *machine, OVERHAULPTR overhaul);

int main(void) {
    JOBPTR *job = NULL;
    MACHINEPTR *machine = NULL;

    int inputMode;
    puts("IO Mode: 1.Keyboard 2.File");
    puts("Please input your input mode: ");
    scanf("%d", &inputMode);
    switch (inputMode) {
        case 1:
            job = getJob();

            puts("Please wait for the output result...");
            machine = schedule(job);
            output(machine, job);

            puts("Please enter overhaul information:");
            getOverhaul();

            machine = reSchedule(machine, job);
            output(machine, job);
            break;
        case 2:
            freopen("input.txt", "r", stdin);
            job = getJob();
            getOverhaul();
            fclose(stdin);

            puts("Please wait for the output result...");
            machine = schedule(job);
            output(machine, job);

            machine = reSchedule(machine, job);
            output(machine, job);
            break;
        default:
            puts("Please input your IO mode: ");
            scanf("%d", &inputMode);
    }
    freeAll(job, machine, overhaul);

    return 0;
}
