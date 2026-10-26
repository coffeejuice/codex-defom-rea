// Isolated source-bound OPI Props/cache/list/recovery prefix. No Close call.
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "m2m/engine/operation_info_guard.hpp"

namespace m2m {
struct Operation;
struct Props;
namespace engine {
struct GuiModel;

enum class OperationInfoCleanupStage {
    InspectProps,
    PropsVirtualDeleteBlocked,
    EngineModelBoundary,
    EngineModelVirtualDeleteBlocked,
    ObjectCleanupBoundary,
    ObjectListEffectsBlocked,
    PositionCleanupBoundary,
    PositionListEffectsBlocked,
    RecoveryCleanupBoundary,
    RecoveryWrapperDeleteBlocked,
    RecoveryFlagsCleanupBoundary,
    RecoveryFlagsDeleteBlocked,
    ItemModelCacheBoundary
};

// These are adapter continuation outcomes, never native success/error returns.
// None denotes completed OPI cleanup or authorizes QObject-boundary expiry.
enum class OperationInfoCleanupOutcome {
    NullPropsSkipReached,
    NonNullPropsVirtualDeleteUnavailable,
    EngineModelCleanupUnavailable,
    NullEngineCacheSkipReached,
    NonNullEngineCacheVirtualDeleteUnavailable,
    ObjectCleanupUnavailable,
    EmptyObjectsSkipReached,
    NonemptyObjectsEffectsUnavailable,
    PositionCleanupUnavailable,
    EmptyPositionsSkipReached,
    NonemptyPositionsEffectsUnavailable,
    RecoveryCleanupUnavailable,
    NullRecoveryWrapperSkipReached,
    NonNullRecoveryWrapperDeleteUnavailable,
    RecoveryFlagsCleanupUnavailable,
    NullRecoveryFlagsSkipReached,
    NonNullRecoveryFlagsDeleteUnavailable,
    ItemModelCacheCleanupUnavailable
};

class OperationInfoCleanupProgress final {
public:
    explicit OperationInfoCleanupProgress(Operation&);
    OperationInfoCleanupProgress(const OperationInfoCleanupProgress&) = delete;
    OperationInfoCleanupProgress& operator=(const OperationInfoCleanupProgress&) = delete;
    OperationInfoCleanupProgress(OperationInfoCleanupProgress&&) = delete;
    OperationInfoCleanupProgress& operator=(OperationInfoCleanupProgress&&) = delete;

    OperationInfoCleanupStage stage() const noexcept { return stage_; }
    OperationInfoGuard identitySnapshot() const noexcept { return identity_; }
    const Props* observedPropsIdentity() const noexcept { return observedProps_; }
    std::uintptr_t observedEngineCacheIdentityToken() const noexcept { return observedEngineCacheToken_; }
    std::size_t observedObjectCount() const noexcept { return observedObjectCount_; }
    std::size_t observedPositionCount() const noexcept { return observedPositionCount_; }
    std::uintptr_t observedRecoveryWrapperIdentityToken() const noexcept { return observedRecoveryWrapperToken_; }
    std::uintptr_t observedRecoveryFlagsIdentityToken() const noexcept { return observedRecoveryFlagsToken_; }
private:
    friend OperationInfoCleanupOutcome attemptOperationInfoPropsCleanup(
        Operation&, OperationInfoCleanupProgress&);
    friend OperationInfoCleanupOutcome attemptOperationInfoEngineCacheCleanup(
        Operation&, OperationInfoCleanupProgress&);
    friend OperationInfoCleanupOutcome attemptOperationInfoObjectListCleanup(
        Operation&, OperationInfoCleanupProgress&);
    friend OperationInfoCleanupOutcome attemptOperationInfoPositionListCleanup(
        Operation&, OperationInfoCleanupProgress&);
    friend OperationInfoCleanupOutcome attemptOperationInfoRecoveryWrapperCleanup(
        Operation&, OperationInfoCleanupProgress&);
    friend OperationInfoCleanupOutcome attemptOperationInfoRecoveryFlagsCleanup(
        Operation&, OperationInfoCleanupProgress&);
    OperationInfoGuard identity_; // metadata only, never owns Operation/Props
    OperationInfoCleanupStage stage_ = OperationInfoCleanupStage::InspectProps;
    const Props* observedProps_ = nullptr; // compare identity only; do not dereference
    std::weak_ptr<GuiModel> observedEngineCache_; // no allocation ownership
    std::uintptr_t observedEngineCacheToken_ = 0; // opaque historical identity only
    std::size_t observedObjectCount_ = 0; // count only; blocked entries were not inspected
    std::size_t observedPositionCount_ = 0; // likewise no position entry inspection
    std::weak_ptr<void> observedRecoveryWrapper_; // canonical owner only, never retains payload
    std::uintptr_t observedRecoveryWrapperToken_ = 0; // opaque historical identity only
    std::weak_ptr<void> observedRecoveryFlags_; // canonical owner only, never retains payload
    std::uintptr_t observedRecoveryFlagsToken_ = 0; // opaque historical identity only
};

// 050690@0506a1 ->0512c0. Only the source null+10 skip is implemented.
// A nonnull pointer stops before virtual+18 at051307; there is no bypass API,
// reset/delete, callback, event delivery, or guard expiry. Retry retains that
// exact source boundary and rejects externally changed Props/OPI identities.
OperationInfoCleanupOutcome attemptOperationInfoPropsCleanup(
    Operation&, OperationInfoCleanupProgress&);

// 0506ae ->05f980, only after this progress completed the Props-null skip.
// Read the actual owned Operation.engineModel cache, NEVER lazy opModel or an
// editor/class borrowed model. Null05f99a skips without deletion/assignment and
// stops before0506bb ->061180. Nonnull stops before virtual+18 at05f9d3.
// Retry validates the reached cache identity and never turns an external reset
// into a completed native deletion. No effects or OPI expiry are performed.
OperationInfoCleanupOutcome attemptOperationInfoEngineCacheCleanup(
    Operation&, OperationInfoCleanupProgress&);

// 0506bb ->061180. After this progress's Props/cache null skips, only an actual
// empty Operation.objs list takes061195 ->061207 with no list/item mutation.
// Nonempty (including null entries) stops before0611a2 element/container work:
// native removal precedes the null test and0658a0 deletion, and is not replayed
// or reordered here. Empty advances only to0506c8 ->061f70 position guard.
OperationInfoCleanupOutcome attemptOperationInfoObjectListCleanup(
    Operation&, OperationInfoCleanupProgress&);

// 0506c8 ->061f70. Requires the explicit Props/cache-null and empty-object steps.
// Actual empty Operation.objpos takes061f85 ->061f98 without entry/list effects;
// nonempty (including null entries) stops before061f91 ->061ec0(this,1).
// Empty advances only to0506d5's recovery+20 condition, not wrapper deletion.
OperationInfoCleanupOutcome attemptOperationInfoPositionListCleanup(
    Operation&, OperationInfoCleanupProgress&);

// 0506d5 tests the actual Operation.recoveryB wrapper pointer, after all explicit
// preceding null/empty steps. Null0506da ->05072e skips deletion AND the050726
// owner-null assignment; stop before050736's +28 condition. Allocated empty is
// nonnull and stops before direct deleting wrapper065440 at050709. No reset,
// entry inspection, flags read, native destructor completion or OPI expiry.
OperationInfoCleanupOutcome attemptOperationInfoRecoveryWrapperCleanup(
    Operation&, OperationInfoCleanupProgress&);

// 050736 tests actual Operation.recoveryFlags after the explicit null-B step.
// Null05073b ->05078f skips direct065400 deletion AND050787 owner assignment;
// stop before05079e's +160 guard read. Every nonnull wrapper, including empty,
// stays before05076a. No reset, entry read, item-model knowledge or OPI expiry.
OperationInfoCleanupOutcome attemptOperationInfoRecoveryFlagsCleanup(
    Operation&, OperationInfoCleanupProgress&);

} // namespace engine
} // namespace m2m
