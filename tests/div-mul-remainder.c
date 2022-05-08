#define CINT_SCALE 50

#include "cint.c"

#include <stdio.h>
#include <sys/time.h>
#include <assert.h>

double microtime() {
    struct timeval time;
    gettimeofday(&time, 0);
    return (double) time.tv_sec + (double) time.tv_usec / 1e6;
}

int main() {
    unsigned sr = time(0);
    srand(sr);
    printf("srand at %u : ", sr);
    double begin = microtime();
    cint a, b, *largest_rand, *lowest_rand, quotient, remainder;
    for (int i = 0; i < (1 << 27); ++i) {
        cint_rand(&a, 1 + rand() % 40);
        cint_rand(&b, 1 + rand() % 40);
        if (cint_compare(&a, &b) >= 0)
            largest_rand = &a, lowest_rand = &b;
        else
            largest_rand = &b, lowest_rand = &a;
        if (lowest_rand->index == CINT_SCALE)
            continue; // avoid division by zero
        cint_div(largest_rand, lowest_rand, &quotient, &remainder);
        assert(cint_compare(lowest_rand, &remainder) >= 0);
        cint_muli(&quotient, lowest_rand);
        cint_addi(&quotient, &remainder);
        assert(memcmp(&quotient, largest_rand, sizeof(cint)) == 0);
    }
    printf("took %gs\n", microtime() - begin);
}

/*
    C:\Users\Famille\CLionProjects\crypto\cmake-build-debug-coverage\crypto.exe
    srand at 1650665818 : took 3576.35s
    srand at 1650675566 : took 3585.44s
    srand at 1650694457 : took 3590.1s
    Process finished with exit code 0
*/
