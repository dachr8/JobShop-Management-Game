//author songyiwen and aojia
//co-author dachr
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "struct.h"

#define ZJQ 100+30*jobNum*machineNum

struct graph {
    int point;
    int order;
    struct graph *ptrA;
    struct graph *ptrB[21];
};

typedef struct list {
    int job;
    int order;
    struct list *nextPtr;
} *LISTPTR;

int len;


void swap(int *a, int *b);

int **initPopulation(const int *times);

void crossover(int **population, int a, int b);

void mutation(int **population, const int *times, int num);

void computeDAGAndStartTime(const int *chromosome, const int *times);

MACHINEPTR *schedule(const int *times) {

    int chromosome[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    len = 10;
    computeDAGAndStartTime(chromosome, times);

    return NULL;
}

MACHINEPTR *reSchedule(MACHINEPTR *machine) {
    return NULL;
}

void swap(int *a, int *b) {
    int s;
    s = *a;
    *a = *b;
    *b = s;
}

int **initPopulation(const int *times) {
    len = 0;
    for (int i = 0; i < jobNum; ++i)
        len += times[i];

    int *p = malloc(sizeof(int) * len);    //对工件加工过程进行编码 即初始染色体
    for (int i = 0, k = 0; i < jobNum; ++i) //jobNum 工件数目
        for (int j = 0; j < times[i]; ++j)  //jobmachinenum 当前工件所需加工步骤数
            p[k++] = i;
    //ps 初始化的种群数量

    int **population = malloc(sizeof(p) * ZJQ);//存储ps条染色体
    srand((unsigned) time(NULL));
    for (int i = 0; i < ZJQ; ++i, population[i] = p)            //对染色体p进行ZJQ次操作 建立初始种群
        for (int j = 0; j < len; ++j)  //随机打乱基因顺序.times[j]是p染色体的长度
            swap(&p[j], &p[rand() % times[j]]);

    return population;
}


LISTPTR cross_buildList(int **population, int pick) {                        //交叉-建立有序偶
    LISTPTR headPtr = NULL, curPtr, lastPtr = NULL;
    int time[jobNum];
    for (int i = 0; i < jobNum; ++i)
        time[i] = 1;
    for (int i = 0; i < len; ++i) {
        curPtr = malloc(sizeof(struct list));
        if (curPtr) {
            curPtr->job = population[pick][i];
            int a = population[pick][i];
            curPtr->order = time[a]++;
            if (!headPtr) {
                headPtr = curPtr;
                lastPtr = headPtr;
                headPtr->nextPtr = NULL;
            } else {
                lastPtr->nextPtr = curPtr;
                lastPtr = curPtr;
            }
        }
    }

    return headPtr;
}

void cross_insert(LISTPTR insertPtr, LISTPTR forwardPtr, LISTPTR backPtr) {     //交叉-插入部分
    LISTPTR innextPtr = insertPtr->nextPtr;
    LISTPTR forPtr = forwardPtr->nextPtr;
    LISTPTR bkPtr = backPtr->nextPtr;
    forwardPtr->nextPtr = bkPtr;
    insertPtr->nextPtr = forPtr;
    backPtr->nextPtr = innextPtr;
}

void crossover(int **population, int a, int b) {                   // 交叉主函数
    LISTPTR bPtr = cross_buildList(population, b);
    int clen = rand() % (len / 2) + 1;

    LISTPTR forwardPtr = cross_buildList(population, a);                       //找a起始、终止交换的位置 b中插入a的位置
    for (int i = 0; i < rand() % ((len / 2) + 1); ++i)
        forwardPtr = forwardPtr->nextPtr;
    LISTPTR backPtr = forwardPtr;
    for (int i = 0; i < clen + 1; ++i)
        backPtr = backPtr->nextPtr;
    LISTPTR insertPtr = bPtr;
    for (int i = 0; i < rand() % ((len / 2) + 1); ++i)
        insertPtr = insertPtr->nextPtr;

    cross_insert(insertPtr, forwardPtr, backPtr);               //将a截出的片段差入b中

    LISTPTR tPtr = bPtr->nextPtr, fPtr = bPtr;
    for (int i = 0; i < clen; ++i) {
        while (tPtr) {
            if (tPtr->job == forwardPtr->job && tPtr->order == forwardPtr->order) {
                fPtr->nextPtr = tPtr->nextPtr;
                forwardPtr = forwardPtr->nextPtr;
            }
            fPtr = fPtr->nextPtr;
            tPtr = fPtr->nextPtr;
        }
        tPtr = bPtr;
    }
    for (int i = 0, g = ZJQ; i < len; ++i) {//将交叉后的子代放到种群中
        population[++g][i] = bPtr->job;
        bPtr = bPtr->nextPtr;
    }
}

void mutation(int **population, const int *times, int num) {
    srand((unsigned) time(NULL));//随机选两个位置调换
    swap(&population[num][rand() % times[num]], &population[num][rand() % times[num]]);
}

void computeDAGAndStartTime(const int *chromosome, const int *times) {
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
    for (int i = 0; i < jobNum; ++i) {
        if (startTime[i][times[i]] > makeSpan)
            makeSpan = startTime[i][times[i]];
    }
}
