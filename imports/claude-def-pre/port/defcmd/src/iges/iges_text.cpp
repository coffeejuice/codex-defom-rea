// DEF_IGS.OUT text route (packet p8-03): read the loop/segment file back (140455e60) and build
// the object's curves from it (14040b100). IGES_READER.md §8; reached only on an allocation
// failure (mode 300) or after the mode-400 re-conversion.
#define _CRT_SECURE_NO_WARNINGS
#include "iges_text.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defcmd/iges.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defrt/stdlib/libm.hpp>

#include "ext_p67.hpp"
#include "iges_ftn.hpp"
#include "ext_p8_01.hpp"
#include "ext_p8_09.hpp"

namespace def::cmd::iges {

void fun_140197d00(def::model::LLOBJ_DATA* obj);
void fun_140197d40(def::model::LLOBJ_DATA* obj);

namespace {
constexpr double kDegRad = 0.017453292519943295; // 140522fd0
constexpr double kEps = 1e-06;                   // 14051f1b8
constexpr std::int32_t kGrow = 2000;             // 140638598 / 14063859c
} // namespace

// @src 140454bc0  empty object (bounding boxes from the zero vector 1406ff040)
void fun_140454bc0(IgsText* t)
{
    t->n_loops = 0;
    t->loops = nullptr;
    t->n_seg = 0;
    t->seg_cap = 0;
    t->segs = nullptr;
    t->n_pts = 0;
    t->pt_cap = 0;
    t->pts = nullptr;
    t->u60 = 0;
    t->u64 = 1;
    t->u6c = 0;
    t->u68 = 0;
    t->u70 = 0;
    t->u74 = 0;
    t->u78 = 0;
    t->items = nullptr;
    t->u7c = 0;
    for (int i = 0; i < 3; ++i) {
        t->bb_min[i] = 0.0f;
        t->bb_max[i] = 0.0f;
        t->bb_min2[i] = 0.0f;
        t->bb_max2[i] = 0.0f;
    }
}

// @src 140454d00  empty loop
void fun_140454d00(IgsLoop* l)
{
    l->n_seg = 0;
    l->seg = nullptr;
    l->f28[0] = 0;
    l->f28[1] = 0;
    l->f28[2] = 1;
    l->f28[3] = 1;
    l->f28[4] = 1;
    l->f28[5] = 1;
    l->f28[6] = 1;
    for (int i = 0; i < 3; ++i) {
        l->bb_min[i] = 0.0f;
        l->bb_max[i] = 0.0f;
    }
}

// @src 140454da0  empty segment (type 1)
void fun_140454da0(IgsSeg* s)
{
    s->b00 = 0;
    s->type = 1;
    s->p1 = 0;
    s->p2 = 0;
    s->pm = 0;
    s->a = 0.0;
}

// @src 140454e00
void fun_140454e00(IgsItem* it)
{
    it->v[0] = 0;
    it->v[1] = 0;
    it->v[2] = 0;
    it->v[3] = 0;
}

// @src 140454e40  free everything, then 140454bc0
void fun_140454e40(IgsText* t)
{
    for (std::int32_t i = 0; i < t->n_loops; ++i) fun_1404553c0(t, i, 4);
    fun_1404553c0(t, 0, 1);
    fun_1404553c0(t, 0, 3);
    fun_1404553c0(t, 0, 2);
    fun_1404553c0(t, 0, 5);
    fun_140454bc0(t);
}

// @src 140454ee0  allocate n entries of a table: 1 points, 2 segments, 3 loops, 4 the segment
// list of loop idx, 5 items
void fun_140454ee0(IgsText* t, std::int32_t n, std::int32_t idx, std::int32_t what)
{
    if (what == 1) {
        fun_1404553c0(t, idx, 1);
        t->pts = static_cast<double*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(n << 1) << 3)));
    } else if (what == 2) {
        fun_1404553c0(t, idx, 2);
        t->segs = static_cast<IgsSeg*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(n) * 0x20)));
        for (std::int32_t i = 0; i < n; ++i) fun_140454da0(&t->segs[i]);
    } else if (what == 3) {
        fun_1404553c0(t, idx, 3);
        t->loops = static_cast<IgsLoop*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(n) * 0x30)));
        for (std::int32_t i = 0; i < n; ++i) fun_140454d00(&t->loops[i]);
    } else if (what == 4) {
        fun_1404553c0(t, idx, 4);
        t->loops[idx].seg = static_cast<std::int32_t*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(n) << 2)));
        for (std::int32_t i = 0; i < n; ++i) t->loops[idx].seg[i] = 0;
    } else if (what == 5) {
        fun_1404553c0(t, idx, 5);
        t->items = static_cast<IgsItem*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(n) * 0x10)));
        for (std::int32_t i = 0; i < n; ++i) fun_140454e00(&t->items[i]);
    } else {
        ext::f14039d450("IGS_AllocSpace : Error in type %d\n", what); // str 1406384f0
    }
}

// @src 140455150  grow a table from old_n to n entries (new entries initialised)
void fun_140455150(IgsText* t, std::int32_t old_n, std::int32_t n, std::int32_t idx, std::int32_t what)
{
    if (what == 1) {
        t->pts = static_cast<double*>(std::realloc(t->pts, static_cast<std::size_t>(static_cast<std::int64_t>(n << 1) << 3)));
        return;
    }
    std::int32_t i = old_n;
    if (what == 2) {
        t->segs = static_cast<IgsSeg*>(std::realloc(t->segs, static_cast<std::size_t>(static_cast<std::int64_t>(n) * 0x20)));
        for (; old_n < n && i < n; ++i) fun_140454da0(&t->segs[i]);
    } else if (what == 3) {
        t->loops = static_cast<IgsLoop*>(std::realloc(t->loops, static_cast<std::size_t>(static_cast<std::int64_t>(n) * 0x30)));
        for (; old_n < n && i < n; ++i) fun_140454d00(&t->loops[i]);
    } else if (what == 4) {
        t->loops[idx].seg = static_cast<std::int32_t*>(
            std::realloc(t->loops[idx].seg, static_cast<std::size_t>(static_cast<std::int64_t>(n) << 2)));
        for (; old_n < n && i < n; ++i) t->loops[idx].seg[i] = 0;
    } else if (what == 5) {
        t->items = static_cast<IgsItem*>(std::realloc(t->items, static_cast<std::size_t>(static_cast<std::int64_t>(n) * 0x10)));
        for (; old_n < n && i < n; ++i) fun_140454e00(&t->items[i]);
    } else {
        ext::f14039d450("IGS_ReAllocSpace : Error in type %d\n", what); // str 140638518
    }
}

// @src 1404553c0  free a table (what as in 140454ee0)
void fun_1404553c0(IgsText* t, std::int32_t idx, std::int32_t what)
{
    if (what == 1) {
        if (t->pts != nullptr) std::free(t->pts);
        t->pts = nullptr;
    } else if (what == 2) {
        if (t->segs != nullptr) std::free(t->segs);
        t->segs = nullptr;
    } else if (what == 3) {
        if (t->loops != nullptr) std::free(t->loops);
        t->loops = nullptr;
    } else if (what == 4) {
        if (t->loops[idx].seg != nullptr) std::free(t->loops[idx].seg);
        t->loops[idx].seg = nullptr;
    } else if (what == 5) {
        if (t->items != nullptr) std::free(t->items);
        t->items = nullptr;
    } else {
        ext::f14039d450("IGS_ReAllocSpace : Error in type %d\n", what); // str 140638540
    }
}

// @src 1404557a0  bounding box of all points, kept twice
void fun_1404557a0(IgsText* t)
{
    fun_14046a810(t->pts, t->n_pts, t->bb_min, t->bb_max);
    std::memcpy(t->bb_min2, t->bb_min, 12);
    std::memcpy(t->bb_max2, t->bb_max, 12);
}

// @src 140455aa0  bounding box of every loop (all != 0) or of loop `start`
void fun_140455aa0(IgsText* t, std::int32_t start, std::int32_t all)
{
    bool first = true;
    const double* pts = t->pts;
    const IgsSeg* segs = t->segs;
    std::int32_t i = 0;
    for (;;) {
        if (t->n_loops <= i) return;
        if (all == 0) {
            if (!first) return;
            first = false;
            i = start;
        }
        IgsLoop* l = &t->loops[i];
        double* buf = static_cast<double*>(def::model::llobj_copy::calloc_double(l->n_seg << 2));
        std::int32_t j = 0;
        for (std::int32_t s = 0; s < l->n_seg; ++s) {
            const IgsSeg& g = segs[l->seg[s]];
            buf[j * 2] = pts[g.p1 * 2];
            buf[j * 2 + 1] = pts[g.p1 * 2 + 1];
            buf[(j + 1) * 2] = pts[g.p2 * 2];
            buf[(j + 1) * 2 + 1] = pts[g.p2 * 2 + 1];
            j = j + 2;
        }
        if (0 < l->n_seg) fun_14046a810(buf, j, l->bb_min, l->bb_max);
        if (buf != nullptr) std::free(buf);
        i = i + 1;
    }
}

// @src 140455e60  read DEF_IGS.OUT: "%d\n" loops, per loop "%d %d\n" (number, segments), per
// segment "%d %s %s %s %s %s %s\n" (index, type, x1, y1, x2, y2, angle in degrees)
std::int32_t fun_140455e60(IgsText* t, const char* name)
{
    std::FILE* f = std::fopen(name, "rt"); // str 140638570
    if (f == nullptr) return -1;
    if (t == nullptr) return -2;
    fun_140454e40(t);
    std::int32_t nl = 0;
    (void)std::fscanf(f, "%d\n", &nl);
    fun_140454ee0(t, nl, 0, 3);
    fun_140454ee0(t, 2000, 0, 2); // 14063856c
    fun_140454ee0(t, 2000, 0, 1); // 140638568
    fun_140454ee0(t, 2000, 0, 5);
    t->n_pts = 0;
    t->n_seg = 0;
    t->pt_cap = 2000;
    t->seg_cap = 2000;
    std::int32_t merged = 0; // local_2d8 (not used further)
    for (std::int32_t l = 0; l < nl; ++l) {
        std::int32_t dummy = 0, ns = 0;
        (void)std::fscanf(f, "%d %d\n", &dummy, &ns);
        t->loops[l].n_seg = ns;
        fun_140454ee0(t, ns, l, 4);
        for (std::int32_t s = 0; s < ns; ++s) {
            char ty[128] = {}, sx1[128] = {}, sy1[128] = {}, sx2[128] = {}, sy2[128] = {}, sa[128] = {};
            (void)std::fscanf(f, "%d %s %s %s %s %s %s\n", &dummy, ty, sx1, sy1, sx2, sy2, sa);
            const double type = std::atof(ty);
            const double x1 = std::atof(sx1);
            const double y1 = std::atof(sy1);
            const double x2 = std::atof(sx2);
            const double y2 = std::atof(sy2);
            const double a = std::atof(sa);
            const double p1[2] = {x1, y1};
            std::int32_t i1 = fun_1404564c0(t, p1);
            if (i1 == 0) {
                i1 = t->n_pts;
            } else {
                merged = merged + 1;
            }
            const double p2[2] = {x2, y2};
            std::int32_t i2 = fun_1404564c0(t, p2);
            if (i2 == 0) {
                i2 = t->n_pts;
            } else {
                merged = merged + 1;
            }
            double ar = a * kDegRad;
            if (std::fabs(ar) < kEps) ar = 6.283185307179586; // 140567488
            std::int32_t im;
            if (type == 2.0) {
                double mid[2];
                fun_14046b2d0(p1, p2, &ar, mid);
                im = fun_1404564c0(t, mid);
                if (im == 0) {
                    im = t->n_pts;
                } else {
                    merged = merged + 1;
                }
            } else {
                im = 0;
            }
            fun_1404565c0(t, ftn_trunc(type), i1 - 1, i2 - 1, im - 1, a);
            t->loops[l].seg[s] = t->n_seg - 1;
        }
    }
    (void)merged;
    std::fclose(f);
    t->n_loops = nl;
    fun_140455150(t, t->pt_cap, t->n_pts, 0, 1);
    fun_140455150(t, t->seg_cap, t->n_seg, 0, 2);
    t->pt_cap = t->n_pts;
    t->seg_cap = t->n_seg;
    fun_1404557a0(t);
    fun_140455aa0(t, 0, 1);
    fun_140456720(t, 1);
    return 0;
}

// @src 1404564c0  index + 1 of an equal point (14046aa40, 1e-6), or 0 after appending p
std::int32_t fun_1404564c0(IgsText* t, const double* p)
{
    const std::int32_t i = fun_14046aa40(t->pts, t->n_pts, p, kEps);
    if (i == 0) {
        if (t->n_pts == t->pt_cap) {
            t->pt_cap = t->pt_cap + kGrow;
            fun_140455150(t, t->n_pts, t->pt_cap, 0, 1);
        }
        t->pts[t->n_pts * 2] = p[0];
        t->pts[t->n_pts * 2 + 1] = p[1];
        t->n_pts = t->n_pts + 1;
    }
    return i;
}

// @src 1404565c0  append a segment (lines: pm = 0, a = 0)
void fun_1404565c0(IgsText* t, std::int32_t type, std::int32_t p1, std::int32_t p2, std::int32_t pm, double a)
{
    const std::int32_t n = t->n_seg;
    if (t->n_seg == t->seg_cap) {
        t->seg_cap = t->seg_cap + kGrow;
        fun_140455150(t, t->n_seg, t->seg_cap, 0, 2);
    }
    t->segs[n].type = type;
    t->segs[n].p1 = p1;
    t->segs[n].p2 = p2;
    if (type == 1) {
        t->segs[n].pm = 0;
        t->segs[n].a = 0.0;
    } else {
        t->segs[n].pm = pm;
        t->segs[n].a = a;
    }
    t->n_seg = t->n_seg + 1;
}

// @src 140456720  -> 140456770, 140456790, 1404567a0 (all empty)
void fun_140456720(IgsText* t, std::int32_t v)
{
    fun_140456770(t, v);
    fun_140456790(t, v);
    fun_1404567a0(t, v);
}
// @src 140456770  empty
void fun_140456770(IgsText*, std::int32_t) {}
// @src 140456790  empty
void fun_140456790(IgsText*, std::int32_t) {}
// @src 1404567a0  empty
void fun_1404567a0(IgsText*, std::int32_t) {}

// @src 14046a810  float bounding box of n points (x, y); the third float of each box is not
// written by the original (stack garbage), 0 here
void fun_14046a810(const double* p, std::int32_t n, float* bb_min, float* bb_max)
{
    float minx = 0.0f, miny = 0.0f, maxx = 0.0f, maxy = 0.0f;
    for (std::int32_t i = 0; i < n; ++i) {
        const double x = p[i * 2];
        const double y = p[i * 2 + 1];
        if (i == 0) {
            maxx = static_cast<float>(x);
            minx = static_cast<float>(x);
            maxy = static_cast<float>(y);
            miny = static_cast<float>(y);
        } else {
            minx = static_cast<float>(!(x > static_cast<double>(minx)) ? x : static_cast<double>(minx));
            miny = static_cast<float>(!(y > static_cast<double>(miny)) ? y : static_cast<double>(miny));
            maxx = static_cast<float>(!(static_cast<double>(maxx) > x) ? x : static_cast<double>(maxx));
            maxy = static_cast<float>(!(static_cast<double>(maxy) > y) ? y : static_cast<double>(maxy));
        }
    }
    bb_min[0] = minx;
    bb_min[1] = miny;
    bb_min[2] = 0.0f;
    bb_max[0] = maxx;
    bb_max[1] = maxy;
    bb_max[2] = 0.0f;
}

// @src 14046aa40  index + 1 of the first point within tol of q (|dx|, |dy| and the distance), else 0
std::int32_t fun_14046aa40(const double* p, std::int32_t n, const double* q, double tol)
{
    for (std::int32_t i = 0; i < n; ++i) {
        const double dx = std::fabs(p[i * 2] - q[0]);     // 140484558
        const double dy = std::fabs(p[i * 2 + 1] - q[1]);
        if (dx <= tol && dy <= tol && std::sqrt(dx * dx + dy * dy) <= tol) return i + 1; // 140484644
    }
    return 0;
}

// @src 14046b2d0  -> 140476bb1
void fun_14046b2d0(const double* p, const double* c, const double* a, double* out) { fun_140476bb1(p, c, a, out); }

// @src 140476bb1  rotate p about c by *a (sincos 1404b3b30); outside the ledger, needed here
void fun_140476bb1(const double* p, const double* c, const double* a, double* out)
{
    static double d[2] = {}, sc[2] = {}; // 14072df70, 14072df80
    const double cy = c[1];
    const double cx = c[0];
    d[0] = p[0] - c[0];
    d[1] = p[1] - cy;
    double s = 0.0, co = 0.0;
    def::rt::libm::ps_sincos(*a, &s, &co);
    sc[1] = co;
    sc[0] = s;
    const double x3 = d[0] * s + cy;
    out[1] = d[1] * co + x3;
    out[0] = (co * d[0] + cx) - s * d[1];
}

// @src 14040b100  curves from the text object DAT_14070c060: one curve per loop (or all loops
// appended to curve 0 when iges+0x18 != 0); rows [type, x1, y1, x2, y2, a*pi/180]
std::int32_t fun_14040b100(Uic2CObjectIgesFile* iges, def::model::LLOBJ_DATA* obj, std::int32_t)
{
    using def::model::OBJ_GEO2D;
    OBJ_GEO2D tmp; // local_b0 (ctor 140275cd0, dtor 140275e00)
    std::int32_t first_done = 0; // local_b8
    IgsText* t = static_cast<IgsText*>(g_iges_text);
    if (obj == nullptr) return -1;
    if (t == nullptr) return -1;
    if (t->n_loops < 1) return -16;
    if (t->n_seg < 1) return -17;
    if (t->n_pts < 1) return -18;
    if (t->loops == nullptr) return -19;
    if (t->segs == nullptr) return -20;
    if (t->pts == nullptr) return -21;
    if (t->segs[0].type != 1 && t->segs[0].type != 2) return -22;
    const std::int32_t nl = t->n_loops;
    if (!(nl < 0x65 || iges->u18 != 0)) {
        fun_140197d40(obj);
        fun_140197d00(obj);
        return -24;
    }
    fun_140197d00(obj);
    tmp.clear_all();
    ext::f14040ce60(iges, nl);
    if (iges->u18 == 0) {
        ext::f140198230(obj, nl);
    } else {
        ext::f140198230(obj, 1);
        ext::f14019a650(obj, nl);
    }
    for (std::int32_t l = 0; l < nl; ++l) {
        OBJ_GEO2D* c;
        if (iges->u18 == 0) {
            c = def::model::geo_kw::curve(*obj, l);
        } else if (first_done == 0) {
            c = def::model::geo_kw::curve(*obj, 0);
            first_done = 1;
        } else {
            tmp.clear_all();
            c = &tmp;
        }
        if (c == nullptr) continue;
        const IgsLoop& lp = t->loops[l];
        const std::int32_t ns = lp.n_seg;
        if (ns < 1) continue;
        c->curve_type = 2;
        c->n_points = ns;
        c->allocate_points();
        double* pts = c->points;
        std::int32_t j = 0;
        for (std::int32_t s = 0; s < ns; ++s) {
            IgsSeg& g = t->segs[lp.seg[s]];
            const std::int32_t p1 = g.p1;
            const std::int32_t p2 = g.p2;
            if (std::fabs(g.a) < kEps && g.type == 2) g.a = 360.0; // 14051f1b8, 14051f210
            const double a = g.a;
            pts[j] = static_cast<double>(g.type);
            pts[j + 1] = t->pts[p1 * 2];
            pts[j + 2] = t->pts[p1 * 2 + 1];
            pts[j + 3] = t->pts[p2 * 2];
            pts[j + 4] = t->pts[p2 * 2 + 1];
            pts[j + 5] = a * kDegRad;
            j = j + 6;
        }
        if (iges->u18 != 0 && first_done != 0) {
            def::model::geo_kw::curve(*obj, 0)->append(&tmp); // 140276e30
            tmp.clear_all();
        }
    }
    return 0;
}

} // namespace def::cmd::iges
