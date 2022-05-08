#ifndef CINT_C
#define CINT_C

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef CINT_SCALE
#define CINT_SCALE 250
#endif

static const size_t cint_exponent = 31;
static const long long int cint_base = 1LL << cint_exponent;
static const char *cint_alpha = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

typedef struct {
    long long int data[CINT_SCALE];
    size_t index;
} cint;

static inline void cint_init(cint *num, long long int value) {
    memset(num, 0, sizeof(cint));
    for (num->index = CINT_SCALE; value; num->data[--num->index] = value % cint_base, value /= cint_base);
}

int cint_compare(const cint *lhs, const cint *rhs) {
    int res = (int) (rhs->index - lhs->index);
    if (res == 0 && lhs->index != CINT_SCALE)
        for (size_t i = rhs->index; !(res = (int) (lhs->data[i] - rhs->data[i])) && ++i < CINT_SCALE;);
    return res;
}

void cint_addi(cint *lhs, const cint *rhs) {
    long long int a = 0;
    size_t i = CINT_SCALE;
    for (; --i >= rhs->index;)
        (a = (lhs->data[i] += rhs->data[i] + a) > cint_base - 1) ? (lhs->data[i] -= cint_base) : 0;
    for (; a; lhs->data[i] += a, (a = lhs->data[i] > cint_base - 1) ? lhs->data[i] -= cint_base : 0, --i);
    if (1 + i < lhs->index) lhs->index = 1 + i;
}

static inline void cint_add(const cint *lhs, const cint *rhs, cint *res) { *res = *lhs; cint_addi(res, rhs); }

void cint_subi(cint *lhs, const cint *rhs) {
    if (lhs->index < CINT_SCALE && rhs->index < CINT_SCALE) {
        if (lhs->index < rhs->index || lhs->data[lhs->index] >= rhs->data[rhs->index]) {
            long long int a = 0;
            size_t i = CINT_SCALE;
            for (; --i >= rhs->index;)
                (a = (lhs->data[i] -= rhs->data[i] + a) < 0) ? (lhs->data[i] += cint_base) : 0;
            for (; a; lhs->data[i] -= a, (a = lhs->data[i] < 0) ? lhs->data[i] += cint_base : 0, --i);
            for (; lhs->index < CINT_SCALE && !lhs->data[lhs->index]; ++lhs->index);
        } else cint_init(lhs, 0);
    }
}

static inline void cint_sub(const cint *lhs, const cint *rhs, cint *res) { *res = *lhs; cint_subi(res, rhs); }

void cint_up2i(cint *number) {
    if (number->index == CINT_SCALE) return;
    for (size_t i = number->index; i < CINT_SCALE; ++i) {
        number->data[i - 1] |= number->data[i] >> (cint_exponent - 1);
        number->data[i] = (number->data[i] << 1) & ~cint_base;
    }
    number->index -= number->data[number->index - 1] != 0 ;
}

static inline void cint_up2(const cint *number, cint *res) { *res = *number; cint_up2i(res); }

void cint_down2i(cint *number) {
    if (number->index == CINT_SCALE) return;
    number->data[CINT_SCALE - 1] >>= 1;
    for (size_t i = CINT_SCALE - 2; i >= number->index; --i) {
        number->data[i + 1] |= (number->data[i] & 1) << (cint_exponent - 1);
        number->data[i] >>= 1;
    }
    number->index += number->data[number->index] == 0 ;
}

static inline void cint_down2(const cint *number, cint *res) { *res = *number; cint_down2i(res); }

void cint_mul(const cint *lhs, const cint *rhs, cint *res) {
    memset(res, 0, sizeof(cint));
    if (lhs->index == CINT_SCALE || rhs->index == CINT_SCALE)
        res->index = CINT_SCALE;
    else {
        res->index = 1 + lhs->index + rhs->index - CINT_SCALE;
        size_t i, j, k = res->index, l;
        for (i = rhs->index; i < CINT_SCALE; ++i, ++k) {
            for (l = k, j = lhs->index; j < CINT_SCALE; ++l, ++j)
                res->data[l] += lhs->data[j] * rhs->data[i];
            for (j = CINT_SCALE; --j >= res->index;) {
                res->data[j - 1] += res->data[j] / cint_base;
                res->data[j] %= cint_base;
            }
        }
        res->index -= res->data[res->index - 1] != 0;
    }
}

static inline void cint_muli(cint *lhs, const cint *rhs) { cint tmp; cint_mul(lhs, rhs, &tmp); *lhs = tmp; }

void cint_nat_muli(cint *number, int factor) {
    long long int a, b = 0;
    if (factor < 1 || number->index == CINT_SCALE) cint_init(number, 0);
    else if(factor == 2) cint_up2i(number);
    else if(factor != 1) {
        for (size_t i = CINT_SCALE;  --i >= number->index; b = (a = factor * number->data[i] + b) / cint_base, number->data[i] = a % cint_base);
        for (; b; number->data[--number->index] = b % cint_base, b /= cint_base);
    }
}

static inline void cint_nat_mul(const cint *number, int factor, cint * res) { *res = *number ; cint_nat_muli(res, factor);}

void cint_div(const cint *lhs, const cint *rhs, cint *q, cint *r) {
    cint_init(r, 0);
    if (rhs->index == CINT_SCALE)
        for (size_t i = q->index = 0; i < CINT_SCALE; q->data[i++] = cint_base - 1); // DBZ
    else {
        long long int a = cint_compare(lhs, rhs);
        cint_init(q, 0);
        if (a) {
            if (a > 0) {
                for (size_t i = lhs->index; i < CINT_SCALE; ++i) {
                    for (a = cint_base; a >>= 1;) {
                        cint_up2i(r);
                        r->data[CINT_SCALE - 1] = a & lhs->data[i] ? r->data[CINT_SCALE - 1] | 1 : r->data[CINT_SCALE - 1] & ~1;
                        r->index -= r->data[r->index - 1] != 0;
                        if (cint_compare(r, rhs) >= 0) {
                            cint_subi(r, rhs);
                            q->data[i] |= a;
                        }
                    }
                }
                q->index = lhs->index + CINT_SCALE - rhs->index - 1 ;
                q->index += q->data[q->index] == 0;
            } else *r = *lhs ;
        } else q->data[q->index = CINT_SCALE - 1] = 1;
    }
}

static inline void cint_divi(cint *lhs, const cint *rhs) { cint q, r; cint_div(lhs, rhs, &q, &r); *lhs = q; }

static inline void cint_modi(cint *lhs, const cint *rhs) { cint q, r; cint_div(lhs, rhs, &q, &r); *lhs = r; }

void cint_sqrt(const cint *number, cint *root) {
    static long long int factor = 0;
    if (factor == 0) factor = 1 + ((long long int) floor(sqrt((double) cint_base)));
    if (CINT_SCALE == number->index)
        cint_init(root, 0);
    else {
        cint approx = {0}, r;
        approx.index = (CINT_SCALE + number->index) >> 1;
        approx.data[approx.index] = 1 + (long long int) floor(sqrt((double) number->data[number->index]));
        if (((number->index + CINT_SCALE) & 1) == 0 && (approx.data[approx.index] *= factor) >= cint_base)
            approx.data[approx.index] = cint_base - 1;
        cint_div(number, &approx, root, &r);
        cint_addi(root, &approx);
        cint_down2i(root);
        if (root->index != approx.index || approx.index != CINT_SCALE - 1 || approx.data[approx.index] - root->data[root->index] >= 2)
            for (; cint_compare(root, &approx) < 0; approx = *root, cint_div(number, &approx, root, &r), cint_addi(root, &approx), cint_down2i(root));
    }
}

static inline void cint_sqrti(cint *lhs) { cint root; cint_sqrt(lhs, &root); *lhs = root; }

void cint_powi(cint *number, size_t power) {
    if (power == 0)
        cint_init(number, 1);
    else if (number->index < CINT_SCALE) {
        cint tmp;
        cint_init(&tmp, 1);
        for (; power > 1; cint_muli(number, number))
            if (power & 1) {
                cint_muli(&tmp, number);
                power = (power - 1) >> 1;
            } else
                power >>= 1;
        cint_muli(number, &tmp);
    }
}

static inline void cint_pow(const cint *number, size_t power, cint *res) { *res = *number; cint_powi(res, power); }

void cint_rand(cint *number, size_t length) {
    memset(number, 0, sizeof(cint));
    number->index = CINT_SCALE - length;
    for (size_t j, k = 0, i = number->index; i < CINT_SCALE; number->data[i++] = (long long int) (k % cint_base))
        for (j = 0; j < sizeof(size_t); ((char *) &k)[j++] = rand());
    for (; number->index < CINT_SCALE && !number->data[number->index]; ++number->index);
}

double cint_log2(const cint * num){
    return num->index == CINT_SCALE ? -INFINITY :
    num->index == CINT_SCALE - 1 ? log2((double) num->data[num->index]) :
    (double) (cint_exponent * (CINT_SCALE - num->index - 2)) + log2((double) (num->data[num->index] * cint_base + num->data[num->index + 1]));
}

static inline double cint_logx(const cint * num, const int base){ return cint_log2(num) / log2(base); }

static char *cint_to_string(const cint *num, const int base_out) {
    size_t a, b, c = 1, d;
    char *s = malloc(2);
    strcpy(s, "0");
    for (size_t i = num->index; i < CINT_SCALE; ++i) {
        for (a = num->data[i], b = c; b;) {
            d = ((char *) memchr(cint_alpha, s[--b], base_out) - cint_alpha) * cint_base + a;
            s[b] = cint_alpha[d % base_out];
            a = d / base_out;
        }
        for (; a; s = realloc(s, ++c + 1), memmove(s + 1, s, c), *s = cint_alpha[a % base_out], a /= base_out);
    } // a magic but "slow" base converter.
    return s;
}

void cint_init_by_string(cint *number, const char *str, const int base) {
    cint_init(number, 0);
    for (; *str; ++str) {
        cint_nat_muli(number, base);
        number->data[CINT_SCALE - 1] += (int) ((char *) memchr(cint_alpha, *str, base) - cint_alpha);
        for (size_t i = CINT_SCALE - 1; number->data[i] >= cint_base; number->data[i] -= cint_base, ++number->data[--i]);
        number->index -= number->data[number->index - 1] != 0;
    }
}

// Designed for pedagogic and testing purpose, using a little-endian architecture.
void cint_init_by_double(cint *number, const double value) {
    uint64_t memory;
    memset(number, 0, sizeof(cint));
    memcpy(&memory, &value, sizeof(value));
    uint64_t ex = (memory << 1 >> 53) - 1023, mantissa_mask = 1ULL << 52;
    if (ex < 1024 && sizeof(double) == 8) {
        long long int cint_mask = 1 << ex % cint_exponent;
        number->index = CINT_SCALE - ex / cint_exponent - 1;
        number->data[number->index] |= cint_mask;
        for (size_t i = number->index; i < CINT_SCALE; ++i, cint_mask = 1LL << cint_exponent)
            for (; cint_mask >>= 1;)
                if (mantissa_mask >>= 1)
                    number->data[i] |= memory & mantissa_mask ? cint_mask : 0;
                else return;
    } else number->index = CINT_SCALE;
}

#endif //CINT_C
