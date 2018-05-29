#ifndef JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H
#define JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H

#include <stdio.h>
#include <time.h>

#define SIZE (4 * jobNum + 4 * machineNum + 40)

#define DOOM (jobNum + 5)

#define LIMIT 300000

typedef struct job {
    int machine, time;
    struct job *nextMachine;
} *JOBPTR;

typedef struct machine {
    int job, order, startTime, endTime;
    struct machine *nextJob;
} *MACHINEPTR;
/*
typedef struct overhaul {
    int startTime, endTime;
    struct overhaul *nextOverhaul;
} *OVERHAULPTR;
*/
int jobNum, machineNum;
JOBPTR *job;
MACHINEPTR *machine;
//OVERHAULPTR *overhaul;

#endif //JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H
