// ctest-labels: p-dbread dbread
// DBREAD entry (src/dbread/lead.cpp): links the whole read path (every subtree reader W1-W4 is
// referenced from fun_1401cc420), and checks the early returns that need no DB file:
// a DB object without a step directory (+0xad8 == 0) gives 2; a step number missing from the
// directory gives 1 and prints "Step %d not found in Database %s." to stdout.
#include <defdbio/dbread/dbread.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <testkit/check.hpp>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>

namespace {

// a raw DB object image large enough for the offsets DBREAD reads (+0x650 name, +0xac8 count,
// +0xad8 directory)
struct FakeDb {
    alignas(8) std::byte raw[0x2000] = {};
    template <class T>
    T& at(std::size_t off) { return *reinterpret_cast<T*>(raw + off); }
};

} // namespace

TEST_CASE(no_directory_returns_2)
{
    auto m = std::make_unique<def::model::DB_STEP_DATA>(3);
    FakeDb db;
    CHECK_EQ(def::dbio::dbread::fun_1401cc420(m.get(), &db, 0), 2);
}

TEST_CASE(missing_step_returns_1)
{
    auto m = std::make_unique<def::model::DB_STEP_DATA>(3);
    FakeDb db;
    std::strcpy(reinterpret_cast<char*>(db.raw + 0x650), "NONE.DB");
    alignas(8) std::byte dir[0x28] = {};
    db.at<std::int32_t>(0xac8) = 1;
    db.at<std::byte*>(0xad8) = dir;
    *reinterpret_cast<std::int32_t*>(dir + 4) = 7; // the only entry holds step 7
    CHECK_EQ(def::dbio::dbread::db_read_step(m.get(), reinterpret_cast<def::model::Cls_1401dbfe0*>(&db), 5, 1, 0), 1);
}

int main() { return testkit::run_all(); }
