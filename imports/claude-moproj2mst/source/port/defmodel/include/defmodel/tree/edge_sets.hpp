// defmodel/tree/edge_sets.hpp: element types and orderings of the two red-black trees in
// Cls_14042a870 (packet p1-14).
//
// Both trees are VS2010 std::set instantiations (debug-iterator build: _Myproxy at +0,
// comparator at +0x10, allocators at +0x18/+0x20, _Myhead +0x30, _Mysize +0x38; nodes are
// {left, parent, right, value at +0x18, color, isnil}). Their code in 0x14042b690-0x140431690 is
// the stock Dinkumware algorithm (insert 14042ca70, erase 14042f160/14042fc50, rotations
// 14042e450/14042e5a0, clear 14042bbc0/14042c0c0), so the port uses std::set with the same strict
// weak ordering: iteration order, which decides the order of Cls_14042a870::u98, depends only on
// the ordering, not on the tree shape.
#pragma once

#include <cstdint>
#include <set>
#include <vector>

namespace def::model {

// Tree at Cls_14042a870+0x18: one entry per surface node (8-byte value, isnil at node+0x21).
struct TreeNodeFlag {
    std::int32_t node = 0;     // +0 key: node number (0-based)
    bool on_boundary = false;  // +4 set by 14042b470 for nodes of boundary edges
};

// Ordering of the node tree: 14042d1b0 returns a.node < b.node (P2-or-later code, used here as
// the set's comparator).
struct TreeNodeLess {
    bool operator()(const TreeNodeFlag& a, const TreeNodeFlag& b) const { return a.node < b.node; }
};

// Tree at Cls_14042a870+0x58: one entry per face edge (24-byte value, isnil at node+0x31).
struct TreeEdge {
    // 14042b700: a = b = -1, empty list. 14042b7b0 stores (min, max) of the two end nodes.
    std::int32_t a = -1;       // +0x00 smaller node number
    std::int32_t b = -1;       // +0x04 larger node number
    std::vector<std::int32_t> u08;  // +0x08 heap int-array handle in the binary (140159ad0,
                                    //       p1-04); cleared by 14019a7d0 when not empty
    bool boundary = false;     // +0x10 1 when inserted, 0 once a second face shares the edge
                               //       (14042b380)

    // @src 14042b760  edge dtor: clear the array when it has elements (1400030b0, 14019a7d0),
    //                 then release its handle (140159b40); the vector frees itself here
    // @src 14042b810  thunk to 14042b760
    // @src 1404318a0  tree-node destroy: ~node (1404318e0), no free (flag 0)
    // @src 1404318e0  tree-node dtor: 14042b810(value at +0x18)
    // @src 140431670  _Alnod.destroy(node) (ICF-folded with __ExceptionPtrDestroy): 1404318a0(node, 0)
    ~TreeEdge() = default;

    TreeEdge() = default;
    TreeEdge(std::int32_t a_, std::int32_t b_, bool boundary_) : a(a_), b(b_), boundary(boundary_) {}
    TreeEdge(const TreeEdge&) = default;
    TreeEdge& operator=(const TreeEdge&) = default;
};

// Ordering of the edge tree: 14042db40 compares (a, b) lexicographically (it takes both edges by
// value and destroys the copies, 14042b760; that has no visible effect).
struct TreeEdgeLess {
    bool operator()(const TreeEdge& p, const TreeEdge& q) const
    {
        if (q.a < p.a) return false;
        if (p.a < q.a) return true;
        if (q.b < p.b) return false;
        if (p.b < q.b) return true;
        return false;
    }
};

using TreeNodeSet = std::set<TreeNodeFlag, TreeNodeLess>;
using TreeEdgeSet = std::set<TreeEdge, TreeEdgeLess>;

// Value of the list at Cls_14042a870+0x98: one boundary edge (14042aa00 pushes {a, b}).
struct TreeIntPair {
    std::int32_t first = 0;
    std::int32_t second = 0;
};

} // namespace def::model
