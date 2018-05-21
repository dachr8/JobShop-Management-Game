//author dachr
#include <stdio.h>
#include "struct.h"

extern JOBPTR *getJobByStdin();

extern void getOverhaulByStdin();

extern MACHINEPTR *schedule(JOBPTR *job);

extern MACHINEPTR *reSchedule(MACHINEPTR *machine, JOBPTR *job);

extern void outputByStdout(MACHINEPTR *machine, JOBPTR *job);

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
            job = getJobByStdin();
            printf("\nPlease wait for the output result...\n");
            machine = schedule(job);
            outputByStdout(machine, job);

            printf("\nPlease enter overhaul information:\n");
            getOverhaulByStdin();
            machine = reSchedule(machine, job);
            outputByStdout(machine, job);
            break;
        case 2:
            freopen("input.txt", "r", stdin);
            job = getJobByStdin();
            getOverhaulByStdin();
            fclose(stdin);
            machine = schedule(job);
            freopen("output.txt", "w", stdout);
            outputByStdout(machine, job);

            machine = reSchedule(machine, job);
            outputByStdout(machine, job);
            fclose(stdout);
            break;
        default:
            printf("Please input your IO mode: ");
            scanf("%d", &inputMode);
    }
    freeAll(job, machine, overhaul);

    return 0;
}
