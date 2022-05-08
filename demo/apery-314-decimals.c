#define CINT_SCALE 400

#include "cint.c"
#include "stdio.h"

void factorial(cint * res, int n){
    // no memoization here.
    cint_init(res, 1);
    for(int i = 2; i <= n; ++i)
        cint_nat_muli(res, i);
}

void combination(cint * res, int n, int k){
    cint a, b ;
    factorial(res, n);
    factorial(&a, k);
    factorial(&b, n - k);
    cint_muli(&a, &b);
    cint_divi(res, &a);
}

int main() {
    cint raiser, a, b, c, sum ;
    cint_init(&sum, 0);
    cint_init(&raiser, 10);
    cint_powi(&raiser, 1024);
    for(int i = 1; i <= 512; ++i)
    {
        c = raiser ;
        long long int j = i * i * i ;
        cint_init(&b, j);
        combination(&a, i + i, i);
        cint_muli(&a, &b);
        cint_divi(&c, &a);
        if (i & 1)
            cint_addi(&sum, &c);
        else
            cint_subi(&sum, &c);
    }
    cint_nat_muli(&sum, 5);
    cint_down2i(&sum);
    char *str = cint_to_string(&sum, 10);
    str[315] = 0;
    printf("zeta 3 = %c.%s", *str, str + 1);
    free(str);
}
/*
    Output zeta 3 = 1.202056903159594285399738161511449990764986292340498881792
    271555341838205786313090186455873609335258146199157795260719418491995998673
    283213776396837207900161453941782949360066719191575522242494243961563909664
    103291159095780965514651279918405105715255988015437109781102039827532566787
    60352233698494166181105701471577
*/
