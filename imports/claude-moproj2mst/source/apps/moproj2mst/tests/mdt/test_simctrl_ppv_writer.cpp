// ctest-labels: m2m-mdt
#include <testkit/check.hpp>

#include "m2m/engine/simctrl_ppv_writer.hpp"

using namespace m2m::engine::simctrl_ppv;

TEST_CASE(simctrl_ppv_writer_emits_shared_rows_then_local_seven_field_rows)
{
    const std::vector<Point> shared{{3, 4, 1.25, -2.5}};
    const std::vector<Point> local{{8, 9, 3.5, 4.75}};
    const std::vector<bool> flags{true};

    const auto result = writePpvPayload(false, true, local, &shared, &flags, 1252);
    CHECK(result.openAttempted);
    CHECK(result.returned);
    CHECK_EQ(result.bytes,
             "2\r\n"
             "0\ttrue\t3\t4\t1.25\t-2.5\t1\r\n"
             "0\tfalse\t8\t9\t3.5\t4.75\t0\r\n");
}

TEST_CASE(simctrl_ppv_writer_counts_shared_rows_even_without_flag_list)
{
    const std::vector<Point> shared{{3, 4, 1.25, -2.5}};
    const std::vector<Point> local{{8, 9, 3.5, 4.75}};

    const auto result = writePpvPayload(false, true, local, &shared, nullptr, 1252);
    CHECK(result.returned);
    CHECK_EQ(result.bytes,
             "2\r\n"
             "0\tfalse\t8\t9\t3.5\t4.75\t0\r\n");
}

TEST_CASE(simctrl_ppv_writer_preserves_opaque_gate_and_open_failure_returns)
{
    const std::vector<Point> local{{1, 2, 3.0, 4.0}};
    const auto gated = writePpvPayload(true, true, local, nullptr, nullptr, 1252);
    CHECK(!gated.openAttempted);
    CHECK(!gated.returned);
    CHECK(gated.bytes.empty());

    const auto failedOpen = writePpvPayload(false, false, local, nullptr, nullptr, 1252);
    CHECK(failedOpen.openAttempted);
    CHECK(!failedOpen.returned);
    CHECK(failedOpen.bytes.empty());

    const std::vector<Point> empty;
    const auto emptyLists = writePpvPayload(false, true, empty, nullptr, nullptr, 1252);
    CHECK(emptyLists.returned);
    CHECK_EQ(emptyLists.bytes, "0\r\n");
}

TEST_CASE(simctrl_ppv_writer_empty_local_gate_skips_root_only_payload)
{
    const std::vector<Point> local;
    const std::vector<Point> shared{{3, 4, 1.25, -2.5}};
    const std::vector<bool> flags{true};

    const auto result = writePpvPayload(true, true, local, &shared, &flags, 1252);
    CHECK(!result.openAttempted);
    CHECK(!result.returned);
    CHECK(result.bytes.empty());
}

int main() { return testkit::run_all(); }
