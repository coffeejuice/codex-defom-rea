// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "m2m/engine/simctrl_ppv_reader.hpp"

using namespace m2m;
using namespace m2m::engine::simctrl_ppv;

namespace {
qt::String s(const char* text) { return qt::String::latin1(text); }
}

TEST_CASE(simctrl_ppv_lookup_uses_exact_ints_strict_double_tolerance_and_first_match)
{
    std::vector<Point> points{{7, 9, 1.0, 2.0}, {7, 9, 1.0 + 0.5e-6, 2.0}};
    CHECK_EQ(findOrAddPoint(points, Point{7, 9, 1.0 + 0.25e-6, 2.0}), 0);
    CHECK_EQ(points.size(), 2u);

    // A difference equal to the exact binary64 threshold fails the native
    // strict comparison; an integer mismatch also prevents deduplication.
    CHECK_EQ(findOrAddPoint(points, Point{7, 9, 1.0e-6, 0.0}), 2);
    CHECK_EQ(findOrAddPoint(points, Point{8, 9, 1.0, 2.0}), 3);
}

TEST_CASE(simctrl_ppv_reader_clears_only_local_and_inserts_token5_flags_at_first_match)
{
    std::vector<Point> local{{90, 91, 92.0, 93.0}};
    std::vector<Point> shared{{4, 5, 1.0, 2.0}, {4, 5, 1.0 + 0.5e-6, 2.0}};
    std::vector<bool> flags{false, true};
    const std::vector<qt::String> lines{
        s("2"),
        s("0\tTRUE\t4\t5\t1.0000005\t2\t0"),
        s("0 true 4 5 1.0000005 2 0"),
    };

    CHECK(readPpvLines(true, lines, local, &shared, &flags));
    CHECK(local.empty());
    CHECK_EQ(shared.size(), 2u); // Both rows deduplicated to the first match.
    CHECK_EQ(flags.size(), 4u);
    CHECK(flags[0]);
    CHECK(flags[1]);
    CHECK(!flags[2]);
    CHECK(flags[3]); // Original second flag shifted; inserts do not replace.
}

TEST_CASE(simctrl_ppv_reader_uses_unicode_whitespace_and_shared_falls_back_to_local)
{
    std::vector<Point> local;
    std::vector<Point> shared;
    std::vector<bool> flags;
    std::u16string row = u"0\u2003TRUE\u2003 3\t4\u00a0 5.5\u2028 6.5";
    const std::vector<qt::String> lines{s("1"), qt::String(std::move(row))};

    CHECK(readPpvLines(true, lines, local, nullptr, &flags));
    CHECK_EQ(local.size(), 1u); // `true` selects shared only when param_3 exists.
    CHECK_EQ(local[0].firstIndex, 3);
    CHECK_EQ(local[0].secondIndex, 4);
    CHECK_EQ(local[0].firstValue, 5.5);
    CHECK_EQ(local[0].secondValue, 6.5);
    CHECK(flags.empty());
}

TEST_CASE(simctrl_ppv_reader_preserves_open_failure_and_partial_row_mutations)
{
    std::vector<Point> local{{10, 11, 12.0, 13.0}};
    std::vector<Point> shared{{20, 21, 22.0, 23.0}};
    std::vector<bool> flags{true};
    const std::vector<qt::String> unopened{s("1"), s("0 false 1 2 3 4")};

    CHECK(!readPpvLines(false, unopened, local, &shared, &flags));
    CHECK_EQ(local.size(), 1u); // QFile open failure precedes local clear.

    const std::vector<qt::String> malformed{
        s("2"), s("0 false 1 2 3 4"), s("only five fields here"),
    };
    CHECK(!readPpvLines(true, malformed, local, &shared, &flags));
    CHECK_EQ(local.size(), 1u); // First row remains after the short second row.
    CHECK_EQ(local[0].firstIndex, 1);
    CHECK_EQ(shared.size(), 1u);
    CHECK_EQ(flags.size(), 1u);
}

TEST_CASE(simctrl_ppv_reader_clears_on_nonpositive_count_and_ignores_extra_rows)
{
    std::vector<Point> local{{1, 2, 3.0, 4.0}};
    std::vector<Point> shared;
    std::vector<bool> flags;
    const std::vector<qt::String> nonpositive{s("0"), s("ignored")};
    CHECK(!readPpvLines(true, nonpositive, local, &shared, &flags));
    CHECK(local.empty());

    const std::vector<qt::String> extra{
        s("1"), s("0 false 1 2 3 4"), s("this trailing row is ignored"),
    };
    CHECK(readPpvLines(true, extra, local, &shared, &flags));
    CHECK_EQ(local.size(), 1u);
}

TEST_CASE(simctrl_ppv_reader_flag_branch_requires_exactly_seven_tokens)
{
    std::vector<Point> local;
    std::vector<Point> shared{{3, 4, 5.0, 6.0}};
    std::vector<bool> flags{false};
    const std::vector<qt::String> lines{
        s("2"),
        s("0 true 3 4 5 6 0"),       // Exactly seven: token 5 parses nonzero.
        s("0 true 3 4 5 6 1 extra"), // Eight: flag stays at its default false.
    };

    CHECK(readPpvLines(true, lines, local, &shared, &flags));
    CHECK_EQ(shared.size(), 1u);
    CHECK_EQ(flags.size(), 3u);
    CHECK(!flags[0]);
    CHECK(flags[1]);
    CHECK(!flags[2]);
}

TEST_CASE(simctrl_ppv_reader_ignores_numeric_conversion_status)
{
    std::vector<Point> local;
    const std::vector<qt::String> lines{s("1"), s("not-an-id false bad bad bad bad")};
    CHECK(readPpvLines(true, lines, local, nullptr, nullptr));
    CHECK_EQ(local.size(), 1u);
    CHECK_EQ(local[0].firstIndex, 0);
    CHECK_EQ(local[0].secondIndex, 0);
    CHECK_EQ(local[0].firstValue, 0.0);
    CHECK_EQ(local[0].secondValue, 0.0);
}

int main() { return testkit::run_all(); }

