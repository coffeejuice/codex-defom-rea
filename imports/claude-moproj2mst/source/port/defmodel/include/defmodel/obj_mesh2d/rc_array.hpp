// defmodel/obj_mesh2d/rc_array.hpp: the reference-counted heap arrays behind OBJ_MESH2D's seven
// handles at 0x1588..0x15b8 (code 0x1403e2280..0x1403e8430, packets p1-13 and p67-1).
//
// Each handle is one pointer to a 0x20-byte block {int refs; T* first; T* last; T* end} (a
// copy-on-write std::vector-like array). The handle's constructor allocates the block (operator
// new 0x20, block ctor: refs = 1 through FUN_140001000, the three pointers 0); its destructor
// drops one reference (FUN_140001010) and, at zero, deletes the block, whose destructor
// ("Destroy") deletes the element array with delete[] (a vector deleting destructor that runs the
// element destructors in reverse and frees the array with its count cookie).
//
// The arrays hold the 2D mesh topology that 1403dedd0 builds (defmodel/obj_mesh2d/topology.hpp):
//
//   handle  element                  one per             built by
//   0x1588  Elem28_1403e2370 node    mesh node           1403deee0 (resize)
//   0x1590  Elem18_1403e2450 edge    mesh element side   1403df640 (push_back)
//   0x1598  Face18 (plain 0x18)      mesh element        1403deee0 (resize)
//   0x15a0  Elem10_1403e2610 vertex  topology vertex     1403dff90 (push_back)
//   0x15a8  Elem20_1403e25a0 curve   boundary curve      1403df970 (push_back)
//   0x15b0  Elem10_1403e2750 loop    loop of curves      1403e0070 (push_back)
//   0x15b8  Elem18_1403e23f0 region  material region     1403deee0 (push_back)
//
// Growth (push_back, one function per element type): reserve(size + 1 + size / 2), i.e. a new
// array of that many default-constructed elements (eh vector ctor), the old elements assigned
// into it, the old array deleted (vector deleting dtor, flag 3); the new element is assigned into
// place. resize(n, val): erase the tail (the elements stay constructed in the array) or
// _Insert_n(end, n - size, val) with capacity size + max(n - size, size). Mutating accessors
// detach first (refs > 1: a fresh copy of the array with refs = 1). The Face18 array is plain
// memory in the binary (operator new / free, 1403e4550, 1403e6cd0); here it is new[] / delete[]
// like the others, which changes no value.
//
// The element members are p1-05's copy-on-write containers (defmodel/sdctl/cow.hpp): an int
// vector (FUN_140159ad0 ctor, FUN_1402207c0 assign, FUN_140159b40 dtor) or a list
// (FUN_140159860 ctor, FUN_14019cbe0 assign, FUN_140159930 dtor). Members are destroyed in
// reverse declaration order, which is the order of the element destructors below.
#pragma once

#include <cstddef>
#include <cstdint>
#include <new>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/sdctl/cow.hpp>

namespace def::model::mesh2d {

// Value of the loop list (Elem10_1403e2750 +0x00): one curve of the loop and its direction.
// 8 bytes, copied as one word (node value ctor 1403e62f0).
struct LoopItem {
    std::uint8_t forward;   // 0x00 1 when the curve is walked from v0 to v1
    std::int32_t curve;     // 0x04 index into +0x15a8
};
static_assert(sizeof(LoopItem) == 8);

// The loop list is one more CowList instantiation (body 0x18, node 0x18 {next, prev, value}):
// @src 1403e30a0  handle ctor: operator new(0x18), body ctor 1403e5cb0
// @src 1403e5cb0  body ctor: refs = 1, sentinel (operator new 0x18, 1403e6320) linked to itself
// @src 1403e6320  sentinel node ctor (identity)
// @src 1403e3110  handle dtor: release 1403e47c0
// @src 1403e47c0  release: refs-- (140001010); at 0 the scalar deleting dtor 1403e4e90
// @src 1403e4e90  body scalar deleting dtor (dtor 1403e5d60, free)
// @src 1403e5d60  body dtor: free every node after the sentinel, then the sentinel
// @src 1403e3f40  handle assign: refs++ on the source (140159910), release own, share
// @src 1403e40a0  detach when refs > 1 (1403e5900)
// @src 1403e5900  refs--, new body cloned from the old one (1403e7610)
// @src 1403e7610  body clone: values copied in order, each inserted before the sentinel
// @src 1403e8040  iterator != (node pointers; Ghidra: ATL::CFileTimeSpan::operator!=)
// @src 1403e8080  iterator deref: node + 0x10
// @src 1403e80a0  iterator post-increment: copy, node = node->next, return the copy
// @src 1403e5870  clear: refs == 1 clears in place (1403e7790), else refs-- and a new empty body
// @src 1403e7790  body clear: size 0, free the nodes, relink the sentinel to itself
// @src 1403e3150  push_back: detach, end() (1403e3f90), insert before it (1403e5de0)
// @src 1403e3f90  end(): detach, iterator(sentinel) (1403e5280)
// @src 1403e5de0  insert: new node 0x18 (value ctor 1403e62f0) before the iterator, size++
// @src 1403e62f0  node value ctor: 8-byte value at +0x10
// @src 1403e31a0  empty: size (body + 0x10) == 0
// @src 1403e3130  size (body + 0x10)
// @src 1403e31d0  at(i): detach, walk i nodes (1403e5f70), node + 0x10
// @src 1403e5f70  walk i nodes from head->next
// @src 1403e3830  pop_back: value of the last node (1403e4070), then, when not empty, erase
//                 the last node (1403e3fd0, 1403e4010 -> 1403e5ed0)
// @src 1403e4070  back: detach, head->prev + 0x10
// @src 1403e3fd0  iterator(head->prev), detaching
// @src 1403e4010  erase(it): detach, 1403e5ed0
// @src 1403e5ed0  unlink and free the node, size--
// @src 1403e5280  iterator(node)
// @src 1403e52a0  iterator copy
using TopoLoopList = CowList<LoopItem>;

// The int list of Elem20_1403e25a0 (+0x00) and the work queues of 1403deee0 / 1403df970: p1-05's
// CowList of 4-byte values (handle ctor 140159860, dtor 140159930, node value ctor 14015aef0).
using IntList = CowList<std::int32_t>;
using IntVector = CowVector<std::int32_t>;

// Element of the array at OBJ_MESH2D+0x1588 (0x28 bytes): one per mesh node.
struct Elem28_1403e2370 {
    IntVector edges;        // 0x00 mesh sides (+0x1590) at this node
    std::int32_t vertex;    // 0x08 topology vertex (+0x15a0) of the node, -1 when none
    std::int32_t u0c;       // 0x0c never written (1403e6260 does not copy it)
    IntVector h10;          // 0x10 not used by the topology code
    IntVector h18;          // 0x18 not used by the topology code
    std::int32_t u20;       // 0x20 -1 in 1403deee0's template
    std::int32_t u24;       // 0x24 -1 in 1403deee0's template

    // @src 1403e2320  ctor: the three vectors (the ints are left unset in the binary; 0 here)
    Elem28_1403e2370() : vertex(0), u0c(0), u20(0), u24(0) {}
    // @src 1403e6260  assign: vectors shared, +0x08, +0x20, +0x24 copied
    Elem28_1403e2370& operator=(const Elem28_1403e2370& o)
    {
        edges = o.edges;
        vertex = o.vertex;
        h10 = o.h10;
        h18 = o.h18;
        u20 = o.u20;
        u24 = o.u24;
        return *this;
    }
    Elem28_1403e2370(const Elem28_1403e2370&) = delete;
    // @src 1403e2370  dtor: releases 0x18, 0x10, 0x00
    ~Elem28_1403e2370() = default;
};

// Element of the array at +0x1590 (0x18 bytes): one per side of a mesh element (shared sides
// once).
struct Elem18_1403e2450 {
    std::int32_t n0;        // 0x00 first node (0-based)
    std::int32_t n1;        // 0x04 second node
    IntVector elems;        // 0x08 elements on this side (1 on the boundary, 2 inside)
    std::int32_t curve;     // 0x10 boundary curve (+0x15a8), -1 until 1403df970
    std::int32_t kind;      // 0x14 0 inside a region, 1 outer boundary, 2 between two regions

    // @src 1403e2420  ctor: the vector
    Elem18_1403e2450() : n0(0), n1(0), curve(0), kind(0) {}
    // @src 1403e38b0  assign
    Elem18_1403e2450& operator=(const Elem18_1403e2450& o)
    {
        n0 = o.n0;
        n1 = o.n1;
        elems = o.elems;
        curve = o.curve;
        kind = o.kind;
        return *this;
    }
    Elem18_1403e2450(const Elem18_1403e2450&) = delete;
    // @src 1403e2450  dtor: releases 0x08
    ~Elem18_1403e2450() = default;
};

// Element of the array at +0x1598 (0x18 bytes, plain data): one per mesh element.
struct Face18 {
    std::int32_t mat;       // 0x00 material group of the element
    std::int32_t region;    // 0x04 region (+0x15b8), -1 until the flood fill reaches it
    std::int32_t edge[4];   // 0x08 sides (+0x1590) of the element
};
static_assert(sizeof(Face18) == 0x18);

// Element of the array at +0x15a0 (0x10 bytes): one per topology vertex.
struct Elem10_1403e2610 {
    std::int32_t node;      // 0x00 mesh node
    std::int32_t u04;       // 0x04 padding
    IntVector curves;       // 0x08 curves (+0x15a8) ending here

    // @src 1403e25e0  ctor: the vector
    Elem10_1403e2610() : node(0), u04(0) {}
    // @src 1403e3920  assign
    Elem10_1403e2610& operator=(const Elem10_1403e2610& o)
    {
        node = o.node;
        curves = o.curves;
        return *this;
    }
    Elem10_1403e2610(const Elem10_1403e2610&) = delete;
    // @src 1403e2610  dtor: releases 0x08
    ~Elem10_1403e2610() = default;
};

// Element of the array at +0x15a8 (0x20 bytes): one per boundary curve.
struct Elem20_1403e25a0 {
    IntList nodes;          // 0x00 mesh nodes along the curve, in order
    IntVector regions;      // 0x08 regions of the sides the curve was walked along
    IntVector loops;        // 0x10 loops (+0x15b0) using the curve
    std::int32_t v0;        // 0x18 start vertex (+0x15a0)
    std::int32_t v1;        // 0x1c end vertex

    // @src 1403e2550  ctor: list, two vectors
    Elem20_1403e25a0() : v0(0), v1(0) {}
    // @src 1403e3960  assign: list (14019cbe0), vectors, the two ints
    Elem20_1403e25a0& operator=(const Elem20_1403e25a0& o)
    {
        nodes = o.nodes;
        regions = o.regions;
        loops = o.loops;
        v0 = o.v0;
        v1 = o.v1;
        return *this;
    }
    Elem20_1403e25a0(const Elem20_1403e25a0&) = delete;
    // @src 1403e25a0  dtor: releases 0x10, 0x08 (140159b40), then 0x00 (140159930)
    ~Elem20_1403e25a0() = default;
};

// Element of the array at +0x15b0 (0x10 bytes): one per loop.
struct Elem10_1403e2750 {
    TopoLoopList items;         // 0x00 curves of the loop with their direction
    IntVector regions;      // 0x08 regions the loop bounds

    // @src 1403e2710  ctor: list (1403e30a0), vector
    Elem10_1403e2750() = default;
    // @src 1403e39f0  assign: list (1403e3f40), vector
    Elem10_1403e2750& operator=(const Elem10_1403e2750& o)
    {
        items = o.items;
        regions = o.regions;
        return *this;
    }
    Elem10_1403e2750(const Elem10_1403e2750&) = delete;
    // @src 1403e2750  dtor: releases 0x08 (140159b40), then the list 0x00 (1403e3110)
    ~Elem10_1403e2750() = default;
};

// Element of the array at +0x15b8 (0x18 bytes): one per region (connected elements of one
// material group).
struct Elem18_1403e23f0 {
    IntVector elems;        // 0x00 mesh elements of the region
    IntVector loops;        // 0x08 loops (+0x15b0) bounding the region
    std::int32_t mat;       // 0x10 material group
    std::int32_t u14;       // 0x14 padding

    // @src 1403e23b0  ctor: the two vectors
    Elem18_1403e23f0() : mat(0), u14(0) {}
    // @src 1403e3a30  assign
    Elem18_1403e23f0& operator=(const Elem18_1403e23f0& o)
    {
        elems = o.elems;
        loops = o.loops;
        mat = o.mat;
        return *this;
    }
    Elem18_1403e23f0(const Elem18_1403e23f0&) = delete;
    // @src 1403e23f0  dtor: releases 0x08, then 0x00
    ~Elem18_1403e23f0() = default;
};
static_assert(sizeof(Elem28_1403e2370) == 0x28);
static_assert(sizeof(Elem18_1403e2450) == 0x18);
static_assert(sizeof(Elem10_1403e2610) == 0x10);
static_assert(sizeof(Elem20_1403e25a0) == 0x20);
static_assert(sizeof(Elem10_1403e2750) == 0x10);
static_assert(sizeof(Elem18_1403e23f0) == 0x18);
static_assert(offsetof(Elem20_1403e25a0, v0) == 0x18);
static_assert(offsetof(Elem18_1403e2450, curve) == 0x10);
static_assert(offsetof(Elem18_1403e23f0, mat) == 0x10);

// The 0x20-byte block. T is the element type.
template <class T>
struct RcArray {
    std::int32_t refs = 1;      // 0x00 FUN_140001000
    T* data = nullptr;          // 0x08 first
    T* u10 = nullptr;           // 0x10 last
    T* u18 = nullptr;           // 0x18 end of storage

    RcArray() = default;
    // clone (1403e6330 / 1403e6930 / 1403e6be0 / 1403e70b0 / 1403e7360 / 1403e7830 / 1403e7ae0 /
    // 1403e7d90): refs = 1; count = (int)size; count < 1 gives three null pointers, else an
    // array of exactly count elements assigned from o's
    // C13 (reviewed against the listing; tests: defcmd/tests/test_objpos2d_c13.cpp):
    // @src 1403e6930  sides (+0x1590) clone: count 1403e43f0, new[] 0x18 with cookie (eh vector
    //                 ctor 1403e2420 / dtor 1403e2450), copy 1403e81a0
    // @src 1403e6be0  elements (+0x1598) clone: count 1403e51b0, plain operator new(count *
    //                 0x18), no cookie and no ctor, byte copy 1403e61f0
    // @src 1403e70b0  vertices (+0x15a0) clone: count 1403e4650, 0x10 (1403e25e0 / 1403e2610),
    //                 copy 1403e82e0
    // @src 1403e7360  curves (+0x15a8) clone: count 1403e4720, 0x20 (1403e2550 / 1403e25a0),
    //                 copy 1403e8350
    // @src 1403e7830  loops (+0x15b0) clone: count 1403e48d0, 0x10 (1403e2710 / 1403e2750; Ghidra
    //                 labels the dtor ~CChevronOwnerDrawMenu), copy 1403e83c0
    // @src 1403e7ae0  regions (+0x15b8) clone: count 1403e4a20, 0x18 (1403e23b0 / 1403e23f0, same
    //                 mislabel), copy 1403e8430
    explicit RcArray(const RcArray& o)
    {
        const std::int32_t count = static_cast<std::int32_t>(o.u10 - o.data);
        if (0 < count) {
            data = new T[static_cast<std::size_t>(count)];
            u10 = data + count;
            u18 = data + count;
            for (std::int32_t i = 0; i < count; ++i) data[i] = o.data[i];
        }
    }
    RcArray& operator=(const RcArray&) = delete;

    // "Destroy" (std::_Ref_count<..>::_Destroy shape): the element array, if any, with delete[].
    ~RcArray()
    {
        if (data != nullptr) {
            delete[] data;
        }
    }

    // block size, no detach: signed (last - first) / sizeof(T) as a 64-bit value
    // @src 1403e43f0  sides (+0x1590): / 0x18
    // @src 1403e4650  vertices (+0x15a0): >> 4
    // @src 1403e4720  curves (+0x15a8): >> 5
    // @src 1403e48d0  loops (+0x15b0): >> 4
    // @src 1403e4a20  regions (+0x15b8): / 0x18
    // @src 1403e5100  nodes (+0x1588): / 0x28
    std::size_t size() const { return static_cast<std::size_t>(u10 - data); }

    // body clear (1403e42c0 / 1403e4420 / 1403e4550 / 1403e4740 / 1403e48f0 / 1403e4a50): delete
    // the array, three null pointers
    // @src 1403e42c0  body clear, 0x1588 nodes: vector deleting dtor (flag 3) of the array
    // @src 1403e4420  the same, 0x1590 sides
    // @src 1403e4550  the same, 0x1598 Face18: free(first) (plain memory; delete[] here)
    void clear_body()
    {
        if (data != nullptr) delete[] data;
        data = nullptr;
        u10 = nullptr;
        u18 = nullptr;
    }

    // reserve to exactly cap elements (1403e5b00 / 1403e5b90 / 1403e5c20 / 1403e5fd0 /
    // 1403e6060 / 1403e60f0 -> 1403e6aa0 / ... / 1403e7f00): new array, the elements assigned
    // over (1403e81a0 / ... / 1403e8430), the old array deleted
    // @src 1403e5b00  reserve, sides (+0x1590): count (1403e43f0), 1403e6aa0, first/last/end
    // @src 1403e6060  reserve, regions (+0x15b8): count (1403e4a20), 1403e7c50, first/last/end
    // @src 1403e6aa0  new 0x18 array of cap (eh vector ctor 1403e2420), copy 1403e81a0, delete[]
    // @src 1403e7c50  new 0x18 array of cap (eh vector ctor 1403e23b0), copy 1403e8430, delete[]
    void reallocate(std::size_t cap)
    {
        const std::size_t n = size();
        T* nb = new T[cap];
        for (std::size_t i = 0; i < n; ++i) nb[i] = data[i];
        if (data != nullptr) delete[] data;
        data = nb;
        u10 = nb + n;
        u18 = nb + cap;
    }

    // _Insert_n(pos, n, val) (1403e64a0 for the 0x28-byte nodes, 1403e6cd0 for Face18; range
    // copies 1403e6180 / 1403e61f0, backward 1403e80f0 / 1403e8210, fill 1403e8150 / 1403e8280)
    // @src 1403e64a0  _Insert_n, nodes (+0x1588, 0x28): grow to size + max(n, size) (new[] with
    //                 eh vector ctor 1403e2320), else shift in place
    // @src 1403e6cd0  _Insert_n, elements (+0x1598, plain 0x18): same, operator new / free and
    //                 byte copies through a stack temporary
    // @src 1403e80f0  copy_backward of 0x28 elements (assign 1403e6260)
    // @src 1403e8150  fill [first, last) with val (assign 1403e6260)
    // @src 1403e8210  copy_backward of 0x18 plain elements
    // @src 1403e8280  fill [first, last) with a 0x18 plain value
    void insert_n(T* pos, std::size_t n, const T& val)
    {
        if (static_cast<std::size_t>(u18 - u10) < n) {
            const std::size_t sz = size();
            std::size_t grow = n;
            if (n < sz) grow = sz;
            const std::size_t cap = sz + grow;
            T* nb = new T[cap];
            T* p = nb;
            for (T* s = data; s != pos; ++s) *p++ = *s;
            for (std::size_t k = n; k != 0; --k) *p++ = val;
            for (T* s = pos; s != u10; ++s) *p++ = *s;
            if (data != nullptr) delete[] data;
            data = nb;
            u10 = p;
            u18 = nb + cap;
        } else {
            const std::size_t after = static_cast<std::size_t>(u10 - pos);
            T* const old_last = u10;
            if (n < after) {
                T* d = u10;
                for (T* s = u10 - n; s != old_last; ++s) *d++ = *s;
                u10 += n;
                T* de = old_last;
                for (T* s = old_last - n; s != pos;) *--de = *--s;
                for (T* q = pos; q != pos + n; ++q) *q = val;
            } else {
                T* q = u10;
                for (std::size_t k = n - after; k != 0; --k) *q++ = val;
                u10 += n - after;
                T* d = u10;
                for (T* s = pos; s != old_last; ++s) *d++ = *s;
                u10 += after;
                for (T* r = pos; r != old_last; ++r) *r = val;
            }
        }
    }
};
static_assert(sizeof(RcArray<Elem28_1403e2370>) == 0x20);

// Handle constructor: operator new(0x20) (FUN_1404797d8), a null result is kept as null.
template <class T>
RcArray<T>* new_rc_array()
{
    return new (std::nothrow) RcArray<T>();
}

// Handle destructor: one reference less (FUN_140001010); at zero the block is deleted.
template <class T>
void release_rc_array(RcArray<T>* block)
{
    block->refs = block->refs - 1;
    if (block->refs == 0 && block != nullptr) {
        delete block;
    }
}

// The handle operations; the binary has one copy of each per element type (same code):
//
//   handle  detach    clone     push_back reserve   size      at        clear     resize
//   0x1588  1403e3cc0 1403e6330 -         -         1403e3bc0 1403e28e0 1403e29c0 1403e2920
//   0x1590  1403e3d20 1403e6930 1403e2ae0 1403e5b00 1403e2a80 1403e2aa0 1403e2b80 -
//   0x1598  1403e3e50 1403e6be0 -         -         1403e3d50 1403e2c40 1403e2d20 1403e2c80
//   0x15a0  1403e3eb0 1403e70b0 1403e2e40 1403e5b90 1403e2de0 1403e2e00 -         -
//   0x15a8  1403e3f10 1403e7360 1403e2fd0 1403e5c20 1403e2f70 1403e2f90 1403e3070 -
//   0x15b0  1403e4100 1403e7830 1403e3300 1403e5fd0 1403e32a0 1403e32c0 1403e33a0 -
//   0x15b8  1403e4160 1403e7ae0 1403e34c0 1403e6060 1403e3460 1403e3480 1403e3560 -
//   local   1403e41e0 1403e7d90 1403e3750 1403e60f0 1403e36c0 1403e36e0 -         -
//   (local: the array of CowVector<uint8_t> handles of 1403e0070, handle ctor 1403e3630 with
//   block ctor 1403e4ad0, release 1403e36a0 -> 1403e4b20 -> 1403e5070; at without detach
//   1403e3720 -> 1403e41c0)
// size, at, first and last are reached through small wrappers that detach first (1403e3be0 /
// 1403e3c10, 1403e3cf0, 1403e3d70 / 1403e3da0, 1403e3e80, 1403e3ee0, 1403e40d0, 1403e4130,
// 1403e4190); the size functions themselves (1403e5100, 1403e43f0, 1403e51b0, 1403e4650,
// 1403e4720, 1403e48d0, 1403e4a20, 1403e4b80) do not.
//
// Census packet C12 (reviewed against the listing; tests: defcmd/tests/test_objpos2d_c12.cpp):
// @src 1403e2e00  0x15a0 at(i): detach (1403e3eb0), first (1403e3e80) + i * 0x10 -> RcHandle::at
// @src 1403e3e80  0x15a0 first, detaching (1403e3eb0) -> RcHandle::at
// @src 1403e2f70  0x15a8 size: 1403e4720 (last - first) >> 5, 64-bit, callers keep the low
//                 dword -> RcHandle::size
// @src 1403e2f90  0x15a8 at(i): detach (1403e3f10), first (1403e3ee0) + i * 0x20 -> RcHandle::at
// @src 1403e3ee0  0x15a8 first, detaching (1403e3f10) -> RcHandle::at
// @src 1403e3070  0x15a8 clear: detach (1403e3f10), body clear 1403e4740 -> RcHandle::clear
// @src 1403e4740  0x15a8 body clear: vector deleting dtor (flag 3) of the array when not null,
//                 then first = last = end = 0 -> RcArray::clear_body
// @src 1403e32a0  0x15b0 size: 1403e48d0 (last - first) >> 4 -> RcHandle::size
// @src 1403e32c0  0x15b0 at(i): detach (1403e4100), first (1403e40d0) + i * 0x10 -> RcHandle::at
// @src 1403e40d0  0x15b0 first, detaching (1403e4100) -> RcHandle::at
// @src 1403e33a0  0x15b0 clear: detach (1403e4100), body clear 1403e48f0 -> RcHandle::clear
// @src 1403e48f0  0x15b0 body clear (as 1403e4740) -> RcArray::clear_body
// @src 1403e3460  0x15b8 size: 1403e4a20 (last - first) / 0x18 -> RcHandle::size
// @src 1403e3480  0x15b8 at(i): detach (1403e4160), first (1403e4130) + i * 0x18 -> RcHandle::at
// @src 1403e4130  0x15b8 first, detaching (1403e4160) -> RcHandle::at
// @src 1403e3560  0x15b8 clear: detach (1403e4160), body clear 1403e4a50 -> RcHandle::clear
// @src 1403e4a50  0x15b8 body clear (as 1403e4740) -> RcArray::clear_body
// @src 1403e3cc0  0x1588 detach: refs > 1 (JBE, unsigned) -> 1403e5560 -> RcHandle::detach
// @src 1403e5560  0x1588 refs-- (140001010, result ignored), operator new(0x20) and the clone
//                 1403e6330 (a failed allocation leaves a null handle; here new throws)
// @src 1403e6330  0x1588 clone: refs = 1; count = 1403e5100 (int); count < 1 gives three null
//                 pointers, else new[] of count Elem28 (cookie, eh vector ctor 1403e2320 /
//                 1403e2370), last = end = first + count, elements assigned by 1403e6180
//                 -> RcArray(const RcArray&). Elem28::u0c is not copied by 1403e6260, so the
//                 clone's u0c is the uninitialised value of 1403e2320 (0 here; never read).
// The loop-list members of C12 (1403e30a0, 1403e3130, 1403e31d0, 1403e3f40, 1403e5f70) are
// tagged at TopoLoopList above; the element assigns (1403e38b0, 1403e3920, 1403e3960, 1403e39f0,
// 1403e3a30) at their structs.
template <class T>
class RcHandle {
public:
    explicit RcHandle(RcArray<T>*& h) : h_(h) {}

    // detach: refs > 1 (unsigned compare) drops one reference and takes a clone (operator new
    // 0x20, clone ctor)
    // @src 1403e3d20  sides (+0x1590) detach -> 1403e55e0
    // @src 1403e3e50  elements (+0x1598) detach -> 1403e56f0
    // @src 1403e3eb0  vertices (+0x15a0) detach -> 1403e5770
    // @src 1403e3f10  curves (+0x15a8) detach -> 1403e57f0
    // @src 1403e4100  loops (+0x15b0) detach -> 1403e5980
    // @src 1403e4160  regions (+0x15b8) detach -> 1403e5a00
    // @src 1403e55e0  refs-- (140001010), operator new(0x20), clone 1403e6930 (a failed
    //                 allocation leaves a null handle; here new throws)
    // @src 1403e56f0  the same with clone 1403e6be0
    // @src 1403e5770  the same with clone 1403e70b0
    // @src 1403e57f0  the same with clone 1403e7360
    // @src 1403e5980  the same with clone 1403e7830
    // @src 1403e5a00  the same with clone 1403e7ae0
    void detach()
    {
        if (static_cast<std::uint32_t>(h_->refs) > 1u) {
            h_->refs = h_->refs - 1;  // FUN_140001010
            h_ = new RcArray<T>(*h_);
        }
    }
    // size: (last - first) / sizeof(T), no detach
    // @src 1403e2a80  sides (+0x1590) size: 1403e43f0 on the block
    // @src 1403e3bc0  nodes (+0x1588) size: 1403e5100 on the block
    // @src 1403e3d50  elements (+0x1598) size: 1403e51b0 on the block
    // @src 1403e2de0  size of the 0x15a0 vertex array (1403e4650 on the block)
    std::int32_t size() const { return static_cast<std::int32_t>(h_->size()); }
    // at(i): detach, then first + i
    // @src 1403e28e0  nodes (+0x1588) at: detach 1403e3cc0, first 1403e3be0, + i * 0x28
    // @src 1403e2aa0  sides (+0x1590) at: detach 1403e3d20, first 1403e3cf0, + i * 0x18
    // @src 1403e3cf0  sides first: detach 1403e3d20, block + 8
    // @src 1403e2c40  at(i), 0x1598 Face18: detach (1403e3e50), first (1403e3d70) + i * 0x18
    // @src 1403e3d70  first, 0x1598: detach (1403e3e50), then block + 8
    // @src 1403e3be0  first, 0x1588: detach (1403e3cc0), then block + 8 (used by resize 1403e2920)
    T& at(std::int64_t i)
    {
        detach();
        return h_->data[i];
    }
    // at(i) without detach (1403e3720)
    T& at_nodetach(std::int64_t i) { return h_->data[i]; }
    // push_back: detach; when last == end reserve(size + 1 + size / 2); assign at last; last++
    // @src 1403e2ae0  sides (+0x1590): detach 1403e3d20, size 1403e2a80, reserve 1403e5b00,
    //                 assign 1403e38b0
    // @src 1403e34c0  regions (+0x15b8): detach 1403e4160, size 1403e3460, reserve 1403e6060,
    //                 assign 1403e3a30
    void push_back(const T& v)
    {
        detach();
        if (h_->u10 == h_->u18) {
            const std::size_t n = h_->size();
            h_->reallocate(n + 1 + n / 2);
        }
        *h_->u10 = v;
        h_->u10 = h_->u10 + 1;
    }
    // clear: detach, then the body clear
    // @src 1403e29c0  clear, 0x1588: detach (1403e3cc0), body clear 1403e42c0
    // @src 1403e2b80  clear, 0x1590: detach (1403e3d20), body clear 1403e4420
    // @src 1403e2d20  clear, 0x1598: detach (1403e3e50), body clear 1403e4550
    void clear()
    {
        detach();
        h_->clear_body();
    }
    // resize(n, val): n < size erases [first + n, last) (nothing behind last to move down; last
    // moves back, the elements stay constructed); else _Insert_n(last, n - size, val) with the
    // position rebased onto the detached array
    // @src 1403e2920  nodes (+0x1588) resize: size 1403e3bc0; erase 1403e3c40 or insert 1403e54d0
    // @src 1403e2c80  elements (+0x1598) resize: size 1403e3d50; erase 1403e3dd0 or 1403e5660
    // @src 1403e3c10  nodes last: detach 1403e3cc0, block + 0x10
    // @src 1403e3da0  elements last: detach 1403e3e50, block + 0x10
    // @src 1403e3c40  nodes erase(a, b): detach, move [b, last) down to a (1403e6180), last
    //                 -= (b - a) / 0x28 elements; resize passes b == last, so only last moves
    //                 and the erased tail stays constructed
    // @src 1403e3dd0  elements erase(a, b): the same with 1403e61f0 and 0x18
    // @src 1403e54d0  nodes insert(pos, n, val): n != 0: offset of pos, detach 1403e3cc0, pos
    //                 rebased on first (1403e3be0), _Insert_n 1403e64a0
    // @src 1403e5660  elements insert(pos, n, val): the same with 1403e3e50 / 1403e3d70 /
    //                 1403e6cd0
    void resize(std::size_t n, const T& val)
    {
        if (n < h_->size()) {
            detach();
            h_->u10 = h_->data + n;
        } else {
            const std::size_t cnt = n - h_->size();
            detach();
            T* pos = h_->u10;
            if (cnt != 0) {
                T* const old_first = h_->data;
                detach();
                pos = h_->data + (pos - old_first);
                h_->insert_n(pos, cnt, val);
            }
        }
    }
    RcArray<T>* block() const { return h_; }

private:
    RcArray<T>*& h_;
};

} // namespace def::model::mesh2d
