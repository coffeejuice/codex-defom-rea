#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <utility>
#include "m2m/engine/conversion_session.hpp"

namespace m2m::engine {

struct BooleanTraceBindingContext;
struct BooleanPumpTrace;
struct BooleanTraceDueResult;
struct BooleanTraceDueValidation;
class BooleanBoundTraceDue;

// A non-owning admission scope with a distinct lifetime. It never owns or
// replaces the event scheduler. Owner must outlive this non-rebindable scope.
class BooleanTraceOwnerScope final {
    struct Lifetime {};
public:
    explicit BooleanTraceOwnerScope(const DiagramLifecycleEvents& owner)
        : owner_(&owner), lifetime_(std::make_shared<const Lifetime>()) {}
    BooleanTraceOwnerScope(const BooleanTraceOwnerScope&) = delete;
    BooleanTraceOwnerScope& operator=(const BooleanTraceOwnerScope&) = delete;
    BooleanTraceOwnerScope(BooleanTraceOwnerScope&&) = delete;
    BooleanTraceOwnerScope& operator=(BooleanTraceOwnerScope&&) = delete;
private:
    friend class BooleanBoundTraceDue;
    friend BooleanTraceDueResult bindBooleanTraceDue(const DiagramLifecycleEvents&,
        const BooleanTraceBindingContext&, const BooleanPumpTrace&);
    friend BooleanTraceDueValidation validateBooleanTraceDueForOwner(const DiagramLifecycleEvents&,
        const BooleanTraceOwnerScope&, const BooleanBoundTraceDue&);
    const DiagramLifecycleEvents* const owner_;
    std::shared_ptr<const Lifetime> lifetime_;
};

// Immutable trace identity supplied by the capture/qualification layer. This
// binder checks equality and structure; it does not authenticate evidence files.
struct BooleanTraceIdentity {
    std::string executableSha256, qtCoreSha256, qtGuiSha256;
    std::string scriptSha256, fixtureSha256;
    std::string actionProfile, nativeSession;
    bool operator==(const BooleanTraceIdentity&) const = default;
};

struct BooleanNativeTimerKey {
    std::uint64_t receiverAddress = 0, receiverLifetime = 0;
    std::uint64_t timerAddress = 0, timerLifetime = 0;
    std::uint64_t registrationOrdinal = 0;
    bool operator==(const BooleanNativeTimerKey&) const = default;
};

struct BooleanTraceRegistration {
    BooleanNativeTimerKey native;
    std::int32_t nativeTimerId = 0;
    unsigned requestedIntervalMs = 0;
    std::uint64_t nativeThread = 0;
    DiagramLifecycleEvents::ReceiverId logicalReceiver = 0;
    DiagramLifecycleEvents::TimerId logicalTimer = 0;
    std::string evidence;
};

struct BooleanTraceDelivery {
    BooleanNativeTimerKey native;
    // Registration ID retained by the timerEvent entry context, before its
    // kill/-1 store; this is not a late read of the already-consumed timer.
    std::int32_t nativeTimerId = 0;
    std::uint64_t nativeThread = 0;
    std::uint64_t enterOrdinal = 0, returnOrdinal = 0;
    // These must be actual selected timerEvent/modelUpdated observations, not
    // readiness inferred from interval, elapsed time, outputs or a screenshot.
    bool timerEventObserved = false, modelUpdatedObserved = false;
    bool matchedCallbackReceiver = false, balanced = false;
    std::string evidence;
};

struct BooleanPumpTrace {
    BooleanTraceIdentity identity;
    PumpPoint point;
    std::uint64_t nativeThread = 0;
    std::uint64_t enterOrdinal = 0, returnOrdinal = 0;
    bool complete = false, balanced = false, registrationsComplete = false;
    bool truncated = false;
    unsigned readErrors = 0, observerErrors = 0, unmatchedFrames = 0;
    std::string evidence;
    std::vector<BooleanTraceDelivery> deliveries;
};

struct BooleanTraceBindingContext {
    // Borrowed for this call only; does not create another event owner. The
    // caller must keep the same live events object and its mapping in scope.
    const DiagramLifecycleEvents* logicalOwner = nullptr;
    const BooleanTraceOwnerScope* ownerScope = nullptr;
    BooleanTraceIdentity expectedIdentity;
    PumpPoint expectedPoint;
    std::uint64_t expectedNativeThread = 0;
    std::vector<BooleanTraceRegistration> registrations;
};

// No raw observation accessor/conversion: mapping alone is not admission to
// another owner. Escaped copies retain only a weak admission-scope lifetime.
class BooleanBoundTraceDue final {
public:
    BooleanBoundTraceDue(const BooleanBoundTraceDue&) = default;
    BooleanBoundTraceDue& operator=(const BooleanBoundTraceDue&) = default;
    BooleanBoundTraceDue(BooleanBoundTraceDue&&) = default;
    BooleanBoundTraceDue& operator=(BooleanBoundTraceDue&&) = default;
private:
    friend BooleanTraceDueResult bindBooleanTraceDue(const DiagramLifecycleEvents&,
        const BooleanTraceBindingContext&, const BooleanPumpTrace&);
    friend BooleanTraceDueValidation validateBooleanTraceDueForOwner(const DiagramLifecycleEvents&,
        const BooleanTraceOwnerScope&, const BooleanBoundTraceDue&);
    BooleanBoundTraceDue(const DiagramLifecycleEvents* owner, const BooleanTraceOwnerScope& scope,
        DueObservation observation, std::vector<DiagramLifecycleEvents::PendingModelUpdate> registrations)
        : owner_(owner), scope_(&scope), lifetime_(scope.lifetime_), observation_(std::move(observation)),
          registrations_(std::move(registrations)) {}
    const DiagramLifecycleEvents* owner_;
    const BooleanTraceOwnerScope* scope_;
    std::weak_ptr<const BooleanTraceOwnerScope::Lifetime> lifetime_;
    DueObservation observation_;
    std::vector<DiagramLifecycleEvents::PendingModelUpdate> registrations_;
};

struct BooleanTraceDueResult {
    std::optional<BooleanBoundTraceDue> binding;
    std::string issue;
};
struct BooleanTraceDueValidation {
    std::optional<DueObservation> observation;
    std::string issue;
};

// Pure qualification/mapping seam: never reads a clock, calls callbacks,
// consumes timers, posts OS messages, or edits dispatchAt. Unknown/incomplete
// evidence returns no observation; complete observed emptiness is explicit.
BooleanTraceDueResult bindBooleanTraceDue(const DiagramLifecycleEvents&,
    const BooleanTraceBindingContext&, const BooleanPumpTrace&);

// Validate immediately before use against the same live scope/owner and still
// pending registrations. No timer is consumed. Shared dispatchAt is unchanged;
// its future session bridge must enforce this same-owner check before dispatch.
BooleanTraceDueValidation validateBooleanTraceDueForOwner(const DiagramLifecycleEvents&,
    const BooleanTraceOwnerScope&, const BooleanBoundTraceDue&);

} // namespace m2m::engine
