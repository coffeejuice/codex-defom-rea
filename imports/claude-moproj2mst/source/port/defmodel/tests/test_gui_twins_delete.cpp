// ctest-labels: mdt gui_twins
#define _CRT_SECURE_NO_WARNINGS
// Tier-1 tests of DEF_PRE "delete object" 14019cdc0 and its callees 1401a9640, 1401fae90, 1401fab70,
// 14019ce70, 1401b05c0, 14017a960 (defmodel/gui_twins_delete.hpp). Expected values are worked out by
// hand from the decompile (deform-rea-work/out/mdtdel/del.c, del2.c).
#include <defmodel/gui_twins_delete.hpp>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>
#include <vector>

#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <testkit/check.hpp>

using namespace def::model;

namespace {

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}

// zero-filled storage for a class whose ctor the test does not need (never destroyed)
template <class T>
T* zeroed()
{
    void* p = ::operator new(sizeof(T), std::align_val_t{16});
    std::memset(p, 0, sizeof(T));
    return static_cast<T*>(p);
}

// a model with objects 1..n created by 14019ccc0 and given the kinds `kinds`
struct Model {
    DB_STEP_DATA m{3};
    explicit Model(std::vector<std::int32_t> kinds)
    {
        llobj_ctor::get_or_create(&m, static_cast<std::int32_t>(kinds.size()));
        for (std::size_t i = 0; i < kinds.size(); ++i)
            at<std::int32_t>(m.objects.at(static_cast<std::int32_t>(i + 1)), 0xc) = kinds[i];
    }
    Cls_1401f46e0* pair(std::int32_t master, std::int32_t slave)
    {
        Cls_1401f46e0* p = pair_append(&m.inter_obj);
        p->u168 = master;
        p->u16c = slave;
        return p;
    }
    std::vector<std::int32_t> numbers() const
    {
        std::vector<std::int32_t> v;
        for (LLOBJ_DATA* o = m.objects.head; o != nullptr; o = o->next) v.push_back(at<std::int32_t>(o, 8));
        return v;
    }
    std::vector<std::int32_t> pairs() const
    {
        std::vector<std::int32_t> v;
        for (Cls_1401f46e0* p = m.inter_obj.head; p != nullptr; p = p->next) {
            v.push_back(p->u168);
            v.push_back(p->u16c);
        }
        return v;
    }
};

} // namespace

TEST_CASE(object_list_remove_head_middle_tail_missing)
{
    Model a({0, 0, 0, 0});
    LLOBJ_DATA* o2 = a.m.objects.at(2);
    LLOBJ_DATA* o4 = a.m.objects.at(4);
    CHECK(object_list_remove(&a.m.objects, o2));
    const std::vector<std::int32_t> n1 = a.numbers();
    CHECK(n1 == (std::vector<std::int32_t>{1, 3, 4}));
    CHECK_EQ(a.m.objects.count, 3);
    CHECK(object_list_remove(&a.m.objects, o4));       // tail: the new tail is its predecessor
    CHECK(a.m.objects.tail == a.m.objects.at(2));
    CHECK(a.m.objects.tail->next == nullptr);
    CHECK(object_list_remove(&a.m.objects, a.m.objects.head));   // head
    CHECK_EQ(at<std::int32_t>(a.m.objects.head, 8), 3);
    CHECK(a.m.objects.head == a.m.objects.tail);
    CHECK(!object_list_remove(&a.m.objects, nullptr));  // not in the list: nothing, false
    CHECK_EQ(a.m.objects.count, 1);
    object_list_remove_number(&a.m.objects, 7);         // missing number: nothing
    CHECK_EQ(a.m.objects.count, 1);
    LLOBJ_DATA* last = a.m.objects.head;
    object_list_remove_number(&a.m.objects, 3);
    CHECK_EQ(a.m.objects.count, 0);
    CHECK(a.m.objects.head == nullptr);
    // the only node: prev is the node itself, so the tail keeps the deleted node's address (the
    // next add sees count 0 and resets head and tail)
    CHECK(a.m.objects.tail == last);
    a.m.objects.tail = nullptr;
    a.m.u1378 = 0;
}

TEST_CASE(pair_remove_object_master_or_slave)
{
    Model a({0, 0});
    a.pair(1, 2);
    a.pair(3, 4);
    a.pair(2, 5);
    a.pair(6, 2);
    a.pair(4, 3);
    pair_remove_object(&a.m.inter_obj, 2);
    const std::vector<std::int32_t> p = a.pairs();
    CHECK(p == (std::vector<std::int32_t>{3, 4, 4, 3}));
    CHECK_EQ(a.m.inter_obj.count, 2);
    CHECK(a.m.inter_obj.tail->u168 == 4);
    Cls_1401f46e0* last = a.m.inter_obj.tail;
    pair_remove_object(&a.m.inter_obj, 4);
    CHECK_EQ(a.m.inter_obj.count, 0);
    CHECK(a.m.inter_obj.head == nullptr);
    CHECK(a.m.inter_obj.tail == last);                  // only node removed: tail keeps its address
    a.m.inter_obj.tail = nullptr;
    Cls_1401f46e0 other(&a.m);
    CHECK(!pair_remove(&a.m.inter_obj, &other));
}

TEST_CASE(clear_bc_refs_of_other_meshed_objects)
{
    // hand-built model: u1378 = 3, width (+0x1640) 2; object 1 (mesh, 2 nodes), 2 (mesh, the
    // deleted number itself), 3 (no mesh)
    DB_STEP_DATA* s = zeroed<DB_STEP_DATA>();
    at<std::int32_t>(s, 0x1640) = 2;
    std::int32_t bc1[4] = {-2, 1, -2, -3};
    std::int32_t bc2[4] = {-2, -2, 0, 0};
    std::int32_t bc3[4] = {-2, -2, -2, -2};
    std::int32_t* bcs[3] = {bc1, bc2, bc3};
    LLOBJ_DATA* prev = nullptr;
    for (int i = 0; i < 3; ++i) {
        LLOBJ_DATA* o = zeroed<LLOBJ_DATA>();
        at<std::int32_t>(o, 8) = i + 1;
        at<std::int32_t>(o, 0x87c) = i == 2 ? 0 : 1;
        at<std::int32_t>(o, 0x880) = 2;
        at<std::int32_t*>(o, 0x990) = bcs[i];
        if (prev) prev->next = o; else s->objects.head = o;
        s->objects.tail = o;
        prev = o;
    }
    s->objects.count = 3;
    s->u1378 = 3;
    step_clear_bc_refs(*s, 2);
    CHECK(bc1[0] == 0 && bc1[1] == 1 && bc1[2] == 0 && bc1[3] == -3);
    CHECK(bc2[0] == -2 && bc2[1] == -2);                // object 2 itself is skipped
    CHECK(bc3[0] == -2 && bc3[3] == -2);                // no mesh: skipped
}

TEST_CASE(trim_empty_tail_by_position_number)
{
    Model a({0, 7, 7});
    const char flag = 0;
    CHECK(step_trim_empty_tail(a.m, &flag));
    CHECK_EQ(a.m.u1378, 1);
    CHECK_EQ(a.m.objects.count, 1);
    CHECK(!step_trim_empty_tail(a.m, &flag));          // object 1 is kind 0: stop at once
    CHECK_EQ(a.m.u1378, 1);

    // position 3 holds object 4 (numbers 1, 2, 4): the kind-7 object at position 3 makes the
    // trim remove object NUMBER 3, which is missing, so nothing is unlinked; u1378 still drops
    Model b({0, 7, 7, 7});
    object_list_remove(&b.m.objects, b.m.objects.at(3));
    b.m.u1378 = 3;
    CHECK(step_trim_empty_tail(b.m, &flag));
    // pos 3: object 4 (kind 7), remove number 3 (missing) -> u1378 2; pos 2: object 2 (kind 7),
    // remove number 2 -> u1378 1; pos 1: object 1 kind 0 -> stop
    CHECK_EQ(b.m.u1378, 1);
    const std::vector<std::int32_t> nb = b.numbers();
    CHECK(nb == (std::vector<std::int32_t>{1, 4}));
}

TEST_CASE(trim_reports_the_null_dereference)
{
    Model a({7, 7});
    a.m.u1378 = 4;                                      // positions 3, 4 hold no object
    const char flag = 0;
    bool hit = false;
    CHECK(!step_trim_empty_tail(a.m, &flag, &hit));
    CHECK(hit);
    a.m.u1378 = 2;
}

TEST_CASE(delete_object_last_and_middle)
{
    Model a({0, 0, 0});
    a.pair(1, 3);
    a.pair(2, 1);
    a.pair(3, 2);
    bool hit = false;
    step_delete_object(a.m, 9, &hit);                   // missing: nothing
    CHECK_EQ(a.m.u1378, 3);
    CHECK_EQ(a.m.inter_obj.count, 3);
    step_delete_object(a.m, 2, &hit);                   // middle: emptied (kind 7), kept
    CHECK_EQ(a.m.u1378, 3);
    LLOBJ_DATA* o2 = a.m.objects.at(2);
    CHECK_EQ(at<std::int32_t>(o2, 0xc), 7);             // 140161260
    CHECK_EQ(at<char>(o2, 0x828), 0);
    const std::vector<std::int32_t> p1 = a.pairs();
    CHECK(p1 == (std::vector<std::int32_t>{1, 3}));
    step_delete_object(a.m, 3, &hit);                   // last: emptied, then 3 and 2 trimmed
    CHECK(!hit);
    CHECK_EQ(a.m.u1378, 1);
    const std::vector<std::int32_t> n = a.numbers();
    CHECK(n == (std::vector<std::int32_t>{1}));
    CHECK_EQ(a.m.inter_obj.count, 0);
}

int main()
{
    return testkit::run_all();
}
