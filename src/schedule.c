//author songyiwen and aojia
//co-author dachr
#include <stdlib.h>
#include <time.h>
#include "jobshop.h"

int len, **population;

void swap(int *a, int *b);

void initPopulation(const int *times);

int rouletteWheelSelection(const int *makespan, int totalMakespan);

int *crossover(const int *a, const int *b);

int computeDAGAndStartTime(const int *chromosome, const int *times);

int schedule(const int *times) {
    int makespan[SIZE], totalMakespan = 0;
    initPopulation(times);
    for (int i = 0; i < SIZE; ++i) {
        makespan[i] = computeDAGAndStartTime(population[i], times);
        totalMakespan += makespan[i];
        for (int j = 0; j < len; ++j) {
            printf("%d", population[i][j]);
        }
        printf(" makespan:%d\n", makespan[i]);
    }/*
    for (int i = 0, flag = 1; (i < SIZE - 1) && flag; ++i) {
        flag = 0;
        for (int j = 0; j < SIZE - 1; ++j)
            if (makespan[j] > makespan[j + 1]) {
                swap(&makespan[j], &makespan[j + 1]);
                swap(population[j], population[j + 1]);
                flag = 1;
            }
    }

    for (int i = 0; i < SIZE; ++i) {
        srand((unsigned) time(NULL));
        int a = rouletteWheelSelection(makespan, totalMakespan), b = rouletteWheelSelection(makespan, totalMakespan);
        while (a == b)
            b = rouletteWheelSelection(makespan, totalMakespan);
        int *child = crossover(population[a], population[b]);
        if (!rand() % 10)
            swap(&child[rand() % len], &child[rand() % len]);
        int childMakespan = computeDAGAndStartTime(child, times);
        for (int j = 0, flag = 1; j < SIZE && flag; ++j)
            if (childMakespan < makespan[j]) {
                totalMakespan = totalMakespan + childMakespan - makespan[SIZE - 1];
                for (int k = j; k < SIZE; ++k) {
                    swap(&childMakespan, &makespan[k]);
                    swap(child, population[k]);
                }
                flag = 0;
            }
        free(child);
    }

    return makespan[0];*/
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void initPopulation(const int *times) {
    len = 0;
    for (int i = 0; i < jobNum; ++i)
        len += times[i];
    //Code the workpiece process to initialize the chromosome
    int *p = malloc(sizeof(int) * len);
    for (int i = 0, k = 0; i < jobNum; ++i)
        for (int j = 0; j < times[i]; ++j)
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
    double fitness = totalMakespan * rand() / (double) RAND_MAX, total = 0;
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

    int start = rand() % len, end = start + rand() % (len - start);
    for (int i = 0, j = 0; i < len; ++i)
        if (start <= i && i <= end)
            child[i] = b[i];
        else
            for (int flag = 1; flag; ++j) {
                flag = 1;
                for (int k = start; k <= end && flag == 1; ++k)
                    if (a[j] == b[k] && indexA[j] == indexB[k])
                        flag = 2;
                if (flag == 1) {
                    child[i] = a[j];
                    flag = 0;
                }
            }

    return child;
}

struct graph {
    int point;
    int machine;
    struct graph *ptrA;
    struct graph *ptrB[21];
};

int computeDAGAndStartTime(const int *chromosome, const int *times) {
    int num = 0, t = 0, p = 0, o = 0;//为计算变量
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
            G[i][j].machine = 0;
            G[i][j].ptrA = NULL;
            for (int k = 0; k < 21; ++k)
                G[i][j].ptrB[k] = NULL;
        }
    }
    for (int i = 0; i < len; ++i, ++T[num]) {//对染色体进行遍历及处理
        num = chromosome[i];//num为工件号
        int flag[jobNum];
        JOBPTR tmpPtr = job[num];//寻找工件及工序所用的临时指针
        t = T[num];
        for (int j = 0; j < t; j++)
            tmpPtr = tmpPtr->nextMachine;
        int r = tmpPtr->machine;//r为对应工件、对应工序的机器号
        G[num][t].point = i;
        G[num][t].machine = r;
        if (t == times[num])
            G[num][t].ptrA = NULL;//指向最后节点
        else if (t)
            G[num][t - 1].ptrA = &G[num][t];//若不是num工件的第一道工序，则将前一道工序指向代表当前工序的节点i
        for (p = 0; p < jobNum; p++)
            flag[p] = 1;
        for (p = 0; p < i && flag[chromosome[p]]; ++p)//p为染色体的操作数
            if (tasksResource[r][chromosome[p]] > -1) {
                for (o = 0; G[chromosome[p]][tasksResource[r][chromosome[p]]].ptrB[o] != NULL; ++o);
                G[chromosome[p]][tasksResource[r][chromosome[p]]].ptrB[o] = &G[num][t];
                flag[chromosome[p]] = 0;
            }//若之前的工件占用的机器与当前nun工件j工序的相同，则将这些节点编号指向当前处理的节点i
        tasksResource[r][num] = t;
    }//构图环节完成

    int current = 0, max = 0;
    for (int i = 0; i < jobNum; ++i) //遍历析取图节点,i代表工件数
        for (int j = 0; j < machineNum; ++j) //j代表工序数
        {
            JOBPTR tmpPtr = job[i];
            for (int m = 0; m < j; ++m)
                tmpPtr = tmpPtr->nextMachine;
            current = tmpPtr->time;
            max = 0;
            for (int m = 0; m < jobNum; ++m)
                for (int n = 0; n < machineNum; ++n) {
                    if (G[m][n].ptrA == &G[i][j] && startTime[m][n] > max)
                        max = startTime[m][n];
                    else
                        for (o = 0; o < 21; ++o)
                            if ((G[m][n].ptrB[o] == &G[i][j]) && startTime[m][n] > max) {
                                max = startTime[m][n];
                                break;
                            }
                }
            startTime[i][j] = max + current;
        }
    int makespan = 0;
    for (int i = 0; i < jobNum; ++i)
        if (startTime[i][times[i] - 1] > makespan)
            makespan = startTime[i][times[i] - 1];
    return makespan;

}
