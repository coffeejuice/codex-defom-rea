// ctest-labels: mdt gui_twins
#define _CRT_SECURE_NO_WARNINGS
// Tier-1 tests of the model utilities the DEF_GUI_MO_64 loader calls as twins (defmodel/gui_twins.hpp):
// 1401cb400, 1401cabc0, 14017ac20, 1401d6df0, 140161770, 140160f50, 140188c90, 140165eb0,
// 140207f50, 14020a340, 140339ba0. Expected values are worked out by hand from the decompile
// (deform-rea-work/out/mdt3/f.c, g.c); the REFTMP defaults 20.0 / 68.0 / 1.0 are the binary's
// DAT_14051f1f0 / DAT_14051f1f8 / DAT_140509c90.
#include <defmodel/gui_twins.hpp>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <new>
#include <vector>

#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
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

// a hand-built object list: objects with the given numbers, linked through +0x3940
struct List {
    Cls_14017a6c0* l = zeroed<Cls_14017a6c0>();
    std::vector<LLOBJ_DATA*> objs;
    explicit List(std::vector<std::int32_t> numbers)
    {
        for (std::int32_t n : numbers) {
            LLOBJ_DATA* o = zeroed<LLOBJ_DATA>();
            at<std::int32_t>(o, 8) = n;
            if (l->head == nullptr) l->head = o; else l->tail->next = o;
            l->tail = o;
            ++l->count;
            objs.push_back(o);
        }
    }
};

} // namespace

TEST_CASE(step_clear_resets_for_the_same_dimension)
{
    DB_STEP_DATA m(3);
    REQUIRE(llobj_ctor::get_or_create(&m, 2) != nullptr);
    CHECK_EQ(m.u1378, 2);
    m.u744 = 7;
    m.u1b5d = 0;
    m.u1738 = 9;
    step_clear(m);
    CHECK_EQ(m.dimension, 3);  // 1401c8c00 called with the old +0x1c
    CHECK_EQ(m.u1378, 0);      // 1401cb440 / 1401c8c00
    CHECK(m.objects.head == nullptr);
    CHECK_EQ(m.objects.count, 0);
    CHECK_EQ(m.u744, -1);      // 1401c8c00: +0x744 = -1
    CHECK_EQ(m.u1b5d, 1);      // +0x1b5d = 1
    CHECK_EQ(m.u1738, 1);      // +0x1738 = 1
}

TEST_CASE(first_real_object_skips_missing_and_kind7)
{
    DB_STEP_DATA m(3);
    LLOBJ_DATA* o3 = llobj_ctor::get_or_create(&m, 3);  // 1, 2 become placeholders of kind 7
    REQUIRE(o3 != nullptr);
    at<std::int32_t>(o3, 0xc) = 7;
    CHECK_EQ(first_real_object_number(m), 0);  // all kind 7
    at<std::int32_t>(o3, 0xc) = 2;
    CHECK_EQ(first_real_object_number(m), 3);
    at<std::int32_t>(llobj_ctor::get_or_create(&m, 2), 0xc) = 0;
    CHECK_EQ(first_real_object_number(m), 2);
    m.u1378 = 1;  // only object 1 is looked at
    CHECK_EQ(first_real_object_number(m), 0);
}

TEST_CASE(first_free_and_last_object_numbers)
{
    List a({1, 2, 4});
    CHECK_EQ(first_free_object_number(a.l), 3);
    List b({2, 1, 3});
    CHECK_EQ(first_free_object_number(b.l), 4);
    List e({});
    CHECK_EQ(first_free_object_number(e.l), 1);

    DB_STEP_DATA* m = zeroed<DB_STEP_DATA>();
    std::memcpy(&m->objects, a.l, sizeof(Cls_14017a6c0));
    CHECK_EQ(last_object_number(*m), 0);  // u1378 < 1
    m->u1378 = 3;
    CHECK_EQ(last_object_number(*m), 4);  // the tail, not the head
    m->objects.tail = nullptr;
    CHECK_EQ(last_object_number(*m), 0);
}

TEST_CASE(copy_keep_number)
{
    DB_STEP_DATA m(3);
    LLOBJ_DATA* src = llobj_ctor::get_or_create(&m, 1);
    LLOBJ_DATA* dst = llobj_ctor::get_or_create(&m, 2);
    REQUIRE(src != nullptr && dst != nullptr);
    std::strcpy(src->name, "SRC OBJECT");
    at<std::int32_t>(src, 0xc) = 2;
    CHECK(llobj_copy_keep_number(dst, src));
    CHECK_EQ(at<std::int32_t>(dst, 8), 2);  // kept
    CHECK_EQ(at<std::int32_t>(dst, 0xc), 2);
    CHECK(std::strcmp(dst->name, "SRC OBJECT") == 0);  // p7 = 1: name copied

    LLOBJ_DATA* orphan = zeroed<LLOBJ_DATA>();  // owner (+0x3a98) null
    at<std::int32_t>(orphan, 8) = 5;
    CHECK(!llobj_copy_keep_number(orphan, src));
    CHECK_EQ(at<std::int32_t>(orphan, 0xc), 0);
}

TEST_CASE(reset_kind7_and_unit_defaults)
{
    LLOBJ_DATA* o = zeroed<LLOBJ_DATA>();
    at<std::int32_t>(o, 0xc) = 7;
    llobj_reset_kind7(o, 1);
    CHECK_EQ(at<std::int32_t>(o, 0xc), 0);
    CHECK_EQ(at<double>(o, 0x1eb8), 20.0);
    CHECK_EQ(at<double>(o, 0x1ec0), 1.0);
    at<std::int32_t>(o, 0xc) = 3;
    llobj_reset_kind7(o, 2);
    CHECK_EQ(at<std::int32_t>(o, 0xc), 3);
    CHECK_EQ(at<double>(o, 0x1eb8), 68.0);
}

TEST_CASE(cls_1401889a0_assign_copies_the_members_only)
{
    unsigned char src[0x128], dst[0x128];
    for (int i = 0; i < 0x128; ++i) {
        src[i] = static_cast<unsigned char>(i ^ 0x5a);
        dst[i] = 0xee;
    }
    cls_1401889a0_assign(dst, src);
    // members from the decompile: [off, size)
    const int members[][2] = {{0x00, 4}, {0x04, 4}, {0x08, 8}, {0x10, 8}, {0x18, 8}, {0x20, 4},
                              {0x28, 8}, {0x30, 8}, {0x38, 1}, {0x39, 1}, {0x3c, 4}, {0x40, 4},
                              {0x44, 4}, {0x4c, 1}, {0x50, 4}, {0x58, 40}, {0xa0, 48}, {0xf0, 48}};
    bool copied[0x128] = {};
    for (const auto& mb : members)
        for (int i = 0; i < mb[1]; ++i) copied[mb[0] + i] = true;
    int bad = 0;
    for (int i = 0; i < 0x128; ++i)
        if (dst[i] != (copied[i] ? src[i] : 0xee)) ++bad;
    CHECK_EQ(bad, 0);
    CHECK_EQ(dst[0x24], 0xee);  // gaps stay
    CHECK_EQ(dst[0x80], 0xee);
    CHECK_EQ(dst[0x120], 0xee);
}

namespace {
// object with a BC array +0x990 (rows of w ints) and +0x998, node count +0x880, object dim +0x39c0
struct BcObj {
    LLOBJ_DATA* o = zeroed<LLOBJ_DATA>();
    DB_STEP_DATA* owner = zeroed<DB_STEP_DATA>();
    std::vector<std::int32_t> a, b;
    BcObj(int dim, std::vector<std::int32_t> codes, std::vector<std::int32_t> fn, int nodes)
        : a(std::move(codes)), b(std::move(fn))
    {
        owner->dimension = dim;
        at<DB_STEP_DATA*>(o, 15000) = owner;
        at<std::int32_t*>(o, 0x990) = a.data();
        at<std::int32_t*>(o, 0x998) = b.data();
        at<std::int32_t>(o, 0x880) = nodes;
        at<std::int32_t>(o, 0x39c0) = dim;
    }
};
} // namespace

TEST_CASE(node_bc_code_cases)
{
    std::int32_t tbl[0x4f] = {};
    tbl[0x12] = 3;  // +0x48 row width
    BcObj x(3, {0, -5, -7, /**/ -205, 0, 0, /**/ -150, 0, 0, /**/ 1, 2, 3, /**/ 4, -1, 0}, {}, 5);
    CHECK_EQ(node_bc_code(x.o, 0, tbl), 0);  // node < 1
    CHECK_EQ(node_bc_code(x.o, 1, tbl), 5);  // first negative: -5
    CHECK_EQ(node_bc_code(x.o, 2, tbl), 5);  // 205 - 200
    CHECK_EQ(node_bc_code(x.o, 3, tbl), 0);  // 150 > 100
    CHECK_EQ(node_bc_code(x.o, 4, tbl), 0);  // no negative entry
    CHECK_EQ(node_bc_code(x.o, 5, tbl), 1);
}

TEST_CASE(bcc_clear_nodes_modes)
{
    // code < 0: entries equal to code in the first min(w_a, w_b) components
    BcObj x(3, {-5, -5, -5, /**/ -5, 1, -6}, {9, 9, 9, 9, 9, 9}, 2);
    const std::int32_t nodes[] = {2, 1};
    std::int32_t wa = 3, wb = 2;
    bcc_clear_nodes(x.o, nodes, 2, x.a.data(), x.b.data(), nullptr, -5, &wa, &wb, 0, 0);
    CHECK(x.a == (std::vector<std::int32_t>{0, 0, -5, /**/ 0, 1, -6}));
    // code 1: component `comp` when it is 1; b indexed with w_b, c with w_a
    BcObj y(3, {1, 1, 0, 1}, {7, 7, 7, 7}, 2);
    double c[4] = {1.5, 2.5, 3.5, 4.5};
    std::int32_t w2 = 2, w1 = 1;
    const std::int32_t n2[] = {2};
    bcc_clear_nodes(y.o, n2, 1, y.a.data(), y.b.data(), c, 1, &w2, &w1, 1, 0);
    CHECK(y.a == (std::vector<std::int32_t>{1, 1, 0, 0}));  // a[(2-1)*2+1]
    CHECK(y.b == (std::vector<std::int32_t>{7, 7, 0, 7}));  // b[(2-1)*1+1]
    CHECK_EQ(c[3], 0.0);
    // code 1 with comp -1 and value >= 0: returns before the first node
    bcc_clear_nodes(y.o, n2, 1, y.a.data(), y.b.data(), c, 1, &w2, &w1, -1, 0);
    // code 0: |c| > 1e-12 cleared; a == 0 and b != 0: b cleared and a indexed with w_b
    BcObj z(3, {5, 5, 0, 5}, {0, 0, 0, 8}, 2);
    double cz[4] = {0, 0, 1e-13, -2.0};
    std::int32_t wa2 = 2, wb1 = 1;
    const std::int32_t n2b[] = {2};
    bcc_clear_nodes(z.o, n2b, 1, z.a.data(), z.b.data(), cz, 0, &wa2, &wb1, 0, 0);
    // idx_a = (2-1)*2+0 = 2: a[2] == 0, b[(2-1)*1+0] = b[1] == 0 -> nothing; c[2] = 1e-13 kept
    CHECK_EQ(cz[2], 1e-13);
    CHECK(z.b == (std::vector<std::int32_t>{0, 0, 0, 8}));
    bcc_clear_nodes(z.o, n2b, 1, z.a.data(), z.b.data(), cz, 0, &wa2, &wb1, 1, 0);
    // comp 1: idx_a = 3 (a = 5), c[3] = -2.0 -> 0
    CHECK_EQ(cz[3], 0.0);
    // 3D owner, code 5: first component with a == 5 and b == value; b indexed with w_a
    BcObj v(3, {5, 5, 5, 5}, {1, 2, 3, 4}, 2);
    std::int32_t wv = 2;
    const std::int32_t n1[] = {2};
    bcc_clear_nodes(v.o, n1, 1, v.a.data(), v.b.data(), nullptr, 5, &wv, &wv, 0, 4);
    CHECK(v.a == (std::vector<std::int32_t>{5, 5, 5, 0}));
    CHECK(v.b == (std::vector<std::int32_t>{1, 2, 3, 0}));
    // the same with a 2D owner: code 5 has no case, nothing changes
    BcObj w(2, {5, 5, 5, 5}, {1, 2, 3, 4}, 2);
    bcc_clear_nodes(w.o, n1, 1, w.a.data(), w.b.data(), nullptr, 5, &wv, &wv, 0, 4);
    CHECK(w.a == (std::vector<std::int32_t>{5, 5, 5, 5}));
}

TEST_CASE(bcc_groups_3d_and_2d)
{
    std::int32_t tbl[0x4f] = {};
    tbl[0x12] = 3;  // ncomp
    tbl[0x13] = 2;  // +0x4c: 140207f50 looks at min(3, 2) components
    const std::vector<std::int32_t> in = {0, -5, -7, /**/ -205, 0, -5, /**/ -150, -3, 0, /**/ -7, -7, 0};
    // 3D object: groups {5: nodes 1, 2}, {7: node 4}; node 3 has code 0 (150) and is not grouped.
    // Group 5 clears entries == -5 in components 0..1; group 7 clears -7 in components 0..1.
    BcObj x(3, in, std::vector<std::int32_t>(12, 0), 4);
    bcc_groups_from_model(x.o, tbl);
    CHECK(x.a == (std::vector<std::int32_t>{0, 0, -7, /**/ -205, 0, -5, /**/ -150, -3, 0, /**/ 0, 0, 0}));
    // 2D object: every negative entry of the grouped nodes (1, 2, 4) becomes 0, all components
    BcObj y(2, in, std::vector<std::int32_t>(12, 0), 4);
    bcc_groups_from_model(y.o, tbl);
    CHECK(y.a == (std::vector<std::int32_t>{0, 0, 0, /**/ 0, 0, 0, /**/ -150, -3, 0, /**/ 0, 0, 0}));
    // null table: nothing
    BcObj z(2, in, std::vector<std::int32_t>(12, 0), 4);
    bcc_groups_from_model(z.o, nullptr);
    CHECK(z.a == in);
}

TEST_CASE(phase_pair_in_mixture_cases)
{
    Material* mat = zeroed<Material>();
    PhasePair* p = zeroed<PhasePair>();
    std::int32_t list[] = {3, 4, 6};
    mat->mstmtr_list = list;
    mat->mstmtr_count = 3;
    p->from = 4;
    p->to = 6;
    p->phastf_on = 1;
    CHECK(!phase_pair_in_mixture(p, mat));  // not a mixture
    mat->mstmtr_flag = 1;
    CHECK(phase_pair_in_mixture(p, mat));
    p->to = 5;
    CHECK(!phase_pair_in_mixture(p, mat));
    p->to = 3;
    p->phastf_on = 0;
    CHECK(!phase_pair_in_mixture(p, mat));
}

int main() { return testkit::run_all(); }
