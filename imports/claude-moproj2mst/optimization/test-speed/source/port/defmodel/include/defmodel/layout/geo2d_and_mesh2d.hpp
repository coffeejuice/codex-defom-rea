// GEO2D_AND_MESH2D (RTTI .?AVGEO2D_AND_MESH2D@@, base C3dCrossSection): a 2D cross-section as
// topology (faces, loops, edges, vertices; each edge holds one _OBJ_GEO2D curve) plus an
// optional mesh (OBJ_MESH2D, owned) and a 4x4 placement matrix.
//
//   vtable   0x140538b58 (2 slots: scalar deleting dtor 140284900, class name 1402848f0; p1-08)
//   ctor     14027f6e0; copy ctor 14027f7a0 (= ctor, then copy_from 140280490)
//   dtor     14027f870 (clear_all, member dtors in reverse order)
//   size     0xc8 (members end at 0xc8; one caller keeps it in a 0xd0 stack slot)
// Constructors, dtor, reset/clear, the edge accessors and the copy (14027f6e0..140280760) are
// ported by p1-07; the class's other methods sit at p1-04/p1-06/p1-08 addresses and are called
// through defmodel/geo/pending.hpp until those packets land.
//
// The four topology lists hold interface pointers; clear_topology frees the objects with free()
// without running their destructors (the original does exactly that), so the owners must
// allocate topology objects with operator new = malloc (geo::MallocObject).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/c3d_cross_section.hpp>
#include <defmodel/layout/layout_check.hpp>
#include <defmodel/sdctl/cow.hpp>

namespace def::model {

class IFace;
class ILoop;
class IEdge;
class IVertex;
class DEF_EDGE;
class OBJ_GEO2D;
class OBJ_MESH2D;

class GEO2D_AND_MESH2D : public C3dCrossSection {
public:
    // @src 14027f6e0  ctor
    GEO2D_AND_MESH2D();
    // @src 14027f7a0  copy ctor: ctor body, then copy_from(other)
    GEO2D_AND_MESH2D(const GEO2D_AND_MESH2D& other);
    GEO2D_AND_MESH2D& operator=(const GEO2D_AND_MESH2D&) = delete;

    // @src 14027f870  dtor (slot 0 scalar deleting dtor 140284900 belongs to p1-08; in C++ it is
    //                 compiler-generated around this body)
    ~GEO2D_AND_MESH2D() override;
    // @src 1402848f0  slot 1: class name (p1-08)
    const char* class_name() override;

    // @src 14027f910  reset: clear_all, result set 0, flags 0x30/0x31 = 0, identity matrix,
    //                 0xb8 = 1; returns 0
    std::int32_t reset();
    // @src 14027f980  delete the result set, clear the topology
    void clear_all();
    // @src 14027f9b0  delete the result set (virtual deleting dtor, flag 1), 0x28 = 0
    void delete_mesh();
    // @src 14027fa30  free every face/loop/edge/vertex object, clear the four lists, then
    //                 1402847d0(this, true)
    void clear_topology();
    // @src 14027fce0  new _OBJ_GEO2D curve wrapped in a new _DEF_EDGE (id = edge count + 1),
    //                 appended to the edge list; returns the curve (0 if new failed)
    OBJ_GEO2D* add_edge_curve();
    // @src 14027ffe0  curve of edge i as _OBJ_GEO2D (0 out of range or not 2D)
    OBJ_GEO2D* edge_curve(std::int32_t i);
    // @src 1402800a0  edge i as _DEF_EDGE (0 out of range)
    DEF_EDGE* edge_at(std::int32_t i);
    // @src 140280490  copy: mesh, curves, topology (when other has faces), flags, matrix, 0xb8
    void copy_from(GEO2D_AND_MESH2D& other);
    // @src 140280760  rebuild faces/loops/vertices and their links from other (edges already
    //                 copied)
    void copy_topology(GEO2D_AND_MESH2D& other);

    // vptr at 0x000 (C3dCrossSection)
    CowList<IFace*> faces;       // 0x008 (body ctor 1402871e0)
    CowList<ILoop*> loops;       // 0x010 (140286ba0)
    CowList<IEdge*> edges;       // 0x018 (140286ec0)
    CowList<IVertex*> vertices;  // 0x020 (140287500)
    OBJ_MESH2D* mesh;            // 0x028 Cls_1403ddf20, new 0x1700 (owned)
    std::uint8_t u30;            // 0x030 (1402849b0 / 1402849d0)
    std::uint8_t u31;            // 0x031
    std::byte pad_032[6];        // 0x032
    std::byte matrix[0x80];      // 0x038 4x4 doubles (p1-06 ctor 140220540; 1402849a0 returns it)
    std::uint8_t ub8;            // 0x0b8 (1402847d0 writes it)
    std::byte pad_0b9[7];        // 0x0b9
    CowList<std::int32_t> uc0;   // 0x0c0 int list (ctor 140159860, p1-04; 14015aef0 stores 4 bytes)
};

static_assert(sizeof(GEO2D_AND_MESH2D) == 0xc8);
DEF_LAYOUT_OFFSET(GEO2D_AND_MESH2D, faces, 0x08);
DEF_LAYOUT_OFFSET(GEO2D_AND_MESH2D, vertices, 0x20);
DEF_LAYOUT_OFFSET(GEO2D_AND_MESH2D, mesh, 0x28);
DEF_LAYOUT_OFFSET(GEO2D_AND_MESH2D, u30, 0x30);
DEF_LAYOUT_OFFSET(GEO2D_AND_MESH2D, matrix, 0x38);
DEF_LAYOUT_OFFSET(GEO2D_AND_MESH2D, ub8, 0xb8);
DEF_LAYOUT_OFFSET(GEO2D_AND_MESH2D, uc0, 0xc0);

} // namespace def::model
