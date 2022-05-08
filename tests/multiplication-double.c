#define CINT_SCALE 50

// Using a little-endian 64-bit architecture.
// This test has no value, it's only a showing.

#include "cint.c"
#include <stdio.h>
#include <float.h>
#include <assert.h>
#include <sys/time.h>

double custom_nextafter(double d, long long int step) {
    long long int tmp;
    memcpy(&tmp, &d, sizeof(d));
    tmp += d > 0 ? step : -step;
    memcpy(&d, &tmp, sizeof(d));
    return d;
}

int main() {
    unsigned sr = time(0);
    srand(sr);
    printf("srand at %u\n", sr);
    long long int step = 1, curr_exponent, mantissa_subtraction;
    cint whole_mantissa, cint_step, one, expected, power_a, power_b, computation;
    double base_dbl, curr_dbl;
    cint_init(&one, 1);
    cint_init(&whole_mantissa, 2);
    cint_powi(&whole_mantissa, 53);
    cint_subi(&whole_mantissa, &one);
    for (int i = 1; i < 16; ++i) {
        step *= i;
        cint_init(&cint_step, step);
        base_dbl = DBL_MAX;
        curr_exponent = 971;
        for (int j = 0; j < 971; ++j) {
            power_a = whole_mantissa;
            cint_init(&power_b, 2);
            cint_powi(&power_b, curr_exponent);
            curr_dbl = base_dbl;
            cint_init_by_double(&expected, curr_dbl);
            mantissa_subtraction = 1;
            for (int k = 0; k < 2048; ++k) {
                mantissa_subtraction += step;
                curr_dbl = custom_nextafter(curr_dbl, -step);
                cint_init_by_double(&expected, curr_dbl);
                computation = power_b;
                cint_subi(&power_a, &cint_step);
                cint_muli(&computation, &power_a);
                assert(memcmp(&expected, &computation, sizeof(cint)) == 0);
                if (rand() << 4 == rand()) {
                    char *str = cint_to_string(&computation, 10);
                    printf("verified that 2^%-3lld * (2^53 - %-16lld) == %s\n", curr_exponent, mantissa_subtraction, str);
                    fflush(stdout);
                    free(str);
                }
            }
            --curr_exponent;
            base_dbl /= 2;
        }
    }
}

/*
 Output about 50 lines of :
 - verified that 2^44  * (2^53 - 1059216238080001) == 139822395906958866284340576256
 - verified that 2^35  * (2^53 - 2493735019776001) == 223800926982722368399474688
*/
