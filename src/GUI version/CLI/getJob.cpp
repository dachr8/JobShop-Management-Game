#include "jobshop.h"

void *getJob() {
    scanf("%d%d", &jobNum, &machineNum);

    job = (JOBPTR *) malloc(jobNum * sizeof(JOBPTR));

    for (int i = 0; i < jobNum; ++i) {
        JOBPTR node = job[i] = (JOBPTR) malloc(sizeof(struct job));
        for (int j = 0; j < machineNum - 1; ++j) {
            scanf("%d %d", &node->machine, &node->time);
            node = node->nextMachine = (JOBPTR) malloc(sizeof(struct job));
        }
        scanf("%d %d", &node->machine, &node->time);
        node->nextMachine = NULL;
    }
}

/*
void getOverhaul() {
    overhaul = malloc(machineNum * sizeof(OVERHAULPTR));
    for (int i = 0; i < machineNum; ++i)
        overhaul[i] = NULL;
    int num = 0, startTime, machine, time;
    while (num != -1) {
        while (scanf("%d%d%d", &startTime, &machine, &time) == 3) {
            OVERHAULPTR tmp;
            if (overhaul[machine]) {
                tmp = overhaul[machine];
                while (tmp->nextOverhaul)
                    tmp = tmp->nextOverhaul;
                tmp = tmp->nextOverhaul = malloc(sizeof(struct overhaul));
            }
            else
                tmp = overhaul[machine] = malloc(sizeof(struct overhaul));
            tmp->startTime = startTime;
            tmp->endTime = startTime + time;
            tmp->nextOverhaul = NULL;
        }
        if (scanf("%d", &num) != 1)
            return;
    }
}
*/
