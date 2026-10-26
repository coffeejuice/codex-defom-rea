// Mode-8 axis point: the surface integrals of the mass-property routine (1403ea82d and its
// callees), packet p67-3. Spec: docs/re/commands/OBJPOS_drop_mode8.md §4 and
// OBJPOS_geo_kernels.md §2.2-2.4. Floating-point order from the listing
// (deform-rea-work/out/p673/c/L_m8.txt, tracer out/p67/fp-audit/tr2.py), not from the decompile.
// The routines' SAVE scratch is written before it is read in every call: C++ locals here.
#include <defcmd/drop/globals_c.hpp>
#include <defcmd/drop/mode8.hpp>

#include <cmath>
#include <cstdint>
#include <cstring>

#include <defrt/port.hpp>

namespace def::cmd::drop {

namespace {

double bits(std::uint64_t u)
{
    double d;
    std::memcpy(&d, &u, 8);
    return d;
}

// X(j, node), node 1-based, j 0-based
inline double& px(double* x, std::int32_t node, int j) { return x[(static_cast<std::int64_t>(node) - 1) * 3 + j]; }

// 3-point triangle rule, area coordinates L1 (0x140620860) and L2 (0x140620880): 2/3 and 1/6
// rounded up
const double kT1[3] = {bits(0x3fe5555555555556ull), bits(0x3fc5555555555557ull), bits(0x3fc5555555555557ull)};
const double kT2[3] = {bits(0x3fc5555555555557ull), bits(0x3fe5555555555556ull), bits(0x3fc5555555555557ull)};
const double kThird = bits(0x3fd5555555555555ull); // DAT_140bb10d0

// Brick node coordinates T(3,8) that 1403ee73e stores at 0x140708a90 on every call
// (DAT_140bb1070 = 1.0, DAT_140bb11b8 = -1.0).
const double kBrick[8][3] = {{-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
                             {-1, -1, 1},  {1, -1, 1},  {1, 1, 1},  {-1, 1, 1}};

// c = a x b as the face routines compute it: c0 = a1*b2 - b1*a2, c1 = a2*b0 - b2*a0,
// c2 = b1*a0 - a1*b0; returns sqrt((c0*c0 + c1*c1) + c2*c2)
inline double cross_len(const double* a, const double* b, double* c)
{
    c[2] = b[1] * a[0] - a[1] * b[0];
    c[1] = a[2] * b[0] - b[2] * a[0];
    c[0] = a[1] * b[2] - b[1] * a[2];
    return std::sqrt(c[2] * c[2] + (c[1] * c[1] + c[0] * c[0]));
}

} // namespace

// @src 1403ee538  gp(4,4) abscissae and gw(4,4) weights of the 1..4-point Gauss rules (column =
// rule order, row = point), zeros elsewhere
void gauss_tables(double* gp, double* gw)
{
    for (int i = 0; i < 16; ++i) gp[i] = 0.0;
    for (int i = 0; i < 16; ++i) gw[i] = 0.0;
    gp[15] = bits(0x3feb8e6dbcf63989ull); // DAT_140bb1188
    gw[0] = 2.0;                          // DAT_140bb1190
    gw[1] = 1.0;                          // DAT_140bb1070
    gw[5] = 1.0;
    gw[2] = bits(0x3fe1c71c71c71c76ull); // DAT_140bb1198
    gw[6] = bits(0x3fec71c71c71c71dull); // DAT_140bb11a0
    gw[10] = bits(0x3fe1c71c71c71c76ull);
    gw[3] = bits(0x3fd64340f7e7b66eull); // DAT_140bb11a8
    gw[7] = bits(0x3fe4de5f840c24c9ull); // DAT_140bb11b0
    gw[11] = bits(0x3fe4de5f840c24c9ull);
    gw[15] = bits(0x3fd64340f7e7b66eull);
    gp[1] = bits(0xbfe279a74590331eull);  // DAT_140bb1150
    gp[5] = bits(0x3fe279a74590331eull);  // DAT_140bb1158
    gp[2] = bits(0xbfe8c97ef43f7244ull);  // DAT_140bb1160
    gp[10] = bits(0x3fe8c97ef43f7244ull); // DAT_140bb1168
    gp[3] = bits(0xbfeb8e6dbcf63989ull);  // DAT_140bb1170
    gp[7] = bits(0xbfd5c23fd9dd3df7ull);  // DAT_140bb1178
    gp[11] = bits(0x3fd5c23fd9dd3df7ull); // DAT_140bb1180
}

// @src 1403ee73e  for brick node k = 1..8 with corner (xk, yk, zk): p = xi*xk + 1.0,
// q = eta*yk + 1.0, r = 1.0 + zeta*zk; N(k) = ((0.125*p)*q)*r; dN(1,k) = ((xk*0.125)*q)*r,
// dN(2,k) = ((yk*0.125)*p)*r, dN(3,k) = ((zk*0.125)*p)*q
void brick_shape(double* xi, double* eta, double* zeta, double* nf, double* dn)
{
    for (int k = 0; k < 8; ++k) {
        const double p = *xi * kBrick[k][0] + 1.0;  // 0x140708b58
        const double q = *eta * kBrick[k][1] + 1.0; // 0x140708b60
        const double d3 = ((kBrick[k][2] * 0.125) * p) * q;
        const double d1p = (kBrick[k][0] * 0.125) * q;
        const double d2p = (kBrick[k][1] * 0.125) * p;
        const double np = (0.125 * p) * q;
        dn[k * 3 + 2] = d3;
        const double r = 1.0 + *zeta * kBrick[k][2]; // 0x140708b68
        nf[k] = np * r;
        dn[k * 3 + 0] = d1p * r;
        dn[k * 3 + 1] = d2p * r;
    }
}

// @src 1403eed50  t(i,k) = (((0.0 + X(i,f1)*dN(k,5)) + X(i,f2)*dN(k,6)) + X(i,f3)*dN(k,7))
// + X(i,f4)*dN(k,8), i = 1..3, k = 1..2
void face_tangents(std::int32_t* /*face_no*/, std::int32_t* fc, double* x, double* dn, double* t)
{
    for (int i = 0; i < 6; ++i) t[i] = 0.0;
    for (int i = 0; i < 3; ++i) {
        for (int k = 0; k < 2; ++k) {
            double s = t[k * 3 + i];
            for (int m = 0; m < 4; ++m) s = s + px(x, fc[m], i) * dn[(m + 4) * 3 + k];
            t[k * 3 + i] = s;
        }
    }
}

// @src 1403eebc7  shape functions at (xi, eta, 1.0), tangents t1, t2; E = (t1x^2 + t1y^2) + t1z^2,
// F = (t1x*t2x + t1y*t2y) + t1z*t2z, G = (t2x^2 + t2y^2) + t2z^2
void face_metric(std::int32_t* face_no, double* xi, double* eta, std::int32_t* fc, double* x,
                 double* e, double* f, double* g)
{
    double a = *xi, b = *eta, one = 1.0; // 0x140708bc8 / bd0 / bd8
    double n[8], dn[24], t[6];           // 0x140708be0, 0x140708c20, 0x140708ce0
    brick_shape(&a, &b, &one, n, dn);
    face_tangents(face_no, fc, x, dn, t);
    *g = t[5] * t[5] + (t[4] * t[4] + t[3] * t[3]);
    *e = t[2] * t[2] + (t[1] * t[1] + t[0] * t[0]);
    *f = t[2] * t[5] + (t[1] * t[4] + t[0] * t[3]);
}

// @src 1403edb4f  integrand k: F = (x, 0, 0), ((x*0.5)*x, 0, 0), (0, (y*0.5)*y, 0),
// (0, 0, (z*0.5)*z) for k = 1..4; with x' = x - Cx etc. (centroid g_massprop[19..21]):
// k = 5..7 the cubes ((v'*v')*v')/3.0 of the two other axes, k = 8..10 ((x'x')y')*-0.5,
// ((x'x')z')*-0.5 (F1), ((y'y')z')*-0.5 (F2); *out = (F1*n1 + F2*n2) + F3*n3, the unused
// components +0.0. Other k: *out not written.
void integrand(double* x, double* y, double* z, double* out, double* n, std::int32_t* k)
{
    double dx = 0.0, dy = 0.0, dz = 0.0; // 0x140708898..8a8 (written only for k > 4)
    if (4 < *k) {
        dx = *x - g_massprop[19];
        dy = *y - g_massprop[20];
        dz = *z - g_massprop[21];
    }
    double f0 = 0.0, f1 = 0.0, f2 = 0.0; // 0x1407088b0..c0
    switch (*k) {
    case 1: f0 = *x; break;
    case 2: f0 = (*x * 0.5) * *x; break;
    case 3: f1 = (*y * 0.5) * *y; break;
    case 4: f2 = (*z * 0.5) * *z; break;
    case 5:
        f1 = ((dy * dy) * dy) / 3.0;
        f2 = ((dz * dz) * dz) / 3.0;
        break;
    case 6:
        f0 = ((dx * dx) * dx) / 3.0;
        f2 = ((dz * dz) * dz) / 3.0;
        break;
    case 7:
        f0 = ((dx * dx) * dx) / 3.0;
        f1 = ((dy * dy) * dy) / 3.0;
        break;
    case 8: f0 = ((dx * dx) * dy) * -0.5; break;
    case 9: f0 = ((dx * dx) * dz) * -0.5; break;
    case 10: f1 = ((dy * dy) * dz) * -0.5; break;
    default: return;
    }
    *out = n[2] * f2 + (n[1] * f1 + f0 * n[0]);
}

// @src 1403ec3cc  type 4: for corner m = 1..4 (next, previous wrap): a = X(next) - X(cur),
// b = X(prev) - X(cur), c = a x b, L = |c|; L > 0: c = c/L; n = c/type + n; area = L + area;
// then area = area*0.25. Type 3: a = X(f2) - X(f1), b = X(f3) - X(f1); L > 0: n = c/L;
// area = L*0.5. Other types: n = 0, area = 0.
void face_normal(double* x, std::int32_t* fc, double* n, double* area, std::int32_t* type)
{
    n[0] = n[1] = n[2] = 0.0;
    const std::int32_t t0 = *type;
    *area = 0.0;
    if (t0 == 4) {
        const std::int32_t nt = *type;
        const std::int32_t first = fc[0];
        std::int32_t cur = 0, nxt = 0, prv = 0; // 0x1407084d4 / d8 / dc
        for (std::int32_t m = 1; m <= *type; ++m) {
            cur = fc[m - 1];
            if (m < *type)
                nxt = fc[m];
            else if (*type == m)
                nxt = first;
            if (m < 2) {
                if (m == 1) prv = fc[nt - 1];
            } else {
                prv = fc[m - 2];
            }
            double a[3], b[3], c[3]; // 0x140708500, 0x140708520, 0x1407084e0
            for (int j = 0; j < 3; ++j) a[j] = px(x, nxt, j) - px(x, cur, j);
            for (int j = 0; j < 3; ++j) b[j] = px(x, prv, j) - px(x, cur, j);
            const double l = cross_len(a, b, c); // 0x140708540
            if (l > 0.0) {
                for (int j = 0; j < 3; ++j) c[j] = c[j] / l;
            }
            const double dt = static_cast<double>(*type);
            for (int j = 0; j < 3; ++j) n[j] = c[j] / dt + n[j];
            *area = l + *area;
        }
        *area = *area * 0.25; // DAT_140bb10c0
    }
    if (*type != 3) return;
    const std::int32_t cur = fc[0], nxt = fc[1], prv = fc[2];
    double a[3], b[3], c[3];
    for (int j = 0; j < 3; ++j) a[j] = px(x, nxt, j) - px(x, cur, j);
    for (int j = 0; j < 3; ++j) b[j] = px(x, prv, j) - px(x, cur, j);
    const double l = cross_len(a, b, c);
    if (l > 0.0) {
        for (int j = 0; j < 3; ++j) n[j] = c[j] / l;
    }
    *area = l * 0.5; // DAT_140bb10c8
}

// @src 1403ed06d  per face i = 1..nf (face first+i-1): a = X(f1) - X(f3), b = X(f2) - X(f3),
// A = |a x b|*0.5, w = A*(1/3); per point q = 1..3: L = (T1(q), T2(q), (1.0 - T1) - T2),
// X_j = ((0.0 + X(j,f1)*L1) + X(j,f2)*L2) + X(j,f3)*L3; *sum = w*f_k(X, n_i) + *sum
void tri_rule(double* x, std::int32_t* f, std::int32_t* first, std::int32_t* nf,
              std::int32_t* type, double* nrm, double* sum, std::int32_t* k)
{
    *sum = 0.0;
    std::int32_t fi = *first;
    for (std::int32_t i = 1; i <= *nf; ++i, ++fi) {
        std::int32_t* fc = f + (static_cast<std::int64_t>(fi) - 1) * 4;
        double a[3], b[3], c[3]; // 0x140708600, 0x140708620, 0x140708640
        for (int j = 0; j < 3; ++j) a[j] = px(x, fc[0], j) - px(x, fc[2], j);
        for (int j = 0; j < 3; ++j) b[j] = px(x, fc[1], j) - px(x, fc[2], j);
        const double ar = cross_len(a, b, c) * 0.5; // 0x140708660
        const double w = ar * kThird;                // 0x1407086b0
        for (int q = 0; q < 3; ++q) {
            double l[3]; // 0x140708690
            l[0] = kT1[q];
            l[1] = kT2[q];
            l[2] = (1.0 - l[0]) - l[1];
            double px0 = 0.0, py0 = 0.0, pz0 = 0.0; // 0x140708670..680
            for (std::int32_t m = 1; m <= *type; ++m) {
                const std::int32_t nd = fc[m - 1];
                const double lm = l[m - 1];
                px0 = px(x, nd, 0) * lm + px0;
                py0 = px(x, nd, 1) * lm + py0;
                pz0 = lm * px(x, nd, 2) + pz0;
            }
            double fv = 0.0; // 0x1407086c0 (always written for k = 1..10)
            integrand(&px0, &py0, &pz0, &fv, nrm + (static_cast<std::int64_t>(i) - 1) * 3, k);
            *sum = w * fv + *sum;
        }
    }
}

// @src 1403ed5af  faces first..first+nf-1, xi = gp(2,i) (outer), eta = gp(2,j) (inner):
// wt = (gw(2,i)*gw(2,j))*sqrt(E*G - F*F); X = sum over the face nodes of X*N(m+4) from 0.0;
// n = (t1 x t2)/|t1 x t2| (no zero test); *sum = wt*f_k(X, n) + *sum. nrm and area unused.
void quad_rule(double* x, std::int32_t* f, std::int32_t* first, std::int32_t* nf,
               std::int32_t* type, double* sum, std::int32_t* k, double* gp, double* gw,
               double* /*nrm*/, double* /*area*/)
{
    double one = 1.0; // 0x1407086c8
    const std::int32_t f0 = *first, cnt = *nf;
    *sum = 0.0;
    for (std::int32_t fi = f0; fi <= f0 - 1 + cnt; ++fi) {
        std::int32_t* fc = f + (static_cast<std::int64_t>(fi) - 1) * 4;
        for (int i = 1; i < 3; ++i) {
            double xi = gp[(i - 1) * 4 + 1]; // 0x1407086d8
            for (int j = 1; j < 3; ++j) {
                double eta = gp[(j - 1) * 4 + 1]; // 0x1407086e8
                std::int32_t fno = fi;
                double e, ff, g; // 0x1407086f0, 0x1407086f8, 0x140708700
                face_metric(&fno, &xi, &eta, fc, x, &e, &ff, &g);
                const double da = std::sqrt(e * g - ff * ff);              // 0x140708708
                const double wt = (gw[(i - 1) * 4 + 1] * gw[(j - 1) * 4 + 1]) * da; // 0x140708710
                double n8[8], dn[24], t[6]; // 0x140708720, 0x140708760, 0x140708820
                brick_shape(&xi, &eta, &one, n8, dn);
                fno = fi;
                face_tangents(&fno, fc, x, dn, t);
                double qx = 0.0, qy = 0.0, qz = 0.0; // 0x140708850..860
                for (std::int32_t m = 1; m <= *type; ++m) {
                    const std::int32_t nd = fc[m - 1];
                    const double nm = n8[m + 3];
                    qx = px(x, nd, 0) * nm + qx;
                    qy = px(x, nd, 1) * nm + qy;
                    qz = nm * px(x, nd, 2) + qz;
                }
                double c[3]; // 0x140708870
                c[0] = t[1] * t[5] - t[2] * t[4];
                c[2] = t[4] * t[0] - t[3] * t[1];
                c[1] = t[2] * t[3] - t[5] * t[0];
                const double l = std::sqrt(c[2] * c[2] + (c[1] * c[1] + c[0] * c[0])); // 0x140708890
                for (int jj = 0; jj < 3; ++jj) c[jj] = c[jj] / l;
                integrand(&qx, &qy, &qz, &ff, c, k); // result into F's slot 0x1407086f8
                *sum = wt * ff + *sum;
            }
        }
    }
}

// @src 1403ea82d  mode 0 (the only mode its caller 1403e8510 uses): normals and areas of faces
// i = 1..nf (face first+i-1, its first `type` node numbers), then for k = 1..10 the integral I_k
// by the quad (type 4) or triangle (type 3) rule: V = I1, C = (I2, I3, I4)/V, I11 = I5,
// I22 = I6, I33 = I7, I12 = I8, I13 = I9, I23 = I10; lower triangle copied, M = V*identity.
void surface_integrals(double* x, std::int32_t* f, std::int32_t* first, std::int32_t* nf,
                       std::int32_t* type, double* nrm, double* area, std::int32_t* mode,
                       double* /*p9*/, std::int32_t* dbg, double* gp, double* gw)
{
    double* const b = g_massprop;
    std::int32_t fi = *first;
    for (std::int32_t i = 1; i <= *nf; ++i, ++fi) {
        double* ni = nrm + (static_cast<std::int64_t>(i) - 1) * 3;
        ni[0] = ni[1] = ni[2] = 0.0;
        std::int32_t fc[4] = {0, 0, 0, 0}; // 0x140708470 (only 1..type are read)
        for (std::int32_t m = 1; m <= *type; ++m) fc[m - 1] = f[(static_cast<std::int64_t>(fi) - 1) * 4 + (m - 1)];
        face_normal(x, fc, ni, area + (i - 1), type);
    }
    double acc = 0.0; // 0x140708480
    if (*mode == 0) {
        for (std::int32_t k = 1; k < 11; ++k) {
            std::int32_t kk = k; // local_258
            if (*type == 4) quad_rule(x, f, first, nf, type, &acc, &kk, gp, gw, nrm, area);
            kk = k;
            if (*type == 3) tri_rule(x, f, first, nf, type, nrm, &acc, &kk);
            if (k == 1) b[18] = acc;
            if (k == 2) b[19] = acc / b[18];
            if (k == 3) b[20] = acc / b[18];
            if (k == 4) b[21] = acc / b[18];
            if (k == 5) b[0] = acc;
            if (k == 6) b[4] = acc;
            if (k == 7) b[8] = acc;
            if (k == 8) b[3] = acc;
            if (k == 9) b[6] = acc;
            if (k == 10) b[7] = acc;
        }
        b[5] = b[7];
        b[2] = b[6];
        b[1] = b[3];
        for (int i = 9; i < 18; ++i) b[i] = 0.0;
        b[17] = b[18];
        b[9] = b[18];
        b[13] = b[18];
    }
    if (*mode == 1 || *mode == 2 || *mode == 3) {
        // modes 1..3 (1403ee161 / 1403ee24b, symmetry planes): 1403ea82d's only caller is
        // 1403e8510 with mode 0 (DAT_140708180 = 0); not ported
        DEF_NOT_PORTED(0x1403ea82d);
    }
    if (0 < *dbg) DEF_NOT_PORTED(0x1403ea82d); // "mass", "center", "Iii", "Iij" WRITEs
}

} // namespace def::cmd::drop
