#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace m2m::engine {

// Explicit schedule input for the one source-bound Save-entry
// processEvents(flags=0) call at 0x140098129. An empty due list is meaningful
// only when the caller supplies a nonempty qualification for that observation.
struct SaveEntryDueObservation {
    enum class Origin { OriginalTrace, DeclaredAdapterSchedule };
    std::string sourceSite;
    unsigned occurrence = 0;
    Origin origin = Origin::DeclaredAdapterSchedule;
    std::string qualification;
    std::vector<std::uint64_t> dueTimerIds;
};

class SaveEntryEventPrefix final {
public:
    using DispatchDue = std::function<void(const std::vector<std::uint64_t>&)>;
    enum class Result { DispatcherAbsent, Polled };

    SaveEntryEventPrefix() = default;
    SaveEntryEventPrefix(const SaveEntryEventPrefix&) = delete;
    SaveEntryEventPrefix& operator=(const SaveEntryEventPrefix&) = delete;

    // Mirrors only the recovered prefix: reject nonzero flags, do no blocking
    // wait, and dispatch at most once. If a dispatcher exists, an explicit
    // observation is required, including for an observed empty due list.
    Result run(unsigned flags, bool dispatcherPresent,
               const std::optional<SaveEntryDueObservation>&,
               const DispatchDue&);

    bool attempted() const noexcept { return attempted_; }

private:
    bool attempted_ = false;
};

} // namespace m2m::engine
