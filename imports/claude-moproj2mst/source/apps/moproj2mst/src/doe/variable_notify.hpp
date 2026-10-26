#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "m2m/doe_session_notifications.hpp"

namespace m2m {
struct Project;
struct SubProject;
struct FlowChart;
struct FlowItem;
namespace engine { class ConversionSession; }
}

namespace m2m::doe {
class StudyIo;
struct Study;
struct InputTableNotifierContext;

enum class SetOprHasDoeEntryOutcome { SuppressedCalleeReturned, KeyConstructionRequired };
// Actual synchronous TLOC caller only. Revalidate Session/Project/SUB/Study/I/O
// and canonical notifier, then suppression/+90. No key/map/list/receiver effect.
SetOprHasDoeEntryOutcome enterSetOprHasDoeNotifierPrefix(
    const InputTableNotifierContext&, Study&, int operationId, bool hasDoe);

// The actual model's lifecycle adapter supplies observations only. An extra
// shared diagnostic lease does not imply the native QObject is alive. Project,
// SubProject and model identities must remain valid while alive is reported.
class SetOprHasDoeReceiver {
public:
    virtual ~SetOprHasDoeReceiver() = default;
    virtual bool nativeReceiverAlive() const noexcept = 0;
    virtual const Project* actualProject() const noexcept = 0;
    virtual const SubProject* actualSubProject() const noexcept = 0;
    virtual const FlowChart* actualFlowModel() const noexcept = 0;
    // nullopt = unavailable, optional(nullptr) = qualified null, otherwise the
    // actual non-null backend identity. Never infer null from a headless model.
    virtual std::optional<const void*> backendWitness() const noexcept = 0;
};

struct SetOprHasDoeRegistration {
    std::weak_ptr<SetOprHasDoeReceiver> receiver;
    std::string bareMethod = "slotSetOprHasDoe";
};

struct SetOprHasDoeRegistrationList {
    std::vector<SetOprHasDoeRegistration> entries;
    std::string qualification; // actual ordered population; not a native-init assertion
    bool mapKeyPresent = false;
};

enum class SetOprHasDoeStop {
    Owner, Population, Method, ReceiverLifetime, ModelIdentity, BackendWitness,
    LogicalItem, IconAllocation, IconBackend
};
enum class SetOprHasDoeVisitOutcome { Pending, ExpiredGuard, NativeDead, NullBackend, MissingItem, Stopped };
enum class SetOprHasDoeCompletion { SuppressedReturn, ReceiverListReturned };

struct SetOprHasDoeVisit {
    std::size_t registrationIndex = 0;
    SetOprHasDoeVisitOutcome outcome = SetOprHasDoeVisitOutcome::Pending;
    const FlowChart* model = nullptr; // diagnostic identities, never owned/copied
    const FlowItem* item = nullptr;
    const void* backend = nullptr;
};

struct SetOprHasDoeAttempt {
    int operationId = 0; // native first argument: int, then bool
    bool hasDoe = false;
    bool ownerVerified = false;
    std::optional<bool> suppressed;
    bool flagReset = false;
    bool keyInserted = false;
    bool snapshotTaken = false;
    std::vector<SetOprHasDoeVisit> visits;
    std::optional<SetOprHasDoeCompletion> completion;
    std::optional<SetOprHasDoeStop> stop;
    std::string error;
};

// Explicit caller-retained binding to the SAME notifier already bound to StudyIo.
// No local notifier/map or default empty population is constructed. The actual
// context supplies registration order, model lifetime and backend qualification.
struct SetOprHasDoeBindings {
    explicit SetOprHasDoeBindings(const std::shared_ptr<SessionNotificationState>& actualOwner,
                                 std::string ownerQualification);
    SetOprHasDoeBindings(const SetOprHasDoeBindings&) = delete;
    SetOprHasDoeBindings& operator=(const SetOprHasDoeBindings&) = delete;
    std::weak_ptr<SessionNotificationState> owner;
    std::string ownerQualification;
    // Optional actual-context constraint. Identity only; this pointer owns no
    // Project lifetime. The session hook revalidates it before each dispatch.
    const Project* requiredProject = nullptr;
    std::optional<SetOprHasDoeRegistrationList> registrations;
    std::vector<SetOprHasDoeAttempt> attempts; // retains partial diagnostics on Error
};

// Bounded source integration with convert.cpp's existing actual session/I/O
// owner binding. This lease does not construct/register flow-chart QObjects,
// infer null backends, or install an empty receiver population. The actual
// ConversionSession must outlive calls made through this lease.
class SetOprHasDoeSessionBinding {
public:
    SetOprHasDoeSessionBinding(const SetOprHasDoeSessionBinding&) = delete;
    SetOprHasDoeSessionBinding& operator=(const SetOprHasDoeSessionBinding&) = delete;
    SetOprHasDoeBindings& notifier() noexcept { return notifier_; }
    const SetOprHasDoeBindings& notifier() const noexcept { return notifier_; }
    // Revalidate live Project, the same actual StudyIo, and the session owner
    // before delegating. Population remains unavailable until explicitly bound.
    void dispatch(engine::ConversionSession& actualSession, int operationId, bool hasDoe);

private:
    friend std::unique_ptr<SetOprHasDoeSessionBinding>
        bindSetOprHasDoeSessionPrefix(engine::ConversionSession& actualSession);
    SetOprHasDoeSessionBinding(Project& project, const std::shared_ptr<StudyIo>& io,
                              const std::shared_ptr<SessionNotificationState>& owner);
    const Project* project_;
    std::weak_ptr<StudyIo> io_;
    SetOprHasDoeBindings notifier_;
};

// Requires convert.cpp's actual existing binding; never creates/rebinds StudyIo
// or its lazy notifier. A successful hook is not completed native registration.
std::unique_ptr<SetOprHasDoeSessionBinding>
bindSetOprHasDoeSessionPrefix(engine::ConversionSession& actualSession);

// 1402cf3c0 ->140086e20 ->140086ca0. Returns only suppression/null-backend/
// missing-logical-item branches; guards before QIcon allocation or backend clear.
// Int operation ID precedes bool hasDoe, matching slotSetOprHasDoe(int,bool).
void dispatchSetOprHasDoePrefix(StudyIo& io, SetOprHasDoeBindings& actualBinding,
                               int operationId, bool hasDoe);

} // namespace m2m::doe
