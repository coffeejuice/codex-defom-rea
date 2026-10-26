// Tier-1 tests for Cls_14042a870, its trees and its ref-counted lists (packet p1-14).
// Expected values come from the decompiled code (comments cite the address), not from the port.
// ctest-labels: p1-14 tree
#include <defmodel/layout/cls_14042a870.hpp>
#include <testkit/check.hpp>

#include <vector>

using namespace def::model;

// 14042a900: u00 = 0, u08 = 3, every container empty.
TEST_CASE(ctor_state)
{
    Cls_14042a870 c;
    CHECK(c.u00 == nullptr);
    CHECK_EQ(c.u08, 3);
    CHECK(c.u10.empty());
    CHECK(c.u18.value.empty());
    CHECK(c.u58.value.empty());
    CHECK(c.u98.empty());
    CHECK(c.ua0.empty());
    CHECK(c.ua8.empty());
    CHECK_EQ(c.u98.use_count(), 1);
}

// 140431130: an unshared holder is emptied in place (140431930), refs stays 1.
TEST_CASE(cow_clear_unshared_keeps_holder)
{
    CowList<TreeIntPair> l;
    l.push_back({1, 2});
    l.push_back({3, 4});
    const void* h = l.body();
    l.clear();
    CHECK(l.body() == h);
    CHECK_EQ(l.use_count(), 1);
    CHECK(l.empty());
}

// 140431130: a shared holder loses one reference (140001010) and the handle gets a new empty
// holder (140431240); the other handle keeps the items.
TEST_CASE(cow_clear_shared_detaches)
{
    CowList<TreeIntPair> a;
    a.push_back({5, 6});
    CowList<TreeIntPair> b(a);
    CHECK_EQ(a.use_count(), 2);
    const void* h = a.body();
    b.clear();
    CHECK(b.body() != h);
    CHECK_EQ(b.use_count(), 1);
    CHECK(b.empty());
    CHECK(a.body() == h);
    CHECK_EQ(a.use_count(), 1);
    CHECK_EQ(a.size(), std::size_t(1));
    CHECK_EQ(a.items().front().first, 5);
    CHECK_EQ(a.items().front().second, 6);
}

// 14042db40: edges are ordered by a, then b; (a, b) equal means the same edge.
TEST_CASE(edge_order)
{
    TreeEdgeSet s;
    s.insert(TreeEdge(2, 1, true));
    s.insert(TreeEdge(1, 5, true));
    s.insert(TreeEdge(1, 3, true));
    s.insert(TreeEdge(0, 9, true));
    auto dup = s.insert(TreeEdge(1, 3, false));
    CHECK(!dup.second);
    CHECK(dup.first->boundary);  // the first insertion is kept
    std::vector<std::pair<int, int>> got;
    for (const TreeEdge& e : s) got.emplace_back(e.a, e.b);
    const std::vector<std::pair<int, int>> want{{0, 9}, {1, 3}, {1, 5}, {2, 1}};
    CHECK(got == want);
}

// 14042d1b0: nodes are ordered by node number only; the flag is not part of the key.
TEST_CASE(node_order)
{
    TreeNodeSet s;
    s.insert({7, false});
    s.insert({-1, true});
    s.insert({3, false});
    CHECK(!s.insert({3, true}).second);
    std::vector<int> got;
    for (const TreeNodeFlag& n : s) got.push_back(n.node);
    CHECK(got == (std::vector<int>{-1, 3, 7}));
}

// 14042a950 / 14042b0c0: clear empties u10, both trees and the three lists; u00 and u08 stay.
TEST_CASE(clear_resets_everything)
{
    Cls_14042a870 c;
    std::int32_t conn[4] = {1, 2, 3, 0};
    c.u00 = conn;
    c.u10.push_back(0);
    c.u18.value.insert({1, true});
    c.u58.value.insert(TreeEdge(0, 1, true));
    c.u98.push_back({0, 1});
    c.ua0.push_back(1);
    c.ua8.push_back(2);
    c.clear();
    CHECK(c.u00 == conn);
    CHECK_EQ(c.u08, 3);
    CHECK(c.u10.empty());
    CHECK(c.u18.value.empty());
    CHECK(c.u58.value.empty());
    CHECK(c.u98.empty());
    CHECK(c.ua0.empty());
    CHECK(c.ua8.empty());
}

// 14042a870 on a populated object, and a list shared with another handle survives the dtor.
TEST_CASE(dtor_releases_shared_list)
{
    CowList<TreeIntPair>* keep = nullptr;
    {
        Cls_14042a870 c;
        c.u98.push_back({4, 8});
        keep = new CowList<TreeIntPair>(c.u98);
        CHECK_EQ(keep->use_count(), 2);
        c.u58.value.insert(TreeEdge(4, 8, true));
    }
    CHECK_EQ(keep->use_count(), 1);
    CHECK_EQ(keep->size(), std::size_t(1));
    delete keep;
}

int main() { return testkit::run_all(); }
