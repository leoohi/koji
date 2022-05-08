#define CINT_SCALE 50

#include "cint.c"
#include <stdio.h>

int cint_print(const cint *number) {
    char *str = cint_to_string(number, 10);
    int res = printf("%s", str);
    free(str);
    return res;
}

#include <sys/time.h>
#include <assert.h>

void sqrt_test(const int test_scale) {
    unsigned sr = time(0);
    srand(sr);
    printf("Begin SQRT test scale %d using '<assert.h>' and srand %u :\n", test_scale, sr);
    cint sqrt_A, sqrt_B, sqrt_AB, product, raiser, raiser_rooted, abs_err;
    const int modulo = 1 << 18, steps = 1 << 18 ;
    cint_init(&raiser, 10);
    cint_powi(&raiser, test_scale << 1);
    cint_sqrt(&raiser, &raiser_rooted);

    for(int i = 0; i < steps; ++i) {

        const long long int a = 1 + (rand() + (rand() << 16)) % modulo ;
        const long long int b = 1 + (rand() + (rand() << 16)) % modulo ;
        const long long int c = a * b ;

        // sqrt(A)
        cint_init(&sqrt_A, a);
        cint_muli(&sqrt_A, &raiser);
        cint_sqrti(&sqrt_A);

        // sqrt(B)
        cint_init(&sqrt_B, b);
        cint_muli(&sqrt_B, &raiser);
        cint_sqrti(&sqrt_B);

        // sqrt(A * B)
        cint_init(&sqrt_AB, c);
        cint_muli(&sqrt_AB, &raiser);
        cint_sqrti(&sqrt_AB);

        // sqrt(A) * sqrt(B)
        cint_mul(&sqrt_A, &sqrt_B, &product);
        cint_divi(&product, &raiser_rooted);

        const int cmp_1 = cint_compare(&sqrt_AB, &product);

        if (cmp_1 == 0) {
            // exact result
            assert(memcmp(&sqrt_AB, &product, sizeof(cint)) == 0);
            printf("%2d%% a = %-10lld b = %-10lld1e%d * sqrt(%lld) - ", 100 * i / steps, a, b, test_scale, c);
            cint_print(&sqrt_AB);
            puts(" == 0");
            fflush(stdout);
        }
        else
        {
            assert(cmp_1 > 0);
            // sqrt(A*B) > sqrt(A) * sqrt(B)
            // bound absolute error
            cint_init(&abs_err, (long long int) ceil(sqrt((double)a) * sqrt((double)b)));
            cint_subi(&sqrt_AB, &product);
            const int cmp_2 = cint_compare(&abs_err, &sqrt_AB) ;
            assert(cmp_2 >= 0);
            cmp_2 || putchar('+');
        }
    }
    putchar('\n');
}

int main(){
    for(int i = 20; i < 50; ++i)
        sqrt_test(i);
}

// Process finished with exit code 0

// you can do verifications using https://3v4l.org
// the program is <?php echo str_replace('.', '', bcsqrt('123456789', 50));
// it should output the same sequence of digits.
