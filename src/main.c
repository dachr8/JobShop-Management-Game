//author dachr
#include <stdio.h>
#include "struct.h"

extern int *getJob();

extern void getOverhaul();

extern int schedule(const int *times);

extern int reSchedule();

extern void output(int makespan);

extern void freeAll(MACHINEPTR *machine, OVERHAULPTR overhaul, int *times);

int main(void) {
    int *times = NULL, inputMode;

    puts("IO Mode: 1.Keyboard 2.File");
    puts("Please input your input mode: ");
    scanf("%d", &inputMode);
    switch (inputMode) {
        case 1:
            puts("Please input the data:");
            times = getJob();
            puts("Please wait for the output result...");
            output(schedule(times));

            puts("Please enter overhaul information:");
            getOverhaul();
            output(reSchedule());
            break;
        case 2:
            freopen("input.txt", "r", stdin);
            times = getJob();
            //getOverhaul();
            fclose(stdin);

            puts("Please wait for the output result...");
            output(schedule(times));
            //output(reSchedule());
            break;
        default:
            puts("Please input your IO mode: ");
            scanf("%d", &inputMode);
    }
    freeAll(machine, overhaul, times);

    return 0;
}
