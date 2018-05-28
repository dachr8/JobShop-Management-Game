//author dachr
#include "jobshop.h"

extern int **population;

extern int *getJob();

extern void getOverhaul();

extern int schedule(const int *times);

extern void output(int makespan);

extern void freeAll(int *times, int **population);

extern int *crossover(const int *a, const int *b);

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

            puts("Please wait for the output result...");
            output(schedule(times));
            break;
        case 2:
            freopen("input.txt", "r", stdin);
            times = getJob();

            puts("Please wait for the output result...");
            schedule(times);
            output(schedule(times));

            getOverhaul();
            fclose(stdin);

            puts("Please wait for the output result...");
            //output(schedule(times));
            break;
        default:
            puts("Please input your IO mode: ");
            scanf("%d", &inputMode);
    }
    freeAll(times, population);

    return 0;
}
