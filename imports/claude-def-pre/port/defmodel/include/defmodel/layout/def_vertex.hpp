// _DEF_VERTEX (RTTI .?AV_DEF_VERTEX@@, interface IVertex): one vertex of a 2D topology: its
// coordinates and the edges that meet there.
//
//   vtable   0x140567448 (4 slots); IVertex 0x140567470 (3 pure slots, no virtual dtor)
//   ctor     14041ce30 (IVertex ctor 14041d810, vptr, edge list, u20 = 0, coordinates 0)
//   dtor     14041cea0 (edge list); scalar deleting dtor 14041d7d0 (slot 3)
//   size     0x28: 140283a30 does operator new(0x28) before 14041ce30; id at 0x24 (140284b80)
//   open     role of u20 and of slot 1 (empty here).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/topo/alloc.hpp>
#include <defmodel/topo/ptr_list.hpp>

namespace def::model {

class IEdge;

class IVertex {
public:
    // @src 14041d810  ctor (vptr only)
    IVertex() = default;

    virtual void collect_edges(topo::CowPtrVector<IEdge>* out) = 0;   // slot 0
    virtual void vslot1() = 0;                                       // slot 1
    virtual std::int32_t edge_count() = 0;                           // slot 2

protected:
    ~IVertex() = default; // not virtual in the binary (IVertex has 3 slots)
};

class DEF_VERTEX : public IVertex {
public:
    // @src 14041ce30  ctor
    DEF_VERTEX();
    // @src 14041cea0  dtor; slot 3 is the scalar deleting dtor 14041d7d0
    virtual ~DEF_VERTEX();

    // @src 14041cee0  slot 0: out = the edges, in list order
    void collect_edges(topo::CowPtrVector<IEdge>* out) override;
    // @src 14041cf70  slot 1: empty
    void vslot1() override;
    // @src 14041d7b0  slot 2: number of edges
    std::int32_t edge_count() override;

    // @src 14041cf80  set the coordinates from xy[0], xy[1]
    void set_xy(const double* xy);
    // @src 14041d000  address of the coordinates
    double* get_xy();
    // @src 14041d020  edge i (0 when out of range); count through slot 2
    IEdge* edge_at(const std::int32_t* i);
    // @src 14041d0b0  append an edge
    void add_edge(IEdge* e);

    // operator new 1404797d8 (malloc) / free in the scalar deleting dtor: owners free these
    // objects with free() (GEO2D_AND_MESH2D clear_topology)
    static void* operator new(std::size_t n) { return topo::malloc_or_throw(n); }
    static void operator delete(void* p) { topo::free_block(p); }

    // vptr at 0x00 (IVertex)
    double xy[2];                     // 0x08, 0x10 =0
    EdgeList edges;       // 0x18 edges meeting here (140284e30)
    std::uint8_t u20;                 // 0x20 =0
    std::int32_t id;                  // 0x24 set by 140284b80 (not written by the ctor)
};

static_assert(sizeof(DEF_VERTEX) == 0x28);
DEF_LAYOUT_OFFSET(DEF_VERTEX, xy, 0x08);
DEF_LAYOUT_OFFSET(DEF_VERTEX, edges, 0x18);
DEF_LAYOUT_OFFSET(DEF_VERTEX, u20, 0x20);
DEF_LAYOUT_OFFSET(DEF_VERTEX, id, 0x24);

} // namespace def::model
