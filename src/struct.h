#ifndef JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H
#define JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H

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

typedef struct overhaul {
    int machine;
    int time;
    int timeline;
    struct overhaul *nextOverhaul;
} *OVERHAULPTR;

int jobNum, machineNum, minMakespan;
JOBPTR *job;
OVERHAULPTR overhaul;

#endif //JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H
