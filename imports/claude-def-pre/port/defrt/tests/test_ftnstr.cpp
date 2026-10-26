// ctest-labels: p1-10 ftnstr
// Expected values are hand-traced from the decompiles of FUN_1403239f0, FUN_140323b90 and
// FUN_140323bf0 (docs/re/status/p1-10.tsv).
#include <defrt/ftnstr/ftnstr.hpp>
#include <testkit/check.hpp>

#include <cstring>

using def::rt::blank_pad;
using def::rt::to_fortran;
using def::rt::trim_fortran;

TEST_CASE(trim_cuts_trailing_blanks_tabs_crlf_and_nuls)
{
    char s[16] = "AB C \t\r\n";
    s[8] = '\0';
    s[9] = ' ';
    trim_fortran(s, 10);
    CHECK(std::strcmp(s, "AB C") == 0);
}

TEST_CASE(trim_full_field_writes_terminator_at_n)
{
    char s[8] = {'A', 'B', 'C', 'D', 'X', 'X', 'X', 'X'};
    trim_fortran(s, 4); // last char kept: NUL at s[4]
    CHECK(std::strcmp(s, "ABCD") == 0);
}

TEST_CASE(trim_all_blank_field_keeps_first_char)
{
    // loop stops at k = 0 with end = 1: NUL at s[1], s[0] stays ' '
    char s[8] = "    ";
    trim_fortran(s, 4);
    CHECK(std::strcmp(s, " ") == 0);
    char t[4] = {'\0', 'Q', 'Q', 'Q'};
    trim_fortran(t, 1); // n = 1, s[0] is a NUL: end = 1, cut = 1
    CHECK_EQ(t[0], '\0');
    CHECK_EQ(t[1], '\0');
}

TEST_CASE(trim_nonpositive_length_writes_s0)
{
    char s[4] = {'A', 'B', 'C', 0};
    trim_fortran(s, 0);
    CHECK_EQ(s[0], '\0');
    char t[4] = {'A', 'B', 'C', 0};
    trim_fortran(t + 1, -3);
    CHECK_EQ(t[1], '\0');
    CHECK_EQ(t[0], 'A');
}

TEST_CASE(blank_pad_pads_without_terminator)
{
    char s[8] = {'A', 'B', 0, 'x', 'x', 'x', 'x', 'x'};
    blank_pad(s, 5);
    CHECK(std::memcmp(s, "AB   xxx", 8) == 0);
    char t[8] = "ABCDEF";
    blank_pad(t, 3); // strlen >= n: unchanged
    CHECK(std::strcmp(t, "ABCDEF") == 0);
}

TEST_CASE(to_fortran_copies_pads_and_returns_source_length)
{
    char d[8];
    std::memset(d, 'x', sizeof d);
    CHECK_EQ(to_fortran(d, "AB", 6), 2u);
    CHECK(std::memcmp(d, "AB    xx", 8) == 0);

    std::memset(d, 'x', sizeof d);
    CHECK_EQ(to_fortran(d, "ABCDEFGHIJ", 4), 10u); // truncated, no terminator, no padding
    CHECK(std::memcmp(d, "ABCDxxxx", 8) == 0);

    std::memset(d, 'x', sizeof d);
    CHECK_EQ(to_fortran(d, "", 3), 0u);
    CHECK(std::memcmp(d, "   xxxxx", 8) == 0);
}

int main() { return testkit::run_all(); }
