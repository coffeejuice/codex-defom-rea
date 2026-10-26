// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "../../src/core/conversion_call_services.hpp"
#include "../../src/core/remesh_terminal_call_service.hpp"

namespace remesh = m2m::engine::simcontrol300::detail;
using m2m::engine::DialogAnswer;
using m2m::engine::SaveCallState;

namespace {
struct Spy final : m2m::detail::RemeshTerminalCallService {
    SaveCallState dispatched{}, failed{};
    remesh::RemeshTerminalRequest dispatchRequest{}, failureRequest{};
    remesh::RemeshTerminalDispatchFailure failure = remesh::RemeshTerminalDispatchFailure::PublicationFailed;
    int dispatches = 0, failures = 0;

    remesh::RemeshTerminalProfile profile() const noexcept override
    { return remesh::RemeshTerminalProfile::FreshDefault; }
    remesh::RemeshTerminalDispatchFailure dispatch(
        const remesh::RemeshTerminalRequest& request, const SaveCallState& state) noexcept override
    {
        dispatchRequest = request;
        dispatched = state;
        ++dispatches;
        return remesh::RemeshTerminalDispatchFailure::EndpointReturned;
    }
    void failClosed(const remesh::RemeshTerminalRequest& request,
                    remesh::RemeshTerminalDispatchFailure why,
                    const SaveCallState& state) noexcept override
    {
        failureRequest = request;
        failure = why;
        failed = state;
        ++failures;
    }
};

void checkSnapshot(const SaveCallState& state)
{
    CHECK(state.answer == DialogAnswer::No);
    CHECK(state.opened);
    CHECK_EQ(state.checkedSubprojects, 3u);
    CHECK(state.validationComplete);
    CHECK(!state.lastOperationModalPending);
    CHECK(state.pendingDefault == DialogAnswer::Cancel);
    CHECK(state.projectWriteAttempted);
    CHECK(state.projectWriteReturned);
    CHECK(!state.projectWriteSucceeded);
    CHECK(state.mstWriteAttempted);
    CHECK(state.mstWriteReturned);
    CHECK(state.mstWriteSucceeded);
}
} // namespace

TEST_CASE(core_remesh_terminal_bridge_copies_live_save_state_only_at_dispatch)
{
    m2m::detail::ConversionCallServices ordinary{};
    CHECK(ordinary.remeshTerminal == nullptr);

    Spy target;
    SaveCallState live;
    live.answer = DialogAnswer::No;
    live.opened = true;
    live.checkedSubprojects = 3;
    live.validationComplete = true;
    live.lastOperationModalPending = false;
    live.pendingDefault = DialogAnswer::Cancel;
    live.projectWriteAttempted = true;
    live.projectWriteReturned = true;
    live.projectWriteSucceeded = false;
    live.mstWriteAttempted = true;
    live.mstWriteReturned = true;
    live.mstWriteSucceeded = true;
    m2m::detail::RemeshTerminalForwarder forwarder(target, live);
    CHECK(forwarder.profile() == remesh::RemeshTerminalProfile::FreshDefault);
    remesh::RemeshTerminalRequest request;
    const auto returned = forwarder.dispatch(request);
    CHECK(returned == remesh::RemeshTerminalDispatchFailure::EndpointReturned);
    live.answer = DialogAnswer::Yes;
    live.checkedSubprojects = 99;
    live.projectWriteSucceeded = true;
    live.mstWriteReturned = false;
    forwarder.failClosed(request, returned);

    CHECK_EQ(target.dispatches, 1);
    CHECK_EQ(target.failures, 1);
    CHECK_EQ(target.dispatchRequest.sourceAddress, request.sourceAddress);
    CHECK_EQ(target.failureRequest.expectedProcessStatus, request.expectedProcessStatus);
    CHECK(target.failure == remesh::RemeshTerminalDispatchFailure::EndpointReturned);
    checkSnapshot(target.dispatched);
    checkSnapshot(target.failed);
}

int main() { return testkit::run_all(); }
