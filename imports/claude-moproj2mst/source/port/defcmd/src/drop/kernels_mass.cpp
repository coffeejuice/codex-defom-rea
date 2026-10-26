// Drop solver kernels (packet p67-3): Gauss table, characteristic length, mass properties.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.1, §5.2, §5.3. FP order from the listing
// (out/p67/drop-solver/L_all.txt, sym_multi.txt). All statics these routines use
// (0x140704ee0..0x140705560, 0x140706b30..0x140706b90) are written before they are read in every
// call, so they are locals here.
#include "kernels_mass_internal.hpp"

#include <defcmd/drop/kernels.hpp>

#include <cmath>

namespace def::cmd::drop {

namespace {

constexpr double kOne = 1.0;     // DAT_140baf930
constexpr double kHalf = 0.5;    // DAT_140baf918
constexpr double kThree = 3.0;   // DAT_140baf928
constexpr double kMHalf = -0.5;  // DAT_140bafba0
constexpr double k0125 = 0.125;  // DAT_140bafc08
constexpr double kTiny = 1e-11;  // DAT_140bafb30

double* xcol(double* x, i32 node) { return x + 3 * static_cast<std::ptrdiff_t>(node - 1); }

} // namespace

// @src 140365e0b  GP(4,4) = 0, W(4,4) = 0, then the 1- to 4-point Gauss-Legendre rules
// (DAT_140bafba8..140bafc00, 15 significant digits); store order of the listing
void fun_140365e0b()
{
    for (double& v : g_gauss_p) v = 0.0;
    for (double& v : g_gauss_w) v = 0.0;
    g_gauss_w[15] = 0.347854845137454;   // cc98 W(4,4)
    g_gauss_w[2] = 0.555555555555556;    // cc30 W(3,1)
    g_gauss_p[7] = -0.339981043584856;   // cbd8 GP(4,2)
    g_gauss_w[10] = 0.555555555555556;   // cc70 W(3,3)
    g_gauss_w[7] = 0.652145154862546;    // cc58 W(4,2)
    g_gauss_w[11] = 0.652145154862546;   // cc78 W(4,3)
    g_gauss_w[6] = 0.888888888888889;    // cc50 W(3,2)
    g_gauss_w[3] = 0.347854845137454;    // cc38 W(4,1)
    g_gauss_p[15] = 0.861136311594053;   // cc18 GP(4,4)
    g_gauss_w[0] = 2.0;                  // cc20 W(1,1)
    g_gauss_w[5] = 1.0;                  // cc48 W(2,2)
    g_gauss_w[1] = 1.0;                  // cc28 W(2,1)
    g_gauss_p[11] = 0.339981043584856;   // cbf8 GP(4,3)
    g_gauss_p[1] = -0.577350269189626;   // cba8 GP(2,1)
    g_gauss_p[5] = 0.577350269189626;    // cbc8 GP(2,2)
    g_gauss_p[2] = -0.774596669241483;   // cbb0 GP(3,1)
    g_gauss_p[10] = 0.774596669241483;   // cbf0 GP(3,3)
    g_gauss_p[3] = -0.861136311594053;   // cbb8 GP(4,1)
}

// @src 1403750cf  *len = sqrt(min over groups g of |bbox(face g)|^2); bbox over the npf nodes
// of face g (not of a face range), max start -1e8, min start 1e8 (DAT_140bafe48/40)
void fun_1403750cf(double* x, i32* faces, i32* npf, i32* ngroups, double* len)
{
    double b = 1e10; // DAT_140bafc50 -> 140706b30
    for (i32 g = 1; g <= *ngroups; ++g) {
        double mx[3];
        double mn[3];
        for (int i = 0; i < 3; ++i) {
            mx[i] = -100000000.0;
            mn[i] = 100000000.0;
        }
        for (i32 k = 1; k <= *npf; ++k) {
            const i32 node = faces[4 * (g - 1) + (k - 1)];
            for (int i = 0; i < 3; ++i) {
                const double v = xcol(x, node)[i];
                if (mn[i] > v) mn[i] = v; // 14037528e: SETA
                if (v > mx[i]) mx[i] = v; // 1403752ac
            }
        }
        double q = 0.0;
        for (int i = 0; i < 3; ++i) {
            const double e = mx[i] - mn[i];
            q = (e * e) + q;
        }
        if (b > q) // 1403753d5: JP/JBE skip
            b = q;
    }
    *len = std::sqrt(b);
}

// @src 140364241  npf = 3: unit normal of the triangle (0 if degenerate); npf = 4: average over
// the 4 corners of the unit corner normals, out_i = (q_i / npf) + out_i (not normalised)
void fun_140364241(double* x, i32* nodes, double* out, i32* npf)
{
    out[0] = 0.0;
    out[1] = 0.0;
    out[2] = 0.0;
    auto corner = [&](i32 n0, i32 n1, i32 n2, double* q) -> double {
        double a[3];
        double b[3];
        for (int i = 0; i < 3; ++i) a[i] = xcol(x, n1)[i] - xcol(x, n0)[i];
        for (int i = 0; i < 3; ++i) b[i] = xcol(x, n2)[i] - xcol(x, n0)[i];
        q[0] = (a[1] * b[2]) - (b[1] * a[2]);
        q[1] = (a[2] * b[0]) - (b[2] * a[0]);
        q[2] = (b[1] * a[0]) - (a[1] * b[0]);
        return std::sqrt((q[2] * q[2]) + ((q[1] * q[1]) + (q[0] * q[0]))); // 1403644bb
    };
    if (*npf == 4) {
        const i32 n = *npf;
        const double dn = static_cast<double>(n); // 1403645b3
        i32 next = 0;
        i32 prev = 0;
        for (i32 k = 1; k <= *npf; ++k) {
            const i32 cur = nodes[k - 1];
            if (k < *npf)
                next = nodes[k];
            else if (*npf == k)
                next = nodes[0];
            if (k >= 2)
                prev = nodes[k - 2];
            else if (k == 1)
                prev = nodes[n - 1];
            double q[3];
            const double l = corner(cur, next, prev, q);
            if (l > 0.0) {
                for (int i = 0; i < 3; ++i) q[i] = q[i] / l;
            }
            for (int i = 0; i < 3; ++i) out[i] = (q[i] / dn) + out[i];
        }
    }
    if (*npf == 3) {
        double q[3];
        const double l = corner(nodes[0], nodes[1], nodes[2], q);
        if (l > 0.0) {
            for (int i = 0; i < 3; ++i) out[i] = q[i] / l;
        }
    }
}

// @src 1403653fe  f = (n3*P3) + ((n2*P2) + (n1*P1)); P per case (§5.3.4); cases > 4 use
// (x, y, z) - c with the current centre g_body.c. Case 0 or > 10: *f unchanged.
void fun_1403653fe(double* x, double* y, double* z, double* f, double* n, i32* kase)
{
    double xp = 0.0;
    double yp = 0.0;
    double zp = 0.0;
    if (*kase > 4) {
        xp = *x - g_body.c[0];
        yp = *y - g_body.c[1];
        zp = *z - g_body.c[2];
    }
    double p1 = 0.0;
    double p2 = 0.0;
    double p3 = 0.0;
    switch (*kase) {
    case 1: p1 = *x; break;
    case 2: p1 = (*x * kHalf) * *x; break;
    case 3: p2 = (*y * kHalf) * *y; break;
    case 4: p3 = (*z * kHalf) * *z; break;
    case 5:
        p2 = ((yp * yp) * yp) / kThree;
        p3 = ((zp * zp) * zp) / kThree;
        break;
    case 6:
        p1 = ((xp * xp) * xp) / kThree;
        p3 = ((zp * zp) * zp) / kThree;
        break;
    case 7:
        p1 = ((xp * xp) * xp) / kThree;
        p2 = ((yp * yp) * yp) / kThree;
        break;
    case 8: p1 = ((xp * xp) * yp) * kMHalf; break;
    case 9: p1 = ((xp * xp) * zp) * kMHalf; break;
    case 10: p2 = ((yp * yp) * zp) * kMHalf; break;
    default: return; // 140365544
    }
    *f = (n[2] * p3) + ((n[1] * p2) + (n[0] * p1));
}

// @src 1403649bb  *s = sum over faces f_first..f_first+nf-1 (triangles) of
// A * integrand(centroid, FV(:,i), case), A = |(X1 - X3) x (X2 - X3)| * 0.5
void fun_1403649bb(double* x, i32* faces, i32* f_first, i32* nf, i32* npf, double* fv, double* s,
                   i32* kase)
{
    *s = 0.0;
    const double dn = static_cast<double>(*npf);
    i32 f = *f_first;
    for (i32 i = 1; i <= *nf; ++i, ++f) {
        const i32* fn = faces + 4 * static_cast<std::ptrdiff_t>(f - 1);
        double a[3];
        double b[3];
        for (int k = 0; k < 3; ++k) a[k] = xcol(x, fn[0])[k] - xcol(x, fn[2])[k];
        for (int k = 0; k < 3; ++k) b[k] = xcol(x, fn[1])[k] - xcol(x, fn[2])[k];
        const double q1 = (a[1] * b[2]) - (b[1] * a[2]);
        const double q2 = (a[2] * b[0]) - (b[2] * a[0]);
        const double q3 = (b[1] * a[0]) - (a[1] * b[0]);
        const double area = std::sqrt((q3 * q3) + ((q2 * q2) + (q1 * q1))) * kHalf; // 140364cac
        double cx = 0.0;
        double cy = 0.0;
        double cz = 0.0;
        for (i32 m = 1; m <= *npf; ++m) {
            const double* p = xcol(x, fn[m - 1]);
            cx = p[0] + cx;
            cy = p[1] + cy;
            cz = p[2] + cz;
        }
        cy = cy / dn;
        cx = cx / dn;
        cz = cz / dn;
        double fval = 0.0; // 140705068: written by the integrand for cases 1..10
        fun_1403653fe(&cx, &cy, &cz, &fval, fv + 3 * static_cast<std::ptrdiff_t>(i - 1), kase);
        *s = (area * fval) + *s; // 140364e2b
    }
}

// @src 1403660b9  node signs (xi_k, eta_k, zeta_k) of the 8-node brick; a = (xi*xi_k) + 1,
// b = (eta*eta_k) + 1, c = 1 + (zeta*zeta_k); N_k = ((0.125*a)*b)*c,
// dN(1,k) = ((xi_k*0.125)*b)*c, dN(2,k) = ((eta_k*0.125)*a)*c, dN(3,k) = ((zeta_k*0.125)*a)*b
void fun_1403660b9(double* xi, double* eta, double* zeta, double* n, double* dn)
{
    // 140705430: signs set from DAT_140baf930 (1.0) / DAT_140baf950 (-1.0) on every call
    static const double sgn[8][3] = {{-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
                                     {-1, -1, 1},  {1, -1, 1},  {1, 1, 1},  {-1, 1, 1}};
    for (int k = 0; k < 8; ++k) {
        const double a = (*xi * sgn[k][0]) + kOne;
        const double b = (*eta * sgn[k][1]) + kOne;
        const double c = kOne + (*zeta * sgn[k][2]);
        dn[3 * k + 2] = ((sgn[k][2] * k0125) * a) * b;
        n[k] = ((k0125 * a) * b) * c;
        dn[3 * k + 0] = ((sgn[k][0] * k0125) * b) * c;
        dn[3 * k + 1] = ((sgn[k][1] * k0125) * a) * c;
    }
}

// @src 1403666cb  T(i,j) = (((0 + x1 D(j,5)) + x2 D(j,6)) + x3 D(j,7)) + x4 D(j,8),
// x_m = X(i, nodes(m)); T is 3 x 2 column-major (t1 = d/dxi, t2 = d/deta)
void fun_1403666cb(i32* /*fidx*/, i32* nodes, double* x, double* dn, double* t)
{
    for (int k = 0; k < 6; ++k) t[k] = 0.0;
    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 2; ++j) {
            double s = t[3 * (j - 1) + (i - 1)];
            for (int m = 1; m <= 4; ++m) {
                s = s + (xcol(x, nodes[m - 1])[i - 1] * dn[3 * (m + 4 - 1) + (j - 1)]);
                t[3 * (j - 1) + (i - 1)] = s;
            }
        }
    }
}

// @src 140366542  E = (t1z*t1z) + ((t1y*t1y) + (t1x*t1x)), F = (t1z*t2z) + ((t1y*t2y) + (t1x*t2x)),
// G = (t2z*t2z) + ((t2y*t2y) + (t2x*t2x)), tangents at (xi, eta, zeta = 1)
void fun_140366542(i32* fidx, double* xi, double* eta, i32* nodes, double* x, double* e, double* fm,
                   double* g)
{
    double lxi = *xi;
    double leta = *eta;
    double lzeta = kOne;
    double nn[8];
    double dn[24];
    fun_1403660b9(&lxi, &leta, &lzeta, nn, dn);
    double t[6];
    fun_1403666cb(fidx, nodes, x, dn, t);
    *g = (t[5] * t[5]) + ((t[4] * t[4]) + (t[3] * t[3]));
    *e = (t[2] * t[2]) + ((t[1] * t[1]) + (t[0] * t[0]));
    *fm = (t[2] * t[5]) + ((t[1] * t[4]) + (t[0] * t[3]));
}

// @src 140364e60  *s = sum over quads f_first..f_first+nf-1 of J * integrand(x, q, case) at the
// single Gauss point (xi, eta) = (GP(1,1), GP(1,1)), J = sqrt((E*G) - (F*F)) * (W(1,1)*W(1,1));
// faces with |t1 x t2| <= 1e-11 are skipped
void fun_140364e60(double* x, i32* faces, i32* f_first, i32* nf, i32* npf, double* s, i32* kase)
{
    double zeta = kOne; // 140705070
    *s = 0.0;
    const i32 last = *f_first + *nf - 1;
    for (i32 f = *f_first; f <= last; ++f) {
        i32* fn = faces + 4 * static_cast<std::ptrdiff_t>(f - 1);
        for (i32 i = 1; i <= 1; ++i) {
            double xi = g_gauss_p[4 * (i - 1)];
            for (i32 j = 1; j <= 1; ++j) {
                double eta = g_gauss_p[4 * (j - 1)];
                i32 fidx = f;
                double e = 0.0;
                double fm = 0.0; // 1407050a0, later overwritten by the integrand value
                double g = 0.0;
                fun_140366542(&fidx, &xi, &eta, fn, x, &e, &fm, &g);
                const double jac =
                    std::sqrt((e * g) - (fm * fm)) * (g_gauss_w[4 * (i - 1)] * g_gauss_w[4 * (j - 1)]);
                double nn[8];
                double dn[24];
                fun_1403660b9(&xi, &eta, &zeta, nn, dn);
                fidx = f;
                double t[6];
                fun_1403666cb(&fidx, fn, x, dn, t);
                double px = 0.0;
                double py = 0.0;
                double pz = 0.0;
                for (i32 m = 1; m <= *npf; ++m) {
                    const double nm = nn[m + 4 - 1];
                    const double* p = xcol(x, fn[m - 1]);
                    px = (p[0] * nm) + px;
                    py = (p[1] * nm) + py;
                    pz = (nm * p[2]) + pz;
                }
                double q[3];
                q[0] = (t[1] * t[5]) - (t[2] * t[4]);
                q[1] = (t[2] * t[3]) - (t[5] * t[0]);
                q[2] = (t[4] * t[0]) - (t[3] * t[1]);
                const double l = std::sqrt((q[2] * q[2]) + ((q[1] * q[1]) + (q[0] * q[0])));
                if (l > kTiny) { // 1403653e9: JP/JA
                    for (int k = 0; k < 3; ++k) q[k] = q[k] / l;
                    fun_1403653fe(&px, &py, &pz, &fm, q, kase);
                    *s = (jac * fm) + *s;
                }
            }
        }
    }
}

// @src 140362db9  face vectors FV(:,1..nf), then mass m, centre c and inertia I of the body
// (density 1) into g_body; constraints nk (0 none, 1/2 planes K(:,1..2), 3 axis K(:,3))
void fun_140362db9(double* x, i32* faces, i32* f_first, i32* f_count, i32* npf, double* fv,
                   i32* nk, double* k)
{
    {
        i32 f = *f_first;
        for (i32 i = 1; i <= *f_count; ++i, ++f) {
            double* o = fv + 3 * static_cast<std::ptrdiff_t>(i - 1);
            o[0] = 0.0;
            o[1] = 0.0;
            o[2] = 0.0;
            i32 nodes[4] = {0, 0, 0, 0}; // 140704ef0
            for (i32 m = 1; m <= *npf; ++m)
                nodes[m - 1] = faces[4 * static_cast<std::ptrdiff_t>(f - 1) + (m - 1)];
            fun_140364241(x, nodes, o, npf);
        }
    }

    double s = 0.0; // 140704f00
    double* in = g_body.inertia;
    auto integrate = [&](i32 j) {
        i32 kase = j;
        if (*npf == 4)
            fun_140364e60(x, faces, f_first, f_count, npf, &s, &kase);
        if (*npf == 3)
            fun_1403649bb(x, faces, f_first, f_count, npf, fv, &s, &kase);
        if (j == 1) g_body.mass = s;
        if (j == 2) g_body.c[0] = s / g_body.mass;
        if (j == 3) g_body.c[1] = s / g_body.mass;
        if (j == 4) g_body.c[2] = s / g_body.mass;
        if (j == 5) in[0] = s;
        if (j == 6) in[4] = s;
        if (j == 7) in[8] = s;
        if (j == 8) in[3] = s;
        if (j == 9) in[6] = s;
        if (j == 10) in[7] = s;
    };
    auto finish = [&]() {
        in[5] = in[7];
        in[2] = in[6];
        in[1] = in[3];
        for (double& v : g_body.massm) v = 0.0;
        g_body.massm[8] = g_body.mass;
        g_body.massm[0] = g_body.mass;
        g_body.massm[4] = g_body.mass;
    };
    // |n_i| == 1.0 (exact) zeroes the products of inertia that involve axis i
    auto zero_products = [&](const double* nrm) {
        if (std::fabs(nrm[0]) == kOne) {
            in[6] = 0.0;
            in[3] = 0.0;
        }
        if (std::fabs(nrm[1]) == kOne) {
            in[7] = 0.0;
            in[3] = 0.0;
        }
        if (std::fabs(nrm[2]) == kOne) {
            in[7] = 0.0;
            in[6] = 0.0;
        }
    };

    if (*nk == 0) {
        for (i32 j = 1; j <= 10; ++j)
            integrate(j);
        finish();
    }
    if (*nk == 1 || *nk == 2) {
        for (i32 j = 1; j <= 10; ++j) {
            integrate(j);
            if (j == 4) {
                double p[3] = {k[0], k[1], k[2]}; // 140704f10
                double n[3] = {k[3], k[4], k[5]}; // 140704f30
                fun_140365a10(g_body.c, p, n);
                if (*nk == 2) {
                    double p2[3] = {k[6], k[7], k[8]};
                    double n2[3] = {k[9], k[10], k[11]};
                    fun_140365a10(g_body.c, p2, n2);
                }
            }
            if (j == 10) {
                const double n[3] = {k[3], k[4], k[5]};
                zero_products(n);
                if (*nk == 2) {
                    const double n2[3] = {k[9], k[10], k[11]};
                    zero_products(n2);
                }
            }
        }
        finish();
    }
    if (*nk == 3) {
        for (i32 j = 1; j <= 7; ++j) {
            integrate(j);
            if (j == 4) {
                double p[3] = {k[12], k[13], k[14]};
                double n[3] = {k[15], k[16], k[17]};
                fun_140365b02(g_body.c, p, n);
            }
            if (j == 7) {
                in[3] = 0.0;
                in[6] = 0.0;
                in[7] = 0.0;
            }
        }
        finish();
    }
}

} // namespace def::cmd::drop
