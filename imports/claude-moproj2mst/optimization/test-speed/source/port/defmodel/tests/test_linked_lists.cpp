// Tier-1 tests of the plain lists of p1-06: the point/direction list (140257340 ..
// 140258400), the 0xe8 record list (140259c70), the window list Cls_140269250 and the
// inter-object list Cls_1401fa020 (empty paths only: their records need the curve init of
// key-common and LLWINDEF_DATA, neither of which is in this branch), and the raw accessors.
// ctest-labels: p1-06 linked
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/linked/accessors.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <cstring>

using namespace def::model;
using namespace def::model::linked;

TEST_CASE(point_node_ctor_and_copy)
{
    Cls_140257340 a;
    CHECK_EQ(a.id, 0);
    CHECK_BITS(a.point[2], 0.0);
    CHECK_BITS(a.direction[0], 0.0);
    CHECK(a.next == nullptr);
    Cls_140257340 b;
    b.id = 7;
    b.point[1] = 2.5;
    b.direction[2] = -1.0;
    b.next = &a;
    a.copy_from(&b);
    CHECK_EQ(a.id, 7);
    CHECK_BITS(a.point[1], 2.5);
    CHECK_BITS(a.direction[2], -1.0);
    CHECK(a.next == nullptr);
}

TEST_CASE(point_list_add_nth_copy_clear)
{
    Cls_140257340List l;
    Cls_140257340* n1 = l.add(10);
    Cls_140257340* n2 = l.add(20);
    Cls_140257340* n3 = l.add(30);
    CHECK_EQ(l.size(), 3);
    CHECK(l.head == n1 && l.tail == n3 && n1->next == n2 && n2->next == n3);
    CHECK(l.nth(0) == n1); // k <= 1 gives the head
    CHECK(l.nth(1) == n1);
    CHECK(l.nth(3) == n3);
    CHECK(l.nth(4) == nullptr);
    CHECK_EQ(n2->id, 20);

    n2->point[0] = 4.0;
    Cls_140257340List c;
    c.add(99);
    c.copy_from(l);
    CHECK_EQ(c.size(), 3);
    CHECK_EQ(c.nth(2)->id, 20); // the copy overwrites the id given to add(0)
    CHECK_BITS(c.nth(2)->point[0], 4.0);
    CHECK(c.tail->next == nullptr);

    l.rotate_all(nullptr, nullptr, 30.0); // null axis: nothing changes
    CHECK_BITS(n2->point[0], 4.0);

    l.clear();
    CHECK(l.head == nullptr && l.tail == nullptr && l.size() == 0);
}

TEST_CASE(record_e8_list_empty_clear)
{
    Cls_140258580List l;
    l.clear();
    CHECK(l.head == nullptr && l.tail == nullptr && l.count == 0);
}

TEST_CASE(window_list_init)
{
    Cls_140269250 w;
    CHECK(w.head == nullptr && w.tail == nullptr);
    CHECK_EQ(w.count, 0);
    CHECK_EQ(w.u10, -1);
    CHECK_EQ(w.u18, 2);
    CHECK(w.owner == nullptr);
    CHECK(w.u28 == 0);
    const std::int32_t three = 3;
    w.set_dimension(&three);
    CHECK_EQ(w.u18, 3);
    w.set_owner(nullptr); // no owner: dimension kept
    CHECK_EQ(w.u18, 3);
    w.clear();
    CHECK_EQ(w.u10, -1); // clear keeps u10 and u18
    CHECK_EQ(w.u18, 3);
}

TEST_CASE(inter_object_list_empty)
{
    Cls_1401fa020 l(nullptr);
    CHECK(l.head == nullptr && l.tail == nullptr && l.count == 0 && l.owner == nullptr);
    l.count = 5; // reset does not free, only zeroes
    l.reset();
    CHECK_EQ(l.count, 0);
    l.clear();
    CHECK_EQ(l.count, 0);
}

TEST_CASE(raw_accessors)
{
    alignas(8) static unsigned char obj[0xa00];
    std::memset(obj, 0, sizeof obj);
    const std::int32_t v44 = 44, v18 = 18, v24 = 24;
    std::memcpy(obj + 0x44, &v44, 4);
    std::memcpy(obj + 0x24, &v24, 4);
    std::memcpy(obj + 0x18, &v18, 4);
    CHECK_EQ(get_i32_44(obj), 44);
    CHECK_EQ(get_i32_18(obj), 18);
    CHECK_EQ(get_i32_24(obj), 24);
    set_u64_20(obj, 0x1122334455667788ull);
    std::uint64_t r = 0;
    std::memcpy(&r, obj + 0x20, 8);
    CHECK(r == 0x1122334455667788ull);
    set_u64_18(obj, 5);
    CHECK_EQ(get_i32_18(obj), 5);

    // handle members: +0x10 and +0x20 point at objects whose +0x10 holds a count
    alignas(8) unsigned char rep1[0x20] = {};
    alignas(8) unsigned char rep2[0x20] = {};
    const std::uint64_t c1 = 3, c2 = 8;
    std::memcpy(rep1 + 0x10, &c1, 8);
    std::memcpy(rep2 + 0x10, &c2, 8);
    void* p1 = rep1;
    void* p2 = rep2;
    std::memcpy(obj + 0x10, &p1, 8);
    std::memcpy(obj + 0x20, &p2, 8);
    CHECK(member10_deref_u64_10(obj) == 3);
    CHECK(member20_deref_u64_10(obj) == 8);

    double coords[3] = {1.0, 2.0, 3.0};
    double* first = coords;
    std::memcpy(rep1 + 0x08, &first, 8);
    CHECK(member10_array_first(obj) == coords);

    // 1401fc040: flag at 0x87c, pointer at 0x9f8
    CHECK_EQ(fun_1401fc040(obj), 0);
    const std::int32_t on = 1, val = 42;
    std::memcpy(obj + 0x87c, &on, 4);
    CHECK_EQ(fun_1401fc040(obj), 0); // pointer still null
    const std::int32_t* pv = &val;
    std::memcpy(obj + 0x9f8, &pv, 8);
    CHECK_EQ(fun_1401fc040(obj), 42);
}

TEST_CASE(sim_control_dsmax_setter)
{
    // only the member store is exercised, on raw storage (the class ctor is p1-03's)
    alignas(Cls_1401e7b10) static unsigned char buf[sizeof(Cls_1401e7b10)];
    std::memset(buf, 0, sizeof buf);
    auto* s = reinterpret_cast<Cls_1401e7b10*>(buf);
    set_dsmax(s, 0.5);
    double d = 0.0;
    std::memcpy(&d, buf + 0x1e0, 8);
    CHECK_BITS(d, 0.5);
}

int main() { return testkit::run_all(); }
