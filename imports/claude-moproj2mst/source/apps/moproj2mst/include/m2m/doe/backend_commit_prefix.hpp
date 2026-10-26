#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace m2m {
struct Project;
struct SubProject;
namespace engine { class ConversionSession; }
}

namespace m2m::doe {
struct SessionNotificationState;
class FlowReceiverWitness;

enum class BackendSetterSite { OldModelDetach, FirstNewModelAttach, SecondNewModelAttach };

// Explicit occurrence from the actual diagram setter caller, not a timestamp or
// an inference from activeSub. The pointer is an identity; this record owns no
// backend lifetime. Repeated setModel calls require distinct diagramCallId values.
struct BackendSetterOccurrence {
    std::uint64_t diagramCallId = 0;
    BackendSetterSite site = BackendSetterSite::FirstNewModelAttach;
    const void* backend = nullptr;
};

struct BackendSetterPrerequisiteContext {
    SessionNotificationState& notifier;
    const FlowReceiverWitness& receiver;
    const Project& project;
    const SubProject& sub;
    BackendSetterOccurrence occurrence;
    const char* signal = "2signalDiagModelCanAddItem()";
    const char* slot = "1slotDiagModelCanAddItem()";
    bool prepend = false;
};

// The derived setter changes this distinct notifier key BEFORE the backend
// store. The default driver refuses. A future actual binding must implement the
// complete registration/removal prefix on the supplied SAME notifier; a test
// driver is only a declared diagnostic profile, not native registration proof.
class BackendSetterPrerequisiteDriver {
public:
    virtual ~BackendSetterPrerequisiteDriver() = default;
    virtual std::string qualification() const { return {}; }
    virtual void registerCanAddItem(const BackendSetterPrerequisiteContext&);
    virtual void removeCanAddItem(const BackendSetterPrerequisiteContext&);
};

enum class BackendCommitStop { Binding, Occurrence, Prerequisite, GenerationChanged };
struct BackendCommitAttempt {
    BackendSetterOccurrence occurrence;
    std::uint64_t callerSite = 0;
    std::string prerequisiteQualification;
    bool prerequisiteStarted = false;
    bool prerequisiteReturned = false;
    bool committed = false;
    std::optional<BackendCommitStop> stop;
    std::string error;
};

// Isolated per-generation projection of 140071b30 ->14014c3f0/store14014c404.
// Does not change FlowReceiverWitness::backendWitness(), install population,
// construct QObjects/backends or call public B121. The actual session must outlive
// each call. Unknown external writes/publication require explicit invalidation.
class BackendCommitPrefix final {
public:
    BackendCommitPrefix(engine::ConversionSession& actualSession,
                        const std::shared_ptr<FlowReceiverWitness>& actualReceiver);
    ~BackendCommitPrefix();
    BackendCommitPrefix(const BackendCommitPrefix&) = delete;
    BackendCommitPrefix& operator=(const BackendCommitPrefix&) = delete;

    // Refusal retains the old projection and any prerequisite-driver effects.
    // The caller owns surrounding setModel order/effects. These occurrence
    // records neither synthesize omitted calls nor close their caller history.
    void apply(engine::ConversionSession& actualSession, BackendSetterOccurrence,
               BackendSetterPrerequisiteDriver&);
    // nullopt = unavailable; optional(nullptr) = qualified null within this
    // isolated projection. A dead/foreign/released actual context is unavailable.
    std::optional<const void*> current(engine::ConversionSession& actualSession) const noexcept;
    void invalidateCurrentEvidence(std::string reason);
    const std::vector<BackendCommitAttempt>& attempts() const noexcept;
    const std::vector<std::string>& invalidations() const noexcept;

private:
    struct State;
    std::unique_ptr<State> state_;
};

} // namespace m2m::doe
