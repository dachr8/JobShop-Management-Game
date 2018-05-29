//author songyiwen and aojia
//co-author dachr
#include <stdlib.h>
#include "jobshop.h"

int len, **population;

void swap(int *a, int *b);

void swapPtr(int **a, int **b);

void initPopulation();

int rouletteWheelSelection(const int *makespan, int totalMakespan);

int *crossover(const int *a, const int *b);

int computeDAGAndStartTime(const int *chromosome, int mode);

int schedule() {
    int makespan[SIZE], totalMakespan = 0, lastMakespan = 0, doomClock = 0;
    initPopulation();

    for (int i = 0; i < SIZE * 5 / 4; ++i) {//Iterative
        if (doomClock == jobNum) {
            for (int j = machineNum; j < SIZE; j++)/
                 swap(&population[j][rand() % len], &population[j][rand() % len]);
            doomClock = 0;
            } else {
        for (int j = 0; j < SIZE / 2; j++)//Shuffle
            swapPtr(&population[j], &population[rand() % (SIZE / 2)]);

        for (int j = 0; j < SIZE / 4; ++j) {
            srand((unsigned) time(NULL));
            if (rand() % 10 < 7) {
                free(population[SIZE / 2 + j]);
                population[SIZE / 2 + j] = crossover(population[j], population[j + SIZE / 4]);//Crossover
                free(population[3 * SIZE / 4 + j]);
                population[3 * SIZE / 4 + j] = crossover(population[j + SIZE / 4], population[j]);
                if (rand() % 10 < 1)//Mutation
                    swap(&population[SIZE / 2 + j][rand() % len], &population[SIZE / 2 + j][rand() % len]);
                if (rand() % 10 < 1)
                    swap(&population[3 * SIZE / 4 + j][rand() % len], &population[3 * SIZE / 4 + j][rand() % len]);
            }
        }
        }
        for (int j = 0; j < SIZE; ++j) {
            makespan[j] = computeDAGAndStartTime(population[j], 0);//Get makespan
            totalMakespan += makespan[j];//
        }

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

    computeDAGAndStartTime(population[0], 1);//Mode 1 for output

    for (int i = 0; i < SIZE; ++i)
        free(population[i]);
    free(population);

    return makespan[0];
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void swapPtr(int **a, int **b) {
    int *t = *a;
    *a = *b;
    *b = t;
}

void initPopulation() {
    len = jobNum * machineNum;
    //Code the workpiece process to initialize the chromosome
    int *p = malloc(sizeof(int) * len);
    for (int i = 0, k = 0; i < jobNum; ++i)
        for (int j = 0; j < machineNum; ++j)
            p[k++] = i;
    //Perform SIZE operations on chromosomes to establish initial populations
    population = malloc(sizeof(p) * SIZE);
    for (int i = 0; i < SIZE; ++i)
        population[i] = malloc(sizeof(int) * len);

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

int rouletteWheelSelection(const int *makespan, int totalMakespan) {
    double fitness = (double) totalMakespan * rand() / RAND_MAX, total = 0;
    for (int i = 0; i < SIZE; ++i) {
        total += (double) makespan[SIZE - i - 1];
        if (total > fitness)
            return i;
    }
}

int *crossover(const int *a, const int *b) {
    int *child = malloc(sizeof(int) * len), indexA[len], indexB[len];
    for (int i = 0; i < len; ++i)
        indexA[i] = indexB[i] = 0;
    for (int i = 0; i < len; ++i)
        for (int j = 0; j < i; ++j) {
            if (a[j] == a[i])
                ++indexA[i];
            if (b[j] == b[i])
                ++indexB[i];
        }

    int startA = rand() % len, endA = startA + rand() % (len - startA), posB = rand() % (len + startA - endA);
    for (int i = 0, j = 0; i < len; ++i)
        if (posB <= i && i <= posB + endA - startA)
            child[i] = a[startA + i - posB];
        else
            for (int flag = 1; flag; ++j) {
                flag = 1;
                for (int k = startA; k <= endA && flag == 1; ++k)
                    if (a[k] == b[j] && indexA[k] == indexB[j])
                        flag = 2;
                if (flag == 1) {
                    child[i] = b[j];
                    flag = 0;
                }
            }

    return child;
}

struct graph {
    int point;
    int machine;
    int tmpTime;
    struct graph *ptrA;
    struct graph *ptrB[15];
};

int computeDAGAndStartTime(const int *chromosome, int mode) {
    int T[jobNum];//构建长度为工件数的全0数组，操作累加器
    int tasksResource[machineNum][jobNum];//存放上一次使用这台机器的节点相对工序号
    int startTime[jobNum][machineNum];//各节点的起始时间，初始化为0
    for (int i = 0; i < machineNum; ++i)
        for (int j = 0; j < jobNum; ++j)
            tasksResource[i][j] = -1;
    struct graph G[jobNum][machineNum];

    for (int i = 0; i < jobNum; ++i) {
        T[i] = 0;
        for (int j = 0; j < machineNum; ++j) {
            startTime[i][j] = 0;
            G[i][j].point = 0;
            G[i][j].tmpTime = 0;
            G[i][j].machine = 0;
            G[i][j].ptrA = NULL;
            for (int k = 0; k < 21; ++k)
                G[i][j].ptrB[k] = NULL;
        }
    }

    for (int i = 0, t = 0, num, flag[jobNum]; i < len; ++i, ++T[num]) {//对染色体进行遍历及处理
        num = chromosome[i];//num为工件号
        JOBPTR tmpPtr = job[num];//寻找工件及工序所用的临时指针
        t = T[num];

        for (int j = 0; j < t; j++)
            tmpPtr = tmpPtr->nextMachine;
        int r = tmpPtr->machine;//r为对应工件、对应工序的机器号

        G[num][t].point = i;
        G[num][t].machine = r;

        if (t < machineNum - 1)
            G[num][t].ptrA = &G[num][t + 1];//若不是num工件的最后一道工序，则将当前工序指向后一道工序的节点i
        else
            G[num][t].ptrA = NULL;//指向最后节点


        for (int p = 0; p < jobNum; p++)
            flag[p] = 1;
        for (int p = 0, o = 0; p < i; ++p)//p为染色体的操作数
            if (tasksResource[r][chromosome[p]] > -1 && flag[chromosome[p]]) {
                for (o = 0; G[chromosome[p]][tasksResource[r][chromosome[p]]].ptrB[o]; ++o);
                G[chromosome[p]][tasksResource[r][chromosome[p]]].ptrB[o] = &G[num][t];
                flag[chromosome[p]] = 0;
            }//若之前的工件占用的机器与当前nun工件j工序的相同，则将这些节点编号指向当前处理的节点i
        tasksResource[r][num] = t;
    }//构图环节完成

    for (int i = 0; i < len; ++i)
        for (int m = 0; m < jobNum; ++m)
            for (int n = 0, max = 0; n < machineNum; ++n)
                if (G[m][n].point == i) {
                    JOBPTR tmpPtr = job[m];
                    for (int k = 0; k < n; ++k)
                        tmpPtr = tmpPtr->nextMachine;
                    G[m][n].tmpTime = tmpPtr->time;
                    max = 0;
                    for (int p = 0; p < jobNum; ++p)
                        for (int q = 0; q < machineNum; ++q) {
                            if ((G[p][q].ptrA == &G[m][n]) && startTime[p][q] > max)
                                max = startTime[p][q];
                            else
                                for (int o = 0; o < jobNum; ++o)
                                    if ((G[p][q].ptrB[o] == &G[m][n]) && startTime[p][q] > max) {
                                        max = startTime[p][q];
                                        break;
                                    }
                        }
                    if (overhaul) {
                        OVERHAULPTR tmp = overhaul[tmpPtr->machine];
                        while (tmp) {
                            if (tmp->startTime <= max && tmp->endTime > max ||
                                tmp->startTime >= max && tmp->startTime < startTime[i][n])
                                max = tmp->endTime;
                            tmp = tmp->nextOverhaul;
                        }
                    }
                    startTime[m][n] = max + G[m][n].tmpTime;
                }

    int makespan = 0;
    for (int i = 0; i < jobNum; ++i)
        makespan = (startTime[i][machineNum - 1] > makespan) ? startTime[i][machineNum - 1] : makespan;

    if (mode) {
        machine = malloc(machineNum * sizeof(MACHINEPTR));
        for (int mach = 0; mach < machineNum; ++mach) {
            MACHINEPTR node = machine[mach] = malloc(sizeof(struct machine)), tmp;

            for (int i = 0; i < len; ++i)
                for (int m = 0, flag = 1; m < jobNum && flag; ++m)
                    for (int n = 0; n < machineNum && flag; ++n)
                        if (G[m][n].point == i && G[m][n].machine == mach) {
                            node->job = m;
                            node->order = n + 1;
                            node->endTime = startTime[m][n];
                            node->startTime = startTime[m][n] - G[m][n].tmpTime;
                            tmp = node;
                            node = node->nextJob = malloc(sizeof(struct machine));
                            flag = 0;
                        }
            tmp->nextJob = NULL;
            free(node);
        }
    }

    return makespan;
}


