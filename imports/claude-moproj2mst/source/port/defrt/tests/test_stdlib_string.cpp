// Tier-1: def::rt::Msvc9String, the VS2008 std::string layout and member functions.
// ctest-labels: p1-01 string
//
// Expected capacities are worked out by hand from _Copy (140004eb0):
//   newres = newsize | 15; if newres / 3 < res / 2 (and no overflow) newres = res + res / 2.
#include <defrt/stdlib/msvc9_string.hpp>
#include <testkit/check.hpp>

#include <cstring>
#include <stdexcept>
#include <string>

using def::rt::Msvc9String;
using def::rt::Msvc9StringLayout;

TEST_CASE(layout_matches_vs2008)
{
    CHECK_EQ(sizeof(Msvc9String), std::size_t{0x28});
    CHECK_EQ(Msvc9StringLayout::aux, std::size_t{0x00});
    CHECK_EQ(Msvc9StringLayout::bx, std::size_t{0x08});
    CHECK_EQ(Msvc9StringLayout::size, std::size_t{0x18});
    CHECK_EQ(Msvc9StringLayout::res, std::size_t{0x20});
}

TEST_CASE(default_and_small_strings_stay_inline)
{
    Msvc9String s; // 140005040: res 15, size 0, buf[0] 0
    CHECK_EQ(s.size(), std::size_t{0});
    CHECK_EQ(s.capacity(), std::size_t{15});
    CHECK_EQ(std::string(s.c_str()), std::string(""));
    const auto* base = reinterpret_cast<const char*>(&s);
    CHECK(s.c_str() == base + 8);

    Msvc9String t("Isothermal 12345"); // 16 characters: one more than the buffer holds
    CHECK_EQ(t.size(), std::size_t{16});
    CHECK_EQ(t.capacity(), std::size_t{31}); // 16 | 15
    CHECK_EQ(t.str(), std::string("Isothermal 12345"));

    Msvc9String u("Heat transfer"); // 13: inline
    CHECK_EQ(u.capacity(), std::size_t{15});
    CHECK(u.c_str() == reinterpret_cast<const char*>(&u) + 8);
}

TEST_CASE(growth_follows_copy)
{
    Msvc9String s;
    s.assign(std::string(20, 'a').c_str(), 20);
    CHECK_EQ(s.capacity(), std::size_t{31}); // 31/3 = 10 >= 15/2 = 7
    s.assign(std::string(40, 'b').c_str(), 40);
    CHECK_EQ(s.capacity(), std::size_t{47}); // 47/3 = 15 >= 31/2 = 15
    s.assign(std::string(48, 'c').c_str(), 48);
    CHECK_EQ(s.capacity(), std::size_t{70}); // 63/3 = 21 < 47/2 = 23: 47 + 23
    s.assign(std::string(71, 'd').c_str(), 71);
    CHECK_EQ(s.capacity(), std::size_t{105}); // 79/3 = 26 < 35: 70 + 35
    CHECK_EQ(s.size(), std::size_t{71});
    CHECK_EQ(s.str(), std::string(71, 'd'));
    s.assign("x", 1); // shrinking never reallocates
    CHECK_EQ(s.capacity(), std::size_t{105});
    CHECK_EQ(s.str(), std::string("x"));
}

TEST_CASE(assign_variants)
{
    Msvc9String s(5, 'z'); // 1401b3730
    CHECK_EQ(s.str(), std::string("zzzzz"));
    s.assign(1, 'q');
    CHECK_EQ(s.str(), std::string("q"));
    s.assign(0, 'q');
    CHECK_EQ(s.size(), std::size_t{0});

    Msvc9String a("abcdefgh");
    Msvc9String b;
    b.assign(a, 2, 3);
    CHECK_EQ(b.str(), std::string("cde"));
    b.assign(a, 6, Msvc9String::npos); // count clipped to the rest
    CHECK_EQ(b.str(), std::string("gh"));
    b.assign(a);
    CHECK_EQ(b.str(), std::string("abcdefgh"));
    Msvc9String c(b);
    CHECK_EQ(c.str(), std::string("abcdefgh"));
    bool threw = false;
    try {
        b.assign(a, 9, 1); // off > size: _Xran
    } catch (const std::out_of_range&) {
        threw = true;
    }
    CHECK(threw);
}

TEST_CASE(assign_from_own_characters)
{
    Msvc9String s("abcdefgh");
    s.assign(s.c_str() + 2, 3); // _Inside: assign(*this, 2, 3) = erase(5), erase(0, 2)
    CHECK_EQ(s.str(), std::string("cde"));
    Msvc9String t("0123456789abcdefghij"); // heap
    t.assign(t, 4, 100);
    CHECK_EQ(t.str(), std::string("456789abcdefghij"));
    CHECK_EQ(t.capacity(), std::size_t{31}); // erase keeps the storage
}

TEST_CASE(erase_and_grow_trim)
{
    Msvc9String s("0123456789");
    s.erase(2, 3);
    CHECK_EQ(s.str(), std::string("0156789"));
    s.erase(4);
    CHECK_EQ(s.str(), std::string("0156"));

    Msvc9String h("0123456789abcdefghij"); // res 31
    CHECK(h.grow(5, true));                 // trim to the buffer, keeping 5 characters
    CHECK_EQ(h.capacity(), std::size_t{15});
    CHECK_EQ(h.str(), std::string("01234"));
    CHECK(!h.grow(0, false));
    CHECK_EQ(h.size(), std::size_t{0});
}

int main() { return testkit::run_all(); }
