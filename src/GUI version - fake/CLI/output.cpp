#include "jobshop.h"

void output(int makespan) {
    for (int i = 0, last = 0; i < machineNum; ++i) {
        printf("\nM%d", i);
        MACHINEPTR tmp = machine[i];

        last = 0;
        OVERHAULPTR tmp2 = NULL;


        while (tmp) {
            if (overhaul) {
                tmp2 = overhaul[i];
                int flag = 1;
                while (tmp2 && flag)
                    if (tmp2->endTime <= tmp->startTime && tmp2->startTime >= last) {
                        printf(" (%d,\"检修\",%d)", tmp2->startTime, tmp2->endTime);
                        tmp2 = tmp2->nextOverhaul;
                    } else
                        flag = 0;
            }
            printf(" (%d,%d-%d,%d)", tmp->startTime, tmp->job, tmp->order, tmp->endTime);
            last = tmp->startTime;
            tmp = tmp->nextJob;
        }
    }
    printf("\nTime Used: %.3lfs", usedTime);
    printf("\nEnd Time: %d\n\n", makespan);
}
