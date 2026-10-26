// defcmd/src/ifd/d2/objects.cpp: object-level (C++) parts of the 2D interference kernel: outer
// boundary loop / outer die curve, rotated boundary of an object (1401a28c0), curve bounding box
// with arcs (140279960, 14027a720, 14027ab80), zero-length segment removal (14027e990,
// 140306a70). Methods of model classes owned by finished packets are free functions here.
// Spec: OBJPOS_interference.md 7.1, 7.4; decompiles and listings in out/p672 and out/p672/d2.
#include "d2.hpp"
#include "../drv/ifd3d.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <defrt/ftn/geoutl.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defcmd/ifd/ifd_internal.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defrt/ftn/sse_ops.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defrt/rdata.hpp>

namespace def::cmd::ifd::d2 {

namespace {

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

// rdata 14051f1b8  1e-6
constexpr double kEps6 = def::rt::rdata::d_14051f1b8;
// rdata 14051f228  180.0
constexpr double k180 = def::rt::rdata::d_14051f228;
// @data 140535ff0  3.1415927 (a short pi literal of the C++ code)
constexpr double kPiShort = 3.1415927;
// rdata 140522fd8  57.29577951308232 (rad -> deg)
constexpr double kRadToDeg = def::rt::rdata::d_140522fd8;

// 1405e7770..1405e77ef: 8 tables [start quadrant][end quadrant]; non-zero = the full-circle
// extreme is reached. Negative sweep: ymax, ymin, xmax, xmin; positive: ymax, ymin, xmax, xmin.
// @data 1405e7770  negative sweep, ymax
constexpr unsigned char kTabNegYmax[16] = {0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0};
// @data 1405e7780  negative sweep, ymin
constexpr unsigned char kTabNegYmin[16] = {0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0};
// @data 1405e7790  negative sweep, xmax
constexpr unsigned char kTabNegXmax[16] = {0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0};
// @data 1405e77a0  negative sweep, xmin
constexpr unsigned char kTabNegXmin[16] = {0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0};
// @data 1405e77b0  positive sweep, ymax
constexpr unsigned char kTabPosYmax[16] = {0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0};
// @data 1405e77c0  positive sweep, ymin
constexpr unsigned char kTabPosYmin[16] = {0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0};
// @data 1405e77d0  positive sweep, xmax
constexpr unsigned char kTabPosXmax[16] = {0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0};
// @data 1405e77e0  positive sweep, xmin
constexpr unsigned char kTabPosXmin[16] = {0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0};

double vmin(double v, double cur) { return (v < cur || v == cur) ? v : cur; }
double vmax(double v, double cur) { return (cur <= v) ? v : cur; }

std::int32_t quadrant(double dx, double dy)
{
    if (0.0 > dx) return (0.0 > dy) ? 2 : 1;
    return (0.0 > dy) ? 3 : 0;
}

} // namespace

// @src 1402204a0  -> 140221390: x ** n by repeated squaring, 1.0 / result for n < 0
// @src 140221390  (absorbed into fun_1402204a0)
double fun_1402204a0(double x, std::int32_t n)
{
    std::uint32_t k = static_cast<std::uint32_t>(n);
    if (n < 0) k = static_cast<std::uint32_t>(-static_cast<std::int64_t>(n));
    double r = 1.0;
    double b = x;
    for (;;) {
        if ((k & 1u) != 0) r = r * b;
        k >>= 1;
        if (k == 0) break;
        b = b * b;
    }
    return n < 0 ? 1.0 / r : r;
}

// @src 14017ed40  box of the nodes of a loop record (+0 count, +8 nodes); a node equal to the
// next one is skipped; false for an empty record
bool fun_14017ed40(void* rec, double* min, double* max, double* xy)
{
    const std::int32_t n = at<std::int32_t>(rec, 0);
    if (n < 1) return false;
    const std::int32_t* nodes = at<std::int32_t*>(rec, 8);
    bool first = true;
    for (std::int32_t k = 0; k < n - 1; ++k) {
        if (nodes[k] == nodes[k + 1]) continue;
        const double* p = xy + (static_cast<std::int64_t>(nodes[k]) * 2 - 2);
        if (first) {
            for (int c = 0; c < 2; ++c) {
                min[c] = p[c];
                max[c] = p[c];
            }
            first = false;
        } else {
            for (int c = 0; c < 2; ++c) {
                if (p[c] <= min[c] && min[c] != p[c]) min[c] = p[c];
                if (max[c] <= p[c] && p[c] != max[c]) max[c] = p[c];
            }
        }
    }
    return true;
}

// @src 1401ffa90  index (1-based) of the loop whose box contains the running box of all loops
// before it (score 1111); 0 when none qualifies; false when the object has no mesh
bool fun_1401ffa90(LLOBJ_DATA* obj, std::int32_t* idx)
{
    double run_min[2] = {0.0, 0.0};
    double run_max[2] = {0.0, 0.0};
    double nmin[2] = {0.0, 0.0};
    double nmax[2] = {0.0, 0.0};
    bool first = true;
    *idx = 0;
    if (at<std::int32_t>(obj, 0x87c) == 0) return false;
    for (std::int32_t r = 0; r < at<std::int32_t>(obj, 0xbd8); ++r) {
        void* rec = &at<std::byte>(obj, 0xbe0 + static_cast<std::size_t>(r) * 0x30);
        if (!fun_14017ed40(rec, nmin, nmax, at<double*>(obj, 0x8a8))) continue;
        if (!first) {
            std::int32_t score = 0;
            for (std::int32_t c = 0; c < 2; ++c) {
                if (nmin[c] <= run_min[c]) {
                    run_min[c] = nmin[c];
                    score = score + static_cast<std::int32_t>(fun_1402204a0(10.0, c)) * 100;
                }
                if (run_max[c] <= nmax[c]) {
                    run_max[c] = nmax[c];
                    score = score + static_cast<std::int32_t>(fun_1402204a0(10.0, c));
                }
            }
            if (score == 0x457) *idx = r + 1;
        } else {
            for (std::int32_t c = 0; c < 2; ++c) {
                run_min[c] = nmin[c];
                run_max[c] = nmax[c];
            }
            *idx = r + 1;
            first = false;
        }
    }
    return true;
}

// @src 1401ff7f0  the same over the die curves (box 140279040); false without geometry (140160cc0)
bool fun_1401ff7f0(LLOBJ_DATA* obj, std::int32_t* idx)
{
    double run_min[2] = {0.0, 0.0};
    double run_max[2] = {0.0, 0.0};
    double nmin[2] = {0.0, 0.0};
    double nmax[2] = {0.0, 0.0};
    double radius = 0.0;
    bool first = true;
    *idx = 0;
    if (!ext::has_geometry(obj)) return false;
    for (std::int32_t i = 0; i < model::geo_kw::curve_count(*obj); ++i) {
        model::OBJ_GEO2D* c = model::geo_kw::curve(*obj, i);
        if (!c->bounding_box(nmin, nmax, &radius)) continue;
        if (!first) {
            std::int32_t score = 0;
            for (std::int32_t k = 0; k < 2; ++k) {
                if (nmin[k] <= run_min[k]) {
                    run_min[k] = nmin[k];
                    score = score + static_cast<std::int32_t>(fun_1402204a0(10.0, k)) * 100;
                }
                if (run_max[k] <= nmax[k]) {
                    run_max[k] = nmax[k];
                    score = score + static_cast<std::int32_t>(fun_1402204a0(10.0, k));
                }
            }
            if (score == 0x457) *idx = i + 1;
        } else {
            for (std::int32_t k = 0; k < 2; ++k) {
                run_min[k] = nmin[k];
                run_max[k] = nmax[k];
            }
            *idx = i + 1;
            first = false;
        }
    }
    return true;
}

// 1401808d0 is ported once, as def::cmd::ifd::node_coords (src/ifd/drv/ifd3d.cpp)
double* fun_1401808d0(void* nodeset) { return ::def::cmd::ifd::node_coords(nodeset); }

// @src 14027a720  rounded corner p0 - p1 - p2 with radius p1[2]: centre c, tangent points a
// (on p0-p1) and b (on p1-p2); 0 for a degenerate corner (outputs untouched)
bool fun_14027a720(double* p0, double* p1, double* p2, double* a, double* b, double* c)
{
    double d2 = p1[0] - p0[0];
    double d3 = p1[1] - p0[1];
    double d4 = p2[0] - p1[0];
    double d5 = p2[1] - p1[1];
    const double l1 = std::sqrt(d2 * d2 + d3 * d3); // 140484644
    const double l2 = std::sqrt(d4 * d4 + d5 * d5);
    if (l1 == 0.0 || l2 == 0.0) return false;
    d2 = d2 / l1;
    d3 = d3 / l1;
    d4 = d4 / l2;
    d5 = d5 / l2;
    const double cr = d2 * d5 - d4 * d3;
    if (cr == 0.0) return false;
    const double sg = (cr <= 0.0) ? -1.0 : 1.0;
    const double r = p1[2];
    if (r == 0.0) return false;
    double rr = r;
    if (cr <= 0.0) rr = -r;
    const double cx = p1[0] + (rr * (d4 - d2)) / cr;
    const double cy = p1[1] + (rr * (d5 - d3)) / cr;
    a[0] = cx - r * sg * -d3;
    a[1] = cy - r * sg * d2;
    b[0] = cx - r * sg * -d5;
    b[1] = cy - r * sg * d4;
    c[0] = cx;
    c[1] = cy;
    return true;
}

// @src 14027ab80  box of the arc from (x1, y1) about (cx, cy) by sweep degrees; |sweep| <= 1e-6
// gives x1 for all four values (as the original)
void fun_14027ab80(double x1, double y1, double cx, double cy, double sweep, double* xmin,
                   double* xmax, double* ymin, double* ymax)
{
    double oxmin = x1, oymin = x1, oymax = x1, oxmax = x1; // [rsp+0x20/0x28/0x30/0x38]
    if (std::fabs(sweep) > kEps6) {
        const double dx = x1 - cx;
        const double dy = y1 - cy;
        const double a0 = rt::libm::crt_atan2(dy, dx); // 140485d6c
        const double a1 = (a0 * k180) / kPiShort + sweep;
        const double r = std::sqrt(dx * dx + dy * dy);
        const double cxmin = cx - r, cxmax = cx + r, cymin = cy - r, cymax = cy + r;
        // rdata 14051f210  360.0, 140537b50  -360.0
        if (sweep >= 360.0 || -360.0 > sweep) {
            oxmin = cxmin;
            oymin = cymin;
            oxmax = cxmax;
            oymax = cymax;
        } else {
            const double ex = cx + r * rt::libm::crt_cos((a1 * kPiShort) / k180); // 140bb3490
            const double ey = cy + r * rt::libm::crt_sin((a1 * kPiShort) / k180); // 140bb3000
            const std::int32_t q0 = quadrant(dx, dy);
            const std::int32_t q1 = quadrant(ex - cx, ey - cy);
            const std::int32_t t = q1 + q0 * 4;
            const bool pos = sweep > 0.0;
            if (q0 == q1) {
                // @data 140537b48  270.0, 140537b40  -270.0
                const bool full = pos ? (sweep > 270.0) : (-270.0 > sweep);
                if (full) {
                    oxmin = cxmin;
                    oymin = cymin;
                    oxmax = cxmax;
                    oymax = cymax;
                } else {
                    oxmin = (ex > x1) ? x1 : ex;
                    oxmax = (x1 > ex) ? x1 : ex;
                    oymin = (ey > y1) ? y1 : ey;
                    oymax = (y1 > ey) ? y1 : ey;
                }
            } else {
                const unsigned char* txmin = pos ? kTabPosXmin : kTabNegXmin;
                const unsigned char* txmax = pos ? kTabPosXmax : kTabNegXmax;
                const unsigned char* tymin = pos ? kTabPosYmin : kTabNegYmin;
                const unsigned char* tymax = pos ? kTabPosYmax : kTabNegYmax;
                oxmin = txmin[t] != 0 ? cxmin : ((ex > x1) ? x1 : ex);
                oxmax = txmax[t] != 0 ? cxmax : ((x1 > ex) ? x1 : ex);
                oymin = tymin[t] != 0 ? cymin : ((ey > y1) ? y1 : ey);
                oymax = tymax[t] != 0 ? cymax : ((y1 > ey) ? y1 : ey);
            }
        }
    }
    if (xmin != nullptr) *xmin = oxmin;
    if (xmax != nullptr) *xmax = oxmax;
    if (ymin != nullptr) *ymin = oymin;
    if (ymax != nullptr) *ymax = oymax;
}

// @src 140279960  bounding box of a curve including the arcs (type 1: rounded corners with
// radius > 1e-6; type 2: arc rows); false when empty
bool fun_140279960(OBJ_GEO2D* c, double* min, double* max)
{
    const std::int32_t d = c->dof();
    if (c->n_points < 1 || c->points == nullptr) return false;
    double* p = c->points;
    if (c->curve_type == 1) {
        max[0] = p[0];
        min[0] = max[0];
        max[1] = p[1];
        min[1] = max[1];
        for (std::int32_t i = 1; i < c->n_points; ++i) {
            double* row = p + static_cast<std::int64_t>(i) * d;
            if (row[2] <= kEps6) {
                min[0] = vmin(row[0], min[0]);
                max[0] = vmax(row[0], max[0]);
                min[1] = vmin(row[1], min[1]);
                max[1] = vmax(row[1], max[1]);
            } else {
                std::int32_t inext = i + 1;
                if (c->n_points <= inext) inext = 0;
                double a[2] = {0.0, 0.0}, b[2] = {0.0, 0.0}, cc[2] = {0.0, 0.0};
                fun_14027a720(p + static_cast<std::int64_t>(i - 1) * d, row,
                              p + static_cast<std::int64_t>(inext) * d, a, b, cc);
                const double a0 = rt::libm::crt_atan2(a[1] - cc[1], a[0] - cc[0]);
                const double a1 = rt::libm::crt_atan2(b[1] - cc[1], b[0] - cc[0]);
                double bx0 = 0.0, bx1 = 0.0, by0 = 0.0, by1 = 0.0;
                fun_14027ab80(a[0], a[1], cc[0], cc[1], -(a1 - a0), &bx0, &bx1, &by0, &by1);
                min[0] = vmin(bx0, min[0]);
                max[0] = vmax(bx1, max[0]);
                min[1] = vmin(by0, min[1]);
                max[1] = vmax(by1, max[1]);
            }
        }
    } else {
        max[0] = p[1];
        min[0] = max[0];
        max[1] = p[2];
        min[1] = max[1];
        for (std::int32_t i = 0; i < c->n_points; ++i) {
            double* row = p + static_cast<std::int64_t>(i) * d;
            if (static_cast<std::int32_t>(row[0]) == 2) {
                double bx0 = 0.0, bx1 = 0.0, by0 = 0.0, by1 = 0.0;
                fun_14027ab80(row[1], row[2], row[3], row[4], (row[5] * k180) / kPiShort, &bx0, &bx1,
                              &by0, &by1);
                min[0] = vmin(bx0, min[0]);
                max[0] = vmax(bx1, max[0]);
                min[1] = vmin(by0, min[1]);
                max[1] = vmax(by1, max[1]);
            } else {
                min[0] = vmin(row[1], min[0]);
                max[0] = vmax(row[1], max[0]);
                min[1] = vmin(row[2], min[1]);
                max[1] = vmax(row[2], max[1]);
                min[0] = vmin(row[3], min[0]);
                max[0] = vmax(row[3], max[0]);
                min[1] = vmin(row[4], min[1]);
                max[1] = vmax(row[4], max[1]);
            }
        }
    }
    return true;
}

// @src 14027e990  type-2 curve: drop every row with |x2 - x1| < 1e-6 and |y2 - y1| < 1e-6
// (columns 1..4, arcs included); set_flag28(1); true when a row was dropped
bool fun_14027e990(OBJ_GEO2D* c)
{
    if (c->n_points < 1 || c->points == nullptr) return false;
    if (c->curve_type != 2) return false;
    const std::int32_t d = c->dof();
    const std::int32_t n = c->n_points;
    auto* mark = static_cast<unsigned char*>(std::malloc(static_cast<std::size_t>(n)));
    std::memset(mark, 0, static_cast<std::size_t>(n));
    std::int32_t dropped = 0;
    for (std::int32_t i = 0; i < n; ++i) {
        const double* row = c->points + static_cast<std::int64_t>(i) * d;
        if (std::fabs(row[3] - row[1]) < kEps6 && std::fabs(row[4] - row[2]) < kEps6) {
            mark[i] = 1;
            dropped = dropped + 1;
        }
    }
    if (0 < dropped) {
        const std::size_t bytes = static_cast<std::size_t>(static_cast<std::int64_t>(n) * d) * 8;
        auto* tmp = static_cast<double*>(std::malloc(bytes));
        std::memcpy(tmp, c->points, bytes);
        std::int32_t k = 0;
        for (std::int32_t i = 0; i < c->n_points; ++i) {
            if (mark[i] == 0) {
                std::memcpy(c->points + static_cast<std::int64_t>(k) * d,
                            tmp + static_cast<std::int64_t>(i) * d, static_cast<std::size_t>(d) * 8);
                k = k + 1;
            }
        }
        c->n_points = k;
        std::free(tmp);
    }
    std::free(mark);
    c->set_flag28(true);
    return 0 < dropped;
}

// @src 140306a70  OR of 14027e990 over the object's curves
bool fun_140306a70(LLOBJ_DATA* obj)
{
    bool r = false;
    for (std::int32_t i = 0; i < model::geo_kw::curve_count(*obj); ++i) {
        r = fun_14027e990(model::geo_kw::curve(*obj, i)) || r;
    }
    return r;
}

// @src 14019a620  -> 140276140 assign
void fun_14019a620(OBJ_GEO2D* dst, OBJ_GEO2D* src) { dst->assign(src); }

// @src 1401a28c0  boundary of one object rotated by theta: kind 2 = mesh boundary loop
// (14033f2bf into buf, reordered into xy), kind 1 = die curve copied into `curve`, converted to
// type 2, rotated by theta in degrees, box 140279960 (spec IF 7.4)
void fun_1401a28c0(DB_STEP_DATA* /*m*/, LLOBJ_DATA* obj, OBJ_GEO2D* curve, double* xy, double* buf,
                   std::int32_t side, std::int32_t* kind, double* theta, std::int32_t* count,
                   double* xmm, double* ymm, double* area)
{
    std::int32_t idx = -1; // local_c0
    std::int32_t sd = side;
    auto mesh_loop = [&]() {
        *kind = 2;
        fun_1401ffa90(obj, &idx);
        void* rec = &at<std::byte>(obj, 0xbe0 + static_cast<std::ptrdiff_t>(idx - 1) * 0x30);
        std::int32_t* nodes = at<std::int32_t*>(rec, 8);
        *count = at<std::int32_t>(rec, 0);
        std::int32_t range[2] = {1, *count};
        fun_14033f2bf(&sd, xy, buf, range, nodes, count, area, xmm, ymm, theta);
    };
    if (!model::geo_kw::geometry_has_edges(*obj)) {
        mesh_loop();
        return;
    }
    *kind = 1;
    auto take = [&](OBJ_GEO2D* src) {
        fun_14019a620(curve, src);
        *count = src->n_points;
        if (src->curve_type != 2) {
            fun_14019a620(curve, src);
            curve->set_curve_type(2);
            *count = curve->n_points;
        }
    };
    if (model::geo_kw::curve_count(*obj) == 1) {
        take(model::geo_kw::curve(*obj, 0));
    } else {
        fun_1401ff7f0(obj, &idx);
        if (idx != -1) take(model::geo_kw::curve(*obj, idx - 1));
    }
    if (idx == -1 && model::geo_kw::curve_count(*obj) != 1) {
        // not reachable: 1401ff7f0 sets idx >= 0
        if (0 < at<std::int32_t>(obj, 0x884) && 0 < at<std::int32_t>(obj, 0x880)) mesh_loop();
        return;
    }
    double center[3] = {0.0, 0.0, 0.0}; // local_a0..
    double axis[3] = {0.0, 0.0, 0.0};   // local_50..
    const double deg = rt::sse::mul(*theta, kRadToDeg);
    curve->rotate(center, axis, deg); // vtable +0x40
    double bmin[2] = {0.0, 0.0};
    double bmax[2] = {0.0, 0.0};
    fun_140279960(curve, bmin, bmax);
    xmm[0] = bmin[0];
    xmm[1] = bmax[0];
    ymm[0] = bmin[1];
    ymm[1] = bmax[1];
    double a = bmin[0], b = bmin[1], c = bmax[0], d = bmax[1];
    fun_14034c209(&a, &b, &c, &d);
}

} // namespace def::cmd::ifd::d2
