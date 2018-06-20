#include "jobshop.h"

struct graph {
    int point;
    int machine;
    int tmpTime;
    struct graph *ptr[20];
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
            for (int k = 0; k < 21; ++k)
                G[i][j].ptr[k] = NULL;
        }
    }

    for (int i = 0, t = 0, max, num, flag[jobNum]; i < len; ++i, ++T[num]) {//对染色体进行遍历及处理
        num = chromosome[i];//num为工件号
        JOBPTR tmpPtr = job[num];//寻找工件及工序所用的临时指针
        t = T[num];

        for (int j = 0; j < t; j++)
            tmpPtr = tmpPtr->nextMachine;
        int r = tmpPtr->machine;//r为对应工件、对应工序的机器号

        G[num][t].tmpTime = tmpPtr->time;
        G[num][t].point = i;
        G[num][t].machine = r;


        max = t == 0 ? 0 : startTime[num][t - 1];
        for (int p = 0; p < jobNum; p++)
            flag[p] = 1;
        for (int p = 0, o = 0; p < i; ++p)//p为染色体的操作数
            if (tasksResource[r][chromosome[p]] > -1 && flag[chromosome[p]]) {
                for (o = 0; G[chromosome[p]][tasksResource[r][chromosome[p]]].ptr[o]; ++o);
                G[chromosome[p]][tasksResource[r][chromosome[p]]].ptr[o] = &G[num][t];
                flag[chromosome[p]] = 0;

                max = startTime[chromosome[p]][tasksResource[r][chromosome[p]]] > max
                      ? startTime[chromosome[p]][tasksResource[r][chromosome[p]]] : max;
            }//若之前的工件占用的机器与当前nun工件j工序的相同，则将这些节点编号指向当前处理的节点i
        tasksResource[r][num] = t;
        startTime[num][t] = max + G[num][t].tmpTime;
        if (overhaul) {
            OVERHAULPTR tmp = overhaul[r];
            while (tmp) {
                /*
                if (tmp->startTime <= max && tmp->endTime > max ||
                    tmp->startTime >= max && tmp->startTime < startTime[num][t])
                    max = tmp->endTime;
                */
                if (tmp->startTime > max && tmp->startTime < startTime[num][t]) {
                    int time = tmp->endTime - tmp->startTime;
                    tmp->startTime = startTime[num][t];
                    tmp->endTime = tmp->startTime + time;
                }
                if (max >= tmp->startTime && max < tmp->endTime)
                    max = tmp->endTime;
                tmp = tmp->nextOverhaul;
            }
        }
        startTime[num][t] = max + G[num][t].tmpTime;
    }

    int makespan = 0;
    for (int i = 0; i < jobNum; ++i)
        makespan = (startTime[i][machineNum - 1] > makespan) ? startTime[i][machineNum - 1] : makespan;

    if (mode) {
        machine = (MACHINEPTR *) malloc(machineNum * sizeof(MACHINEPTR));
        for (int mach = 0; mach < machineNum; ++mach) {
            MACHINEPTR node = machine[mach] = (MACHINEPTR) malloc(sizeof(struct machine)), tmp;

            for (int i = 0; i < len; ++i)
                for (int m = 0, flag = 1; m < jobNum && flag; ++m)
                    for (int n = 0; n < machineNum && flag; ++n)
                        if (G[m][n].point == i && G[m][n].machine == mach) {
                            node->job = m;
                            node->order = n;
                            node->endTime = startTime[m][n];
                            node->startTime = startTime[m][n] - G[m][n].tmpTime;
                            tmp = node;
                            node = node->nextJob = (MACHINEPTR) malloc(sizeof(struct machine));
                            flag = 0;
                        }
            tmp->nextJob = NULL;
            free(node);
        }
    }

    return makespan;
}
