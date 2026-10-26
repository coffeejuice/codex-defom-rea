// OBJPOS 2D mirror FUN_1403066a0 and its callees (OBJPOS.md 6.6, OBJPOS_geo_kernels.md 4). Double
// precision; the point kernel is 140309640 (Vector2 helpers of defcmd/vec3.hpp).
#include <defcmd/objpos.hpp>
#include <defcmd/objpos_mirror.hpp>
#include <defcmd/vec3.hpp>

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/port.hpp>
#include <defmodel/statevar/geo2d_kernels.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/rdata.hpp>

#include "internal.hpp"

namespace def::cmd::objpos {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;
using model::LLWINDEF_DATA;
using namespace detail;

namespace {

// rdata 14051f260
constexpr double kTiny = def::rt::rdata::d_14051f260;
// rdata 140509fb8
constexpr double kTwo = def::rt::rdata::d_140509fb8;
// rdata 14051bed8
constexpr double kZero = def::rt::rdata::d_14051bed8;
// @data 14051bec0  sign bit of a double
constexpr std::uint64_t kSign = 0x8000000000000000ull;

// @src 1403067b0  geometry: every edge curve of geometry 0 (when it has edges)
void geometry_mirror_2d(LLOBJ_DATA* o, const double* a, const double* b)
{
    if (v3::near2(a, b, kTiny) || a == nullptr || b == nullptr || !model::geo_kw::geometry_has_edges(*o)) return;
    for (std::int32_t i = 0; i < model::geo_kw::curve_count(*o); i = i + 1) {
        geo2d_mirror(model::geo_kw::curve(*o, i), a, b);  // 1401982c0
    }
}

// @src 1403050b0  2D ELMCON: nodes 1 and 3 (0-based) of every 4-node element swapped
void elements_reverse_2d(LLOBJ_DATA* o)
{
    std::int32_t* c = at<std::int32_t*>(o, 0x9e8);
    if (at<std::int32_t>(o, 0x884) <= 0 || c == nullptr) return;
    const std::int32_t ne = at<std::int32_t>(o, 0x884);
    for (std::int32_t i = 0; i < ne * 4; i = i + 4) {
        const std::int32_t t = c[i + 1];
        c[i + 1] = c[i + 3];
        c[i + 3] = t;
    }
}

// @src 1401ffd00  every boundary record re-oriented (when 0 < +0xbd8 < 100)
void boundary_reorient(LLOBJ_DATA* o)
{
    const std::int32_t nb = at<std::int32_t>(o, 0xbd8);
    if (nb <= 0 || 99 < nb) return;
    for (std::int32_t k = 0; k < at<std::int32_t>(o, 0xbd8); k = k + 1) {
        boundary_record_reorient(reinterpret_cast<model::Cls_14017e740*>(ptr(o, 0xbe0 + static_cast<std::size_t>(k) * 0x30)));
    }
}

// @src 140306860  nodes (when +0x87c): RZ only (2 per node); then ELMCON and the boundary records
void mesh_mirror_2d(LLOBJ_DATA* o, const double* a, const double* b)
{
    if (v3::near2(a, b, kTiny) || at<std::int32_t>(o, 0x87c) == 0) return;
    double* rz = at<double*>(o, 0x8a8);
    if (rz != nullptr) {
        for (std::int32_t i = 0; i < at<std::int32_t>(o, 0x880); i = i + 1) mirror_point_2d(rz + i * 2, a, b);
    }
    elements_reverse_2d(o);
    boundary_reorient(o);
}

// @src 14026a020  every window of a window list
void winlist_mirror_2d(model::Cls_140269250* l, const double* a, const double* b)
{
    for (std::int32_t k = 0; k < model::winlist_count(l); k = k + 1) {
        LLWINDEF_DATA* w = model::winlist_nth(l, k + 1);
        if (w != nullptr) window_mirror_2d(w, a, b);
    }
}

// @src 140306b70  remesh windows (obj+0x2d10, list +0x40)
void remesh_mirror_2d(unsigned char* rm, const double* a, const double* b)
{
    winlist_mirror_2d(reinterpret_cast<model::Cls_140269250*>(rm + 0x40), a, b);
}

// @src 140306bb0  BC window lists +0x78 and +0xa8 (obj+0x2e68)
void bcwin_mirror_2d(unsigned char* bc, const double* a, const double* b)
{
    winlist_mirror_2d(reinterpret_cast<model::Cls_140269250*>(bc + 0x78), a, b);
    winlist_mirror_2d(reinterpret_cast<model::Cls_140269250*>(bc + 0xa8), a, b);
}

// @src 140306ae0  movement: CNTRAX centre +0x630
void movement_mirror_2d(unsigned char* mv, const double* a, const double* b) { mirror_point_2d(&at<double>(mv, 0x630), a, b); }

// @src 140306b20  process: REFPOS point +0x30 when the type +0x28 is 1
void process_mirror_2d(unsigned char* pr, const double* a, const double* b)
{
    if (at<std::int32_t>(pr, 0x28) == 1) mirror_point_2d(&at<double>(pr, 0x30), a, b);
}

// @src 14019f830  windows of the inter-object entries owned by object `no`, 2D window mirror
void model_mirror_owned_2d(DB_STEP_DATA* m, const double* a, const double* b, std::int32_t no)
{
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        LLOBJ_DATA* ob = static_cast<LLOBJ_DATA*>(model::step_object_at(*m, i + 1));  // 1401d6ed0
        if (ob == nullptr || at<std::int32_t>(ob, 0xc) == 7 || at<std::int32_t>(ob, 8) != no) continue;  // 140160f10
        const std::int32_t id = at<std::int32_t>(ob, 8);
        for (std::int32_t k = 0; k < model::pair_count(&m->inter_obj); k = k + 1) {
            model::Cls_1401f46e0* e = model::pair_nth(&m->inter_obj, k + 1);  // 14019d5e0
            if (e == nullptr || model::winlist_count(&e->u188) <= 0) continue;
            if (!((e->u170 == 2 && id == e->u168) || (e->u170 == 1 && id == e->u16c))) continue;
            for (std::int32_t j = 0; j < model::winlist_count(&e->u188); j = j + 1) {
                LLWINDEF_DATA* w = model::winlist_nth(&e->u188, j + 1);
                if (w != nullptr) window_mirror_2d(w, a, b);
            }
        }
    }
}

} // namespace

// @src 140309640  mirror the point at the line a-b: d = unit(b - a) (zero when |b - a| <= 1e-12),
// w = p - a, r = (w.d) d - w, p = p + 2 r (Vector2 helpers, each a separate rounding)
void mirror_point_2d(double* pt, const double* a, const double* b)
{
    if (v3::near2(a, b, kTiny)) return;
    const v3::V2 va = v3::make2(a);                 // 140308730
    const v3::V2 vb = v3::make2(b);
    const v3::V2 vp = v3::make2(pt);
    v3::V2 d = v3::minus(vb, va);                   // 1403089b0
    d = v3::unit(d);                                // 140308810
    const v3::V2 w = v3::minus(vp, va);
    const double t = v3::dot(w, d);                 // 140308a90
    const v3::V2 q = v3::mul(d, t);                 // 140308940
    const v3::V2 r = v3::minus(q, w);
    const v3::V2 s = v3::mul(r, kTwo);
    const v3::V2 out = v3::plus(vp, s);             // 140308a20
    pt[0] = out.x;
    pt[1] = out.y;
}

// @src 140278740  _OBJ_GEO2D edge: type 1 records mirror (r0, r1); other types mirror (r1, r2) and
// (r3, r4) and flip the sign of r5; then the records are re-ordered (1402788a0)
void geo2d_mirror(model::OBJ_GEO2D* e, const double* a, const double* b)
{
    if (v3::near2(a, b, kTiny)) return;
    const std::int32_t type = e->curve_type;
    const std::int32_t w = e->dof();  // 1402760e0
    for (std::int32_t r = 0; r < e->n_points; r = r + 1) {
        if (type == 1) {
            mirror_point_2d(e->points + r * w, a, b);
        } else {
            mirror_point_2d(e->points + (r * w + 1), a, b);
            mirror_point_2d(e->points + (r * w + 3), a, b);
            std::uint64_t bits;
            std::memcpy(&bits, e->points + (r * w + 5), 8);
            bits = bits ^ kSign;
            std::memcpy(e->points + (r * w + 5), &bits, 8);
        }
    }
    e->reverse_points();  // 1402788a0
}

// @src 140268890  2D window: every point (2 doubles) mirrored when a and b are given, then the
// orientation check 140265bf0(w, &1)
void window_mirror_2d(LLWINDEF_DATA* w, const double* a, const double* b)
{
    if (a == nullptr || b == nullptr) return;
    for (std::int32_t i = 0; i < w->npts; i = i + 1) mirror_point_2d(w->pts + i * 2, a, b);
    const char reverse = 1;
    window_orient(w, &reverse);
}

// @src 140265bf0  window orientation: nothing without points; 2D windows (14025c930): signed area
// 140309550 >= 0.0 keeps the order, else (when *reverse) the points are reversed through a
// calloc'd copy (140257150). The 3D branch (normal sum 140240d80, each component zeroed below
// 1e-12, all >= 0 keeps the order) is not reached by the 2D mirror and is not ported.
bool window_orient(LLWINDEF_DATA* w, const char* reverse)
{
    if (w->npts <= 0) return false;
    if (!model::window_is_2d(w)) DEF_NOT_PORTED(0x140240d80);
    const double area = model::geo2d::polygon_area(w->npts, w->pts);  // 140309550
    if (kZero <= area) return true;
    if (*reverse != 0) {
        auto* tmp = static_cast<double*>(model::llobj_copy::calloc_double(w->npts << 1));  // 140257150
        for (std::int32_t i = 0; i < w->npts; i = i + 1) {
            std::memcpy(tmp + ((w->npts + -1) - i) * 2, w->pts + (i << 1), 0x10);
        }
        std::memcpy(w->pts, tmp, static_cast<std::size_t>(static_cast<std::int64_t>(w->npts << 1) << 3));
        if (tmp != nullptr) std::free(tmp);
    }
    return false;
}

// @src 140305360  reverse the first *n ints (i <-> *n-1-i), nothing when *n < 0 or v is null
void reverse_ints(const std::int32_t* n, std::int32_t* v)
{
    if (*n < 0 || v == nullptr) return;
    const std::int32_t cnt = *n;
    for (std::int32_t i = 0; i < (cnt + 1) / 2; i = i + 1) {
        const std::int32_t j = (*n - i) + -1;
        if (j != i) {
            const std::int32_t t = v[i];
            v[i] = v[j];
            v[j] = t;
        }
    }
}

// @src 140305420  reverse the first *n doubles (8-byte copies; no null or sign check)
void reverse_doubles(const std::int32_t* n, double* v)
{
    const std::int32_t cnt = *n;
    for (std::int32_t i = 0; i < (cnt + 1) / 2; i = i + 1) {
        const std::int32_t j = (*n - i) + -1;
        if (j != i) {
            std::uint64_t t;
            std::memcpy(&t, v + i, 8);
            std::memcpy(v + i, v + j, 8);
            std::memcpy(v + j, &t, 8);
        }
    }
}

// @src 140305180  per boundary loop k (0 < +0xbd8 < 100) with node count c > 0: the first c-1
// entries from the loop offset reversed in ECCDEF, ECDEFN, ECPRES, ECCTMP, ECTMFN, ECHFLX, ECCATM,
// ECATFN and ECHFLX again (ECAFLX +0xb28 never). Offset: 0 for loop 0, else previous offset + 1 +
// the current loop's count (as in the binary).
void face_arrays_reverse_2d(LLOBJ_DATA* o)
{
    const std::int32_t nb = at<std::int32_t>(o, 0xbd8);
    if (nb <= 0 || 99 < nb) return;
    std::int32_t prev = 0;
    for (std::int32_t k = 0; k < at<std::int32_t>(o, 0xbd8); k = k + 1) {
        std::int32_t c = at<std::int32_t>(o, 0xbe0 + static_cast<std::size_t>(k) * 0x30);
        const std::int32_t off = k == 0 ? 0 : prev + 1 + c;
        prev = off;
        if (0 < c) {
            c = c + -1;
            auto i4 = [&](std::size_t f) { return at<std::int32_t*>(o, f) + off; };
            auto d8 = [&](std::size_t f) { return at<double*>(o, f) + off; };
            reverse_ints(&c, i4(0xae8));
            reverse_ints(&c, i4(0xaf0));
            reverse_doubles(&c, d8(0xaf8));
            reverse_ints(&c, i4(0xb00));
            reverse_ints(&c, i4(0xb08));
            reverse_doubles(&c, d8(0xb10));
            reverse_ints(&c, i4(0xb18));
            reverse_ints(&c, i4(0xb20));
            reverse_doubles(&c, d8(0xb10));
        }
    }
}

// @src 1403066a0  2D mirror at the line a-b (nothing when a ~ b, 1e-12)
void mirror_2d(LLOBJ_DATA* o, const double* a, const double* b)
{
    if (v3::near2(a, b, kTiny)) return;  // 140308d60
    geometry_mirror_2d(o, a, b);
    mesh_mirror_2d(o, a, b);
    face_arrays_reverse_2d(o);
    remesh_mirror_2d(ptr(o, 0x2d10), a, b);
    bcwin_mirror_2d(ptr(o, 0x2e68), a, b);
    movement_mirror_2d(ptr(o, 0x2160), a, b);
    process_mirror_2d(ptr(o, 0x1ea0), a, b);
    if (o->owner != nullptr) model_mirror_owned_2d(o->owner, a, b, at<std::int32_t>(o, 8));
}

} // namespace def::cmd::objpos
