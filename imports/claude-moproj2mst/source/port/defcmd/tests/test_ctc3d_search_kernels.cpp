// ctest-labels: p67-7 ctc3d
// Tier-1 tests of the geometric kernels of group "search" (packet p67-7):
//  * 140066ff1, 140066a50, 140070bf5, 1400702c8, 1400839c6: golden bits from the ORIGINAL routines,
//    called in a copy of DEF_PRE_64.exe mapped at its preferred base (harness
//    deform-rea-work/scripts_p677_search/gold.cpp, built /Od; pure floating-point call trees, no
//    I/O). Random inputs with NaN / signed zero / tiny / degenerate cases (ctc3d_search_gold.inc).
//    1400702c8 and 1400839c6 keep SAVE variables between calls, so their cases run in the
//    generator's order from a fresh image.
//  * 140062fb2: hand cases.
#include <defcmd/ctc3d/search.hpp>
#include <testkit/check.hpp>

#include <bit>
#include <cstdint>

using namespace def::cmd::ctc3d;

namespace {
#include "ctc3d_search_gold.inc"

double bits(std::uint64_t u) { return std::bit_cast<double>(u); }
i32 ibits(std::uint64_t u) { return static_cast<i32>(static_cast<std::uint32_t>(u)); }
} // namespace

TEST_CASE(cross_140066ff1_golden)
{
    for (int c = 0; c < g66ff1_n; ++c) {
        const std::uint64_t* g = g66ff1 + 10 * c;
        double a[3], b[3], r[3] = {-7, -7, -7}, q = -7;
        for (int i = 0; i < 3; ++i) {
            a[i] = bits(g[i]);
            b[i] = bits(g[3 + i]);
        }
        fun_140066ff1(a, b, r, &q);
        for (int i = 0; i < 3; ++i) CHECK_BITS(r[i], bits(g[6 + i]));
        CHECK_BITS(q, bits(g[9]));
    }
}

TEST_CASE(normal_140066a50_golden)
{
    for (int c = 0; c < g66a50_n; ++c) {
        const std::uint64_t* g = g66a50 + 13 * c;
        double x[9], n[3] = {-7, -7, -7};
        i32 bad = -7;
        for (int i = 0; i < 9; ++i) x[i] = bits(g[i]);
        fun_140066a50(x, n, &bad);
        for (int i = 0; i < 3; ++i) CHECK_BITS(n[i], bits(g[9 + i]));
        CHECK_EQ(bad, ibits(g[12]));
    }
}

TEST_CASE(segment_140070bf5_golden)
{
    for (int c = 0; c < g70bf5_n; ++c) {
        const std::uint64_t* g = g70bf5 + 14 * c;
        double p[3], a[3], b[3], f[3] = {-7, -7, -7}, r = -7, d = -7;
        for (int i = 0; i < 3; ++i) {
            p[i] = bits(g[i]);
            a[i] = bits(g[3 + i]);
            b[i] = bits(g[6 + i]);
        }
        fun_140070bf5(p, a, b, f, &r, &d);
        for (int i = 0; i < 3; ++i) CHECK_BITS(f[i], bits(g[9 + i]));
        CHECK_BITS(r, bits(g[12]));
        CHECK_BITS(d, bits(g[13]));
    }
}

TEST_CASE(triangle_1400702c8_golden)
{
    reset_image();
    for (int c = 0; c < g702c8_n; ++c) {
        const std::uint64_t* g = g702c8 + 23 * c;
        double p[3], x[9], q[3] = {-7, -7, -7}, s = -7, t = -7;
        for (int i = 0; i < 3; ++i) p[i] = bits(g[i]);
        for (int i = 0; i < 9; ++i) x[i] = bits(g[3 + i]);
        double d = bits(g[12]);
        i32 mode = ibits(g[13]), found = ibits(g[14]), code = -9;
        fun_1400702c8(p, x, q, &d, &s, &t, &mode, &found, &code);
        for (int i = 0; i < 3; ++i) CHECK_BITS(q[i], bits(g[15 + i]));
        CHECK_BITS(d, bits(g[18]));
        CHECK_BITS(s, bits(g[19]));
        CHECK_BITS(t, bits(g[20]));
        CHECK_EQ(found, ibits(g[21]));
        CHECK_EQ(code, ibits(g[22]));
    }
}

TEST_CASE(volume_sign_1400839c6_golden)
{
    reset_image();
    for (int c = 0; c < g839c6_n; ++c) {
        const std::uint64_t* g = g839c6 + 19 * c;
        double p[3], x[12];
        for (int i = 0; i < 3; ++i) p[i] = bits(g[i]);
        for (int i = 0; i < 12; ++i) x[i] = bits(g[3 + i]);
        i32 nv = ibits(g[15]), edge = ibits(g[16]), sgn = -7;
        fun_1400839c6(p, x, &nv, &edge, &sgn);
        CHECK_EQ(sgn, ibits(g[17]));
        CHECK_BITS(m<double>(0x1406d3648), bits(g[18]));
    }
}

TEST_CASE(node_list_140062fb2)
{
    i32 n = 4, flag = 9;
    i32 c1[4] = {3, 4, 5, 6};
    fun_140062fb2(c1, &n, &flag);
    CHECK_EQ(flag, 1);
    i32 c2[4] = {3, 4, 5, -6};
    fun_140062fb2(c2, &n, &flag);
    CHECK_EQ(flag, -1);
    CHECK_EQ(c2[3], 6);
    i32 c3[4] = {-3, 4, 5, -6};
    fun_140062fb2(c3, &n, &flag);
    CHECK_EQ(flag, 0);
    CHECK_EQ(c3[0], 3);
    CHECK_EQ(c3[3], 6);
    i32 c4[4] = {0, 4, 5, 0}; // last 0 -> -1 then first 0 -> 0
    fun_140062fb2(c4, &n, &flag);
    CHECK_EQ(flag, 0);
    i32 n3 = 3;
    i32 c5[4] = {1, 2, -3, 77};
    fun_140062fb2(c5, &n3, &flag);
    CHECK_EQ(flag, -1);
    CHECK_EQ(c5[2], 3);
    CHECK_EQ(c5[3], 77);
}

int main() { return testkit::run_all(); }
