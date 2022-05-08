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

int get_rand_number(){
    int n = 0 ;
    for(size_t i = 0; i < sizeof(n); ++i)
        ((char*)&n)[i] = rand() ;
    if (n < 0) n = -n ;
    return n ;
}

int main() {
    unsigned sr = time(0);
    srand(sr);
    printf("srand at %u : ", sr);
    double begin = microtime();
    for (int i = 0; i < (1 << 30); ++i) {
        long long int a = get_rand_number();
        long long int b = get_rand_number();
        cint A, B, C, D;
        cint_init(&A, a);
        cint_init(&B, b);
        cint_mul(&A, &B, &C);
        cint_init(&D, a * b);
        assert(memcmp(&C, &D, sizeof(cint)) == 0);
    }
    printf("took %gs\n", microtime() - begin);
}

/*
    C:\Users\Famille\CLionProjects\crypto\cmake-build-debug\crypto.exe
    srand at 1650541330 : took 153.561s
    srand at 1650605351 : took 156.844s
    srand at 1650660395 : took 161.344s
    Process finished with exit code 0
*/
