// p4-02b: LLOBJ_DATA helpers the counts frame calls (mesh/geometry ranges, material groups,
// boundary node list, 2D outline entity count).
#include <defdbio/gendb/counts_frame.hpp>

#include <cstdlib>
#include <intrin.h>
#include <cstring>

#include <defdbio/gendb/counts_ext.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/members/ftn_geom.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/statevar/geo2d_kernels.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::dbio::gendb {

using model::field;

namespace {

// operator new 1404797d8 of a byte count computed as count * size with the overflow check
// (SUB168(ZEXT816(size) * count, 8) != 0 -> -1, i.e. the allocation fails)
void* new_array(std::int64_t count, std::uint64_t size)
{
    unsigned __int64 hi = 0;
    unsigned __int64 lo = _umul128(static_cast<unsigned __int64>(count), size, &hi);
    if (hi != 0) {
        lo = ~0ull;
    }
    return std::malloc(static_cast<std::size_t>(lo));
}

std::int32_t obj_kind(LLOBJ_DATA* o) { return field<std::int32_t>(o, 0xc); }
std::int32_t obj_nodes(LLOBJ_DATA* o) { return field<std::int32_t>(o, 0x8a0 + 0x12c); } // 140182840
std::int32_t obj_elems(LLOBJ_DATA* o) { return field<std::int32_t>(o, 0x9e0 + 0xe8); }  // 1401854f0
std::int32_t* obj_groups(LLOBJ_DATA* o) { return field<std::int32_t*>(o, 0x9f8); }     // per element

// 1402f9360: the _OBJ_GEO3D has nodes and elements
bool geo3d_filled(const model::OBJ_GEO3D* g) { return 0 < g->node_count() && 0 < g->elem_count(); }

} // namespace

// @src 14015ff30  mark the materials of the element groups as used (+4 = 1) and or `flags` into
// their +8; false (and *bad = the group) when a group is not a material number
bool object_mark_groups(LLOBJ_DATA* o, std::uint32_t flags, std::int32_t* bad)
{
    bool ok = true;
    if (o->owner != nullptr) {
        std::int32_t nmax = material_list_max_number(&o->owner->materials);
        for (std::int32_t i = 0; i < obj_elems(o); i = i + 1) {
            std::int32_t g = obj_groups(o)[i];
            if (g < 1 || nmax < g) {
                ok = false;
                *bad = g;
            } else {
                Material* mat = model::material_list_find(&o->owner->materials, g);
                if (mat == nullptr) {
                    ok = false;
                    *bad = g;
                } else {
                    field<std::int32_t>(mat, 4) = 1;
                    field<std::uint32_t>(mat, 8) = field<std::uint32_t>(mat, 8) | flags;
                }
            }
        }
    }
    return ok;
}

// @src 140160260  collect the phases of the mixture materials used by the element groups into the
// object's group list (count +0x3590, table u3598); *single = 1 and *matno = the material when a
// group is a phase material (MSTMTR flag 2) without components
bool object_group_list(LLOBJ_DATA* o, bool* single, std::int32_t* matno)
{
    if (o->owner == nullptr) {
        return true;
    }
    MaterialList* ml = &o->owner->materials;
    std::int32_t nmax = material_list_max_number(ml);
    auto* used = static_cast<std::int32_t*>(new_array(nmax + 1, 4));
    auto* phase = static_cast<std::int32_t*>(new_array(nmax + 1, 4));
    auto* list = static_cast<std::int32_t*>(new_array(nmax + 1, 4));
    for (std::int32_t i = 0; i <= nmax; i = i + 1) {
        used[i] = 0;
        phase[i] = 0;
        list[i] = 0;
    }
    std::int32_t ne = obj_elems(o);
    for (std::int32_t e = 0; e < ne; e = e + 1) {
        std::int32_t g = obj_groups(o)[e];
        if (0 < g && g <= nmax) {
            used[g] = 1;
        }
    }
    *single = false;
    *matno = 0;
    for (std::int32_t i = 1; i <= nmax; i = i + 1) {
        if (used[i] != 1) {
            continue;
        }
        Material* mat = model::material_list_find(ml, i);
        if (mat == nullptr) {
            continue;
        }
        if (mat->mstmtr_count < 1) {
            if (mat->mstmtr_flag == 2) {
                *single = true;
                *matno = i;
            }
        } else {
            for (std::int32_t k = 0; k < mat->mstmtr_count; k = k + 1) {
                std::int32_t p = mat->mstmtr_list[k];
                if (0 < p && p <= nmax) {
                    phase[p] = 1;
                }
            }
        }
    }
    std::int32_t& cnt = field<std::int32_t>(o, 0x3590);
    cnt = 0;
    for (std::int32_t i = 1; i <= nmax; i = i + 1) {
        if (phase[i] == 1) {
            list[cnt] = i;
            cnt = cnt + 1;
        }
    }
    dynary_set_int_list(&o->u3598, cnt, list);
    std::free(used);
    std::free(phase);
    std::free(list);
    return true;
}

// @src 140160c20  true when the object's mesh is not required: the simulation is in the mode of
// 1401f3d90 (kind 0 objects only), or a kind 0 object has fewer than 4 nodes or no elements
bool object_skip_mesh(LLOBJ_DATA* o)
{
    bool r = false;
    if (o->owner != nullptr && gendb_ext().fun_1401f3d90(&o->owner->sim_ctrl) && obj_kind(o) == 0) {
        r = true;
    }
    if (obj_kind(o) == 0) {
        if (3 < obj_nodes(o) && 0 < obj_elems(o)) {
            return r;
        }
        r = true;
    }
    return r;
}

// @src 140160cc0  2D model: the object has 2D curves (140160d20); else the flag u3990 (140160d90)
bool object_has_geometry(LLOBJ_DATA* o)
{
    if (o->owner != nullptr && rt::dimension_is_2(o->owner)) {
        return gendb_ext().fun_140160d20(o);
    }
    return o->u3990 != 0; // 140160d90
}

// @src 140167d70  nodes of the two _OBJ_GEO3D surfaces (s3328, s3458) that have nodes and elements
std::int32_t object_geo3d_nodes(LLOBJ_DATA* o)
{
    std::int32_t n = 0;
    auto* a = reinterpret_cast<model::OBJ_GEO3D*>(o->s3328);
    auto* b = reinterpret_cast<model::OBJ_GEO3D*>(o->s3458);
    if (geo3d_filled(a)) {
        n = a->node_count();
    }
    if (geo3d_filled(b)) {
        n = n + b->node_count();
    }
    return n;
}

// @src 140167e00  elements of the same two surfaces
std::int32_t object_geo3d_elems(LLOBJ_DATA* o)
{
    std::int32_t n = 0;
    auto* a = reinterpret_cast<model::OBJ_GEO3D*>(o->s3328);
    auto* b = reinterpret_cast<model::OBJ_GEO3D*>(o->s3458);
    if (geo3d_filled(a)) {
        n = a->elem_count();
    }
    if (geo3d_filled(b)) {
        n = n + b->elem_count();
    }
    return n;
}

// @src 1401ff5b0  coordinate range of the mesh nodes; false when the object has no mesh (+0x87c)
bool object_mesh_range(LLOBJ_DATA* o, double* min, double* max)
{
    if (o->u87c[0] == 0) {
        return false;
    }
    return node_coord_range(o->s8a0, min, max);
}

// @src 140181ae0  per-coordinate min/max over the node array (+8 doubles, count +0x12c, width =
// first int of the owner's table at +0x15f8); false when there is no owner or no data.
// 140181c48: if (min > v) min = v; else if (v > max) max = v (COMISD/JBE, unordered keeps both)
bool node_coord_range(void* nodes, double* min, double* max)
{
    bool first = true;
    std::int32_t* tbl = gendb_ext().fun_14017f020(nodes);
    double* data = field<double*>(nodes, 0x8);
    if (tbl == nullptr || data == nullptr) {
        return false;
    }
    std::int32_t w = *gendb_ext().fun_14017f020(nodes);
    std::int32_t n = field<std::int32_t>(nodes, 0x12c);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        if (first) {
            for (std::int32_t j = 0; j < w; j = j + 1) {
                std::memcpy(&min[j], &data[i * w + j], 8);
                std::memcpy(&max[j], &data[i * w + j], 8);
            }
            first = false;
        } else {
            for (std::int32_t j = 0; j < w; j = j + 1) {
                double v = data[i * w + j];
                if (min[j] > v) {
                    min[j] = v;
                } else if (v > max[j]) {
                    max[j] = v;
                }
            }
        }
    }
    return true;
}

// @src 1401ff600  min/max over the bounding boxes of the 2D geometry curves; false when none
// (1401ff766: if (min > cmin) min = cmin; if (cmax > max) max = cmax)
bool object_geo2d_range(LLOBJ_DATA* o, double* min, double* max)
{
    double cmin[2] = {0.0, 0.0};
    double cmax[2] = {0.0, 0.0};
    double radius = 0.0;
    bool first = true;
    bool any = false;
    if (!object_has_geometry(o)) {
        return false;
    }
    for (std::int32_t i = 0; i < gendb_ext().fun_140198310(o); i = i + 1) {
        OBJ_GEO2D* g = gendb_ext().fun_1401982c0(o, &i);
        if (g->bounding_box(cmin, cmax, &radius)) {
            if (!first) {
                for (std::int32_t k = 0; k < 2; k = k + 1) {
                    if (min[k] > cmin[k]) {
                        min[k] = cmin[k];
                    }
                    if (cmax[k] > max[k]) {
                        max[k] = cmax[k];
                    }
                }
            } else {
                for (std::int32_t k = 0; k < 2; k = k + 1) {
                    min[k] = cmin[k];
                    max[k] = cmax[k];
                }
                first = false;
            }
            any = true;
        }
    }
    return any;
}

// @src 1402039d0  the object has 2D curves and they pass 140304e40(o, 0)
bool object_geo2d_usable(LLOBJ_DATA* o)
{
    bool r = gendb_ext().fun_140160d20(o);
    if (r && !gendb_ext().fun_140304e40(o, 0)) {
        r = false;
    }
    return r;
}

// @src 14017e990  3D: rebuild the sorted list of boundary nodes (u008, count u000) from the
// boundary faces (u010 quads in u018); the list is reallocated only when the count changes
void boundary_node_list(Cls_14017e740* b, const std::int32_t* nnode)
{
    if (!rt::dimension_is_3(b->u028)) {
        return;
    }
    auto* mark = static_cast<char*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(*nnode))));
    for (std::int32_t i = 0; i < *nnode; i = i + 1) {
        mark[i] = 0;
    }
    auto* faces = static_cast<std::int32_t*>(b->u018);
    for (std::int32_t i = 0; i < b->u010; i = i + 1) {
        for (std::int32_t j = 0; j < 4; j = j + 1) {
            mark[faces[j + i * 4] - 1] = 1;
        }
    }
    std::int32_t cnt = 0;
    for (std::int32_t i = 0; i < *nnode; i = i + 1) {
        if (mark[i] != 0) {
            cnt = cnt + 1;
        }
    }
    if (cnt != b->u000) {
        b->u000 = cnt;
        if (b->u008 != nullptr) {
            std::free(b->u008);
        }
        b->u008 = nullptr;
        b->u008 = gendb_ext().fun_140256dc0(b->u000);
    }
    cnt = 0;
    auto* list = static_cast<std::int32_t*>(b->u008);
    for (std::int32_t i = 0; i < *nnode; i = i + 1) {
        if (mark[i] != 0) {
            list[cnt] = i + 1;
            cnt = cnt + 1;
        }
    }
    std::free(mark);
}

// @src 140276c70  number of DB entities of a 2D outline: the points of a segment curve (type 2),
// else the entities the outline kernel 14032b34b makes of its points
std::int32_t geo2d_entity_count(OBJ_GEO2D* g)
{
    std::int32_t nent[2];
    if (g->curve_type == 2) {
        nent[0] = g->n_points;
    } else {
        nent[0] = 0;
        std::int32_t n = g->n_points;
        if (g->fun_1402765e0()) {
            n = g->n_points;
        }
        std::int32_t sz = n * 0x3c;
        auto* buf = static_cast<double*>(new_array(sz, 8));
        std::memset(buf, 0, static_cast<std::size_t>(static_cast<std::int64_t>(sz) << 3));
        model::fun_140345f5b(g->points, &n);
        model::geo2d::points_to_entities(&n, g->points, nent, buf);
        std::free(buf);
    }
    return nent[0];
}

// @src 1401320a0  table of type 0x7c: ncomp = n values list[i] (as doubles), axis[0][0] = 0
void dynary_set_int_list(DYNARY* t, std::int32_t n, const std::int32_t* list)
{
    if (t->type != 0x7c) {
        return;
    }
    t->ncomp = n;
    model::dynary_alloc(t);
    std::uint64_t zero = 0;
    std::memcpy(t->axis[0], &zero, 8);
    for (std::int32_t i = 0; i < t->ncomp; i = i + 1) {
        t->values[i] = static_cast<double>(list[i]);
    }
}

} // namespace def::dbio::gendb
