// Geometry kernel library, quadrilateral projection group (defrt/ftn/geoutl_proj.hpp).
// Transcribed from the PathScale listings (instruction addresses in the comments where the order
// is not a left-to-right reading). sqrt is the inline SQRTSD instruction (std::sqrt compiles to
// it); fabs is the ANDPD with DAT_140bb2638 (std::fabs). SAVE variables are written where and
// when the binary writes them, and values the binary re-reads from memory are re-read here.
// 1404d41f8 (PNTEDG) lives in geoutl_d41f8.cpp (p67-3).
#include <defrt/ftn/geoutl.hpp>

#include <defrt/ftn/sse_ops.hpp>

#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace def::rt::geoutl {

// Offsets of the SAVE blocks (the comments in the header give the addresses).
static_assert(offsetof(QuadProjState, b1b_c) == 0x0);
static_assert(offsetof(QuadProjState, b1b_i) == 0x4);
static_assert(offsetof(QuadProjState, b1b_node) == 0x8);
static_assert(offsetof(QuadProjState, b1d_c) == 0xc);
static_assert(offsetof(QuadProjState, b1d_i) == 0x10);
static_assert(offsetof(QuadProjState, nonplanar) == 0x14);
static_assert(offsetof(QuadProjState, newton_fail) == 0x18);
static_assert(offsetof(QuadProjState, edge_code) == 0x1c);
static_assert(offsetof(QuadProjState, i) == 0x20);
static_assert(offsetof(QuadProjState, c1) == 0x30);
static_assert(offsetof(QuadProjState, c2) == 0x60);
static_assert(offsetof(QuadProjState, a) == 0x90);
static_assert(offsetof(QuadProjState, b_copy) == 0xb0);
static_assert(offsetof(QuadProjState, skip) == 0xc0);
static_assert(offsetof(QuadProjState, ntri) == 0xc4);
static_assert(offsetof(QuadProjState, j) == 0xc8);
static_assert(offsetof(QuadProjState, tri) == 0xd0);
static_assert(offsetof(QuadProjState, tri_s) == 0x120);
static_assert(offsetof(QuadProjState, st) == 0x130);
static_assert(offsetof(QuadProjState, nroot) == 0x150);
static_assert(offsetof(QuadProjState, ierr) == 0x154);
static_assert(offsetof(QuadProjState, u) == 0x160);
static_assert(offsetof(QuadProjState, loc) == 0x180);
static_assert(offsetof(QuadProjState, k) == 0x184);
static_assert(offsetof(QuadProjState, cen) == 0x190);
static_assert(offsetof(QuadProjState, j2) == 0x1b0);
static_assert(offsetof(QuadProjState, k_copy) == 0x1b4);
static_assert(offsetof(QuadProjState, k2) == 0x1b8);
static_assert(offsetof(QuadProjState, t3) == 0x1c0);
static_assert(offsetof(QuadProjState, q3) == 0x210);
static_assert(offsetof(QuadProjState, d3) == 0x230);
static_assert(offsetof(QuadProjState, code3) == 0x238);
static_assert(offsetof(QuadProjState, n) == 0x240);
static_assert(offsetof(QuadProjState, len) == 0x260);
static_assert(offsetof(QuadProjState, one) == 0x268);
static_assert(offsetof(QuadProjState, rot) == 0x270);
static_assert(offsetof(QuadProjState, xl) == 0x2c0);
static_assert(offsetof(QuadProjState, roots) == 0x320);
static_assert(offsetof(QuadProjState, nr) == 0x360);
static_assert(offsetof(QuadProjState, ir) == 0x364);
static_assert(offsetof(QuadProjState, rs) == 0x368);
static_assert(offsetof(QuadProjState, u2) == 0x380);
static_assert(offsetof(QuadProjState, dr) == 0x3a0);
static_assert(sizeof(QuadProjState) == 0x3a8);
static_assert(offsetof(QuadLocState, count) == 0x10);
static_assert(offsetof(QuadLocState, s_lo) == 0x20);
static_assert(offsetof(QuadLocState, d) == 0x50);
static_assert(offsetof(QuadPlaneState, n) == 0x18);
static_assert(offsetof(QuadPlaneState, q0) == 0x38);
static_assert(offsetof(QuadPlaneState, rot) == 0x58);
static_assert(offsetof(QuadPlaneState, xl) == 0xa8);
static_assert(offsetof(QuadPlaneState, k) == 0x108);
static_assert(offsetof(QuadPlaneState, dz) == 0x110);
static_assert(offsetof(QuadPlaneState, pl) == 0x118);
static_assert(offsetof(QuadPlaneState, dmin) == 0x138);
static_assert(offsetof(QuadPlaneState, iedge) == 0x140);
static_assert(offsetof(QuadPlaneState, outside) == 0x144);
static_assert(offsetof(QuadPlaneState, idx) == 0x148);
static_assert(offsetof(QuadPlaneState, st) == 0x158);
static_assert(offsetof(QuadPlaneState, nroot) == 0x168);
static_assert(offsetof(QuadPlaneState, ierr) == 0x16c);
static_assert(offsetof(QuadPlaneState, u) == 0x178);
static_assert(offsetof(QuadInvState, rot) == 0x20);
static_assert(offsetof(QuadInvState, q0) == 0x70);
static_assert(offsetof(QuadInvState, i) == 0x90);
static_assert(offsetof(QuadInvState, pl) == 0xa0);
static_assert(offsetof(QuadInvState, j) == 0xc0);
static_assert(offsetof(QuadInvState, kc) == 0xc4);
static_assert(offsetof(QuadInvState, xl) == 0xd0);
static_assert(offsetof(QuadInvState, tol) == 0x130);
static_assert(offsetof(QuadInvState, dmin) == 0x138);
static_assert(offsetof(QuadInvState, q) == 0x140);
static_assert(offsetof(QuadInvState, iedge) == 0x160);
static_assert(offsetof(QuadInvState, outside) == 0x164);
static_assert(offsetof(QuadInvState, idx) == 0x168);
static_assert(offsetof(TriLocState, i) == 0x20);
static_assert(offsetof(TriLocState, dl) == 0x28);
static_assert(offsetof(ColSortState, tmp) == 0x18);

namespace {

double bits(std::uint64_t u) { return std::bit_cast<double>(u); }

// ADDSD / MULSD: add(dest, src), mul(dest, src) in the listing's operand order, so that the NaN
// the original returns (the destination's when both are NaN) is reproduced (defrt/ftn/sse_ops.hpp).
using def::rt::sse::add;
using def::rt::sse::mul;

// Constants (.rdata DAT_140bb2xxx unless noted).
const double kEps3 = bits(0x3f50624dd2f1a9fcull);    // 0.001: DAT_140643a80 (.data) and DAT_140644870
const double kPlanarTol = bits(0x3eb0c6f7a0b5ed8dull); // DAT_140bb2680 1e-6
const double kTinyLen = bits(0x3d719799812dea11ull);   // DAT_140bb26b8 1e-12
const double kBig = bits(0x4202a05f20000000ull);       // DAT_140bb2760 1e10
const double kNegBig = bits(0xc202a05f20000000ull);    // DAT_140bb2768 -1e10
const double kStartDist = 100000.0;                    // DAT_140bb2770
const double kDistGrow = 1.5;                          // DAT_140bb2778
const double kTol1f = bits(0x3f847ae140000000ull);     // DAT_140bb2880 (double)0.01f
const double kTol2f = bits(0x3f947ae140000000ull);     // DAT_140bb2888 (double)0.02f
const double kInvBound = 100000000.0;                  // DAT_140bb2890 1e8
const double kOffTol = 5.0;                            // DAT_140bb2898

// DAT_140643a90 (.data): the quad's corners in (s, t): (-1,-1), (1,-1), (1,1), (-1,1).
const double kCorner[4][2] = {{-1.0, -1.0}, {1.0, -1.0}, {1.0, 1.0}, {-1.0, 1.0}};
// DAT_140643a40 (.data): (0, 0, 0), the point P of fun_1404ccf58 in fun_1404d275e's frame.
const double kOrigin[3] = {0.0, 0.0, 0.0};

} // namespace

void reset_proj_globals() {
    g_quad_proj = QuadProjState{};
    g_quad_loc = QuadLocState{};
    g_quad_plane = QuadPlaneState{};
    g_quad_inv = QuadInvState{};
    g_tri_loc = TriLocState{};
    g_col_sort = ColSortState{};
}

// @src 1404d203b  project a point on a bilinear quad (entry)
void fun_1404d203b(const double* x, const double* p, double* s, double* t, double* q,
                   double* dist, std::int32_t* flag) {
    QuadProjState& g = g_quad_proj;
    g.nonplanar = 0;
    *flag = 0;
    fun_1404d4a0f(x, p, s, t, dist, &g.nonplanar);
    if (g.nonplanar == 0) { // planar: fun_1404d4a0f set s, t and dist; no location code
        fun_1404d6974(x, q, s, t);
        return;
    }
    g.newton_fail = 0;
    g.edge_code = 0;
    for (g.i = 1; g.i <= 6; ++g.i) {
        g.c1[g.i - 1] = 0.0;
        g.c2[g.i - 1] = 0.0;
    }
    // Coefficients of the two Newton equations (1404d2177..1404d23cb), one axis per pass.
    g.i = 1;
    do {
        const int i = g.i;
        const double x1 = x[i - 1];
        const double x2 = x[i + 2];
        const double x3 = x[i + 5];
        const double s12 = add(x1, x2);        // XMM8
        double bb = x2 - x[i - 1];             // XMM3
        double aa = add(x3, s12);              // XMM7
        const double p4 = mul(p[i - 1], 4.0);  // XMM15 (DAT_140bb2690)
        const double x4 = x[i + 8];            // XMM14
        double dd = x1 - x2;                   // XMM1
        aa = add(aa, x4);
        bb = add(bb, x3);
        double cc = x3 - s12;                  // XMM2
        dd = add(dd, x3);
        aa = aa - p4;
        bb = bb - x4;
        cc = add(cc, x4);
        dd = dd - x4;
        g.a = aa;
        g.b_copy = bb;
        g.b = bb;
        g.c = cc;
        g.d_copy = dd;
        g.d = dd;
        const double ad = mul(aa, dd); // XMM5
        const double bc = mul(bb, cc); // XMM6
        g.c2[5] = add(mul(aa, bb), g.c2[5]);
        g.c2[4] = add(mul(bb, bb), g.c2[4]);
        g.c2[3] = add(add(ad, g.c2[3]), bc);
        g.c2[2] = add(mul(mul(bb, 2.0), dd), g.c2[2]);
        g.c2[1] = add(mul(cc, dd), g.c2[1]);
        g.c2[0] = add(mul(dd, dd), g.c2[0]);
        g.c1[5] = add(mul(aa, cc), g.c1[5]);
        g.c1[4] = add(add(ad, g.c1[4]), bc);
        g.c1[3] = add(mul(cc, cc), g.c1[3]);
        g.c1[2] = add(mul(mul(cc, 2.0), dd), g.c1[2]);
        g.c1[1] = add(mul(dd, bb), g.c1[1]);
        g.c1[0] = add(mul(dd, dd), g.c1[0]);
        g.i = i + 1;
    } while (g.i <= 3);
    fun_1404d5a1d(g.c1, g.c2, s, t, x, p, &g.newton_fail);
    bool inside;
    if (g.newton_fail == 1) {
        fun_1404d275e(x, p, q, dist, s, t, &g.edge_code);
        if (g.edge_code >= 10000) { // degenerate edge triangle: project on the other 3 corners
            g.skip = g.edge_code / 10000;
            g.ntri = 0;
            for (g.j = 1; g.j <= 4; ++g.j) {
                if (g.skip != g.j) {
                    const int jj = g.j;
                    g.ntri = g.ntri + 1;
                    double* dst = g.tri + (g.ntri - 1) * 3; // ntri <= 3 (skip is 1..4)
                    dst[0] = x[(jj - 1) * 3];
                    dst[1] = x[(jj - 1) * 3 + 1];
                    dst[2] = x[(jj - 1) * 3 + 2];
                }
            }
            fun_1404e2914(g.tri, p, &g.tri_s, &g.tri_t, q, dist, flag);
            fun_1404ddeba(x, q, g.st, &g.nroot, &g.ierr);
            if (*flag != 0) return; // no location code
            *t = g.st[1];
            *s = g.st[0];
        }
        inside = !(*s > 1.0) && !(*t > 1.0) && !(*s < -1.0) && !(*t < -1.0) && g.edge_code != 1;
    } else {
        inside = !(*s > 1.0) && !(*t > 1.0) && !(*s < -1.0) && !(*t < -1.0); // NaN: inside
    }
    if (inside) {
        fun_1404d6974(x, q, s, t);
        fun_1404d68dd(q, p, g.u, dist);
    } else {
        fun_1404d41f8(const_cast<double*>(x), const_cast<double*>(p), s, t, q, dist);
    }
    fun_1404d3e13(s, t, &g.loc);
}

// @src 1404d275e  edge fall-back of the quad projection (four corner-corner-centroid triangles)
void fun_1404d275e(const double* x, const double* p, double* q, double* dist, double* s,
                   double* t, std::int32_t* code) {
    QuadProjState& g = g_quad_proj;
    *s = 2.0;
    g.k = 1;
    *t = 2.0;
    do { // centroid
        const int i = g.k;
        double acc = 0.0;
        g.j2 = 1;
        g.cen[i - 1] = 0.0;
        do {
            acc = add(acc, mul(x[(g.j2 - 1) * 3 + (i - 1)], 0.25)); // DAT_140bb2660
            g.j2 = g.j2 + 1;
            g.cen[i - 1] = acc;
        } while (g.j2 <= 4);
        g.k = g.k + 1;
    } while (g.k <= 3);

    g.k = 1;
    *dist = kStartDist;
    std::int32_t next = 2; // R15
    for (;;) {
        const std::int32_t kk = g.k;
        g.k_copy = kk;
        g.k2 = next;
        if (kk == 4) g.k2 = 1;
        for (g.j2 = 1; g.j2 <= 3; ++g.j2) {
            const int j = g.j2;
            g.t3[6 + j - 1] = g.cen[j - 1];
            g.t3[j - 1] = x[(kk - 1) * 3 + j - 1];
            g.t3[3 + j - 1] = x[(g.k2 - 1) * 3 + j - 1];
        }
        fun_1404d3094(g.t3, p, g.q3, &g.d3, &g.code3);
        if (g.code3 == 10000) {
            *code = kk * 10000;
            return;
        }
        if (std::fabs(*dist) > std::fabs(g.d3)) {
            if (g.code3 == 0 && g.d3 == 0.0) { // P lies in this triangle: its normal
                fun_1404d3d43(g.t3, g.n);
                *code = g.code3;
                goto frame;
            }
            *dist = g.d3;
            q[0] = g.q3[0];
            q[1] = g.q3[1];
            *code = g.code3;
            q[2] = g.q3[2];
        }
        g.k = kk + 1;
        next = next + 1;
        if (g.k > 4) break;
    }
    {
        const std::int32_t c = *code;
        const std::int32_t nc = static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(c));
        const std::int32_t ac = nc < 0 ? c : nc; // NEG / CMOVS
        if (ac == 1) return;
        if (c == 2) return;
    }
    for (g.k = 1; g.k <= 3; ++g.k) g.n[g.k - 1] = q[g.k - 1] - p[g.k - 1];
    g.len = 0.0;
    // 1404d2c33..1404d2ccb: n1*n1 + len, then len + n2*n2, then n3*n3 + len
    g.k = 1;
    g.len = add(mul(g.n[0], g.n[0]), g.len);
    g.k = 2;
    g.len = add(g.len, mul(g.n[1], g.n[1]));
    g.k = 3;
    g.len = add(mul(g.n[2], g.n[2]), g.len);
    g.k = 4;
    g.len = std::sqrt(g.len);
    if (kTinyLen > g.len) {
        fun_1404d3d43(g.t3, g.n); // last triangle (k = 4)
    } else {
        for (g.k = 1; g.k <= 3; ++g.k) g.n[g.k - 1] = g.n[g.k - 1] / g.len;
    }
frame:
    g.one = 1;
    *dist = mul(*dist, kDistGrow);
    fun_1404cf5c0(g.n, g.rot);
    {
        const std::int32_t four = 4;
        fun_1404d0939(x, p, g.rot, g.xl, &g.one, &four);
    }
    fun_1404ccf58(g.xl, kOrigin, g.roots, &g.nr, code);
    if (*code != 0) return;
    g.ir = 1;
    if (g.nr < 1) return;
    do {
        g.rt = g.roots[(g.ir - 1) * 2 + 1];
        g.rs = g.roots[(g.ir - 1) * 2];
        fun_1404d6974(x, g.q3, &g.rs, &g.rt);
        fun_1404d68dd(p, g.q3, g.u2, &g.dr);
        const double dr = g.dr;
        if (std::fabs(*dist) > std::fabs(dr)) {
            q[2] = g.q3[2];
            q[1] = g.q3[1];
            q[0] = g.q3[0];
            *dist = dr;
            *t = g.rt;
            *s = g.rs;
        }
        g.ir = g.ir + 1;
    } while (g.ir <= g.nr);
}

// @src 1404d4a0f  planar quad projection (local frame, point in polygon, inverse bilinear map)
void fun_1404d4a0f(const double* x, const double* p, double* s, double* t, double* dist,
                   std::int32_t* nonplanar) {
    QuadPlaneState& g = g_quad_plane;
    g.three = 3;
    g.one = 1;
    *nonplanar = 1;
    fun_1404e33b7(x, x + 9, &g.same);
    g.nv = 4;
    if (g.same == 1) { // triangle: s, t keep their incoming values for the origin below
        g.nv = 3;
        fun_1404d3d43(x, g.n);
    } else {
        *t = 0.0;
        *s = 0.0;
        fun_1404cccfb(x, s, t, g.n);
    }
    fun_1404d6974(x, g.q0, s, t);
    fun_1404cf5c0(g.n, g.rot);
    std::int32_t cnt = 4;
    fun_1404d0939(x, g.q0, g.rot, g.xl, &g.one, &cnt);
    g.k = 2;
    for (;;) {
        g.dz = std::fabs(g.xl[3 * g.k - 1] - g.xl[2]);
        if (g.dz > kPlanarTol) goto mapped; // NaN: planar
        g.k = g.k + 1;
        if (g.k > 4) break;
    }
    *nonplanar = 0;
mapped:
    cnt = 1;
    fun_1404d0939(p, g.q0, g.rot, g.pl, &g.one, &cnt);
    {
        const std::int32_t closed = 1;
        const std::int32_t useidx = 0;
        cnt = 4;
        g.dmin = kBig;
        fun_1404cc289(g.xl, g.pl, g.q0, &g.dmin, &g.iedge, &cnt, &g.outside, &closed, g.idx,
                      &useidx, &g.three);
    }
    if (g.outside <= 0) {
        fun_1404ccf58(g.xl, g.pl, g.st, &g.nroot, &g.ierr);
        *t = g.st[1];
        *s = g.st[0];
    }
    if (*nonplanar == 1) return;
    if (g.outside <= 0) {
        fun_1404d6974(x, g.q0, s, t);
        fun_1404d68dd(p, g.q0, g.u, dist);
    } else {
        fun_1404d41f8(const_cast<double*>(x), const_cast<double*>(p), s, t, g.q0, dist);
    }
}

// @src 1404ddeba  (s,t) of a point in a quad by the planar inverse map, with tolerance checks
void fun_1404ddeba(const double* x, const double* p, double* st, std::int32_t* nroot,
                   std::int32_t* ierr) {
    QuadInvState& g = g_quad_inv;
    double s0 = 0.0;
    double t0 = 0.0;
    fun_1404cccfb(x, &s0, &t0, g.n);
    fun_1404cf5c0(g.n, g.rot);
    s0 = 0.0;
    t0 = 0.0;
    fun_1404d6974(x, g.q0, &s0, &t0);
    g.i = 1;
    do { // P in the frame: pl(i) = sum_j R(i,j) (p(j) - q0(j)), from 0.0
        const int i = g.i;
        double acc = 0.0;
        g.j = 1;
        g.pl[i - 1] = 0.0;
        do {
            const int j = g.j;
            acc = add(acc, mul(p[j - 1] - g.q0[j - 1], g.rot[(j - 1) * 3 + (i - 1)]));
            g.j = j + 1;
            g.pl[i - 1] = acc;
        } while (g.j <= 3);
        g.i = i + 1;
    } while (g.i <= 3);
    g.kc = 1;
    do { // corners in the frame
        g.i = 1;
        do {
            const int i = g.i;
            const int kc = g.kc;
            double acc = 0.0;
            g.j = 1;
            g.xl[(kc - 1) * 3 + (i - 1)] = 0.0;
            do {
                const int j = g.j;
                acc = add(acc, mul(x[(kc - 1) * 3 + (j - 1)] - g.q0[j - 1], g.rot[(j - 1) * 3 + (i - 1)]));
                g.j = j + 1;
                g.xl[(kc - 1) * 3 + (i - 1)] = acc;
            } while (g.j <= 3);
            g.i = i + 1;
        } while (g.i <= 3);
        g.kc = g.kc + 1;
    } while (g.kc <= 4);

    g.tol = g_common_140b84720.u08; // 140b84728
    if (g.tol == 0.0) g.tol = mul(g_common_140b84720.u10, kTol1f);
    if (g.tol == 0.0) g.tol = kTol2f;
    {
        const std::int32_t n4 = 4;
        const std::int32_t closed = 1;
        const std::int32_t useidx = 0;
        const std::int32_t ld3 = 3;
        g.dmin = kInvBound;
        fun_1404cc289(g.xl, g.pl, g.q, &g.dmin, &g.iedge, &n4, &g.outside, &closed, g.idx,
                      &useidx, &ld3);
    }
    if (g.outside > 0 && g.dmin > g.tol) {
        *ierr = 1;
        return;
    }
    fun_1404ccf58(g.xl, g.pl, st, nroot, ierr);
    if (*ierr != 0) return;
    fun_1404d6974(g.xl, g.q, st, st + 1);
    if (std::fabs(g.q[2] - g.pl[2]) > mul(g.tol, kOffTol)) *ierr = 1;
}

// @src 1404d3e13  location code of (s,t) in the quad: 0 inside, edge k, or -corner
void fun_1404d3e13(const double* s, const double* t, std::int32_t* code) {
    QuadLocState& g = g_quad_loc;
    const double e = kEps3;
    const double hi = 1.0 - e; // XMM3
    const double lo = add(e, -1.0); // XMM5
    g.hi = hi;
    *code = 0;
    g.lo = lo;
    {
        const double sv = *s;
        if (sv > lo && hi > sv) {
            const double tv = *t;
            if (tv > lo && hi > tv) return;
        }
    }
    g.count = 0;
    g.k = 1;
    const double ep1 = add(e, 1.0); // XMM0
    const double m1me = -1.0 - e; // XMM4
    std::int32_t next = 2;
    for (;;) {
        g.k2 = next;
        if (g.k == 4) g.k2 = 1;
        const int k = g.k;
        const int k2 = g.k2;
        const double sk2 = kCorner[k2 - 1][0];
        const double sk = kCorner[k - 1][0];
        g.s_lo = m1me;
        g.s_hi = ep1;
        g.t_lo = m1me;
        g.t_hi = ep1;
        if (sk2 == sk) { // edge at constant s
            if (0.0 > sk) {
                g.s_lo = m1me;
                g.s_hi = lo;
            } else {
                g.s_hi = ep1;
                g.s_lo = hi;
            }
        }
        const double tk = kCorner[k - 1][1];
        const double tk2 = kCorner[k2 - 1][1];
        if (tk2 == tk) { // edge at constant t
            if (0.0 > tk) {
                g.t_lo = m1me;
                g.t_hi = lo;
            } else {
                g.t_lo = hi;
                g.t_hi = ep1;
            }
        }
        const double sv = *s;
        if (sv >= g.s_lo && g.s_hi >= sv) {
            const double tv = *t;
            if (tv >= g.t_lo && g.t_hi >= tv) {
                g.count = g.count + 1;
                *code = g.k;
            }
        }
        g.k = g.k + 1;
        next = next + 1;
        if (g.k > 4) break;
    }
    if (g.count == 1) return;
    std::int32_t c = -1;
    g.k = 1;
    g.dmin = 2.0; // DAT_140bb2698
    for (;;) {
        const int k = g.k;
        double ds = *s - kCorner[k - 1][0];
        double dt = *t - kCorner[k - 1][1];
        ds = mul(ds, ds);
        dt = mul(dt, dt);
        const double d2 = add(dt, ds);
        const double d = std::sqrt(d2);
        g.d2 = d2;
        g.d = d;
        if (g.dmin > d) {
            g.dmin = d;
            *code = c;
        }
        g.k = k + 1;
        c = c - 1;
        if (g.k > 4) break;
    }
}

// @src 1404e37c4  location code of a point in a triangle (vertex -i, edge i+1, inside 0)
void fun_1404e37c4(const double* s, const double* t, std::int32_t* code) {
    TriLocState& g = g_tri_loc;
    const double sv = *s;
    const double l3a = 1.0 - sv;
    const double tv = *t;
    g.l[0] = sv;
    g.i = 1;
    g.l[1] = tv;
    g.l[2] = l3a - tv;
    for (;;) {
        const double v = std::fabs(add(g.l[g.i - 1], -1.0));
        g.dl = v;
        if (kEps3 > v) { // DAT_140644870
            *code = -g.i;
            return;
        }
        g.i = g.i + 1;
        if (g.i > 3) break;
    }
    g.i = 1;
    for (;;) {
        const std::int32_t i = g.i;
        if (kEps3 > std::fabs(g.l[i - 1])) {
            *code = i + 1;
            if (i + 1 <= 3) return;
            *code = 1;
            return;
        }
        g.i = i + 1;
        if (g.i > 3) {
            *code = 0;
            return;
        }
    }
}

// @src 1404d1b1c  box of the listed points, widened by a margin
void fun_1404d1b1c(const double* x, double* box, const double* margin, const std::int32_t* n,
                   const std::int32_t* idx) {
    QuadProjState& g = g_quad_proj;
    for (g.b1b_c = 1; g.b1b_c <= 3; ++g.b1b_c) {
        box[g.b1b_c - 1] = kBig;
        box[g.b1b_c + 2] = kNegBig;
    }
    g.b1b_i = 1;
    if (*n >= 1) {
        do {
            g.b1b_node = idx[g.b1b_i - 1];
            const std::int64_t node = g.b1b_node;
            for (g.b1b_c = 1; g.b1b_c <= 3; ++g.b1b_c) {
                const std::int64_t c = g.b1b_c;
                const double v = x[c + node * 3 - 4];
                if (box[c - 1] > v) box[c - 1] = v;
                if (v > box[c + 2]) box[c + 2] = v;
            }
            g.b1b_i = g.b1b_i + 1;
        } while (*n >= g.b1b_i);
    }
    const double z0 = box[2];
    const double m = *margin;
    const double y0 = box[1];
    const double x0 = box[0];
    box[2] = z0 - m;
    box[1] = y0 - m;
    box[0] = x0 - m;
    const double x1 = add(m, box[3]);
    const double z1 = add(m, box[5]);
    const double y1 = add(m, box[4]);
    box[3] = x1;
    box[5] = z1;
    box[4] = y1;
}

// @src 1404d1ed7  overlap of two boxes (closed intervals)
void fun_1404d1ed7(const double* a, const double* b, std::int32_t* ov) {
    *ov = 0;
    if (b[0] > a[3]) return;
    if (b[1] > a[4]) return;
    if (b[2] > a[5]) return;
    if (a[0] > b[3]) return;
    if (a[1] > b[4]) return;
    if (a[2] > b[5]) return;
    *ov = 1;
}

// @src 1404d1d05  box of the points 1..n, widened by a margin
void fun_1404d1d05(const double* x, double* box, const double* margin, const std::int32_t* n) {
    QuadProjState& g = g_quad_proj;
    for (g.b1d_c = 1; g.b1d_c <= 3; ++g.b1d_c) {
        box[g.b1d_c - 1] = kBig;
        box[g.b1d_c + 2] = kNegBig;
    }
    g.b1d_i = 1;
    if (*n >= 1) {
        do {
            const std::int64_t pt = g.b1d_i;
            for (g.b1d_c = 1; g.b1d_c <= 3; ++g.b1d_c) {
                const std::int64_t c = g.b1d_c;
                const double v = x[c + pt * 3 - 4];
                if (box[c - 1] > v) box[c - 1] = v;
                if (v > box[c + 2]) box[c + 2] = v;
            }
            g.b1d_i = g.b1d_i + 1;
        } while (*n >= g.b1d_i);
    }
    const double z0 = box[2];
    const double m = *margin;
    const double y0 = box[1];
    const double x0 = box[0];
    box[2] = z0 - m;
    box[1] = y0 - m;
    box[0] = x0 - m;
    const double x1 = add(m, box[3]);
    const double z1 = add(m, box[5]);
    const double y1 = add(m, box[4]);
    box[3] = x1;
    box[5] = z1;
    box[4] = y1;
}

// @src 1404d93ae  bubble sort of the columns of a(ld, n) by one row
void fun_1404d93ae(double* a, const std::int32_t* key, const std::int32_t* n,
                   const std::int32_t* order, const std::int32_t* ld) {
    ColSortState& g = g_col_sort;
    const std::int64_t ld64 = *ld;
    const std::int32_t nn = *n;
    g.i = 1;
    const std::int64_t stride = ld64 >= 0 ? ld64 : 0; // CMOVGE
    const std::int32_t last = nn - 1;
    if (last < 1) return;
    const std::int32_t rows = static_cast<std::int32_t>(ld64);
    const bool any_row = rows >= 1;
    std::int32_t jstart = 2; // EDX
    for (;;) {
        g.m = nn + 1;
        g.j = jstart;
        if (g.j <= nn) {
            std::int32_t lower = nn; // EBX, always m - 1 below
            for (;;) {
                g.m = g.m - 1;
                lower = lower - 1;
                const std::int64_t off_lo = (static_cast<std::int64_t>(lower) - 1) * stride;
                const std::int64_t off_hi = (static_cast<std::int64_t>(g.m) - 1) * stride;
                const std::int64_t kr = *key;
                const double v_lo = a[kr + off_lo - 1];
                const double v_hi = a[kr + off_hi - 1];
                const bool swap = (*order == 1) ? (v_lo > v_hi) : (v_hi > v_lo);
                if (swap) {
                    g.r = 1;
                    if (any_row) {
                        do {
                            const std::int64_t r = g.r;
                            const double hi = a[r + off_hi - 1];
                            const double lo = a[r + off_lo - 1];
                            a[r + off_lo - 1] = hi;
                            if (r <= 64) g.tmp[r - 1] = lo; // unbounded in the binary
                            a[r + off_hi - 1] = lo;
                            g.r = g.r + 1;
                        } while (g.r <= rows);
                    }
                }
                g.j = g.j + 1;
                if (g.j > nn) break;
            }
        }
        g.i = g.i + 1;
        jstart = jstart + 1;
        if (g.i > last) break;
    }
}

} // namespace def::rt::geoutl
