//author dachr
#include "jobshop.h"

#define LIMIT 300000

extern void *getJob();

extern void getOverhaul();

extern int schedule();

extern void output(int makespan);

extern void freeAll();

int main(void) {
    while (clock() < LIMIT) {
        overhaul = NULL;

        freopen("la01.txt", "r", stdin);//
        getJob();

        puts("Please wait for the output result...");
        output(schedule());

        getOverhaul();//
        fclose(stdin);

        puts("Please wait for the output result...");
        output(schedule());

        freeAll();

        return 0;
    }
}
