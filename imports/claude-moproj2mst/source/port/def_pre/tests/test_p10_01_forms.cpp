// Tier 1 for the form readers of menus 3-6 (packet p10-01): field_of 1401b1240, is_number
// 1401b1450, to_double 1401b08d0, read_doubles 140007a80, read_double 140008030,
// read_double_range 140008220, dynary_is_constant 14012fa90. Expected values follow from the
// recovered code (and agree with the MENU3/MENU4 oracle sessions).
// ctest-labels: p10-01 def_pre
#define _CRT_SECURE_NO_WARNINGS
#include <def_pre/forms.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

#include <defmodel/layout/dynary.hpp>
#include <defrt/stdlib/stdio_compat.hpp>

using namespace def::pre;

namespace {

struct Input {
    std::istringstream in;
    std::streambuf* old;
    explicit Input(const std::string& s) : in(s), old(std::cin.rdbuf(in.rdbuf())) { std::cin.clear(); }
    ~Input()
    {
        std::cin.rdbuf(old);
        std::cin.clear();
    }
};

} // namespace

TEST_CASE(field_of_keeps_the_delimiter_of_field_zero)
{
    CHECK_EQ(field_of("1 2", ' ', 0), std::string("1 "));
    CHECK_EQ(field_of("1 2", ' ', 1), std::string("2"));
    CHECK_EQ(field_of("5", ' ', 0), std::string("5"));
    CHECK_EQ(field_of("5", ' ', 1), std::string("5")); // no delimiter: the whole string
    CHECK_EQ(field_of("a b c", ' ', 1), std::string("b "));
    CHECK_EQ(field_of("", ' ', 0), std::string(""));
}

TEST_CASE(is_number_digits_minus_dot)
{
    CHECK(is_number("", 0));
    CHECK(is_number("-1.5", 0));
    CHECK(!is_number("1e5", 0));
    CHECK(!is_number("+1", 0));
    CHECK(!is_number("1 ", 0));
}

TEST_CASE(to_double_defaults_to_zero)
{
    CHECK_EQ(to_double("2.5"), 2.5);
    CHECK_EQ(to_double(""), 0.0);
    CHECK_EQ(to_double("abc"), 0.0);
}

TEST_CASE(read_doubles_quirks)
{
    double v[3] = {7.0, 8.0, 9.0};
    {
        Input in("4\n");
        read_doubles(nullptr, v, 3); // one integer fills every slot
    }
    CHECK_EQ(v[0], 4.0);
    CHECK_EQ(v[2], 4.0);
    {
        Input in("1 2 3\n");
        read_doubles(nullptr, v, 3); // field 0 is "1 ": not digits, nothing stored
    }
    CHECK_EQ(v[1], 4.0);
    {
        Input in("0.5\n");
        read_doubles(nullptr, v, 3); // '.' is not a digit
    }
    CHECK_EQ(v[0], 4.0);
    {
        Input in("\n");
        read_doubles(nullptr, v, 2); // empty: digits (vacuously), 0.0 everywhere asked
    }
    CHECK_EQ(v[0], 0.0);
    CHECK_EQ(v[1], 0.0);
    CHECK_EQ(v[2], 4.0);
}

TEST_CASE(read_double_and_range)
{
    def::rt::init_stdio_compat();
    {
        Input in("abc\n");
        CHECK_EQ(read_double(nullptr), kNoValue);
    }
    {
        Input in("  -2.5 \n");
        CHECK_EQ(read_double(nullptr), -2.5);
    }
    {
        Input in("\n");
        CHECK_EQ(read_double_range(nullptr, 0.0, 1.0, ""), 0.0); // empty answer: 0
    }
    {
        Input in("5\n0.25\n");
        CHECK_EQ(read_double_range(nullptr, 0.0, 1.0, ""), 0.25); // 5 out of range, asked again
    }
    {
        Input in("1e3\n");
        CHECK_EQ(read_double_range(nullptr, 0.0, 1e30, ""), kNoValue);
    }
}

TEST_CASE(dynary_constant_test)
{
    def::model::DYNARY d;
    d.ndim = 0;
    CHECK(dynary_is_constant(d));
    d.ndim = 1;
    CHECK(!dynary_is_constant(d));
    d.ndim = 0;
}

int main() { return testkit::run_all(); }
