// Tier-1: def::rt::crt printf, the VS2008 CRT's floating-point formatting.
// ctest-labels: p1-01 stdio printf
//
// Expected strings are worked out by hand from the CRT's algorithm: $I10_OUTPUT forms 18 digits
// of the value and rounds them half up to 17; _fptostr rounds those 17 half up to the precision.
// The exact decimal value of each double is given in the comment.
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/stdio_compat_test.hpp>
#include <testkit/check.hpp>

#include <limits>
#include <string>

using def::rt::crt::format;

TEST_CASE(seventeen_significant_digits)
{
    // 0.1 = 0.1000000000000000055511...: 18 digits ...0005 -> 17 digits 10000000000000001
    CHECK_EQ(format("%.20f", 0.1), std::string("0.10000000000000001000"));
    // 1e23 = 99999999999999991611392: 18 digits 999999999999999916 -> 99999999999999992
    CHECK_EQ(format("%f", 1e23), std::string("99999999999999992000000.000000"));
    CHECK_EQ(format("%.25e", 0.1), std::string("1.0000000000000001000000000e-001"));
}

TEST_CASE(double_rounding_through_seventeen_digits)
{
    // 1015.07125 = 1015.07124999999996362...: 18 digits 101507124999999996 round to
    // 17 digits 10150712500000000; to 8 digits 1.0150713 (the exact value gives ...712)
    CHECK_EQ(format("%15.7E", 1015.07125), std::string(" 1.0150713E+003"));
    // 0.0473895715335 = 0.04738957153349999951...: 17 digits 47389571533500000 -> 11 digits
    CHECK_EQ(format("%20.10E", 0.0473895715335), std::string("   4.7389571534E-002"));
    CHECK_EQ(format("%20.10E", -0.0473895715335), std::string("  -4.7389571534E-002"));
    // 245.5025 = 245.50249999999999772...: %g (6 digits) gives 245.503
    CHECK_EQ(format("%g", 245.5025), std::string("245.503"));
}

TEST_CASE(layout_of_e_f_g)
{
    CHECK_EQ(format("%20.10E", 0.0), std::string("   0.0000000000E+000"));
    CHECK_EQ(format("%20.10E", -0.0), std::string("  -0.0000000000E+000")); // __dtold keeps the sign
    CHECK_EQ(format("%e", 123.0), std::string("1.230000e+002"));
    CHECK_EQ(format("%E", 1.0e-300), std::string("1.000000E-300"));
    CHECK_EQ(format("%.0f", 2.5), std::string("3"));
    CHECK_EQ(format("%.2f", 0.125), std::string("0.13"));
    CHECK_EQ(format("%.2f", 0.0004), std::string("0.00"));
    CHECK_EQ(format("%.2f", 0.006), std::string("0.01"));
    CHECK_EQ(format("%g", 100000.0), std::string("100000"));
    CHECK_EQ(format("%g", 1000000.0), std::string("1e+006"));
    CHECK_EQ(format("%g", 0.0001), std::string("0.0001"));
    CHECK_EQ(format("%g", 0.00001), std::string("1e-005"));
    CHECK_EQ(format("%G", 2.5e20), std::string("2.5E+020"));
    CHECK_EQ(format("%#.0f", 3.0), std::string("3."));
    CHECK_EQ(format("%#g", 1.0), std::string("1.00000"));
    CHECK_EQ(format("%+010.3f", 3.14159), std::string("+00003.142"));
    CHECK_EQ(format("%-10.2e|", -1.5), std::string("-1.50e+000|"));
}

TEST_CASE(other_conversions_and_specials)
{
    CHECK_EQ(format("%5d|%-4s|%7.3f|%%|%c", 42, "ab", 3.14159, 'x'),
             std::string("   42|ab  |  3.142|%|x"));
    CHECK_EQ(format("%I64d %lld %ld %x", 1234567890123LL, -5LL, 7L, 255), std::string("1234567890123 -5 7 ff"));
    CHECK_EQ(format("%*.*f", 8, 2, 1.005), std::string("    1.00")); // 1.00499999999999989: 17 digits 10049999999999999
    const double inf = std::numeric_limits<double>::infinity();
    CHECK_EQ(format("%f", inf), std::string("1.#INF00"));
    CHECK_EQ(format("%20.10E", -inf), std::string("  -1.#INF000000E+000"));
}

// '%' before a character that is no conversion (state table 14056fb30): only that character is
// written, flags/width/size after the '%' are dropped, a '*' still takes its argument. Expected
// strings are msvcrt.dll's sprintf output for the same calls (the same _output state table).
TEST_CASE(percent_before_a_non_conversion_character)
{
    CHECK_EQ(format("%10d %\n", 5), std::string("         5 \n"));   // DEFINT .RMS line, str 14051e500
    CHECK_EQ(format("a%qb"), std::string("aqb"));
    CHECK_EQ(format("%5q|"), std::string("q|"));
    CHECK_EQ(format("%-08.3lq|"), std::string("q|"));
    CHECK_EQ(format("%*q|%d", 7, 9), std::string("q|9"));
    CHECK_EQ(format("%%|"), std::string("%|"));
    CHECK_EQ(format("x%"), std::string("x"));
}

int main() { return testkit::run_all(); }
