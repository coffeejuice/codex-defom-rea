// Bounded Save-entry processEvents(flags=0) prefix; no public Save binding.
// ctest-labels: m2m-s
#include <testkit/check.hpp>
#include "m2m/engine/save_event_prefix.hpp"

#include <stdexcept>
#include <utility>
#include <vector>

using namespace m2m::engine;

namespace {
SaveEntryDueObservation observation(std::vector<std::uint64_t> due = {})
{
    return {"0x140098129", 0, SaveEntryDueObservation::Origin::DeclaredAdapterSchedule,
            "candidate-profile-save-entry", std::move(due)};
}
}

TEST_CASE(save_entry_one_nonblocking_zero_flag_poll_dispatches_ordered_due_ids)
{
    SaveEntryEventPrefix prefix;
    std::vector<std::uint64_t> received;
    unsigned calls = 0;
    const auto dispatch = [&](const auto& ids) { ++calls; received = ids; };
    CHECK(prefix.run(0, true, observation({8, 3}), dispatch) ==
          SaveEntryEventPrefix::Result::Polled);
    CHECK(prefix.attempted());
    CHECK_EQ(calls, 1u);
    CHECK_EQ(received.size(), 2u);
    CHECK_EQ(received[0], 8u);
    CHECK_EQ(received[1], 3u);
}

TEST_CASE(explicit_empty_schedule_is_not_inferred_and_is_still_one_poll)
{
    SaveEntryEventPrefix prefix;
    unsigned calls = 0;
    CHECK(prefix.run(0, true, observation({}), [&](const auto& ids) {
        ++calls; CHECK(ids.empty());
    }) == SaveEntryEventPrefix::Result::Polled);
    CHECK_EQ(calls, 1u);
}

TEST_CASE(missing_or_mismatched_observation_fails_before_prefix_is_consumed)
{
    SaveEntryEventPrefix prefix;
    bool threw = false;
    try { (void)prefix.run(0, true, std::nullopt, [](const auto&) {}); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
    CHECK(!prefix.attempted());

    auto wrong = observation(); wrong.sourceSite = "0x1400b94b3";
    threw = false;
    try { (void)prefix.run(0, true, wrong, [](const auto&) {}); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
    CHECK(!prefix.attempted());
}

TEST_CASE(nonzero_flags_and_duplicate_poll_are_rejected)
{
    SaveEntryEventPrefix prefix;
    bool threw = false;
    try { (void)prefix.run(1, true, observation(), [](const auto&) {}); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
    CHECK(!prefix.attempted());

    CHECK(prefix.run(0, true, observation(), [](const auto&) {}) ==
          SaveEntryEventPrefix::Result::Polled);
    threw = false;
    try { (void)prefix.run(0, true, observation(), [](const auto&) {}); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
}

TEST_CASE(absent_dispatcher_has_no_event_delivery_and_rejects_schedule)
{
    SaveEntryEventPrefix prefix;
    CHECK(prefix.run(0, false, std::nullopt, {}) ==
          SaveEntryEventPrefix::Result::DispatcherAbsent);
    CHECK(prefix.attempted());

    SaveEntryEventPrefix other;
    bool threw = false;
    try { (void)other.run(0, false, observation(), [](const auto&) {}); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
    CHECK(!other.attempted());
}

TEST_CASE(callback_failure_does_not_replay_the_poll)
{
    SaveEntryEventPrefix prefix;
    unsigned calls = 0;
    bool threw = false;
    try {
        (void)prefix.run(0, true, observation({4}), [&](const auto&) {
            ++calls; throw std::runtime_error("dispatch callback failed");
        });
    } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
    CHECK_EQ(calls, 1u);
    CHECK(prefix.attempted());
}

int main() { return testkit::run_all(); }
