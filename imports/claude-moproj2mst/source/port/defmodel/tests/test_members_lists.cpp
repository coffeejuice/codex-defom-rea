// ctest-labels: p1-10 members
// Tier 1 for the p1-10 list members of DB_STEP_DATA and SdctlGList: the copy-on-write lists at
// +0x33b8 (Cls_14032f100 / Cls_14032f920 / CustomVarDef), the inter-material list at +0x1358
// (Cls_140339c90) and the SdctlGList+0x30 helper (Cls_1403331d0). Expected values are traced by
// hand from the decompiles named in docs/re/status/p1-10.tsv.
#include <defmodel/layout/cls_14032f100.hpp>
#include <defmodel/layout/cls_140339c90.hpp>
#include <defmodel/members/custom_state_var.hpp>
#include <defmodel/members/sdctl_index.hpp>
#include <testkit/check.hpp>

using namespace def::model;

namespace {

void push_back(StrListRep* rep, const char* s)
{
    StrListNode* n = new StrListNode{rep->head, rep->head->prev, std::string(s)};
    rep->head->prev->next = n;
    rep->head->prev = n;
    rep->size = rep->size + 1;
}

} // namespace

TEST_CASE(step_member_33b8_constructs_empty_unshared_list)
{
    Cls_14032f100 m; // 14032f100: list handle, then u0 = 0
    CHECK_EQ(m.u0, 0);
    REQUIRE(m.u8.list != nullptr);
    CHECK_EQ(m.u8.list->refs, 1);
    CHECK_EQ(m.u8.list->size, 0u);
    CHECK(m.u8.list->head->next == m.u8.list->head); // 1403303a0: sentinel linked to itself
    CHECK(m.u8.list->head->prev == m.u8.list->head);
    CHECK_EQ(m.u8.list->head->value.id, 0); // sentinel holds a constructed value (140330880)
}

TEST_CASE(custom_var_def_ctor_setters_reset)
{
    CustomVarDef v; // 14032ece0
    CHECK_EQ(v.id, 0);
    CHECK(v.name.empty());
    CHECK_EQ(v.items.rep->refs, 1);
    v.set_id(7);
    v.set_name("TEMP");
    push_back(v.items.rep, "a");
    push_back(v.items.rep, "b");
    CHECK_EQ(v.id, 7);
    CHECK(v.name == "TEMP");
    CHECK_EQ(v.items.rep->size, 2u);
    StrListRep* before = v.items.rep;
    v.reset(); // 14032ee50: name "", items cleared in place (unshared)
    CHECK(v.name.empty());
    CHECK_EQ(v.id, 7); // reset leaves the int
    CHECK(v.items.rep == before);
    CHECK_EQ(v.items.rep->size, 0u);
    CHECK(v.items.rep->head->next == v.items.rep->head);
}

TEST_CASE(string_list_clear_detaches_when_shared)
{
    StrListHandle a;
    push_back(a.rep, "x");
    StrListRep* shared = a.rep;
    shared->refs = 2; // a second handle shares it
    a.clear();        // 14032ff10: refs -> 1 on the old rep, new empty rep for a
    CHECK(a.rep != shared);
    CHECK_EQ(a.rep->refs, 1);
    CHECK_EQ(a.rep->size, 0u);
    CHECK_EQ(shared->refs, 1);
    CHECK_EQ(shared->size, 1u); // the other owner keeps its data
    release(shared);            // 14032fdb0: last reference deletes
}

TEST_CASE(inter_material_list_ctor_reset_clear_empty)
{
    auto* owner = reinterpret_cast<DB_STEP_DATA*>(static_cast<std::uintptr_t>(0x1000));
    Cls_140339c90 l(owner); // 140339c90
    CHECK(l.owner == owner);
    CHECK(l.head == nullptr);
    CHECK(l.tail == nullptr);
    CHECK_EQ(l.count, 0);
    l.count = 3; // reset does not free, only zeroes (140339cd0)
    l.reset();
    CHECK_EQ(l.count, 0);
    l.clear(); // 14033a120 on an empty list
    CHECK(l.head == nullptr);
}

TEST_CASE(sdctl_index_dtor_zeroes_records)
{
    SdctlIndexRecord r1{&r1, &r1}, r2{&r2, &r2}, cur{&cur, &cur};
    auto* rep = new PtrListRep();
    rep->insert(rep->head, &r1);
    rep->insert(rep->head, &r2);
    CHECK_EQ(rep->size, 2u);
    CHECK(rep->head->next->value == &r1); // insert before the sentinel appends
    CHECK(rep->head->prev->value == &r2);

    auto* idx = new Cls_1403331d0();
    idx->list = new PtrListHandle(rep);
    idx->current = &cur;
    Cls_1403331d0::destroy(idx, 1); // 140333190 -> 1403331d0: detach_records(true), delete list
    CHECK(r1.a == nullptr);
    CHECK(r1.b == nullptr);
    CHECK(r2.a == nullptr);
    CHECK(cur.a == nullptr);
    CHECK(cur.b == nullptr);
}

TEST_CASE(sdctl_index_partial_detach_keeps_a)
{
    SdctlIndexRecord r1{&r1, &r1};
    Cls_1403331d0 idx;
    auto* rep = new PtrListRep();
    rep->insert(rep->head, &r1);
    idx.list = new PtrListHandle(rep);
    idx.detach_records(false); // 140333230(this, 0)
    CHECK(r1.a == &r1);
    CHECK(r1.b == nullptr);
}

TEST_CASE(sdctl_index_shared_list_is_copied_before_iteration)
{
    SdctlIndexRecord r1{&r1, &r1};
    auto* rep = new PtrListRep();
    rep->insert(rep->head, &r1);
    rep->refs = 2; // shared with another handle
    PtrListHandle h(rep);
    PtrListIter b = h.begin(); // 140333670 -> 140333820 -> 140333910: copy
    CHECK(h.rep != rep);
    CHECK_EQ(h.rep->refs, 1);
    CHECK_EQ(h.rep->size, 1u);
    CHECK(*b == &r1);
    CHECK_EQ(rep->refs, 1);
    release(rep); // 140333870
}

int main() { return testkit::run_all(); }
