#define CINT_SCALE 30

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
    cint a, b, c, one;
    cint_init(&one, 1);
    for (int i = 0; i < (1 << 30); ++i) {
        if (rand() & 1) cint_rand(&a, 2 + rand() % 8); else cint_init(&a, rand());
        b = a;
        if (rand() & 1) cint_rand(&c, 2 + rand() % 8); else cint_init(&c, rand());
        cint_addi(&c, &one);
        cint_muli(&a, &c);
        cint_divi(&a, &c);
        assert(memcmp(&a, &b, sizeof(cint)) == 0);
    }
    printf("took %gs.", microtime() - begin);
}

/*
    C:\Users\Famille\CLionProjects\crypto\cmake-build-debug\crypto.exe
    srand at 1650524974 : took 198.314s.
    srand at 1650524974 : took 198.314s.
    srand at 1650583890 : took 3158.11s.
    Process finished with exit code 0
*/
