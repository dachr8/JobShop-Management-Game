#include<stdio.h>
#include "struct.h"

int main()
{
    return 0;
}

int ComputeDAGAndStartTime(JOBPTR*job,int jobNum,int machineNum,const int *rst,const int *times)
{
    struct graph{
        int point;
        int order;
        struct graph *ptrA;
        struct graph *ptrB[jobNum];
    };
    typedef struct graph GRAPH;
    int i,j,num,t,p;//为计算变量
    int current = 0;
    int len = 0;//染色体长度
    for(len = 0;rst[len]!='\0';len++);
    int T[jobNum];//构建长度为工件数的全0数组，操作累加器
    //int lastTaskJob[len];//构造存储每个上一次加入图的工序号，初始化为-1********
    int tasksResource[machineNum][jobNum];//存放上一次使用这台机器的节点相对工序号
    int st[len];//记录每个节点对应的工序在其工件内使第几道工序

    for(i = 0;i<jobNum;i++)
        T[i] = 0;

    /*for(i = 0;i<len;i++)
    {
        lastTaskJob[i] = -1;
    }*/
    for(i = 0;i<machineNum;i++)
    {
        for(j = 0;j<jobNum;j++)
            tasksResource[i][j] = -1;
    }

    GRAPH G[jobNum][machineNum];
    for(i = 0;i<jobNum;i++)
    {
        for(j = 0;j<machineNum;j++)
        {
            G[i][j].ptrA = NULL;
            for(int m  = 0;m<jobNum;m++)
                G[i][j].ptrB[m] = NULL;
        }
    }
    for(i = 0;i<len;i++)//对染色体进行遍历及处理
    {
        JOBPTR tempPtr;//寻找工件及工序所用的临时指针
        int r;//r为对应工件、对应工序的机器号
        num = rst[i];//num为工件号
        tempPtr = job[0];
        t = T[num];
        st[i] = t;//存入节点对应的工序在其工件内是第几道工序
        for (int a = 0; a < num; a++) {
            tempPtr = tempPtr->nextMachine;
        }
        r = tempPtr->machine;//机器号
        G[num][t].point = i;
        G[num][t].order = t;
        if (t + 1 == times[num]) {
            G[num][t].ptrA = NULL;
        }//指向最后节点
        else if (t > 0) {
            G[num][t - 1].ptrA = &G[num][t];
        }//若不是num工件的第一道工序，则将前一道工序指向代表当前工序的节点i
        for (p = 0; p < i; p++)//p为染色体的操作数
        {
            if (tasksResource[r][rst[p]] > -1) {
                for (int o = 0; o < jobNum; o++) {
                    for (int k = 0; k < machineNum; k++) {
                        if (G[o][k].point == p) {
                            for (int q = 0; G[o][k].ptrB[q] != NULL; q++) {
                                G[o][k].ptrB[q] = &G[num][t];
                            }
                        }
                    }
                }
            }
        }//若之前的工件占用的机器与当前nun工件j工序的相同，则将这些节点编号指向当前处理的节点i

        T[num]++;
        //lastTaskJob[num] = i;
        tasksResource[r][num] = i;
        //构图环节完成
    }

        int startTime[jobNum][machineNum];//各节点的起始时间，初始化为0
        for(i = 0;i<jobNum;i++)
        {
            for(j = 0;j<machineNum;j++)
                startTime[i][j] = 0;
        }
        int max;
        for(i = 0;i<jobNum;i++)//遍历析取图节点
        {
            for(j = 0;j<machineNum;j++)
            {
                max = 0;
                JOBPTR tempPtr = job[0];
                if(j == 0)
                    startTime[i][j] = 0;
                else
                {
                    for(int m = 0;m<G[i][j].order;m++)
                    {
                        tempPtr = tempPtr->nextMachine;
                    }
                    current = tempPtr->time;
                    for(int m = 0;m<jobNum;m++)
                    {
                        for(int n = 0;n<machineNum;n++)
                        {
                            for(int o = 0;G[m][n].ptrB[o]!=NULL;o++)
                            {
                                if(G[m][n].ptrA == &G[i][j]||G[m][n].ptrB[o] == &G[i][j])
                                {
                                    if(startTime[m][n]>max)
                                    {
                                        max = startTime[m][n];
                                    }
                                }
                            }
                        }
                    }

                    startTime[i][j] = max + current;
                }
            }
        }
        int makespan = 0;
        for(i = 0;i<jobNum;i++)
        {
            if(startTime[i][times[i]]>makespan)
                makespan = startTime[i][times[i]];
        }
        return  makespan;
}
