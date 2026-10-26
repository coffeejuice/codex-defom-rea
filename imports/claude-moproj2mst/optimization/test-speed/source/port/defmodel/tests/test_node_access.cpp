// ctest-labels: p67-3 node_access
// Node-set accessors (defmodel/node_access.hpp): 14017f020, 1401808d0, 1401809c0, 1401d48d0.
// The set is a real Cls_14017ef20 built under the p1-04 recorder (its state variables and the
// 140329e60 sizing call go to the recorder); the owner model is zeroed storage (only the table
// at +0x15f8, the byte +0x1b58 and the dimension +0x1c could be read). Expectations by hand from
// the decompiles: 14017ffe0 allocates set+8 = n * dims[0] doubles for key 0 when it is null,
// leaves it for key 2, never touches the int arrays or n for keys 0 / 2, and always passes
// (n, dims, key) to 140329e60 when n > 0; 1401d48d0 returns at once (1401d2b70 == 1).
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/node_access.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <cstdlib>
#include <vector>

#include "llobj_recorder.hpp"

using namespace def::model;
using llobj_test::Recorder;
using llobj_test::u;

namespace {

// Zeroed model memory (sizeof covers +0x15f8 .. +0x1b64).
struct Model {
    std::vector<std::uint64_t> mem = std::vector<std::uint64_t>(sizeof(DB_STEP_DATA) / 8 + 1, 0);
    DB_STEP_DATA* get() { return reinterpret_cast<DB_STEP_DATA*>(mem.data()); }
};

}  // namespace

TEST_CASE(dims_table_is_owner_plus_15f8_or_null)
{
    Recorder rec;
    Cls_14017ef20 s;
    CHECK(fun_14017f020(&s) == nullptr);
    Model m;
    s.u130 = m.get();
    CHECK_EQ(u(fun_14017f020(&s)), u(m.mem.data()) + 0x15f8u);
    s.u130 = nullptr;
}

TEST_CASE(coordinates_allocated_once_with_key_0)
{
    Recorder rec;
    Cls_14017ef20 s;
    Model m;
    std::int32_t* dims = m.get()->u15f8;
    dims[0] = 2;     // 2 coordinates per node (2D)
    dims[2] = 3;     // width of id 2, only for the 140329e60 argument check
    s.u130 = m.get();
    s.n = 5;
    rec.calls.clear();

    double* x = fun_1401808d0(&s);
    REQUIRE(x != nullptr);
    CHECK(x == s.u008);
    for (int i = 0; i < 10; ++i) CHECK_EQ(x[i], 0.0);  // calloc(5 * 2, 8)
    x[9] = 1.5;
    auto sized = rec.only(0x140329e60);
    REQUIRE(sized.size() == 1u);
    CHECK_EQ(sized[0].a[0], u(&s));
    CHECK_EQ(sized[0].a[1], 5u);
    CHECK_EQ(sized[0].a[2], u(dims));
    CHECK_EQ(sized[0].a[3], 0u);
    for (int k = 0; k < 7; ++k) CHECK(s.u0f0[k] == nullptr);  // key 0: no int arrays
    CHECK_EQ(s.n, 5);

    // second call: the existing array is kept (14017ffe0 tests set+8 != 0)
    double* y = fun_1401808d0(&s);
    CHECK(y == x);
    CHECK_EQ(y[9], 1.5);
    CHECK_EQ(rec.only(0x140329e60).size(), 2u);

    std::free(s.u008);
    s.u008 = nullptr;
    s.u130 = nullptr;
}

TEST_CASE(no_owner_and_no_nodes)
{
    Recorder rec;
    Cls_14017ef20 s;
    rec.calls.clear();
    // n == 0: 14017ffe0 does nothing, 1401d48d0 is skipped (no owner), set+8 stays null
    CHECK(fun_1401808d0(&s) == nullptr);
    CHECK(rec.only(0x140329e60).empty());
    CHECK(fun_1401809c0(&s) == nullptr);
    CHECK(rec.calls.empty());
}

TEST_CASE(urz_resizes_id_2_and_returns_slot_18)
{
    Recorder rec;
    Cls_14017ef20 s;
    Model m;
    std::int32_t* dims = m.get()->u15f8;
    dims[0] = 3;
    dims[2] = 2;
    s.u130 = m.get();
    s.n = 4;
    double data[8] = {};
    s.u010[1] = data;  // set+0x18, the slot of id 2 (14017f2f0 registers id 2 there)
    rec.calls.clear();

    CHECK(fun_1401809c0(&s) == data);
    CHECK(s.u008 == nullptr);  // key 2: coordinates not allocated
    auto sized = rec.only(0x140329e60);
    REQUIRE(sized.size() == 1u);
    CHECK_EQ(sized[0].a[1], 4u);
    CHECK_EQ(sized[0].a[2], u(dims));
    CHECK_EQ(sized[0].a[3], 2u);
    CHECK_EQ(rec.calls.size(), 1u);  // nothing else (1401d48d0 makes no calls)
    s.u010[1] = nullptr;
    s.u130 = nullptr;
}

TEST_CASE(layout_sync_changes_nothing)
{
    Model m;
    DB_STEP_DATA* p = m.get();
    p->u1b58 = 0;  // either value of +0x1b58: 1401d2b70 returns 1
    p->step_version = 20000;
    std::vector<std::uint64_t> before = m.mem;
    for (std::int32_t w : {0, 2, -1, 0x11, 0x3c, 0x22}) {
        fun_1401d48d0(p, &w);
        CHECK(m.mem == before);
    }
    p->u1b58 = 1;
    before = m.mem;
    std::int32_t all = -1;
    fun_1401d48d0(p, &all);
    CHECK(m.mem == before);
}

int main() { return testkit::run_all(); }
