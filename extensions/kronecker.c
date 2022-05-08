//  This function returns the Kronecker symbol of A and B.
int cint_kronecker(const cint *lhs, const cint *rhs) {
    static const int s[8] = {0, 1, 0, -1, 0, -1, 0, 1};
    cint a, b, c, d, *e, *f, *g, *h;
    size_t i;
    int j;
    if (lhs->index == CINT_SCALE)
        j = rhs->index == CINT_SCALE - 1 && rhs->data[rhs->index] == 1;
    else if (rhs->index == CINT_SCALE)
        j = lhs->index == CINT_SCALE - 1 && lhs->data[lhs->index] == 1;
    else if ((lhs->data[CINT_SCALE - 1] | rhs->data[CINT_SCALE - 1]) & 1) {
        a = *lhs, b = *rhs, e = &a, f = &b, h = &d;
        for (i = 0; !(f->data[CINT_SCALE - 1] & 1); ++i, cint_down2i(f));
        // When B is odd Jacobi and Kronecker symbols are identical, in factorization algorithms B is often the prime.
        // When B is an odd prime, Jacobi symbol is equal to the Legendre symbol.
        for (j = i & 1 ? s[e->data[CINT_SCALE - 1] & 7] : 1; e->index < CINT_SCALE;) {
            for (i = 0; !(e->data[CINT_SCALE - 1] & 1); ++i, cint_down2i(e));
            i & 1 ? j *= s[f->data[CINT_SCALE - 1] & 7] : 0;
            e->data[CINT_SCALE - 1] & f->data[CINT_SCALE - 1] & 2 ? j = -j : 0;
            cint_div(f, e, &c, h), g = f, f = e, e = h, h = g;
        }
        j *= f->index == CINT_SCALE - 1 && f->data[CINT_SCALE - 1] == 1;
    } else j = 0;
    return j;
}
