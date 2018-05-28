#ifndef JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H
#define JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H

#include <stdio.h>

#define SIZE (4*jobNum*machineNum)

typedef struct job {
    int machine, time;
    struct job *nextMachine;
} *JOBPTR;

typedef struct machine {
    int job, order, startTime, endTime;
    struct machine *nextJob;
} *MACHINEPTR;

typedef struct overhaul {
    int startTime, endTime;
    struct overhaul *nextOverhaul;
} *OVERHAULPTR;

int jobNum, machineNum;
JOBPTR *job;
MACHINEPTR *machine;
OVERHAULPTR *overhaul;

#endif //JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H
