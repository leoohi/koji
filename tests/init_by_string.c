#define CINT_SCALE 10

#include "cint.c"
#include "stdio.h"
#include "assert.h"

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
    unsigned long long int max = cint_base, expected, digit;
    max = (max * max + max) >> 4;
    char str[50] = "";
    cint num;
    int i, j, k, any_base;
    for (i = 0; i < (1 << 30); ++i) {
        expected = 0;
        any_base = 2 + rand() % 61;
        j = rand() % 50;
        for (k = 0; k < j; ++k) {
            digit = rand() % any_base;
            if ((expected * any_base + digit) > max)
                break;
            expected *= any_base;
            expected += digit;
            str[k] = cint_alpha[digit];
        }
        str[k] = 0;
        cint_init_by_string(&num, str, any_base);
        unsigned long long int cint_data = num.data[CINT_SCALE - 1] + num.data[CINT_SCALE - 2] * cint_base;
        assert(expected == cint_data);
        if (cint_data >= cint_base) assert(num.index == CINT_SCALE - 2);
        else if (cint_data) assert(num.index == CINT_SCALE - 1);
        else
            assert(num.index == CINT_SCALE);
    }
    printf("took %gs.", microtime() - begin);
}

/*
    C:\Users\Famille\CLionProjects\crypto\cmake-build-debug-coverage\crypto.exe
    srand at 1650571578 : took 441.414s.
    srand at 1650581427 : took 510.123s. // CINT_SCALE at 250
    srand at 1650581998 : took 441.403s.
    srand at 1650659692 : took 447.651s.
    Process finished with exit code 0
*/
