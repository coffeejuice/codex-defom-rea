// The geometry list instantiations A-D (cow_list.hpp) over p1-05's CowList: behaviour the
// decompile of 140284c20..140288690 shows. Expected values are derived by hand from that code.
// ctest-labels: p1-08 mesh2d
#include <defmodel/mesh2d/cow_list.hpp>
#include <testkit/check.hpp>

#include <cstdint>

using namespace def::model;

namespace {
// Stand-in pointers; the list never dereferences its elements.
IEdge* ptr(int i) { return reinterpret_cast<IEdge*>(static_cast<std::uintptr_t>(0x1000 + i * 0x10)); }
} // namespace

TEST_CASE(new_list_is_empty_with_one_reference)
{
    EdgeList l; // 140284e30 / 140286ec0
    CHECK_EQ(l.use_count(), 1);
    CHECK_EQ(l.size(), 0u);
    CHECK(l.empty());
}

TEST_CASE(push_back_appends_in_order)
{
    EdgeList l;
    l.push_back(ptr(1)); // 140284f40 -> 140286ff0 before the sentinel
    l.push_back(ptr(2));
    l.push_back(ptr(3));
    CHECK_EQ(l.size(), 3u);
    CHECK(l.mutable_at(0) == ptr(1)); // 140284ff0 / 140287180
    CHECK(l.mutable_at(2) == ptr(3));
    CHECK(l.at(1) == ptr(2)); // 140285040 (no detach)
}

TEST_CASE(shared_body_detaches_on_write)
{
    EdgeList a;
    a.push_back(ptr(1));
    EdgeList b(a); // shared body, refs 2
    CHECK_EQ(a.use_count(), 2);
    b.push_back(ptr(2)); // make_unique 140285e20: refs > 1 -> drop one, copy (1402880d0), insert
    CHECK_EQ(a.use_count(), 1);
    CHECK_EQ(b.use_count(), 1);
    CHECK_EQ(a.size(), 1u);
    CHECK_EQ(b.size(), 2u);
    CHECK(b.at(0) == ptr(1));
}

TEST_CASE(read_at_does_not_detach)
{
    EdgeList a;
    a.push_back(ptr(1));
    EdgeList b(a);
    CHECK(b.at(0) == ptr(1));
    CHECK_EQ(a.use_count(), 2);
    CHECK(b.mutable_at(0) == ptr(1)); // writing access detaches
    CHECK_EQ(a.use_count(), 1);
}

TEST_CASE(clear_in_place_or_new_body)
{
    EdgeList a;
    a.push_back(ptr(1));
    a.clear(); // 140286770: refs == 1 -> 140288250
    CHECK_EQ(a.size(), 0u);
    a.push_back(ptr(2));
    EdgeList b(a);
    b.clear(); // refs 2: drop a reference, new empty body
    CHECK_EQ(b.size(), 0u);
    CHECK_EQ(a.size(), 1u);
    CHECK_EQ(a.use_count(), 1);
}

int main() { return testkit::run_all(); }
