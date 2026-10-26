// LLOBJ_DATA sub-objects: Cls_140185570, Cls_14017e740, Cls_140197a50, and the reset/dtor call
// order of Cls_1401860a0, Cls_14018b220, Cls_140189440, Cls_1401956d0 (their ctors are Phase 2,
// so these run on zeroed storage); Cls_140185c60 and Cls_1401889a0 dtors.
// ctest-labels: p1-04 llobj
#include <defmodel/layout/dynary.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/llobj/cls_140185570.hpp>
#include <defmodel/llobj/cls_140185c60.hpp>
#include <defmodel/llobj/cls_1401860a0.hpp>
#include <defmodel/llobj/cls_1401889a0.hpp>
#include <defmodel/llobj/cls_140189440.hpp>
#include <defmodel/llobj/cls_14018b220.hpp>
#include <defmodel/llobj/cls_1401956d0.hpp>
#include <defmodel/llobj/cls_140197a50.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>
#include <vector>

#include "llobj_recorder.hpp"

using namespace def::model;
using llobj_test::Call;
using llobj_test::raw_object;
using llobj_test::Recorder;
using llobj_test::u;

namespace {

// (address, offset of a[0] from base) for every recorded call
std::vector<std::pair<std::uint64_t, std::uint64_t>> trace(const Recorder& r, const void* base)
{
    std::vector<std::pair<std::uint64_t, std::uint64_t>> v;
    for (const Call& c : r.calls) v.emplace_back(c.addr, c.a[0] - u(base));
    return v;
}

using Trace = std::vector<std::pair<std::uint64_t, std::uint64_t>>;

} // namespace

TEST_CASE(arrays_0x70_ctor_and_free)
{
    Cls_140185570 s;
    CHECK(s.u060 == nullptr);
    CHECK_EQ(s.u068, 3);
    for (void* p : s.arrays) CHECK(p == nullptr);
    s.arrays[0] = std::malloc(8);
    s.arrays[11] = std::malloc(8);
    s.free_arrays();
    for (void* p : s.arrays) CHECK(p == nullptr);
    s.set_u068(5);
    CHECK_EQ(s.u068, 5);
}

TEST_CASE(element_0x30_ctor_owner_free)
{
    Cls_14017e740 e;
    CHECK(e.u028 == nullptr);
    CHECK_EQ(e.u000, 0);
    e.u000 = 4;
    e.u010 = 2;
    e.u008 = std::malloc(16);
    e.u018 = std::malloc(16);
    e.u020 = std::malloc(16);
    int owner = 0;
    e.free_arrays();
    CHECK(e.u008 == nullptr);
    CHECK(e.u018 == nullptr);
    CHECK(e.u020 == nullptr);
    CHECK_EQ(e.u000, 0);
    CHECK_EQ(e.u010, 0);
    e.u000 = 9;
    e.set_owner(&owner);
    CHECK(e.u028 == &owner);
    CHECK_EQ(e.u000, 0);
}

TEST_CASE(arrays_0x20_free_only_when_count_positive)
{
    auto* s = raw_object<Cls_140197a50>();
    s->u08 = std::malloc(4);
    s->u10 = std::malloc(4);
    s->u18 = std::malloc(4);
    s->count = 0;
    s->free_arrays();  // nothing: count is not > 0
    CHECK(s->u08 != nullptr);
    s->count = 2;
    s->free_arrays();  // frees, keeps the pointers, count = 0
    CHECK_EQ(s->count, 0);
    s->free_arrays();  // count 0 again: no double free
    ::operator delete(s);
}

TEST_CASE(sub_1ea0_reset_and_dtor_order)
{
    Recorder rec;
    auto* s = raw_object<Cls_1401860a0>();
    std::int32_t nc = 7;
    std::memcpy(reinterpret_cast<std::byte*>(s) + 0x278, &nc, 4);
    s->~Cls_1401860a0();
    const Trace want = {{0x14012f480, 0xc0}, {0x14012f480, 0x140}, {0x140259c70, 0x200}, {0x14025ab20, 0x218},
                        {0x14012f480, 0x238}, {0x14012f410, 0x238}, {0x14025ae70, 0x218}, {0x1402598a0, 0x200},
                        {0x14012f410, 0x140}, {0x14012f410, 0xc0}};
    CHECK(trace(rec, s) == want);
    std::memcpy(&nc, reinterpret_cast<std::byte*>(s) + 0x278, 4);
    CHECK_EQ(nc, 0);
    ::operator delete(s);
}

TEST_CASE(sub_2160_reset_and_dtor_order)
{
    Recorder rec;
    auto* s = raw_object<Cls_14018b220>();
    s->raw[0x8b8] = std::byte{1};
    s->~Cls_14018b220();
    Trace want;
    for (std::uint64_t o : {0x0e8, 0x070, 0x208, 0x2a0, 0x3b0, 0x448, 0x660, 0x6d8, 0x8c0, 0x968, 0x9e0, 0x790, 0x808})
        want.emplace_back(0x14012f480, o);
    for (std::uint64_t o : {0x9e0, 0x968, 0x8c0, 0x808, 0x790, 0x6d8, 0x660, 0x590, 0x518, 0x448, 0x3b0, 0x338,
                            0x2a0, 0x208, 0x0e8, 0x070})
        want.emplace_back(0x14012f410, o);
    CHECK(trace(rec, s) == want);
    CHECK(s->raw[0x8b8] == std::byte{0});
    ::operator delete(s);
}

TEST_CASE(sub_2d10_reset_and_dtor_order)
{
    Recorder rec;
    auto* s = raw_object<Cls_140189440>();
    s->u0d8 = 5;
    s->u0e0 = std::malloc(8);
    s->u0e8 = std::malloc(8);
    s->~Cls_140189440();
    const Trace want = {{0x1402699c0, 0x40}, {0x140269320, 0x128}, {0x140269320, 0xf8}, {0x140269320, 0x40}};
    CHECK(trace(rec, s) == want);
    CHECK(s->u0e0 == nullptr);
    CHECK(s->u0e8 == nullptr);
    CHECK_EQ(s->u0d8, 0);
    ::operator delete(s);
}

TEST_CASE(sub_2e68_reset_and_dtor_order)
{
    Recorder rec;
    auto* s = raw_object<Cls_1401956d0>();
    s->~Cls_1401956d0();
    Trace want = {{0x1402581b0, 0x18}};
    for (std::uint64_t o : {0x48, 0x78, 0xa8, 0x168, 0xd8, 0x108, 0x138}) want.emplace_back(0x1402699c0, o);
    for (std::uint64_t o : {0x168, 0x138, 0x108, 0xd8, 0xa8, 0x78, 0x48}) want.emplace_back(0x140269320, o);
    want.emplace_back(0x140257c60, 0x30);
    want.emplace_back(0x140257c60, 0x18);
    CHECK(trace(rec, s) == want);
    ::operator delete(s);
}

TEST_CASE(sub_b58_dtor_calls_base_dtor_and_2be8_dtor_is_empty)
{
    Recorder rec;
    auto* s = raw_object<Cls_140185c60>();
    s->~Cls_140185c60();
    CHECK(trace(rec, s) == (Trace{{0x140329cb0, 0}}));
    ::operator delete(s);
    rec.calls.clear();
    auto* e = raw_object<Cls_1401889a0>();
    e->~Cls_1401889a0();
    CHECK(rec.calls.empty());
    ::operator delete(e);
}

int main() { return testkit::run_all(); }
