// ctest-labels: key-common
// Tier-1 tests of the KEY line helpers (140413900-140416aa0) and crt::scan_format. Expected
// values follow from the decompiled code (zero outputs, fgets 0x400, ',' -> ' ', one sscanf,
// -1 -> 1) and the C/MSVC scanf rules, worked out by hand in each case.
#include "key_test_util.hpp"

#include <defkey/crt_scan.hpp>
#include <defkey/globals_key_common.hpp>
#include <defkey/line_io.hpp>
#include <testkit/check.hpp>

#include <cstring>
#include <string>

using namespace def::key;
using keytest::InFile;

TEST_CASE(scan_format_drops_far_prefix)
{
    CHECK_EQ(crt::scan_format("%Flf"), std::string("%lf"));
    CHECK_EQ(crt::scan_format("%i %Flf %Flf"), std::string("%i %lf %lf"));
    CHECK_EQ(crt::scan_format("%*Fs %39s %%F"), std::string("%*s %39s %%F"));
    CHECK_EQ(crt::scan_format("%[F,]%Nd"), std::string("%[F,]%d"));
    CHECK_EQ(crt::scan_format("%I64d %Fhd"), std::string("%I64d %hd"));
    double d = -1.0;
    CHECK_EQ(crt::sscanf("1.5", "%Flf", &d), 1);
    CHECK_BITS(d, 1.5);
}

TEST_CASE(int_formats_and_bases)
{
    InFile in("t_li_ints.txt", "  010 0x10 -7\n12,34\n\n5\n");
    std::int32_t a = -1, b = -1, c = -1;
    int n = -9;
    CHECK_EQ(read_iii(in.get(), &a, &b, &c, &n), 0);
    CHECK_EQ(a, 8);   // %i: leading 0 is octal
    CHECK_EQ(b, 16);  // 0x is hex
    CHECK_EQ(c, -7);
    CHECK_EQ(n, 3);
    CHECK_EQ(read_ii(in.get(), &a, &b), 0); // comma separates
    CHECK_EQ(a, 12);
    CHECK_EQ(b, 34);
    CHECK_EQ(read_i(in.get(), &a, true), 0); // one blank line skipped
    CHECK_EQ(a, 5);
    CHECK_EQ(read_i(in.get(), &a), 1); // EOF
    CHECK_EQ(a, 0);                    // zeroed before the read
}

TEST_CASE(blank_short_and_bad_lines)
{
    InFile in("t_li_blank.txt", "\n   \t\n7\nabc 5\n3 x 4\n");
    std::int32_t a = 9, b = 9, c = 9;
    int n = 9;
    CHECK_EQ(read_iii(in.get(), &a, &b, &c, &n), 1); // "\n": sscanf -1
    CHECK_EQ(n, -1);
    CHECK_EQ(read_ii(in.get(), &a, &b), 1); // whitespace only: -1
    CHECK_EQ(read_iii(in.get(), &a, &b, &c, &n), 0); // short line: rest stays 0
    CHECK_EQ(a, 7);
    CHECK_EQ(b, 0);
    CHECK_EQ(c, 0);
    CHECK_EQ(n, 1);
    CHECK_EQ(read_ii(in.get(), &a, &b), 0); // non-numeric first token: 0 conversions, success
    CHECK_EQ(a, 0);
    CHECK_EQ(b, 0);
    CHECK_EQ(read_iii(in.get(), &a, &b, &c, &n), 0); // stops at "x"
    CHECK_EQ(a, 3);
    CHECK_EQ(b, 0);
    CHECK_EQ(n, 1);
}

TEST_CASE(skip_blank_lines_repeat)
{
    InFile in("t_li_skip.txt", "\n\n\n1 2 3 4 5\n\n\n6\n");
    std::int32_t v[5];
    CHECK_EQ(read_i5(in.get(), &v[0], &v[1], &v[2], &v[3], &v[4], true), 0);
    CHECK_EQ(v[4], 5);
    std::int32_t a = 0;
    // read_i skips only one blank line: the second blank line is the value line, sscanf -1 -> 1
    CHECK_EQ(read_i(in.get(), &a, true), 1);
    CHECK_EQ(read_i(in.get(), &a, false), 0);
    CHECK_EQ(a, 6);
}

TEST_CASE(double_formats)
{
    InFile in("t_li_dbl.txt", "1.5E+002 -2.5e-3 7\n1,2\n4.0\n4 5\n0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9\n");
    double a = 0, b = 0, c = 0;
    CHECK_EQ(read_d3(in.get(), &a, &b, &c), 0);
    CHECK_BITS(a, 150.0);
    CHECK_BITS(b, -2.5e-3);
    CHECK_BITS(c, 7.0);
    CHECK_EQ(read_dd(in.get(), &a, &b), 0);
    CHECK_BITS(a, 1.0);
    CHECK_BITS(b, 2.0);
    // 140414d10: exactly one field resets all three
    CHECK_EQ(read_d3_not_one(in.get(), &a, &b, &c), 0);
    CHECK_BITS(a, 0.0);
    CHECK_EQ(read_d3_not_one(in.get(), &a, &b, &c), 0);
    CHECK_BITS(a, 4.0);
    CHECK_BITS(b, 5.0);
    CHECK_BITS(c, 0.0);
    double v[9];
    double* const p[9] = {&v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8]};
    CHECK_EQ(read_d9(in.get(), p), 0);
    CHECK_BITS(v[8], 0.9);
}

TEST_CASE(mixed_formats)
{
    InFile in("t_li_mix.txt", "3 2.5\n1 2 3 4.5 6\n2.5 7\n1 2 3 4.5 9\n");
    std::int32_t i = 0, j = 0, k = 0, m = 0;
    double d = 0, e = 0;
    CHECK_EQ(read_i_d(in.get(), &i, &d), 0);
    CHECK_EQ(i, 3);
    CHECK_BITS(d, 2.5);
    CHECK_EQ(read_iii_dd(in.get(), &i, &j, &k, &d, &e), 0);
    CHECK_EQ(k, 3);
    CHECK_BITS(d, 4.5);
    CHECK_BITS(e, 6.0);
    CHECK_EQ(read_d_i(in.get(), &d, &i), 0);
    CHECK_BITS(d, 2.5);
    CHECK_EQ(i, 7);
    CHECK_EQ(read_iii_d_i(in.get(), &i, &j, &k, &d, &m), 0);
    CHECK_EQ(m, 9);
}

TEST_CASE(int_reads_a_double_token_partially)
{
    // "%i" stops at '.', the next "%i" fails on ".5": one conversion.
    InFile in("t_li_part.txt", "2.5 7\n");
    std::int32_t a = 0, b = 0;
    CHECK_EQ(read_ii(in.get(), &a, &b), 0);
    CHECK_EQ(a, 2);
    CHECK_EQ(b, 0);
}

TEST_CASE(read_i10_copies_min_n)
{
    InFile in("t_li_i10.txt", "1 2 3 4 5 6 7 8 9 10 11\n\n");
    std::int32_t dst[12];
    for (std::int32_t& x : dst) x = -1;
    int n = 12;
    CHECK_EQ(read_i10(in.get(), dst, &n), 0);
    CHECK_EQ(dst[9], 10);
    CHECK_EQ(dst[10], -1); // at most 10 copied
    CHECK_EQ(n, 12);
    n = 3;
    CHECK_EQ(read_i10(in.get(), dst, &n), 0); // blank line still returns 0
    CHECK_EQ(dst[0], 0);
    CHECK_EQ(dst[3], 4); // untouched
    CHECK_EQ(read_i10(in.get(), dst, &n), 1);
}

TEST_CASE(line_overflow_splits)
{
    // A 1100-character line: fgets(0x400) takes 1023 characters, the rest is the next "line".
    std::string longline(1020, ' ');
    longline += "123456789\n";
    InFile in("t_li_long.txt", longline);
    std::int32_t a = -1;
    CHECK_EQ(read_i(in.get(), &a), 0);
    CHECK_EQ(a, 123); // characters 1020..1022 are "123"
    CHECK_EQ(read_i(in.get(), &a), 0);
    CHECK_EQ(a, 456789);
}

TEST_CASE(strings)
{
    InFile in("t_li_str.txt", "rest of keyword line\n  My title \t \r\nexactly\n");
    char s[16];
    std::memset(s, 'x', sizeof s);
    CHECK_EQ(read_string_next_line(in.get(), s, 16), 0);
    // text mode turns "\r\n" into "\n"; cut there, strncpy 15 of "  My title \t " (NUL-padded),
    // then trim trailing NUL/space/\n/\r from s[14] down: the tab stays
    CHECK_EQ(std::string(s), std::string("  My title \t"));
    char t[5];
    CHECK_EQ(read_string_line(in.get(), t, 5), 0); // 4 chars copied, s[4] cut point
    CHECK_EQ(std::string(t), std::string("exac"));
    CHECK_EQ(read_string_line(in.get(), t, 5), 1);
}

TEST_CASE(tokenizer)
{
    reset_globals();
    InFile in("t_li_tok.txt", "a, b\tc  d\n\nx y z\n");
    int cnt = 0;
    CHECK_EQ(read_tokens(in.get(), &cnt, 10), 0);
    CHECK_EQ(cnt, 4);
    CHECK_EQ(std::string(token(2)), std::string("c"));
    CHECK_EQ(std::string(token(4)), std::string(""));
    CHECK_EQ(std::string(token(-1)), std::string(""));
    CHECK_EQ(std::string(token(128)), std::string(""));
    CHECK_EQ(read_tokens(in.get(), &cnt, 10), 0); // blank line: slot 0 counted, null
    CHECK_EQ(cnt, 1);
    CHECK_EQ(std::string(token(0)), std::string(""));
    CHECK_EQ(read_tokens(in.get(), &cnt, 2), 0); // max 2: only two slots used
    CHECK_EQ(cnt, 2);
    CHECK_EQ(std::string(token(1)), std::string("y"));
    CHECK_EQ(read_tokens(in.get(), &cnt, 2), 1);
}

TEST_CASE(token_conversions_are_decimal)
{
    reset_globals();
    InFile in("t_li_conv.txt", "010 0x10 2.5e1\n");
    int cnt = 0;
    read_tokens(in.get(), &cnt, 5);
    CHECK_EQ(token_int(0), 10); // strtol base 10, unlike %i
    CHECK_EQ(token_int(1), 0);
    CHECK_BITS(token_double(2), 25.0);
    CHECK_EQ(token_int(5), 0); // "" -> 0
}

TEST_CASE(replace_char_all)
{
    char s[] = "1,2,,3";
    replace_char(s, ',', ' ');
    CHECK_EQ(std::string(s), std::string("1 2  3"));
}

int main() { return testkit::run_all(); }
