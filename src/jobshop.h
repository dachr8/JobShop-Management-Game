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
    int totalTime;
    struct machine *nextJob;
} *MACHINEPTR;

#endif //JOBSHOP_MANAGEMENT_GAME_JOBSHOP_H
