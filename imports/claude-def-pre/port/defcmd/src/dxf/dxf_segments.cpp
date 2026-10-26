// defcmd/src/dxf/dxf_segments.cpp: entity records -> segment array, OCS transform, the size /
// tolerance pass and the parse driver (p8-04). Spec: docs/re/commands/DXF_READER.md sections 3,
// 7, 8. All C++ double math in the binary's order (unoptimised MSVC code: one operation per
// source operation, left to right).
#define _CRT_SECURE_NO_WARNINGS
#include "dxf_internal.hpp"

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <share.h>

#include <defcmd/dxf.hpp>
#include <defcmd/globals_p8_04.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defrt/stdlib/libm.hpp>

#include "ext.hpp"

namespace def::cmd::dxf {

using def::model::OBJ_GEO2D;

namespace {
constexpr double k_deg2rad = 0.017453292519943295;  // DAT_140522fd0
constexpr double k_rad2deg = 57.29577951308232;     // DAT_140522fd8
constexpr double k_eps8 = 1e-8;                     // DAT_1405691a8
constexpr double k_eps6 = 1e-6;                     // DAT_14051f1b8
constexpr double k_size_to_tol = 0.01;              // DAT_14051f258

// 140484644 sqrt: SQRTSD for finite x >= 0 (specials through _handle_error, not reached: the
// arguments are sums of squares), so std::sqrt is identical; 140484558 fabs: sign cleared
double crt_sqrt(double x) { return std::sqrt(x); }
double crt_fabs(double x) { return std::fabs(x); }
} // namespace

// @src 1404099c0  G
double fun_1404099c0() { return dat_14062bc68; }
// @src 140428f70  G = v
void fun_140428f70(double v) { dat_14062bc68 = v; }

void reset_globals()
{
    dat_14062bc68 = 1e-9;
    conv_ctx() = ConvContext{};
}

// @src 140428850  vector (x, y, z)
static Vec3 fun_140428850(double x, double y, double z) { return Vec3{x, y, z}; }

// @src 1404289a0  length: sqrt((x*x + y*y) + z*z)
static double fun_1404289a0(const Vec3* v) { return crt_sqrt(v->x * v->x + v->y * v->y + v->z * v->z); }

// @src 1404288c0  normalised copy (unchanged when the length is 0)
static Vec3 fun_1404288c0(const Vec3* v)
{
    Vec3 r = *v;
    const double len = fun_1404289a0(v);
    if (len == 0.0) return r;
    r.x = r.x / len;
    r.y = r.y / len;
    r.z = r.z / len;
    return r;
}

// @src 140428a10  cross product a x b
static Vec3 fun_140428a10(const Vec3* a, const Vec3* b)
{
    Vec3 r;
    r.x = a->y * b->z - a->z * b->y;
    r.y = a->z * b->x - a->x * b->z;
    r.z = a->x * b->y - a->y * b->x;
    return r;
}

// @src 140427f90  extrusion is (0, 0, 1) or (0, 0, 0): no transform
static std::int32_t fun_140427f90(const double* e)
{
    if (e[0] == 0.0 && e[1] == 0.0 && e[2] == 1.0) return 1;
    if (e[0] == 0.0 && e[1] == 0.0 && e[2] == 0.0) return 1;
    return 0;
}

// @src 140428140  arbitrary-axis algorithm with threshold G: Ax, Ay and the normalised N
static std::int32_t fun_140428140(const double* ext, Vec3* ax, Vec3* ay, Vec3* n_out)
{
    Vec3 n = fun_140428850(ext[0], ext[1], ext[2]);
    n = fun_1404288c0(&n);
    const Vec3 wy = fun_140428850(0.0, 1.0, 0.0);
    const Vec3 wz = fun_140428850(0.0, 0.0, 1.0);
    Vec3 a;
    const double g = fun_1404099c0();
    if (g <= crt_fabs(n.x) || g <= crt_fabs(n.y)) {
        a = fun_140428a10(&wz, &n);
    } else {
        a = fun_140428a10(&wy, &n);
    }
    a = fun_1404288c0(&a);
    Vec3 b = fun_140428a10(&n, &a);
    b = fun_1404288c0(&b);
    *ax = a;
    *ay = b;
    *n_out = n;
    return 1;
}

// @src 140428510  p = (x*Ax.x + y*Ax.y, x*Ay.x + y*Ay.y)
static std::int32_t fun_140428510(double* p, const Vec3* ax, const Vec3* ay)
{
    const double x = p[0];
    const double ayx = ay->x;
    const double ayy = ay->y;
    p[0] = p[0] * ax->x + p[1] * ax->y;
    p[1] = x * ayx + p[1] * ayy;
    return 1;
}

// @src 1404285b0  angle (degrees) through the transform: atan2 of the transformed unit vector
static std::int32_t fun_1404285b0(double* a, const Vec3* ax, const Vec3* ay)
{
    const double r = *a * k_deg2rad;
    double cs[2];
    cs[0] = def::rt::libm::crt_cos(r);  // 140bb3490
    cs[1] = def::rt::libm::crt_sin(r);  // 140bb3000
    fun_140428510(cs, ax, ay);
    *a = ext::crt_atan2(cs[1], cs[0]) * k_rad2deg;  // 140485d6c
    return 1;
}

// @src 140427f00  transform both points of a line-shaped record {p1, p2, ext} by its extrusion
static std::int32_t fun_140427f00(LineRec* r)
{
    if (fun_140427f90(r->ext) == 0) {
        Vec3 ax, ay, n;
        fun_140428140(r->ext, &ax, &ay, &n);
        fun_140428510(&r->x1, &ax, &ay);
        fun_140428510(&r->x2, &ax, &ay);
    }
    return 1;
}

// @src 140428030  transform an arc record: centre, both angles, then order them
static std::int32_t fun_140428030(ArcRec* r)
{
    if (fun_140427f90(r->ext) == 0) {
        Vec3 ax, ay, n;
        fun_140428140(r->ext, &ax, &ay, &n);
        fun_140428510(&r->cx, &ax, &ay);
        fun_1404285b0(&r->a50, &ax, &ay);
        fun_1404285b0(&r->a51, &ax, &ay);
        if (r->a51 <= r->a50 && r->a50 != r->a51) {
            const double t = r->a51;
            r->a51 = r->a50;
            r->a50 = t;
        }
    }
    return 1;
}

// @src 140422d40  zeroed n*6 doubles (0 when n < 1)
double* fun_140422d40(std::int32_t n)
{
    if (n < 1) return nullptr;
    return def::model::geo2d_alloc_zeroed(n * 6);  // 140275fa0
}

// @src 140422d90  free the segment array
void fun_140422d90(double* p)
{
    if (p != nullptr) def::model::geo2d_free(p);  // 140276060
}

// @src 1404231c0  segment array from the entity records: lines, polylines (with bulge arcs and
// the closing edge), arcs. *n_seg = segment count, *n_pts = point count (2 per line, 3 per arc).
std::int32_t fun_1404231c0(std::int32_t* n_seg, std::int32_t* n_pts, double** data, Work* w)
{
    std::int32_t rc = 0;
    *n_pts = 0;
    *n_seg = 0;
    *data = nullptr;
    std::int32_t n = w->n_lines + w->n_arcs;
    for (int i = 0; i < w->n_polys; ++i) {
        const PolyRec* p = &w->polys[i];
        n = n + -1 + p->n_vert;
        if (p->flag != 0 && 2 < p->n_vert) n = n + 1;
    }
    if (n < 1) return -500;
    double* buf = static_cast<double*>(std::calloc(static_cast<std::size_t>(n * 6), 8));
    if (buf == nullptr) return -1;
    int k = 0;
    // lines
    for (int i = 0; i < w->n_lines; ++i) {
        LineRec* r = &w->lines[i];
        fun_140427f00(r);
        buf[k] = 1.0;  // DAT_140509c90
        buf[k + 1] = r->x1;
        buf[k + 2] = r->y1;
        buf[k + 3] = r->x2;
        buf[k + 4] = r->y2;
        buf[k + 5] = 0.0;
        k += 6;
        *n_pts = *n_pts + 1;
        *n_pts = *n_pts + 1;
    }
    // polylines
    for (int i = 0; i < w->n_polys; ++i) {
        const PolyRec* p = &w->polys[i];
        for (int b = 1; b < p->n_vert; ++b) {
            const Vertex* v0 = &p->v[b - 1];
            const Vertex* v1 = &p->v[b];
            if (v0->bulge == 0.0) {
                LineRec t;
                t.x1 = v0->x;
                t.y1 = v0->y;
                t.x2 = v1->x;
                t.y2 = v1->y;
                std::memcpy(t.ext, p->ext, sizeof t.ext);
                fun_140427f00(&t);
                buf[k] = 1.0;
                buf[k + 1] = t.x1;
                buf[k + 2] = t.y1;
                buf[k + 3] = t.x2;
                buf[k + 4] = t.y2;
                buf[k + 5] = 0.0;
                k += 6;
                *n_pts = *n_pts + 1;
                *n_pts = *n_pts + 1;
            } else {
                double th = ext::crt_atan(v0->bulge);  // 1404865ac
                th = 4.0 * th;                         // DAT_14051f248
                const double mx = (v1->x + v0->x) / 2.0;  // DAT_140509fb8
                const double my = (v1->y + v0->y) / 2.0;
                const double a = mx - v1->x;
                const double e = my - v1->y;
                const double h = crt_sqrt(a * a + e * e);
                if (h != 0.0 && ext::crt_tan(th / 2.0) != 0.0) {
                    double d = ext::crt_tan(th / 2.0);  // 140487b70
                    d = h / d;
                    const double ox = (e * d) / h;
                    const double oy = (a * d) / h;
                    LineRec t;
                    t.x2 = mx + ox;
                    t.y2 = my - oy;
                    t.x1 = v0->x;
                    t.y1 = v0->y;
                    std::memcpy(t.ext, p->ext, sizeof t.ext);
                    fun_140427f00(&t);
                    buf[k] = 2.0;
                    buf[k + 1] = t.x1;
                    buf[k + 2] = t.y1;
                    buf[k + 3] = t.x2;
                    buf[k + 4] = t.y2;
                    if (0.0 <= th) {
                        buf[k + 1] = v0->x;
                        buf[k + 2] = v0->y;
                        buf[k + 3] = t.x2;
                        buf[k + 4] = t.y2;
                        buf[k + 5] = th;
                    } else {
                        buf[k + 1] = v1->x;
                        buf[k + 2] = v1->y;
                        buf[k + 3] = t.x2;
                        buf[k + 4] = t.y2;
                        buf[k + 5] = -th;  // xor with the sign bit (DAT_14051bec0)
                    }
                    k += 6;
                    *n_pts = *n_pts + 1;
                    *n_pts = *n_pts + 1;
                    *n_pts = *n_pts + 1;
                }
            }
        }
        if (p->flag != 0 && 2 < p->n_vert) {
            if (std::memcmp(&p->v[p->n_vert - 1], &p->v[0], sizeof(Vertex)) == 0) {
                n = n + -1;
            } else {
                LineRec t;
                t.x1 = p->v[p->n_vert - 1].x;
                t.y1 = p->v[p->n_vert - 1].y;
                t.x2 = p->v[0].x;
                t.y2 = p->v[0].y;
                std::memcpy(t.ext, p->ext, sizeof t.ext);
                fun_140427f00(&t);
                const double dx = t.x2 - t.x1;
                const double dy = t.y2 - t.y1;
                if (k_eps8 < dx || k_eps8 < dy) {
                    buf[k] = 1.0;
                    buf[k + 1] = t.x1;
                    buf[k + 2] = t.y1;
                    buf[k + 3] = t.x2;
                    buf[k + 4] = t.y2;
                    buf[k + 5] = 0.0;
                    k += 6;
                    *n_pts = *n_pts + 1;
                    *n_pts = *n_pts + 1;
                } else {
                    n = n + -1;
                }
            }
        }
    }
    // arcs
    for (int i = 0; i < w->n_arcs; ++i) {
        ArcRec* r = &w->arcs[i];
        if (r->r <= 0.0) {
            n = n + -1;
            continue;
        }
        fun_140428030(r);
        const double s = r->a50 * k_deg2rad;
        double sw = r->a51 - r->a50;
        if (sw < 0.0) sw = sw + 360.0;  // DAT_14051f210
        sw = sw * k_deg2rad;
        buf[k] = 2.0;
        double c = def::rt::libm::crt_cos(s);  // 140bb3490
        buf[k + 1] = r->cx + r->r * c;
        c = def::rt::libm::crt_sin(s);         // 140bb3000
        buf[k + 2] = r->cy + r->r * c;
        buf[k + 3] = r->cx;
        buf[k + 4] = r->cy;
        buf[k + 5] = sw;
        k += 6;
        *n_pts = *n_pts + 1;
        *n_pts = *n_pts + 1;
        *n_pts = *n_pts + 1;
    }
    *n_seg = n;
    double* out = fun_140422d40(*n_seg + 2);
    rc = 0;
    if (out == nullptr) {
        rc = -1;
    } else {
        std::memcpy(out, buf, static_cast<std::size_t>(n * 0x30));
    }
    std::free(buf);
    *data = out;
    return rc;
}

// @src 140277420  _OBJ_GEO2D: n segment rows (curve type 2) copied from data
std::int64_t fun_140277420(OBJ_GEO2D* g, std::int32_t n, const double* data)
{
    if (n < 1) return 0;
    if (data == nullptr) return 0;
    g->n_points = n;
    g->curve_type = 2;
    double* dst = g->allocate_points();  // 140275f30
    if (dst == nullptr) return -1;
    const std::int32_t dof = g->dof();   // 1402760e0
    std::memcpy(dst, data, static_cast<std::size_t>(g->n_points * dof * 8));
    return 0;
}

// @src 1403fb460  pack two floats (0x0c-byte result; the third float is not written)
Vec2f fun_1403fb460(float x, float y)
{
    Vec2f r;
    r.x = x;
    r.y = y;
    r.z = 0.0f;
    return r;
}

// @src 1403078f0  end point of an arc in float: rotate start about centre by angle (radians, or
// degrees when `degrees`), Fortran rotation 14043658f
Vec2f fun_1403078f0(Vec2f start, Vec2f center, float angle, std::int32_t degrees)
{
    double p[2] = {static_cast<double>(start.x), static_cast<double>(start.y)};
    double c[2] = {static_cast<double>(center.x), static_cast<double>(center.y)};
    double a;
    if (degrees == 0) {
        a = static_cast<double>(angle);
    } else {
        a = static_cast<double>(angle) * k_deg2rad;
    }
    double out[2];
    def::rt::ftn::rotate2d(p, c, &a, out);
    return fun_1403fb460(static_cast<float>(out[0]), static_cast<float>(out[1]));
}

// @src 14027ba50  _OBJ_GEO2D: smallest relevant squared distance (> 1e-6) -> its square root
// in *size (0 when none). Curve type 1: consecutive points; type 2: the segment walk below, in
// which "prev" always comes from the LAST segment.
std::int64_t fun_14027ba50(OBJ_GEO2D* g, double* size)
{
    bool first = true;
    double m = 0.0;
    *size = 0.0;
    const std::int32_t dof = g->dof();
    if (g->n_points < 1 || g->points == nullptr) return 0;
    const double* P = g->points;
    auto upd = [&](double d) {
        if (!first || d <= k_eps6) {
            if (d < m && k_eps6 < d) m = d;
        } else {
            first = false;
            m = d;
        }
    };
    if (g->curve_type == 1) {
        for (int i = 1; i < g->n_points; ++i) {
            const double a = P[(i - 1) * dof] - P[i * dof];
            const double b = P[(i - 1) * dof + 1] - P[i * dof + 1];
            upd(a * a + b * b);
        }
    } else {
        const double* last = P + (g->n_points - 1) * dof;
        double px, py;
        auto arc_end_of_last = [&]() {
            const Vec2f s = fun_1403fb460(static_cast<float>(last[1]), static_cast<float>(last[2]));
            const Vec2f c = fun_1403fb460(static_cast<float>(last[3]), static_cast<float>(last[4]));
            return fun_1403078f0(s, c, static_cast<float>(last[5]), 0);
        };
        if (static_cast<std::int32_t>(last[0]) == 1) {
            px = last[3];
            py = last[4];
        } else {
            const Vec2f e = arc_end_of_last();
            px = static_cast<double>(e.x);
            py = static_cast<double>(e.y);
        }
        for (int i = 0; i < g->n_points; ++i) {
            const double* s = P + i * dof;
            if (static_cast<std::int32_t>(s[0]) == 1) {
                px = px - s[1];
                py = py - s[2];
                upd(px * px + py * py);
                const double c = s[1] - s[3];
                double d = s[2] - s[4];
                d = d * d;
                upd(c * c + d);
                px = last[3];
                py = last[4];
            } else {
                const Vec2f e = arc_end_of_last();
                px = px - s[1];
                py = py - s[2];
                upd(px * px + py * py);
                const double c = s[1] - static_cast<double>(e.x);
                double d = s[2] - static_cast<double>(e.y);
                d = d * d;
                upd(c * c + d);
                px = static_cast<double>(e.x);
                py = static_cast<double>(e.y);
            }
        }
    }
    if (0.0 < m) *size = crt_sqrt(m);
    return 1;
}

// @src 140422ab0  open, parse, build the segments, then (always after the open) the size pass and
// G = 0.01 * size. Returns 0, -100, -200, -300, -1, -400, -10000, -500.
std::int32_t fun_140422ab0(const char* name, std::int32_t* n_pts, std::int32_t* n_seg, double** data)
{
    *n_pts = 0;
    *n_seg = 0;
    *data = nullptr;
    if (name == nullptr) return -100;
    if (static_cast<int>(std::strlen(name)) < 1) return -200;
    if (fun_140428b10(name) == 0) return -300;
    Work* w = fun_140422f10(0);
    if (w == nullptr) return -1;
    std::FILE* f = _fsopen(name, "rb", _SH_DENYNO);  // 14047b934, str 140569298
    if (f == nullptr) return -400;  // (the work struct is not freed here, as in the original)
    std::int32_t rc = fun_140422f70(f, w);
    std::fclose(f);
    if (rc == 0) rc = fun_1404231c0(n_seg, n_pts, data, w);
    double size = 0.0;
    {
        OBJ_GEO2D geo;  // 140275cd0
        fun_140277420(&geo, *n_seg, *data);
        fun_14027ba50(&geo, &size);
        const double g = size * k_size_to_tol;
        fun_140428f70(g);
        fun_140424390(&w);
    }  // 140275e00
    return rc;
}

} // namespace def::cmd::dxf
