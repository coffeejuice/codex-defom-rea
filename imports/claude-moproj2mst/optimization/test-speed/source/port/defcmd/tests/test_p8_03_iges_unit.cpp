// Tier-1 unit tests of the IGES reader (packet p8-03): the Fortran field conversions and pass 2
// on a segment table (mode 700, the DXF path of p8-04).
// ctest-labels: p8-03
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>
#include <string>

#include <defcmd/iges.hpp>

#include "../src/iges/ftn_read.hpp"

using namespace def::cmd::iges;

TEST_CASE(fortran_fields)
{
    std::int32_t v = 7;
    CHECK(ftnrd::read_int("       ", &v));
    CHECK_EQ(v, 0);
    CHECK(ftnrd::read_int("  -12 3", &v));
    CHECK_EQ(v, -123);
    CHECK(!ftnrd::read_int("   12a", &v));
    double x = 1.0;
    CHECK(ftnrd::read_real_e0("   ", &x));
    CHECK_BITS(x, 0.0);
    CHECK(ftnrd::read_real_e0(" 0.25D1", &x));
    CHECK_BITS(x, 2.5);
    CHECK(ftnrd::read_real_e0("1.5+2", &x));
    CHECK_BITS(x, 150.0);
    CHECK(ftnrd::read_real_e0("25", &x));
    CHECK_BITS(x, 25.0);
    // T8-2: the exact decimal of 0.3 converts below 0.3
    CHECK(ftnrd::read_real_e0("0.299999999999999988897769753748434595763683319091796875", &x));
    CHECK(x != 0.3);
    CHECK(!ftnrd::read_real_e0("1.2.3", &x));
    CHECK(!ftnrd::read_real_e0("2HAB", &x));
    CHECK_EQ(ftnrd::field("ABC", 2, 4), std::string("BC  "));
}

// T8-2: values of FUN_1404c971c read from the binary itself (mapped-image harness
// deform-rea-work/scripts_p803/ew0diff, 1.1 M vectors, 0 mismatches)
TEST_CASE(ew0_runtime_conversion)
{
    struct V {
        const char* s;
        std::uint64_t bits;
    };
    static const V v[] = {
        // the four probes that a correctly rounded model got wrong
        {"1.0000000000000002220446049250313080847263336181640625", 0x3ff0000000000001ull},
        {"1.00000000000000011102230246251565404236316680908203126", 0x3ff0000000000000ull},
        {"1.000000000000000444089209850062616169452667236328125", 0x3ff0000000000002ull},
        {"1.000000000000000333066907387546962127089500427246093750", 0x3ff0000000000001ull},
        {"5.07141598158837", 0x40144921456555ccull},
        {"5.0714159815883697e+00", 0x40144921456555ccull},
        {"5755.43139553431", 0x40b67b6e6ff00f80ull},
        {"5755.4313955343096", 0x40b67b6e6ff00f80ull},
        // 19-digit integer cut to 53 bits, not rounded
        {"1180591620717411303424.0", 0x444fffffffffffffull},
        {"9007199254740993", 0x4340000000000000ull},
        // one scaling by 10**|e| from repeated squaring
        {"3.82714061247971e+11", 0x425646e2486ffe25ull},
        {"5.91576405375690e-06", 0x3ed8d000d7507206ull},
        {"0.000000000000000000001234567890123456789", 0x3b97520105bbfffaull},
        {"-1.5-2", 0xbf8eb851eb851eb8ull},
        {"1D308", 0x7fe1ccf385ebc8a3ull},
        {"1.7976931348623157E308", 0x7ff0000000000000ull},
        {"2.2250738585072014E-308", 0x000ffffffffffffdull},
        {"4.9E-324", 0x0000000000000001ull},
        {"2.4703282292062328E-324", 0x0000000000000000ull},
        {"1E-400", 0x0000000000000000ull},
        {"0E400", 0xfff8000000000000ull},
        // Q is accepted but its exponent is not read; signs and empty parts
        {"1.5Q2", 0x3ff8000000000000ull},
        {"-0", 0x8000000000000000ull},
        {"-.5", 0xbfe0000000000000ull},
        {"0.0000000000000000000000000000001E31", 0x3ff0000000000000ull},
    };
    for (const V& e : v) {
        const double d = ftnrd::ew0_value(e.s);
        std::uint64_t b;
        std::memcpy(&b, &d, 8);
        CHECK_EQ(b, e.bits);
    }
    double x = 0.0;
    CHECK(ftnrd::read_real_e0(" 5755.4313955343096 ", &x));
    CHECK_BITS(x, 5755.43139553431);
}

TEST_CASE(pass2_mode700_square)
{
    reset_globals();
    g_iges_rw = static_cast<double*>(std::calloc(static_cast<std::size_t>(g_iges_rw_size), 8));
    g_iges_iw = static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(g_iges_iw_size), 4));
    // a unit square, the second line reversed, the third one 1e-6 off
    double t[4 * 6] = {
        1, 0, 0, 1, 0, 0,  //
        1, 1, 1, 1, 0, 0,  //
        1, 1, 1, 0, 1 + 1e-6, 0, //
        1, 0, 1, 0, 0, 0,
    };
    IgesCtx& c = g_iges_ctx;
    c.n_records = 4;
    c.n_points = 0;
    c.tol = 0.01;
    c.table = t;
    const int r = fun_14040c220("", &c, 2, 700);
    CHECK_EQ(r, 0);
    CHECK_EQ(c.n_records, 4);
    // one chain, every row a line between matched (averaged) corners
    for (int k = 0; k < 4; ++k) CHECK_BITS(t[k * 6], 1.0);
    const double mid = (1.0 + (1.0 + 1e-6)) * 0.5;
    int n_mid = 0;
    for (int k = 0; k < 24; ++k) {
        CHECK(t[k] != 1.0 + 1e-6);
        if (t[k] == mid) ++n_mid;
    }
    CHECK_EQ(n_mid, 2);
    c.table = nullptr;
    reset_globals();
}

int main() { return testkit::run_all(); }
