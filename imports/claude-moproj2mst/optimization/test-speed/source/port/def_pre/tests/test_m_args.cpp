// Tier 1: command-line parser FUN_14001b7b0 and quote stripping FUN_14001b700.
// Expected values follow from the recovered code and agree with the oracle cases in
// harness/mcli/mcli_cases.py (quote-o: '"q.txt"' -> "q.txtt", t-quoted: '"2"' -> 22).
// ctest-labels: m-cli def_pre
#define _CRT_SECURE_NO_WARNINGS
#include <def_pre/m_cli.hpp>
#include <testkit/check.hpp>

#include <cstring>
#include <string>
#include <vector>

using namespace def::pre;

namespace {

struct Crash {
    std::uint32_t status;
};
void throw_crash(std::uint32_t status) { throw Crash{status}; }

// argv with modifiable strings and the terminating NULL.
struct Argv {
    std::vector<std::string> s;
    std::vector<char*> p;
    explicit Argv(std::vector<std::string> args) : s(std::move(args))
    {
        s.insert(s.begin(), "DEF_PRE_64.exe");
        for (auto& x : s) p.push_back(x.data());
        p.push_back(nullptr);
    }
    int argc() const { return static_cast<int>(s.size()); }
    char** argv() { return p.data(); }
};

std::uint32_t parse(std::vector<std::string> args, MArgs& a)
{
    Argv v(std::move(args));
    return parse_m_args(v.argc(), v.argv(), a);
}

std::uint32_t crash_of(std::vector<std::string> args)
{
    set_abnormal_exit_hook(&throw_crash);
    MArgs a;
    try {
        parse(std::move(args), a);
    } catch (const Crash& c) {
        set_abnormal_exit_hook(nullptr);
        return c.status;
    }
    set_abnormal_exit_hook(nullptr);
    return 0;
}

std::string stripped(const char* in)
{
    char buf[64];
    std::strcpy(buf, in);
    strip_quotes(buf);
    return buf;
}

} // namespace

TEST_CASE(modes_and_flags)
{
    MArgs a;
    CHECK_EQ(parse({"-m", "header", "-d", "a.DB", "-o", "h.txt"}, a), kFlagOut | kFlagDb);
    CHECK_EQ(a.mode, 1);
    CHECK_EQ(std::string(a.db), std::string("a.DB"));
    CHECK_EQ(std::string(a.out), std::string("h.txt"));

    const char* names[] = {"steplist", "stepList", "lastStepNo", "simsteplist", "simStepList",
                           "diff", "reclist", "summary"};
    const int modes[] = {2, 2, 3, 4, 4, 5, 6, 7};
    for (int i = 0; i < 8; ++i) {
        MArgs b;
        CHECK_EQ(parse({"-m", names[i]}, b), 0u); // -m alone sets no flag
        CHECK_EQ(b.mode, modes[i]);
    }
    MArgs c;
    parse({"-m", "HEADER"}, c);
    CHECK_EQ(c.mode, 0);
    MArgs d;
    parse({"-m", "stepList", "-m", "bogus"}, d); // unknown keeps the earlier mode
    CHECK_EQ(d.mode, 2);
    MArgs e;
    parse({"-m", "header", "-m", "lastStepNo"}, e);
    CHECK_EQ(e.mode, 3);
}

TEST_CASE(attached_values_and_letters)
{
    MArgs a;
    CHECK_EQ(parse({"-mheader", "-dX.DB", "-oy.txt"}, a), kFlagOut | kFlagDb);
    CHECK_EQ(a.mode, 1);
    CHECK_EQ(std::string(a.db), std::string("X.DB"));
    CHECK_EQ(std::string(a.out), std::string("y.txt"));
    MArgs b;
    CHECK_EQ(parse({"-M", "lastStepNo", "-D", "a", "-O", "o"}, b), kFlagOut | kFlagDb);
    CHECK_EQ(b.mode, 3);
}

TEST_CASE(second_d_s_r_t)
{
    MArgs a;
    const std::uint32_t f =
        parse({"-d", "one", "-d", "two", "-s", "3", "-s", "4", "-r", "7", "-T", "2"}, a);
    CHECK_EQ(f, kFlagDb | kFlagDb2 | kFlagS1 | kFlagS2 | kFlagR | kFlagT);
    CHECK_EQ(std::string(a.db), std::string("one"));
    CHECK_EQ(std::string(a.db2), std::string("two"));
    CHECK_EQ(a.s1, 3);
    CHECK_EQ(a.s2, 4);
    CHECK_EQ(a.r, 7);
    CHECK_EQ(a.t, 2);
    MArgs b;
    parse({"-o", "a", "-o", "b"}, b);
    CHECK_EQ(std::string(b.out), std::string("b"));
}

TEST_CASE(parsing_stops_and_skips)
{
    MArgs a;
    CHECK_EQ(parse({"x", "-d", "a"}, a), 0u); // first non-option ends the loop
    MArgs b;
    CHECK_EQ(parse({"-x", "-", "-d", "a"}, b), kFlagDb); // unknown letters and "-" skipped
    MArgs c;
    CHECK_EQ(parse({"-x", "-q", "9", "-d", "a"}, c), 0u); // "9" ends the loop
}

TEST_CASE(quote_stripping)
{
    CHECK_EQ(stripped("abc"), std::string("abc"));
    CHECK_EQ(stripped("abc\""), std::string("abc"));
    CHECK_EQ(stripped("\"q.txt\""), std::string("q.txtt"));   // terminator at the old end
    CHECK_EQ(stripped("\"abc"), std::string("abcc"));
    CHECK_EQ(stripped("\""), std::string(""));
    CHECK_EQ(stripped("\"\""), std::string("\""));            // start 1 == end 1: no copy
    MArgs a;
    parse({"-t", "\"2\""}, a);
    CHECK_EQ(a.t, 22);
    MArgs b;
    parse({"-d", ""}, b); // empty value: no stripping, flag still set
    CHECK_EQ(std::string(b.db), std::string(""));
}

TEST_CASE(crashes_of_the_original)
{
    CHECK_EQ(crash_of({"-m", "header", "-o"}), kStatusAccessViolation);
    CHECK_EQ(crash_of({"-d"}), kStatusAccessViolation);
    CHECK_EQ(crash_of({"-o", "x", "-m"}), kStatusAccessViolation);
    CHECK_EQ(crash_of({"-t"}), kStatusAccessViolation);
    CHECK_EQ(crash_of({"-r", std::string(1023, '7')}), 0u);
    CHECK_EQ(crash_of({"-r", std::string(1024, '7')}), kStatusStackBufferOverrun);
}

TEST_CASE(help_exits_zero)
{
    MArgs a;
    try {
        parse({"-d", "a", "-H", "-o"}, a); // -o after -h is never parsed
        CHECK(false);
    } catch (const ExitRequest& e) {
        CHECK_EQ(e.code, 0);
    }
}

TEST_CASE(dispatcher_checks_o_then_d)
{
    MArgs a;
    try {
        dispatch_m(kFlagDb, a);
        CHECK(false);
    } catch (const ExitRequest& e) {
        CHECK_EQ(e.code, -1);
    }
    try {
        dispatch_m(kFlagOut | kFlagS1, a);
        CHECK(false);
    } catch (const ExitRequest& e) {
        CHECK_EQ(e.code, -1);
    }
    a.mode = 7; // parsed-only mode: banner, nothing else
    dispatch_m(kFlagOut | kFlagDb, a);
}

int main() { return testkit::run_all(); }
