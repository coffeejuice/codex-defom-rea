// Element and node state-variable sets Cls_14017ef20 (14017ef20..140180390, 140182840, 14019a370)
// and Cls_140182880 (140182880..140185530, 14019a3f0, 14019a450): construction registers the
// variables in the decompile's order, alloc/copy sizes, the skip lists of the copies, 140183c20.
// ctest-labels: p1-04 llobj
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>
#include <vector>

#include "llobj_recorder.hpp"

using namespace def::model;
using llobj_test::Call;
using llobj_test::Recorder;
using llobj_test::u;

namespace {

std::vector<std::int32_t> skip_ids(const Call& c)
{
    const auto* l = reinterpret_cast<const CowList<std::int32_t>*>(c.a[2]);
    return {l->items().begin(), l->items().end()};
}

} // namespace

TEST_CASE(elem_vars_ctor_registers_28_variables_in_order)
{
    Recorder rec;
    Cls_14017ef20 s;
    // base ctor, delete-old (140329d10), then 28 x (operator new + ctor, append 140329e30)
    REQUIRE(rec.calls.size() == 2u + 28u * 2u);
    CHECK_EQ(rec.calls[0].addr, 0x140329c70u);
    CHECK_EQ(rec.calls[0].a[0], u(&s));
    CHECK_EQ(rec.calls[1].addr, 0x140329d10u);
    const std::int32_t ids[28] = {1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15,
                                  16, 0x18, 0x19, 0x1b, 0x1c, 0x1a, 0x1d, 0x22, 0x23, 0x11, 0x1e, 0x1f, 0x20, 0x21};
    for (int k = 0; k < 28; ++k) {
        const Call& c = rec.calls[2 + 2 * k];
        CHECK_EQ(c.addr, k < 24 ? 0x14032a990u : 0x14032aa20u);
        CHECK_EQ(c.a[0], 0x38u);
        CHECK_EQ(static_cast<std::int32_t>(c.a[1]), ids[k]);
        CHECK_EQ(c.a[2], u(&s.u010[k]));
        CHECK_EQ(rec.calls[3 + 2 * k].addr, 0x140329e30u);
    }
    CHECK(s.u008 == nullptr);
    CHECK_EQ(s.u128, 300);
    CHECK_EQ(s.count(), 0);
    CHECK(s.u130 == nullptr);
    CHECK_EQ(s.u138, 3);
    CHECK_EQ(s.u13c, 0);
    s.set_u138(7);
    CHECK_EQ(s.u138, 7);
    rec.calls.clear();
}

TEST_CASE(elem_vars_alloc_copy_free)
{
    Recorder rec;
    Cls_14017ef20 a;
    Cls_14017ef20 b;
    std::int32_t dims[0x18] = {};
    dims[0] = 2;
    for (int k = 0; k < 6; ++k) dims[0x12 + k] = k + 1;
    b.alloc(3, dims);
    CHECK_EQ(b.count(), 3);
    for (int e = 0; e < 6; ++e) b.u008[e] = 0.5 * e;
    for (int k = 0; k < 6; ++k)
        for (int e = 0; e < 3 * (k + 1); ++e) b.u0f0[k][e] = 100 * k + e;
    auto sized = rec.only(0x140329e60);
    REQUIRE(sized.size() == 1u);
    CHECK_EQ(sized[0].a[1], 3u);
    CHECK_EQ(sized[0].a[2], u(dims));
    CHECK_EQ(sized[0].a[3], 0xffffffffu);
    CHECK(b.u0f0[6] == nullptr);  // 0x120 is not allocated by 140180150

    rec.calls.clear();
    std::vector<std::vector<std::int32_t>> skips;
    rec.result = [&](const Call& c) -> std::uint64_t {
        if (c.addr == 0x140329ef0) skips.push_back(skip_ids(c));
        return 1;
    };
    a.copy_from(b, 3, dims, false);
    a.copy_from(b, 3, dims, true);
    REQUIRE(skips.size() == 2u);
    CHECK(skips[0] == std::vector<std::int32_t>{0x11});
    CHECK(skips[1].empty());
    CHECK_EQ(a.count(), 3);
    CHECK(std::memcmp(a.u008, b.u008, 6 * sizeof(double)) == 0);
    for (int k = 0; k < 6; ++k) CHECK(std::memcmp(a.u0f0[k], b.u0f0[k], 3 * (k + 1) * sizeof(std::int32_t)) == 0);

    rec.calls.clear();
    a.copy_from(b, 0, dims, false);  // n == 0: free only
    CHECK(a.u008 == nullptr);
    CHECK_EQ(a.count(), 0);
    CHECK(rec.only(0x140329dc0).size() == 1u);
    CHECK(rec.only(0x140329ef0).empty());
    rec.calls.clear();
}

TEST_CASE(elem_vars_dtor_frees_then_base_dtor)
{
    Recorder rec;
    {
        Cls_14017ef20 s;
        rec.calls.clear();
    }
    REQUIRE(rec.calls.size() == 2u);
    CHECK_EQ(rec.calls[0].addr, 0x140329dc0u);
    CHECK_EQ(rec.calls[1].addr, 0x140329cb0u);
}

TEST_CASE(node_vars_ctor_registers_25_variables_in_order)
{
    Recorder rec;
    Cls_140182880 s;
    REQUIRE(rec.calls.size() == 2u + 25u * 2u);
    CHECK_EQ(rec.calls[0].addr, 0x140329c70u);
    CHECK_EQ(rec.calls[1].addr, 0x140329d10u);
    struct E { std::uint64_t ctor; std::uint64_t size; std::int32_t id; bool owner; };
    const E want[25] = {
        {0x14032c950, 0x40, 0x26, true},  {0x14032c8c0, 0x38, 0x27, false}, {0x14032c8c0, 0x38, 0x28, false},
        {0x14032c8c0, 0x38, 0x29, false}, {0x14032c8c0, 0x38, 0x2a, false}, {0x14032ca80, 0x40, 0x2b, true},
        {0x14032c8c0, 0x38, 0x2c, false}, {0x14032cbb0, 0x40, 0x2d, true},  {0x14032ccd0, 0x38, 0x2e, false},
        {0x14032c8c0, 0x38, 0x2f, false}, {0x14032c8c0, 0x38, 0x30, false}, {0x14032c8c0, 0x38, 0x31, false},
        {0x14032c8c0, 0x38, 0x32, false}, {0x14032c8c0, 0x38, 0x33, false}, {0x14032c8c0, 0x38, 0x34, false},
        {0x14032c8c0, 0x38, 0x35, false}, {0x14032c8c0, 0x38, 0x36, false}, {0x14032c8c0, 0x38, 0x37, false},
        {0x14032c8c0, 0x38, 0x38, false}, {0x14032c8c0, 0x38, 0x39, false}, {0x14032c8c0, 0x38, 0x3a, false},
        {0x14032c8c0, 0x38, 0x3b, false}, {0x14032ceb0, 0x40, 0x3e, true},  {0x14032c8c0, 0x38, 0x3f, false},
        {0x14032c8c0, 0x38, 0x3c, false},
    };
    for (int k = 0; k < 25; ++k) {
        const Call& c = rec.calls[2 + 2 * k];
        CHECK_EQ(c.addr, want[k].ctor);
        CHECK_EQ(c.a[0], want[k].size);
        CHECK_EQ(static_cast<std::int32_t>(c.a[1]), want[k].id);
        CHECK_EQ(c.a[2], u(&s.u020[k]));
        CHECK_EQ(c.a[3], want[k].owner ? u(&s) : 0u);
        CHECK_EQ(rec.calls[3 + 2 * k].addr, 0x140329e30u);
    }
    CHECK(s.u0f0 == nullptr);
    CHECK_EQ(s.u0f8, 3);
    CHECK(s.u0f8_is_3());
    s.set_u0f8(2);
    CHECK(!s.u0f8_is_3());
    CHECK_EQ(s.count(), 0);
    CHECK_EQ(s.width(), 0);
    rec.calls.clear();
}

TEST_CASE(node_vars_alloc_copy_and_skip_order)
{
    Recorder rec;
    Cls_140182880 a;
    Cls_140182880 b;
    std::byte src[0x100] = {};
    const std::int32_t per_node = 3;
    std::memcpy(src + 0xf4, &per_node, 4);
    b.alloc(2, src, 4);
    CHECK_EQ(b.count(), 2);
    CHECK_EQ(b.width(), 4);
    for (int e = 0; e < 8; ++e) b.u008[e] = e + 1;
    for (int e = 0; e < 6; ++e) b.u018[e] = 10 * e;

    std::vector<std::vector<std::int32_t>> skips;
    rec.result = [&](const Call& c) -> std::uint64_t {
        if (c.addr == 0x140329ef0) skips.push_back(skip_ids(c));
        return 1;
    };
    a.copy_from(b, 2, 4, src, false, false, false);
    a.copy_from(b, 2, 4, src, true, true, true);
    a.copy_from(b, 2, 4, src, true, false, true);
    REQUIRE(skips.size() == 3u);
    // order of 140183900: 0x3c (b6), 0x28 (b8), then 0x31..0x34 (b7)
    CHECK(skips[0] == (std::vector<std::int32_t>{0x3c, 0x28, 0x31, 0x32, 0x33, 0x34}));
    CHECK(skips[1].empty());
    CHECK(skips[2] == (std::vector<std::int32_t>{0x31, 0x32, 0x33, 0x34}));
    CHECK(std::memcmp(a.u008, b.u008, 8 * 4) == 0);
    CHECK(std::memcmp(a.u018, b.u018, 6 * 4) == 0);
    CHECK_EQ(a.width(), 4);
    rec.calls.clear();
}

TEST_CASE(node_vars_fun_140183c20)
{
    Recorder rec;
    Cls_140182880 s;
    CHECK_EQ(s.fun_140183c20(0), 1);  // no owner
    std::vector<std::byte> owner(0x3f28);
    s.u0f0 = reinterpret_cast<LLOBJ_DATA*>(owner.data());
    const std::int32_t three = 3, two = 2;
    std::memcpy(owner.data() + 0x888 + 4 * 2, &three, 4);
    std::memcpy(owner.data() + 0x888 + 4 * 1, &two, 4);
    CHECK_EQ(s.fun_140183c20(1), 1);  // owner value != 3
    s.u0f8 = 3;
    s.m = 8;
    CHECK_EQ(s.fun_140183c20(2), 8);
    s.m = 4;
    CHECK_EQ(s.fun_140183c20(2), 1);
    s.u0f8 = 2;
    CHECK_EQ(s.fun_140183c20(2), 4);
    s.u0f0 = nullptr;
    s.u0f8 = 3;
    rec.calls.clear();
}

int main() { return testkit::run_all(); }
