#include "jobshop.h"

void swapPtr(int **a, int **b) {
    int *t = *a;
    *a = *b;
    *b = t;
}
