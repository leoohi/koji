#include "cint.c"

#include <stdio.h>
int cint_print(const cint *number) {
    char *str = cint_to_string(number, 10);
    int res = printf("%s", str);
    free(str);
    return res;
}

int main(void){
    cint number ;
    // initialize a base 62 number
    cint_init_by_string(&number, "G4LIM9VYhB0", 62);
    cint_print(&number);
    // 13487587989675406230
}

// You can put it into a cint.c file then compile + execute :
// gcc -O3 -std=c99 -Wall -pedantic cint.c ; ./a.out ;
