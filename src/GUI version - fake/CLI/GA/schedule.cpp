#include "jobshop.h"

int schedule() {
    int makespan[SIZE], lastMakespan = 0, doomClock = 0, maxL = 2, survivor = machineNum;
    initPopulation();
    clock_t startClock = clock();
    if (jobNum * machineNum >= 100) {
        ++maxL;
        survivor = 1;
    }
    for (int l = 0; l < maxL && (clock() - startClock < LIMIT / (l + 1)); ++l, doomClock = DOOM) {
        for (int i = 0; i < SIZE * 8; ++i) {//Iterative
            if (doomClock == DOOM) {
                for (int j = survivor; j < SIZE; ++j)
                    swap(&population[j][rand() % len], &population[j][rand() % len]);
                doomClock = 0;
            } else {
                for (int j = 0; j < SIZE / 2; ++j)//Shuffle
                    swapPtr(&population[j], &population[rand() % (SIZE / 2)]);

                for (int j = 0; j < SIZE / 4; ++j) {
                    srand((unsigned) time(NULL));
                    if (rand() % 10 < 7) {
                        free(population[SIZE / 2 + j]);
                        population[SIZE / 2 + j] = crossover(population[j],
                                                             population[j + SIZE / 4]);//Crossover
                        free(population[3 * SIZE / 4 + j]);
                        population[3 * SIZE / 4 + j] = crossover(population[j + SIZE / 4], population[j]);
                        if (rand() % 10 < 1)//Mutation
                            swap(&population[SIZE / 2 + j][rand() % len],
                                 &population[SIZE / 2 + j][rand() % len]);
                        if (rand() % 10 < 1)
                            swap(&population[3 * SIZE / 4 + j][rand() % len],
                                 &population[3 * SIZE / 4 + j][rand() % len]);
                    }
                }
            }

            for (int j = 0; j < SIZE; ++j)
                makespan[j] = computeDAGAndStartTime(population[j], 0);//Get makespan

            for (int j = 0, flag = 1; (j < SIZE - 1) && flag; ++j) {
                flag = 0;
                for (int k = 0; k < SIZE - 1; ++k)//Bubble sort by makespan
                    if (makespan[k] >= makespan[k + 1]) {
                        swap(&makespan[k], &makespan[k + 1]);
                        swapPtr(&population[k], &population[k + 1]);
                        flag = 1;
                    }
            }

            doomClock = makespan[0] == lastMakespan ? doomClock + 1 : 0;
            lastMakespan = makespan[0];
        }
    }

    computeDAGAndStartTime(population[0], 1);//Mode 1 for output

    for (int i = 1; i < SIZE; ++i)
        free(population[i]);

    return makespan[0];
}
