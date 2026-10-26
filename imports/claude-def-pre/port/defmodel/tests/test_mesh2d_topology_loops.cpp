// ctest-labels: p67-1 mesh2d c10
// Tier-1 tests of the OBJ_MESH2D loop builder 1403e0070 / 1403e0bd0 (census packet C10,
// src/obj_mesh2d/topology_loops.cpp) and of CowVector::push_back (140220810 / 140220b80). The
// vertex / curve / region arrays are filled by hand; the expected loops are traced through the
// decompile of 1403e0070 (comments).
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/obj_mesh2d/rc_array.hpp>
#include <defmodel/obj_mesh2d/topology.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <initializer_list>
#include <vector>

using namespace def::model;
using namespace def::model::mesh2d;

namespace {

void add_vertex(OBJ_MESH2D* m, std::int32_t node, std::initializer_list<std::int32_t> curves)
{
    Elem10_1403e2610 v;
    v.node = node;
    for (std::int32_t c : curves) v.curves.push_back(c);
    RcHandle<Elem10_1403e2610>(m->h15a0).push_back(v);
}

void add_curve(OBJ_MESH2D* m, std::int32_t v0, std::int32_t v1, std::initializer_list<std::int32_t> regions)
{
    Elem20_1403e25a0 c;
    c.v0 = v0;
    c.v1 = v1;
    for (std::int32_t r : regions) c.regions.push_back(r);
    RcHandle<Elem20_1403e25a0>(m->h15a8).push_back(c);
}

void add_regions(OBJ_MESH2D* m, std::int32_t n)
{
    for (std::int32_t i = 0; i < n; ++i) {
        Elem18_1403e23f0 r;
        r.mat = i + 1;
        RcHandle<Elem18_1403e23f0>(m->h15b8).push_back(r);
    }
}

std::vector<std::int32_t> ints(const IntVector& v) { return std::vector<std::int32_t>(v.begin(), v.end()); }

// loop items as forward * 100 + curve
std::vector<std::int32_t> items(const TopoLoopList& l)
{
    std::vector<std::int32_t> out;
    for (const LoopItem& it : l.items()) out.push_back(it.forward * 100 + it.curve);
    return out;
}

using V = std::vector<std::int32_t>;

} // namespace

// One region bounded by one closed curve (v0 == v1): one loop, no work items.
TEST_CASE(closed_curve_gives_one_loop)
{
    auto* m = new OBJ_MESH2D();
    add_regions(m, 1);
    add_vertex(m, 7, {0});
    add_curve(m, 0, 0, {0});
    fun_1403e0070(m);
    RcHandle<Elem10_1403e2750> loops(m->h15b0);
    RcHandle<Elem20_1403e25a0> curves(m->h15a8);
    RcHandle<Elem18_1403e23f0> regions(m->h15b8);
    REQUIRE(loops.size() == 1);
    CHECK(items(loops.at_nodetach(0).items) == V({100}));
    CHECK(ints(loops.at_nodetach(0).regions) == V({0}));
    CHECK(ints(regions.at_nodetach(0).loops) == V({0}));
    CHECK(ints(curves.at_nodetach(0).loops) == V({0}));
    delete m;
}

// A square split in two regions: c0 (P -> Q, region 0), c1 the interface (P -> Q, regions 0 and
// 1), c2 (Q -> P, region 1). Trace:
//   loop 0 from (c0, entry 0): stack {0,P} {1,Q}; at Q (dir 1) c1 entry 0 is region 0 and ends
//   at Q with v1, so item (0, c1) and next {1,P}; at P nothing is left for region 0. Region
//   check: c0's region 0 already has loop 0.
//   loop 1 from (c1, entry 1): at Q (dir 1) c2 starts at Q, item (1, c2). Region check: region 0
//   is not on c2, region 1 already has the loop.
TEST_CASE(two_regions_share_an_interface_curve)
{
    auto* m = new OBJ_MESH2D();
    add_regions(m, 2);
    add_vertex(m, 0, {0, 1, 2});  // P
    add_vertex(m, 3, {0, 1, 2});  // Q
    add_curve(m, 0, 1, {0});
    add_curve(m, 0, 1, {0, 1});
    add_curve(m, 1, 0, {1});
    fun_1403e0070(m);
    RcHandle<Elem10_1403e2750> loops(m->h15b0);
    RcHandle<Elem20_1403e25a0> curves(m->h15a8);
    RcHandle<Elem18_1403e23f0> regions(m->h15b8);
    REQUIRE(loops.size() == 2);
    CHECK(items(loops.at_nodetach(0).items) == V({100, 1}));
    CHECK(ints(loops.at_nodetach(0).regions) == V({0}));
    CHECK(items(loops.at_nodetach(1).items) == V({101, 102}));
    CHECK(ints(loops.at_nodetach(1).regions) == V({1}));
    CHECK(ints(regions.at_nodetach(0).loops) == V({0}));
    CHECK(ints(regions.at_nodetach(1).loops) == V({1}));
    CHECK(ints(curves.at_nodetach(0).loops) == V({0}));
    CHECK(ints(curves.at_nodetach(1).loops) == V({0, 1}));
    CHECK(ints(curves.at_nodetach(2).loops) == V({1}));
    delete m;
}

// An annulus (region 0) around a disk (region 1): c0 the outer closed curve (region 0), c1 the
// inner closed curve (regions 0 and 1). Loop 0 = c0; loop 1 starts from (c1, entry 0) for
// region 0, and in the region check region 1 has every curve of the loop (c1), so loop 1 also
// bounds region 1, c1's entry 1 is flagged and no third loop is made. c1's loop list gets loop
// 1 only once (the region check pushes to the region and the loop, not to the curve).
TEST_CASE(region_with_all_curves_on_the_loop_shares_it)
{
    auto* m = new OBJ_MESH2D();
    add_regions(m, 2);
    add_vertex(m, 0, {0});
    add_vertex(m, 5, {1});
    add_curve(m, 0, 0, {0});
    add_curve(m, 1, 1, {0, 1});
    fun_1403e0070(m);
    RcHandle<Elem10_1403e2750> loops(m->h15b0);
    RcHandle<Elem20_1403e25a0> curves(m->h15a8);
    RcHandle<Elem18_1403e23f0> regions(m->h15b8);
    REQUIRE(loops.size() == 2);
    CHECK(items(loops.at_nodetach(0).items) == V({100}));
    CHECK(ints(loops.at_nodetach(0).regions) == V({0}));
    CHECK(items(loops.at_nodetach(1).items) == V({101}));
    CHECK(ints(loops.at_nodetach(1).regions) == V({0, 1}));
    CHECK(ints(regions.at_nodetach(0).loops) == V({0, 1}));
    CHECK(ints(regions.at_nodetach(1).loops) == V({1}));
    CHECK(ints(curves.at_nodetach(0).loops) == V({0}));
    CHECK(ints(curves.at_nodetach(1).loops) == V({1}));
    delete m;
}

// Direction bookkeeping when a curve is reached at its v1 end from a dir 0 stack entry: a
// triangle of one region, c0 A -> B, c1 C -> B, c2 C -> A. Loop 0 from c0: stack {0,A} {1,B}.
//   pop {1,B}: c1 (v1 == B): item (0, c1), next {1, C}; push.
//   pop {1,C}: c2 (v0 == C): item (1, c2), next {1, A}; push.
//   pop {1,A}: nothing left. pop {0,A}: nothing left.
TEST_CASE(triangle_walks_in_direction)
{
    auto* m = new OBJ_MESH2D();
    add_regions(m, 1);
    add_vertex(m, 0, {0, 2});  // A
    add_vertex(m, 1, {0, 1});  // B
    add_vertex(m, 2, {1, 2});  // C
    add_curve(m, 0, 1, {0});
    add_curve(m, 2, 1, {0});
    add_curve(m, 2, 0, {0});
    fun_1403e0070(m);
    RcHandle<Elem10_1403e2750> loops(m->h15b0);
    REQUIRE(loops.size() == 1);
    CHECK(items(loops.at_nodetach(0).items) == V({100, 1, 102}));
    delete m;
}

// Same triangle, but the first curve is the one whose v0 sits on a dir 0 entry: c0 B -> A is
// region 0's start; stack {0,B} {1,A}. pop {1,A}: c2 C -> A ends at A: item (0, c2), next
// {1,C}. pop {1,C}: c1 C -> B starts at C: item (1, c1), next {1,B}. Then nothing.
TEST_CASE(triangle_reversed_start)
{
    auto* m = new OBJ_MESH2D();
    add_regions(m, 1);
    add_vertex(m, 0, {0, 2});  // A
    add_vertex(m, 1, {0, 1});  // B
    add_vertex(m, 2, {1, 2});  // C
    add_curve(m, 1, 0, {0});
    add_curve(m, 2, 1, {0});
    add_curve(m, 2, 0, {0});
    fun_1403e0070(m);
    RcHandle<Elem10_1403e2750> loops(m->h15b0);
    REQUIRE(loops.size() == 1);
    CHECK(items(loops.at_nodetach(0).items) == V({100, 2, 101}));
    delete m;
}

// The dir 0 side: c0 P -> Q, c1 closed at Q, c2 between P and R (one region). Stack {0,P}
// {1,Q}. pop {1,Q}: c1 (v0 == Q): item (1, c1), next {1,Q}; pop {1,Q}: nothing. pop {0,P}: c2
// reached from a dir 0 entry: R -> P (v1 == P) gives item (1, c2) and next {0,R}; P -> R (v0 ==
// P) gives item (0, c2). pop {0,R}: nothing.
TEST_CASE(dir0_side_of_the_first_curve)
{
    for (int variant = 0; variant < 2; ++variant) {
        auto* m = new OBJ_MESH2D();
        add_regions(m, 1);
        add_vertex(m, 0, {0, 2});  // P
        add_vertex(m, 1, {0, 1});  // Q
        add_vertex(m, 2, {2});     // R
        add_curve(m, 0, 1, {0});
        add_curve(m, 1, 1, {0});
        if (variant == 0) {
            add_curve(m, 2, 0, {0});
        } else {
            add_curve(m, 0, 2, {0});
        }
        fun_1403e0070(m);
        RcHandle<Elem10_1403e2750> loops(m->h15b0);
        REQUIRE(loops.size() == 1);
        CHECK(items(loops.at_nodetach(0).items) == (variant == 0 ? V({100, 101, 102}) : V({100, 101, 2})));
        delete m;
    }
}

// The curve array is shared (refs 2): the detaching accessors of 1403e0070 give the mesh its own
// copy and leave the shared block (still referenced once) as it was.
TEST_CASE(shared_curve_array_is_detached)
{
    auto* m = new OBJ_MESH2D();
    add_regions(m, 1);
    add_vertex(m, 0, {0});
    add_curve(m, 0, 0, {0});
    RcArray<Elem20_1403e25a0>* shared = m->h15a8;
    shared->refs = shared->refs + 1;
    fun_1403e0070(m);
    CHECK(m->h15a8 != shared);
    CHECK_EQ(shared->refs, 1);
    CHECK_EQ(shared->data[0].loops.size(), 0u);
    CHECK(ints(m->h15a8->data[0].loops) == V({0}));
    release_rc_array(shared);
    delete m;
}

// 1403e0bd0: first zero flag in curve order, then entry order; -1 / -1 when none.
TEST_CASE(first_unused_flag)
{
    CurveFlags* block = new_rc_array<CowVector<std::uint8_t>>();
    {
        RcHandle<CowVector<std::uint8_t>> f(block);
        CowVector<std::uint8_t> a, b, c;
        a.push_back(1);
        b.push_back(1);
        b.push_back(1);
        b.push_back(0);
        c.push_back(0);
        f.push_back(a);
        f.push_back(CowVector<std::uint8_t>());  // no entries
        f.push_back(b);
        f.push_back(c);
    }
    std::int32_t curve = 99, slot = 99;
    fun_1403e0bd0(nullptr, block, &curve, &slot);
    CHECK_EQ(curve, 2);
    CHECK_EQ(slot, 2);
    block->data[2].mutable_at(2) = 1;
    fun_1403e0bd0(nullptr, block, &curve, &slot);
    CHECK_EQ(curve, 3);
    CHECK_EQ(slot, 0);
    block->data[3].mutable_at(0) = 1;
    fun_1403e0bd0(nullptr, block, &curve, &slot);
    CHECK_EQ(curve, -1);
    CHECK_EQ(slot, -1);
    release_rc_array(block);
}

// 140220810 / 140220b80: growth to size + 1 + size / 2 when full: capacities 1, 2, 4, 7, 11.
TEST_CASE(cow_vector_push_back_growth)
{
    CowVector<std::uint8_t> v;
    std::vector<std::size_t> caps;
    for (int i = 0; i < 8; ++i) {
        v.push_back(static_cast<std::uint8_t>(i));
        caps.push_back(static_cast<std::size_t>(v.body()->end - v.body()->first));
    }
    CHECK(caps == std::vector<std::size_t>({1, 2, 4, 4, 7, 7, 7, 11}));
    CHECK_EQ(v.size(), 8u);
    CHECK_EQ(v[7], 7);
    // a shared body is cloned (capacity = size) before the push
    CowVector<std::uint8_t> w = v;
    w.push_back(9);
    CHECK_EQ(v.size(), 8u);
    CHECK_EQ(w.size(), 9u);
    CHECK_EQ(w.body()->end - w.body()->first, 13);  // clone of 8, then 8 + 1 + 4
    CHECK_EQ(v.use_count(), 1);
}

int main() { return testkit::run_all(); }
