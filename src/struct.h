#ifndef JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H
#define JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H

int jobNum, machineNum, makeSpan;
struct machine overhaul;

typedef struct job {
    int machine;
    int time;
    struct job *nextMachine;
} *JOBPTR;

typedef struct machine {
    int job;
    int time;
    int timeline;
    struct machine *nextJob;
} *MACHINEPTR;

#endif //JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H
