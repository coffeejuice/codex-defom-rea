// OBJPOS translate tree (FUN_14021d0b0 and callees). Spec: docs/re/commands/OBJPOS.md 6.1.
// Every translation is x = x + d per component (one rounding each).
#include <defcmd/objpos.hpp>
#include <defmodel/node_access.hpp>
#include <defcmd/objpos_parts.hpp>
#include <defcmd/vec3.hpp>

#include <cstdint>

#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>

#include "internal.hpp"

namespace def::cmd::objpos {

using model::Cls_140269250;
using model::DB_STEP_DATA;
using model::LLOBJ_DATA;
using model::LLWINDEF_DATA;
using namespace detail;

// @src 140268040  window points += d (2 or 3 comps by the window's dimension); 3D windows then
// refresh their faces (140267760)
void window_translate(LLWINDEF_DATA* w, const double* d)
{
    if (d == nullptr || w->pts == nullptr) return;
    const std::int32_t width = model::window_is_2d(w) ? 2 : 3;
    for (std::int32_t i = 0; i < w->npts; i = i + 1) {
        for (std::int32_t j = 0; j < width; j = j + 1) {
            w->pts[i * width + j] = w->pts[i * width + j] + d[j];
        }
    }
    if (model::window_is_3d(w)) model::window_build_faces(w);
}

// @src 140269eb0  every window of the list
void winlist_translate(Cls_140269250* l, const double* d)
{
    for (std::int32_t k = 0; k < model::winlist_count(l); k = k + 1) {
        LLWINDEF_DATA* w = model::winlist_nth(l, k + 1);
        if (w != nullptr) window_translate(w, d);
    }
}

namespace {

// @src 14021f6b0  remesh windows (obj+0x2d10, list +0x40)
void remesh_translate(unsigned char* rm, const double* d)
{
    winlist_translate(reinterpret_cast<Cls_140269250*>(rm + 0x40), d);
}

// @src 1402576e0  SYMSUF record point += d (3 comps)
void axis_point_translate(model::linked::Cls_140257340* p, const double* d)
{
    if (d == nullptr) return;
    for (std::int32_t j = 0; j < 3; j = j + 1) p->point[j] = p->point[j] + d[j];
}

// @src 140258390  every record of the list
void axis_list_translate(model::linked::Cls_140257340List* l, const double* d)
{
    for (std::int32_t k = 0; k < l->size(); k = k + 1) {
        model::linked::Cls_140257340* p = l->nth(k + 1);
        if (p != nullptr) axis_point_translate(p, d);
    }
}

// @src 14021f7c0  BC windows (obj+0x2e68): 3D SYMSUF list +0x18, window lists +0x78 and +0xa8
void bcwin_translate(unsigned char* bc, const double* d)
{
    if (at<DB_STEP_DATA*>(bc, 0x198)->dimension == 3) {  // 1400015f0 on the owner +0x198
        axis_list_translate(reinterpret_cast<model::linked::Cls_140257340List*>(bc + 0x18), d);
    }
    winlist_translate(reinterpret_cast<Cls_140269250*>(bc + 0x78), d);
    winlist_translate(reinterpret_cast<Cls_140269250*>(bc + 0xa8), d);
}

// @src 14021dba0  movement (obj+0x2160): CNTRAX centre +0x630 (3D also CNTRA2 +0x760) += d; for
// movement type 4 (+0x68) STROKE +0x170 += dir_raw * projection of d on the unit direction
// @src 140308e80  (inlined here) 2D copy of the direction: out[0..1] = in[0..1] (2 doubles)
void movement_translate(unsigned char* mv, const double* d)
{
    if (d == nullptr) return;
    double dir[3] = {0.0, 0.0, 0.0};
    std::int32_t n;
    const DB_STEP_DATA* owner = at<DB_STEP_DATA*>(mv, 0xa80);
    if (owner->dimension != 2) {
        n = 3;
        v3::copy(&at<double>(mv, 0x50), dir);  // 14023b050
    } else {
        n = 2;
        dir[0] = at<double>(mv, 0x50);  // 140308e80
        dir[1] = at<double>(mv, 0x58);
    }
    for (std::int32_t j = 0; j < n; j = j + 1) {
        at<double>(mv, 0x630 + 8 * j) = at<double>(mv, 0x630 + 8 * j) + d[j];
        if (at<DB_STEP_DATA*>(mv, 0xa80)->dimension == 3) {
            at<double>(mv, 0x760 + 8 * j) = at<double>(mv, 0x760 + 8 * j) + d[j];
        }
    }
    if (at<std::int32_t>(mv, 0x68) == 4) {
        const double s = movctl_projection(mv, d, &at<double>(mv, 0x50));
        for (std::int32_t j = 0; j < n; j = j + 1) {
            at<double>(mv, 0x170 + 8 * j) = at<double>(mv, 0x170 + 8 * j) + dir[j] * s;
        }
    }
}

// @src 1402595a0  WPAXIS record: +8 += d; types 1, 3 also +0x20; type 4 also +0x98
void wpaxis_item_translate(unsigned char* it, const double* d)
{
    if (d == nullptr) return;
    for (std::int32_t j = 0; j < 3; j = j + 1) {
        at<double>(it, 8 + 8 * j) = at<double>(it, 8 + 8 * j) + d[j];
        const std::int32_t type = at<std::int32_t>(it, 4);
        if (type == 1 || type == 3) at<double>(it, 0x20 + 8 * j) = at<double>(it, 0x20 + 8 * j) + d[j];
        if (at<std::int32_t>(it, 4) == 4) at<double>(it, 0x98 + 8 * j) = at<double>(it, 0x98 + 8 * j) + d[j];
    }
}

// @src 14025a410  every WPAXIS record (count 14019a550, record 140259b60)
void wpaxis_translate(unsigned char* list, const double* d)
{
    auto& l = *reinterpret_cast<model::linked::Cls_140258580List*>(list);
    for (std::int32_t k = 0; k < model::geo_kw::wpaxis_count(l); k = k + 1) {
        unsigned char* it = reinterpret_cast<unsigned char*>(model::geo_kw::wpaxis_nth(l, k + 1));
        if (it != nullptr) wpaxis_item_translate(it, d);
    }
}

// @src 14021f210  process (obj+0x1ea0): REFPOS point +0x30 when type +0x28 == 1; 3D: ROTSYM
// point +0x1c0 unless the axis +0x1d8 is ~0 (1e-6), WPAXIS list +0x200
void process_translate(unsigned char* pr, const double* d)
{
    double zero[3] = {0.0, 0.0, 0.0};
    if (d == nullptr) return;
    const std::int32_t n = at<DB_STEP_DATA*>(pr, 0x2b8)->dimension != 2 ? 3 : 2;
    if (at<std::int32_t>(pr, 0x28) == 1) {
        for (std::int32_t j = 0; j < n; j = j + 1) at<double>(pr, 0x30 + 8 * j) = at<double>(pr, 0x30 + 8 * j) + d[j];
    }
    if (at<DB_STEP_DATA*>(pr, 0x2b8)->dimension == 3) {
        if (!v3::near(&at<double>(pr, 0x1d8), zero, 1e-6)) {  // DAT_14051f1b8
            for (std::int32_t j = 0; j < n; j = j + 1) {
                at<double>(pr, 0x1c0 + 8 * j) = at<double>(pr, 0x1c0 + 8 * j) + d[j];
            }
        }
        wpaxis_translate(pr + 0x200, d);
    }
}

// @src 140220080  GEO23 block (obj+0x2be8): frames gated by +0x81 / +0xd0 (never set in batch)
void geo23_translate(unsigned char* g, const double* d)
{
    if (at<char>(g, 0x81) != 0) {
        for (std::int32_t j = 0; j < 3; j = j + 1) {
            at<double>(g, 0xa0 + 8 * j) = at<double>(g, 0xa0 + 8 * j) + d[j];
            at<double>(g, 0xb8 + 8 * j) = at<double>(g, 0xb8 + 8 * j) + d[j];
        }
    }
    if (at<char>(g, 0xd0) != 0) {
        for (std::int32_t j = 0; j < 3; j = j + 1) at<double>(g, 0x108 + 8 * j) = at<double>(g, 0x108 + 8 * j) + d[j];
    }
}

// @src 14018b190  remesh windows of `rm` whose owner number (+0x36c) is *no
void remesh_translate_following(unsigned char* rm, const double* d, const std::int32_t* no)
{
    Cls_140269250* l = reinterpret_cast<Cls_140269250*>(rm + 0x40);
    for (std::int32_t k = 0; k < model::winlist_count(l); k = k + 1) {
        LLWINDEF_DATA* w = model::winlist_nth(l, k + 1);
        if (w != nullptr && w->orient_id == *no) window_translate(w, d);
    }
}

// @src 140197850  BC window lists +0x48, +0x78, +0xa8, +0x168 of `bc`, windows owned by *no
void bcwin_translate_following(unsigned char* bc, const double* d, const std::int32_t* no)
{
    for (std::size_t off : {std::size_t{0x48}, std::size_t{0x78}, std::size_t{0xa8}, std::size_t{0x168}}) {
        Cls_140269250* l = reinterpret_cast<Cls_140269250*>(bc + off);
        for (std::int32_t k = 0; k < model::winlist_count(l); k = k + 1) {
            LLWINDEF_DATA* w = model::winlist_nth(l, k + 1);
            if (w != nullptr && w->orient_id == *no) window_translate(w, d);
        }
    }
}

// @src 14021d290  windows of object o that follow object *no
void object_translate_following(LLOBJ_DATA* o, const double* d, const std::int32_t* no)
{
    remesh_translate_following(ptr(o, 0x2d10), d, no);
    bcwin_translate_following(ptr(o, 0x2e68), d, no);
}

// @src 14019ef20  windows of other objects and of inter-object entries not owned by `no` that
// follow object `no`
void model_translate_following(DB_STEP_DATA* m, const double* d, std::int32_t no)
{
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        LLOBJ_DATA* o = static_cast<LLOBJ_DATA*>(model::step_object_at(*m, i + 1));
        if (o != nullptr && at<std::int32_t>(o, 0xc) != 7 && at<std::int32_t>(o, 8) != no) {  // 140160f10
            object_translate_following(o, d, &no);
        }
    }
    for (std::int32_t i = 0; i < model::pair_count(&m->inter_obj); i = i + 1) {
        model::Cls_1401f46e0* e = model::pair_nth(&m->inter_obj, i + 1);  // 14019d5e0
        if (e == nullptr || model::winlist_count(&e->u188) <= 0) continue;
        if (e->u170 == 2 && no == e->u168) continue;
        if (e->u170 == 1 && no == e->u16c) continue;
        for (std::int32_t k = 0; k < model::winlist_count(&e->u188); k = k + 1) {
            LLWINDEF_DATA* w = model::winlist_nth(&e->u188, k + 1);
            if (w != nullptr && w->orient_id == no) window_translate(w, d);
        }
    }
}

// @src 14019f100  windows of the inter-object entries owned by object `no` (master of a type 2
// entry or slave of a type 1 entry)
void model_translate_owned(DB_STEP_DATA* m, const double* d, std::int32_t no)
{
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        LLOBJ_DATA* o = static_cast<LLOBJ_DATA*>(model::step_object_at(*m, i + 1));
        if (o == nullptr || at<std::int32_t>(o, 0xc) == 7 || at<std::int32_t>(o, 8) != no) continue;
        const std::int32_t id = at<std::int32_t>(o, 8);
        for (std::int32_t k = 0; k < model::pair_count(&m->inter_obj); k = k + 1) {
            model::Cls_1401f46e0* e = model::pair_nth(&m->inter_obj, k + 1);
            if (e == nullptr || model::winlist_count(&e->u188) <= 0) continue;
            if (!((e->u170 == 2 && id == e->u168) || (e->u170 == 1 && id == e->u16c))) continue;
            for (std::int32_t j = 0; j < model::winlist_count(&e->u188); j = j + 1) {
                LLWINDEF_DATA* w = model::winlist_nth(&e->u188, j + 1);
                if (w != nullptr) window_translate(w, d);
            }
        }
    }
}

// @src 14021d190  windows, movement, process, GEO23, then windows of other objects / entries
void translate_parts(LLOBJ_DATA* o, const double* d, const char* flag_a)
{
    remesh_translate(ptr(o, 0x2d10), d);
    bcwin_translate(ptr(o, 0x2e68), d);
    movement_translate(ptr(o, 0x2160), d);
    process_translate(ptr(o, 0x1ea0), d);
    geo23_translate(ptr(o, 0x2be8), d);
    if (o->owner != nullptr && *flag_a != 0) model_translate_following(o->owner, d, at<std::int32_t>(o, 8));
    if (o->owner != nullptr) model_translate_owned(o->owner, d, at<std::int32_t>(o, 8));
}

// @src 140285550  4x4 placement: translation row (+0x60..+0x70) += d
void matrix_translate(unsigned char* mtx, const double* d)
{
    at<double>(mtx, 0x60) = at<double>(mtx, 0x60) + d[0];
    at<double>(mtx, 0x68) = at<double>(mtx, 0x68) + d[1];
    at<double>(mtx, 0x70) = at<double>(mtx, 0x70) + d[2];
}

// @src 1402816c0  cross-section placement unless its flag +0x31 (140284ae0) is set
// @src 140284ae0  (inlined here) GEO2D_AND_MESH2D flag +0x31 getter (also in 140281660)
void cross_section_translate(model::GEO2D_AND_MESH2D* g, const double* d)
{
    if (d != nullptr && g->u31 == 0) matrix_translate(reinterpret_cast<unsigned char*>(g->matrix), d);
}

// @src 1402000a0  rigid geometry: 3D surfaces (obj+0x3c00, virtual translate) and the cross
// sections of the geometry list; 2D the curves of geometry 0 (virtual translate)
void geometry_translate(LLOBJ_DATA* o, const double* d)
{
    if (!owner_is_3d(o)) {
        if (d != nullptr && model::geo_kw::geometry_has_edges(*o)) {
            for (std::int32_t i = 0; i < model::geo_kw::curve_count(*o); i = i + 1) {
                model::geo_kw::curve(*o, i)->translate(d);
            }
        }
    } else if (d != nullptr && model::geo_kw::has_surface_flag(*o)) {
        for (std::int32_t i = 0; i < model::geo_kw::surface_count(*o); i = i + 1) {
            model::geo_kw::surface(*o, i)->translate(d);
        }
        if (model::geo_kw::geometry_has_edges(*o)) {
            for (std::int32_t k = 0; k < model::geo_kw::geometry_count(*o); k = k + 1) {
                cross_section_translate(model::geo_kw::geometry(*o, k), d);
            }
        }
    }
}

// @src 140181cb0  node coordinates (node set +8, width *14017f020) += d
void nodes_translate(unsigned char* ns, const double* d)
{
    if (d == nullptr || node_widths(ns) == nullptr || at<double*>(ns, 8) == nullptr) return;
    const std::int32_t w = *node_widths(ns);
    double* x = at<double*>(ns, 8);
    for (std::int32_t i = 0; i < at<std::int32_t>(ns, 300); i = i + 1) {
        for (std::int32_t j = 0; j < w; j = j + 1) x[i * w + j] = x[i * w + j] + d[j];
    }
}

// @src 140200610  mesh (when +0x87c): nodes, then FLDGEO / FLWNET surfaces
void mesh_translate(LLOBJ_DATA* o, const double* d)
{
    if (at<std::int32_t>(o, 0x87c) == 0 || d == nullptr) return;
    nodes_translate(ptr(o, 0x8a0), d);
    if (model::geo_kw::has_fldgeo(*o)) reinterpret_cast<model::OBJ_GEO3D*>(ptr(o, 0x3328))->translate(d);
    if (model::geo_kw::has_flwnet(*o)) reinterpret_cast<model::OBJ_GEO3D*>(ptr(o, 0x3458))->translate(d);
}

} // namespace

// 140194f90: ported in defdbio/objcheck/objcheck.hpp (press_stroke_projection)
double movctl_projection(const unsigned char* mv, const double* d, const double* dir)
{
    return dbio::objcheck::press_stroke_projection(const_cast<unsigned char*>(mv), d, dir);
}

// 14017f020: ported in defmodel/node_access.hpp (fun_14017f020)
const std::int32_t* node_widths(const unsigned char* ns)
{
    return model::fun_14017f020(reinterpret_cast<model::Cls_14017ef20*>(const_cast<unsigned char*>(ns)));
}

// @src 14021d0b0
void translate(LLOBJ_DATA* o, const double* d, const char* flag_a, const char* flag_b)
{
    const double zero[3] = {0.0, 0.0, 0.0};
    if (d == nullptr) return;
    // 140158d00: the object's own dimension +0x39c0 (not the owner's)
    const bool tiny = at<std::int32_t>(o, 0x39c0) != 2 ? v3::near(d, zero, 1e-30) : v3::near2(d, zero, 1e-30);
    if (tiny) return;  // DAT_14051bee0
    if (*flag_b != 0) geometry_translate(o, d);
    mesh_translate(o, d);
    translate_parts(o, d, flag_a);
}

} // namespace def::cmd::objpos
