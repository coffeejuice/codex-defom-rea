// FUN_140235c30 set_unit_system and the conversion tables it rebuilds (1403f8b70, 1403f9f20).
// Expected values: the branches of the decompile and the static records read from the binary
// (port/defrt/scripts/gen_unit_tables.py), worked out by hand here.
// ctest-labels: units kw-merge
#include <cstring>
#include <string>

#include <defrt/ftn/unit_system.hpp>
#include <defrt/ftn/units.hpp>
#include <testkit/check.hpp>

using namespace def::rt::ftn;

TEST_CASE(flag_by_unit_code)
{
    set_unit_system(2);
    CHECK_EQ(g_units.unit_flag, 1);
    CHECK_EQ(g_conv.english, 1);
    CHECK_EQ(unit_system(), 2);
    set_unit_system(1);
    CHECK_EQ(g_units.unit_flag, 0);
    CHECK_EQ(g_conv.english, 0);
    CHECK_EQ(unit_system(), 1);
    set_unit_system(7);  // any other code: SI
    CHECK_EQ(g_units.unit_flag, 0);
    CHECK_EQ(g_conv.english, 0);
}

TEST_CASE(tables_rebuilt)
{
    set_unit_system(1);
    CHECK_EQ(g_conv.count, 28);
    CHECK_EQ(g_conv.u8c, 6000);
    CHECK_EQ(g_conv.u90, 4);
    CHECK_EQ(g_conv.u94, 0);
    REQUIRE(g_conv.factors != nullptr);
    REQUIRE(g_conv.units != nullptr);
    CHECK_EQ(std::string(g_conv.factors[1].name), std::string("Length"));
    CHECK(testkit::same_bits(g_conv.factors[1].f[1], 0.03937008));
    CHECK(testkit::same_bits(g_conv.factors[1].f[2], 25.4));
    CHECK(testkit::same_bits(g_conv.factors[1].f[3], 1.0));  // +0x80 overwritten from +0x68
    // SI: labels 1 and 4 become label 3 ("mm"); English: label 2 ("in")
    CHECK_EQ(std::string(g_conv.units[1].label[1]), std::string("mm"));
    CHECK_EQ(std::string(g_conv.units[1].label[4]), std::string("mm"));
    set_unit_system(2);
    CHECK_EQ(std::string(g_conv.units[1].label[1]), std::string("in"));
    CHECK_EQ(std::string(g_conv.units[1].label[4]), std::string("in"));
    CHECK_EQ(std::string(g_conv.units[1].label[0]), std::string("Length"));
}

int main() { return testkit::run_all(); }
