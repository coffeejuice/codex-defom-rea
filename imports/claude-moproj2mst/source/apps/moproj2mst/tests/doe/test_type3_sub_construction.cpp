// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <algorithm>
#include <vector>

#include "m2m/common.hpp"
#include "../../src/ops/type3_sub_construction.hpp"

using namespace m2m::mst::detail;

namespace {
Type3SubConstructionHooks successfulHooks()
{
    Type3SubConstructionHooks h;
    h.allocateManager = [] { return std::optional{Type3ManagerIdentity{20}}; };
    h.managerConstructorPrefix = [](auto, auto, auto) { return true; };
    h.constructManager = [](auto, auto, auto) { return true; };
    h.allocateOptimizer = [] { return std::optional{Type3OptimizerIdentity{28}}; };
    h.constructOptimizer = [](auto) { return true; };
    return h;
}
}

TEST_CASE(type3_sub_false_directory_attempt_stops_sequence_but_is_ignored)
{
    auto h = successfulHooks();
    int calls = 0;
    h.directoryAttempts = {[&] { ++calls; return false; }, [&] { ++calls; return true; }};
    const auto r = constructType3SubOwners({1}, {2}, h);
    CHECK_EQ(calls, 1);
    CHECK(r.completedNormally);
    CHECK(r.manager == Type3ManagerIdentity{20});
    CHECK(r.optimizer == Type3OptimizerIdentity{28});
    CHECK(r.completed[2].kind == Type3SubConstructionEventKind::DirectoryAttempt);
    CHECK(!r.completed[2].result);
}

TEST_CASE(type3_sub_manager_constructor_failure_does_not_publish)
{
    auto h = successfulHooks();
    h.constructManager = [](auto, auto, auto) { return false; };
    const auto r = constructType3SubOwners({1}, {2}, h);
    CHECK(!r.completedNormally);
    CHECK(!r.manager);
    CHECK(!r.optimizer);
    CHECK(r.completed.back().kind == Type3SubConstructionEventKind::InFlightAllocationReleased);
    CHECK_EQ(r.completed.back().identity, 20u);
}

TEST_CASE(type3_sub_optimizer_failure_keeps_published_manager)
{
    auto h = successfulHooks();
    h.constructOptimizer = [](auto) { return false; };
    const auto r = constructType3SubOwners({1}, {2}, h);
    CHECK(!r.completedNormally);
    CHECK(r.manager == Type3ManagerIdentity{20});
    CHECK(!r.optimizer);
    CHECK_EQ(r.completed.back().identity, 28u);
}

TEST_CASE(type3_sub_null_allocations_publish_null_slots)
{
    auto h = successfulHooks();
    h.allocateManager = [] { return std::optional<Type3ManagerIdentity>{}; };
    h.allocateOptimizer = [] { return std::optional<Type3OptimizerIdentity>{}; };
    const auto r = constructType3SubOwners({1}, {2}, h);
    CHECK(r.completedNormally);
    CHECK(!r.manager);
    CHECK(!r.optimizer);
    CHECK(r.completed[1].kind == Type3SubConstructionEventKind::ManagerPublished);
    CHECK(r.completed.back().kind == Type3SubConstructionEventKind::OptimizerPublished);
}

TEST_CASE(type3_sub_zero_identity_allocation_is_null_and_skips_constructor)
{
    auto h = successfulHooks();
    int managerCalls = 0;
    int optimizerCalls = 0;
    h.allocateManager = [] { return std::optional{Type3ManagerIdentity{0}}; };
    h.constructManager = [&](auto, auto, auto) { ++managerCalls; return true; };
    h.allocateOptimizer = [] { return std::optional{Type3OptimizerIdentity{0}}; };
    h.constructOptimizer = [&](auto) { ++optimizerCalls; return true; };
    const auto r = constructType3SubOwners({1}, {2}, h);
    CHECK(r.completedNormally);
    CHECK_EQ(managerCalls, 0);
    CHECK_EQ(optimizerCalls, 0);
    CHECK(!r.manager);
    CHECK(!r.optimizer);
    REQUIRE(r.completed.size() == 4u);
    CHECK(r.completed[0].kind == Type3SubConstructionEventKind::ManagerAllocated);
    CHECK(r.completed[1].kind == Type3SubConstructionEventKind::ManagerPublished);
    CHECK(r.completed[2].kind == Type3SubConstructionEventKind::OptimizerAllocated);
    CHECK(r.completed[3].kind == Type3SubConstructionEventKind::OptimizerPublished);
}

TEST_CASE(type3_sub_success_publishes_only_after_each_constructor)
{
    auto h = successfulHooks();
    const auto r = constructType3SubOwners({1}, {2}, h);
    CHECK(r.completedNormally);
    CHECK(r.manager == Type3ManagerIdentity{20});
    CHECK(r.optimizer == Type3OptimizerIdentity{28});
    std::vector<Type3SubConstructionEventKind> sequence;
    for (const auto& event : r.completed) sequence.push_back(event.kind);
    const std::vector<Type3SubConstructionEventKind> expected{
        Type3SubConstructionEventKind::ManagerAllocated,
        Type3SubConstructionEventKind::ManagerConstructorPrefixCompleted,
        Type3SubConstructionEventKind::ManagerConstructed,
        Type3SubConstructionEventKind::ManagerPublished,
        Type3SubConstructionEventKind::OptimizerAllocated,
        Type3SubConstructionEventKind::OptimizerConstructed,
        Type3SubConstructionEventKind::OptimizerPublished,
    };
    CHECK(sequence == expected);
    const auto managerConstructed = std::find(sequence.begin(), sequence.end(),
        Type3SubConstructionEventKind::ManagerConstructed);
    const auto managerPublished = std::find(sequence.begin(), sequence.end(),
        Type3SubConstructionEventKind::ManagerPublished);
    const auto optimizerConstructed = std::find(sequence.begin(), sequence.end(),
        Type3SubConstructionEventKind::OptimizerConstructed);
    const auto optimizerPublished = std::find(sequence.begin(), sequence.end(),
        Type3SubConstructionEventKind::OptimizerPublished);
    REQUIRE(managerConstructed != sequence.end());
    REQUIRE(managerPublished != sequence.end());
    REQUIRE(optimizerConstructed != sequence.end());
    REQUIRE(optimizerPublished != sequence.end());
    CHECK(managerConstructed < managerPublished);
    CHECK(managerPublished < optimizerConstructed);
    CHECK(optimizerConstructed < optimizerPublished);
}

int main() { return testkit::run_all(); }
