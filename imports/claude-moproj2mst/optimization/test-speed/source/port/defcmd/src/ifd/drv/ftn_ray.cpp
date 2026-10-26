// Fortran kernels of the 3D interference distance (packet p67-2): the driver 1403539a1 and the
// ray cast of one node 1403541fc. Spec: docs/re/commands/OBJPOS_interference.md sections 5.4 and
// 5.8 (both listing-checked); listings deform-rea-work/out/p672/all_lst.txt.
//
// Arrays are Fortran: P(3, n) column-major, faces F(4, n) with 1-based node ids. The routines keep
// their locals in SAVE storage (0x140703c00..0x140703e88); only the bilinear result XE survives from
// one call to the next in a way that matters (a failed inverse map leaves the previous value, which
// 1404d6974 then uses), so all of them live in one struct here.
#include "ifd3d.hpp"

#include <cmath>
#include <cstdlib>
#include <vector>

#include <defcmd/ifd/cube.hpp>
#include <defrt/ftn/geoutl.hpp>
#include <defrt/stdlib/libm.hpp>

namespace def::cmd::ifd {

namespace geo = def::rt::geoutl;

// @data 140703c00  SAVE locals of 1403539a1 and 1403541fc
struct RayState {
    // 1403539a1
    std::int32_t verbose = 0;  // 140703c00 (0 = cfnCUBE quiet)
    std::int32_t type = 1;     // 140703c04 (polygon objects)
    double zmax = 0.0;         // 140703c08
    double zmin = 0.0;         // 140703c10
    std::int32_t k = 0;        // 140703c18
    double shift = 0.0;        // 140703c20
    std::int32_t f = 0;        // 140703c28
    std::int32_t j = 0;        // 140703c2c
    std::int32_t n = 0;        // 140703c30
    double unused40[10] = {};  // 140703c40 (argument 6 of 1403541fc, never read)
    double dn = 0.0;           // 140703c90
    // 1403541fc
    double box[6] = {};        // 140703cf0 query box (corner 0, corner 1)
    std::int32_t nc = 0;       // 140703d20 candidate count
    std::int32_t i = 0;        // 140703d24
    double xe[4] = {};         // 140703d30 XE(2, 2) of the inverse bilinear map (kept between calls)
    double v[4][3] = {};       // 140703d70 V(3, 4), column-major: v[s][i] = V(i+1, s+1)
    std::int32_t nv = 0;       // 140703dd0
    std::int32_t cx = 0;       // 140703dd4
    std::int32_t cy = 0;       // 140703dd8
    double nrm[3] = {};        // 140703de0
    double e = 0.0;            // 140703e00
    std::int32_t m = 0;        // 140703e08
    double q = 0.0;            // 140703e10
    double dmin = 0.0;         // 140703e18
    double p[3] = {};          // 140703e20 Q of 1404cc289, then P of 1404d6974 / 1404e34eb
    std::int32_t edge = 0;     // 140703e40
    std::int32_t inout = 0;    // 140703e44
    std::int32_t idx = 0;      // 140703e48
    double tp = 0.0;           // 140703e50
    std::int32_t used_tp = 0;  // 140703e58
    std::int32_t kxe = 0;      // 140703e5c
    std::int32_t err = 0;      // 140703e60 (bilinear err / triangle "bad")
    double h = 0.0;            // 140703e68
    double b[3] = {};          // 140703e70 barycentric b1, b2, b3
};

static RayState g_ray;

void reset_ray_state() { g_ray = RayState{}; }

namespace {

// @data 140baf898  -1e5 (zmax start)
constexpr double kZmaxStart = -100000.0;
// @data 140baf8a0  1e5 (zmin start, "no hit" distance)
constexpr double kNoHit = 100000.0;
// @data 140604ed8  facing test
constexpr double kFacing = 1e-8;
// @data 140604ee0  degenerate projected edge (squared)
constexpr double kEdge2 = 1e-15;
// @data 140baf8b0  start distance of the point-in-polygon test
constexpr double kPipStart = 1e8;
// @data 140baf8c0  barycentric acceptance (1.001f widened)
constexpr double kBary = 1.0010000467300415;

// @data 140604eb0  ray direction (0, 0, 1)
double g_zdir[3] = {0.0, 0.0, 1.0};
// @data 140604ef0  query point (0, 0, 0)
double g_origin[3] = {0.0, 0.0, 0.0};

} // namespace

// @src 1403541fc  ray from node p along +z onto the faces F1 of the fixed body P1: *dn = the
// smallest hit height (1e5 when none). The query box reaches *dcur above p (the running minimum).
void fun_1403541fc(double* p, double* p1, std::int32_t* f1, std::int32_t* /*nf1*/, double* z,
                   double* /*unused*/, double* dn, double* dcur)
{
    RayState& s = g_ray;
    std::int32_t zero_a = 0;
    std::int32_t zero_b = 0;
    s.box[0] = p[0];
    s.box[1] = p[1];
    s.box[2] = p[2];
    std::int32_t one_a = 1;
    std::int32_t one_b = 1;
    s.box[5] = *dcur + s.box[2];
    s.box[3] = s.box[0];
    s.box[4] = s.box[1];
    cube::fun_140228c60(&one_a, &one_b, s.box, &s.nc, &zero_a, &zero_b);
    std::vector<std::int32_t> list;  // Fortran ALLOCATE (1404b62c4) / DEALLOCATE (1404b6d74)
    if (0 < s.nc) {
        list.resize(static_cast<std::size_t>(s.nc));
        one_a = 1;
        one_b = 1;
        zero_a = 0;
        cube::fun_140229f30(&one_a, &one_b, list.data(), &zero_a);
    }
    const std::int32_t ncand = s.nc;
    s.i = 1;
    *dn = kNoHit;
    if (0 < ncand) {
        do {
            const std::int32_t face = list[static_cast<std::size_t>(s.i - 1)];
            const std::int32_t* fr = f1 + static_cast<std::ptrdiff_t>(face - 1) * 4;
            // vertices in reverse order: V(:, s) = P1(:, abs(F1(5 - s))) - p
            for (std::int32_t sv = 1; sv <= 4; ++sv) {
                std::int32_t node = fr[4 - sv];
                node = node < 0 ? -node : node;
                const double* x = p1 + static_cast<std::ptrdiff_t>(node - 1) * 3;
                for (int c = 0; c < 3; ++c) s.v[sv - 1][c] = x[c] - p[c];
            }
            std::int32_t a1 = fr[0] < 0 ? -fr[0] : fr[0];
            std::int32_t a4 = fr[3] < 0 ? -fr[3] : fr[3];
            s.nv = (a1 == a4) ? 3 : 4;
            s.cx = 0;
            s.cy = 0;
            for (std::int32_t sv = 1; sv <= s.nv; ++sv) {
                const double vx = s.v[sv - 1][0];
                const double vy = s.v[sv - 1][1];
                if (vx <= 0.0 && vx != 0.0) ++s.cx;
                if (vy <= 0.0 && vy != 0.0) ++s.cy;
            }
            if (s.cx == 0 || s.cx == s.nv) goto next;
            if (s.cy == 0 || s.cy == s.nv) goto next;
            if (s.nv == 4) {
                double xi = 0.0;
                double eta = 0.0;
                geo::fun_1404cccfb(&s.v[0][0], &xi, &eta, s.nrm);
            }
            if (s.nv == 3) geo::fun_1404d3d43(&s.v[0][0], s.nrm);
            s.e = 0.0;
            s.e = z[0] * s.nrm[0] + s.e;
            s.e = s.e + z[1] * s.nrm[1];
            s.e = s.e + z[2] * s.nrm[2];
            if (s.e <= 0.0 || std::fabs(s.e) < kFacing) goto next;
            s.m = 0;
            for (std::int32_t sv = 1; sv <= s.nv; ++sv) {
                std::int32_t t = sv + 1;
                if (s.nv < t) t = 1;
                const double dy = s.v[sv - 1][1] - s.v[t - 1][1];
                const double dx = s.v[sv - 1][0] - s.v[t - 1][0];
                s.q = dy * dy + dx * dx;
                if (s.q < kEdge2) ++s.m;
            }
            if (1 < s.m) goto next;
            {
                std::int32_t use_idx = 0;
                std::int32_t stride = 3;
                std::int32_t closed = 1;
                s.dmin = kPipStart;
                geo::fun_1404cc289(&s.v[0][0], g_origin, s.p, &s.dmin, &s.edge, &s.nv, &s.inout,
                                   &closed, &s.idx, &use_idx, &stride);
            }
            if (s.inout < 1) {
                double acc = 0.0;
                acc = s.v[0][0] * s.nrm[0] + acc;
                acc = acc + s.v[0][1] * s.nrm[1];
                acc = acc + s.v[0][2] * s.nrm[2];
                s.tp = acc / s.e;
                s.used_tp = 0;
                if (s.nv == 4) {
                    geo::fun_1404ccf58(&s.v[0][0], g_origin, s.xe, &s.kxe, &s.err);
                    geo::fun_1404d6974(&s.v[0][0], s.p, &s.xe[0], &s.xe[1]);
                    s.h = s.p[2];
                } else {
                    geo::fun_1404e2d44(&s.v[0][0], g_origin, &s.b[0], &s.b[1], &s.b[2], &s.err);
                    std::int32_t three = 3;
                    geo::fun_1404e34eb(&s.v[0][0], s.p, &s.b[0], &s.b[1], &three);
                    if (kBary < std::fabs(s.b[0]) || kBary < std::fabs(s.b[1]) || kBary < std::fabs(s.b[2])) {
                        s.h = s.tp;
                        s.used_tp = 1;
                    } else {
                        s.h = s.p[2];
                    }
                }
                if (s.h < *dn) *dn = s.h;
            }
        next:
            s.i = s.i + 1;
        } while (s.i <= s.nc);
    }
}

// @src 1403539a1  signed travel *d of the travelling body P2 (nodes of its faces F2) along +z
// until it touches the faces F1 of the fixed body P1, plus |tol|; *err = 1 when no node hits.
// With *flag == 1 the travelling body is first moved down so that its top is level with the fixed
// body's bottom (the shift is subtracted again from *d).
void fun_1403539a1(double* p1, double* p2, double* tol, std::int32_t* f1, std::int32_t* f2,
                   std::int32_t* nf1, std::int32_t* nf2, std::int32_t* nn1, std::int32_t* nn2,
                   std::int32_t* work, double* d, std::int32_t* /*sign*/, std::int32_t* err,
                   std::int32_t* flag)
{
    RayState& s = g_ray;
    std::int32_t one = 1;
    s.verbose = 0;
    cube::fun_140225640(&one, &s.verbose);
    std::int32_t id = 1;
    std::int32_t sub = 1;
    std::int32_t offset = 1;
    std::int32_t npp = 4;
    s.type = 1;
    cube::fun_140225ff0(&id, &sub, &s.type, nn1, p1, nf1, &npp, f1, &offset, &s.verbose);
    s.zmin = kNoHit;
    s.zmax = kZmaxStart;
    for (s.k = 1; s.k <= *nn2; ++s.k) {
        const double zk = p2[(s.k - 1) * 3 + 2];
        if (s.zmax <= zk) s.zmax = zk;
        work[s.k - 1] = 0;
    }
    for (s.k = 1; s.k <= *nn1; ++s.k) {
        const double zk = p1[(s.k - 1) * 3 + 2];
        if (zk <= s.zmin) s.zmin = zk;
    }
    s.shift = 0.0;
    if (*flag == 1 && s.zmin < s.zmax) {
        s.shift = s.zmax - s.zmin;
        for (s.k = 1; s.k <= *nn2; ++s.k) p2[(s.k - 1) * 3 + 2] = p2[(s.k - 1) * 3 + 2] - s.shift;
    }
    *d = kNoHit;
    for (s.k = 1; s.k <= *nn2; ++s.k) work[s.k - 1] = 0;
    for (s.f = 1; s.f <= *nf2; ++s.f) {
        for (s.j = 1; s.j <= 4; ++s.j) {
            const std::int32_t v = f2[(s.f - 1) * 4 + (s.j - 1)];
            s.n = v < 0 ? -v : v;
            if (1 <= work[s.n - 1]) continue;
            fun_1403541fc(p2 + static_cast<std::ptrdiff_t>(s.n - 1) * 3, p1, f1, nf1, g_zdir,
                          s.unused40, &s.dn, d);
            if (s.dn < *d) *d = s.dn;
            work[s.n - 1] = 1;
        }
    }
    if (*d == kNoHit) *err = 1;
    *d = (std::fabs(*tol) + *d) - s.shift;  // 140353f59: andpd, addsd, subsd
    cube::fun_140225820(&s.verbose);
}

} // namespace def::cmd::ifd
