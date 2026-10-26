// ctest-labels: p1-11 topo
// Tier-1 tests of p1-11's container code: the copy-on-write pointer vector (14041d830..14041dc40),
// the list helpers with push_front (140413790), the folded pointer helpers (14008c3c0..410) and
// the layouts. Expected values are derived by hand from the decompiled code (comments).
#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/def_loop.hpp>
#include <defmodel/layout/def_vertex.hpp>
#include <defmodel/layout/obj_geo2d_digitized.hpp>
#include <defmodel/topo/ptr_list.hpp>
#include <defmodel/topo/ptr_ops.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <cstdlib>

using namespace def::model;
using topo::CowPtrVector;

namespace {
int g_vals[16];
int* v(int i) { return &g_vals[i]; }
} // namespace

TEST_CASE(sizes_match_operator_new_sizes)
{
    // 140283980 new(0x20), 14027fce0 new(0x38), 140283a30 new(0x28), 140277830 new(0x50)
    CHECK_EQ(sizeof(DEF_LOOP), std::size_t{0x20});
    CHECK_EQ(sizeof(DEF_EDGE), std::size_t{0x38});
    CHECK_EQ(sizeof(DEF_VERTEX), std::size_t{0x28});
    CHECK_EQ(sizeof(OBJ_GEO2D_DIGITIZED), std::size_t{0x50});
    CHECK_EQ(sizeof(CowPtrVector<int>::Rep), std::size_t{0x20}); // 1402f8400 new(0x20)
}

TEST_CASE(objects_come_from_malloc)
{
    // GEO2D_AND_MESH2D's clear_topology runs the dtor and free()s the block (operator new is
    // malloc in the binary), so the classes allocate with malloc
    auto* l = new DEF_LOOP;
    l->~DEF_LOOP();
    std::free(l);
    auto* v = new DEF_VERTEX;
    v->~DEF_VERTEX();
    std::free(v);
    OBJ_GEO_BASE* g = new OBJ_GEO2D_DIGITIZED;
    delete g; // virtual dtor, class operator delete (free)
}

TEST_CASE(cow_vector_push_back_growth)
{
    // 14041d830: when last == end, reserve(size + 1 + size / 2): capacities 1, 2, 4, 7, 11
    CowPtrVector<int> a;
    const std::int64_t expect_cap[] = {1, 2, 4, 4, 7, 7, 7, 11};
    for (int i = 0; i < 8; ++i) {
        int* p = v(i);
        a.push_back(&p);
        CHECK_EQ(a.size(), std::int64_t{i + 1});
        CHECK_EQ(a.rep()->end - a.rep()->first, expect_cap[i]);
    }
    for (int i = 0; i < 8; ++i) CHECK(a[i] == v(i));
}

TEST_CASE(cow_vector_detach_on_write)
{
    CowPtrVector<int> a;
    for (int i = 0; i < 3; ++i) {
        int* p = v(i);
        a.push_back(&p);
    }
    CowPtrVector<int> b(a); // shared, refs 2
    CHECK_EQ(a.use_count(), 2);
    int* p = v(9);
    b.push_back(&p); // 14041d920 -> 14041d9c0: b gets a copy with capacity == size (3), then grows to 5
    CHECK_EQ(a.use_count(), 1);
    CHECK_EQ(b.use_count(), 1);
    CHECK_EQ(a.size(), std::int64_t{3});
    CHECK_EQ(b.size(), std::int64_t{4});
    CHECK_EQ(b.rep()->end - b.rep()->first, std::int64_t{5}); // 3 + 1 + 3/2
    CHECK(b[3] == v(9));
    CHECK(a.rep()->first != b.rep()->first);
}

TEST_CASE(cow_vector_clear_frees_buffer)
{
    CowPtrVector<int> a;
    int* p = v(1);
    a.push_back(&p);
    a.clear(); // 14041d8d0 -> 14041d950
    CHECK(a.rep()->first == nullptr);
    CHECK(a.rep()->last == nullptr);
    CHECK(a.rep()->end == nullptr);
    CHECK_EQ(a.size(), std::int64_t{0});

    // copy of an empty representation: three null pointers (14041dad0, count < 1)
    CowPtrVector<int> b(a);
    b.clear();
    CHECK_EQ(a.use_count(), 1);
    CHECK(b.rep() != a.rep());
    CHECK(b.rep()->first == nullptr);
}

TEST_CASE(list_push_front_and_at)
{
    EdgeList l;
    IEdge* e[3] = {reinterpret_cast<IEdge*>(v(0)), reinterpret_cast<IEdge*>(v(1)),
                   reinterpret_cast<IEdge*>(v(2))};
    topo::list_push_back(l, e[1]);
    topo::list_push_front(l, e[0]); // 140413790 inserts before begin()
    topo::list_push_back(l, e[2]);
    CHECK_EQ(topo::list_size(l), 3);
    for (int i = 0; i < 3; ++i) CHECK(topo::list_at(l, i) == e[i]);
    topo::list_clear(l);
    CHECK_EQ(topo::list_size(l), 0);
}

TEST_CASE(pointer_helpers)
{
    void* a = v(3);
    void* b = nullptr;
    topo::noop();
    CHECK(topo::ptr_reset(&a) == &a); // 14008c3d0
    CHECK(a == nullptr);
    CHECK(!topo::ptr_nonnull(&a));    // 14008c410
    b = v(4);
    topo::ptr_assign(&a, &b);         // 14008c3e0
    CHECK(topo::ptr_equal(&a, &b));   // 14008c400
    CHECK(topo::ptr_nonnull(&a));
}

int main() { return testkit::run_all(); }
