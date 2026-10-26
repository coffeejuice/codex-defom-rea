// FIXGEO clean-up of a curve's digitized profile and the PNTGEO.GEO read-back (packet p9-03a).
// Path: 140153c00 -> 1402778c0 -> 14041e270 -> 14027ece0 (FIXGEO, owner p9-01), 14041e4d0,
// 14041ea80, 14045a040. FIXGEO itself: DXF_IGES_READERS.md 5.4 (called here with delPnt 0, so
// FIXGEO.EXE's PNTGEO.GEO stays for 14041e4d0, which 14045a040 then deletes).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cnvt3d_sweep.hpp>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defkey/crt_scan.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo2d_digitized.hpp>

#include "internal.hpp"

namespace def::cmd::cnvt3d_sweep {

using namespace detail;
using model::OBJ_GEO2D;
using model::OBJ_GEO2D_DIGITIZED;

// @src 14041f0f0  PTR_s_PNTGEO_GEO_140639798
const char* fun_14041f0f0() { return "PNTGEO.GEO"; }
// @src 14041f100  PTR_DAT_1406397a8 -> "rt"
const char* fun_14041f100() { return "rt"; }

// @src 14041e1f0  sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)) (140484644)
double fun_14041e1f0(double x1, double y1, double x2, double y2)
{
    const double dx = (x2 - x1) * (x2 - x1);
    const double dy = (y2 - y1) * (y2 - y1);
    return std::sqrt(dx + dy);
}

// @src 1402778c0
void fun_1402778c0(OBJ_GEO2D* g) { fun_14041e270(g->ext, 0, 1); }

// @src 14041e270
std::int32_t fun_14041e270(OBJ_GEO2D_DIGITIZED* d, std::int32_t code, char close)
{
    const char* name = fun_14041f0f0();
    FixgeoPar par;  // 140220390
    OBJ_GEO2D* g = new OBJ_GEO2D();  // 1404797d8(0x30), 140275cd0
    g->assign(d->owner);             // 140276140
    par.tol_a = d->u18;
    par.angle_a = d->u20;
    par.angle_b = d->u28;
    par.b28 = d->u30 != 0;
    par.b29 = d->u34 != 0;
    std::int32_t out = 0;  // local_70
    std::int32_t r = E().fun_14027ece0(g, &par, &out, 0, 0);
    std::int32_t saved = 0;  // local_6c, never read
    if (code == 0x17) saved = r;
    (void)saved;
    if (r == 0 || r == 1) {
        r = fun_14041e4d0(d);
        if (r == 0 && close != 0) r = fun_14041ea80(d);
    }
    fun_14045a040(name);
    delete g;  // slot 0, flag 1
    return r;  // ~FixgeoPar: 140220420
}

// @src 14041e4d0  PNTGEO.GEO: "1", then "n m", then n lines "%d %Flf %Flf" (id, x, y)
std::int32_t fun_14041e4d0(OBJ_GEO2D_DIGITIZED* d)
{
    std::FILE* f = std::fopen(fun_14041f0f0(), fun_14041f100());
    if (f == nullptr) return -10;
    char buf[1024];
    if (std::fgets(buf, 0x400, f) == nullptr) {
        std::fclose(f);
        return -11;
    }
    std::int32_t kind = 0;
    if (key::crt::sscanf(buf, "%d", &kind) != 1) {
        std::fclose(f);
        return -12;
    }
    if (kind != 1) {
        std::fclose(f);
        return -13;
    }
    if (std::fgets(buf, 0x400, f) == nullptr) {
        std::fclose(f);
        return -14;
    }
    std::int32_t n = 0;
    std::int32_t second = 0;  // local_14, unused
    if (key::crt::sscanf(buf, "%d %d", &n, &second) != 2) {
        std::fclose(f);
        return -15;
    }
    if (n < 1) {
        std::fclose(f);
        return -16;
    }
    d->n_points = n;
    double* p = d->alloc_points();  // 14041dfa0
    if (p == nullptr) {
        std::fclose(f);
        return -1;
    }
    std::int32_t r = 0;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        if (std::fgets(buf, 0x400, f) == nullptr) {
            r = -17;
            break;
        }
        std::int32_t id = 0;
        double x = 0.0;
        double y = 0.0;
        if (key::crt::sscanf(buf, "%d %Flf %Flf", &id, &x, &y) != 3) {
            r = -18;
            break;
        }
        p[0] = x;
        p[1] = y;
        p = p + 2;
    }
    std::fclose(f);
    return r;
}

// @src 14041ea80  close the polygon: when |tol| is not >= the last-to-first distance (COMISD |tol|,
//                 dist; JC: a NaN distance also appends), append the first point
std::int32_t fun_14041ea80(OBJ_GEO2D_DIGITIZED* d)
{
    if (0 < d->n_points && d->points != nullptr) {
        const double x0 = d->points[0];
        const double y0 = d->points[1];
        const std::int32_t n = d->n_points;
        const double* last = d->points + n * 2;
        const double dist = fun_14041e1f0(last[-2], last[-1], x0, y0);
        if (!(std::fabs(d->u18) >= dist)) {
            d->n_points = d->n_points + 1;
            const std::int32_t m = d->n_points * 2;
            double* q = static_cast<double*>(std::malloc(static_cast<std::size_t>(m) * 8));  // 1404797d8
            if (q == nullptr) std::abort();
            std::memcpy(q, d->points, static_cast<std::size_t>(n * 16));
            d->free_points();  // 14041df50
            d->points = q;
            q[m - 1] = y0;
            q[m - 2] = x0;
        }
    }
    return 0;
}

// @src 14045a040  open-test (mode 14041f100), then remove (0x14047e2cc)
std::int32_t fun_14045a040(const char* name)
{
    std::FILE* f = std::fopen(name, fun_14041f100());
    if (f == nullptr) return 1;
    std::fclose(f);
    return std::remove(name) == 0 ? 1 : 0;
}

} // namespace def::cmd::cnvt3d_sweep
