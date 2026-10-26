// SdctlPtrCollection / SdctlGList / SdctlPtrList<_OBJ_GEO2D>: constructed field values, clear
// order and node freeing, the trivial slots (140333f50, 140330ea0, 14019ab70), against the
// decompiled code of 140330ee0, 140333090, 1403321a0, 14019b980.
// ctest-labels: p1-05 sdctl
#include <defmodel/layout/sdctl_ptr_list_obj_geo2d.hpp>
#include <defmodel/sdctl/sdctl_node.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <vector>

using namespace def::model;

namespace {

// SdctlGList leaves slot 4 pure; a test subclass records the items clear hands to it.
struct RecordingList : SdctlGList {
    std::vector<void*>* seen;
    explicit RecordingList(std::vector<void*>* s) : seen(s) {}
    void delete_item(void* item) override { seen->push_back(item); }
};

SdctlGListNode* make_node(void* item, SdctlGListNode* next)
{
    auto* n = static_cast<SdctlGListNode*>(std::malloc(sizeof(SdctlGListNode)));
    n->item = item;
    n->u08 = nullptr;
    n->next = next;
    return n;
}

} // namespace

TEST_CASE(glist_ctor_fields)
{
    std::vector<void*> seen;
    RecordingList l(&seen);
    CHECK_EQ(int(l.owns), 0);       // 140333090
    CHECK(l.head == nullptr && l.tail == nullptr && l.u20 == nullptr && l.u30 == nullptr);
    CHECK_EQ(l.cur_index, -1);      // 140330ee0: 0x28 = 0xffffffff
    CHECK_EQ(l.n, 0);
    CHECK_EQ(l.count(), 0);
}

TEST_CASE(glist_clear_visits_items_in_order_and_resets)
{
    std::vector<void*> seen;
    RecordingList l(&seen);
    int a = 0, b = 0, c = 0;
    SdctlGListNode* n3 = make_node(&c, nullptr);
    SdctlGListNode* n2 = make_node(&b, n3);
    SdctlGListNode* n1 = make_node(&a, n2);
    l.head = n1;
    l.tail = n3;
    l.u20 = n2;
    l.n = 3;
    l.cur_index = 1;
    CHECK_EQ(l.count(), 3);
    l.clear();  // 1403321a0: nodes are freed with free(); a leak checker would catch misses
    CHECK(seen == (std::vector<void*>{&a, &b, &c}));
    CHECK(l.head == nullptr && l.tail == nullptr && l.u20 == nullptr);
    CHECK_EQ(l.n, 0);
    CHECK_EQ(l.cur_index, -1);
    l.clear();  // empty: nothing visited
    CHECK_EQ(seen.size(), std::size_t{3});
}

TEST_CASE(trivial_slots)
{
    std::vector<void*> seen;
    RecordingList l(&seen);
    int x = 0, y = 0;
    CHECK(l.vslot3_140333f50(&x) == &x);   // 140333f50
    CHECK(l.differs(&x, &y));              // 140330ea0
    CHECK(!l.differs(&x, &x));
}

TEST_CASE(ptr_list_obj_geo2d_not_owning)
{
    SdctlPtrList_OBJ_GEO2D l;   // 14019aae0
    CHECK_EQ(int(l.owns), 0);
    CHECK_EQ(l.cur_index, -1);
    int dummy = 0;
    // owns == 0: slot 4 (14019b980) does not touch the item
    l.head = make_node(&dummy, make_node(nullptr, nullptr));
    l.n = 2;
    CHECK_EQ(l.count(), 2);     // 14019ab50 -> 14019ab70
    l.delete_item(&dummy);
    l.clear();                  // 14019abb0 -> 1403321a0
    CHECK_EQ(l.count(), 0);
    CHECK(l.head == nullptr);
    // the destructor (14019ab10) clears again; with an empty list nothing happens
}

TEST_CASE(ptr_list_through_base_pointer)
{
    SdctlPtrCollection* p = new SdctlPtrList_OBJ_GEO2D();
    CHECK_EQ(p->count(), 0);
    p->clear();
    delete p;                   // slot 2 scalar deleting dtor 14019ad70
}

int main() { return testkit::run_all(); }
