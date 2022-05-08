#define CINT_SCALE 50

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
    cint a, b;
    for(int i = 0; i < (1 << 20); ++i) {
        int size = rand() % 50, base = 2 + rand() % 61 ;
        cint_rand(&a, size);
        char *str = cint_to_string(&a, base);
        cint_init_by_string(&b, str, base);
        assert(memcmp(&a, &b, sizeof(cint)) == 0);
        free(str);
    }
    printf("took %gs.", microtime() - begin);
}

/*
  C:\Users\Famille\CLionProjects\crypto\cmake-build-debug-coverage\crypto.exe
  srand at 1650571453 : took 73.18s.
  Process finished with exit code 0
*/
