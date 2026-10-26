// ctest-labels: p67-1 objpos
// Census packet C13 (2D OBJPOS topology arrays, defmodel/obj_mesh2d/rc_array.hpp): the detach
// functions of OBJ_MESH2D +0x1590..+0x15b8 (1403e3d20 / 3e50 / 3eb0 / 3f10 / 4100 / 4160 ->
// 1403e55e0 / 56f0 / 5770 / 57f0 / 5980 / 5a00), the block clones (1403e6930 / 6be0 / 70b0 /
// 7360 / 7830 / 7ae0), the block sizes (1403e43f0 / 4650 / 4720 / 48d0 / 4a20 / 5100) and the
// loop list's detach / clone with its iterator helpers (1403e40a0, 1403e5900, 1403e7610,
// 1403e8040, 1403e8080, 1403e80a0). Hand-made inputs; expected values derived from the listing.
#include <defmodel/obj_mesh2d/rc_array.hpp>
#include <testkit/check.hpp>

#include <cstdint>

using namespace def::model;
using namespace def::model::mesh2d;

namespace {

IntVector ints(std::initializer_list<std::int32_t> v)
{
    IntVector r;
    for (std::int32_t x : v) r.insert(r.mutable_end(), 1, x);
    return r;
}

template <class T>
struct Handle {
    RcArray<T>* h = new_rc_array<T>();
    ~Handle() { release_rc_array(h); }
    RcHandle<T> op() { return RcHandle<T>(h); }
};

// Share the block with a second owner, detach through the handle and check the clone shape:
// the old block keeps refs - 1, the new one has refs 1 and capacity == size == count.
template <class T>
RcArray<T>* share_and_detach(Handle<T>& a, std::int32_t count)
{
    RcArray<T>* shared = a.h;
    ++shared->refs;
    a.op().detach();
    CHECK(a.h != shared);
    CHECK_EQ(shared->refs, 1);
    CHECK_EQ(a.h->refs, 1);
    CHECK_EQ(a.op().size(), count);
    CHECK(a.h->u18 == a.h->u10);  // clone: exactly count elements, no spare capacity
    return shared;
}

} // namespace

// 1403e43f0 / 1403e3d20 / 1403e55e0 / 1403e6930: sides; growth leaves spare capacity, the clone
// does not; the element vector is shared (refs + 1), not copied
TEST_CASE(c13_side_array_detach_clone)
{
    Handle<Elem18_1403e2450> s;
    Elem18_1403e2450 e;
    e.elems = ints({4, 5});
    for (int i = 0; i < 4; ++i) {
        e.n0 = i;
        e.n1 = i + 1;
        e.curve = -1;
        e.kind = i % 3;
        s.op().push_back(e);
    }
    CHECK_EQ(s.op().size(), 4);
    CHECK_EQ(static_cast<std::int32_t>(s.h->u18 - s.h->data), 4);  // 0 -> 1 -> 2 -> 4 (1+1+n/2)
    s.op().push_back(e);
    CHECK_EQ(static_cast<std::int32_t>(s.h->u18 - s.h->data), 7);  // 4 + 1 + 2
    const std::int32_t refs_before = e.elems.use_count();
    RcArray<Elem18_1403e2450>* old = share_and_detach(s, 5);
    CHECK_EQ(e.elems.use_count(), refs_before + 5);  // five more owners of the same body
    for (int i = 0; i < 4; ++i) {
        CHECK_EQ(s.h->data[i].n0, i);
        CHECK_EQ(s.h->data[i].n1, i + 1);
        CHECK_EQ(s.h->data[i].curve, -1);
        CHECK_EQ(s.h->data[i].kind, i % 3);
        CHECK(s.h->data[i].elems.body() == e.elems.body());
    }
    s.op().at(0).n0 = 77;
    CHECK_EQ(old->data[0].n0, 0);
    release_rc_array(old);
}

// 1403e3e50 / 1403e56f0 / 1403e6be0 / 1403e51b0 count: the plain element array clones bytes
TEST_CASE(c13_element_array_detach_clone)
{
    Handle<Face18> f;
    Face18 v{2, -1, {1, 2, 3, 4}};
    f.op().resize(3, v);
    f.op().at(1).region = 6;
    RcArray<Face18>* old = share_and_detach(f, 3);
    CHECK_EQ(f.h->data[0].mat, 2);
    CHECK_EQ(f.h->data[1].region, 6);
    CHECK_EQ(f.h->data[2].edge[3], 4);
    release_rc_array(old);
}

// 1403e4650 / 1403e3eb0 / 1403e5770 / 1403e70b0: vertices
TEST_CASE(c13_vertex_array_detach_clone)
{
    Handle<Elem10_1403e2610> v;
    Elem10_1403e2610 e;
    e.curves = ints({0, 3});
    for (int i = 0; i < 3; ++i) {
        e.node = 10 * i;
        v.op().push_back(e);
    }
    CHECK_EQ(v.op().size(), 3);  // (last - first) >> 4
    RcArray<Elem10_1403e2610>* old = share_and_detach(v, 3);
    CHECK_EQ(v.h->data[2].node, 20);
    CHECK_EQ(v.h->data[2].curves.size(), 2u);
    CHECK_EQ(v.h->data[2].curves[1], 3);
    release_rc_array(old);
}

// 1403e4720 / 1403e3f10 / 1403e57f0 / 1403e7360: curves (list, two vectors, two ints)
TEST_CASE(c13_curve_array_detach_clone)
{
    Handle<Elem20_1403e25a0> c;
    Elem20_1403e25a0 e;
    e.nodes.push_back(5);
    e.nodes.push_back(6);
    e.regions = ints({1});
    e.loops = ints({0, 1});
    e.v0 = 2;
    e.v1 = 9;
    c.op().push_back(e);
    c.op().push_back(e);
    CHECK_EQ(c.op().size(), 2);  // >> 5
    RcArray<Elem20_1403e25a0>* old = share_and_detach(c, 2);
    CHECK(c.h->data[1].nodes.body() == e.nodes.body());
    CHECK_EQ(c.h->data[1].nodes.size(), 2u);
    CHECK_EQ(c.h->data[1].loops[1], 1);
    CHECK_EQ(c.h->data[1].v0, 2);
    CHECK_EQ(c.h->data[1].v1, 9);
    release_rc_array(old);
}

// 1403e48d0 / 1403e4100 / 1403e5980 / 1403e7830: loops
TEST_CASE(c13_loop_array_detach_clone)
{
    Handle<Elem10_1403e2750> l;
    Elem10_1403e2750 e;
    e.items.push_back(LoopItem{1, 4});
    e.regions = ints({2});
    l.op().push_back(e);
    CHECK_EQ(l.op().size(), 1);  // >> 4
    RcArray<Elem10_1403e2750>* old = share_and_detach(l, 1);
    CHECK(l.h->data[0].items.body() == e.items.body());
    CHECK_EQ(l.h->data[0].regions[0], 2);
    release_rc_array(old);
}

// 1403e4a20 / 1403e4160 / 1403e5a00 / 1403e7ae0: regions
TEST_CASE(c13_region_array_detach_clone)
{
    Handle<Elem18_1403e23f0> r;
    Elem18_1403e23f0 e;
    e.elems = ints({0, 1, 2});
    e.loops = ints({0});
    for (int i = 0; i < 2; ++i) {
        e.mat = 100 + i;
        r.op().push_back(e);
    }
    CHECK_EQ(r.op().size(), 2);  // / 0x18
    RcArray<Elem18_1403e23f0>* old = share_and_detach(r, 2);
    CHECK_EQ(r.h->data[0].mat, 100);
    CHECK_EQ(r.h->data[1].mat, 101);
    CHECK_EQ(r.h->data[1].elems.size(), 3u);
    release_rc_array(old);
}

// 1403e5100: node block size / 0x28 (nodes are built by resize)
TEST_CASE(c13_node_block_size)
{
    Handle<Elem28_1403e2370> n;
    Elem28_1403e2370 t;
    t.vertex = -1;
    n.op().resize(5, t);
    CHECK_EQ(static_cast<std::int32_t>(n.h->size()), 5);
    CHECK_EQ(n.op().size(), 5);
}

// clone of an empty shared block (count < 1): three null pointers, refs 1
TEST_CASE(c13_empty_block_clone)
{
    Handle<Elem18_1403e23f0> r;
    RcArray<Elem18_1403e23f0>* shared = r.h;
    ++shared->refs;
    r.op().detach();
    CHECK(r.h != shared);
    CHECK(r.h->data == nullptr);
    CHECK(r.h->u10 == nullptr);
    CHECK(r.h->u18 == nullptr);
    CHECK_EQ(r.h->refs, 1);
    release_rc_array(shared);
}

// detach on an unshared block (refs == 1) keeps the block
TEST_CASE(c13_detach_unshared_keeps_block)
{
    Handle<Elem10_1403e2610> v;
    Elem10_1403e2610 e;
    v.op().push_back(e);
    RcArray<Elem10_1403e2610>* b = v.h;
    v.op().detach();
    CHECK(v.h == b);
}

// 1403e40a0 / 1403e5900 / 1403e7610 (iterators 1403e8040 / 8080 / 80a0): a shared loop list
// detaches into a clone with the values in the same order; the other owner keeps its list
TEST_CASE(c13_loop_list_detach_clone_order)
{
    TopoLoopList a;
    a.push_back(LoopItem{1, 3});
    a.push_back(LoopItem{0, 7});
    a.push_back(LoopItem{1, 2});
    TopoLoopList b(a);
    CHECK_EQ(a.use_count(), 2);
    b.push_back(LoopItem{0, 9});  // detach + clone, then append
    CHECK_EQ(a.use_count(), 1);
    CHECK_EQ(b.use_count(), 1);
    CHECK_EQ(a.size(), 3u);
    CHECK_EQ(b.size(), 4u);
    const std::int32_t expect[] = {3, 7, 2, 9};
    const std::uint8_t fwd[] = {1, 0, 1, 0};
    int i = 0;
    for (const LoopItem& it : b.items()) {
        CHECK_EQ(it.curve, expect[i]);
        CHECK_EQ(it.forward, fwd[i]);
        ++i;
    }
}

int main() { return testkit::run_all(); }
