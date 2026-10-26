// _DEF_EDGE (RTTI .?AV_DEF_EDGE@@, interface IEdge): one edge of a 2D topology; owns the curve
// (OBJ_GEO_BASE, a _OBJ_GEO2D in practice) and knows its two end vertices and the loops using it.
//
//   vtable   0x1405673e0 (6 slots); IEdge 0x140567418 (5 pure slots, no virtual dtor)
//   ctor     14041c300 (IEdge ctor 14041ce00, vptr, list, vector, zeroes, 140284af0)
//   dtor     14041c3a0 (deletes the curve through its slot 0, then the handles reversed);
//            scalar deleting dtor 14041cdc0 (slot 5)
//   size     0x38: 14027fce0 does operator new(0x38) before 14041c300
//   open     who writes the vertices (0x18/0x20: callers outside Phase 1 through offsets);
//            element type of u28; roles of slots 0, 1, 4 (empty here).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/topo/alloc.hpp>
#include <defmodel/topo/ptr_list.hpp>

namespace def::model {

class ILoop;
class IVertex;
class OBJ_GEO_BASE;

class IEdge {
public:
    // @src 14041ce00  ctor (vptr only)
    IEdge() = default;

    virtual void vslot0(void* a) = 0;                       // slot 0
    virtual void vslot1() = 0;                              // slot 1
    virtual topo::CowPtrVector<void>* u28_vector() = 0;     // slot 2
    virtual std::int32_t vslot3() = 0;                      // slot 3
    virtual void vslot4(void* a) = 0;                       // slot 4

protected:
    ~IEdge() = default; // not virtual in the binary (IEdge has 5 slots)
};

class DEF_EDGE : public IEdge {
public:
    // @src 14041c300  ctor
    DEF_EDGE();
    // @src 14041c3a0  dtor; slot 5 is the scalar deleting dtor 14041cdc0
    virtual ~DEF_EDGE();

    // @src 14041c430  slot 0: empty
    void vslot0(void* a) override;
    // @src 14041c450  slot 1: empty
    void vslot1() override;
    // @src 14041c460  slot 2: address of u28
    topo::CowPtrVector<void>* u28_vector() override;
    // @src 14041cdb0  slot 3: returns -1
    std::int32_t vslot3() override;
    // @src 14041c480  slot 4: empty
    void vslot4(void* a) override;

    // 140284990 (p1-08 edge_curve): curve at +0x08
    OBJ_GEO_BASE* get_geo() const;
    // @src 14041c4a0  set the curve (takes ownership)
    void set_geo(OBJ_GEO_BASE* g);
    // @src 14041c590  end vertex i (0 or 1), not range-checked
    IVertex* vertex(const std::int32_t* i) const;
    // @src 14041c5c0  number of loops using this edge
    std::int32_t loop_count();
    // @src 14041c5f0  set id
    void set_id(const std::int32_t* v);
    // @src 14041c610  id
    std::int32_t get_id() const;
    // @src 14041c630  loop i (0 when out of range)
    ILoop* loop_at(const std::int32_t* i);
    // @src 14041c6c0  append a loop
    void add_loop(ILoop* l);
    // @src 14041cc10  curve cast to _OBJ_GEO2D, result unused; returns false
    bool fun_14041cc10();
    // @src 14041cc70  curve cast to _OBJ_GEO2D, result unused
    void fun_14041cc70();
    // @src 14041ccd0  _OBJ_GEO2D flag at +0x28 of the curve; true when the curve is not 2D
    bool curve_flag28();
    // @src 14041cd40  set that flag when the curve is 2D
    void set_curve_flag28(bool v);
    // 140284a10 (p1-08 edge_get_34): visited flag
    bool visited() const;
    // 140284a30 (p1-08 edge_set_34): set visited flag
    void set_visited(bool v);

    // operator new 1404797d8 (malloc) / free in the scalar deleting dtor: owners free these
    // objects with free() (GEO2D_AND_MESH2D clear_topology)
    static void* operator new(std::size_t n) { return topo::malloc_or_throw(n); }
    static void operator delete(void* p) { topo::free_block(p); }

    // vptr at 0x00 (IEdge)
    OBJ_GEO_BASE* geo;                       // 0x08 =0; owned curve
    LoopList loops;              // 0x10 loops using the edge (140284c20)
    IVertex* vertices[2];                    // 0x18, 0x20 =0; end vertices
    topo::CowPtrVector<void> u28;            // 0x28 (1402f8400); slot 2 returns its address
    std::int32_t id;                         // 0x30 =0
    std::uint8_t u34;                        // 0x34 =0 (140284af0); visited flag in order_edges
    std::uint8_t u35;                        // 0x35 =0 (140284af0)
};

static_assert(sizeof(DEF_EDGE) == 0x38);
DEF_LAYOUT_OFFSET(DEF_EDGE, geo, 0x08);
DEF_LAYOUT_OFFSET(DEF_EDGE, loops, 0x10);
DEF_LAYOUT_OFFSET(DEF_EDGE, vertices, 0x18);
DEF_LAYOUT_OFFSET(DEF_EDGE, u28, 0x28);
DEF_LAYOUT_OFFSET(DEF_EDGE, id, 0x30);
DEF_LAYOUT_OFFSET(DEF_EDGE, u34, 0x34);
DEF_LAYOUT_OFFSET(DEF_EDGE, u35, 0x35);

} // namespace def::model
