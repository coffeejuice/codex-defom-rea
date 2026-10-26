// OBJ_MESH2D boundary topology, census packet C08 (root 1403df940): the curves of
// 1403df970 and the helpers only they reach. See defmodel/obj_mesh2d/topology.hpp. The loops
// (1403e0070, C10) are in topology_loops.cpp / topology.cpp, the sides and regions (1403dedd0,
// C11) in topology.cpp. Decompile / listing: deform-rea-work/out/w2/p67-1-2d/d1.c, lst.txt.
// Integer code only.
//
// Container code of C08 that lives in the shared templates (rc_array.hpp, sdctl/cow.hpp), all
// checked against the decompile:
// @src 1403e2e40  vertex (+0x15a0) push_back: detach (1403e3eb0), when last == end reserve
//                 1403e5b90(size + 1 + size / 2), assign (1403e3920) at last, last += 0x10
//                 (RcHandle::push_back)
// @src 1403e2fd0  curve (+0x15a8) push_back: the same with 1403e3f10 / 1403e5c20 / 1403e3960,
//                 last += 0x20
// @src 1403e5b90  vertex reserve(cap): size (1403e4650), first = 1403e7220(cap, first, last),
//                 last = first + size, end = first + cap (RcArray::reallocate)
// @src 1403e5c20  curve reserve(cap): the same with 1403e4720 / 1403e74d0
// @src 1403e7220  new array of cap Elem10 (eh vector ctor 1403e25e0 / 1403e2610, count cookie),
//                 [first, last) assigned in (1403e82e0), the old array's vector deleting dtor
//                 (flag 3: every constructed element of the old capacity) when first != 0
// @src 1403e74d0  the same for Elem20 (1403e2550 / 1403e25a0, copy 1403e8350)
// The per-curve flag array of 1403e0070 (an RcArray of CowVector<uint8_t>, 8-byte elements):
// @src 1403e41e0  detach when refs > 1 (unsigned) (RcHandle::detach)
// @src 1403e5a80  refs-- (140001010), operator new(0x20), clone 1403e7d90; a null result kept
// @src 1403e7d90  clone: refs = 1 (140001000), count = (int)size (1403e4b80); count < 1 gives
//                 three null pointers, else exactly count elements (14019aa20 / 14019aa90)
//                 assigned from the source (1403e84a0) (RcArray(const RcArray&))
// @src 1403e4b80  size: (last - first) >> 3, no detach
// @src 1403e84a0  assign [first, last) into dst, element by element (1403e3b50)
// @src 1403e3b50  CowVector<uint8_t> assign: refs++ on the source (140159910), release own
//                 (14019b310), share (CowVector::operator=)
// @src 1403e5440  vector deleting dtor of that element array (14019aa90 per element; flag 2 with
//                 the count cookie at -8) (delete[] in RcArray)
// The loop list (TopoLoopList, rc_array.hpp) of 1403e0070:
// @src 1403e37f0  handle ctor wrapper (1403e30a0)
// @src 1403e3810  handle dtor wrapper (1403e3110)
// @src 1403e3830  pop_back: value of the last node (1403e4070), then, when not empty (1403e31a0),
//                 erase the last node (1403e3fd0, 1403e4010 -> 1403e5ed0)
// @src 1403e4070  back: detach (1403e40a0), head->prev + 0x10
// @src 1403e3fd0  iterator(head->prev), detaching
// @src 1403e4010  erase(it): detach, 1403e5ed0
// @src 1403e5ed0  unlink the node, free it, size--, iterator(next)
// @src 1403e5870  clear: refs == 1 clears in place (1403e7790), else refs-- and a new empty body
//                 (operator new 0x18, 1403e5cb0) (CowList::clear)
// @src 1403e7790  body clear: size 0, free every node after the sentinel, relink the sentinel
#include <defmodel/obj_mesh2d/topology.hpp>

#include <defmodel/layout/obj_mesh2d.hpp>

#include <cstdint>

namespace def::model {

using mesh2d::Elem10_1403e2610;
using mesh2d::Elem18_1403e2450;
using mesh2d::Elem20_1403e25a0;
using mesh2d::Elem28_1403e2370;
using mesh2d::Face18;
using mesh2d::IntList;
using mesh2d::IntVector;
using mesh2d::RcHandle;

namespace {

// ---- p1-05 container calls as 1403df970 makes them (the same small wrappers are in
// topology.cpp for the other packets; kept per file so the packets merge independently) ---------

// 140220810: push_back on an int vector (detach, grow 1402217c0 / 140222010, store)
void push(IntVector& v, std::int32_t x) { v.push_back(x); }

// 1400030b0: size of an int vector, truncated to int by the callers
std::int32_t size_of(const IntVector& v) { return static_cast<std::int32_t>(v.size()); }

// 140005100: element i of an int vector, detaching
std::int32_t& at(IntVector& v, std::int32_t i)
{
    return v.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

// @src 1403e2470  vertex (+0x15a0) curve list (+0x08) contains *v: size 1400030b0, then 140005100
//                 per entry; true at the first equal value
// 1403e24e0 (curve region list +0x08, same code) is the same function body
bool contains(IntVector& list, const std::int32_t* v)
{
    const std::int32_t n = size_of(list);
    for (std::int32_t i = 0; i < n; ++i) {
        if (at(list, i) == *v) return true;
    }
    return false;
}

// 140220700: push_back on an int list (detach 140159f90, end() 140159ef0, insert 14015aa90)
void list_push_back(IntList& l, std::int32_t x) { l.push_back(x); }

// @src 1403e2780  push_front on an int list: detach (140159f90), begin() (140159eb0), insert
//                 before it (14015aa90)
// @src 140159eb0  begin(): detach (140159f90), iterator(head->next) (14015a5b0)
void list_push_front(IntList& l, std::int32_t x)
{
    auto it = l.mutable_begin();
    l.mutable_items().insert(it, x);
}

// 1403e35d0: pop_back of an int list (the "queue" of 1403df970 is a stack): the value of the
// last node (1403e3b20), then, when the list is not empty (1401599b0), erase the last node
std::int32_t list_pop_back(IntList& l)
{
    // On an empty list the binary reads the value slot of the list's sentinel node (never
    // written, so indeterminate); std::list::back() is undefined there, so the port returns 0.
    // Both callers pop only after !list_empty(queue), so the empty case is never reached.
    auto& items = l.mutable_items();
    const std::int32_t v = items.empty() ? 0 : items.back();
    if (!l.empty()) items.pop_back();
    return v;
}

// 1401599b0: int list empty (body + 0x10 == 0)
bool list_empty(const IntList& l) { return l.empty(); }

// 140159950: int list size; 140159a70: element i, detaching
std::int32_t list_size(const IntList& l) { return static_cast<std::int32_t>(l.size()); }
std::int32_t& list_at(IntList& l, std::int32_t i)
{
    return l.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

} // namespace

// @src 1403df940  curves, then loops
void fun_1403df940(OBJ_MESH2D* m)
{
    fun_1403df970(m);
    fun_1403e0070(m);
}

// @src 1403df970  boundary curves: from each first unassigned side, walk alike sides node to node
//
// Stack slots (listing): start [0x38], ci [0x34], node [0x58], back [0x64], curve [0x48], queue
// [0x50], side [0x40], vertices [0x88] / [0x80].
void fun_1403df970(OBJ_MESH2D* m)
{
    RcHandle<Elem28_1403e2370> nodes(m->h1588);
    RcHandle<Elem18_1403e2450> sides(m->h1590);
    RcHandle<Face18> faces(m->h1598);
    RcHandle<Elem10_1403e2610> vertices(m->h15a0);
    RcHandle<Elem20_1403e25a0> curves(m->h15a8);

    curves.clear();                      // 1403e3070
    std::int32_t start = 0;              // [0x38]
    std::int32_t ci = -1;                // [0x34]
    Elem20_1403e25a0* ce = nullptr;      // [0x48]
    IntList queue;  // used as a stack                       // [0x50] 1403e3590
    std::int32_t node = -1;              // [0x58]
    for (;;) {
        start = fun_1403dfef0(m);
        if (start < 0) return;           // queue: 1403e35b0
        Elem18_1403e2450* s0 = &sides.at(start);  // 1403e2aa0
        ci = ci + 1;
        s0->curve = ci;
        Elem20_1403e25a0 tmp;            // 1403e2550
        tmp.v0 = -1;
        tmp.v1 = -1;
        curves.push_back(tmp);           // 1403e2fd0
        ce = &curves.at(ci);             // 1403e2f90
        node = s0->n0;
        list_push_back(ce->nodes, node);
        list_push_back(queue, node);
        node = s0->n1;
        list_push_back(ce->nodes, node);
        list_push_back(queue, node);
        std::uint8_t back = 1;           // [0x64]
        while (!list_empty(queue)) {
            node = list_pop_back(queue);
            if (node == list_at(ce->nodes, 0)) back = 0;
            IntVector& ne = nodes.at(node).edges;  // 1403e28e0 [0x20]
            const std::int32_t cnt = size_of(ne);  // [0x98]
            std::int32_t cand = -1;      // [0x9c]
            for (std::int32_t i = 0; i < cnt; ++i) {  // [0xa8]
                cand = at(ne, i);
                Elem18_1403e2450* cs = &sides.at(cand);  // [0xa0] 1403e2aa0
                if (!fun_1403e0cd0(m, &cand, &start)) continue;
                if (cs->curve >= 0) continue;
                cs->curve = ci;
                if (cs->n0 == node) {
                    // a side with both ends on this node keeps its curve and is passed over
                    if (cs->n1 == node) continue;
                    node = cs->n1;
                } else {
                    node = cs->n0;
                }
                if (back == 0) {
                    list_push_front(ce->nodes, node);  // 1403e2780
                } else {
                    list_push_back(ce->nodes, node);
                }
                list_push_back(queue, node);
                break;
            }
        }
        const std::int32_t n = list_size(ce->nodes);  // 140159950
        const std::int32_t va = fun_1403dff90(m, &list_at(ce->nodes, 0));
        const std::int32_t vb = fun_1403dff90(m, &list_at(ce->nodes, n - 1));
        ce->v0 = va;
        ce->v1 = vb;
        Elem10_1403e2610* ve = &vertices.at(va);  // 1403e2e00
        if (!contains(ve->curves, &ci)) push(ve->curves, ci);  // 1403e2470
        ve = &vertices.at(vb);
        if (!contains(ve->curves, &ci)) push(ve->curves, ci);
        const std::int32_t ns = size_of(s0->elems);
        std::int32_t el = -1;
        std::int32_t reg = -1;
        for (std::int32_t i = 0; i < ns; ++i) {
            el = at(s0->elems, i);
            reg = faces.at(el).region;   // 1403e2c40 + 4
            if (!contains(ce->regions, &reg)) push(ce->regions, reg);  // 1403e24e0
        }
    }  // tmp: 1403e25a0 per iteration
}

// @src 1403dfef0  first side with kind != 0 and no curve, -1 when none
std::int32_t fun_1403dfef0(OBJ_MESH2D* m)
{
    RcHandle<Elem18_1403e2450> sides(m->h1590);
    const std::int32_t n = sides.size();  // 1403e2a80
    for (std::int32_t i = 0; i < n; ++i) {
        const Elem18_1403e2450& s = sides.at(i);  // 1403e2aa0
        if (s.kind != 0 && s.curve < 0) return i;
    }
    return -1;
}

// @src 1403dff90  vertex of a node, created on first use
std::int32_t fun_1403dff90(OBJ_MESH2D* m, const std::int32_t* node)
{
    RcHandle<Elem28_1403e2370> nodes(m->h1588);
    RcHandle<Elem10_1403e2610> vertices(m->h15a0);
    (void)vertices.size();  // 1403e2de0, result unused
    Elem28_1403e2370* n = &nodes.at(*node);  // 1403e28e0
    if (n->vertex < 0) {
        Elem10_1403e2610 tmp;           // local_28 (1403e25e0)
        tmp.node = *node;
        vertices.push_back(tmp);        // 1403e2e40
        n->vertex = vertices.size() - 1;
    }                                   // tmp: 1403e2610
    return n->vertex;
}

// @src 1403e0cd0  sides alike: same kind, same number of elements, and every element of *a has an
//                 element of *b in its region (one element each: the two regions equal)
bool fun_1403e0cd0(OBJ_MESH2D* m, const std::int32_t* a, const std::int32_t* b)
{
    RcHandle<Elem18_1403e2450> sides(m->h1590);
    RcHandle<Face18> faces(m->h1598);
    Elem18_1403e2450* sa = &sides.at(*a);
    Elem18_1403e2450* sb = &sides.at(*b);
    if (sa->kind != sb->kind) return false;
    const std::int32_t na = size_of(sa->elems);
    const std::int32_t nb = size_of(sb->elems);
    if (na != nb) return false;
    if (na == 1) {
        const std::int32_t ea = at(sa->elems, 0);
        const std::int32_t eb = at(sb->elems, 0);
        const Face18* fa = &faces.at(ea);
        const Face18* fb = &faces.at(eb);
        return fa->region == fb->region;
    }
    for (std::int32_t i = 0; i < na; ++i) {
        bool hit = false;
        const Face18* fa = &faces.at(at(sa->elems, i));
        for (std::int32_t k = 0; k < nb; ++k) {
            const Face18* fb = &faces.at(at(sb->elems, k));
            if (fa->region == fb->region) hit = true;
        }
        if (!hit) return false;
    }
    return true;
}

} // namespace def::model
