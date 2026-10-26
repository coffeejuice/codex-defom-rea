#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "flow_receiver_witness.hpp"

namespace m2m::engine { class ConversionSession; }

namespace m2m::doe {

struct FlowReceiverPopulationPrefixEntry {
    std::size_t constructorOrdinal = 0;
    FlowReceiverConstructionSource source = FlowReceiverConstructionSource::NormalSub;
    std::uint64_t registrationSite = 0;
    const void* identityToken = nullptr; // Diagnostic identity; never dereferenced.
    SetOprHasDoeRegistration registration; // Weak receiver; no population qualification.
};

// Isolated snapshot of actual Session constructor history. This deliberately
// cannot certify or install a complete native registration population. Dead
// history remains ordered; later constructor events require a new snapshot.
class FlowReceiverPopulationPrefix final {
public:
    bool completeNativePopulation() const noexcept { return false; }
    const std::vector<FlowReceiverPopulationPrefixEntry>& entries() const noexcept
    { return entries_; }
    std::shared_ptr<SessionNotificationState> owner() const noexcept { return owner_.lock(); }
    // Recheck the original Session/Project/registry/notifier identities before
    // consuming diagnostics. A released registry cannot be distinguished from
    // all-dead history through the current registry interface.
    void validateAgainst(engine::ConversionSession& actualSession) const;

private:
    friend FlowReceiverPopulationPrefix
        projectFlowReceiverPopulationPrefix(engine::ConversionSession& actualSession);
    FlowReceiverPopulationPrefix(const Project*,
        const std::shared_ptr<FlowReceiverWitnessRegistry>&,
        const std::shared_ptr<SessionNotificationState>&);
    const Project* project_;
    std::weak_ptr<FlowReceiverWitnessRegistry> registry_;
    std::weak_ptr<SessionNotificationState> owner_;
    std::vector<FlowReceiverPopulationPrefixEntry> entries_;
};

// Allows pre-SUB0/pre-I/O observation. Existing I/O must already observe the
// actual Session notifier. Never creates/rebinds I/O or changes registrations,
// suppression, flag90, map keys, task lists or current Session selection.
FlowReceiverPopulationPrefix
projectFlowReceiverPopulationPrefix(engine::ConversionSession& actualSession);

} // namespace m2m::doe
