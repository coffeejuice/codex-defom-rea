// Cls_14042a870 (no RTTI; named after its dtor, as in docs/phase1-packets.md): surface
// boundary-edge helper embedded in _OBJ_GEO3D at +0x80 (_OBJ_GEO3D ctors 1402f9020/1402f9100 call
// the ctor 14042a7e0 on this+0x80, the dtor 1402f91f0 calls 14042a870). Packet p1-14.
//
//   size     0xb0 (it is the last member of the 0x130-byte _OBJ_GEO3D)
//   ctor     14042a7e0: member ctors, then 14042a900 (u00 = 0, u08 = 3, clear u10, reset)
//   dtor     14042a870: clear (14042a950), then member dtors in reverse order
//   role     from code outside Phase 1 (not in the batch ledger): 14042b130 walks the faces of
//            the connectivity array u00 (4 int32 per face, u08 = 3 used) and inserts every node
//            into u18 and every edge into u58, where an edge seen twice loses its boundary flag;
//            14042b470 flags the nodes of boundary edges; 14042aa00 lists the boundary edges in
//            u58 order into u98; 14042aed0 sorts the nodes into ua0 (boundary) and ua8 (inner);
//            14042aba0 chains the boundary edges into a loop.
//
// The two std::set members keep their 0x40-byte slots (Padded), so every offset matches the
// binary; the ref-counted list handles are 8 bytes as in the binary.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/tree/cow_list.hpp>
#include <defmodel/tree/edge_sets.hpp>

namespace def::model {

// A member kept in a fixed-size slot so that the members after it stay at the binary's offsets.
template <class T, std::size_t Size>
struct Padded {
    static_assert(sizeof(T) < Size, "slot too small for this standard library");
    T value;
    std::byte pad[Size - sizeof(T)];
};

class Cls_14042a870 {
public:
    // @src 14042a7e0  ctor (P2 in the ledger; ported with the class)
    Cls_14042a870();
    // @src 14042a870  dtor
    ~Cls_14042a870();

    Cls_14042a870(const Cls_14042a870&) = delete;
    Cls_14042a870& operator=(const Cls_14042a870&) = delete;

    // @src 14042a900  init: u00 = 0, u08 = 3, clear u10, reset (P2 in the ledger)
    void init();
    // @src 14042a950  clear: clear u10, then reset
    void clear();
    // @src 14042b0c0  reset: clear both trees and the three result lists
    void reset();

    std::int32_t* u00;                   // 0x00 face connectivity (4 int32 per face), not owned
    std::int32_t u08;                    // 0x08 nodes per face, 3
    std::byte pad_0c[4];                 // 0x0c
    CowList<std::int32_t> u10;           // 0x10 face list (int list handle, p1-04 type)
    Padded<TreeNodeSet, 0x40> u18;       // 0x18 node tree
    Padded<TreeEdgeSet, 0x40> u58;       // 0x58 edge tree
    CowList<TreeIntPair> u98;            // 0x98 boundary edges, in u58 order
    CowList<std::int32_t> ua0;           // 0xa0 boundary nodes
    CowList<std::int32_t> ua8;           // 0xa8 inner nodes
};

static_assert(sizeof(Cls_14042a870) == 0xb0);
DEF_LAYOUT_OFFSET(Cls_14042a870, u00, 0x00);
DEF_LAYOUT_OFFSET(Cls_14042a870, u08, 0x08);
DEF_LAYOUT_OFFSET(Cls_14042a870, u10, 0x10);
DEF_LAYOUT_OFFSET(Cls_14042a870, u18, 0x18);
DEF_LAYOUT_OFFSET(Cls_14042a870, u58, 0x58);
DEF_LAYOUT_OFFSET(Cls_14042a870, u98, 0x98);
DEF_LAYOUT_OFFSET(Cls_14042a870, ua0, 0xa0);
DEF_LAYOUT_OFFSET(Cls_14042a870, ua8, 0xa8);

} // namespace def::model
