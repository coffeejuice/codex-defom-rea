// _DEF_LOOP (RTTI .?AV_DEF_LOOP@@, interface ILoop): a closed chain of edges of a 2D topology
// (GEO2D_AND_MESH2D keeps its loops in a list at +0x10; 140283980 creates them).
//
//   vtable   0x140565350 (6 slots); ILoop 0x140565388 (5 pure slots, no virtual dtor)
//   ctor     140412010 (ILoop ctor 140413710, vptr, two list handles, u1c = 0)
//   dtor     140412080 (list handles reversed); scalar deleting dtor 1404136d0 (slot 5)
//   size     0x20: 140283980 does operator new(0x20) before 140412010; id at 0x18 (140284b60)
//   lists    edges (IEdge*), faces (IFace*): p1-08 CowList (mesh2d/cow_list.hpp)
//   open     roles of slots 0..2 (empty here).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/topo/alloc.hpp>
#include <defmodel/topo/ptr_list.hpp>

namespace def::model {

class IEdge;
class IFace;
class IVertex;
class OBJ_GEO2D;

class ILoop {
public:
    // @src 140413710  ctor (vptr only)
    ILoop() = default;

    virtual void vslot0(void* a) = 0;                    // slot 0
    virtual void vslot1() = 0;                           // slot 1
    virtual void vslot2(void* a) = 0;                    // slot 2
    virtual std::int32_t edge_count() = 0;               // slot 3
    virtual bool inside_bbox_of(ILoop* other) = 0;        // slot 4

protected:
    ~ILoop() = default; // not virtual in the binary (ILoop has 5 slots)
};

class DEF_LOOP : public ILoop {
public:
    // @src 140412010  ctor
    DEF_LOOP();
    // @src 140412080  dtor; slot 5 is the scalar deleting dtor 1404136d0
    virtual ~DEF_LOOP();

    // @src 1404120d0  slot 0: empty
    void vslot0(void* a) override;
    // @src 1404120f0  slot 1: empty
    void vslot1() override;
    // @src 140412100  slot 2: empty
    void vslot2(void* a) override;
    // @src 140412120  slot 3: number of edges
    std::int32_t edge_count() override;
    // @src 140412610  slot 4: box of this loop's curve lies inside the other loop's (140278ed0)
    bool inside_bbox_of(ILoop* other) override;

    // @src 140412150  edge i (0 when out of range); count through slot 3
    IEdge* edge_at(const std::int32_t* i);
    // @src 1404121e0  append an edge
    void add_edge(IEdge* e);
    // @src 1404123b0  face i (0 when out of range)
    IFace* face_at(const std::int32_t* i);
    // @src 140412440  append a face
    void add_face(IFace* f);
    // @src 1404125e0  number of faces
    std::int32_t face_count();

    // @src 1404126d0  build the loop's closed curve in out (orders the edges first)
    void to_geo2d(OBJ_GEO2D* out);
    // @src 140412da0  reorder the edge list into a chain; true when the chain closes
    bool order_edges();
    // @src 1404133a0  first unvisited edge in list that touches vertex v
    static IEdge* find_unvisited_edge(IVertex* v, EdgeList* list);

    // operator new 1404797d8 (malloc) / free in the scalar deleting dtor: owners free these
    // objects with free() (GEO2D_AND_MESH2D clear_topology)
    static void* operator new(std::size_t n) { return topo::malloc_or_throw(n); }
    static void operator delete(void* p) { topo::free_block(p); }

    // vptr at 0x00 (ILoop)
    EdgeList edges;       // 0x08 edge list (140284e30)
    FaceList faces;                   // 0x10 faces (140285080, IFace* list)
    std::int32_t id;                  // 0x18 set by 140284b60 (not written by the ctor)
    std::uint8_t u1c;                 // 0x1c =0
};

static_assert(sizeof(DEF_LOOP) == 0x20);
DEF_LAYOUT_OFFSET(DEF_LOOP, edges, 0x08);
DEF_LAYOUT_OFFSET(DEF_LOOP, faces, 0x10);
DEF_LAYOUT_OFFSET(DEF_LOOP, id, 0x18);
DEF_LAYOUT_OFFSET(DEF_LOOP, u1c, 0x1c);

} // namespace def::model
