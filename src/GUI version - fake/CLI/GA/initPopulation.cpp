#include "jobshop.h"

void initPopulation() {
    len = jobNum * machineNum;
    //Code the workpiece process to initialize the chromosome
    int *p = (int *) malloc(sizeof(int) * len);
    for (int i = 0, k = 0; i < jobNum; ++i)
        for (int j = 0; j < machineNum; ++j)
            p[k++] = i;
    //Perform SIZE operations on chromosomes to establish initial populations
    population = (int **) malloc(sizeof(p) * SIZE);
    for (int i = 0; i < SIZE; ++i)
        population[i] = (int *) malloc(sizeof(int) * len);

    srand((unsigned) time(NULL));
    for (int i = 0; i < SIZE; i++) {//Randomly disrupted gene sequence, len is the length of the p chromosome
        for (int j = 0; j < len; ++j) {
            swap(&p[j], &p[rand() % len]);
            for (int k = 0; k < len; ++k)
                population[i][k] = p[k];
        }
    }
    free(p);
}
