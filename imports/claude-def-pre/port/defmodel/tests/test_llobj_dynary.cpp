// DYNARY index/get/set (140130530/720/800), the DYNARY list Cls_1401364a0 (1401364a0..140136910,
// 14013c1b0) and the DYNARY scalar deleting dtor 14013c170. Expected values are worked out from
// the decompiled code in the comments.
// ctest-labels: p1-04 llobj
#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstring>
#include <limits>
#include <new>

#include "llobj_recorder.hpp"

using namespace def::model;
using namespace def::model::llobj;
using llobj_test::Recorder;

namespace {

DYNARY* raw_dynary(int ndim, int n0, int n1, int n2, int ncomp, double* values)
{
    DYNARY* d = llobj_test::raw_object<DYNARY>();
    d->ndim = ndim;
    d->n[0] = n0;
    d->n[1] = n1;
    d->n[2] = n2;
    d->ncomp = ncomp;
    d->values = values;
    return d;
}

} // namespace

TEST_CASE(index_ndim0_is_invalid)
{
    DYNARY* d = raw_dynary(0, 3, 3, 3, 1, nullptr);
    CHECK_EQ(dynary_index(*d, 0, 0, 0, 0), -1);
    ::operator delete(d);
}

TEST_CASE(index_ndim1_is_i_times_ncomp_plus_c)
{
    DYNARY* d = raw_dynary(1, 3, 0, 0, 2, nullptr);
    CHECK_EQ(dynary_index(*d, 2, 0, 0, 1), 5);    // 2*2 + 1
    CHECK_EQ(dynary_index(*d, 0, 0, 0, 0), 0);
    CHECK_EQ(dynary_index(*d, 3, 0, 0, 0), -1);   // i >= n0
    CHECK_EQ(dynary_index(*d, -1, 0, 0, 0), -1);
    CHECK_EQ(dynary_index(*d, 0, 1, 0, 0), -1);   // n1 = 0 clamps to 1, so j must be 0
    CHECK_EQ(dynary_index(*d, 0, 0, 0, 2), -1);   // c >= ncomp
    // ncomp = 0: bounds use max(ncomp, 1), the index uses ncomp itself: i*0 + c
    d->ncomp = 0;
    CHECK_EQ(dynary_index(*d, 2, 0, 0, 0), 0);
    ::operator delete(d);
}

TEST_CASE(index_ndim2_and_3_column_major_with_component_last)
{
    DYNARY* d = raw_dynary(2, 3, 4, 0, 2, nullptr);
    // j*n0 + i + c*n0*n1*n2' + k*n0*n1 = 2*3 + 1 + 1*3*4*1 + 0 = 19
    CHECK_EQ(dynary_index(*d, 1, 2, 0, 1), 19);
    CHECK_EQ(dynary_index(*d, 1, 4, 0, 1), -1);
    d->ndim = 3;
    d->n[0] = 2;
    d->n[1] = 3;
    d->n[2] = 4;
    d->ncomp = 1;
    // 2*2 + 1 + 0 + 3*2*3 = 23
    CHECK_EQ(dynary_index(*d, 1, 2, 3, 0), 23);
    CHECK_EQ(dynary_index(*d, 1, 2, 4, 0), -1);
    ::operator delete(d);
}

TEST_CASE(get_returns_zero_without_values_or_index)
{
    double v[6] = {10, 11, 12, 13, 14, 15};
    DYNARY* d = raw_dynary(1, 3, 0, 0, 2, v);
    CHECK_BITS(dynary_get(*d, 2, 0, 0, 1), 15.0);
    CHECK_BITS(dynary_get(*d, 3, 0, 0, 1), 0.0);
    d->values = nullptr;
    CHECK_BITS(dynary_get(*d, 2, 0, 0, 1), 0.0);
    ::operator delete(d);
}

TEST_CASE(set_flushes_magnitudes_outside_1e_30_to_1e30)
{
    double v[2] = {7, 7};
    DYNARY* d = raw_dynary(1, 2, 0, 0, 1, v);
    dynary_set(*d, 1.5, 0, 0, 0, 0);
    CHECK_BITS(v[0], 1.5);
    dynary_set(*d, 2e-30, 1, 0, 0, 0);
    CHECK_BITS(v[1], 2e-30);
    const double flushed[] = {1e-31, -1e-30, 1e-30, 1e30, -1e30, 2e30, std::numeric_limits<double>::quiet_NaN(),
                              std::numeric_limits<double>::infinity(), 0.0};
    for (double x : flushed) {
        v[0] = 7;
        dynary_set(*d, x, 0, 0, 0, 0);
        CHECK_BITS(v[0], 0.0);
    }
    dynary_set(*d, -3.25, 0, 0, 0, 0);
    CHECK_BITS(v[0], -3.25);
    // out of range: nothing written
    dynary_set(*d, 9.0, 2, 0, 0, 0);
    CHECK_BITS(v[1], 2e-30);
    ::operator delete(d);
}

TEST_CASE(dynary_list_add_at_clear)
{
    Recorder rec;
    rec.result = [](const llobj_test::Call& c) -> std::uint64_t {
        if (c.addr == 0x14012d050)
            return llobj_test::u(llobj_test::raw_object<DYNARY>());  // operator new(0x78) + ctor
        return 0;
    };
    Cls_1401364a0 list;
    CHECK(list.head == nullptr);
    CHECK(list.tail == nullptr);
    CHECK_EQ(list.size(), 0);
    DYNARY* a = list.add();
    DYNARY* b = list.add();
    DYNARY* c = list.add();
    CHECK_EQ(list.size(), 3);
    CHECK(list.head == a);
    CHECK(list.tail == c);
    CHECK(dynary_next(a) == b);
    CHECK(dynary_next(b) == c);
    CHECK(dynary_next(c) == nullptr);
    const auto news = rec.only(0x14012d050);
    CHECK(news.size() == 3u);
    CHECK_EQ(news[0].a[0], 0x78u);
    // 1-based; n < 2 gives the head; past the end gives 0
    CHECK(list.at(0) == a);
    CHECK(list.at(1) == a);
    CHECK(list.at(2) == b);
    CHECK(list.at(3) == c);
    CHECK(list.at(4) == nullptr);
    rec.calls.clear();
    list.clear();  // 14013c170(p, 1) per node in order: dtor 14012f410, then free
    auto dt = rec.only(0x14012f410);
    REQUIRE(dt.size() == 3u);
    CHECK_EQ(dt[0].a[0], llobj_test::u(a));
    CHECK_EQ(dt[1].a[0], llobj_test::u(b));
    CHECK_EQ(dt[2].a[0], llobj_test::u(c));
    CHECK(list.head == nullptr);
    CHECK(list.tail == nullptr);
    CHECK_EQ(list.size(), 0);
}

TEST_CASE(int_accessors_read_0x10_and_0x1c)
{
    DYNARY* d = raw_dynary(1, 0, 0, 0, 1, nullptr);
    d->var[0] = 77;
    d->n[0] = 5;
    CHECK_EQ(int_at_10(d), 77);
    CHECK_EQ(int_at_1c(d), 5);
    ::operator delete(d);
}

int main() { return testkit::run_all(); }
