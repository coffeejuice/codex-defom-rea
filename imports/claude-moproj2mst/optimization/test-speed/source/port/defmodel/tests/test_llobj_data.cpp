// LLOBJ_DATA: virtual slots (14015fa60/14015faa0/14015fbd0), reset 14015fbf0 (call order and the
// fields it writes, with and without the full flag), 140162070, 140197ec0, 140199b10/1401999d0/
// 140199b60 (absorbed from p1-05), 140158f50; the object list Cls_14017a6c0 (14017a6c0..14017ab70);
// the accessors 140158ec0/140158f10/140158f30/140159190/140158fa0. The LLOBJ_DATA ctor is Phase 2,
// so the object is zeroed storage with the fields each case needs.
// ctest-labels: p1-04 llobj
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/accessors.hpp>
#include <defmodel/llobj/llobj_parts.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>
#include <new>
#include <vector>

#include "llobj_recorder.hpp"

using namespace def::model;
using llobj_test::Call;
using llobj_test::raw_object;
using llobj_test::Recorder;
using llobj_test::u;

namespace {
using Trace = std::vector<std::pair<std::uint64_t, std::uint64_t>>;

LLOBJ_DATA* raw_llobj() { return raw_object<LLOBJ_DATA>(); }
void free_llobj(LLOBJ_DATA* o) { ::operator delete(o); }
} // namespace

TEST_CASE(virtual_slots_name_and_first_geometry)
{
    LLOBJ_DATA* o = raw_llobj();
    // zeroed storage has no vptr: call the overrides directly
    o->LLOBJ_DATA::set_name("DIE");
    char out[0x54] = {};
    o->LLOBJ_DATA::get_name(out);
    CHECK(std::strcmp(out, "DIE") == 0);
    CHECK(std::strcmp(o->name, "DIE") == 0);
    int g = 0;
    o->u3c00[0] = &g;
    CHECK(o->LLOBJ_DATA::vslot2() == &g);
    free_llobj(o);
}

TEST_CASE(reset_full_call_order_and_fields)
{
    Recorder rec;
    LLOBJ_DATA* o = raw_llobj();
    auto* owner = reinterpret_cast<DB_STEP_DATA*>(std::uintptr_t{0x1234560});
    o->owner = owner;
    int g1 = 0, g2 = 0;
    o->u3c00[0] = &g1;
    o->u3c00[57] = &g2;
    int sv = 0;
    o->u3998 = &sv;
    o->u3bf8 = u(&g1);
    o->u39a0 = u(std::malloc(16));
    o->u39c8 = u(std::malloc(16));
    o->u39d8 = u(std::malloc(16));
    o->u39d0 = o->u39d4 = o->u39e0 = o->u39e4 = o->u39e8 = o->u39ec = 9;
    o->u39f0 = 0;
    o->u010 = o->u014 = o->u818 = o->u81c = o->u820 = 5;
    std::memcpy(o->u018, "abc", 4);
    std::memcpy(o->u418, "def", 4);
    o->u3990 = 1;
    for (auto& v : o->u87c) v = 3;
    llobj::parts::part_3920(*o).count = 0;

    const char full = 1;
    o->reset(&full);

    Trace want = {{0x140329dc0, 0x8a0}, {0x140329dc0, 0x9e0},
                  {0x1402f9340, u(&g1) - u(o)}, {0x1, u(&g1) - u(o)},
                  {0x1402f9340, u(&g2) - u(o)}, {0x1, u(&g2) - u(o)},
                  {0x1402f9340, 0x3328}, {0x1402f9340, 0x3458},
                  {0x14012f480, 0x1ea0 + 0xc0}, {0x14012f480, 0x1ea0 + 0x140}, {0x140259c70, 0x1ea0 + 0x200},
                  {0x14025ab20, 0x1ea0 + 0x218}, {0x14012f480, 0x1ea0 + 0x238}};
    for (std::uint64_t t : {0x0e8, 0x070, 0x208, 0x2a0, 0x3b0, 0x448, 0x660, 0x6d8, 0x8c0, 0x968, 0x9e0, 0x790, 0x808})
        want.emplace_back(0x14012f480, 0x2160 + t);
    want.emplace_back(0x1402699c0, 0x2d10 + 0x40);
    want.emplace_back(0x1402581b0, 0x2e68 + 0x18);
    for (std::uint64_t t : {0x48, 0x78, 0xa8, 0x168, 0xd8, 0x108, 0x138}) want.emplace_back(0x1402699c0, 0x2e68 + t);
    want.emplace_back(0x140301d10, u(&sv) - u(o));
    want.emplace_back(0x140301ad0, u(&sv) - u(o));
    want.emplace_back(0x14019abb0, 0x3a60);
    want.emplace_back(0x2, u(&g1) - u(o));
    Trace got;
    for (const Call& c : rec.calls) got.emplace_back(c.addr, c.a[0] - u(o));
    CHECK(got == want);

    CHECK(o->u3c00[0] == nullptr);
    CHECK(o->u3c00[57] == nullptr);
    CHECK_EQ(o->u3bf8, 0u);
    CHECK_EQ(o->u39a0, 0u);
    CHECK_EQ(o->u39c8, 0u);
    CHECK_EQ(o->u39d0, 0);
    CHECK_EQ(o->u39d8, 0u);
    CHECK_EQ(o->u39d4, 0);
    CHECK_EQ(o->u39e0, 0);
    CHECK_EQ(o->u39ec, 0);
    CHECK_EQ(o->u39f0, 1);
    CHECK(o->u3998 == &sv);  // reset leaves the object; 140197ec0 deletes it
    CHECK_EQ(o->u010, 0);
    CHECK_EQ(o->u014, 0);
    CHECK_EQ(o->u818, 0);
    CHECK_EQ(o->u81c, 0);
    CHECK_EQ(o->u820, 0);
    CHECK(o->u018[0] == '\0');
    CHECK(o->u418[0] == '\0');
    CHECK_EQ(o->u3990, 0);
    for (auto v : o->u87c) CHECK_EQ(v, 0);
    for (int i = 0; i < 100; ++i) CHECK(llobj::parts::part_be0(*o, i).u028 == owner);
    CHECK(o->get_owner() == owner);
    free_llobj(o);
}

TEST_CASE(reset_not_full_keeps_geometry_objects)
{
    Recorder rec;
    LLOBJ_DATA* o = raw_llobj();
    int g = 0;
    o->u3c00[3] = &g;
    const char full = 0;
    o->reset(&full);
    CHECK(o->u3c00[3] == &g);
    CHECK(rec.only(0x1402f9340).size() == 3u);  // the object, 0x3328, 0x3458
    CHECK(rec.only(0x1).empty());
    CHECK(rec.only(0x140301d10).empty());  // u3998 == 0
    CHECK(rec.only(0x2).empty());          // u3bf8 == 0
    free_llobj(o);
}

TEST_CASE(delete_u3998_and_free_u39a0)
{
    Recorder rec;
    LLOBJ_DATA* o = raw_llobj();
    int sv = 0;
    o->u3998 = &sv;
    o->delete_u3998();
    REQUIRE(rec.calls.size() == 1u);
    CHECK_EQ(rec.calls[0].addr, 0x14019a670u);
    CHECK_EQ(rec.calls[0].a[0], u(&sv));
    CHECK_EQ(rec.calls[0].a[1], 1u);
    CHECK(o->u3998 == nullptr);
    o->delete_u3998();
    CHECK(rec.calls.size() == 1u);
    o->free_u39a0();
    CHECK_EQ(o->u39a0, 0u);
    free_llobj(o);
}

TEST_CASE(object_list_ctor_at_clear)
{
    Recorder rec;
    std::vector<LLOBJ_DATA*> deleted;
    rec.result = [&](const Call& c) -> std::uint64_t {
        if (c.addr == 0x14019a290) {
            deleted.push_back(reinterpret_cast<LLOBJ_DATA*>(c.a[0]));
            ::operator delete(reinterpret_cast<void*>(c.a[0]));
        }
        return 0;
    };
    auto* owner = reinterpret_cast<DB_STEP_DATA*>(std::uintptr_t{0x7770});
    {
        Cls_14017a6c0 list(owner);
        CHECK(list.owner == owner);
        CHECK(list.head == nullptr);
        CHECK_EQ(list.count, 0);
        LLOBJ_DATA* a = raw_llobj();
        LLOBJ_DATA* b = raw_llobj();
        a->next = b;
        list.head = a;
        list.tail = b;
        list.count = 2;
        CHECK(list.at(0) == a);
        CHECK(list.at(1) == a);
        CHECK(list.at(2) == b);
        CHECK(list.at(3) == nullptr);
        list.clear();
        REQUIRE(deleted.size() == 2u);
        CHECK(deleted[0] == a);
        CHECK(deleted[1] == b);
        CHECK(list.head == nullptr);
        CHECK(list.tail == nullptr);
        CHECK_EQ(list.count, 0);
        CHECK(list.owner == owner);  // reset keeps the owner
    }
    CHECK(deleted.size() == 2u);  // dtor on the empty list deletes nothing
}

TEST_CASE(accessors_list_sizes_and_step_flag)
{
    Recorder rec;
    std::byte obj[0x20] = {};
    auto* l8 = new (obj + 8) CowList<void*>();
    l8->push_back(nullptr);
    l8->push_back(nullptr);
    CHECK_EQ(llobj::list8_size_a(obj), 2u);
    CHECK_EQ(llobj::list8_size_b(obj), 2u);
    std::byte obj2[0x20] = {};
    auto* l18 = new (obj2 + 0x18) CowList<void*>();
    CHECK_EQ(llobj::list18_size(obj2), 0u);

    std::byte item[0x10] = {};
    auto* li = new (item + 8) CowList<void*>();
    rec.result = [&](const Call& c) -> std::uint64_t {
        if (c.addr == 0x140283f80) {
            CHECK_EQ(*reinterpret_cast<const std::int32_t*>(c.a[1]), 0);
            return u(item);
        }
        return 0;
    };
    CHECK_EQ(llobj::first_item_has_items(obj), 0);  // first item's list is empty
    li->push_back(nullptr);
    CHECK_EQ(llobj::first_item_has_items(obj), 1);
    CHECK_EQ(llobj::first_item_has_items(obj2 + 0x10), 0);  // own list (at +8 = obj2+0x18) empty
    CHECK(rec.only(0x140283f80).size() == 2u);
    li->~CowList();
    l18->~CowList();
    l8->~CowList();

    auto* step = static_cast<DB_STEP_DATA*>(::operator new(sizeof(DB_STEP_DATA)));
    std::memset(static_cast<void*>(step), 0, sizeof(DB_STEP_DATA));
    const std::uint8_t one = 1;
    llobj::set_step_u1b5c(step, &one);
    CHECK_EQ(reinterpret_cast<const std::uint8_t*>(step)[0x1b5c], 1);
    ::operator delete(step);
}

int main() { return testkit::run_all(); }
