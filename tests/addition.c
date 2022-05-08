#define CINT_SCALE 10

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
        cint first, step, sum, last, expected;
        if (rand() & 1) cint_rand(&first, 1 + rand() % 8); else cint_init(&first, rand());
        if (rand() & 1) cint_rand(&step, 1 + rand() % 8); else cint_init(&step, rand());
        cint_init(&sum, 0);
        last = first;
        int iterations = 4 + rand();
        iterations += iterations & 1;
        for (int j = 1; j <= iterations; ++j) {
            cint_addi(&sum, &last);
            if (j < iterations)
                cint_addi(&last, &step);
        }
        expected = last;
        cint_addi(&expected, &first);
        cint_nat_muli(&expected, iterations);
        cint_down2i(&expected); // divide by two
        // assert that for any random parameters : (first_term + last_term) * n_iterations / 2 = sum_all_terms
        // it's like : 1 + 3 + 5 + 7 + 9 + 11 = (11 + 1) * 6 / 2
        assert(memcmp(&expected, &sum, sizeof(cint)) == 0);
    }
    printf("took %gs.", microtime() - begin);
}

/*
    C:\Users\Famille\CLionProjects\crypto\cmake-build-debug\crypto.exe
    srand at 1650521910 : took 323.095s.
    srand at 1650580389 : took 331.484s.
    srand at 1650657663 : took 319.216s.
    Process finished with exit code 0
*/
