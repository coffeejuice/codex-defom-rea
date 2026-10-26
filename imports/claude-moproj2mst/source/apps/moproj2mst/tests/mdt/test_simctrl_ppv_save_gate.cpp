// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "m2m/engine/simctrl_ppv_save_gate.hpp"

#include <vector>

using namespace m2m::engine::simctrl_ppv;

TEST_CASE(ppv_save_gate_skips_root_only_points_when_local_list_is_empty)
{
    const std::vector<int> local;
    const std::vector<int> shared{1, 2, 3};
    const std::vector<bool> flags{true, false, true};

    CHECK(local.empty());
    CHECK(!shared.empty());
    CHECK_EQ(shared.size(), flags.size());
    CHECK(!ppvWriterGateAllows(local.size()));
}

TEST_CASE(ppv_save_gate_enters_writer_when_local_list_is_nonempty)
{
    const std::vector<int> local{7};
    CHECK(ppvWriterGateAllows(local.size()));
}

int main() { return testkit::run_all(); }
