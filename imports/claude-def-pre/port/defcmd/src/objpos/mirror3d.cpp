// OBJPOS 3D mirror FUN_1401fd850 and its object-level callees (OBJPOS.md 6.5,
// OBJPOS_interference_kernels.md 1.7, OBJPOS_geo_kernels.md 5). Single precision throughout: the
// float matrix and point routines are in mirror_float.cpp.
#include <defcmd/objpos.hpp>
#include <defcmd/objpos_mirror.hpp>
#include <defcmd/vec3.hpp>

#include <cstdint>

#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/geo_kw/boundary.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/linked/matrix4.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/llobj/cls_140185570.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/rdata.hpp>

#include "internal.hpp"

namespace def::cmd::objpos {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;
using namespace detail;

namespace {

// rdata 14051f260
constexpr double kTiny = def::rt::rdata::d_14051f260;

bool zero_normal(const double* n)
{
    const double zero[3] = {0.0, 0.0, 0.0};
    return v3::near(n, zero, kTiny);  // 14023ace0
}

// @src 1401fddc0  rigid geometry: every surface obj+0x3c00[k] (k < +0x3994) when the object has
// one (+0x3990)
void geometry_mirror(LLOBJ_DATA* o, const double* p, const double* n)
{
    if (p == nullptr || n == nullptr || zero_normal(n) || !model::geo_kw::has_surface_flag(*o)) return;
    for (std::int32_t k = 0; k < model::geo_kw::surface_count(*o); k = k + 1) {
        geo3d_mirror(model::geo_kw::surface(*o, k), p, n);
    }
}

// @src 1401fde90  mesh: RZ only (float), element orientation; then FLDGEO / FLWNET
void mesh_mirror(LLOBJ_DATA* o, const double* p, const double* n)
{
    if (p == nullptr || n == nullptr || zero_normal(n)) return;
    if (at<std::int32_t>(o, 0x87c) != 0) {
        F44 m = mirror_matrix(p, n);
        double* rz = at<double*>(o, 0x8a8);
        for (std::int32_t i = 0; i < at<std::int32_t>(o, 0x880); i = i + 1) mirror_point_f(rz + i * 3, &m);
        fix_element_orientation(o);
    }
    if (model::geo_kw::has_fldgeo(*o)) geo3d_mirror(reinterpret_cast<model::OBJ_GEO3D*>(ptr(o, 0x3328)), p, n);
    if (model::geo_kw::has_flwnet(*o)) geo3d_mirror(reinterpret_cast<model::OBJ_GEO3D*>(ptr(o, 0x3458)), p, n);
}

// @src 1401ff2b0  remesh windows (obj+0x2d10, list +0x40)
void remesh_mirror(unsigned char* rm, const double* p, const double* n) { winlist_mirror_3d(rm + 0x40, p, n); }

// @src 1401ff2f0  BC windows (obj+0x2e68): window lists +0x78, +0xa8, then the SYMSUF list +0x18
// (no dimension test here)
void bcwin_mirror(unsigned char* bc, const double* p, const double* n)
{
    winlist_mirror_3d(bc + 0x78, p, n);
    winlist_mirror_3d(bc + 0xa8, p, n);
    axis_list_mirror(reinterpret_cast<model::linked::Cls_140257340List*>(bc + 0x18), p, n);
}

// @src 1401fea60  movement: CNTRAX centre +0x630 -> M(c), axis +0x648 -> M(a) - M(0) (float
// subtraction, then widened); the same for CNTRA2 (+0x760, +0x778). No normalisation.
void movement_mirror(unsigned char* mv, const double* p, const double* n)
{
    if (p == nullptr || n == nullptr || zero_normal(n)) return;
    F44 m = mirror_matrix(p, n);
    mirror_point_f(&at<double>(mv, 0x630), &m);
    F3 z = f3(0.0f, 0.0f, 0.0f);
    F3 mz;
    f44_apply(&z, &mz, &m);
    for (std::size_t base : {std::size_t{0x648}, std::size_t{0x778}}) {
        if (base == 0x778) mirror_point_f(&at<double>(mv, 0x760), &m);
        double* a = &at<double>(mv, base);
        F3 v = f3(static_cast<float>(a[0]), static_cast<float>(a[1]), static_cast<float>(a[2]));
        F3 q;
        f44_apply(&v, &q, &m);
        const float dx = q.x - mz.x;
        const float dy = q.y - mz.y;
        const float dz = q.z - mz.z;
        a[0] = static_cast<double>(dx);
        a[1] = static_cast<double>(dy);
        a[2] = static_cast<double>(dz);
    }
}

// @src 1401ff040  process: REFPOS point +0x30 when the REFPOS type +0x28 is 1
void process_mirror(unsigned char* pr, const double* p, const double* n)
{
    if (at<std::int32_t>(pr, 0x28) != 1 || p == nullptr || n == nullptr || zero_normal(n)) return;
    F44 m = mirror_matrix(p, n);
    mirror_point_f(&at<double>(pr, 0x30), &m);
}

// @src 14021f940  GEO23 block (obj+0x2be8): the two frames gated by +0x81 / +0xd0 (never set in
// batch, OBJPOS_geo_kernels.md 5); every vector mirrored as a point
void geo23_mirror(unsigned char* g, const double* p, const double* n)
{
    if (p == nullptr || n == nullptr || zero_normal(n)) return;
    F44 m = mirror_matrix(p, n);
    if (at<char>(g, 0x81) != 0) {
        mirror_point_f(&at<double>(g, 0x88), &m);
        mirror_point_f(&at<double>(g, 0xa0), &m);
        mirror_point_f(&at<double>(g, 0xb8), &m);
    }
    if (at<char>(g, 0xd0) != 0) {
        mirror_point_f(&at<double>(g, 0xd8), &m);
        mirror_point_f(&at<double>(g, 0xf0), &m);
        mirror_point_f(&at<double>(g, 0x108), &m);
    }
}

// @src 14019f660  windows of the inter-object entries owned by object `no` (master of a type 2
// entry or slave of a type 1 entry), 3D window mirror
void model_mirror_owned(DB_STEP_DATA* m, const double* p, const double* n, std::int32_t no)
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
                model::LLWINDEF_DATA* w = model::winlist_nth(&e->u188, j + 1);
                if (w != nullptr) window_mirror_3d(w, p, n);
            }
        }
    }
}

} // namespace

// @src 1401fe410  element node order: element kind +0x890 == 2 (tet) swaps nodes 1 and 2, == 4
// (brick) 1<->3 and 5<->7 (0-based, xor swaps), for every element of ELMCON (+0x9e8, +0x894 nodes
// per element)
void fix_element_orientation(LLOBJ_DATA* o)
{
    auto* c = at<std::uint32_t*>(o, 0x9e8);
    if (at<std::int32_t>(o, 0x884) <= 0 || c == nullptr) return;
    auto xswap = [c](std::int32_t a, std::int32_t b) {
        c[a] = c[a] ^ c[b];
        c[b] = c[b] ^ c[a];
        c[a] = c[a] ^ c[b];
    };
    for (std::int32_t e = 0; e < at<std::int32_t>(o, 0x884); e = e + 1) {
        const std::int32_t npe = at<std::int32_t>(o, 0x894);
        if (at<std::int32_t>(o, 0x890) == 2) {
            xswap(e * npe + 1, e * npe + 2);
        } else if (at<std::int32_t>(o, 0x890) == 4) {
            xswap(e * npe + 1, e * npe + 3);
            xswap(e * npe + 5, e * npe + 7);
        }
    }
}

// @src 140201fb0  re-orient one boundary record: 2D model (owner +0x28): its node list reversed
// (140305360 on the node count); 3D: when face 0 is a triangle (F1 == |F4|) every face swaps
// nodes 2 and 3, else nodes 2 and 4 (1-based)
void boundary_record_reorient(model::Cls_14017e740* r)
{
    if (model::model_is_2d(static_cast<DB_STEP_DATA*>(r->u028))) {  // 1400015e0
        if (-1 < r->u000) reverse_ints(&r->u000, static_cast<std::int32_t*>(r->u008));
        return;
    }
    if (r->u010 <= 0) return;
    auto* f = static_cast<std::uint32_t*>(r->u018);
    const std::int32_t d = static_cast<std::int32_t>(f[3]);
    const std::int32_t a4 = d < 0 ? static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(d)) : d;  // 140484ab8
    const std::int32_t a1 = static_cast<std::int32_t>(f[0]);
    for (std::int32_t i = 0; i < r->u010; i = i + 1) {
        f = static_cast<std::uint32_t*>(r->u018);
        const std::int32_t other = a1 == a4 ? 2 : 3;
        std::uint32_t& x = f[i * 4 + 1];
        std::uint32_t& y = f[i * 4 + other];
        x = x ^ y;
        y = y ^ x;
        x = x ^ y;
    }
}

// @src 140281710  display placement of a geometry-list entry: the float mirror matrix widened to
// doubles, copied element by element into an identity Matrix4 (140220540, 1402854d0: no
// transposition although the float class is column-vector and Matrix4 row-vector), then
// M_item = M_item * M (140281660, unless the entry's flag +0x31)
void cross_section_mirror(model::GEO2D_AND_MESH2D* g, const double* p, const double* n)
{
    if (p == nullptr || n == nullptr || zero_normal(n)) return;
    F44 mf = mirror_matrix(p, n);
    double md[16];
    for (std::int32_t k = 0; k < 16; k = k + 1) md[k] = static_cast<double>(mf.m[k]);
    model::linked::Matrix4 t;                      // 140220540
    for (std::int32_t k = 0; k < 16; k = k + 1) t.m[k] = md[k];  // 1402854d0
    if (g->u31 == 0) {                             // 140281660 / 140284ae0
        auto* mi = reinterpret_cast<model::linked::Matrix4*>(g->matrix);
        model::linked::Matrix4& r = mi->multiply_by(t);  // 140221a90
        mi->assign(r);                                   // 140220670
    }
}

// @src 1401fd850  3D mirror at the plane (p, n)
void mirror_3d(LLOBJ_DATA* o, const double* p, const double* n)
{
    // backups (constructed and given the owner even when nothing is done)
    model::Cls_14017e740 faces;    // 14017e740
    model::Cls_140185570 arrays;   // 140185570
    faces.set_owner(o->owner);     // 140158f50, 14017e770
    arrays.u060 = o->owner;        // 1401855d0: owner, its dimension (140158e90 -> 14019a4b0)
    if (arrays.u060 != nullptr) arrays.set_u068(o->owner->dimension);
    arrays.init();
    if (zero_normal(n)) return;  // dtors 1401855b0, 14017e720
    if (0 < at<std::int32_t>(o, 0x88c)) {
        model::llobj_copy::layer_copy(reinterpret_cast<std::int32_t*>(&faces),
                                      reinterpret_cast<const std::int32_t*>(ptr(o, 0xbe0)));  // 14017ec20
        model::llobj_copy::arrays_copy(arrays.arrays, reinterpret_cast<void* const*>(ptr(o, 0xae8)),
                                       at<std::int32_t>(o, 0x88c));  // 140185a10
    }
    geometry_mirror(o, p, n);
    mesh_mirror(o, p, n);
    model::geo_kw::build_boundary(*o);  // 1401fc290
    remesh_mirror(ptr(o, 0x2d10), p, n);
    bcwin_mirror(ptr(o, 0x2e68), p, n);
    movement_mirror(ptr(o, 0x2160), p, n);
    process_mirror(ptr(o, 0x1ea0), p, n);
    geo23_mirror(ptr(o, 0x2be8), p, n);
    const std::int32_t ng = model::geo_kw::geometry_count(*o);  // 1401983e0
    for (std::int32_t i = 0; i < ng; i = i + 1) cross_section_mirror(model::geo_kw::geometry(*o, i), p, n);
    if (o->owner != nullptr) model_mirror_owned(o->owner, p, n, at<std::int32_t>(o, 8));
    if (0 < at<std::int32_t>(o, 0x88c)) {
        boundary_record_reorient(&faces);
        const auto* bf = static_cast<const std::int32_t*>(faces.u018);
        for (std::int32_t i = 0; i < at<std::int32_t>(o, 0x88c); i = i + 1) {
            // 140200ff0; the original stores at index -1 when the face is not found
            const std::int32_t k = model::bc::face_slot(*o, bf[i * 4], bf[i * 4 + 1], bf[i * 4 + 2], bf[i * 4 + 3]);
            void* const* src = arrays.arrays;
            void** dst = reinterpret_cast<void**>(ptr(o, 0xae8));
            static_cast<std::int32_t*>(dst[0])[k] = static_cast<const std::int32_t*>(src[0])[i];
            static_cast<std::int32_t*>(dst[1])[k] = static_cast<const std::int32_t*>(src[1])[i];
            static_cast<std::uint64_t*>(dst[2])[k] = static_cast<const std::uint64_t*>(src[2])[i];
            static_cast<std::int32_t*>(dst[3])[k] = static_cast<const std::int32_t*>(src[3])[i];
            static_cast<std::int32_t*>(dst[4])[k] = static_cast<const std::int32_t*>(src[4])[i];
            static_cast<std::uint64_t*>(dst[5])[k] = static_cast<const std::uint64_t*>(src[5])[i];
            static_cast<std::int32_t*>(dst[6])[k] = static_cast<const std::int32_t*>(src[6])[i];
            static_cast<std::int32_t*>(dst[7])[k] = static_cast<const std::int32_t*>(src[7])[i];
            static_cast<std::uint64_t*>(dst[8])[k] = static_cast<const std::uint64_t*>(src[8])[i];
        }
    }
}

} // namespace def::cmd::objpos
