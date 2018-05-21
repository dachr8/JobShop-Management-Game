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
    printf("IO Mode: 1.Keyboard 2.File\n");
    printf("Please input your input mode: ");
    scanf("%d", &inputMode);
    switch (inputMode) {
        case 1:
            job = getJob();
            printf("\nPlease wait for the output result...\n");
            machine = schedule(job);
            output(machine, job);

            printf("\nPlease enter overhaul information:\n");
            getOverhaul();
            machine = reSchedule(machine, job);
            output(machine, job);
            break;
        case 2:
            freopen("input.txt", "r", stdin);
            freopen("output.txt", "w", stdout);
            job = getJob();
            getOverhaul();
            machine = schedule(job);
            output(machine, job);

            machine = reSchedule(machine, job);
            output(machine, job);
            fclose(stdin);
            fclose(stdout);
            break;
        default:
            printf("Please input your IO mode: ");
            scanf("%d", &inputMode);
    }
    freeAll(job, machine, overhaul);

    return 0;
}
