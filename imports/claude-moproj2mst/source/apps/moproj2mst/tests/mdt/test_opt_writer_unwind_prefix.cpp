// ctest-labels: m2m-mdt opt-writer-unwind-prefix
#include <testkit/check.hpp>

#include <array>
#include <cstddef>

#include "m2m/engine/opt_writer_unwind_prefix.hpp"

using namespace m2m::engine;

namespace {
OptWriterUnwindPrepared prepared()
{
    OptWriterUnwindPrepared p;
    p.admission = OptWriterUnwindAdmission::AdmittedCppUnwindAtState4;
    p.identities = {101, 202, 303};
    p.exceptionIdentity = 404;
    p.cleanupReturns.fill(OptWriterCleanupReturn::Returned);
    return p;
}

void checkEvent(const OptWriterUnwindEvent& event, OptWriterUnwindEventKind kind,
                OptWriterUnwindResource resource, std::uint64_t identity,
                int fromState, int toState)
{
    CHECK(event.kind == kind);
    CHECK(event.resource == resource);
    CHECK_EQ(event.identity, identity);
    CHECK_EQ(event.fromState, fromState);
    CHECK_EQ(event.toState, toState);
}
}

TEST_CASE(opt_writer_unwind_state4_orders_distinct_resource_cleanup_and_retains_exception)
{
    const auto p = prepared();
    const auto result = projectOptWriterState4Unwind(p);
    CHECK(result.stop == OptWriterUnwindStop::State4CleanupReturned);
    REQUIRE(result.state.has_value());
    CHECK_EQ(*result.state, -1);
    CHECK(!result.awaitingReturn.has_value());
    CHECK_EQ(result.exceptionIdentity, p.exceptionIdentity);
    CHECK_EQ(result.identities.stream, p.identities.stream);
    CHECK_EQ(result.identities.file, p.identities.file);
    CHECK_EQ(result.identities.path, p.identities.path);
    REQUIRE(result.events.size() == 9);
    const std::array<OptWriterUnwindResource, 3> resources{
        OptWriterUnwindResource::Stream, OptWriterUnwindResource::File,
        OptWriterUnwindResource::Path};
    const std::array<std::uint64_t, 3> tokens{101, 202, 303};
    const std::array<int, 4> states{4, 1, 0, -1};
    for (std::size_t i = 0; i < resources.size(); ++i) {
        checkEvent(result.events[3*i], OptWriterUnwindEventKind::SuccessorStatePublished,
            resources[i], tokens[i], states[i], states[i+1]);
        checkEvent(result.events[3*i+1], OptWriterUnwindEventKind::CleanupEntered,
            resources[i], tokens[i], states[i], states[i+1]);
        checkEvent(result.events[3*i+2], OptWriterUnwindEventKind::CleanupReturned,
            resources[i], tokens[i], states[i], states[i+1]);
    }
    // Prepared inputs are tokens and premises, not mutable native resources.
    CHECK_EQ(p.identities.stream, 101u);
    CHECK_EQ(p.identities.file, 202u);
    CHECK_EQ(p.identities.path, 303u);
    CHECK_EQ(p.exceptionIdentity, 404u);
}

TEST_CASE(opt_writer_unwind_each_unavailable_cleanup_return_retains_only_reached_prefix)
{
    const auto completed = projectOptWriterState4Unwind(prepared());
    const std::array<OptWriterUnwindResource, 3> resources{
        OptWriterUnwindResource::Stream, OptWriterUnwindResource::File,
        OptWriterUnwindResource::Path};
    const std::array<int, 3> publishedStates{1, 0, -1};
    for (std::size_t blocked = 0; blocked < resources.size(); ++blocked) {
        auto p = prepared();
        p.cleanupReturns[blocked] = OptWriterCleanupReturn::Unavailable;
        const auto result = projectOptWriterState4Unwind(p);
        CHECK(result.stop == OptWriterUnwindStop::CleanupReturnUnavailable);
        REQUIRE(result.state.has_value());
        CHECK_EQ(*result.state, publishedStates[blocked]);
        REQUIRE(result.awaitingReturn.has_value());
        CHECK(*result.awaitingReturn == resources[blocked]);
        CHECK_EQ(result.exceptionIdentity, p.exceptionIdentity);
        REQUIRE(result.events.size() == 3*blocked + 2);
        for (std::size_t i = 0; i < result.events.size(); ++i) {
            const auto& expected = completed.events[i];
            checkEvent(result.events[i], expected.kind, expected.resource,
                expected.identity, expected.fromState, expected.toState);
        }
        CHECK(result.events.back().kind == OptWriterUnwindEventKind::CleanupEntered);
        // Even state -1 is not completion until the path cleanup returns.
        CHECK(result.stop != OptWriterUnwindStop::State4CleanupReturned);
    }
}

TEST_CASE(opt_writer_unwind_requires_admitted_cpp_state4_before_any_cleanup_projection)
{
    auto p = prepared();
    p.admission = OptWriterUnwindAdmission::Unavailable;
    const auto result = projectOptWriterState4Unwind(p);
    CHECK(result.stop == OptWriterUnwindStop::AdmissionUnavailable);
    CHECK(result.events.empty());
    CHECK(!result.state.has_value());
    CHECK(!result.awaitingReturn.has_value());
    CHECK_EQ(result.exceptionIdentity, p.exceptionIdentity);
}

TEST_CASE(opt_writer_unwind_rejects_null_or_aliased_resource_tokens_without_cleanup)
{
    for (int invalid = 0; invalid < 7; ++invalid) {
        auto p = prepared();
        switch (invalid) {
        case 0: p.identities.stream = 0; break;
        case 1: p.identities.file = 0; break;
        case 2: p.identities.path = 0; break;
        case 3: p.identities.file = p.identities.stream; break;
        case 4: p.identities.path = p.identities.stream; break;
        case 5: p.identities.path = p.identities.file; break;
        case 6: p.exceptionIdentity = 0; break;
        }
        const auto result = projectOptWriterState4Unwind(p);
        CHECK(result.stop == OptWriterUnwindStop::InvalidPreparedIdentity);
        CHECK(result.events.empty());
        CHECK(!result.state.has_value());
        CHECK(!result.awaitingReturn.has_value());
        CHECK_EQ(result.exceptionIdentity, p.exceptionIdentity);
    }
}

TEST_CASE(opt_writer_unwind_default_return_premises_do_not_assume_stream_cleanup_returned)
{
    auto p = prepared();
    p.cleanupReturns = {};
    const auto result = projectOptWriterState4Unwind(p);
    CHECK(result.stop == OptWriterUnwindStop::CleanupReturnUnavailable);
    REQUIRE(result.awaitingReturn.has_value());
    CHECK(*result.awaitingReturn == OptWriterUnwindResource::Stream);
    REQUIRE(result.events.size() == 2);
    CHECK(result.events.back().kind == OptWriterUnwindEventKind::CleanupEntered);
}

int main()
{
    return testkit::run_all();
}
