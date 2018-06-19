#include "jobshop.h"

int *crossover(const int *a, const int *b) {
    int *child = (int *) malloc(sizeof(int) * len), indexA[len], indexB[len];
    for (int i = 0; i < len; ++i)
        indexA[i] = indexB[i] = 0;
    for (int i = 0; i < len; ++i)
        for (int j = 0; j < i; ++j) {
            if (a[j] == a[i])
                ++indexA[i];
            if (b[j] == b[i])
                ++indexB[i];
        }

    int startA = rand() % len, endA = startA + rand() % (len - startA), posB = rand() % (len + startA - endA);
    for (int i = 0, j = 0; i < len; ++i)
        if (posB <= i && i <= posB + endA - startA)
            child[i] = a[startA + i - posB];
        else
            for (int flag = 1; flag; ++j) {
                flag = 1;
                for (int k = startA; k <= endA && flag == 1; ++k)
                    if (a[k] == b[j] && indexA[k] == indexB[j])
                        flag = 2;
                if (flag == 1) {
                    child[i] = b[j];
                    flag = 0;
                }
            }

    return child;
}
