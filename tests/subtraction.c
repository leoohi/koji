#define CINT_SCALE 50

#include "cint.c"

#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

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
    for (int i = 0; i < (1 << 20); ++i) {
        cint a, b, c, d;
        // Fibonacci like sequences with random initialization.
        if (rand() & 1)  cint_rand(&a, 1 + rand() % 3);
        else cint_init(&a, rand());
        if (rand() & 1) cint_rand(&b, 1 + rand() % 3);
        else cint_init(&b, rand());
        for (int j = 0; j < (1 << 10); ++j) {
            c = a;
            cint_addi(&a, &b);
            cint_sub(&a, &c, &d);
            // Fibonacci classic assertion.
            assert(memcmp(&d, &b, sizeof(cint)) == 0);
            b = c;
        }
    }
    printf("completed in %gs.", microtime() - begin);
}

/*
    C:\Users\Famille\CLionProjects\crypto\cmake-build-debug\crypto.exe
    srand at 1650516895 : completed in 183.415s.
    srand at 1650660867 : completed in 183.807s.
    Process finished with exit code 0
*/
