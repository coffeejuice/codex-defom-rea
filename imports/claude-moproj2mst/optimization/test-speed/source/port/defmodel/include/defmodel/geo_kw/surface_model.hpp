// defmodel/geo_kw/surface_model.hpp: the surface side of an OBJ_GEO3D as the kw-g2geo surface
// family uses it (packet kw-g2geo): sizing, element ids / patch ids (POLYTP), and the die point and
// die edge sets (OBJ_GEO3D +0x50 / +0x58, DIEPNT / DIEEDG). Decompiles:
// deform-rea-work/out/g2geo/fam_surface.c.
//
// Both set members are 8-byte COW list handles (bodies 0x18: refs, sentinel node, count):
//   +0x50  ctor 1402fedd0, dtor 1402fee40, clear 1402ff680; nodes 0x28 = next, prev, 0x18-byte
//          DiePointSet (body clone 140300540, node ctors 1403000c0 / 1403000f0)
//   +0x58  ctor 1402fef20, dtor 1402fef90, clear 1402ff790; nodes 0x28 with a 0x18-byte
//          DieEdgeSet (clone 140300780, node ctors 140300120 / 140300150)
// (wired in defmodel/src/geo/pending_wiring.cpp). Copying a record shares its inner list handle
// (140300220 / 140300280: three ints, then the handle copy = refs++), which is what CowList's
// copy ctor does; the record dtors (1402fea60 / 1402feb20) clear the inner list (1402ff5f0 /
// 14015a620) and release it, which for a shared handle is the same as releasing it.
#pragma once

#include <cstdint>

#include <defmodel/sdctl/cow.hpp>

namespace def::model {
class LLOBJ_DATA;
class OBJ_GEO3D;
}  // namespace def::model

namespace def::model::geo_kw {

// a die point: x, y, z, p4, p5 (0x28 bytes, the payload of a 0x38-byte list node)
struct DiePoint {
    double v[5] = {0, 0, 0, 0, 0};

    // @src 1402fe850  x, y, z
    void set_xyz(double x, double y, double z);
    // @src 1402fe8a0  +0x18
    void set_p4(double p);
    // @src 1402fe8c0  +0x20
    void set_p5(double p);
    // @src 1402b7420  +0x00
    double x() const;
    // @src 1402b7430  +0x08
    double y() const;
    // @src 1402b7440  +0x10
    double z() const;
    // @src 1402b7460  +0x18
    double p4() const;
    // @src 1402b7450  +0x20
    double p5() const;
};
static_assert(sizeof(DiePoint) == 0x28);

// point-set record, 0x18 bytes in the original node
struct DiePointSet {
    std::int32_t id = 0;       // +0x00
    std::int32_t type = 0;     // +0x04
    std::int32_t extra = 0;    // +0x08
    CowList<DiePoint> points;  // +0x10 (1402b74b0; handle ctor 1402fecf0, body 1402ff930)

    // 1402fea10 ctor: the list, id = type = extra = 0 (the default member initialisers)

    // @src 1402feaa0
    void set_id(const std::int32_t* v);
    // @src 1402feac0
    std::int32_t get_id() const;
    // @src 1402fe8e0
    void set_type(const std::int32_t* v);
    // @src 1402b7470
    std::int32_t get_type() const;
    // @src 1402fe900
    void set_extra(const std::int32_t* v);
    // @src 1402b7480  extra when type == 1, else 0
    std::int32_t extra_if_type1() const;
    // @src 14019a000  number of points (14019a800 on +0x10)
    std::int32_t point_count() const;
};

// edge-set record, 0x18 bytes in the original node
struct DieEdgeSet {
    std::int32_t id = 0;          // +0x00
    std::int32_t type = 0;        // +0x04
    std::int32_t extra = 0;       // +0x08
    CowList<std::int32_t> nodes;  // +0x10 (1402b7510; ctor 140159860: 8-byte node values, of
                                  // which the original reads and writes only the low 4 bytes)

    // 1402fead0 ctor: the list, id = type = extra = 0 (the default member initialisers)

    // @src 1402feb60
    void set_id(const std::int32_t* v);
    // @src 1402feb80
    std::int32_t get_id() const;
    // @src 1402fe930
    void set_type(const std::int32_t* v);
    // @src 1402b74d0
    std::int32_t get_type() const;
    // @src 1402fe950
    void set_extra(const std::int32_t* v);
    // @src 1402b74e0  extra when type == 1, else 0
    std::int32_t extra_if_type1() const;
    // @src 14019a060  number of nodes (140159950 on +0x10)
    std::int32_t node_count() const;
};

using DiePointSets = CowList<DiePointSet>;
using DieEdgeSets = CowList<DieEdgeSet>;
static_assert(sizeof(DiePointSets) == 8 && sizeof(DieEdgeSets) == 8);

// The handles inside an OBJ_GEO3D (its u50 / u58 members): 1402b74c0 / 1402b7520.
DiePointSets& die_point_sets(void* geo3d);
DieEdgeSets& die_edge_sets(void* geo3d);

// ---- OBJ_GEO3D surface operations (free functions over the class of p1-06) -------------------

// 140006410: &coords[0] after a detach (null without nodes)
double* geo3d_coords(OBJ_GEO3D* s);
// 140005510: &elems[0] after a detach (null without elements)
std::int32_t* geo3d_elems(OBJ_GEO3D* s);
// 140199fa0: &elem_ids[0] after a detach (null without ids)
std::int32_t* geo3d_elem_ids(OBJ_GEO3D* s);
// 1402fcd90: resize nodes (1402f9600) and elements (1402f96a0), new element flags (1402f9970),
// resize the element ids (1402f9a50)
void geo3d_size(OBJ_GEO3D* s, const std::int32_t* nnode, const std::int32_t* nelem);
// 1402fbb30: empty (FLWNET calls it after reading a mesh)
void geo3d_fun_1402fbb30(OBJ_GEO3D* s);
// 1402fbb70: +0x48 (patch filter)
std::int32_t geo3d_patch_filter(OBJ_GEO3D* s);
// 1402fd8a0: 1000 < v < 2000
bool is_patch_type(std::int32_t v);
// 1402fd7f0: element i1 (1-based) has a patch id that passes the +0x48 filter
bool geo3d_elem_in_patch(OBJ_GEO3D* s, std::int32_t i1);
// 1402fce80: clear the patch list +0x40 when not empty, +0x48 = 0
void geo3d_clear_patches(OBJ_GEO3D* s);
// 1402fced0: rebuild the patch list +0x40 from the element ids (id - 1000, unique, in order)
void geo3d_rebuild_patches(OBJ_GEO3D* s);
// the patch list +0x40: count, entry i (the low 32 bits of each 8-byte value)
std::int32_t geo3d_patch_count(OBJ_GEO3D* s);
std::int32_t geo3d_patch_at(OBJ_GEO3D* s, std::int32_t i);

// 1402f93b0 / 1402f93f0: the point / edge set list is not empty
bool geo3d_has_point_sets(OBJ_GEO3D* s);
bool geo3d_has_edge_sets(OBJ_GEO3D* s);
// 1402fd020: find the point set with this id (updating its type when it differs) or append a
// new one {id, type, 0, empty}; returns it
DiePointSet* geo3d_point_set_for(OBJ_GEO3D* s, const std::int32_t* id, const std::int32_t* type);
// 1402fd3e0: point set k (1-based); out of range -> a static empty record (1406fed80)
DiePointSet* geo3d_point_set(OBJ_GEO3D* s, const std::int32_t* k);
// 1402fd490 / 1402fd740: the same for edge sets (static record 1406feda0)
DieEdgeSet* geo3d_edge_set_for(OBJ_GEO3D* s, const std::int32_t* id, const std::int32_t* type);
DieEdgeSet* geo3d_edge_set(OBJ_GEO3D* s, const std::int32_t* k);

// list operations of the original, on the CowList handles
void point_sets_push_back(DiePointSets& l, const DiePointSet& r);  // 1402fee60
DiePointSet& point_sets_at(DiePointSets& l, std::int32_t i);       // 1402feeb0
void edge_sets_push_back(DieEdgeSets& l, const DieEdgeSet& r);     // 1402fefb0
DieEdgeSet& edge_sets_at(DieEdgeSets& l, std::int32_t i);          // 1402ff000
void points_clear(CowList<DiePoint>& l);                           // 1402ff5f0
void points_push_back(CowList<DiePoint>& l, const DiePoint& p);    // 1402fed80
DiePoint& points_at(CowList<DiePoint>& l, std::int32_t i);         // 1402b7560
std::int32_t& ints_at(CowList<std::int32_t>& l, std::int32_t i);   // 140159a70

// ---- object-level predicates (LLOBJ_DATA) ------------------------------------------------------
// 140160db0: surface flag (+0x3990), surface 0 not null, it has point sets
bool object_has_die_points(LLOBJ_DATA& obj);
// 140160e00: the same for edge sets
bool object_has_die_edges(LLOBJ_DATA& obj);
// 140203a20: surface flag, and surface 0 has nodes and elements
bool object_has_surface_mesh(LLOBJ_DATA& obj);

}  // namespace def::model::geo_kw
