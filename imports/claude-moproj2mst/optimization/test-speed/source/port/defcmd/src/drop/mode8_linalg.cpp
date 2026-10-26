// Mode-8 axis point: the eigen solver and the small vector routines of the surface mass-property
// code (packet p67-3). Spec: docs/re/commands/OBJPOS_drop_mode8.md §5. Floating-point order from
// the listing (deform-rea-work/out/p673/c/L_m8.txt), traced with out/p67/fp-audit/tr2.py.
#include <defcmd/drop/mode8.hpp>

#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>

namespace def::cmd::drop {

namespace {

inline double fabs_mask(double v) { return std::fabs(v); } // ANDPD with DAT_140bb1048

inline double neg_bits(double v) // XORPD with DAT_140bb1000 (-0.0)
{
    std::uint64_t u;
    std::memcpy(&u, &v, 8);
    u ^= 0x8000000000000000ull;
    std::memcpy(&v, &u, 8);
    return v;
}

} // namespace

// @src 1403f0b89  cyclic Jacobi (the Numerical Recipes scheme): v = I, b = d = diag(a), z = 0;
// per sweep (1..50): sm = sum |a(ip,iq)| over ip < iq (from 0.0, iq inner); sm == 0 -> return;
// tresh = (sm*0.2)/(n*n) for sweeps 1..3, else 0; rotations as below; then b += z, d = b, z = 0.
// The work vectors b, z (0x140708fd8, 0x140709f78, 500 each) are set before use in every call.
void jacobi_eigen(double* a, std::int32_t* n, std::int32_t* np, double* d, double* v,
                  std::int32_t* nrot)
{
    const std::int64_t ld = *np > 0 ? *np : 0;
    auto A = [&](std::int32_t i, std::int32_t j) -> double& { return a[(i - 1) + (j - 1) * ld]; };
    auto V = [&](std::int32_t i, std::int32_t j) -> double& { return v[(i - 1) + (j - 1) * ld]; };
    const std::int32_t nn = *n;
    std::vector<double> b(static_cast<std::size_t>(nn > 0 ? nn : 0) + 1);
    std::vector<double> z(static_cast<std::size_t>(nn > 0 ? nn : 0) + 1);

    if (0 < nn) {
        for (std::int32_t ip = 1; ip <= *n; ++ip) {
            for (std::int32_t iq = 1; iq <= *n; ++iq) V(ip, iq) = 0.0;
            V(ip, ip) = 1.0;
        }
        for (std::int32_t ip = 1; ip <= *n; ++ip) {
            z[ip] = 0.0;
            b[ip] = A(ip, ip);
            d[ip - 1] = b[ip];
        }
    }
    const std::int32_t n2 = nn * nn; // cvtsi2sd of the integer product
    *nrot = 0;
    std::int32_t sweep = 1; // 0x14070af20
    for (;;) {
        double sm = 0.0; // 0x14070af28
        for (std::int32_t ip = 1; ip <= nn - 1; ++ip)
            for (std::int32_t iq = ip + 1; iq <= *n; ++iq) sm = fabs_mask(A(ip, iq)) + sm;
        if (sm == 0.0) return; // 1403f0fc9: COMISD; JP continues, JZ returns
        double tresh;          // 0x14070af30
        if (sweep < 4)
            tresh = (sm * 0.2) / static_cast<double>(n2);
        else
            tresh = 0.0;
        for (std::int32_t ip = 1; ip <= nn - 1; ++ip) {
            for (std::int32_t iq = ip + 1; iq <= *n; ++iq) {
                const double apq = A(ip, iq);
                const double g = fabs_mask(apq) * 100.0; // 0x14070af38
                if (4 < sweep && fabs_mask(d[ip - 1]) == g + fabs_mask(d[ip - 1]) &&
                    fabs_mask(d[iq - 1]) == g + fabs_mask(d[iq - 1])) {
                    A(ip, iq) = 0.0;
                    continue;
                }
                if (!(fabs_mask(apq) > tresh)) continue; // 1403f1602 JP/JBE skip
                const double dip = d[ip - 1];
                double h = d[iq - 1] - dip; // 0x14070af40
                double t;                   // 0x14070af48
                if (fabs_mask(h) == fabs_mask(h) + g) {
                    t = apq / h;
                } else {
                    const double theta = (h * 0.5) / apq; // 0x14070af50
                    t = 1.0 / (std::sqrt(theta * theta + 1.0) + fabs_mask(theta));
                    if (0.0 > theta) t = neg_bits(t);
                }
                const double c = 1.0 / std::sqrt(t * t + 1.0); // 0x14070af58
                const double s = t * c;                         // 0x14070af60
                const double tau = s / (1.0 + c);               // 0x14070af68
                h = t * apq;
                z[ip] = z[ip] - h;
                d[ip - 1] = dip - h;
                z[iq] = h + z[iq];
                A(ip, iq) = 0.0;
                d[iq - 1] = h + d[iq - 1];
                auto rot = [&](double& p, double& q) {
                    const double gg = p; // 0x14070af38
                    const double hh = q; // 0x14070af40
                    p = gg - ((gg * tau) + hh) * s;
                    q = ((gg - tau * hh) * s) + hh;
                };
                for (std::int32_t j = 1; j <= ip - 1; ++j) rot(A(j, ip), A(j, iq));
                for (std::int32_t j = ip + 1; j <= iq - 1; ++j) rot(A(ip, j), A(j, iq));
                for (std::int32_t j = iq + 1; j <= *n; ++j) rot(A(ip, j), A(iq, j));
                for (std::int32_t j = 1; j <= *n; ++j) rot(V(j, ip), V(j, iq));
                *nrot = *nrot + 1;
            }
        }
        for (std::int32_t ip = 1; ip <= *n; ++ip) {
            const double bb = b[ip] + z[ip];
            z[ip] = 0.0;
            b[ip] = bb;
            d[ip - 1] = bb;
        }
        sweep = sweep + 1;
        if (50 < sweep) return;
    }
}

// @src 1403eef06  c = a x b: c1 = a2*b3 - b2*a3, c3 = a1*b2 - b1*a2, c2 = a3*b1 - b3*a1
void cross3(double* a, double* b, double* c)
{
    const double c0 = a[1] * b[2] - b[1] * a[2];
    const double c2 = a[0] * b[1] - b[0] * a[1];
    const double c1 = a[2] * b[0] - b[2] * a[0];
    c[0] = c0;
    c[2] = c2;
    c[1] = c1;
}

// @src 1403f0590  *r = 0.0, then *r = a(i)*b(i) + *r for i = 1..n
void dot_n(double* a, double* b, double* r, std::int32_t* n)
{
    *r = 0.0;
    for (std::int32_t i = 1; i <= *n; ++i) *r = a[i - 1] * b[i - 1] + *r;
}

} // namespace def::cmd::drop
