// ctest-labels: p67-7 ctc3d
// Tier-1 tests of group "pipeline" (packet p67-7): common BC codes (14003a978, 14003a580) on
// hand-made code arrays, the column sorts (14008b65b and the quicksort pairs) on arrays with
// ties, the partner list setters and the message formats of 140052f49. Expected sort orders come
// from hand traces (4-column case) and from an independent simulation of the decompile
// (deform-rea-work/scripts_p677_pipeline/sortsim.py, sortcheck.py) for the longer ones; the
// longer ones are cases where the shared SAVE indices change the order of equal keys against a
// plain quicksort.
#include <defcmd/ctc3d/decide.hpp>
#include <defcmd/ctc3d/pipeline.hpp>

#include <defrt/ftn/fio.hpp>
#include <testkit/check.hpp>

#include <vector>

using namespace def::cmd::ctc3d;

namespace {

// rows of a(nrow, ncol) as a flat column-major vector
std::vector<i32> row(const std::vector<i32>& a, int nrow, int r)
{
    std::vector<i32> out;
    for (std::size_t j = 0; j * nrow < a.size(); ++j) {
        out.push_back(a[j * nrow + (r - 1)]);
    }
    return out;
}

} // namespace

TEST_CASE(common_code_fixed_direction)
{
    reset_image();
    // bcc(3, node): node 1 (1,0,0), node 2 (1,1,0), node 3 (1,0,5)
    const i32 bcc[] = {1, 0, 0, 1, 1, 0, 1, 0, 5};
    const i32 conn[] = {1, 2, 3};
    const i32 n = 3, ld = 3;
    i32 code = -9, dir = -9;
    fun_14003a978(bcc, conn, &n, &ld, &code, &dir);
    CHECK_EQ(code, 1);
    CHECK_EQ(dir, 1);
}

TEST_CASE(common_code_any_component)
{
    reset_image();
    const i32 bcc[] = {0, 7, 0, 7, 0, 0, 0, 0, 7};
    const i32 conn[] = {1, 2, 3};
    const i32 n = 3, ld = 3;
    i32 code = -9, dir = -9;
    fun_14003a978(bcc, conn, &n, &ld, &code, &dir);
    CHECK_EQ(code, 7);
    CHECK_EQ(dir, 0);
}

TEST_CASE(common_code_none)
{
    reset_image();
    // code 1 in different directions: not common
    const i32 bcc[] = {1, 0, 0, 0, 1, 0};
    const i32 conn[] = {1, 2};
    const i32 n = 2, ld = 3;
    i32 code = -9, dir = -9;
    fun_14003a978(bcc, conn, &n, &ld, &code, &dir);
    CHECK_EQ(code, 0);
    CHECK_EQ(dir, 0);
    // negative first node: nothing
    const i32 conn2[] = {-1, 2};
    fun_14003a978(bcc, conn2, &n, &ld, &code, &dir);
    CHECK_EQ(code, 0);
}

TEST_CASE(common_code_single_node_keeps_stale_direction)
{
    reset_image();
    // first call leaves DAT_1406559dc = 2
    const i32 bcc[] = {0, 1, 0, 0, 1, 0};
    const i32 conn[] = {1, 2};
    const i32 n2 = 2, ld = 3;
    i32 code = 0, dir = 0;
    fun_14003a978(bcc, conn, &n2, &ld, &code, &dir);
    CHECK_EQ(code, 1);
    CHECK_EQ(dir, 2);
    // one node with code 1 in component 1: the direction is the SAVE value of the last match
    const i32 bcc1[] = {1, 0, 0};
    const i32 one = 1;
    const i32 c1[] = {1};
    fun_14003a978(bcc1, c1, &one, &ld, &code, &dir);
    CHECK_EQ(code, 1);
    CHECK_EQ(dir, 2);
}

TEST_CASE(code_list_three_codes)
{
    reset_image();
    // two nodes (1, 1, 603): fixed x, fixed y, code 603
    const i32 bcc[] = {1, 1, 603, 1, 1, 603};
    const i32 conn[] = {1, 2};
    const i32 n = 2, ld = 3;
    i32 cnt = -1;
    i32 out[16] = {};
    fun_14003a580(bcc, conn, &n, &ld, &cnt, out);
    REQUIRE(cnt == 3);
    CHECK_EQ(out[0], 1);
    CHECK_EQ(out[1], 1);
    CHECK_EQ(out[2], 1);
    CHECK_EQ(out[3], 2);
    CHECK_EQ(out[4], 603);
    CHECK_EQ(out[5], 0);
}

TEST_CASE(code_list_fixed_code_clears_lower_rows)
{
    reset_image();
    // node 1 (1, 0, 1), node 2 (0, 1, 1): common code 1 in direction 3; removing it also clears
    // the 1s of rows 1 and 2 (the clearing loop runs over rows 1..dir), so nothing is left
    const i32 bcc[] = {1, 0, 1, 0, 1, 1};
    const i32 conn[] = {1, 2};
    const i32 n = 2, ld = 3;
    i32 cnt = -1;
    i32 out[16] = {};
    fun_14003a580(bcc, conn, &n, &ld, &cnt, out);
    REQUIRE(cnt == 1);
    CHECK_EQ(out[0], 1);
    CHECK_EQ(out[1], 3);
}

TEST_CASE(code_list_negative_and_none)
{
    reset_image();
    // contact code -3 shared in different components, plus a code 5 only on node 1
    const i32 bcc[] = {-3, 5, 0, 0, 0, -3};
    const i32 conn[] = {1, 2};
    const i32 n = 2, ld = 3;
    i32 cnt = -1;
    i32 out[16] = {};
    fun_14003a580(bcc, conn, &n, &ld, &cnt, out);
    REQUIRE(cnt == 1);
    CHECK_EQ(out[0], -3);
    CHECK_EQ(out[1], 0);
    const i32 none[] = {0, 0, 0, 4, 4, 4};
    fun_14003a580(none, conn, &n, &ld, &cnt, out);
    CHECK_EQ(cnt, 0);
}

TEST_CASE(quicksort_ascending_ties_hand_trace)
{
    reset_image();
    // columns (key, payload); hand-traced: pivot 1 at the front, then 14008c159 on 2..4
    std::vector<i32> a = {2, 10, 1, 20, 2, 30, 1, 40};
    const i32 nrow = 2, key = 1, lo = 1, hi = 4;
    fun_14008bf48(a.data(), &nrow, &key, &lo, &hi);
    CHECK(row(a, 2, 1) == (std::vector<i32>{1, 1, 2, 2}));
    CHECK(row(a, 2, 2) == (std::vector<i32>{20, 40, 30, 10}));
}

TEST_CASE(quicksort_descending_ties)
{
    reset_image();
    std::vector<i32> a = {2, 10, 1, 20, 2, 30, 1, 40};
    const i32 nrow = 2, key = 1, lo = 1, hi = 4;
    fun_14008bb26(a.data(), &nrow, &key, &lo, &hi);
    CHECK(row(a, 2, 1) == (std::vector<i32>{2, 2, 1, 1}));
    CHECK(row(a, 2, 2) == (std::vector<i32>{30, 10, 20, 40}));

    const std::vector<i32> keys = {5, 4, 3, 2, 1, 6, 7, 3, 3, 1};
    std::vector<i32> b;
    for (std::size_t i = 0; i < keys.size(); ++i) {
        b.push_back(keys[i]);
        b.push_back(static_cast<i32>(i + 1));
    }
    const i32 hi2 = 10;
    fun_14008bb26(b.data(), &nrow, &key, &lo, &hi2);
    CHECK(row(b, 2, 1) == (std::vector<i32>{7, 6, 5, 4, 3, 3, 3, 2, 1, 1}));
    CHECK(row(b, 2, 2) == (std::vector<i32>{7, 6, 1, 2, 9, 3, 8, 4, 5, 10}));
}

TEST_CASE(quicksort_save_index_reuse)
{
    reset_image();
    // a case where the nested call's SAVE index changes the order of equal keys against a plain
    // quicksort (which would give ... 4/8, 4/11 ...)
    const std::vector<i32> keys = {4, 4, 4, 2, 1, 4, 1, 4, 4, 1, 4, 3, 2, 1};
    std::vector<i32> a;
    for (std::size_t i = 0; i < keys.size(); ++i) {
        a.push_back(keys[i]);
        a.push_back(static_cast<i32>(i + 1));
    }
    const i32 nrow = 2, key = 1, lo = 1, hi = 14;
    fun_14008bf48(a.data(), &nrow, &key, &lo, &hi);
    CHECK(row(a, 2, 1) == (std::vector<i32>{1, 1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 4, 4, 4}));
    CHECK(row(a, 2, 2) == (std::vector<i32>{7, 5, 10, 14, 4, 13, 12, 11, 8, 6, 3, 1, 9, 2}));
}

TEST_CASE(edge_table_sort)
{
    reset_image();
    // edge records (n1, n2, polygon) as 1400501d7 builds them, sorted ascending on all rows
    std::vector<i32> e = {3, 7, 1, 1, 2, 1, 2, 3, 1, 1, 3, 1, 3, 7, 2, 1, 2, 2,
                          2, 5, 2, 2, 3, 2, 1, 4, 3, 4, 5, 3, 1, 3, 3, 3, 4, 3};
    const i32 nrow = 3, ncol = 12, dir = 1;
    fun_14008b65b(e.data(), &nrow, &ncol, &dir);
    const std::vector<i32> want = {1, 2, 1, 1, 2, 2, 1, 3, 1, 1, 3, 3, 1, 4, 3, 2, 3, 1,
                                   2, 3, 2, 2, 5, 2, 3, 4, 3, 3, 7, 1, 3, 7, 2, 4, 5, 3};
    CHECK(e == want);
}

TEST_CASE(swap_columns)
{
    reset_image();
    std::vector<i32> a = {1, 2, 3, 4, 5, 6};
    const i32 nrow = 3, i = 1, j = 2;
    fun_14008ba00(a.data(), &nrow, &i, &j);
    CHECK(a == (std::vector<i32>{4, 5, 6, 1, 2, 3}));
    CHECK_EQ(m<i32>(0x1406dc270), 4);
}

TEST_CASE(partner_list)
{
    reset_image();
    const i32 parts[] = {3, 1, 4, 2};
    i32 one = 1;
    const i32 n = 4;
    fun_1400511e4(&one, &n, parts);
    CHECK_EQ(dat_14077867c, 4);
    CHECK_EQ(dat_140771918[1], 3);
    CHECK_EQ(dat_140771918[2], 1);
    CHECK_EQ(dat_140771918[3], 4);
    CHECK_EQ(dat_140771918[4], 2);
    CHECK_EQ(m<i32>(0x140676700), 5);
    const i32 obj = 3, val = 1;
    fun_140082ed6(&obj, &val);
    CHECK_EQ(dat_140778684[3], 1);
}

TEST_CASE(corner_node_messages)
{
    namespace fio = def::rt::ftn::fio;
    CHECK_EQ(fio::Write(1, "(' Info: Critical Corner Node Idenfied - Object Number :'       , I5)")
                 .i(2)
                 .text(),
             std::string(" Info: Critical Corner Node Idenfied - Object Number :    2"));
    CHECK_EQ(fio::Write(1, "(' Info: Node: ', I7)").i(117).text(), std::string(" Info: Node:     117"));
}

int main() { return testkit::run_all(); }
