#define CINT_SCALE 3850
#include "cint.c"

#include <stdio.h>
int cint_print(const cint *number) {
    char *str = cint_to_string(number, 10);
    int res = printf("%s", str);
    free(str);
    return res;
}

// Compute and print the Factorial 10000 number (35660 digits)
int main() {
    cint number_1 ;
    cint_init(&number_1, 1);
    for (int i = 2; i <= 10000; ++i)
        cint_nat_muli(&number_1, i);
    cint_print(&number_1);
}

// You can put it into a factorial.c file then compile + execute :
// gcc -O3 -std=c99 -Wall -pedantic factorial.c ; ./a.out ;
