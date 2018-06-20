//author dachr/zhangzhibo
#include "jobshop.h"

extern void *getJob();

extern int schedule();

extern void output(int makespan);

extern void freeAll();

int main(void) {
    while (clock() < LIMIT) {
        getJob();

        output(schedule());

        freeAll();

        return 0;
    }
}
