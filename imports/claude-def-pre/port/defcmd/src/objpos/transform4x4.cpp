// The 4x4 transform of a successful drop, FUN_1401fe240 = 1401fe290 (rigid surfaces) + 1401fe310
// (nodes, FLDGEO / FLWNET) + 1401fd730 (windows, movement, process, GEO23, display placements,
// inter-object windows). Double precision; the matrix is the Matrix4 layout (row-vector convention,
// translation in row 3): p'_j = (((0.0 + M(0,j) x) + M(1,j) y) + M(2,j) z) + M(3,j) (listing
// l_xf.txt, 140241270).
#include <defcmd/objpos.hpp>
#include <defcmd/objpos_mirror.hpp>
#include <defcmd/vec3.hpp>

#include <cstdint>

#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/linked/matrix4.hpp>
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

// rdata 140509c90
constexpr double kOne = def::rt::rdata::d_140509c90;
// rdata 14051f260
constexpr double kTiny = def::rt::rdata::d_14051f260;
// rdata 14051f1b8
constexpr double kTiny6 = def::rt::rdata::d_14051f1b8;

// the 4-component product of 140241270 / 14030a170
void xf4_vec(const double* m16, const double* v, double* out)
{
    for (std::int16_t j = 0; j < 4; j = j + 1) {
        double s = 0.0;
        for (std::int16_t k = 0; k < 4; k = k + 1) s = s + m16[k * 4 + j] * v[k];
        out[j] = s;
    }
}

} // namespace

// @src 140269fb0  every window of a window list
static void winlist_xf4(model::Cls_140269250* l, const double* m16)
{
    for (std::int32_t k = 0; k < model::winlist_count(l); k = k + 1) {
        LLWINDEF_DATA* w = model::winlist_nth(l, k + 1);
        if (w != nullptr) window_xf4(w, m16);
    }
}

// @src 140257830  axis-point record: point +8 transformed; vector +0x20 -> M(v) - M(0) (14023b1f0),
// normalised (14023b720)
static void axis_point_xf4(model::linked::Cls_140257340* r, const double* m16)
{
    double z[3] = {0.0, 0.0, 0.0};
    fun_140241270(m16, r->point);
    fun_140241270(m16, z);
    fun_140241270(m16, r->direction);
    v3::sub(r->direction, z, r->direction);
    v3::normalize(r->direction);
}

// @src 140258490  every record of an axis-point list
static void axis_list_xf4(model::linked::Cls_140257340List* l, const double* m16)
{
    for (std::int32_t k = 0; k < l->size(); k = k + 1) {
        model::linked::Cls_140257340* r = l->nth(k + 1);
        if (r != nullptr) axis_point_xf4(r, m16);
    }
}

// @src 1401fe290  rigid surfaces obj+0x3c00[k] (k < +0x3994) when the object has them (+0x3990)
void fun_1401fe290(LLOBJ_DATA* o, double* m16)
{
    if (!model::geo_kw::has_surface_flag(*o)) return;
    for (std::int32_t k = 0; k < model::geo_kw::surface_count(*o); k = k + 1) {
        geo3d_xf4(model::geo_kw::surface(*o, k), m16);
    }
}

// @src 1401fe3a0  FLDGEO / FLWNET surfaces
void fun_1401fe3a0(LLOBJ_DATA* o, double* m16)
{
    if (model::geo_kw::has_fldgeo(*o)) geo3d_xf4(reinterpret_cast<model::OBJ_GEO3D*>(ptr(o, 0x3328)), m16);
    if (model::geo_kw::has_flwnet(*o)) geo3d_xf4(reinterpret_cast<model::OBJ_GEO3D*>(ptr(o, 0x3458)), m16);
}

// @src 1401fe310  nodes (RZ, 3 per node, when +0x87c), then 1401fe3a0
void fun_1401fe310(LLOBJ_DATA* o, double* m16)
{
    if (at<std::int32_t>(o, 0x87c) != 0) {
        for (std::int32_t i = 0; i < at<std::int32_t>(o, 0x880); i = i + 1) {
            fun_140241270(m16, at<double*>(o, 0x8a8) + i * 3);
        }
    }
    fun_1401fe3a0(o, m16);
}

// @src 14021f730  remesh windows (obj+0x2d10, list +0x40)
static void remesh_xf4(unsigned char* rm, const double* m16)
{
    winlist_xf4(reinterpret_cast<model::Cls_140269250*>(rm + 0x40), m16);
}

// @src 14021f8d0  BC windows (obj+0x2e68): 3D SYMSUF list +0x18, window lists +0x78, +0xa8
static void bcwin_xf4(unsigned char* bc, const double* m16)
{
    if (model::model_is_3d(at<DB_STEP_DATA*>(bc, 0x198))) {  // 1400015f0
        axis_list_xf4(reinterpret_cast<model::linked::Cls_140257340List*>(bc + 0x18), m16);
    }
    winlist_xf4(reinterpret_cast<model::Cls_140269250*>(bc + 0x78), m16);
    winlist_xf4(reinterpret_cast<model::Cls_140269250*>(bc + 0xa8), m16);
}

// @src 14021dfa0  movement: 3D: for CNTRAX (+0x630 centre, +0x648 axis) and CNTRA2 (+0x760,
// +0x778), when the axis is not ~0 (1e-12): centre transformed, axis -> M(a) - M(0), normalised;
// 2D: CNTRAX centre by 14030a170
static void movement_xf4(unsigned char* mv, const double* m16)
{
    double z[3] = {0.0, 0.0, 0.0};
    const double zero[3] = {0.0, 0.0, 0.0};
    if (!model::model_is_2d(at<DB_STEP_DATA*>(mv, 0xa80))) fun_140241270(m16, z);
    if (!model::model_is_2d(at<DB_STEP_DATA*>(mv, 0xa80))) {
        for (std::size_t c : {std::size_t{0x630}, std::size_t{0x760}}) {
            double* axis = &at<double>(mv, c + 0x18);
            if (!v3::near(axis, zero, kTiny)) {
                fun_140241270(m16, &at<double>(mv, c));
                fun_140241270(m16, axis);
                v3::sub(axis, z, axis);
                v3::normalize(axis);
            }
        }
    } else {
        xf4_point_2d(m16, &at<double>(mv, 0x630));
    }
}

// @src 14021f550  process: REFPOS point +0x30 when the type +0x28 is 1 (2D 14030a170); 3D ROTSYM
// point +0x1c0 and axis +0x1d8 (-> M(a) - M(0), normalised) when the axis is not ~0 (1e-6)
static void process_xf4(unsigned char* pr, const double* m16)
{
    double z[3] = {0.0, 0.0, 0.0};
    if (at<std::int32_t>(pr, 0x28) == 1) {
        if (!model::model_is_2d(at<DB_STEP_DATA*>(pr, 0x2b8))) {
            fun_140241270(m16, &at<double>(pr, 0x30));
        } else {
            xf4_point_2d(m16, &at<double>(pr, 0x30));
        }
    }
    if (model::model_is_3d(at<DB_STEP_DATA*>(pr, 0x2b8))) {
        double* axis = &at<double>(pr, 0x1d8);
        if (!v3::near(axis, z, kTiny6)) {
            fun_140241270(m16, &at<double>(pr, 0x1c0));
            fun_140241270(m16, z);
            fun_140241270(m16, axis);
            v3::sub(axis, z, axis);
            v3::normalize(axis);
        }
    }
}

// @src 1402202a0  GEO23 block (obj+0x2be8): frames gated by +0x81 / +0xd0 (never set in batch):
// vectors transformed as points and normalised, points transformed
static void geo23_xf4(unsigned char* g, const double* m16)
{
    if (at<char>(g, 0x81) != 0) {
        fun_140241270(m16, &at<double>(g, 0x88));
        v3::normalize(&at<double>(g, 0x88));
        fun_140241270(m16, &at<double>(g, 0xa0));
        fun_140241270(m16, &at<double>(g, 0xb8));
    }
    if (at<char>(g, 0xd0) != 0) {
        fun_140241270(m16, &at<double>(g, 0xd8));
        v3::normalize(&at<double>(g, 0xd8));
        fun_140241270(m16, &at<double>(g, 0xf0));
        v3::normalize(&at<double>(g, 0xf0));
        fun_140241270(m16, &at<double>(g, 0x108));
    }
}

// @src 140281b80  display placement: Matrix4 (identity 140220540) := m16 (1402854d0), then
// M_item = M_item * M (140281660, unless the entry's flag +0x31)
// @src 1402854d0  (inlined here) Matrix4 := 4x4 doubles, element by element, rows i, columns j
//                 (also inlined in 140281710, mirror3d.cpp)
static void cross_section_xf4(model::GEO2D_AND_MESH2D* g, const double* m16)
{
    model::linked::Matrix4 t;
    for (std::int32_t i = 0; i < 4; i = i + 1) {
        for (std::int32_t j = 0; j < 4; j = j + 1) t.m[i * 4 + j] = m16[i * 4 + j];
    }
    if (g->u31 == 0) {
        auto* mi = reinterpret_cast<model::linked::Matrix4*>(g->matrix);
        model::linked::Matrix4& r = mi->multiply_by(t);  // 140221a90
        mi->assign(r);                                   // 140220670
    }
}

// @src 14019f4a0  windows of the inter-object entries owned by object `no`
static void model_xf4_owned(DB_STEP_DATA* m, const double* m16, std::int32_t no)
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
                if (w != nullptr) window_xf4(w, m16);
            }
        }
    }
}

// @src 1401fd730  the object's other parts
void fun_1401fd730(LLOBJ_DATA* o, double* m16)
{
    remesh_xf4(ptr(o, 0x2d10), m16);
    bcwin_xf4(ptr(o, 0x2e68), m16);
    movement_xf4(ptr(o, 0x2160), m16);
    process_xf4(ptr(o, 0x1ea0), m16);
    geo23_xf4(ptr(o, 0x2be8), m16);
    const std::int32_t ng = model::geo_kw::geometry_count(*o);  // 1401983e0
    for (std::int32_t i = 0; i < ng; i = i + 1) cross_section_xf4(model::geo_kw::geometry(*o, i), m16);
    if (o->owner != nullptr) model_xf4_owned(o->owner, m16, at<std::int32_t>(o, 8));
}

// @src 140241270  p := (p, 1) * M (sum from 0.0, k ascending; x, y, z kept)
void fun_140241270(const double* m16, double* p)
{
    const double v[4] = {p[0], p[1], p[2], kOne};
    double out[4];
    xf4_vec(m16, v, out);
    p[0] = out[0];
    p[1] = out[1];
    p[2] = out[2];
}

// @src 14030a170  p := (p.x, p.y, 0.0, 1) * M (x, y kept)
void xf4_point_2d(const double* m16, double* p)
{
    const double v[4] = {p[0], p[1], 0.0, kOne};
    double out[4];
    xf4_vec(m16, v, out);
    p[0] = out[0];
    p[1] = out[1];
}

// @src 140268290  window points (2D windows 14030a170, else 140241270; 2 or 3 doubles per point)
// when the window has points; 3D windows then refresh their faces (140267760)
void window_xf4(LLWINDEF_DATA* w, const double* m16)
{
    if (w->pts == nullptr) return;
    const std::int32_t width = model::window_is_2d(w) ? 2 : 3;
    for (std::int32_t i = 0; i < w->npts; i = i + 1) {
        if (!model::window_is_2d(w)) {
            fun_140241270(m16, w->pts + i * width);
        } else {
            xf4_point_2d(m16, w->pts + i * width);
        }
    }
    if (model::window_is_3d(w)) model::window_build_faces(w);
}

// @src 1402fccf0  _OBJ_GEO3D: every point (written through 140005e70) when there are faces and
// points
void geo3d_xf4(model::OBJ_GEO3D* g, const double* m16)
{
    const std::int32_t points = g->node_count();
    const std::int32_t faces = g->elem_count();
    if (0 < faces && 0 < points) {
        for (std::int32_t i = 0; i < points; i = i + 1) {
            fun_140241270(m16, &g->coords.mutable_at(static_cast<std::size_t>(i * 3)));
        }
    }
}

// @src 1401fe240  drop transform
void transform_4x4(LLOBJ_DATA* o, const double* m16)
{
    double* m = const_cast<double*>(m16);  // the binary's parts take a non-const pointer
    fun_1401fe290(o, m);
    fun_1401fe310(o, m);
    fun_1401fd730(o, m);
}

} // namespace def::cmd::objpos
