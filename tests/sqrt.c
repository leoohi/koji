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

static inline void test_sqrt(const cint *number){
    static cint one = {0};
    if (one.index == 0)
        cint_init(&one, 1);
    cint sqrt, sqrt_plus_one ;
    cint_sqrt(number, &sqrt);
    cint_add(&sqrt, &one, &sqrt_plus_one);
    cint_muli(&sqrt, &sqrt);
    cint_muli(&sqrt_plus_one, &sqrt_plus_one);
    int cmp_1 = cint_compare(&sqrt, number);
    int cmp_2 = cint_compare(&sqrt_plus_one, number);
    assert(cmp_2 > 0);
    if (cmp_1 == 0 && putchar('+'))
        assert(memcmp(number, &sqrt, sizeof(cint)) == 0);
    else
        assert(cmp_1 < 0);
}

int main(void) {
    unsigned sr = time(0) ;
    srand(sr);
    printf("srand at %u\n(", sr);
    double a = microtime();
    cint number ;
    for (size_t i = 0, count = 1 << 25; i < count; ++i) {
        (i | count / 16 - 1) == i ? printf(") %llu%%(", i * 100 / count) , fflush(stdout) : 0;
        cint_rand(&number, 1 + rand() % 8);
        test_sqrt(&number);
    }
    printf(")\ntook %gs\n", microtime() - a);
}

/*
C:\Users\Famille\CLionProjects\crypto\cmake-build-debug\crypto.exe
srand at 1650422246
(+++++) 6%(++++) 12%(+++++) 18%(++++) 24%(+++++) 31%(+++++) 37%(++++) 43%(+++++) 49%(++++)
56%(+++++) 62%(+++++) 68%(++++) 74%(+++++) 81%(+++++) 87%(++++) 93%(+++++) 99%()
took 182s
Process finished with exit code 0
*/
