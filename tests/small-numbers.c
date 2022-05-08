#define CINT_SCALE 10

#include "cint.c"
#include <assert.h>

void test_addi(int lhs, int rhs, int res) {
    cint a, b, expected;
    cint_init(&a, lhs);
    cint_init(&b, rhs);
    cint_addi(&a, &b);
    cint_init(&expected, res);
    assert(memcmp(&a, &expected, sizeof(cint)) == 0);
}

void test_subi(int lhs, int rhs, int res) {
    cint a, b, expected;
    cint_init(&a, lhs);
    cint_init(&b, rhs);
    cint_subi(&a, &b);
    cint_init(&expected, res);
    assert(memcmp(&a, &expected, sizeof(cint)) == 0);
}

void test_muli(int lhs, int rhs, int res) {
    cint a, b, expected;
    cint_init(&a, lhs);
    cint_init(&b, rhs);
    cint_muli(&a, &b);
    cint_init(&expected, res);
    assert(memcmp(&a, &expected, sizeof(cint)) == 0);
}

void test_div(int lhs, int rhs, int quo, int rem) {
    cint a, b, q, r, expected_q, expected_r;
    cint_init(&a, lhs);
    cint_init(&b, rhs);
    cint_div(&a, &b, &q, &r);
    cint_init(&expected_q, quo);
    cint_init(&expected_r, rem);
    assert(memcmp(&q, &expected_q, sizeof(cint)) == 0);
    assert(memcmp(&r, &expected_r, sizeof(cint)) == 0);
}

void test_sqrti(int n, int res) {
    cint a, expected;
    cint_init(&a, n);
    cint_sqrti(&a);
    cint_init(&expected, res);
    assert(memcmp(&a, &expected, sizeof(cint)) == 0);
}

void test_up2i(int n, int res) {
    cint a, expected;
    cint_init(&a, n);
    cint_up2i(&a);
    cint_init(&expected, res);
    assert(memcmp(&a, &expected, sizeof(cint)) == 0);
}

void test_down2i(int n, int res) {
    cint a, expected;
    cint_init(&a, n);
    cint_down2i(&a);
    cint_init(&expected, res);
    assert(memcmp(&a, &expected, sizeof(cint)) == 0);
}

int main(void) {
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1000; ++j) {
            test_addi(i, j, i + j);
            test_subi(i, j, i > j ? i - j : 0);
            test_muli(i, j, i * j);
            if (j)
                test_div(i, j, i / j, i % j);
            test_sqrti(i + j, (int) sqrt(i + j));
        }
        test_up2i(i, i << 1);
        test_down2i(i, i >> 1);
    }
}
