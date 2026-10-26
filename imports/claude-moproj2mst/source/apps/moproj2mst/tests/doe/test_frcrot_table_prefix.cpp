// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <utility>
#include <string>
#include <vector>

#include "../../src/doe/frcrot_table_prefix.hpp"

using namespace m2m::doe;

TEST_CASE(frcrot_table_mode_zero_and_unknown_do_not_consume_input)
{
    FrcrotTablePrefixInput input;
    input.mode = 0;
    input.modeZeroConstant = 8.5;
    auto result = applyFrcrotTablePrefix(input);
    CHECK(result.disposition == FrcrotTablePrefixDisposition::Supported);
    CHECK_EQ(result.dimension, 0);
    CHECK_BITS(result.constant, 8.5);
    CHECK(result.destinations.empty());
    CHECK(result.scanReturnCodes.empty());
    CHECK(!result.trailingLineReadReached);
    input.mode = 99;
    result = applyFrcrotTablePrefix(input);
    CHECK(result.disposition == FrcrotTablePrefixDisposition::Supported);
    CHECK_EQ(result.tableStatus, 0);
}

TEST_CASE(frcrot_table_modes_use_distinct_codes_and_negative_count_clamps_to_zero)
{
    for (const auto [mode, code] : {std::pair<int, int>{1, 1}, {2, 4}}) {
        FrcrotTablePrefixInput input;
        input.mode = mode;
        input.preparedCount = -4;
        input.allocationAndZeroFillSucceeded = true;
        auto result = applyFrcrotTablePrefix(input);
        CHECK(result.disposition == FrcrotTablePrefixDisposition::Supported);
        CHECK_EQ(result.dimension, 1);
        CHECK_EQ(result.code, code);
        CHECK_EQ(result.normalizedCount, 0);
        CHECK(result.axis.empty());
        CHECK(result.values.empty());
        CHECK(!result.trailingLineReadReached);
    }
}

TEST_CASE(frcrot_table_scans_x_then_y_and_ignores_the_trailing_line_result)
{
    FrcrotTablePrefixInput input;
    input.mode = 1;
    input.preparedCount = 2;
    input.allocationAndZeroFillSucceeded = true;
    input.trailingLineReadReached = true;
    input.observations = {{1, 10.0}, {1, 20.0}, {1, 30.0}, {1, 40.0}};
    const auto result = applyFrcrotTablePrefix(input);
    CHECK(result.disposition == FrcrotTablePrefixDisposition::Supported);
    CHECK(result.destinations == std::vector<std::string>({"X0", "Y0", "X1", "Y1"}));
    CHECK(result.axis == std::vector<double>({10.0, 30.0}));
    CHECK(result.values == std::vector<double>({20.0, 40.0}));
    CHECK(result.trailingLineReadReached);
    CHECK_EQ(result.tableStatus, 0);
}

TEST_CASE(frcrot_table_x_failure_preserves_prior_row_and_skips_its_y)
{
    FrcrotTablePrefixInput input;
    input.mode = 2;
    input.preparedCount = 2;
    input.allocationAndZeroFillSucceeded = true;
    input.observations = {{1, 1.0}, {1, 2.0}, {-1, 0.0}};
    const auto result = applyFrcrotTablePrefix(input);
    CHECK(result.disposition == FrcrotTablePrefixDisposition::Supported);
    CHECK(result.axis == std::vector<double>({1.0, 0.0}));
    CHECK(result.values == std::vector<double>({2.0, 0.0}));
    CHECK(result.destinations == std::vector<std::string>({"X0", "Y0", "X1"}));
    CHECK_EQ(result.tableStatus, 1);
    CHECK(!result.trailingLineReadReached);
}

TEST_CASE(frcrot_table_y_failure_keeps_same_row_axis_and_zero_scan_continues)
{
    FrcrotTablePrefixInput input;
    input.mode = 1;
    input.preparedCount = 2;
    input.allocationAndZeroFillSucceeded = true;
    input.observations = {{1, 5.0}, {-1, 0.0}};
    const auto result = applyFrcrotTablePrefix(input);
    CHECK(result.disposition == FrcrotTablePrefixDisposition::Supported);
    CHECK(result.axis == std::vector<double>({5.0, 0.0}));
    CHECK(result.values == std::vector<double>({0.0, 0.0}));
    CHECK_EQ(result.tableStatus, 1);
    CHECK(result.destinations == std::vector<std::string>({"X0", "Y0"}));
    CHECK(result.scanReturnCodes == std::vector<int>({1, -1}));

    input.observations = {{0, 9.0}, {1, 6.0}, {1, 7.0}, {0, 9.0}};
    input.trailingLineReadReached = true;
    const auto continued = applyFrcrotTablePrefix(input);
    CHECK(continued.disposition == FrcrotTablePrefixDisposition::Supported);
    CHECK(continued.axis == std::vector<double>({0.0, 7.0}));
    CHECK(continued.values == std::vector<double>({6.0, 0.0}));
    CHECK(continued.destinations == std::vector<std::string>({"X0", "Y0", "X1", "Y1"}));
    CHECK(continued.scanReturnCodes == std::vector<int>({0, 1, 1, 0}));
    CHECK_EQ(continued.tableStatus, 0);
    CHECK(continued.trailingLineReadReached);
}

TEST_CASE(frcrot_table_rejects_unqualified_shapes_codes_and_allocation)
{
    FrcrotTablePrefixInput input;
    input.mode = 1;
    input.preparedCount = 1;
    input.allocationAndZeroFillSucceeded = true;
    input.observations = {{7, 1.0}, {1, 2.0}};
    CHECK(applyFrcrotTablePrefix(input).disposition == FrcrotTablePrefixDisposition::Unsupported);
    input.observations = {{1, 1.0}, {1, 2.0}};
    CHECK(applyFrcrotTablePrefix(input).disposition == FrcrotTablePrefixDisposition::Unsupported);
    input.observations = {{1, 1.0}};
    input.allocationAndZeroFillSucceeded = false;
    CHECK(applyFrcrotTablePrefix(input).disposition == FrcrotTablePrefixDisposition::Unsupported);
}

TEST_CASE(frcrot_table_rejects_observations_after_x_or_y_eof)
{
    FrcrotTablePrefixInput input;
    input.mode = 1;
    input.preparedCount = 1;
    input.allocationAndZeroFillSucceeded = true;
    input.observations = {{-1, 0.0}, {1, 2.0}};
    CHECK(applyFrcrotTablePrefix(input).disposition == FrcrotTablePrefixDisposition::Unsupported);
    input.observations = {{1, 1.0}, {-1, 0.0}, {1, 2.0}};
    CHECK(applyFrcrotTablePrefix(input).disposition == FrcrotTablePrefixDisposition::Unsupported);
}

TEST_CASE(frcrot_table_candidate_count_and_boundary_matrix)
{
    FrcrotTablePrefixInput input;
    input.mode = 2;
    input.allocationAndZeroFillSucceeded = true;
    input.preparedCount = 0;
    CHECK(applyFrcrotTablePrefix(input).disposition == FrcrotTablePrefixDisposition::Supported);

    input.preparedCount = 7;
    input.observations.assign(14, FrcrotScanObservation{1, 3.0});
    input.trailingLineReadReached = true;
    const auto maximum = applyFrcrotTablePrefix(input);
    CHECK(maximum.disposition == FrcrotTablePrefixDisposition::Supported);
    CHECK_EQ(maximum.normalizedCount, 7);
    CHECK_EQ(maximum.destinations.size(), 14u);

    input.preparedCount = 8;
    input.observations.clear();
    input.trailingLineReadReached = false;
    CHECK(applyFrcrotTablePrefix(input).disposition == FrcrotTablePrefixDisposition::Unsupported);

    input.preparedCount = 1;
    input.observations.clear();
    CHECK(applyFrcrotTablePrefix(input).disposition == FrcrotTablePrefixDisposition::Unsupported);

    input.observations = {{1, 1.0}, {1, 2.0}};
    CHECK(applyFrcrotTablePrefix(input).disposition == FrcrotTablePrefixDisposition::Unsupported);
}

int main() { return testkit::run_all(); }
