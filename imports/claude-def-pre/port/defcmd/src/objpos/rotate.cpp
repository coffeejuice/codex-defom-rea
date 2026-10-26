// OBJPOS rotate tree (FUN_14021d2f0 and callees). Spec: docs/re/commands/OBJPOS.md 6.2-6.4, 6.6a.
// Points rotate with the Fortran kernel 140325691 (3D, angle (a/45)*(pi/4)) or 14032ab90 (2D);
// vectors as R(v) - R(0); rigid die cross sections with the 4x4 matrix of 1402214c0 built from
// (a*pi)/180 and the CRT sin/cos.
#include <defcmd/objpos.hpp>
#include <defcmd/objpos_parts.hpp>
#include <defcmd/vec3.hpp>

#include <cmath>
#include <cstdint>

#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/linked/matrix4.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/members/ftn_geom.hpp>
#include <defmodel/statevar/geo2d_kernels.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/ftn/helpers.hpp>

#include "internal.hpp"

namespace def::cmd::objpos {

using model::Cls_140269250;
using model::DB_STEP_DATA;
using model::LLOBJ_DATA;
using model::LLWINDEF_DATA;
using namespace detail;

namespace {

// 140325691 (p1-10): 3D point rotation; the 5th argument some callers pass is ignored
inline void rot3(double* p, const double* o, const double* d, const double* a)
{
    model::fun_140325691(p, o, d, a);
}
// 14032ab90 (p1-09): 2D point rotation about c by *a degrees
inline void rot2(double* p, const double* a, const double* c)
{
    model::geo2d::rotate_point_deg(p, a, c);
}

} // namespace

// @src 140268160  window points: 3D (needs dir) 140325691, 2D 14032ab90; 3D windows refresh faces
void window_rotate(LLWINDEF_DATA* w, const double* origin, const double* dir, double angle)
{
    if (origin == nullptr || w->pts == nullptr) return;
    double a = angle;
    std::int32_t width;
    if (!model::window_is_2d(w)) {
        if (dir == nullptr) return;
        width = 3;
    } else {
        width = 2;
    }
    for (std::int32_t i = 0; i < w->npts; i = i + 1) {
        if (!model::window_is_2d(w)) rot3(w->pts + i * width, origin, dir, &a);
        else rot2(w->pts + i * width, &a, origin);
    }
    if (model::window_is_3d(w)) model::window_build_faces(w);
}

// @src 140269f20  every window of the list
void winlist_rotate(Cls_140269250* l, const double* origin, const double* dir, double angle)
{
    for (std::int32_t k = 0; k < model::winlist_count(l); k = k + 1) {
        LLWINDEF_DATA* w = model::winlist_nth(l, k + 1);
        if (w != nullptr) window_rotate(w, origin, dir, angle);
    }
}

namespace {

// @src 14021f6e0  remesh windows (list +0x40)
void remesh_rotate(unsigned char* rm, const double* o, const double* d, double a)
{
    winlist_rotate(reinterpret_cast<Cls_140269250*>(rm + 0x40), o, d, a);
}

// @src 14021f830  BC windows: 3D SYMSUF list +0x18 (140258400), window lists +0x78, +0xa8
void bcwin_rotate(unsigned char* bc, const double* o, const double* d, double a)
{
    if (at<DB_STEP_DATA*>(bc, 0x198)->dimension == 3) {
        reinterpret_cast<model::linked::Cls_140257340List*>(bc + 0x18)->rotate_all(o, d, a);
    }
    winlist_rotate(reinterpret_cast<Cls_140269250*>(bc + 0x78), o, d, a);
    winlist_rotate(reinterpret_cast<Cls_140269250*>(bc + 0xa8), o, d, a);
}

// @src 14021dd70  movement (only for objects with die geometry): 3D CNTRAX / CNTRA2 centre and
// axis (when the axis is not ~0, 1e-12): centre R(c), axis R(a) - R(0) normalised; 2D CNTRAX
// centre. The direction mv+0x50 is not rotated.
void movement_rotate(unsigned char* mv, const double* o, const double* d, double angle)
{
    double z0[3] = {0.0, 0.0, 0.0};
    double zero[3] = {0.0, 0.0, 0.0};
    double a = angle;
    if (at<DB_STEP_DATA*>(mv, 0xa80)->dimension != 2) rot3(z0, o, d, &a);
    if (at<DB_STEP_DATA*>(mv, 0xa80)->dimension != 2) {
        if (!v3::near(&at<double>(mv, 0x648), zero, 1e-12)) {  // DAT_14051f260
            rot3(&at<double>(mv, 0x630), o, d, &a);
            rot3(&at<double>(mv, 0x648), o, d, &a);
            v3::sub(&at<double>(mv, 0x648), z0, &at<double>(mv, 0x648));
            v3::normalize(&at<double>(mv, 0x648));
        }
        if (!v3::near(&at<double>(mv, 0x778), zero, 1e-12)) {
            rot3(&at<double>(mv, 0x760), o, d, &a);
            rot3(&at<double>(mv, 0x778), o, d, &a);
            v3::sub(&at<double>(mv, 0x778), z0, &at<double>(mv, 0x778));
            v3::normalize(&at<double>(mv, 0x778));
        }
    } else {
        rot2(&at<double>(mv, 0x630), &a, o);
    }
}

// @src 1402596a0  WPAXIS record: types 1, 3: both vectors as points; others: +8 as point, +0x20 as
// a direction (R(v) - R(0), normalised) unless ~0 (1e-6)
void wpaxis_item_rotate(unsigned char* it, const double* o, const double* d, double angle)
{
    double z0[3] = {0.0, 0.0, 0.0};
    if (o == nullptr || d == nullptr) return;
    double a = angle;
    if (at<std::int32_t>(it, 4) == 1 || at<std::int32_t>(it, 4) == 3) {
        rot3(&at<double>(it, 8), o, d, &a);
        rot3(&at<double>(it, 0x20), o, d, &a);
    } else {
        rot3(&at<double>(it, 8), o, d, &a);
        if (!v3::near(&at<double>(it, 0x20), z0, 1e-6)) {  // DAT_14051f1b8
            rot3(z0, o, d, &a);
            rot3(&at<double>(it, 0x20), o, d, &a);
            v3::sub(&at<double>(it, 0x20), z0, &at<double>(it, 0x20));
            v3::normalize(&at<double>(it, 0x20));
        }
    }
}

// @src 14025a480  every WPAXIS record
void wpaxis_rotate(unsigned char* list, const double* o, const double* d, double a)
{
    auto& l = *reinterpret_cast<model::linked::Cls_140258580List*>(list);
    for (std::int32_t k = 0; k < model::geo_kw::wpaxis_count(l); k = k + 1) {
        unsigned char* it = reinterpret_cast<unsigned char*>(model::geo_kw::wpaxis_nth(l, k + 1));
        if (it != nullptr) wpaxis_item_rotate(it, o, d, a);
    }
}

// @src 14021f390  process: REFPOS point when type 1; 3D ROTSYM point and axis (unless ~0, 1e-6),
// WPAXIS records
void process_rotate(unsigned char* pr, const double* o, const double* d, double angle)
{
    double z0[3] = {0.0, 0.0, 0.0};
    double a = angle;
    if (at<std::int32_t>(pr, 0x28) == 1) {
        if (at<DB_STEP_DATA*>(pr, 0x2b8)->dimension != 2) rot3(&at<double>(pr, 0x30), o, d, &a);
        else rot2(&at<double>(pr, 0x30), &a, o);
    }
    if (at<DB_STEP_DATA*>(pr, 0x2b8)->dimension == 3) {
        if (!v3::near(&at<double>(pr, 0x1d8), z0, 1e-6)) {
            rot3(&at<double>(pr, 0x1c0), o, d, &a);
            rot3(z0, o, d, &a);
            rot3(&at<double>(pr, 0x1d8), o, d, &a);
            v3::sub(&at<double>(pr, 0x1d8), z0, &at<double>(pr, 0x1d8));
            v3::normalize(&at<double>(pr, 0x1d8));
        }
        wpaxis_rotate(pr + 0x200, o, d, a);
    }
}

// @src 140220190  GEO23 block frames (gated by +0x81 / +0xd0, never set in batch): every vector
// rotated as a point
void geo23_rotate(unsigned char* g, const double* o, const double* d, double angle)
{
    double a = angle;
    if (at<char>(g, 0x81) != 0) {
        rot3(&at<double>(g, 0x88), o, d, &a);
        rot3(&at<double>(g, 0xa0), o, d, &a);
        rot3(&at<double>(g, 0xb8), o, d, &a);
    }
    if (at<char>(g, 0xd0) != 0) {
        rot3(&at<double>(g, 0xd8), o, d, &a);
        rot3(&at<double>(g, 0xf0), o, d, &a);
        rot3(&at<double>(g, 0x108), o, d, &a);
    }
}

// @src 14019f2c0  windows of the inter-object entries owned by object `no`
void model_rotate_owned(DB_STEP_DATA* m, const double* o, const double* d, const double* angle, std::int32_t no)
{
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        LLOBJ_DATA* ob = static_cast<LLOBJ_DATA*>(model::step_object_at(*m, i + 1));
        if (ob == nullptr || at<std::int32_t>(ob, 0xc) == 7 || at<std::int32_t>(ob, 8) != no) continue;
        const std::int32_t id = at<std::int32_t>(ob, 8);
        for (std::int32_t k = 0; k < model::pair_count(&m->inter_obj); k = k + 1) {
            model::Cls_1401f46e0* e = model::pair_nth(&m->inter_obj, k + 1);
            if (e == nullptr || model::winlist_count(&e->u188) <= 0) continue;
            if (!((e->u170 == 2 && id == e->u168) || (e->u170 == 1 && id == e->u16c))) continue;
            for (std::int32_t j = 0; j < model::winlist_count(&e->u188); j = j + 1) {
                LLWINDEF_DATA* w = model::winlist_nth(&e->u188, j + 1);
                if (w != nullptr) window_rotate(w, o, d, *angle);
            }
        }
    }
}

// @src 140281660  cross-section placement M_item = M_item * M (unless flag +0x31)
void cross_section_rotate(model::GEO2D_AND_MESH2D* g, const model::linked::Matrix4* m)
{
    if (m == nullptr || g->u31 != 0) return;
    auto* mi = reinterpret_cast<model::linked::Matrix4*>(g->matrix);
    model::linked::Matrix4& r = mi->multiply_by(*m);
    mi->assign(r);
}

// @src 1402002b0  rigid geometry: 3D surfaces (virtual rotate, needs dir) then the cross sections
// of the geometry list with the 4x4 matrix of (a*pi)/180; 2D the curves of geometry 0
void geometry_rotate(LLOBJ_DATA* o, const double* origin, const double* dir, double angle)
{
    if (!owner_is_3d(o)) {
        if (origin != nullptr && model::geo_kw::geometry_has_edges(*o)) {
            for (std::int32_t i = 0; i < model::geo_kw::curve_count(*o); i = i + 1) {
                model::geo_kw::curve(*o, i)->rotate(origin, dir, angle);
            }
        }
    } else if (origin != nullptr && model::geo_kw::has_surface_flag(*o) && dir != nullptr) {
        for (std::int32_t i = 0; i < model::geo_kw::surface_count(*o); i = i + 1) {
            model::geo_kw::surface(*o, i)->rotate(origin, dir, angle);
        }
        if (model::geo_kw::geometry_has_edges(*o)) {
            const double rad = (angle * 3.141592653589793) / 180.0;  // DAT_14051f230, DAT_14051f228
            for (std::int32_t k = 0; k < model::geo_kw::geometry_count(*o); k = k + 1) {
                model::GEO2D_AND_MESH2D* g = model::geo_kw::geometry(*o, k);
                model::linked::Matrix4 m;
                model::linked::Matrix4 r = model::linked::rotation_about_axis(origin, dir, &rad);
                m.assign(r);
                cross_section_rotate(g, &m);
            }
        }
    }
}

// @src 140181db0  node set (obj+0x8a0): RZ in place (when *coords), then DRZ, URZ, FRZ, FRZB, PRZ,
// PRZB as R(v) - R(0); in 2D a vector field only when its width is 2. 3D needs dir.
// @src 14019a390  (inlined here) node set dimension test: +0x138 == 2
// @src 14019a3c0  (inlined here) node set dimension test: +0x138 == 3
void nodes_rotate(unsigned char* ns, const double* origin, const double* dir, double angle, const char* coords)
{
    double z0[3] = {0.0, 0.0, 0.0};
    double a = angle;
    if (origin == nullptr || node_widths(ns) == nullptr || at<double*>(ns, 8) == nullptr) return;
    const bool is2 = at<std::int32_t>(ns, 0x138) == 2;  // 14019a390
    const bool is3 = at<std::int32_t>(ns, 0x138) == 3;  // 14019a3c0
    if (!is2) {
        if (is3) {
            if (dir == nullptr) return;
            rot3(z0, origin, dir, &a);
        }
    } else {
        rot2(z0, &a, origin);
    }
    const std::int32_t* wd = node_widths(ns);
    const std::int32_t n = at<std::int32_t>(ns, 300);
    if (*coords != 0 && at<double*>(ns, 8) != nullptr) {
        const std::int32_t w = wd[0];
        for (std::int32_t i = 0; i < n; i = i + 1) {
            double* p = at<double*>(ns, 8) + i * w;
            if (!is2) {
                if (is3) rot3(p, origin, dir, &a);
            } else {
                rot2(p, &a, origin);
            }
        }
    }
    for (int f = 1; f <= 6; ++f) {
        double* arr = at<double*>(ns, 8 + 8 * static_cast<std::size_t>(f));
        if (arr == nullptr) continue;
        const std::int32_t w = wd[f];
        if (is2 && w != 2) continue;
        for (std::int32_t i = 0; i < n; i = i + 1) {
            double* v = arr + i * w;
            if (!is2) {
                if (is3) {
                    rot3(v, origin, dir, &a);
                    v3::sub(v, z0, v);  // 14023b1f0
                }
            } else {
                rot2(v, &a, origin);
                v3::sub2(v, z0, v);  // 140308fb0
            }
        }
    }
}

// @src 1402006c0  mesh (when +0x87c): nodes, element tensors, FLDGEO / FLWNET surfaces
void mesh_rotate(LLOBJ_DATA* o, const double* origin, const double* dir, double angle)
{
    if (at<std::int32_t>(o, 0x87c) == 0 || origin == nullptr) return;
    const char coords = 1;
    nodes_rotate(ptr(o, 0x8a0), origin, dir, angle, &coords);
    elems_rotate(ptr(o, 0x9e0), origin, dir, angle);
    if (model::geo_kw::has_fldgeo(*o)) reinterpret_cast<model::OBJ_GEO3D*>(ptr(o, 0x3328))->rotate(origin, dir, angle);
    if (model::geo_kw::has_flwnet(*o)) reinterpret_cast<model::OBJ_GEO3D*>(ptr(o, 0x3458))->rotate(origin, dir, angle);
}

} // namespace

// @src 14021d2f0
void rotate(LLOBJ_DATA* o, const double* origin, const double* dir, double angle_deg)
{
    double a = angle_deg;
    if (!(1e-30 <= std::fabs(a))) return;  // DAT_14051bee0
    geometry_rotate(o, origin, dir, a);
    mesh_rotate(o, origin, dir, a);
    remesh_rotate(ptr(o, 0x2d10), origin, dir, a);
    bcwin_rotate(ptr(o, 0x2e68), origin, dir, a);
    if (has_die_geometry(o)) movement_rotate(ptr(o, 0x2160), origin, dir, a);
    process_rotate(ptr(o, 0x1ea0), origin, dir, a);
    geo23_rotate(ptr(o, 0x2be8), origin, dir, a);
    if (o->owner != nullptr) model_rotate_owned(o->owner, origin, dir, &a, at<std::int32_t>(o, 8));
}

} // namespace def::cmd::objpos
