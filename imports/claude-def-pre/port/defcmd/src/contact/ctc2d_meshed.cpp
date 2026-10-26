// 2D GENCTC meshed master (GENCTC.md 4.5): driver 14034fc56 and the nearest node / segment search
// 140434952. Control flow and operation order from the listings (out/p676/asm.txt in
// deform-rea-work), not from the decompile: Ghidra drops the 11th argument of 140434952 and the
// RCX/RDX arguments of the 14043556b calls (both &14070d6f0, &14070d700), and invents a write to
// DAT_140bb1558. Comparisons keep the COMISD/JP polarity of the original (NaN paths noted).
// Fortran 1-based address arithmetic is kept: xy - 0x10 + i*0x10 is xy(1, i).
#include <defcmd/ctc2d_ftn.hpp>
#include <defcmd/ctc2d_meshed.hpp>
#include <defcmd/ctc2d_msg.hpp>

#include <cmath>
#include <cstddef>

namespace def::cmd::ftn {

namespace {

// @data 140baf840  start value of the best distance (1e10)
constexpr double k_big = 1e10;
// @data 140bb1558  rotation angle for the missing previous neighbour (3ff921fa00000000)
constexpr double k_rot_prev = 1.5707950592041016;
// @data 140bb1560  rotation angle for the missing next neighbour (bff921fa00000000)
constexpr double k_rot_next = -1.5707950592041016;
// @data 140bb1568  minimum edge length of the corner test (3ddb7cdfe0000000)
constexpr double k_eps = 1.000000013351432e-10;
// @data 140bb1520  convexity threshold of the corner test (3fa99999a0000000)
constexpr double k_conv = 0.05000000074505806;
// 140baf848 / 140bb1528 abs masks (std::fabs), 140baf850 sign mask (unary minus)

// xy(1, i) of a Fortran xy(2, *) array: base - 0x10 + i*0x10 with a 64-bit index
template <class T>
T* xy_at(T* base, std::int32_t i)
{
    return base + (static_cast<std::ptrdiff_t>(i) * 2 - 2);
}

// NEG r32 / CMOVS: -x unless that is negative, so INT_MIN stays INT_MIN
std::int32_t iabs32(std::int32_t x)
{
    const auto m = static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(x));
    return m < 0 ? x : m;
}

} // namespace

void reset_ctc2d_meshed_save()
{
    g_save_14034fc56 = {};
    g_save_140434952 = {};
}

// @src 140434952  nearest master node / segment of one loop, corner and orientation rules
void fun_140434952(const double* mxy, const double* p, double* foot, double* dist,
                   std::int32_t* iseg, const std::int32_t* n, std::int32_t* orient,
                   const std::int32_t* closed, const std::int32_t* list,
                   const std::int32_t* use_list, const std::int32_t* /*p11: never read*/)
{
    auto& s = g_save_140434952;
    const std::int32_t cnt = *n;
    s.nseg = cnt;
    s.knear = 0;
    s.dnear = *dist;
    s.k = 1;
    // nearest node (1404349e6)
    if (cnt >= 1) {
        const double px = p[0];
        const double py = p[1];
        for (;;) {
            s.node = s.k;
            if (*use_list != 0) s.node = list[s.k - 1];
            const double* c = xy_at(mxy, s.node);
            const double dx = c[0] - px;
            const double dy = c[1] - py;
            const double r = std::sqrt(dy * dy + dx * dx);  // 140434a76: dy*dy + dx*dx
            s.dk = r;
            if (s.dnear > r) {  // 140434a8c JP/JBE: NaN keeps the old one
                s.dnear = r;
                s.knear = s.k;
            }
            s.k = s.k + 1;
            if (s.k > cnt) break;
        }
    }
    if (*closed != 1) s.nseg = cnt - 1;
    s.k = 1;
    // nearest segment (140434c6a); kb runs one ahead (EBP)
    if (s.nseg >= 1) {
        std::int32_t kb = 2;
        for (;;) {
            s.q[0] = p[0];
            s.q[1] = p[1];
            const std::int32_t k = s.k;
            s.ka = k;
            s.kb = kb;
            if (kb > cnt) s.kb = 1;
            if (*use_list != 0) {
                s.na = list[k - 1];
                s.nb = list[s.kb - 1];
            } else {
                s.na = k;
                s.nb = s.kb;
            }
            const std::int32_t mode = 2;
            fun_1404345c0(s.q, xy_at(mxy, s.na), xy_at(mxy, s.nb), &s.len, &s.d, &s.s, &mode);
            const double ad = std::fabs(s.d);
            const double ab = std::fabs(*dist);
            if (!(ad > ab)) {  // 140434bda COMISD/JA: ties and NaN accept
                s.dbest = s.d;
                s.sbest = s.s;
                *dist = ad;
                foot[0] = s.q[0];
                *iseg = k;
                foot[1] = s.q[1];
            }
            s.k = k + 1;
            ++kb;
            if (s.k > s.nseg) break;
        }
    }
    // 140434cf6
    const std::int32_t is = *iseg;
    if (is == 0) return;
    if (*dist == 0.0) {  // 140434d12 JP/JZ -> 1404354ea
        *iseg = iabs32(is);
        *orient = 0;
        return;
    }
    const double dbest = s.dbest;
    if (s.dnear > std::fabs(dbest)) {  // 140434d46 JP/JBE: segment strictly nearer than any node
        *orient = 1;
        if (0.0 > dbest) *orient = -1;
        *iseg = iabs32(*iseg);
        return;
    }
    // corner: the nearest node is at least as near as the nearest segment (140434df4)
    const std::int32_t kn = iabs32(s.knear);
    *iseg = kn;
    s.kprev = kn - 1;
    if (kn == 1 && *closed == 1) s.kprev = cnt;  // 140435536
    s.knext = kn + 1;
    if (s.knext > cnt && *closed == 1) s.knext = 1;  // 140435558
    const double px = p[0];
    const double py = p[1];
    if (*use_list != 0) {
        s.nprev = s.kprev;
        if (s.kprev != 0) s.nprev = list[s.kprev - 1];
        s.ncorner = list[kn - 1];
        s.nnext = s.knext;
        if (s.knext <= cnt) s.nnext = list[s.knext - 1];
    } else {  // 1404352c6
        s.nprev = s.kprev;
        s.ncorner = kn;
        s.nnext = s.knext;
    }
    s.q[0] = px;
    s.q[1] = py;
    *orient = 1;
    const double* c = xy_at(mxy, s.ncorner);
    if (s.kprev != 0) {  // 140434f13
        const double* pp = xy_at(mxy, s.nprev);
        s.prev[0] = pp[0];
        s.prev[1] = pp[1];
    } else {  // 14043532a: no previous node, rotate (next - corner) by +k_rot_prev
        const double cy = c[1];
        const double cx = c[0];
        const double* pn = xy_at(mxy, s.nnext);
        s.ang = k_rot_prev;
        s.b[0] = pn[0] - cx;
        s.b[1] = pn[1] - cy;
        fun_14043556b(s.b, &s.ang);
        s.prev[0] = cx + s.b[0];
        s.prev[1] = cy + s.b[1];
    }
    if (s.knext > cnt) {  // 14043541a: no next node, rotate (prev node - corner) by k_rot_next
        const double cy = c[1];
        const double cx = c[0];
        const double* pp = xy_at(mxy, s.nprev);  // the node nprev, not s.prev
        s.ang = k_rot_next;
        s.b[0] = pp[0] - cx;
        s.b[1] = pp[1] - cy;
        fun_14043556b(s.b, &s.ang);
        s.next[0] = cx + s.b[0];
        s.next[1] = cy + s.b[1];
    } else {
        const double* pn = xy_at(mxy, s.nnext);
        s.next[0] = pn[0];
        s.next[1] = pn[1];
    }
    // 140434fb5: both kernel calls measure only (mode 1); results stay in SAVE
    const std::int32_t mode1 = 1;
    fun_1404345c0(s.q, s.prev, c, &s.len1, &s.d1, &s.s1, &mode1);
    fun_1404345c0(s.q, c, s.next, &s.len2, &s.d2, &s.s2, &mode1);
    // 14043506b: i = 1..2 (counter in 14070d680, left at 3)
    for (std::int32_t i = 1; i <= 2; ++i) {
        s.b[i - 1] = s.next[i - 1] - c[i - 1];
        s.k = i + 1;
        s.a[i - 1] = s.prev[i - 1] - c[i - 1];
    }
    const double la = std::sqrt(s.a[1] * s.a[1] + s.a[0] * s.a[0]);  // 14043516f
    const double lb = std::sqrt(s.b[1] * s.b[1] + s.b[0] * s.b[0]);  // 140435173
    s.la = la;
    s.lb = lb;
    if (la <= k_eps) return;  // 1404351a5 JP/JBE: NaN goes on
    if (k_eps >= lb) return;  // 1404351bf JP/JNC
    const double a0 = s.a[0] / la;
    const double b0 = s.b[0] / lb;
    const double b1 = s.b[1] / lb;
    const double a1 = s.a[1] / la;
    const double cr = b0 * a1 - a0 * b1;  // 140435227: (b0*a1) - (a0*b1)
    s.b[1] = b1;
    s.b[0] = b0;
    s.a[1] = a1;
    s.a[0] = a0;
    s.cross = cr;
    if (k_conv >= std::fabs(cr)) return;  // 14043524b JP/JNC
    if (cr >= 0.0) return;                // 140435276 JP/JNC
    *orient = -1;                         // concave corner
}

// @src 14034fc56  meshed master driver
void fun_14034fc56(double* xy, const std::int32_t* n, const std::int32_t* nodes, double* mxy,
                   const std::int32_t* nloops, std::int32_t* loops, std::int32_t* mnodes,
                   const std::int32_t* mobj, std::int32_t* bc3, const double* tol,
                   std::int32_t* changed, const std::int32_t* do_codes)
{
    auto& s = g_save_14034fc56;
    s.i = 1;
    if (*n < 1) return;
    const auto kcode = static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(*mobj));
    for (;;) {
        s.i_mod20 = s.i % 20;
        if (s.i_mod20 == 0) s.u668 = 0;
        s.node = nodes[s.i - 1];
        s.dbest = k_big;
        s.dist = k_big;
        s.j = 1;
        if (*nloops >= 1) {
            for (;;) {
                // 140350142
                const std::int32_t first = loops[2 * (static_cast<std::ptrdiff_t>(s.j) - 1)];
                const std::int32_t last = loops[2 * (static_cast<std::ptrdiff_t>(s.j) - 1) + 1];
                s.first = first;
                s.last = last;
                s.count = last - first;
                s.n1 = mnodes[first - 1];
                s.n2 = mnodes[first];
                const std::int32_t closed = 1;
                const std::int32_t use_list = 1;
                const std::int32_t p11 = 2;
                fun_140434952(mxy, xy_at(xy, s.node), s.foot, &s.dist, &s.iseg, &s.count,
                              &s.orient, &closed, &mnodes[first - 1], &use_list, &p11);
                if (s.iseg > 0) {  // 140350261
                    const double d = s.dist;
                    const double ad = std::fabs(d);
                    if (std::fabs(s.dbest) > ad) {  // 14035029a JP/JBE
                        s.dbest = d;
                        if (s.orient < 0) s.dbest = -ad;
                        // 14035005e: measure against mxy(iseg), mxy(iseg + 1) -- the loop
                        // position used directly as a node number, as in the original
                        s.bseg = s.iseg;
                        s.n1 = s.iseg;
                        s.n2 = s.iseg + 1;
                        const std::int32_t mode1 = 1;
                        fun_1404345c0(xy_at(xy, s.node), xy_at(mxy, s.iseg),
                                      xy_at(mxy, s.iseg + 1), &s.len, &s.d, &s.s, &mode1);
                        s.s2 = s.s;
                    }
                }
                s.j = s.j + 1;
                if (s.j > *nloops) break;
            }
        }
        // 14034feca: contact when d <= tol and (d >= 0 or |d| <= tol); NaN takes contact
        const double t = *tol;
        bool contact = !(t < s.dbest);
        if (contact && 0.0 > s.dbest && std::fabs(s.dbest) > t) contact = false;
        if (contact) {
            // 14034ff26: bseg (carried) indexes mnodes from its start
            const std::int32_t b = s.bseg;
            s.n1 = mnodes[b - 1];
            s.n2 = mnodes[b];
            const std::int32_t mode2 = 2;
            *changed = 1;
            fun_1404345c0(xy_at(xy, s.node), xy_at(mxy, s.n1), xy_at(mxy, s.n2), &s.len,
                          &s.dbest, &s.s, &mode2);
            if (*do_codes != 0) {
                // 14034fff4: code rule of GENCTC.md 4.3, codes bc3(1..2, node)
                std::int32_t* cc = bc3 + (static_cast<std::ptrdiff_t>(s.node) * 3 - 3);
                const std::int32_t c0 = cc[0];
                if (kcode != c0) {
                    const std::int32_t c1 = cc[1];
                    if (kcode != c1) {
                        if (c1 == 0) {
                            cc[1] = kcode;  // 140350341
                        } else if (c1 <= 0) {  // 14035032d
                            if (c0 <= 0) cc[0] = c1;
                            cc[1] = kcode;
                        } else if (c0 <= 0) {
                            cc[0] = kcode;  // 140350052
                        } else {
                            ctc2d_warn_1001(s.node, cc[0], cc[1]);  // 14034fcf0
                        }
                    }
                }
            }
        }
        s.i = s.i + 1;
        if (s.i > *n) break;
    }
}

} // namespace def::cmd::ftn
