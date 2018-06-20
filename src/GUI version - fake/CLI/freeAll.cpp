#include "jobshop.h"

void freeAll() {
    for (int i = 0; i < jobNum; ++i) {
        while (job[i]) {
            JOBPTR tmp = job[i];
            job[i] = job[i]->nextMachine;
            free(tmp);
        }
    }
    free(job);
    for (int i = 0; i < machineNum; ++i) {
        while (machine[i]) {
            MACHINEPTR tmp1 = machine[i];
            machine[i] = machine[i]->nextJob;
            free(tmp1);
        }

        if (overhaul)
            while (overhaul[i]) {
                OVERHAULPTR tmp2 = overhaul[i];
                overhaul[i] = overhaul[i]->nextOverhaul;
                free(tmp2);
            }
    }
    free(machine);
    if (population) {
        free(population[0]);
        free(population);
    }
}
