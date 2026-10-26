// GEO2D_AND_MESH2D topology helpers and the small field accessors of the 2D topology classes
// that sit in packet p1-08's range (0x1402838d0-0x140284b80), plus OBJ_MESH2D getters.
//
// GEO2D_AND_MESH2D (RTTI, 0xc8 bytes, base C3dCrossSection) is p1-07's class; _DEF_FACE is p1-07's,
// _DEF_LOOP/_DEF_EDGE/_DEF_VERTEX are p1-11's, OBJ_MESH2D is p1-13's. Until their layout
// headers land, these functions read the recovered offsets through mesh2d::field<T>() and call
// the other packets' functions through mesh2d::Ext (function pointers, DEF_NOT_PORTED by
// default). Both get replaced by named members and direct calls when the headers exist.
//
// GEO2D_AND_MESH2D fields used here:
//   +0x08 FaceList faces   +0x10 LoopList loops   +0x18 EdgeList edges   +0x20 VertexList vertices
//   +0x30 uint8 (1402849b0/1402849d0)   +0x38 0x80-byte block (p1-06, 140220540)   +0xb8 uint8
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/mesh2d/cow_list.hpp>

namespace def::model {

class GEO2D_AND_MESH2D;
class DEF_FACE;
class DEF_LOOP;
class DEF_EDGE;
class DEF_VERTEX;
class OBJ_GEO_BASE;
class OBJ_MESH2D;

namespace mesh2d {

template <class T>
T& field(void* obj, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(obj) + off);
}
template <class T>
const T& field(const void* obj, std::size_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const std::byte*>(obj) + off);
}

// Calls into other packets, until their code lands (tests install fakes).
struct Ext {
    DEF_FACE* (*new_face)();     // operator new(0x48) + 1402f7b20 (p1-07)
    DEF_LOOP* (*new_loop)();     // operator new(0x20) + 140412010 (p1-11)
    DEF_VERTEX* (*new_vertex)(); // operator new(0x28) + 14041ce30 (p1-11)
    DEF_FACE* (*face_cast)(IFace*);       // __RTDynamicCast IFace -> _DEF_FACE
    DEF_LOOP* (*loop_cast)(ILoop*);       // __RTDynamicCast ILoop -> _DEF_LOOP
    DEF_VERTEX* (*vertex_cast)(IVertex*); // __RTDynamicCast IVertex -> _DEF_VERTEX
    DEF_EDGE* (*edge_at)(GEO2D_AND_MESH2D*, std::int32_t);    // 1402800a0 (p1-07)
    void (*edge_set_number)(DEF_EDGE*, const std::int32_t*);  // 14041c5f0 (p1-11): +0x30
    void (*edge_set_curve_flag)(DEF_EDGE*, std::uint8_t);     // 14041cd40 (p1-11)
    std::int32_t (*geo_slot11)(OBJ_GEO_BASE*);                // virtual slot 11 (vtable +0x58)
};
Ext& ext();       // current table
void reset_ext(); // back to the DEF_NOT_PORTED defaults

} // namespace mesh2d

// ---- GEO2D_AND_MESH2D -------------------------------------------------------------------------

FaceList& geo2d_faces(GEO2D_AND_MESH2D* g);      // +0x08
LoopList& geo2d_loops(GEO2D_AND_MESH2D* g);      // +0x10
EdgeList& geo2d_edges(GEO2D_AND_MESH2D* g);      // +0x18
VertexList& geo2d_vertices(GEO2D_AND_MESH2D* g); // +0x20

// 1402838d0 / 140283980 / 140283a30: new face / loop / vertex numbered count+1, appended.
DEF_FACE* geo2d_add_face(GEO2D_AND_MESH2D* g);
DEF_LOOP* geo2d_add_loop(GEO2D_AND_MESH2D* g);
DEF_VERTEX* geo2d_add_vertex(GEO2D_AND_MESH2D* g);

// 140283f80 / 140284030 / 1402840e0: element *i (0-based) cast to the class, or nullptr when
// the list is empty, *i < 0, *i >= count or the cast fails.
DEF_FACE* geo2d_face_at(GEO2D_AND_MESH2D* g, const std::int32_t* i);
DEF_LOOP* geo2d_loop_at(GEO2D_AND_MESH2D* g, const std::int32_t* i);
DEF_VERTEX* geo2d_vertex_at(GEO2D_AND_MESH2D* g, const std::int32_t* i);

// 140284190 / 140284220 / 1402842b0 / 140284340: number vertices / edges / loops / faces 1..n
// in list order.
void geo2d_renumber_vertices(GEO2D_AND_MESH2D* g);
void geo2d_renumber_edges(GEO2D_AND_MESH2D* g);
void geo2d_renumber_loops(GEO2D_AND_MESH2D* g);
void geo2d_renumber_faces(GEO2D_AND_MESH2D* g);

// 1402847d0: +0xb8 = *v; when it becomes 0, every edge gets edge_set_curve_flag(edge, 0).
void geo2d_set_b8(GEO2D_AND_MESH2D* g, const char* v);

// 1402849a0 / 1402849b0 / 1402849d0
void* geo2d_block_38(GEO2D_AND_MESH2D* g);
void geo2d_set_30(GEO2D_AND_MESH2D* g, std::uint8_t v);
std::uint8_t geo2d_get_30(const GEO2D_AND_MESH2D* g);

// 140284960: o, unless its virtual slot 11 returns nonzero (then nullptr).
OBJ_GEO_BASE* geo_if_slot11_zero(OBJ_GEO_BASE* o);

// ---- topology field accessors --------------------------------------------------------------

OBJ_GEO_BASE* edge_curve(const DEF_EDGE* e);                 // 140284990: +0x08
std::uint8_t edge_get_34(const DEF_EDGE* e);                 // 140284a10
std::uint8_t edge_get_35(const DEF_EDGE* e);                 // 140284a20
void edge_set_34(DEF_EDGE* e, std::uint8_t v);               // 140284a30
void edge_set_35(DEF_EDGE* e, std::uint8_t v);               // 140284a50
void edge_clear_34_35(DEF_EDGE* e);                          // 140284af0
void loop_set_1c(DEF_LOOP* l, std::uint8_t v);               // 1402849e0
std::uint8_t loop_get_1c(const DEF_LOOP* l);                 // 140284a00
void loop_set_number(DEF_LOOP* l, const std::int32_t* n);    // 140284b60: +0x18
void vertex_set_20(DEF_VERTEX* v, std::uint8_t b);           // 140284a70
std::uint8_t vertex_get_20(const DEF_VERTEX* v);             // 140284a90
void vertex_set_number(DEF_VERTEX* v, const std::int32_t* n); // 140284b80: +0x24
void face_set_number(DEF_FACE* f, const std::int32_t* n);    // 140284b40: +0x44

// ---- OBJ_MESH2D ----------------------------------------------------------------------------

// 140284aa0: 140182840(mesh + 8) = int at mesh+0x8+0x12c (node count of the member at +8)
std::int32_t mesh2d_count_08(const OBJ_MESH2D* m);
// 140284ac0: 1401854f0(mesh + 0x148) = int at mesh+0x148+0xe8 (count of the member at +0x148)
std::int32_t mesh2d_count_148(const OBJ_MESH2D* m);

} // namespace def::model
