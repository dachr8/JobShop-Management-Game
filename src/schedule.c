//author songyiwen and aojia
//co-author dachr
#include <stdlib.h>
#include <time.h>
#include "jobshop.h"

int len, **population;

void swap(int *a, int *b);

void initPopulation(const int *times);

int *crossover(const int *a, const int *b);

int computeDAGAndStartTime(const int *chromosome, const int *times);

int schedule(const int *times) {
    int makespan[SIZE];
    initPopulation(times);
    for (int i = 0; i < SIZE; ++i)
        makespan[i] = computeDAGAndStartTime(population[i], times);
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
        int *child = crossover(population[rand() % len], population[rand() % len]);
        if (!rand() % 10)
            swap(&child[rand() % len], &child[rand() % len]);
        int childMakespan = computeDAGAndStartTime(child, times);
        for (int j = 0, flag = 1; j < SIZE && flag; ++j)
            if (childMakespan < makespan[j]) {
                for (int k = j; k < SIZE; ++k) {
                    swap(&childMakespan, &makespan[k]);
                    swap(child, population[k]);
                }
                flag = 0;
            }
        free(child);
    }

    return makespan[0];
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
    srand((unsigned) time(NULL));
    for (int i = 0; i < SIZE; population[i++] = p)
        for (int j = 0; j < len; ++j)//Randomly disrupted gene sequence, len is the length of the p chromosome
            swap(&p[j], &p[rand() % len]);
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
    int order;
    struct graph *ptrA;
    struct graph *ptrB[21];
};

int computeDAGAndStartTime(const int *chromosome, const int *times) {
    int num, t, p;//为计算变量
    int T[jobNum];//构建长度为工件数的全0数组，操作累加器
    int tasksResource[machineNum][jobNum];//存放上一次使用这台机器的节点相对工序号
    int st[len];//记录每个节点对应的工序在其工件内使第几道工序
    int startTime[jobNum][machineNum];//各节点的起始时间，初始化为0
    //int lastTaskJob[len];//构造存储每个上一次加入图的工序号，初始化为-1********
    /*for(i = 0;i<len;i++)
    {
        lastTaskJob[i] = -1;
    }*/
    for (int i = 0; i < machineNum; ++i)
        for (int j = 0; j < jobNum; ++j)
            tasksResource[i][j] = -1;

    struct graph G[jobNum][machineNum];
    for (int i = 0; i < jobNum; ++i) {
        T[i] = 0;
        for (int j = 0; j < machineNum; ++j) {
            startTime[i][j] = 0;
            G[i][j].ptrA = NULL;
            for (int k = 0; k < jobNum; ++k)
                G[i][j].ptrB[k] = NULL;
        }
    }

    for (int i = 0; i < len; ++i, ++T[num]) {//对染色体进行遍历及处理
        num = chromosome[i];//num为工件号
        JOBPTR tmpPtr = job[num];//寻找工件及工序所用的临时指针
        st[i] = t = T[num];//存入节点对应的工序在其工件内是第几道工序
        for (int j = 0; j < t; ++j)
            tmpPtr = tmpPtr->nextMachine;
        int r = tmpPtr->machine;//r为对应工件、对应工序的机器号
        G[num][t].point = i;
        G[num][t].order = t;
        if (t + 1 == times[num])
            G[num][t].ptrA = NULL;//指向最后节点
        else if (t)
            G[num][t - 1].ptrA = &G[num][t];//若不是num工件的第一道工序，则将前一道工序指向代表当前工序的节点i
        for (p = 0; p < i; ++p)//p为染色体的操作数
            if (tasksResource[r][chromosome[p]] > -1)
                for (int o = 0; o < jobNum; ++o)
                    for (int k = 0; k < machineNum; ++k)
                        if (G[o][k].point == p)
                            for (int q = 0; G[o][k].ptrB[q] != NULL; q++)
                                G[o][k].ptrB[q] = &G[num][t];
        //若之前的工件占用的机器与当前nun工件j工序的相同，则将这些节点编号指向当前处理的节点i
        //lastTaskJob[num] = i;
        tasksResource[r][num] = i;
        //构图环节完成
    }

    for (int i = 0; i < jobNum; ++i)//遍历析取图节点
        for (int j = 0; j < machineNum; ++j) {
            int max = 0;
            JOBPTR tmpPtr = job[i];
            if (!j)
                startTime[i][j] = 0;
            else {
                for (int m = 0; m < G[i][j].order; ++m)
                    tmpPtr = tmpPtr->nextMachine;

                int current = tmpPtr->time;
                for (int m = 0; m < i; ++m)
                    for (int n = 0; n < j; ++n)
                        for (int o = 0; G[m][n].ptrB[o] != NULL; ++o)
                            if ((G[m][n].ptrA == &G[i][j] || G[m][n].ptrB[o] == &G[i][j]) && startTime[m][n] > max)
                                max = startTime[m][n];
                startTime[i][j] = max + current;
            }
        }
    int makespan = 0;
    for (int i = 0; i < jobNum; ++i)
        if (startTime[i][times[i]] > makespan)
            makespan = startTime[i][times[i]];

    return makespan;
}
