// Tier-1 tests of the small Fortran helpers and unit conversions (packet p1-12). Expected values
// are worked out from the decompiled code (comments) and the constants read from the binary.
// ctest-labels: p1-12 ftn
#include <defrt/ftn/helpers.hpp>
#include <defrt/ftn/units.hpp>
#include <testkit/check.hpp>

#include <bit>
#include <cmath>
#include <cstdint>

using namespace def::rt::ftn;

TEST_CASE(units_table_and_modes)
{
    reset_unit_globals();
    CHECK_EQ(unit_system(), 1);
    g_units.unit_flag = 3;
    CHECK_EQ(unit_system(), 2);
    CHECK(testkit::same_bits(unit_factor(1, 1), std::bit_cast<double>(0x3fa42850ac14f154ULL)));
    CHECK(testkit::same_bits(unit_factor(1, 2), 25.4));
    CHECK(testkit::same_bits(unit_factor(5, 7), 1.0)); // other modes: DAT_140509c90
    g_units.factor[4] = 3.25;
    CHECK(testkit::same_bits(unit_factor(4, 3), 3.25));
}

TEST_CASE(temperature)
{
    reset_unit_globals();
    const double k18 = std::bit_cast<double>(0x3ffccccccccccccdULL);
    CHECK(testkit::same_bits(convert_temperature(1, 100.0), 100.0 * k18 + 32.0));
    CHECK(testkit::same_bits(convert_temperature(2, 212.0), (212.0 - 32.0) / k18));
    // Mode 3 follows the user's factor: 1.8 -> C->F, 0.5556 -> F->C, anything else unchanged.
    g_units.temp_factor = k18;
    CHECK(testkit::same_bits(convert_temperature(3, 10.0), 10.0 * k18 + 32.0));
    g_units.temp_factor = std::bit_cast<double>(0x3fe1c71c71cd3832ULL);
    CHECK(testkit::same_bits(convert_temperature(3, 50.0), (50.0 - 32.0) / k18));
    g_units.temp_factor = 1.0;
    CHECK(testkit::same_bits(convert_temperature(3, 50.0), 50.0));
}

TEST_CASE(min_max_of_a_column)
{
    // a(ld=3, 4 columns), column 2 is 5, -1, 7, 2
    const double a[12] = {0, 5, 0, 0, -1, 0, 0, 7, 0, 0, 2, 0};
    const std::int32_t ld = 3, n = 4, col = 2, init = 1;
    double lo = 0, hi = 0;
    column_min_max(a, &ld, &n, &col, &lo, &hi, &init);
    CHECK_EQ(lo, -1.0);
    CHECK_EQ(hi, 7.0);
    // Without init the bounds carry over.
    const std::int32_t no = 0;
    lo = -5;
    column_min_max(a, &ld, &n, &col, &lo, &hi, &no);
    CHECK_EQ(lo, -5.0);
}

TEST_CASE(vectors_and_rotation)
{
    double v[3] = {3, 0, 4};
    CHECK_EQ(vec3_normalize(v), 5.0);
    CHECK(testkit::same_bits(v[0], 3.0 / 5.0));
    double z[3] = {0, 0, 0};
    CHECK_EQ(vec3_normalize(z), 0.0);
    const double p[2] = {2, 1}, c[2] = {1, 1}, ang = 0.0;
    double out[2];
    rotate2d(p, c, &ang, out);
    CHECK_EQ(out[0], 2.0);
    CHECK_EQ(out[1], 1.0);
}

TEST_CASE(degrees_flagged_records)
{
    double r[12] = {2, 0, 0, 0, 0, 180, 1, 0, 0, 0, 0, 90};
    const std::int32_t n = 2;
    degrees_to_radians_if_flagged(r, &n);
    CHECK(testkit::same_bits(r[5], 180.0 * std::bit_cast<double>(0x3f91df46a2529d39ULL)));
    CHECK_EQ(r[11], 0.0);
}

TEST_CASE(string_length_plus_one)
{
    std::int32_t len = 8;
    last_nonblank_plus1(&len, "ab c    ");
    CHECK_EQ(len, 5);
    len = 4;
    last_nonblank_plus1(&len, "    ");
    CHECK_EQ(len, 1);
}

int main() { return testkit::run_all(); }
