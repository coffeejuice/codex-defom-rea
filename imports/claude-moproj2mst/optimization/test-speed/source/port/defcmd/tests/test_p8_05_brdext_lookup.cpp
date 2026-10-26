// Tier-1: BRDEXT's DB lookup (14013d230 / 14013cc10 / comparator 14013ca60 / MSVC qsort) with
// several <PROBID><suffix>.DB files, which no oracle case covers. Expected values by hand:
//   files (FindFirstFileA order on NTFS): P.DB P1.DB P2.DB P3.DB PX.DB Q5.DB
//   matches of PROBID "P" (prefix, stem != "P"): suffixes "1" "2" "3" "X" (m = 4)
//   the comparator sees the zeroed allocator bytes of each slot -> "" -> 0: all pairs equal;
//   qsort with 4 <= 8 elements = shortsort: each pass swaps the first element with the last:
//   [1 2 3 X] -> [X 2 3 1] -> [3 2 X 1] -> [2 3 X 1]; slots 4, 5 = "".
//   pick: first k with abs(%d) >= abs(step): step 1 -> k 0 ("2"), 2 -> 0, 3 -> 1 ("3"),
//   -3 -> 1, 4 -> -1 ("X" reads as 0).
// ctest-labels: p8-05 brdext
#include <defcmd/brdext.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <filesystem>
#include <string>
#include <vector>

#include <process.h>

namespace fs = std::filesystem;
using namespace def::cmd::brdext;

namespace {

fs::path make_dir()
{
    const fs::path d = fs::temp_directory_path() / ("p805_lookup_" + std::to_string(_getpid()));
    std::error_code ec;
    fs::remove_all(d, ec);
    fs::create_directories(d);
    for (const char* n : {"P.DB", "P1.DB", "P2.DB", "P3.DB", "PX.DB", "Q5.DB"}) {
        std::FILE* f = std::fopen((d / n).string().c_str(), "wb");
        std::fclose(f);
    }
    fs::create_directories(d / "P9.DB"); // a directory: skipped by 1401b1630
    return d;
}

} // namespace

TEST_CASE(pick_suffix_order_and_choice)
{
    const fs::path d = make_dir();
    const std::string dir = d.string() + "\\";
    const std::string prob = "P";
    struct C { std::int32_t step, k; } cases[] = {{1, 0}, {2, 0}, {3, 1}, {-3, 1}, {4, -1}};
    for (const C& c : cases) {
        std::vector<std::string> arr(6, "junk");
        const std::int32_t k = pick_suffix(&c.step, prob, dir, arr);
        CHECK_EQ(k, c.k);
        CHECK(arr[0] == "2");
        CHECK(arr[1] == "3");
        CHECK(arr[2] == "X");
        CHECK(arr[3] == "1");
        CHECK(arr[4] == "");
        CHECK(arr[5] == "");
    }
    std::error_code ec;
    fs::remove_all(d, ec);
}

TEST_CASE(find_db_found_and_not_found)
{
    const fs::path d = make_dir();
    std::int32_t step = 3;
    std::string prob = "P";
    std::string db = "ORIG.DB";
    CHECK(find_db(&step, prob, d.string(), db));
    CHECK(prob == "P3");
    CHECK(db == d.string() + "\\P3.DB");

    step = 7;
    prob = "P";
    db = "ORIG.DB";
    CHECK(!find_db(&step, prob, d.string(), db));
    CHECK(prob == "P");
    CHECK(db == "ORIG.DB");

    step = 1;
    prob = "Q";
    CHECK(find_db(&step, prob, d.string(), db)); // Q5: 5 >= 1
    CHECK(prob == "Q5");
    std::error_code ec;
    fs::remove_all(d, ec);
}

TEST_CASE(comparator_reads_slot_bytes)
{
    SuffixSlot a{}, b{};
    CHECK_EQ(compare_suffix(&a, &b), 0);
    a.alloc_bytes[0] = '7';
    b.alloc_bytes[0] = '-';
    b.alloc_bytes[1] = '9';
    CHECK_EQ(compare_suffix(&a, &b), -1); // 7 < abs(-9)
    CHECK_EQ(compare_suffix(&b, &a), 1);
}

int main() { return testkit::run_all(); }
