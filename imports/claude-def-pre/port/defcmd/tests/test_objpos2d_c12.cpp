// ctest-labels: p67-1 objpos
// Census packet C12 (2D OBJPOS topology arrays, defmodel/obj_mesh2d/rc_array.hpp): the per-type
// handle operations size / at / clear / detach / clone of OBJ_MESH2D +0x1588..+0x15b8, the loop
// list (1403e30a0, 1403e3130, 1403e31d0, 1403e3f40, 1403e5f70) and the element assigns
// (1403e38b0, 1403e3920, 1403e3960, 1403e39f0, 1403e3a30). Hand-made inputs; expected values
// derived from the listing.
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

} // namespace

// 1403e3070 / 1403e4740: clear on a fresh block leaves three null pointers; size 0 (1403e2f70)
TEST_CASE(c12_curve_array_clear_and_size)
{
    Handle<Elem20_1403e25a0> c;
    CHECK_EQ(c.op().size(), 0);
    Elem20_1403e25a0 e;
    e.v0 = 3;
    e.v1 = 7;
    c.op().push_back(e);
    c.op().push_back(e);
    CHECK_EQ(c.op().size(), 2);  // (last - first) >> 5
    c.op().clear();
    CHECK(c.h->data == nullptr);
    CHECK(c.h->u10 == nullptr);
    CHECK(c.h->u18 == nullptr);
    CHECK_EQ(c.op().size(), 0);
}

// 1403e2f90 / 1403e3ee0: at(i) detaches a shared block, then first + i
TEST_CASE(c12_curve_array_at_detaches_shared_block)
{
    Handle<Elem20_1403e25a0> c;
    Elem20_1403e25a0 e;
    for (int i = 0; i < 3; ++i) {
        e.v0 = i;
        e.v1 = 10 + i;
        c.op().push_back(e);
    }
    RcArray<Elem20_1403e25a0>* shared = c.h;
    ++shared->refs;  // a second owner
    Elem20_1403e25a0& x = c.op().at(2);
    CHECK(c.h != shared);
    CHECK_EQ(shared->refs, 1);
    CHECK_EQ(c.h->refs, 1);
    CHECK_EQ(x.v0, 2);
    CHECK_EQ(x.v1, 12);
    CHECK(&x == c.h->data + 2);
    x.v0 = 99;
    CHECK_EQ(shared->data[2].v0, 2);  // the other owner keeps its copy
    release_rc_array(shared);
}

// 1403e3cc0 / 1403e5560 / 1403e6330: clone of the node array keeps exactly size elements
// (capacity == size), assigns vertex / u20 / u24 and shares the vectors
TEST_CASE(c12_node_array_clone)
{
    Handle<Elem28_1403e2370> n;
    Elem28_1403e2370 tmpl;
    tmpl.vertex = -1;
    tmpl.u20 = -1;
    tmpl.u24 = -1;
    n.op().resize(5, tmpl);  // capacity 5
    n.op().at(1).edges = ints({4, 5});
    n.op().push_back(tmpl);   // grows to 5 + 1 + 2 = 8
    CHECK_EQ(static_cast<int>(n.h->u18 - n.h->data), 8);
    RcArray<Elem28_1403e2370>* shared = n.h;
    ++shared->refs;
    n.op().detach();
    CHECK(n.h != shared);
    CHECK_EQ(n.op().size(), 6);
    CHECK_EQ(static_cast<int>(n.h->u18 - n.h->data), 6);
    CHECK_EQ(n.h->data[3].vertex, -1);
    CHECK_EQ(n.h->data[5].u24, -1);
    CHECK(n.h->data[1].edges.body() == shared->data[1].edges.body());  // shared, not copied
    CHECK_EQ(n.h->data[1].edges.use_count(), 2);
    // refs == 1: no clone
    RcArray<Elem28_1403e2370>* own = n.h;
    n.op().detach();
    CHECK(n.h == own);
    release_rc_array(shared);
}

// clone of an empty shared block: count < 1 gives three null pointers
TEST_CASE(c12_clone_of_empty_block)
{
    Handle<Elem28_1403e2370> n;
    RcArray<Elem28_1403e2370>* shared = n.h;
    ++shared->refs;
    n.op().detach();
    CHECK(n.h != shared);
    CHECK(n.h->data == nullptr && n.h->u10 == nullptr && n.h->u18 == nullptr);
    release_rc_array(shared);
}

// 0x15a0 vertices (1403e2e00 / 1403e3e80, assign 1403e3920: node and curves, not +0x04)
TEST_CASE(c12_vertex_at_and_assign)
{
    Handle<Elem10_1403e2610> v;
    Elem10_1403e2610 e;
    e.node = 42;
    e.u04 = 7;
    e.curves = ints({1, 2, 3});
    v.op().push_back(e);
    Elem10_1403e2610& x = v.op().at(0);
    CHECK_EQ(x.node, 42);
    CHECK_EQ(x.u04, 0);  // not copied
    CHECK_EQ(x.curves.size(), 3u);
    CHECK(x.curves.body() == e.curves.body());
}

// 0x15b0 loops: size (1403e32a0), at (1403e32c0), clear (1403e33a0); assign 1403e39f0 shares
// the list (1403e3f40) and the vector
TEST_CASE(c12_loop_array)
{
    Handle<Elem10_1403e2750> l;
    Elem10_1403e2750 e;
    e.items.push_back(LoopItem{1, 4});
    e.items.push_back(LoopItem{0, 6});
    e.regions = ints({2});
    l.op().push_back(e);
    l.op().push_back(e);
    l.op().push_back(e);
    CHECK_EQ(l.op().size(), 3);  // (last - first) >> 4
    Elem10_1403e2750& x = l.op().at(1);
    CHECK(x.items.body() == e.items.body());
    CHECK_EQ(e.items.use_count(), 1 + 3);  // e and the three elements; the spare slot (capacity 4) has its own default body
    CHECK_EQ(static_cast<int>(l.h->u18 - l.h->data), 4);  // growth 0 -> 1 -> 2 -> 4 (size + 1 + size / 2)
    l.op().clear();
    CHECK_EQ(l.op().size(), 0);
    CHECK(l.h->data == nullptr);
    CHECK_EQ(e.items.use_count(), 1);
}

// 0x15b8 regions: size (1403e3460), at (1403e3480), clear (1403e3560); assign 1403e3a30 copies
// mat, not +0x14
TEST_CASE(c12_region_array)
{
    Handle<Elem18_1403e23f0> r;
    Elem18_1403e23f0 e;
    e.mat = 3;
    e.u14 = 9;
    e.elems = ints({0, 1, 2});
    r.op().push_back(e);
    e.mat = 4;
    r.op().push_back(e);
    CHECK_EQ(r.op().size(), 2);
    CHECK_EQ(r.op().at(0).mat, 3);
    CHECK_EQ(r.op().at(1).mat, 4);
    CHECK_EQ(r.op().at(1).u14, 0);
    CHECK_EQ(r.op().at(1).elems.size(), 3u);
    r.op().clear();
    CHECK_EQ(r.op().size(), 0);
}

// 1403e38b0: side assign copies n0, n1, the vector (shared), curve and kind
TEST_CASE(c12_side_assign)
{
    Elem18_1403e2450 a;
    a.n0 = 1;
    a.n1 = 2;
    a.elems = ints({5});
    a.curve = -1;
    a.kind = 2;
    Elem18_1403e2450 b;
    b = a;
    CHECK_EQ(b.n0, 1);
    CHECK_EQ(b.n1, 2);
    CHECK_EQ(b.curve, -1);
    CHECK_EQ(b.kind, 2);
    CHECK(b.elems.body() == a.elems.body());
}

// 1403e3960: curve assign: list, two vectors, v0 / v1
TEST_CASE(c12_curve_assign)
{
    Elem20_1403e25a0 a;
    a.nodes.push_back(3);
    a.nodes.push_back(4);
    a.regions = ints({1});
    a.loops = ints({0, 2});
    a.v0 = 5;
    a.v1 = 6;
    Elem20_1403e25a0 b;
    b = a;
    CHECK(b.nodes.body() == a.nodes.body());
    CHECK(b.regions.body() == a.regions.body());
    CHECK(b.loops.body() == a.loops.body());
    CHECK_EQ(b.v0, 5);
    CHECK_EQ(b.v1, 6);
}

// loop list: ctor (1403e30a0) empty with one reference; size (1403e3130); at(i) detaches and
// walks i nodes from head->next (1403e31d0 / 1403e5f70); assign shares (1403e3f40)
TEST_CASE(c12_loop_list)
{
    TopoLoopList a;
    CHECK_EQ(a.use_count(), 1);
    CHECK_EQ(a.size(), 0u);
    a.push_back(LoopItem{1, 10});
    a.push_back(LoopItem{0, 11});
    a.push_back(LoopItem{1, 12});
    CHECK_EQ(a.size(), 3u);
    TopoLoopList b;
    b = a;
    CHECK(b.body() == a.body());
    CHECK_EQ(a.use_count(), 2);
    b = b;  // self-assign: refs++ before the release keeps the body
    CHECK_EQ(a.use_count(), 2);
    LoopItem& x = b.mutable_at(2);
    CHECK(b.body() != a.body());
    CHECK_EQ(a.use_count(), 1);
    CHECK_EQ(x.curve, 12);
    CHECK_EQ(x.forward, 1);
    x.curve = 77;
    CHECK_EQ(a.at(2).curve, 12);
    CHECK_EQ(b.mutable_at(0).curve, 10);
}

int main() { return testkit::run_all(); }
