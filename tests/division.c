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

long long int get_rand_number(){
    long long int n = 0 ;
    for(size_t i = 0; i < sizeof(n); ++i)
        ((char*)&n)[i] = rand() ;
    if (n < 0) n = -n ;
    return n ? n : 1 ;
}

int main() {
    unsigned sr = time(0);
    srand(sr);
    printf("srand at %u : ", sr);
    double begin = microtime();
    for (int i = 0; i < (1 << 30); ++i) {
        long long int a = get_rand_number();
        long long int b = get_rand_number();
        cint A, B, C, D, E, F;
        cint_init(&A, a);
        cint_init(&B, b);
        cint_div(&A, &B, &C, &D);
        cint_init(&E, a / b);
        cint_init(&F, a % b);
        assert(memcmp(&C, &E, sizeof(cint)) == 0);
        assert(memcmp(&D, &F, sizeof(cint)) == 0);
    }
    printf("took %gs\n", microtime() - begin);
}

/*
	C:\Users\Famille\CLionProjects\crypto\cmake-build-debug\crypto.exe
	srand at 1650540780 : took 535.092s
	srand at 1650580801 : took 535.296s
	srand at 1650659027 : took 613.352s
	Process finished with exit code 0
*/
