#pragma once

#include "m2m/doe/backend_commit_prefix.hpp"

#include <cstddef>
#include <functional>

namespace m2m::doe {

enum class BackendPrerequisiteRecordOrigin { ImportedProfile, RegistrationPrefix };
struct BackendPrerequisiteRecord {
    std::uint64_t identity = 0; // native pointer identity token; never dereferenced
    std::weak_ptr<FlowReceiverWitness> receiver;
    std::string bareMethod;
    BackendPrerequisiteRecordOrigin origin = BackendPrerequisiteRecordOrigin::ImportedProfile;
};

struct BackendPrerequisiteMapState {
    bool keyPresent = false;
    std::vector<std::uint64_t> orderedRecordIds;
    std::string qualification;
};

// Caller-supplied actual same-key state. The default is unavailable, not absent.
// Repeated IDs in the ordered list are aliases of one record, not duplicate
// allocations. Imported metadata does not claim native allocation observations.
struct BackendPrerequisiteMapProfile {
    BackendPrerequisiteMapState map;
    std::vector<BackendPrerequisiteRecord> records;
};

enum class BackendPrerequisiteAction { Register, Remove };
enum class BackendPrerequisiteStop { Binding, Context, MapState, Record, GenerationChanged, Effect };
enum class BackendPrerequisiteStage { RecordReady, KeyInserted, LocalListReady, MapWritten };
struct BackendPrerequisiteAttempt {
    BackendPrerequisiteAction action = BackendPrerequisiteAction::Register;
    BackendSetterOccurrence occurrence;
    std::optional<std::uint64_t> allocatedRecord;
    std::optional<std::uint64_t> removedRecord;
    std::size_t aliasesRemoved = 0;
    bool keyInserted = false;
    bool snapshotTaken = false;
    bool duplicate = false;
    std::vector<std::uint64_t> localRecordIds;
    bool mapWritten = false;
    bool completed = false;
    std::optional<BackendPrerequisiteStop> stop;
    std::string error;
};

// Isolated 1402cfbe0/1402cffd0 projection for the exact backend-setter key/slot.
// Does not install native map population, alter unrelated notifier flags or
// publish a FlowReceiverWitness backend. Actual Session must outlive calls.
class BackendPrerequisiteRegistry final : public BackendSetterPrerequisiteDriver {
public:
    explicit BackendPrerequisiteRegistry(engine::ConversionSession& actualSession);
    ~BackendPrerequisiteRegistry() override;
    BackendPrerequisiteRegistry(const BackendPrerequisiteRegistry&) = delete;
    BackendPrerequisiteRegistry& operator=(const BackendPrerequisiteRegistry&) = delete;

    // One-time explicit evidence binding, allowed after an unknown-map refusal.
    // A profile cannot replace a map already changed by this registry.
    void bindInitialMap(BackendPrerequisiteMapProfile);
    std::string qualification() const override;
    void registerCanAddItem(const BackendSetterPrerequisiteContext&) override;
    void removeCanAddItem(const BackendSetterPrerequisiteContext&) override;

    const std::optional<BackendPrerequisiteMapState>& observedMap() const noexcept;
    const std::vector<BackendPrerequisiteRecord>& retainedRecords() const noexcept;
    const std::vector<BackendPrerequisiteAttempt>& attempts() const noexcept;

    // Diagnostic injection only; no native callback/event-loop behavior inferred.
    using DiagnosticObserver = std::function<void(BackendPrerequisiteStage,
                                                  const BackendPrerequisiteAttempt&)>;
    void setDiagnosticObserver(DiagnosticObserver);

private:
    struct State;
    std::unique_ptr<State> state_;
    void execute(const BackendSetterPrerequisiteContext&, BackendPrerequisiteAction);
};

} // namespace m2m::doe
