//author dachr
#include <stdio.h>
#include "struct.h"

extern int *getJob();

extern void getOverhaul();

extern MACHINEPTR *schedule(int *times);

extern MACHINEPTR *reSchedule(MACHINEPTR *machine);

extern void output(MACHINEPTR *machine);

extern void freeAll(MACHINEPTR *machine, OVERHAULPTR overhaul, int *times);

int main(void) {
    int *times = NULL, inputMode;
    MACHINEPTR *machine = NULL;

    puts("IO Mode: 1.Keyboard 2.File");
    puts("Please input your input mode: ");
    scanf("%d", &inputMode);
    switch (inputMode) {
        case 1:
            times = getJob();
            puts("Please wait for the output result...");
            machine = schedule(times);
            output(machine);

            puts("Please enter overhaul information:");
            getOverhaul();
            machine = reSchedule(machine);
            output(machine);
            break;
        case 2:
            freopen("input.txt", "r", stdin);
            times = getJob();
            getOverhaul();
            fclose(stdin);

            puts("Please wait for the output result...");
            machine = schedule(times);
            output(machine);
            machine = reSchedule(machine);
            output(machine);
            break;
        default:
            puts("Please input your IO mode: ");
            scanf("%d", &inputMode);
    }
    freeAll(machine, overhaul, times);

    return 0;
}
