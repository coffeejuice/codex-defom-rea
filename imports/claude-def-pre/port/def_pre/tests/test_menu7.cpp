// Tier 1 for the console and string helpers of menu 7 (packet p4-01): trim 1401b1060, is_digits
// 1401b1390, to_int 1401b0870, starts_with 1401b0940, ends_with 1401b0cc0, read_line 140009c50
// and read_int 140008600 with scripted std::cin. Expected values follow from the recovered code.
// ctest-labels: p4-01 def_pre
#include <def_pre/menu7.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

using namespace def::pre;

namespace {

// Feeds std::cin from a string for the lifetime of the object.
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

// The helpers take the main object but do not read it.
def::model::Cls_14001c770& no_main()
{
    return *reinterpret_cast<def::model::Cls_14001c770*>(std::uintptr_t{0x10});
}

} // namespace

TEST_CASE(trim_blanks_and_question_marks)
{
    CHECK_EQ(trim("  abc ?\t"), std::string("abc"));
    CHECK_EQ(trim("?x?"), std::string("x"));
    CHECK_EQ(trim(""), std::string(""));
    CHECK_EQ(trim("   "), std::string("   ")); // all blank: first 0, last n-1
    CHECK_EQ(trim("a b"), std::string("a b"));
}

TEST_CASE(digits_and_numbers)
{
    CHECK(is_digits("0123", 0));
    CHECK(!is_digits("12a", 0));
    CHECK(!is_digits("-1", 0));
    CHECK(is_digits("", 0));
    CHECK(is_digits("x12y", 1)); // range [1, n-1)
    CHECK_EQ(to_int("42"), 42);
    CHECK_EQ(to_int("abc"), 0);
    CHECK_EQ(to_int(" 7x"), 7);
}

TEST_CASE(prefix_and_suffix)
{
    CHECK(starts_with("No", "n", false));
    CHECK(starts_with("nope", "n", false));
    CHECK(!starts_with("No", "n", true));
    CHECK(!starts_with("", "n", false));
    CHECK(starts_with("ABC", "ab", false));
    CHECK(ends_with("RING.DB", ".DB"));
    CHECK(ends_with("ring.db", ".DB"));
    CHECK(!ends_with(".DB", ".DB")); // must be strictly longer
    CHECK(!ends_with("RING", ".DB"));
}

TEST_CASE(read_line_trims)
{
    Input in("  RING.DB ?\n   \nlast");
    CHECK_EQ(read_line(no_main()), std::string("RING.DB"));
    CHECK_EQ(read_line(no_main()), std::string("")); // a blank line
    CHECK_EQ(read_line(no_main()), std::string("last"));
}

TEST_CASE(read_int_default_range_and_text)
{
    char prompt[208];
    format200(prompt, " DB Generation Option =      %d ? ", 2);
    CHECK_EQ(std::string(prompt + 0x17), std::string("      2 ? ")); // the default after "="
    {
        Input in("\n");
        CHECK_EQ(read_int(no_main(), 0, 2, prompt, true), 2);
    }
    {
        Input in("1\n");
        CHECK_EQ(read_int(no_main(), 0, 2, prompt, true), 1);
    }
    {
        Input in("7\n0\n"); // out of range: asks again
        CHECK_EQ(read_int(no_main(), 0, 2, prompt, true), 0);
    }
    {
        Input in("x\n");
        CHECK_EQ(read_int(no_main(), 0, 2, prompt, true), -0xf423d);
    }
}

int main() { return testkit::run_all(); }
