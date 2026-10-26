// ctest-labels: kw-g2obj
// LLOBJ_DATA copy from another object (1401613a0) and its callees (llobj_ctor/llobj_copy.hpp).
// The LLOBJ_DATA ctor is not runnable here, so both objects are zeroed storage with the fields
// each case needs; calls into other packets are recorded through llobj::ext::g_hook. Expected
// values follow the decompile (deform-rea-work/out/g2obj/copy_closure.c, copy_deep*.c).
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <testkit/check.hpp>

#include <cstdint>
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

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(p) + off);
}

struct Owner {
    std::byte bytes[0x40] = {};
    explicit Owner(std::int32_t dim) { std::memcpy(bytes + 0x1c, &dim, 4); }
};

// std::list body of the object at +0x3998: +8 head node, +0x10 size; node +0 next, value at +0x10
struct Node {
    Node* next;
    void* prev;
    std::uint64_t value;
};
struct Body {
    void* alloc;
    Node* head;
    std::uint64_t size;
};
struct FakeList {
    Node head{};
    Node n1{};
    Body body{};
    Body* holder = nullptr;  // the 8-byte object at LLOBJ_DATA+0x3998 points at the body
    FakeList()
    {
        head.next = &n1;
        n1.next = &head;
        body.head = &head;
        body.size = 1;
        holder = &body;
    }
};

std::vector<std::byte*> g_records;

// Emulates the p1-09 iterator helpers and the LLWINDEF_DATA allocation; everything else 0.
std::uint64_t emulate(const Call& c)
{
    switch (c.addr) {
    case 0x1403020c0:  // iterator ctor
        *reinterpret_cast<std::uint64_t*>(c.a[0]) = c.a[1];
        return c.a[0];
    case 0x140301e30: {  // end(): the head node
        Body* b = *reinterpret_cast<Body**>(c.a[0]);
        *reinterpret_cast<std::uint64_t*>(c.a[1]) = u(b->head);
        return c.a[1];
    }
    case 0x140302020:  // *it: address of the node's value
        return *reinterpret_cast<std::uint64_t*>(c.a[0]) + 0x10;
    case 0x140302040: {  // it++
        auto* it = reinterpret_cast<std::uint64_t*>(c.a[0]);
        *reinterpret_cast<std::uint64_t*>(c.a[1]) = *it;
        *it = u(reinterpret_cast<Node*>(*it)->next);
        return c.a[1];
    }
    case 0x14025c100: {  // operator new(0x920) + LLWINDEF_DATA ctor
        auto* r = static_cast<std::byte*>(::operator new(0x920));
        std::memset(r, 0, 0x920);
        g_records.push_back(r);
        return u(r);
    }
    default:
        return 0;
    }
}

std::size_t count(const Recorder& rec, std::uint64_t addr) { return rec.only(addr).size(); }

LLOBJ_DATA* raw_llobj() { return raw_object<LLOBJ_DATA>(); }

} // namespace

TEST_CASE(null_source_only_clears)
{
    Recorder rec;
    rec.result = emulate;
    Owner owner(2);
    LLOBJ_DATA* d = raw_llobj();
    d->owner = reinterpret_cast<DB_STEP_DATA*>(&owner);
    at<std::int32_t>(d, 0x87c) = 5;
    at<std::int32_t>(d, 0xc) = 9;
    std::int32_t sizes[0x40] = {};
    bool t = true;
    llobj_copy_from(d, nullptr, sizes, &t, &t, &t, &t, &t, &t);
    CHECK_EQ(at<std::int32_t>(d, 0x87c), 0);  // 14015fbf0 cleared it
    CHECK_EQ(at<std::int32_t>(d, 0xc), 9);    // nothing copied
    CHECK_EQ(count(rec, 0x140329e60), std::size_t{0});
    ::operator delete(d);
}

TEST_CASE(copy_2d_owner)
{
    Recorder rec;
    rec.result = emulate;
    g_records.clear();
    Owner owner(2);
    LLOBJ_DATA* d = raw_llobj();
    LLOBJ_DATA* s = raw_llobj();
    void* own = &owner;
    d->owner = reinterpret_cast<DB_STEP_DATA*>(own);
    s->owner = reinterpret_cast<DB_STEP_DATA*>(own);
    at<void*>(d, 0x1ea0 + 0x2b8) = own;  // property sub-object owner
    at<void*>(d, 0x2d10) = own;          // mesh-generation owner
    at<void*>(d, 0x2e68 + 0x198) = own;  // boundary-condition owner

    at<std::int32_t>(s, 0x8) = 11;
    at<std::int32_t>(s, 0xc) = 12;
    std::memcpy(reinterpret_cast<char*>(s) + 0x828, "BILLET", 7);
    at<std::int32_t>(s, 0x87c) = 1;
    at<std::int32_t>(s, 0x880) = 0;
    at<std::int32_t>(s, 0x884) = 2;
    at<std::int32_t>(s, 0x888) = 7;
    at<std::int32_t>(s, 0x88c) = 99;
    at<std::int32_t>(s, 0x890) = 2;
    at<std::int32_t>(s, 0x3984) = 31;
    at<std::int32_t>(s, 0x3988) = 32;
    at<std::int32_t>(s, 0x3588) = 33;
    at<std::int32_t>(s, 0x358c) = 34;
    at<std::int32_t>(s, 0x398c) = 35;
    at<std::int32_t>(s, 0x3980) = 36;
    at<std::int32_t>(s, 0x3aa0) = 41;
    at<std::int32_t>(s, 0x3aa4) = 42;
    at<std::uint8_t>(s, 0x3aac) = 43;
    at<std::uint64_t>(s, 0x3ab0 + 19 * 8) = 44;

    // layers: 0 has n = 3 (2D contribution 2), 1 has n = 1, m = 2 (contribution 0)
    at<std::int32_t>(s, 0xbd8) = 2;
    std::int32_t l0[3] = {1, 2, 3};
    std::byte* sl0 = reinterpret_cast<std::byte*>(s) + 0xbe0;
    std::byte* sl1 = sl0 + 0x30;
    at<std::int32_t>(sl0, 0) = 3;
    at<void*>(sl0, 8) = l0;
    std::int32_t l1a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    std::int32_t l1b[2] = {9, 10};
    at<std::int32_t>(sl1, 0) = 1;
    std::int32_t one = 77;
    at<void*>(sl1, 8) = &one;
    at<std::int32_t>(sl1, 0x10) = 2;
    at<void*>(sl1, 0x18) = l1a;
    at<void*>(sl1, 0x20) = l1b;

    // node arrays: n = 0x88c - 2 + 2 * layers = 2 - 2 + 4 = 4
    std::int32_t a0[4] = {5, 6, 7, 8};
    double a2[4] = {0.5, 1.5, 2.5, 3.5};
    at<void*>(s, 0xae8) = a0;
    at<void*>(s, 0xae8 + 16) = a2;

    // property sub-object
    std::byte* sp = reinterpret_cast<std::byte*>(s) + 0x1ea0;
    at<std::int32_t>(sp, 0x28) = 5;
    at<std::int32_t>(sp, 0x80) = 6;      // 2D block
    at<std::uint64_t>(sp, 0x1b8) = 7;    // 3D block: not copied
    at<std::int32_t>(sp, 0x2b4) = 8;
    DYNARY* sd = reinterpret_cast<DYNARY*>(sp + 0x140);
    double ax[2] = {1.0, 2.0};
    double vals[2] = {10.0, 20.0};
    sd->type = 3;
    sd->ndim = 1;
    sd->n[0] = 2;
    sd->ncomp = 1;
    sd->axis[0] = ax;
    sd->values = vals;

    // movement sub-object
    std::byte* sm = reinterpret_cast<std::byte*>(s) + 0x2160;
    std::memcpy(sm, "MOVE", 5);
    at<std::uint64_t>(sm, 0x648 + 16) = 51;
    at<std::uint8_t>(sm, 0xa78) = 52;

    // mesh-generation sub-object: one window record in the list at +0x40
    std::byte* sg = reinterpret_cast<std::byte*>(s) + 0x2d10;
    at<std::uint64_t>(sg, 0x10) = 61;
    at<std::int32_t>(sg, 0x70) = 62;     // 2D
    at<std::int32_t>(sg, 0x78) = 63;     // 3D: not copied
    at<std::int32_t>(sg, 0xd8) = 2;
    double ge0[2] = {0.25, 0.75};
    std::int32_t ge8[2] = {3, 4};
    at<void*>(sg, 0xe0) = ge0;
    at<void*>(sg, 0xe8) = ge8;
    at<std::int32_t>(sg, 0xf0) = 64;
    // the window list at +0x40 stays empty: its copy is kw-g3's winlist_copy (140269340)

    std::int32_t sizes[0x40] = {};
    sizes[0x3d] = 1;  // read at +0xf4 by 140183770
    const bool t = true, f = false;
    llobj_copy_from(d, s, sizes, &t, &f, &t, &t, &t, &t);

    CHECK_EQ(at<std::int32_t>(d, 0x8), 11);
    CHECK_EQ(at<std::int32_t>(d, 0xc), 12);
    CHECK(std::strcmp(reinterpret_cast<char*>(d) + 0x828, "BILLET") == 0);
    CHECK_EQ(at<std::int32_t>(d, 0x87c), 1);
    CHECK_EQ(at<std::int32_t>(d, 0x884), 2);
    CHECK_EQ(at<std::int32_t>(d, 0x888), 7);
    CHECK_EQ(at<std::int32_t>(d, 0x3588), 33);
    CHECK_EQ(at<std::int32_t>(d, 0x3980), 36);
    CHECK_EQ(at<std::int32_t>(d, 0x398c), 35);
    // 140165500 with kind 2
    CHECK_EQ(at<std::int32_t>(d, 0x890), 2);
    CHECK_EQ(at<std::int32_t>(d, 0x894), 4);
    CHECK_EQ(at<std::int32_t>(d, 0x89c), 3);
    // layers and 140165d30: (3 - 1) + (1 - 1)
    CHECK_EQ(at<std::int32_t>(d, 0xbd8), 2);
    CHECK_EQ(at<std::int32_t>(d, 0x88c), 2);
    std::byte* dl0 = reinterpret_cast<std::byte*>(d) + 0xbe0;
    std::byte* dl1 = dl0 + 0x30;
    CHECK_EQ(at<std::int32_t>(dl0, 0), 3);
    CHECK(at<void*>(dl0, 8) != l0);
    CHECK_EQ(static_cast<std::int32_t*>(at<void*>(dl0, 8))[2], 3);
    CHECK_EQ(static_cast<std::int32_t*>(at<void*>(dl1, 8))[0], 77);
    CHECK_EQ(static_cast<std::int32_t*>(at<void*>(dl1, 0x18))[7], 8);
    CHECK_EQ(static_cast<std::int32_t*>(at<void*>(dl1, 0x20))[1], 10);
    // node arrays of size 4
    CHECK_EQ(static_cast<std::int32_t*>(at<void*>(d, 0xae8))[3], 8);
    CHECK(at<void*>(d, 0xae8 + 8) != nullptr);  // allocated, src has none
    CHECK(static_cast<double*>(at<void*>(d, 0xae8 + 16))[3] == 3.5);
    // properties
    std::byte* dp = reinterpret_cast<std::byte*>(d) + 0x1ea0;
    CHECK_EQ(at<std::int32_t>(dp, 0x28), 5);
    CHECK_EQ(at<std::int32_t>(dp, 0x80), 6);
    CHECK_EQ(at<std::uint64_t>(dp, 0x1b8), std::uint64_t{0});
    CHECK_EQ(at<std::int32_t>(dp, 0x2b4), 8);
    DYNARY* dd = reinterpret_cast<DYNARY*>(dp + 0x140);
    CHECK_EQ(dd->type, 3);
    CHECK_EQ(dd->ndim, 1);
    CHECK_EQ(dd->n[0], 2);
    CHECK(dd->axis[0] != ax && dd->axis[0][1] == 2.0);
    CHECK(dd->values != vals && dd->values[1] == 20.0);
    // movement
    std::byte* dm = reinterpret_cast<std::byte*>(d) + 0x2160;
    CHECK(std::strcmp(reinterpret_cast<char*>(dm), "MOVE") == 0);
    CHECK_EQ(at<std::uint64_t>(dm, 0x648 + 16), std::uint64_t{51});
    CHECK_EQ(at<std::uint8_t>(dm, 0xa78), std::uint8_t{52});
    // mesh generation
    std::byte* dg = reinterpret_cast<std::byte*>(d) + 0x2d10;
    CHECK_EQ(at<std::uint64_t>(dg, 0x10), std::uint64_t{61});
    CHECK_EQ(at<std::int32_t>(dg, 0x70), 62);
    CHECK_EQ(at<std::int32_t>(dg, 0x78), 0);
    CHECK_EQ(at<std::int32_t>(dg, 0xd8), 2);
    CHECK(static_cast<double*>(at<void*>(dg, 0xe0))[1] == 0.75);
    CHECK_EQ(static_cast<std::int32_t*>(at<void*>(dg, 0xe8))[1], 4);
    CHECK_EQ(at<std::int32_t>(dg, 0xf0), 64);
    CHECK_EQ(count(rec, 0x1402f9b60), std::size_t{2});  // 140199c20 x2; window: no mesh
    CHECK_EQ(count(rec, 0x140257c80), std::size_t{0});  // 2D owner
    CHECK_EQ(count(rec, 0x4), std::size_t{0});
    // the tail
    CHECK_EQ(at<std::int32_t>(d, 0x3aa0), 41);
    CHECK_EQ(at<std::int32_t>(d, 0x3aa4), 42);
    CHECK_EQ(at<std::uint8_t>(d, 0x3aac), std::uint8_t{43});
    CHECK_EQ(at<std::uint64_t>(d, 0x3ab0 + 19 * 8), std::uint64_t{44});
}

TEST_CASE(copy_3d_owner_flags_off)
{
    Recorder rec;
    rec.result = emulate;
    g_records.clear();
    Owner owner(3);
    LLOBJ_DATA* d = raw_llobj();
    LLOBJ_DATA* s = raw_llobj();
    void* own = &owner;
    d->owner = reinterpret_cast<DB_STEP_DATA*>(own);
    at<void*>(d, 0x1ea0 + 0x2b8) = own;
    at<void*>(d, 0x2d10) = own;
    at<void*>(d, 0x2e68 + 0x198) = own;
    int geoA = 0, geoB = 0;
    d->u3c00[5] = &geoA;
    s->u3c00[5] = &geoB;
    at<std::uint8_t>(s, 0x3990) = 1;
    at<std::int32_t>(s, 0x3994) = 77;
    at<std::int32_t>(d, 0x8) = 3;
    at<std::int32_t>(s, 0x8) = 11;
    std::memcpy(reinterpret_cast<char*>(s) + 0x828, "DIE", 4);
    at<std::int32_t>(s, 0x890) = 4;
    at<std::int32_t>(s, 0x87c) = 1;
    at<std::int32_t>(s, 0x884) = 1;
    at<std::int32_t>(s, 0xbd8) = 150;  // clamped to 100
    // every layer has m = 0 except layer 0 (m = 3): 3D contribution m
    at<std::int32_t>(reinterpret_cast<std::byte*>(s) + 0xbe0, 0x10) = 3;
    std::int32_t m16[12] = {};
    std::int32_t m3[3] = {};
    at<void*>(reinterpret_cast<std::byte*>(s) + 0xbe0, 0x18) = m16;
    at<void*>(reinterpret_cast<std::byte*>(s) + 0xbe0, 0x20) = m3;
    std::byte* sp = reinterpret_cast<std::byte*>(s) + 0x1ea0;
    at<std::int32_t>(sp, 0x80) = 6;    // 2D block: not copied
    at<std::uint64_t>(sp, 0x1b8) = 7;  // 3D block
    at<std::uint64_t>(sp, 0x1d8 + 16) = 9;
    std::byte* sg = reinterpret_cast<std::byte*>(s) + 0x2d10;
    at<std::int32_t>(sg, 0x70) = 62;
    at<std::int32_t>(sg, 0x78) = 63;
    at<std::uint64_t>(sg, 0x98) = 64;

    std::int32_t sizes[0x40] = {};
    const bool f = false;
    llobj_copy_from(d, s, sizes, &f, &f, &f, &f, &f, &f);

    CHECK_EQ(at<std::int32_t>(d, 0x8), 3);       // p8 off: not copied (14015fbf0 keeps +0x8)
    CHECK_EQ(reinterpret_cast<char*>(d)[0x828], '\0');  // p7 off
    CHECK_EQ(at<std::int32_t>(d, 0x894), 8);     // kind 4
    CHECK_EQ(at<std::int32_t>(d, 0x898), 6);
    CHECK_EQ(at<std::int32_t>(d, 0x89c), 4);
    CHECK_EQ(at<std::int32_t>(d, 0xbd8), 100);
    CHECK_EQ(at<std::int32_t>(d, 0x88c), 3);     // 140165d30, 3D: sum of m
    // 1401618b0
    CHECK_EQ(at<std::uint8_t>(d, 0x3990), std::uint8_t{1});
    CHECK_EQ(at<std::int32_t>(d, 0x3994), 77);
    const auto slot = rec.only(0x4);
    CHECK_EQ(slot.size(), std::size_t{1});
    if (slot.size() == 1) {
        CHECK_EQ(slot[0].a[0], u(&geoA));
        CHECK_EQ(slot[0].a[1], u(&geoB));
    }
    // 1402f9340 on geoA: by 14015fbf0, then by 1401618b0
    std::size_t resets = 0;
    for (const Call& c : rec.only(0x1402f9340))
        if (c.a[0] == u(&geoA)) ++resets;
    CHECK_EQ(resets, std::size_t{2});
    std::byte* dp = reinterpret_cast<std::byte*>(d) + 0x1ea0;
    CHECK_EQ(at<std::int32_t>(dp, 0x80), 0);
    CHECK_EQ(at<std::uint64_t>(dp, 0x1b8), std::uint64_t{7});
    CHECK_EQ(at<std::uint64_t>(dp, 0x1d8 + 16), std::uint64_t{9});
    std::byte* dg = reinterpret_cast<std::byte*>(d) + 0x2d10;
    CHECK_EQ(at<std::int32_t>(dg, 0x70), 0);
    CHECK_EQ(at<std::int32_t>(dg, 0x78), 63);
    CHECK_EQ(at<std::uint64_t>(dg, 0x98), std::uint64_t{64});
    // 140195c80: 3D owner copies the list at +0x18 (140257c80)
    const auto l3 = rec.only(0x140257c80);
    CHECK_EQ(l3.size(), std::size_t{1});
    if (l3.size() == 1) {
        CHECK_EQ(l3[0].a[0], u(reinterpret_cast<std::byte*>(d) + 0x2e68 + 0x18));
        CHECK_EQ(l3[0].a[1], u(reinterpret_cast<std::byte*>(s) + 0x2e68 + 0x18));
    }
}

TEST_CASE(state_list_copy_2d)
{
    Recorder rec;
    rec.result = emulate;
    Owner owner(2);
    LLOBJ_DATA* d = raw_llobj();
    LLOBJ_DATA* s = raw_llobj();
    void* own = &owner;
    d->owner = reinterpret_cast<DB_STEP_DATA*>(own);
    at<void*>(d, 0x1ea0 + 0x2b8) = own;
    at<void*>(d, 0x2d10) = own;
    at<void*>(d, 0x2e68 + 0x198) = own;
    FakeList dl, sl;
    d->u3998 = &dl.holder;
    s->u3998 = &sl.holder;
    std::int32_t sizes[0x40] = {};
    const bool t = true;
    llobj_copy_from(d, s, sizes, &t, &t, &t, &t, &t, &t);
    // 14015fbf0 once, then 1401619c0 from 140161840 (2D) and from 1401613a0
    CHECK_EQ(count(rec, 0x140301d10), std::size_t{3});
    CHECK_EQ(count(rec, 0x140301ad0), std::size_t{3});  // 1 (reset) + 1 per copy (src size 1)
    const auto cp = rec.only(0x140280490);
    CHECK_EQ(cp.size(), std::size_t{2});
    for (const Call& c : cp) {
        CHECK_EQ(c.a[0], u(&dl.n1) + 0x10);
        CHECK_EQ(c.a[1], u(&sl.n1) + 0x10);
    }
}

TEST_CASE(helpers)
{
    using namespace llobj_copy;
    // 1402585b0 defaults
    std::byte a[0xe8];
    std::memset(a, 0xcc, sizeof a);
    axis_record_init(a);
    CHECK(at<double>(a, 0xb8) == 360.0);
    CHECK_EQ(at<std::uint64_t>(a, 0xe0), std::uint64_t{0});
    CHECK_EQ(at<std::int32_t>(a, 0x60), 0);
    // 140259950 numbering
    std::byte list[0x18] = {};
    std::int32_t* r1 = axis_list_add(list, 0);
    std::int32_t* r2 = axis_list_add(list, 7);
    CHECK_EQ(*r1, 1);
    CHECK_EQ(*r2, 7);
    CHECK(axis_list_at(list, 2) == reinterpret_cast<std::byte*>(r2));
    CHECK(axis_list_at(list, 0) == reinterpret_cast<std::byte*>(r1));
    ::operator delete(r1);
    ::operator delete(r2);
    // 140163300
    LLOBJ_DATA* o = raw_llobj();
    set_layer_count(o, 101);
    CHECK_EQ(at<std::int32_t>(o, 0xbd8), 100);
    // 140165170: 0 when +0x87c is 0
    at<std::int32_t>(o, 0x884) = 1;
    CHECK_EQ(node_array_size(o), 0);
    ::operator delete(o);
    // 140256dc0 / 140257150: 0 for n == 0
    CHECK(calloc_int(0) == nullptr);
    CHECK(calloc_double(0) == nullptr);
}

int main() { return testkit::run_all(); }
