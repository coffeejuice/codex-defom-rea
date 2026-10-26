// OBJ_MESH2D boundary topology, the loop-building half (census packet C10, root 1403e0070, called
// from 1403df940 after the curves). See defmodel/obj_mesh2d/topology.hpp for the data model and
// rc_array.hpp for the arrays. Decompile / listing: deform-rea-work/out/w2/p67-1-2d/d1.c,
// helpers.c, lst.txt; deform-rea-work/out/w2/p67-1-c10/c10.c, aux.c. Integer code only.
//
// The container helpers of the packet are the generic templates of rc_array.hpp / cow.hpp:
//
// local array of per-curve flag vectors (RcArray<CowVector<uint8_t>>, 8-byte elements):
// @src 1403e3630  handle ctor: operator new(0x20), block ctor 1403e4ad0 (null kept as null)
// @src 1403e4ad0  block ctor: refs = 1 (140001000), first / last / end = 0
// @src 1403e36a0  handle dtor: release 1403e4b20
// @src 1403e4b20  release: refs-- (140001010); at 0 and non-null, 1403e5070(block, 1)
// @src 1403e5070  block scalar deleting dtor: Destroy 1403e5360, free when flag & 1
// @src 1403e5360  Destroy: the element array, if any, with the vector deleting dtor (flag 3)
// @src 1403e36c0  size (1403e4b80: (last - first) / 8), no detach
// @src 1403e36e0  at(i) for writing: detach (1403e41e0), first (1403e4190) + i * 8
// @src 1403e4190  first, detaching (1403e41e0)
// @src 1403e3720  at(i) without detach: first (1403e41c0) + i * 8
// @src 1403e41c0  first, no detach
// @src 1403e3750  push_back: detach; last == end: reserve(size + 1 + size / 2) (1403e60f0);
//                 assign at last (1403e3b50); last += 8
// @src 1403e60f0  reserve: new array (1403e7f00), last = new + size, end = new + cap
// @src 1403e7f00  new[] of cap CowVector<uint8_t> (count cookie, eh vector ctor 14019aa20 /
//                 dtor 14019aa90), the old elements assigned over (1403e84a0), the old array
//                 deleted (vector deleting dtor, flag 3)
//   = mesh2d::new_rc_array, release_rc_array, RcHandle::size / at / at_nodetach / push_back,
//     RcArray::reallocate
//
// the loop array +0x15b0 (Elem10_1403e2750, 0x10-byte elements):
// @src 1403e3300  push_back: detach (1403e4100); last == end: reserve(size + 1 + size / 2)
//                 (1403e5fd0); assign at last (1403e39f0); last += 0x10
// @src 1403e5fd0  reserve: new array (1403e79a0), last = new + size, end = new + cap
// @src 1403e79a0  new[] of cap Elem10_1403e2750 (eh vector ctor 1403e2710 / dtor 1403e2750),
//                 the old elements assigned over (1403e83c0), the old array deleted (flag 3)
//   = RcHandle::push_back, RcArray::reallocate
//
// the loop item list (TopoLoopList, rc_array.hpp, which also tags these):
// @src 1403e3150  push_back: detach (1403e40a0), end() (1403e3f90), insert before it
// @src 1403e3f90  end(): detach, iterator(sentinel)
// @src 1403e31a0  empty: size (body + 0x10) == 0
//
// the per-curve flag vector (CowVector<uint8_t>, cow.hpp):
// @src 140220b80  push_back (CowVector::push_back: detach 14019aef0, grow 140221970)
// @src 140221970  reserve: new buffer (1402225b0), first / last / end (CowVector::push_back)
// @src 1402225b0  operator new(cap), copy (14019cb90), free the old buffer
// @src 140220b50  byte i for writing: detach (14019aef0), first (140220fa0) + i
//                 (CowVector::mutable_at)
// @src 140220fa0  first, detaching (CowVector::mutable_begin)
// @src 140220f60  size (14019c800: last - first), no detach (CowVector::size)
// @src 1403e2820  byte i without detach: first (1403e3ba0) + i (CowVector::operator[])
// @src 1403e3ba0  first, no detach
#include <defmodel/obj_mesh2d/topology.hpp>

#include <defmodel/layout/obj_mesh2d.hpp>

#include <cstdint>

namespace def::model {

using mesh2d::CurveFlags;
using mesh2d::Elem10_1403e2610;
using mesh2d::Elem10_1403e2750;
using mesh2d::Elem18_1403e23f0;
using mesh2d::Elem20_1403e25a0;
using mesh2d::IntVector;
using mesh2d::LoopItem;
using mesh2d::RcHandle;

namespace {

using FlagVector = CowVector<std::uint8_t>;

// 140220810: push_back on an int vector (detach, grow 1402217c0 / 140222010, store)
void push(IntVector& v, std::int32_t x) { v.push_back(x); }

// 1400030b0: size of an int vector, truncated to int by the callers
std::int32_t size_of(const IntVector& v) { return static_cast<std::int32_t>(v.size()); }

// 140005100: element i of an int vector, detaching
std::int32_t& at(IntVector& v, std::int32_t i)
{
    return v.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

// "contains" members, one per element type (same code): size 1400030b0 once, then 140005100
// per entry; true at the first equal value.
// @src 1403e2630  loop (+0x15b0) region list (+0x08) contains *v (entry == *v)
// @src 1403e26a0  region (+0x15b8) loop list (+0x08) contains *v (*v == entry)
// 1403e24e0 (curve region list, C11) is the same code; called here through this helper too.
bool contains(IntVector& list, const std::int32_t* v)
{
    const std::int32_t n = size_of(list);
    for (std::int32_t i = 0; i < n; ++i) {
        if (at(list, i) == *v) return true;
    }
    return false;
}

// 1403e3830: pop_back of the loop item list: the value of the last node (detaching), then,
// when the list is not empty, erase the last node (rc_array.hpp)
LoopItem loop_pop_back(mesh2d::TopoLoopList& l)
{
    auto& items = l.mutable_items();
    const LoopItem v = items.back();
    if (!l.empty()) items.pop_back();
    return v;
}

// 1403e31d0: loop item list element i, detaching
LoopItem& loop_at(mesh2d::TopoLoopList& l, std::int32_t i)
{
    return l.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

// 1403e3130: loop item list size (body + 0x10), no detach
std::int32_t loop_size(const mesh2d::TopoLoopList& l) { return static_cast<std::int32_t>(l.size()); }

// 140220b50 on the flag vector of curve c (1403e36e0: the array element, detaching)
std::uint8_t& flag_at(RcHandle<FlagVector>& flags, std::int32_t c, std::int32_t i)
{
    FlagVector& v = flags.at(static_cast<std::int64_t>(c));
    return v.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

} // namespace

// @src 1403e0070  loops: for each (curve, region entry) not used yet (1403e0bd0), a loop that
//                 starts with the curve (forward) and grows through the curves of that region
//                 at the vertices it reaches (a stack of {direction, vertex}); then every
//                 region of the loop's first curve whose curves all lie on the loop gets the loop
// Locals (listing, RSP-relative): curve [0x38], slot [0xa8], region [0x50], li [0x40], vertex
// [0x30], dir [0xfc], item [0xe0], next [0xe8], entry counters [0x54] / [0x98], their bounds
// [0xbc] / [0xc0]; flags handle [0x70], loop record [0xd0], work list [0xc8].
void fun_1403e0070(OBJ_MESH2D* m)
{
    RcHandle<Elem10_1403e2610> vertices(m->h15a0);
    RcHandle<Elem20_1403e25a0> curves(m->h15a8);
    RcHandle<Elem10_1403e2750> loops(m->h15b0);
    RcHandle<Elem18_1403e23f0> regions(m->h15b8);

    loops.clear();                                    // 1403e33a0
    const std::int32_t nc = curves.size();            // 1403e2f70 [0x88]
    CurveFlags* flags_block = mesh2d::new_rc_array<FlagVector>();  // [0x70] 1403e3630
    RcHandle<FlagVector> flags(flags_block);
    Elem20_1403e25a0* ce = nullptr;                   // [0x78] / [0x90]
    std::int32_t c = 0;                               // [0x54]
    std::int32_t k = 0;                               // [0x98]
    for (c = 0; c < nc; ++c) {
        ce = &curves.at(c);                           // 1403e2f90
        {
            FlagVector fresh;                         // [0xb0] 14019aa20
            flags.push_back(fresh);                   // 1403e3750
            const std::int32_t nr = size_of(ce->regions);  // [0xac]
            for (k = 0; k < nr; ++k) {
                const std::uint8_t zero = 0;          // [0x110]
                flags.at(c).push_back(zero);          // 1403e36e0, 140220b80
            }
        }                                             // 14019aa90
    }

    std::int32_t curve = -1;                          // [0x38]
    std::int32_t region = -1;                         // [0x50]
    std::int32_t slot = -1;                           // [0xa8]
    std::int32_t li = -1;                             // [0x40]
    std::int32_t vtx = -1;                            // [0x30]
    Elem10_1403e2610* ve = nullptr;                   // [0xa0]
    Elem10_1403e2750* le = nullptr;                   // [0x68]
    Elem18_1403e23f0* re = nullptr;                   // [0x28]
    LoopItem item = {0, 0};                           // [0xe0] curve of the loop + direction
    LoopItem next = {0, 0};                           // [0xe8] {direction, vertex} to go on from
    std::uint8_t dir = 0;                             // [0xfc]
    for (;;) {
        fun_1403e0bd0(m, flags_block, &curve, &slot);
        if (curve < 0) break;
        ce = &curves.at(curve);
        region = at(ce->regions, slot);
        Elem10_1403e2750 tmp;                         // [0xd0] 1403e2710
        tmp.items.clear();                            // 1403e5870
        tmp.regions.clear();                          // 14019a7d0
        item.forward = 1;
        item.curve = curve;
        tmp.items.push_back(item);                    // 1403e3150
        flag_at(flags, curve, slot) = 1;              // 1403e36e0, 140220b50
        loops.push_back(tmp);                         // 1403e3300
        li = loops.size();                            // 1403e32a0
        li = li - 1;
        le = &loops.at(li);                           // 1403e32c0
        re = &regions.at(region);                     // 1403e3480
        push(re->loops, li);
        push(ce->loops, li);
        if (!contains(le->regions, &region)) push(le->regions, region);  // 1403e2630
        mesh2d::TopoLoopList work;                    // [0xc8] 1403e37f0 (used as a stack)
        if (ce->v0 != ce->v1) {
            // LoopItem layout, used here for {direction, vertex}
            next.curve = ce->v0;
            next.forward = 0;
            work.push_back(next);                     // 1403e3150
            next.curve = ce->v1;
            next.forward = 1;
            work.push_back(next);
        }
        while (!work.empty()) {                       // 1403e31a0
            next = loop_pop_back(work);               // 1403e3830
            vtx = next.curve;
            dir = next.forward;
            ve = &vertices.at(vtx);                   // 1403e2e00
            const std::int32_t nvc = size_of(ve->curves);  // [0xf8]
            for (c = 0; c < nvc; ++c) {
                curve = at(ve->curves, c);
                ce = &curves.at(curve);               // 1403e2f90
                const std::int32_t nr = size_of(ce->regions);  // [0x100]
                slot = -1;
                for (k = 0; k < nr; ++k) {
                    if (flag_at(flags, curve, k) == 1) continue;
                    if (at(ce->regions, k) == region) {
                        slot = k;
                        break;
                    }
                }
                if (slot < 0) continue;
                flag_at(flags, curve, slot) = 1;
                push(ce->loops, li);
                item.curve = curve;
                // neither end at vtx (a curve listed at a vertex always ends there): item keeps
                // its direction and next.curve keeps the value the previous curve stored there
                // (only next.forward is set), as in the binary
                if (dir != 0) {
                    next.forward = 1;
                    if (ce->v0 == vtx) {
                        item.forward = 1;
                        next.curve = ce->v1;
                    } else if (ce->v1 == vtx) {
                        item.forward = 0;
                        next.curve = ce->v0;
                    }
                } else {
                    next.forward = 0;
                    if (ce->v0 == vtx) {
                        item.forward = 0;
                        next.curve = ce->v1;
                    } else if (ce->v1 == vtx) {
                        item.forward = 1;
                        next.curve = ce->v0;
                    }
                }
                le->items.push_back(item);            // 1403e3150
                work.push_back(next);
            }
        }
        // regions whose every curve is on this loop get the loop as well
        const std::int32_t nitems = loop_size(le->items);  // [0xbc] 1403e3130
        item = loop_at(le->items, 0);                 // 1403e31d0
        std::int32_t ic = item.curve;                 // [0xb8]
        Elem20_1403e25a0* c0 = &curves.at(ic);        // [0xf0]
        // The binary keeps the entry counter and its bound in [0x98] / [0xc0] and reuses both in
        // the flag loop below, so after a region is added the scan of c0's regions continues
        // from the last flag index, against the last curve's region count. Kept as is.
        std::int32_t bound = size_of(c0->regions);    // [0xc0]
        for (k = 0; k < bound; ++k) {
            region = at(c0->regions, k);
            re = &regions.at(region);
            if (contains(re->loops, &li)) continue;   // 1403e26a0
            std::uint8_t all = 1;                     // [0x104]
            for (c = 0; c < nitems; ++c) {
                item = loop_at(le->items, c);
                ic = item.curve;
                ce = &curves.at(ic);
                if (!contains(ce->regions, &region)) {  // 1403e24e0
                    all = 0;
                    break;
                }
            }
            if (all == 0) continue;
            for (c = 0; c < nitems; ++c) {
                item = loop_at(le->items, c);
                ic = item.curve;
                ce = &curves.at(ic);
                bound = size_of(ce->regions);
                for (k = 0; k < bound; ++k) {
                    const std::int32_t r = at(ce->regions, k);  // [0x108]
                    if (r == region) {
                        flag_at(flags, ic, k) = 1;
                        break;
                    }
                }
            }
            push(re->loops, li);
            if (!contains(le->regions, &region)) push(le->regions, region);  // 1403e2630
        }
    }  // per iteration: work 1403e3810, tmp 1403e2750

    // The binary then walks every loop's items and regions into locals it never uses. Only
    // reads, but through the detaching accessors (1403e32c0, 1403e31d0, 140005100), so kept.
    const std::int32_t nl = loops.size();             // 1403e32a0 [0x80]
    for (std::int32_t i = 0; i < nl; ++i) {           // [0x10c]
        Elem10_1403e2750* l = &loops.at(i);           // 1403e32c0 [0x48]
        const std::int32_t ni = loop_size(l->items);  // [0x84]
        for (c = 0; c < ni; ++c) (void)loop_at(l->items, c).curve;
        const std::int32_t nr = size_of(l->regions);  // [0x3c]
        for (k = 0; k < nr; ++k) (void)at(l->regions, k);
    }
    mesh2d::release_rc_array(flags_block);            // 1403e36a0
}

// @src 1403e0bd0  first (curve, entry) whose flag is 0, in curve order then entry order; both
//                 -1 when every flag is set. Reads through the non-detaching accessors.
void fun_1403e0bd0(OBJ_MESH2D* /*m*/, CurveFlags*& flags, std::int32_t* curve, std::int32_t* slot)
{
    RcHandle<FlagVector> f(flags);
    const std::int32_t n = f.size();                  // 1403e36c0
    for (*curve = 0; *curve < n; *curve = *curve + 1) {
        const std::int32_t cnt = static_cast<std::int32_t>(f.at_nodetach(*curve).size());  // 1403e3720, 140220f60
        for (*slot = 0; *slot < cnt; *slot = *slot + 1) {
            const std::int64_t i = *slot;
            if (f.at_nodetach(*curve)[static_cast<std::size_t>(i)] == 0) return;  // 1403e3720, 1403e2820
        }
    }
    *curve = -1;
    *slot = -1;
}

} // namespace def::model
