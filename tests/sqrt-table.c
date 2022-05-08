#define CINT_SCALE 100

#include "cint.c"
#include <stdio.h>

int main(void) {
    cint n, raiser, sqrt;
    cint_init(&raiser, 10);
    cint_powi(&raiser, 200);
    for (int i = 2; i < 100; ++i)
    {
        cint_init(&n, i);
        cint_muli(&n, &raiser);
        cint_sqrt(&n, &sqrt);
        char * str = cint_to_string(&sqrt, 10), first;
        first = *str ; *str = '.';
        printf("sqrt(%2d) = %c%s\n", i, first, str);
        free(str);
    }
}

/*

sqrt( 2) = 1.4142135623...
sqrt( 3) = 1.7320508075...
sqrt( 4) = 2.0000000000...
sqrt( 5) = 2.2360679774...
...
sqrt(99) = 9.9498743710...

*/
