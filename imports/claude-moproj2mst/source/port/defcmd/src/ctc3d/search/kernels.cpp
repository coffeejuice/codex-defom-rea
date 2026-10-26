// GENCTC 3D engine, group "search": geometric kernels of MDSPS2_REC (GENCTC_3d_search.md sec. 10)
// and the polygon helpers 14004f363 / 140062fb2. Floating-point order from the listings
// (deform-rea-work/out/p677/lst/L_<addr>.txt); + - * / go through def::rt::sse in the listing's
// dest, src order, so that NaN results match too. SAVE scratch that every call writes before it
// reads it (and that no other routine references: refs.txt) is kept in locals; SAVE variables
// that can be read before they are written in a call live in the static image (common.hpp).
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/sse_ops.hpp>

#include <cmath>

namespace def::cmd::ctc3d {

namespace fio = def::rt::ftn::fio;
namespace geo = def::rt::geoutl;

namespace {

using def::rt::sse::add;
using def::rt::sse::div;
using def::rt::sse::mul;
using def::rt::sse::sub;

// DAT_140badec8: 1e-16f widened (0x3c9cd2b2a0000000)
constexpr double kCrossTiny = 1.0000000168623835e-16;
// DAT_140badec0: 1e-11f widened (0x3da5fd7fe0000000)
constexpr double kNormTiny = 9.999999960041972e-12;
// DAT_140bae1c8: 1e-13f widened (0x3d3c25c260000000)
constexpr double kSegTiny = 9.9999998245167e-14;
// DAT_140bad7f8: 1e-8f widened (0x3e45798ee0000000)
constexpr double kTiny8f = 9.99999993922529e-09;
// DAT_140bada38: 0.25
constexpr double kQuarter = 0.25;

} // namespace

// @src 140062fb2  polygon node list convention (flag 1 / -1 last node negative / 0 first <= 0)
void fun_140062fb2(i32* conn, const i32* n, i32* flag)
{
    const i32 k = *n;
    const i32 last = conn[k - 1];
    *flag = 1;
    if (last < 1) {
        *flag = -1;
        conn[k - 1] = -last;
    }
    const i32 first = conn[0];
    if (0 < first) {
        return;
    }
    *flag = 0;
    conn[0] = -first;
}

// @src 14004f363  coordinates and count of the nodes of a polygon with |node| > 0
void fun_14004f363(i32* obj, i32* inst, i32* poly, double* x, i32* conn, i32* cnt)
{
    i32& flag = m<i32>(0x140676400);  // 140062fb2 flag (not used)
    i32& nvalid = m<i32>(0x140676404);
    i32& k = m<i32>(0x140676408);
    i32& node = m<i32>(0x14067640c);
    fun_140227e00(obj, inst, poly, conn, cnt);
    fun_140062fb2(conn, cnt, &flag);
    nvalid = 0;
    for (k = 1; k <= *cnt; ++k) {
        const i32 c = conn[k - 1];
        node = c < 0 ? -c : c; // NEG + CMOVS (INT_MIN stays INT_MIN)
        if (0 < node) {
            fun_140227fc0(obj, inst, &node, x + 3 * (k - 1));
            ++nvalid;
        }
    }
    *cnt = nvalid;
}

// @src 140066ff1  cross product c = a x b normalised, *q = |c|
void fun_140066ff1(const double* a, const double* b, double* c, double* q)
{
    // a(1), b(1) are read again after c(1) is stored (140067034, 140067038); the other loads
    // come first, so c may alias a or b as in the original.
    const double b2 = b[2];
    const double a1 = a[1];
    const double b1 = b[1];
    const double a2 = a[2];
    const double b0 = b[0];
    const double a0 = a[0];
    const double c0 = sub(mul(a1, b2), mul(b1, a2));
    c[0] = c0;
    const double c1 = sub(mul(a2, b[0]), mul(b2, a[0]));
    const double c2 = sub(mul(a0, b1), mul(b0, a1));
    const double s = add(mul(c2, c2), add(mul(c1, c1), mul(c0, c0))); // 140067059..14006706d
    c[2] = c2;
    c[1] = c1;
    *q = s;
    if (kCrossTiny < std::fabs(s) || s != s) { // COMISD tiny, |s|; JP/JC: unordered normalises
        const double l = def::rt::sse::sqrt(*q);
        const double r2 = div(c2, l);
        const double r1 = div(c1, l);
        const double r0 = div(c0, l);
        *q = l;
        c[2] = r2;
        c[1] = r1;
        c[0] = r0;
        return;
    }
    c[2] = 0.0;
    c[1] = 0.0;
    c[0] = 0.0;
    *q = 0.0;
}

// @src 140066a50  triangle normal with a retry on unit edge vectors
void fun_140066a50(const double* x, double* n, i32* bad)
{
    *bad = 0;
    double a[3]; // DAT_1406a1ee0
    double b[3]; // DAT_1406a1f00
    double q;    // DAT_1406a1f20
    a[0] = sub(x[3], x[0]);
    a[1] = sub(x[4], x[1]);
    b[1] = sub(x[7], x[1]);
    b[2] = sub(x[8], x[2]);
    a[2] = sub(x[5], x[2]);
    b[0] = sub(x[6], x[0]);
    fun_140066ff1(a, b, n, &q);
    if (kNormTiny < std::fabs(q) || q != q) { // JP/JC: return
        return;
    }
    const double la = def::rt::sse::sqrt(add(mul(a[2], a[2]), add(mul(a[1], a[1]), mul(a[0], a[0]))));
    const double lb = def::rt::sse::sqrt(add(mul(b[2], b[2]), add(mul(b[1], b[1]), mul(b[0], b[0]))));
    for (int k = 0; k < 3; ++k) {
        if (la > 0.0) {
            a[k] = div(a[k], la);
        }
        if (lb > 0.0) {
            b[k] = div(b[k], lb);
        }
    }
    fun_140066ff1(a, b, n, &q);
    if (kNormTiny < std::fabs(q) || q != q) {
        return;
    }
    *bad = 1;
}

// @src 140070bf5  nearest point on the segment a-b
void fun_140070bf5(const double* p, const double* a, const double* b, double* f, double* r,
                   double* dist)
{
    double e[3]; // DAT_1406ae080
    double g[3]; // DAT_1406ae0a0
    for (int k = 0; k < 3; ++k) {
        e[k] = sub(p[k], a[k]);
        g[k] = sub(b[k], a[k]);
    }
    const double le = def::rt::sse::sqrt(add(mul(e[2], e[2]), add(mul(e[1], e[1]), mul(e[0], e[0]))));
    double lf = def::rt::sse::sqrt(add(mul(g[2], g[2]), add(mul(g[1], g[1]), mul(g[0], g[0]))));
    if (le <= kSegTiny) { // JNC (an unordered le goes on)
        const double p1 = p[1];
        const double p0 = p[0];
        f[2] = p[2];
        f[1] = p1;
        f[0] = p0;
        *dist = 0.0;
        *r = 0.0;
        return;
    }
    if (lf <= kSegTiny) {
        const i32 unit = dat_14077a7bc;
        fio::Write(unit).a("Potential Geometry Error at Point :").end(); // DAT_140bae1d0
        fio::Write(unit, "(3F15.7)").d(a[0]).d(a[1]).d(a[2]).end(); // 1005(3F15.7), 1405c8390
        lf = kSegTiny;
    }
    for (int k = 0; k < 3; ++k) {
        e[k] = div(e[k], le);
        g[k] = div(g[k], lf);
    }
    const double c = add(mul(e[2], g[2]), add(mul(e[1], g[1]), mul(e[0], g[0]))); // DAT_1406ae0d0
    // DAT_1406ae0d8 = sqrt(max(0, 1 - c*c)) is computed and never used
    double rr = div(mul(c, le), lf);
    *r = rr;
    if (rr > 1.0) {
        *r = 1.0;
    }
    if (0.0 >= *r) { // JP/JC skip: a NaN stays
        *r = 0.0;
    }
    const double k = mul(*r, lf);
    f[0] = add(mul(k, g[0]), a[0]);
    f[1] = add(mul(k, g[1]), a[1]);
    f[2] = add(mul(k, g[2]), a[2]);
    fun_14007a47d(const_cast<double*>(p), f, dist);
}

// @src 1400702c8  projection of a point on a triangle (ray along -n, else the nearest edge)
void fun_1400702c8(const double* p, const double* x, double* q, double* d, double* s, double* t,
                   i32* mode, i32* found, i32* code)
{
    double* n = mp<double>(0x1406adef0);
    i32& bad = m<i32>(0x1406adf10);
    double* dir = mp<double>(0x1406adf20);
    double& din = m<double>(0x1406adf40);
    double& ss = m<double>(0x1406adf48);
    double& tt = m<double>(0x1406adf50);
    double* qq = mp<double>(0x1406adf60);   // Q of the ray, then the edge feet (3,3) at +0x20
    double* foot = mp<double>(0x1406adf80); // foot(3, edge)
    double* rr = mp<double>(0x1406adfd0);   // edge parameters
    double* de = mp<double>(0x1406adff0);   // edge distances
    i32& ecode = m<i32>(0x1406ae010);
    double& dbest = m<double>(0x1406ae018);
    double& third = m<double>(0x1406ae020); // third natural coordinate (written, never read)
    double& ssum = m<double>(0x1406ae028);
    i32& kq = m<i32>(0x1406ae030);
    double* u = mp<double>(0x1406ae040);
    double& ulen = m<double>(0x1406ae060);
    double& dot = m<double>(0x1406ae068);
    // vertex table V1 = (1, 0), V2 = (0, 1), V3 = (0, 0) (.data, DAT_1405c8300..1405c8340)
    const double v00 = m<double>(0x1405c8300);
    const double v01 = m<double>(0x1405c8308);
    const double v02 = m<double>(0x1405c8310);
    const double v10 = m<double>(0x1405c8320);
    const double v11 = m<double>(0x1405c8330);
    const double v12 = m<double>(0x1405c8338);
    const double v20 = m<double>(0x1405c8340);

    fun_140066a50(x, n, &bad);
    if (bad == 0) {
        dir[0] = -n[0];
        din = *d;
        dir[1] = -n[1];
        *code = 0;
        dir[2] = -n[2];
        fun_1400647a0(const_cast<double*>(p), dir, n, const_cast<double*>(x), &din, &ss, &tt, qq,
                      found, mode, &bad);
        if (*found != 0) {
            if (std::fabs(din) > std::fabs(*d) && *mode == 1) {
                *found = 0;
                return;
            }
            q[2] = qq[2];
            q[1] = qq[1];
            q[0] = qq[0];
            *d = din;
            *t = tt;
            *s = ss;
            return;
        }
    }
    fun_140070bf5(p, x, x + 3, foot, &rr[0], &de[0]);
    fun_140070bf5(p, x + 3, x + 6, foot + 3, &rr[1], &de[1]);
    fun_140070bf5(p, x + 6, x, foot + 6, &rr[2], &de[2]);
    {
        const double r = rr[0];
        const double omr = sub(1.0, r);
        ecode = 1;
        dbest = de[0];
        ss = add(mul(omr, v00), mul(r, v01));
        tt = add(mul(omr, v01), mul(r, v10));
        third = 0.0;
    }
    if (std::fabs(de[0]) >= std::fabs(de[1])) { // COMISD |d1|, |d2|; JP/JC skip
        const double r = rr[1];
        const double omr = sub(1.0, r);
        dbest = de[1];
        ss = 0.0;
        tt = add(mul(v10, omr), mul(r, v12));
        third = add(mul(v12, omr), mul(r, v20));
        ecode = 2;
    }
    double absbest = std::fabs(dbest); // [RSP+0xa0]
    if (absbest >= std::fabs(de[2])) {
        const double r = rr[2];
        const double omr = sub(1.0, r);
        dbest = de[2];
        third = add(mul(omr, v20), mul(r, v02));
        ss = add(mul(omr, v11), mul(r, v00));
        tt = 0.0;
        ecode = 3;
        absbest = std::fabs(de[2]);
    }
    if (absbest > std::fabs(*d) && *mode == 1) {
        *found = 0;
        return;
    }
    *s = ss;
    if (std::fabs(sub(1.0, ss)) <= kTiny8f) {
        ecode = -1;
    }
    *t = tt;
    if (std::fabs(sub(1.0, tt)) <= kTiny8f) {
        ecode = -2;
    }
    ssum = std::fabs(add(ss, tt));
    if (ssum <= kTiny8f) {
        ecode = -3;
    }
    const i32 c = ecode;
    const i32 ac = c < 0 ? -c : c;
    *d = dbest;
    *code = c;
    // quirk kept: the foot of edge |code| (for a vertex code too)
    for (kq = 1; kq <= 3; ++kq) {
        q[kq - 1] = foot[3 * (ac - 1) + (kq - 1)];
    }
    geo::fun_1404d68dd(p, q, u, &ulen);
    dot = add(mul(n[2], u[2]), add(mul(n[1], u[1]), mul(n[0], u[0])));
    if (dot > 0.0) {
        *d = -absbest;
    }
}

// @src 1400839c6  sign of the tetrahedron volume (triangle, or quad centroid + edge) and p
void fun_1400839c6(const double* p, const double* x, const i32* nv, const i32* edge, i32* sgn)
{
    double* v = mp<double>(0x1406d35c0);    // V(3,4): three points and p
    double* c = mp<double>(0x1406d3620);    // centroid sum
    i32& k = m<i32>(0x1406d3640);
    i32& knext = m<i32>(0x1406d3644);
    double& vol = m<double>(0x1406d3648);
    *sgn = 1;
    if (*nv == 3) {
        for (int i = 0; i < 9; ++i) {
            v[i] = x[i];
        }
    } else if (*nv == 4) {
        c[0] = 0.0;
        c[1] = 0.0;
        c[2] = 0.0;
        for (k = 1; k <= 4; ++k) {
            for (int i = 0; i < 3; ++i) {
                c[i] = add(x[3 * (k - 1) + i], c[i]);
            }
        }
        for (int i = 0; i < 3; ++i) {
            v[i] = mul(c[i], kQuarter);
        }
        for (int i = 0; i < 3; ++i) {
            v[3 + i] = x[3 * (*edge - 1) + i];
        }
        knext = *edge + 1;
        if (4 < knext) {
            knext = 1;
        }
        for (int i = 0; i < 3; ++i) {
            v[6 + i] = x[3 * (knext - 1) + i];
        }
    }
    for (int i = 0; i < 3; ++i) {
        v[9 + i] = p[i];
    }
    geo::fun_1404ca2cb(v, &vol);
    const double lim = m<double>(0x1405c8890); // 1e-10 (.data)
    if (std::fabs(vol) <= lim) {
        *sgn = 0;
    }
    if (-lim > vol) {
        *sgn = -1;
    }
}

} // namespace def::cmd::ctc3d
