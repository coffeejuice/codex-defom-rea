// Tier-1: printf formatting after def::rt::init_stdio_compat() matches the 2014 CRT.
// ctest-labels: p1-01 stdio
//
// Expected strings follow the VS2008 CRT's formatting rules: _cftoe writes a three-digit
// exponent, _cfltcvt prints infinities and NaNs as "1.#INF" / "1.#IND" / "1.#QNAN" padded or cut
// to the precision, and _fptostr rounds the decimal digits half up (away from zero).
#include <defrt/stdlib/stdio_compat_test.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <limits>
#include <string>

namespace {

template <class... A>
std::string fmt(const char* f, A... a)
{
    char buf[256];
    std::snprintf(buf, sizeof buf, f, a...);
    return buf;
}

const double kInf = std::numeric_limits<double>::infinity();
const double kNan = std::numeric_limits<double>::quiet_NaN();

} // namespace

TEST_CASE(three_digit_exponents)
{
    CHECK_EQ(fmt("%20.10E", 0.0), std::string("   0.0000000000E+000"));
    CHECK_EQ(fmt("%20.10E", -1234.5), std::string("  -1.2345000000E+003"));
    CHECK_EQ(fmt("%e", 123.0), std::string("1.230000e+002"));
    CHECK_EQ(fmt("%E", 1.0e-300), std::string("1.000000E-300"));
    CHECK_EQ(fmt("%g", 1.0e-5), std::string("1e-005"));
    CHECK_EQ(fmt("%G", 2.5e20), std::string("2.5E+020"));
    CHECK_EQ(fmt("%g", 0.0001), std::string("0.0001"));
}

TEST_CASE(rounding_half_away_from_zero)
{
    // exact binary ties: the old CRT rounds the digit string up
    CHECK_EQ(fmt("%.0f", 0.5), std::string("1"));
    CHECK_EQ(fmt("%.0f", 2.5), std::string("3"));
    CHECK_EQ(fmt("%.0f", -2.5), std::string("-3"));
    CHECK_EQ(fmt("%.1f", 0.25), std::string("0.3"));
    CHECK_EQ(fmt("%.2E", 1.125), std::string("1.13E+000"));
    // not a tie in binary (0.15 is 0.1499999999999999944...)
    CHECK_EQ(fmt("%.1f", 0.15), std::string("0.1"));
}

TEST_CASE(infinity_and_nan_text)
{
    CHECK_EQ(fmt("%f", kInf), std::string("1.#INF00"));
    CHECK_EQ(fmt("%f", -kInf), std::string("-1.#INF00"));
    CHECK_EQ(fmt("%f", kNan), std::string("1.#QNAN0"));
    CHECK_EQ(fmt("%f", kInf * 0.0), std::string("-1.#IND00")); // x64 default NaN is negative
    CHECK_EQ(fmt("%E", kInf), std::string("1.#INF00E+000"));
    // the old CRT rounds the text "1.#INF" like digits: precision 2 gives "1.#J"
    CHECK_EQ(fmt("%10.2E", -kInf), std::string("-1.#JE+000"));
}

int main() { return testkit::run_all(); }
