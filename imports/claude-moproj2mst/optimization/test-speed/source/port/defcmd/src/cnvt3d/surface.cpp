// CNVT3D: surface-patch helpers of 3D object surfaces (surface 0, obj+0x3c00[0]): cut-plane faces
// of a partial revolve and the patch list (point list at OBJ_GEO3D+0x28, id + 600 per face in the
// face flags +0x20). Packet p9-01; 1402059a0, 14023c770 and 14023e050 forward to their Phase 4 owner
// (defdbio/gendb/pre_steps.hpp). Comparisons follow the listing (NaN behaviour of comisd/jbe noted per test).
#include <defcmd/cnvt3d.hpp>

#include <cmath>
#include <cstdlib>

#include <defcmd/geolib.hpp>
#include <defcmd/vec3.hpp>
#include <defdbio/gendb/pre_steps.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defrt/stdlib/libm.hpp>

#include "internal.hpp"

namespace def::cmd::cnvt3d {

using namespace detail;
using model::OBJ_GEO3D;
using model::linked::Cls_140257340;
using model::linked::Cls_140257340List;

namespace {

Cls_140257340List* patches(OBJ_GEO3D* s) { return reinterpret_cast<Cls_140257340List*>(s->u28); }  // 140158e60

// 140258260: the smallest id >= 1 not used in the list (1402580f0 lookups)
std::int32_t next_free_id(Cls_140257340List* l)
{
    std::int32_t id = 1;
    while (model::geo_kw::symsuf_find(*l, id) != nullptr) id = id + 1;
    return id;
}

double length(const double* v) { return v3::len(v3::make(v)); }  // 140239a30

} // namespace

// 14023c770 and 14023e050: owner Phase 4 (defdbio gendb pre_steps.hpp, surface_normals.cpp)
double plane_distance(const double* p, const double* n, const double* q)
{
    return dbio::gendb::plane_distance(p, n, q);
}

void triangle_normal(const double* a, const double* b, const double* c, double* n)
{
    dbio::gendb::triangle_normal(a, b, c, n);
}

// 1402059a0: owner Phase 4 (defdbio gendb pre_steps.hpp, object_surface_normals)
void rebuild_patches(LLOBJ_DATA* o, char copy_axis) { dbio::gendb::object_surface_normals(o, copy_axis); }

// @src 140205e40
void assign_patches(LLOBJ_DATA* o, char copy_axis)
{
    OBJ_GEO3D* s0 = model::geo_kw::surface(*o, 0);
    if (patches(s0)->size() == 0) {
        rebuild_patches(o, copy_axis);
        return;
    }
    if (!model::geo_kw::has_surface_flag(*o)) return;
    if (copy_axis != 0) patches(s0)->copy_from(*reinterpret_cast<Cls_140257340List*>(add(o, 0x2e80)));
    OBJ_GEO3D* s = model::geo_kw::surface(*o, 0);
    const double cl = s->char_length();
    const std::int32_t nf = s->elem_count();
    double* xyz = s->coords.mutable_begin();
    std::int32_t* el = s->elems.mutable_begin();
    std::int32_t* ids = s->elem_flags;
    Cls_140257340List* l = patches(s);
    for (std::int32_t f = 0; f < nf; f = f + 1) {
        if (el[f * 4 + 3] > 0) continue;
        const std::int32_t i0 = el[f * 4] - 1;
        double n[3];
        triangle_normal(xyz + i0 * 3, xyz + (el[f * 4 + 1] - 1) * 3, xyz + (el[f * 4 + 2] - 1) * 3, n);
        const double len = length(n);
        if (1e-6 > len) continue;
        Cls_140257340* node = nullptr;
        bool found = false;
        for (std::int32_t k = 0; k < l->size(); k = k + 1) {
            node = l->nth(k + 1);
            if (node == nullptr) continue;
            const double d2 = v3::dist2(n, node->direction);
            if (!(0.01 > d2)) continue;
            const double d = plane_distance(xyz + i0 * 3, node->direction, node->point);
            if (0.001 * cl > d) {
                found = true;
                break;
            }
        }
        ids[f] = found ? node->id + 600 : 0;
    }
}

// @src 140198d30
void mark_cut_faces(LLOBJ_DATA* o, double start, double ang)
{
    OBJ_GEO3D* s = reinterpret_cast<OBJ_GEO3D*>(o->u3c00[0]);
    const std::int32_t nf = s->elem_count();
    std::int32_t* el = s->elems.mutable_begin();
    double* xyz = s->coords.mutable_begin();
    if (!(360.0 > ang)) {  // comisd 360, ang; jbe
        rebuild_patches(o, 0);
        return;
    }
    constexpr double kPi = 3.141592653589793;  // DAT_14051f230
    const double a1 = ((start + 90.0) * kPi) / 180.0;
    const double n1[3] = {rt::libm::crt_cos(a1), rt::libm::crt_sin(a1), 0.0};  // 140bb3490, 140bb3000
    const double a2 = (((start + ang) + 90.0) * kPi) / 180.0;
    const double n2[3] = {rt::libm::crt_cos(a2), rt::libm::crt_sin(a2), 0.0};
    const double q[3] = {0.0, 0.0, 0.0};
    for (std::int32_t f = 0; f < nf; f = f + 1) {
        auto* p0 = static_cast<double*>(std::malloc(0x18));
        auto* p1 = static_cast<double*>(std::malloc(0x18));
        auto* p2 = static_cast<double*>(std::malloc(0x18));
        for (int j = 0; j < 3; ++j) p0[j] = xyz[(el[f * 4] - 1) * 3 + j];
        for (int j = 0; j < 3; ++j) p1[j] = xyz[(el[f * 4 + 1] - 1) * 3 + j];
        for (int j = 0; j < 3; ++j) p2[j] = xyz[(el[f * 4 + 2] - 1) * 3 + j];
        const bool in1 = plane_distance(p0, n1, q) <= 1e-8 && plane_distance(p1, n1, q) <= 1e-8 &&
                         plane_distance(p2, n1, q) <= 1e-8;
        const bool in2 = !in1 && plane_distance(p0, n2, q) <= 1e-8 && plane_distance(p1, n2, q) <= 1e-8 &&
                         plane_distance(p2, n2, q) <= 1e-8;
        if (in1 || in2) el[f * 4 + 3] = -el[f * 4 + 3];
        std::free(p0);
        std::free(p1);
        std::free(p2);
    }
    rebuild_patches(o, 0);
    bool removed = false;
    Cls_140257340List* l = patches(reinterpret_cast<OBJ_GEO3D*>(o->u3c00[0]));
    for (std::int32_t k = l->size(); -1 < k; k = k - 1) {
        Cls_140257340* node = l->nth(k + 1);
        if (node == nullptr) continue;
        const double* dir = node->direction;
        double l1 = length(dir);
        double l2 = length(n1);
        bool par = std::fabs(std::fabs(geolib::fun_14023a380(dir, n1)) - l1 * l2) < 1e-6;
        if (!par) {
            l1 = length(dir);
            l2 = length(n2);
            par = std::fabs(std::fabs(geolib::fun_14023a380(dir, n2)) - l1 * l2) < 1e-6;
        }
        if (!par) {
            removed = true;
            dbio::objcheck::plane_list_remove(l, node);  // 140257f70
        }
    }
    if (removed) assign_patches(o, 0);
}

} // namespace def::cmd::cnvt3d
