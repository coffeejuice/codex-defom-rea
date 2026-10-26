#pragma once

#include "m2m/engine.hpp"
#include "../engine/simcontrol300_remesh_services.hpp"

namespace m2m::detail {

// Borrowed only by one synchronous conversion worker. The public conversion
// entrypoints never install this service.
class RemeshTerminalCallService {
public:
    virtual ~RemeshTerminalCallService() = default;
    virtual engine::simcontrol300::detail::RemeshTerminalProfile profile() const noexcept = 0;
    virtual engine::simcontrol300::detail::RemeshTerminalDispatchFailure dispatch(
        const engine::simcontrol300::detail::RemeshTerminalRequest&,
        const engine::SaveCallState&) noexcept = 0;
    virtual void failClosed(
        const engine::simcontrol300::detail::RemeshTerminalRequest&,
        engine::simcontrol300::detail::RemeshTerminalDispatchFailure,
        const engine::SaveCallState&) noexcept = 0;
};

// The leaf writer requests this only at its failed-open boundary. Copy the
// live Save state at dispatch, then reuse that exact value if dispatch returns.
class RemeshTerminalForwarder final : public engine::simcontrol300::detail::RemeshTerminalService {
public:
    RemeshTerminalForwarder(RemeshTerminalCallService& target,
                            const engine::SaveCallState& live) noexcept
        : target_(target), live_(live) {}

    engine::simcontrol300::detail::RemeshTerminalProfile profile() const noexcept override
    { return target_.profile(); }

    engine::simcontrol300::detail::RemeshTerminalDispatchFailure dispatch(
        const engine::simcontrol300::detail::RemeshTerminalRequest& request) noexcept override
    {
        snapshot_ = live_;
        captured_ = true;
        return target_.dispatch(request, snapshot_);
    }

    void failClosed(const engine::simcontrol300::detail::RemeshTerminalRequest& request,
                    engine::simcontrol300::detail::RemeshTerminalDispatchFailure failure) noexcept override
    {
        if (!captured_) snapshot_ = live_; // Defensive; the writer dispatches first.
        target_.failClosed(request, failure, snapshot_);
    }

private:
    RemeshTerminalCallService& target_;
    const engine::SaveCallState& live_;
    engine::SaveCallState snapshot_{};
    bool captured_ = false;
};

} // namespace m2m::detail
