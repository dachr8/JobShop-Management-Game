#include "jobshop.h"

void overhaul_thread() {
    if (!overhaul) {
        overhaul = (OVERHAULPTR *) malloc(machineNum * sizeof(OVERHAULPTR));
        for (int i = 0; i < machineNum; ++i)
            overhaul[i] = NULL;
    }

    int machine = (int) spinner->value();
    OVERHAULPTR tmp;
    if (overhaul[machine]) {
        tmp = overhaul[machine];
        while (tmp->nextOverhaul)
            tmp = tmp->nextOverhaul;
        tmp = tmp->nextOverhaul = (OVERHAULPTR) malloc(sizeof(struct overhaul));
    } else
        tmp = overhaul[machine] = (OVERHAULPTR) malloc(sizeof(struct overhaul));

    tmp->startTime = timeline;
    tmp->endTime = tmp->startTime + atoi(input->value());
    tmp->nextOverhaul = NULL;
    makespan = computeDAGAndStartTime(population[0], 1);//Mode 1 for output;
    scroll2->clear();
    draw_gantt_cb(scroll2);
}
