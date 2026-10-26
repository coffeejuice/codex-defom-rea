// Tier-1: std::string helpers 1401b0870 / 1401b0940 and the path wrapper 1401b19c0 (stem
// 1401b1a30). Expected values worked out by hand from the disassembly (str.asm of p8-05).
// ctest-labels: p8-05 str
#include <defrt/stdlib/str_helpers.hpp>
#include <testkit/check.hpp>

#include <string>

using def::rt::Cls_1401b19c0;

TEST_CASE(str_to_int_values)
{
    CHECK_EQ(def::rt::str_to_int("5"), 5);
    CHECK_EQ(def::rt::str_to_int("-12x"), -12);
    CHECK_EQ(def::rt::str_to_int(""), 0);   // sscanf fails, v stays 0
    CHECK_EQ(def::rt::str_to_int("_3"), 0);
    CHECK_EQ(def::rt::str_to_int(" 7"), 7);
}

TEST_CASE(str_starts_with_exact_and_folded)
{
    CHECK(def::rt::str_starts_with("HAMMER_SI5", "HAMMER_SI", true));
    CHECK(!def::rt::str_starts_with("hammer_si5", "HAMMER_SI", true));
    CHECK(def::rt::str_starts_with("hammer_si5", "HAMMER_SI", false));
    CHECK(def::rt::str_starts_with("ABC", "abc", false));
    CHECK(!def::rt::str_starts_with("AB", "abc", false)); // shorter: NUL vs 'c'
    CHECK(!def::rt::str_starts_with("AB", "abc", true));
    CHECK(def::rt::str_starts_with("anything", "", true));
    CHECK(def::rt::str_starts_with("[x", "{x", false) == false); // only A-Z fold
}

TEST_CASE(str_trim_values)
{
    CHECK(def::rt::str_trim("  ab ?") == "ab");
    CHECK(def::rt::str_trim("\tx y\r\n") == "x y");
    CHECK(def::rt::str_trim("   ") == "   "); // all blank: first stays 0, last n - 1 (1401b10b3/10c2)
    CHECK(def::rt::str_trim("") == "");
}

TEST_CASE(path_stem)
{
    CHECK(Cls_1401b19c0("HAM5.DB").stem() == "HAM5");
    CHECK(Cls_1401b19c0("C:\\a\\b\\HAM5.DB").stem() == "HAM5");
    CHECK(Cls_1401b19c0("d/x.y.z").stem() == "x.y");
    CHECK(Cls_1401b19c0("NOEXT").stem() == "NOEXT");
    CHECK(Cls_1401b19c0("dir\\NOEXT").stem() == "NOEXT");
    CHECK(Cls_1401b19c0(".DB").stem() == "");
    CHECK(Cls_1401b19c0("").stem() == "");
    CHECK(Cls_1401b19c0("a.b\\c").stem() == "a"); // last '.' is before the last '\'
}

int main() { return testkit::run_all(); }
