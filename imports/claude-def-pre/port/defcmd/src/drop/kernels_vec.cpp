// Drop solver kernels (packet p67-3): vector helpers, node velocities, 6x6 solve.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.4, §5.10, §5.15. FP order from the listing
// (out/p67/drop-solver/L_all.txt). The binary keeps its loop counters in statics
// (0x140705900.., 0x1407056bc, 0x1407056e8..); they are written before they are read in every
// call, so they are locals here.
#include <defcmd/drop/kernels.hpp>

#include <cmath>
#include <cstdint>

namespace def::cmd::drop {

// @src 140367f0b  *out = 0; for k: *out = (a_k*b_k) + *out (the sum lives in *out, as in the listing)
void fun_140367f0b(double* a, double* b, double* out, i32* n)
{
    *out = 0.0;
    for (i32 k = 1; k <= *n; ++k)
        *out = (a[k - 1] * b[k - 1]) + *out;
}

// @src 140366881  c = a x b; loads and stores in the listing's order (c(1) is stored before
// b(2), b(1), a(1) are read again, which matters only when c aliases a or b)
void fun_140366881(double* a, double* b, double* c)
{
    const double b3 = b[2];
    const double a2 = a[1];
    const double b2 = b[1];
    const double a3 = a[2];
    const double t5 = a2 * b3;
    const double t7 = b2 * a3;
    const double b1 = b[0];
    const double a1 = a[0];
    c[0] = t5 - t7;
    const double t1 = a1 * b[1];  // 1403668b3
    const double t0 = a3 * b[0];  // 1403668bd
    const double t2 = b3 * a[0];  // 1403668c1
    const double t3 = b1 * a2;    // 1403668c5
    c[2] = t1 - t3;
    c[1] = t0 - t2;
}

// @src 140367df1  y(i) = ((0.0 + A(i,1)x(1)) + A(i,2)x(2)) + ..., A n x n column-major
void fun_140367df1(double* A, double* x, double* y, i32* n)
{
    const i32 nn = *n;
    const std::int64_t ld = nn > 0 ? nn : 0;
    for (i32 i = 1; i <= nn; ++i) {
        double s = 0.0;
        y[i - 1] = s;
        for (i32 k = 1; k <= nn; ++k) {
            s = s + (A[(k - 1) * ld + (i - 1)] * x[k - 1]);
            y[i - 1] = s;
        }
    }
}

// @src 140367c86  C(i,j) = ((0.0 + A(i,1)B(1,j)) + A(i,2)B(2,j)) + ..., loops i (outer), j, k
void fun_140367c86(double* A, double* B, double* C, i32* n)
{
    const i32 nn = *n;
    const std::int64_t ld = nn > 0 ? nn : 0;
    for (i32 i = 1; i <= nn; ++i) {
        for (i32 j = 1; j <= nn; ++j) {
            double s = 0.0;
            C[(j - 1) * ld + (i - 1)] = s;
            for (i32 k = 1; k <= nn; ++k) {
                s = s + (A[(k - 1) * ld + (i - 1)] * B[(j - 1) * ld + (k - 1)]);
                C[(j - 1) * ld + (i - 1)] = s;
            }
        }
    }
}

// @src 140375025  b(1:n) = a(1:n)
void fun_140375025(double* a, double* b, i32* n)
{
    for (i32 k = 1; k <= *n; ++k)
        b[k - 1] = a[k - 1];
}

// @src 1403735bd  a(1:n) = 0
void fun_1403735bd(i32* a, i32* n)
{
    for (i32 k = 1; k <= *n; ++k)
        a[k - 1] = 0;
}

// @src 1403668de  VN(:,j) = v + w x (X(:,j) - c) in the listing's order (140366a4a..140366a99)
void fun_1403668de(double* x, double* vn, i32* n)
{
    for (i32 j = 1; j <= *n; ++j) {
        const double wy = g_body.w[1];
        double r[3];
        for (int i = 0; i < 3; ++i)
            r[i] = x[3 * (j - 1) + i] - g_body.c[i];
        double* o = vn + 3 * (j - 1);
        o[0] = ((wy * r[2]) + g_body.v[0]) - (r[1] * g_body.w[2]);
        o[1] = ((g_body.w[2] * r[0]) + g_body.v[1]) - (r[2] * g_body.w[0]);
        o[2] = ((r[1] * g_body.w[0]) + g_body.v[2]) - (wy * r[0]);
    }
}

// @src 140366ac3  Gaussian elimination with row pivoting (column search, strict >), A n x n
// column-major, b overwritten by the solution; *err = 1 when the pivot is <= 1e-10 (ordered
// compare: a NaN pivot passes, 140366bb8). A(i,k) below the pivot is not zeroed.
void fun_140366ac3(double* a, double* b, i32* n, i32* err)
{
    const i32 nn = *n;
    const std::int64_t ld = nn > 0 ? nn : 0;
    auto A = [&](i32 i, i32 j) -> double& { return a[(j - 1) * ld + (i - 1)]; };
    *err = 0;
    for (i32 k = 1; k <= nn; ++k) {
        i32 p = k;
        double amax = std::fabs(A(k, k));
        for (i32 i = k + 1; i <= nn; ++i) {
            const double v = std::fabs(A(i, k));
            if (v > amax) { // 140366f9e: JP/JBE skip
                amax = v;
                p = i;
            }
        }
        if (amax <= 1e-10) { // DAT_140bafc18
            *err = 1;
            return;
        }
        if (p != k) {
            for (i32 c = k; c <= nn; ++c) {
                const double t = A(k, c);
                A(k, c) = A(p, c);
                A(p, c) = t;
            }
            const double t = b[k - 1];
            b[k - 1] = b[p - 1];
            b[p - 1] = t;
        }
        for (i32 i = k + 1; i <= nn; ++i) {
            for (i32 c = k + 1; c <= nn; ++c)
                A(i, c) = A(i, c) - ((A(i, k) * A(k, c)) / A(k, k));
            b[i - 1] = b[i - 1] - ((A(i, k) * b[k - 1]) / A(k, k));
        }
    }
    // The binary also runs these lines for n <= 0 (out-of-range access); the drop always has n = 6.
    if (nn < 1)
        return;
    b[nn - 1] = b[nn - 1] / A(nn, nn);
    for (i32 i = nn - 1; i >= 1; --i) {
        double s = b[i - 1];
        for (i32 c = nn; c >= i + 1; --c) {
            s = s - (A(i, c) * b[c - 1]);
            b[i - 1] = s;
        }
        b[i - 1] = s / A(i, i);
    }
}

} // namespace def::cmd::drop
