#include "m2m/engine/operation_info_cleanup.hpp"

#include "m2m/common.hpp"
#include "m2m/model.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("OperationInfo Props cleanup: ") + detail);
}
}

OperationInfoCleanupProgress::OperationInfoCleanupProgress(Operation& operation)
    : identity_(operation.operationInfoIdentity().guard())
{
    if (identity_.get() != &operation)
        unavailable("progress requires a live OPI identity");
}

OperationInfoCleanupOutcome attemptOperationInfoPropsCleanup(
    Operation& operation, OperationInfoCleanupProgress& progress)
{
    // Expired metadata stays null even if a later allocation reuses its address.
    if (progress.identity_.get() != &operation ||
        operation.operationInfoIdentity().get() != &operation)
        unavailable("progress does not bind this live OPI identity");

    const auto* props = operation.props.get();
    switch (progress.stage_) {
    case OperationInfoCleanupStage::InspectProps:
        progress.observedProps_ = props;
        if (props) {
            progress.stage_ = OperationInfoCleanupStage::PropsVirtualDeleteBlocked;
            return OperationInfoCleanupOutcome::NonNullPropsVirtualDeleteUnavailable;
        }
        // 0512ce/0512d3 null skip to051327; caller then reaches0506ae.
        progress.stage_ = OperationInfoCleanupStage::EngineModelBoundary;
        return OperationInfoCleanupOutcome::NullPropsSkipReached;
    case OperationInfoCleanupStage::PropsVirtualDeleteBlocked:
        if (props != progress.observedProps_)
            unavailable("Props changed since the guarded virtual-delete boundary");
        return OperationInfoCleanupOutcome::NonNullPropsVirtualDeleteUnavailable;
    case OperationInfoCleanupStage::EngineModelBoundary:
        if (props)
            unavailable("Props appeared after the source null skip");
        // Preserve the original Props-only API's stop. Its separate cache
        // continuation is opt-in and cannot be reached by replaying this step.
        return OperationInfoCleanupOutcome::EngineModelCleanupUnavailable;
    case OperationInfoCleanupStage::EngineModelVirtualDeleteBlocked:
    case OperationInfoCleanupStage::ObjectCleanupBoundary:
    case OperationInfoCleanupStage::ObjectListEffectsBlocked:
    case OperationInfoCleanupStage::PositionCleanupBoundary:
    case OperationInfoCleanupStage::PositionListEffectsBlocked:
    case OperationInfoCleanupStage::RecoveryCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryWrapperDeleteBlocked:
    case OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked:
    case OperationInfoCleanupStage::ItemModelCacheBoundary:
        return attemptOperationInfoEngineCacheCleanup(operation, progress);
    }
    unavailable("unknown adapter continuation stage");
}

OperationInfoCleanupOutcome attemptOperationInfoEngineCacheCleanup(
    Operation& operation, OperationInfoCleanupProgress& progress)
{
    if (progress.identity_.get() != &operation ||
        operation.operationInfoIdentity().get() != &operation)
        unavailable("progress does not bind this live OPI identity");

    if (progress.stage_ == OperationInfoCleanupStage::InspectProps ||
        progress.stage_ == OperationInfoCleanupStage::PropsVirtualDeleteBlocked)
        unavailable("engine cache cleanup requires the completed Props-null skip");
    if (operation.props)
        unavailable("Props appeared after the source null skip");

    const auto& cache = operation.engineModel;
    switch (progress.stage_) {
    case OperationInfoCleanupStage::EngineModelBoundary:
        if (cache) {
            progress.observedEngineCache_ = cache;
            progress.observedEngineCacheToken_ = reinterpret_cast<std::uintptr_t>(cache.get());
            progress.stage_ = OperationInfoCleanupStage::EngineModelVirtualDeleteBlocked;
            return OperationInfoCleanupOutcome::NonNullEngineCacheVirtualDeleteUnavailable;
        }
        // 05f99a jumps directly to05f9e6: no reset/delete/null assignment.
        progress.stage_ = OperationInfoCleanupStage::ObjectCleanupBoundary;
        return OperationInfoCleanupOutcome::NullEngineCacheSkipReached;
    case OperationInfoCleanupStage::EngineModelVirtualDeleteBlocked:
        if (!cache || reinterpret_cast<std::uintptr_t>(cache.get()) != progress.observedEngineCacheToken_ ||
            !operation.sameEngineModelOwner(progress.observedEngineCache_))
            unavailable("Engine cache changed since the guarded virtual-delete boundary");
        return OperationInfoCleanupOutcome::NonNullEngineCacheVirtualDeleteUnavailable;
    case OperationInfoCleanupStage::ObjectCleanupBoundary:
        if (cache) unavailable("Engine cache appeared after the source null skip");
        return OperationInfoCleanupOutcome::ObjectCleanupUnavailable;
    case OperationInfoCleanupStage::ObjectListEffectsBlocked:
    case OperationInfoCleanupStage::PositionCleanupBoundary:
    case OperationInfoCleanupStage::PositionListEffectsBlocked:
    case OperationInfoCleanupStage::RecoveryCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryWrapperDeleteBlocked:
    case OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked:
    case OperationInfoCleanupStage::ItemModelCacheBoundary:
        return attemptOperationInfoObjectListCleanup(operation, progress);
    case OperationInfoCleanupStage::InspectProps:
    case OperationInfoCleanupStage::PropsVirtualDeleteBlocked:
        break; // rejected before the cache read above
    }
    unavailable("unknown adapter continuation stage");
}

OperationInfoCleanupOutcome attemptOperationInfoObjectListCleanup(
    Operation& operation, OperationInfoCleanupProgress& progress)
{
    if (progress.identity_.get() != &operation ||
        operation.operationInfoIdentity().get() != &operation)
        unavailable("progress does not bind this live OPI identity");

    switch (progress.stage_) {
    case OperationInfoCleanupStage::ObjectCleanupBoundary:
    case OperationInfoCleanupStage::ObjectListEffectsBlocked:
    case OperationInfoCleanupStage::PositionCleanupBoundary:
    case OperationInfoCleanupStage::PositionListEffectsBlocked:
    case OperationInfoCleanupStage::RecoveryCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryWrapperDeleteBlocked:
    case OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked:
    case OperationInfoCleanupStage::ItemModelCacheBoundary:
        break;
    default:
        unavailable("object cleanup requires the completed Props/cache-null skips");
    }
    if (operation.props) unavailable("Props appeared after the source null skip");
    if (operation.engineModel) unavailable("Engine cache appeared after the source null skip");

    const auto count = operation.objs.size();
    switch (progress.stage_) {
    case OperationInfoCleanupStage::ObjectCleanupBoundary:
        if (count != 0) {
            progress.observedObjectCount_ = count;
            progress.stage_ = OperationInfoCleanupStage::ObjectListEffectsBlocked;
            return OperationInfoCleanupOutcome::NonemptyObjectsEffectsUnavailable;
        }
        // 061195 zero-count jump returns before first-element access/removal.
        progress.stage_ = OperationInfoCleanupStage::PositionCleanupBoundary;
        return OperationInfoCleanupOutcome::EmptyObjectsSkipReached;
    case OperationInfoCleanupStage::ObjectListEffectsBlocked:
        if (count != progress.observedObjectCount_)
            unavailable("Object list changed since the nonempty cleanup boundary");
        return OperationInfoCleanupOutcome::NonemptyObjectsEffectsUnavailable;
    case OperationInfoCleanupStage::PositionCleanupBoundary:
        if (count != 0) unavailable("Objects appeared after the source empty-list skip");
        return OperationInfoCleanupOutcome::PositionCleanupUnavailable;
    case OperationInfoCleanupStage::PositionListEffectsBlocked:
    case OperationInfoCleanupStage::RecoveryCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryWrapperDeleteBlocked:
    case OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked:
    case OperationInfoCleanupStage::ItemModelCacheBoundary:
        if (count != 0) unavailable("Objects appeared after the source empty-list skip");
        return attemptOperationInfoPositionListCleanup(operation, progress);
    default:
        break; // rejected before any payload/list read above
    }
    unavailable("unknown adapter continuation stage");
}

OperationInfoCleanupOutcome attemptOperationInfoPositionListCleanup(
    Operation& operation, OperationInfoCleanupProgress& progress)
{
    if (progress.identity_.get() != &operation ||
        operation.operationInfoIdentity().get() != &operation)
        unavailable("progress does not bind this live OPI identity");

    switch (progress.stage_) {
    case OperationInfoCleanupStage::PositionCleanupBoundary:
    case OperationInfoCleanupStage::PositionListEffectsBlocked:
    case OperationInfoCleanupStage::RecoveryCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryWrapperDeleteBlocked:
    case OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked:
    case OperationInfoCleanupStage::ItemModelCacheBoundary:
        break;
    default:
        unavailable("position cleanup requires the completed Props/cache-null and empty-object steps");
    }
    if (operation.props) unavailable("Props appeared after the source null skip");
    if (operation.engineModel) unavailable("Engine cache appeared after the source null skip");
    if (!operation.objs.empty()) unavailable("Objects appeared after the source empty-list skip");

    const auto count = operation.objpos.size();
    switch (progress.stage_) {
    case OperationInfoCleanupStage::PositionCleanupBoundary:
        if (count != 0) {
            progress.observedPositionCount_ = count;
            progress.stage_ = OperationInfoCleanupStage::PositionListEffectsBlocked;
            return OperationInfoCleanupOutcome::NonemptyPositionsEffectsUnavailable;
        }
        // 061f85 zero-count branch returns before061ec0's removal/delete path.
        progress.stage_ = OperationInfoCleanupStage::RecoveryCleanupBoundary;
        return OperationInfoCleanupOutcome::EmptyPositionsSkipReached;
    case OperationInfoCleanupStage::PositionListEffectsBlocked:
        if (count != progress.observedPositionCount_)
            unavailable("Position list changed since the nonempty cleanup boundary");
        return OperationInfoCleanupOutcome::NonemptyPositionsEffectsUnavailable;
    case OperationInfoCleanupStage::RecoveryCleanupBoundary:
        if (count != 0) unavailable("Positions appeared after the source empty-list skip");
        return OperationInfoCleanupOutcome::RecoveryCleanupUnavailable;
    case OperationInfoCleanupStage::RecoveryWrapperDeleteBlocked:
    case OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked:
    case OperationInfoCleanupStage::ItemModelCacheBoundary:
        if (count != 0) unavailable("Positions appeared after the source empty-list skip");
        return attemptOperationInfoRecoveryWrapperCleanup(operation, progress);
    default:
        break; // rejected before any payload/list read above
    }
    unavailable("unknown adapter continuation stage");
}

OperationInfoCleanupOutcome attemptOperationInfoRecoveryWrapperCleanup(
    Operation& operation, OperationInfoCleanupProgress& progress)
{
    if (progress.identity_.get() != &operation ||
        operation.operationInfoIdentity().get() != &operation)
        unavailable("progress does not bind this live OPI identity");

    switch (progress.stage_) {
    case OperationInfoCleanupStage::RecoveryCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryWrapperDeleteBlocked:
    case OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked:
    case OperationInfoCleanupStage::ItemModelCacheBoundary:
        break;
    default:
        unavailable("recovery wrapper cleanup requires all explicit preceding null/empty steps");
    }
    if (operation.props) unavailable("Props appeared after the source null skip");
    if (operation.engineModel) unavailable("Engine cache appeared after the source null skip");
    if (!operation.objs.empty()) unavailable("Objects appeared after the source empty-list skip");
    if (!operation.objpos.empty()) unavailable("Positions appeared after the source empty-list skip");

    const auto& wrapper = operation.recoveryB;
    switch (progress.stage_) {
    case OperationInfoCleanupStage::RecoveryCleanupBoundary:
        if (wrapper) {
            progress.observedRecoveryWrapper_ = wrapper;
            progress.observedRecoveryWrapperToken_ = reinterpret_cast<std::uintptr_t>(wrapper.get());
            progress.stage_ = OperationInfoCleanupStage::RecoveryWrapperDeleteBlocked;
            return OperationInfoCleanupOutcome::NonNullRecoveryWrapperDeleteUnavailable;
        }
        // Null0506da ->05072e bypasses both065440 and050726's owner assignment.
        progress.stage_ = OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary;
        return OperationInfoCleanupOutcome::NullRecoveryWrapperSkipReached;
    case OperationInfoCleanupStage::RecoveryWrapperDeleteBlocked:
        if (!wrapper || reinterpret_cast<std::uintptr_t>(wrapper.get()) != progress.observedRecoveryWrapperToken_ ||
            wrapper.owner_before(progress.observedRecoveryWrapper_) || progress.observedRecoveryWrapper_.owner_before(wrapper))
            unavailable("Recovery wrapper changed since the guarded deleting-wrapper boundary");
        return OperationInfoCleanupOutcome::NonNullRecoveryWrapperDeleteUnavailable;
    case OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary:
        if (wrapper) unavailable("Recovery wrapper appeared after the source null skip");
        return OperationInfoCleanupOutcome::RecoveryFlagsCleanupUnavailable;
    case OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked:
    case OperationInfoCleanupStage::ItemModelCacheBoundary:
        if (wrapper) unavailable("Recovery wrapper appeared after the source null skip");
        return attemptOperationInfoRecoveryFlagsCleanup(operation, progress);
    default:
        break; // rejected before payload/owner reads above
    }
    unavailable("unknown adapter continuation stage");
}

OperationInfoCleanupOutcome attemptOperationInfoRecoveryFlagsCleanup(
    Operation& operation, OperationInfoCleanupProgress& progress)
{
    if (progress.identity_.get() != &operation ||
        operation.operationInfoIdentity().get() != &operation)
        unavailable("progress does not bind this live OPI identity");

    switch (progress.stage_) {
    case OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary:
    case OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked:
    case OperationInfoCleanupStage::ItemModelCacheBoundary:
        break;
    default:
        unavailable("recovery flags cleanup requires all explicit preceding null/empty steps including null recovery wrapper");
    }
    if (operation.props) unavailable("Props appeared after the source null skip");
    if (operation.engineModel) unavailable("Engine cache appeared after the source null skip");
    if (!operation.objs.empty()) unavailable("Objects appeared after the source empty-list skip");
    if (!operation.objpos.empty()) unavailable("Positions appeared after the source empty-list skip");
    if (operation.recoveryB) unavailable("Recovery wrapper appeared after the source null skip");

    const auto& flags = operation.recoveryFlags;
    switch (progress.stage_) {
    case OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary:
        if (flags) {
            progress.observedRecoveryFlags_ = flags;
            progress.observedRecoveryFlagsToken_ = reinterpret_cast<std::uintptr_t>(flags.get());
            progress.stage_ = OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked;
            return OperationInfoCleanupOutcome::NonNullRecoveryFlagsDeleteUnavailable;
        }
        // Null05073b ->05078f bypasses both065400 and050787's owner assignment.
        progress.stage_ = OperationInfoCleanupStage::ItemModelCacheBoundary;
        return OperationInfoCleanupOutcome::NullRecoveryFlagsSkipReached;
    case OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked:
        if (!flags || reinterpret_cast<std::uintptr_t>(flags.get()) != progress.observedRecoveryFlagsToken_ ||
            flags.owner_before(progress.observedRecoveryFlags_) || progress.observedRecoveryFlags_.owner_before(flags))
            unavailable("Recovery flags changed since the guarded deleting-wrapper boundary");
        return OperationInfoCleanupOutcome::NonNullRecoveryFlagsDeleteUnavailable;
    case OperationInfoCleanupStage::ItemModelCacheBoundary:
        if (flags) unavailable("Recovery flags appeared after the source null skip");
        return OperationInfoCleanupOutcome::ItemModelCacheCleanupUnavailable;
    default:
        break; // rejected before payload/owner reads above
    }
    unavailable("unknown adapter continuation stage");
}
} // namespace m2m::engine
