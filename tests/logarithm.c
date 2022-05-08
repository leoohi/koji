#define CINT_SCALE 40

#include "cint.c"

#include <stdio.h>
#include <sys/time.h>
#include <assert.h>

double microtime() {
    struct timeval time;
    gettimeofday(&time, 0);
    return (double) time.tv_sec + (double) time.tv_usec / 1e6;
}

double get_rand_double() {
    double res = 0;
    do {
        for (size_t i = 0; i < sizeof(double); ++i)
            ((char *) &res)[i] = rand();
        if (res < 0)
            res = -res;
    } while (res < 1);
    modf(res, &res);
    return res;
}

int main() {
    unsigned sr = time(0);
    srand(sr);
    printf("srand at %u :\n", sr);
    cint number;
    double cint_above = 0, above_input = 0, cint_below = 0, below_input = 0, any, log_c, log_cint, begin = microtime();
    for (int i = 0; i < (1 << 30); ++i) {
        any = get_rand_double();
        log_c = log2(any);
        // cint_init_by_double is designed for testing and pedagogic
        // purpose only, it's expecting a little-endian architecture.
        cint_init_by_double(&number, any);
        log_cint = cint_log2(&number);
        if (log_cint > log_c) {
            cint_above = log_cint - log_c;
            above_input = any;
        } else if (log_cint < log_c) {
            cint_below = log_c - log_cint;
            below_input = any;
        }
    }
    printf("when cint answers above C : input was %g - cint was greater by %.18f (%g).\n", above_input, cint_above, cint_above);
    printf("when cint answers below C : input was %g - cint was lower by %.18f (%g).\n", below_input, cint_below, cint_below);
    printf("took %gs\n", microtime() - begin);
}

/*
    C:\Users\Famille\CLionProjects\crypto\cmake-build-debug\crypto.exe
    srand at 1650582553 :
    when cint answers above C : input was 9.96178e+033 - cint was greater by 0.000000000000014211 (1.42109e-014).
    when cint answers below C : input was 2.47318e+086 - cint was lower by 0.000000000001705303 (1.7053e-012).
    took 371.196s
    Process finished with exit code 0
*/
