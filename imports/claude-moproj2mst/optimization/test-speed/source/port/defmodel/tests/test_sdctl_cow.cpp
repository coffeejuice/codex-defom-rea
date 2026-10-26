// Copy-on-write handles (defmodel/sdctl/cow.hpp): sharing, detach, resize/insert/erase and the
// growth policy of _Insert_n (14019bd10 / 14015e550), checked against the decompiled code.
// ctest-labels: p1-05 sdctl
#include <defmodel/sdctl/cow.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <vector>

using def::model::CowList;
using def::model::CowVector;

namespace {

template <class T>
std::vector<T> items(const CowVector<T>& v)
{
    return std::vector<T>(v.begin(), v.end());
}

std::size_t capacity(const CowVector<std::int32_t>& v)
{
    return static_cast<std::size_t>(v.body()->end - v.body()->first);
}

} // namespace

TEST_CASE(cow_vector_ctor_is_empty_with_one_reference)
{
    CowVector<double> v;  // 14019af90: refs = 1, three null pointers
    CHECK_EQ(v.use_count(), 1);
    CHECK_EQ(v.size(), std::size_t{0});
    CHECK(v.body()->first == nullptr && v.body()->last == nullptr && v.body()->end == nullptr);
}

TEST_CASE(cow_vector_copy_shares_and_write_detaches)
{
    CowVector<std::int32_t> a;
    a.resize(3, 7);
    CowVector<std::int32_t> b(a);
    CHECK_EQ(a.use_count(), 2);
    CHECK(a.body() == b.body());
    b.mutable_at(1) = 9;  // 140005ae0: refs > 1 -> refs--, clone
    CHECK(a.body() != b.body());
    CHECK_EQ(a.use_count(), 1);
    CHECK_EQ(b.use_count(), 1);
    CHECK(items(a) == (std::vector<std::int32_t>{7, 7, 7}));
    CHECK(items(b) == (std::vector<std::int32_t>{7, 9, 7}));
    // clone capacity = size exactly (1400033f0 / 140002ba0)
    CHECK_EQ(capacity(b), std::size_t{3});
}

TEST_CASE(cow_vector_assign_releases_old_body)
{
    CowVector<std::int32_t> a, b;
    a.resize(2, 1);
    b = a;  // 1402f09f0
    CHECK(a.body() == b.body());
    CHECK_EQ(a.use_count(), 2);
    b = b;  // self assignment: refs++ then release keeps the body
    CHECK_EQ(a.use_count(), 2);
}

TEST_CASE(cow_vector_growth_policy)
{
    // _Insert_n: when end - last < n, newcap = size + max(n, size)
    CowVector<std::int32_t> v;
    v.resize(1, 5);                     // 0 + max(1, 0) = 1
    CHECK_EQ(capacity(v), std::size_t{1});
    v.resize(2, 6);                     // 1 + max(1, 1) = 2
    CHECK_EQ(capacity(v), std::size_t{2});
    v.resize(3, 7);                     // 2 + max(1, 2) = 4
    CHECK_EQ(capacity(v), std::size_t{4});
    v.resize(4, 8);                     // fits
    CHECK_EQ(capacity(v), std::size_t{4});
    v.resize(14, 9);                    // 4 + max(10, 4) = 14
    CHECK_EQ(capacity(v), std::size_t{14});
    CHECK(items(v) == (std::vector<std::int32_t>{5, 6, 7, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9}));
    v.resize(2, 0);                     // erase keeps the buffer
    CHECK_EQ(capacity(v), std::size_t{14});
    CHECK(items(v) == (std::vector<std::int32_t>{5, 6}));
}

TEST_CASE(cow_vector_insert_middle_both_branches)
{
    CowVector<std::int32_t> v;
    v.resize(8, 0);
    for (int i = 0; i < 8; ++i) v.mutable_at(static_cast<std::size_t>(i)) = i;
    v.resize(5, 0);  // capacity 8, size 5: {0,1,2,3,4}
    // n < after: insert 2 at index 1 (after = 4)
    v.insert(v.mutable_begin() + 1, 2, 42);
    CHECK(items(v) == (std::vector<std::int32_t>{0, 42, 42, 1, 2, 3, 4}));
    // n >= after: insert 1 at index 6 (after = 1), fits in capacity 8
    v.insert(v.mutable_begin() + 6, 1, 77);
    CHECK(items(v) == (std::vector<std::int32_t>{0, 42, 42, 1, 2, 3, 77, 4}));
    CHECK_EQ(capacity(v), std::size_t{8});
    // reallocation in the middle: size 8 + max(1, 8) = 16
    v.insert(v.mutable_begin(), 1, -1);
    CHECK(items(v) == (std::vector<std::int32_t>{-1, 0, 42, 42, 1, 2, 3, 77, 4}));
    CHECK_EQ(capacity(v), std::size_t{16});
}

TEST_CASE(cow_vector_insert_on_shared_rebases_position)
{
    CowVector<std::int32_t> a;
    a.resize(3, 1);
    CowVector<std::int32_t> b(a);
    // pos points into the shared buffer; 14019b6f0 rebases it onto the detached copy
    const std::int32_t* shared_first = b.begin();
    b.insert(const_cast<std::int32_t*>(shared_first) + 1, 1, 5);
    CHECK(items(a) == (std::vector<std::int32_t>{1, 1, 1}));
    CHECK(items(b) == (std::vector<std::int32_t>{1, 5, 1, 1}));
}

TEST_CASE(cow_vector_clear_frees)
{
    CowVector<void*> v;
    v.resize(4, nullptr);
    CowVector<void*> w(v);
    w.clear();  // detach then free (14019a7d0)
    CHECK_EQ(v.size(), std::size_t{4});
    CHECK_EQ(w.size(), std::size_t{0});
    CHECK(w.body()->first == nullptr && w.body()->end == nullptr);
}

TEST_CASE(cow_vector_clone_of_empty_is_null)
{
    CowVector<std::int32_t> a;
    a.resize(2, 3);
    a.resize(0, 0);  // size 0, buffer kept
    CowVector<std::int32_t> b(a);
    b.detach();      // count < 1 -> three null pointers
    CHECK(b.body()->first == nullptr);
    CHECK(a.body()->first != nullptr);
}

TEST_CASE(cow_list_push_and_index)
{
    CowList<std::int32_t> l;  // 140183900 builds {0x3c, 0x28, 0x31..0x34}
    l.push_back(0x3c);
    l.push_back(0x28);
    l.push_back(0x31);
    CHECK_EQ(l.size(), std::size_t{3});
    CHECK_EQ(l.at(0), 0x3c);
    CHECK_EQ(l.at(2), 0x31);
    CowList<std::int32_t> m(l);  // 14019acb0
    CHECK_EQ(l.use_count(), 2);
    m.push_back(0x32);           // detach: clone in order
    CHECK_EQ(l.size(), std::size_t{3});
    CHECK_EQ(m.size(), std::size_t{4});
    CHECK_EQ(m.at(3), 0x32);
    CHECK_EQ(l.use_count(), 1);
}

TEST_CASE(cow_list_mutable_at_detaches)
{
    CowList<void*> l;
    int x = 0, y = 0;
    l.push_back(&x);
    CowList<void*> m(l);
    m.mutable_at(0) = &y;  // 14019a860
    CHECK(l.at(0) == &x);
    CHECK(m.at(0) == &y);
}

TEST_CASE(cow_list_clear_shared_gets_new_body)
{
    CowList<void*> l;
    l.push_back(nullptr);
    CowList<void*> m(l);
    m.clear();  // 14015a620: refs != 1 -> refs--, new empty body
    CHECK_EQ(l.size(), std::size_t{1});
    CHECK_EQ(l.use_count(), 1);
    CHECK_EQ(m.size(), std::size_t{0});
    l.clear();  // refs == 1: in place (14015e4b0)
    CHECK_EQ(l.size(), std::size_t{0});
    CHECK_EQ(l.use_count(), 1);
}

TEST_CASE(cow_list_mutable_iteration_detaches)
{
    CowList<std::int32_t> l;
    l.push_back(1);
    l.push_back(2);
    CowList<std::int32_t> m(l);
    for (auto it = m.mutable_begin(); it != m.mutable_end(); ++it) *it += 10;
    CHECK_EQ(l.at(0), 1);
    CHECK_EQ(m.at(0), 11);
    CHECK_EQ(m.at(1), 12);
    CHECK_EQ(l.use_count(), 1);
    CHECK_EQ(m.use_count(), 1);
}

int main() { return testkit::run_all(); }
