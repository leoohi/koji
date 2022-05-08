#define CINT_SCALE 2250
#include "cint.c"

#include <stdio.h>
int cint_print(const cint *number) {
    char *str = cint_to_string(number, 10);
    int res = printf("%s", str);
    free(str);
    return res;
}

// Compute and print the 100000th Fibonacci number (20899 digits)
int main() {
    cint number_1, number_2, number_3;
    cint_init(&number_1, 0);
    cint_init(&number_2, 1);
    for (int i = 0; i < 100000; ++i) {
        number_3 = number_1;
        cint_addi(&number_1, &number_2);
        number_2 = number_3;
    }
    cint_print(&number_1);
}

// You can put it into a fibonacci.c file then compile + execute :
// gcc -O3 -std=c99 -Wall -pedantic fibonacci.c ; ./a.out ;
