// gendb3d/fold.cpp: 3D fold meshes FLDGEO (obj+0x3328) / FLWNET (obj+0x3458) into LLFLD / IIFLD
// (packet p4-05; docs/re/dbmap/objarrays.md section 7).
#include <defdbio/gendb3d/obj3d.hpp>

#include <cstdlib>

#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/accessors.hpp>

#include "gendb3d/access.hpp"
#include "gendb3d/ext_obj3d.hpp"

namespace def::dbio::g3d {

using model::OBJ_GEO3D;

// @src 1402f9df0  copy src into dst (1402f9b60); with *negate the 4th node of every element
//                 becomes -abs(.) (140484ab8 = abs)
static void copy_geo_negated(OBJ_GEO3D* dst, OBJ_GEO3D* src, const char* negate)
{
    dst->assign(src);
    if (*negate != '\0') {
        const std::int32_t ne = dst->elem_count();
        for (std::int32_t e = 0; e < ne; e = e + 1) {
            const std::int32_t v = std::abs(dst->elems.mutable_at(static_cast<std::size_t>(e * 4 + 3)));
            dst->elems.mutable_at(static_cast<std::size_t>(e * 4 + 3)) = -v;
        }
    }
}

// @src 1402fa4f0  dst := nodes of a then b, elements of a then b (b's node numbers + nodes of a;
//                 with *negate b's 4th node is -abs(. + nodes of a))
static void merge_geo(OBJ_GEO3D* dst, OBJ_GEO3D* ga, OBJ_GEO3D* gb, const char* negate)
{
    std::int32_t nodes = 0;
    std::int32_t elems = 0;
    std::int32_t na = ga->node_count();
    nodes = gb->node_count();
    nodes = na + nodes;
    std::int32_t ea = ga->elem_count();
    elems = gb->elem_count();
    elems = ea + elems;
    ext::f1402f9780(dst, &nodes, &elems);
    const double* ca = model::linked::member10_array_first(ga);      // 14026a3c0
    const std::int32_t* xa = ext::f1402fe9d0(ga);
    const double* cb = model::linked::member10_array_first(gb);
    const std::int32_t* xb = ext::f1402fe9d0(gb);
    for (std::int32_t k = 0; k < ga->node_count(); k = k + 1) {
        for (std::int32_t c = 0; c < 3; c = c + 1) {
            dst->coords.mutable_at(static_cast<std::size_t>(k * 3 + c)) = ca[k * 3 + c];
        }
    }
    const std::int32_t base = ga->node_count();
    for (std::int32_t k = 0; k < gb->node_count(); k = k + 1) {
        for (std::int32_t c = 0; c < 3; c = c + 1) {
            dst->coords.mutable_at(static_cast<std::size_t>((base + k) * 3 + c)) = cb[k * 3 + c];
        }
    }
    for (std::int32_t k = 0; k < ga->elem_count(); k = k + 1) {
        for (std::int32_t c = 0; c < 4; c = c + 1) {
            dst->elems.mutable_at(static_cast<std::size_t>(c + k * 4)) = xa[c + k * 4];
        }
    }
    const std::int32_t ebase = ga->elem_count();
    for (std::int32_t k = 0; k < gb->elem_count(); k = k + 1) {
        for (std::int32_t c = 0; c < 4; c = c + 1) {
            std::int32_t v = xb[c + k * 4] + base;
            if (c == 3 && *negate != '\0') {
                v = std::abs(xb[k * 4 + 3] + base);
                v = -v;
            }
            dst->elems.mutable_at(static_cast<std::size_t>(c + (ebase + k) * 4)) = v;
        }
    }
}

// @src 14016da70  IIOBJ words 15..18 = fold node / face ranges (running offsets m+0xd0 / 0xd4);
//                 nodes (3 reals) to llfld, faces (4 ints, nodes + first - 1, 4th sign kept) to iifld
void write_fold_info(LLOBJ_DATA* obj, std::int32_t* iiobj, double* llfld, std::int32_t* iifld,
                     std::int32_t* n_nodes, std::int32_t* n_faces)
{
    iiobj[0x3c / 4] = 0;
    iiobj[0x40 / 4] = 0;
    iiobj[0x44 / 4] = 0;
    iiobj[0x48 / 4] = 0;
    const bool has_geo = ext::f140160e50(obj);
    const bool has_net = ext::f140160eb0(obj);
    if (!has_geo && !has_net) return;
    iiobj[0x3c / 4] = fold_node_offset(owner_of(obj)) + 1;
    iiobj[0x40 / 4] = fold_node_offset(owner_of(obj)) + ext::f140167d70(obj);
    iiobj[0x44 / 4] = fold_face_offset(owner_of(obj)) + 1;
    iiobj[0x48 / 4] = fold_face_offset(owner_of(obj)) + ext::f140167e00(obj);
    *n_nodes = (iiobj[0x40 / 4] - iiobj[0x3c / 4]) + 1;
    *n_faces = (iiobj[0x48 / 4] - iiobj[0x44 / 4]) + 1;
    OBJ_GEO3D tmp; // 1402f9020 ... 1402f91f0
    OBJ_GEO3D* g = nullptr;
    auto* geo = reinterpret_cast<OBJ_GEO3D*>(at(obj, 0x3328));
    auto* net = reinterpret_cast<OBJ_GEO3D*>(at(obj, 0x3458));
    if (!has_geo || has_net) {
        if (!has_geo && has_net) {
            const char neg = 1;
            copy_geo_negated(&tmp, net, &neg);
            g = &tmp;
        }
        else {
            const char neg = 1;
            merge_geo(&tmp, geo, net, &neg);
            g = &tmp;
        }
    }
    else {
        g = geo;
    }
    const double* xyz = ext::f140006410(g);
    for (std::int32_t k = 0; k < *n_nodes; k = k + 1) {
        llfld[k * 3] = xyz[k * 3];
        llfld[k * 3 + 1] = xyz[k * 3 + 1];
        llfld[k * 3 + 2] = xyz[k * 3 + 2];
    }
    const std::int32_t off = iiobj[0x3c / 4] - 1;
    const std::int32_t* f = ext::f140005510(g);
    for (std::int32_t k = 0; k < *n_faces; k = k + 1) {
        iifld[k << 2] = f[k << 2] + off;
        iifld[k * 4 + 1] = f[k * 4 + 1] + off;
        iifld[k * 4 + 2] = f[k * 4 + 2] + off;
        const std::int32_t v = f[k * 4 + 3];
        if (v < 1) {
            iifld[k * 4 + 3] = v - off;
        }
        else {
            iifld[k * 4 + 3] = v + off;
        }
    }
}

} // namespace def::dbio::g3d
