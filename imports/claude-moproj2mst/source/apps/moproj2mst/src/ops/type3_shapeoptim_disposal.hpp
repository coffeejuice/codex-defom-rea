#pragma once

#include "type3_shapeoptim_identity_view.hpp"

#include <optional>
#include <vector>

namespace m2m::mst::detail {

// Caller-supplied stable prepared profile assumes opaque tree and Qt calls
// return, preserve D and the selected graph, and do not reenter. Only supplied
// guard destinations are projected as cleared. D retention is a model
// assumption; its native survival is unknown.
struct Type3ShapeOptimDisposalAssumptions {
    bool oldTreeRequiresRelease = false;
    bool qobjectHasGuards = false;
    bool qobjectHasChildren = false;
    std::vector<Type3ShapeOptimIdentity> guardDestinations;
};

enum class Type3ShapeOptimDisposalEvent {
    DerivedVtables, TreeRelease, DerivedListPublished, BaseVtables,
    BaseListReset, OperationGuardRemoved, SecondaryListReset,
    SecondaryListDestroyed, QObjectGuardClear, QObjectChildCleanup,
    QObjectCompletion, PropertyReleased, OperationPropertyCleared
};

enum class Type3ShapeOptimDisposalResult { Complete, InvalidPreparedIdentity, Refused };

struct Type3ShapeOptimDisposalProjection {
    Type3ShapeOptimDisposalResult result = Type3ShapeOptimDisposalResult::Complete;
    Type3ShapeOptimOptionalIdentity operationProperty;
    Type3ShapeOptimOptionalIdentity property;
    Type3ShapeOptimOptionalIdentity data;
    bool listPublishedEmpty = false;
    bool operationGuardRemoved = false;
    bool secondaryListDestroyed = false;
    bool qobjectCompleted = false;
    bool propertyReleased = false;
    std::vector<Type3ShapeOptimIdentity> clearedGuards;
    std::vector<Type3ShapeOptimDisposalEvent> events;
};

// Refusal is a host-injected boundary before the named event; it is not a
// native return code or exception model. Graph and data are never modified.
Type3ShapeOptimDisposalProjection projectType3ShapeOptimDisposal(
    const Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity operation,
    const Type3ShapeOptimDisposalAssumptions& assumptions,
    std::optional<Type3ShapeOptimDisposalEvent> refuseBefore = std::nullopt);

}  // namespace m2m::mst::detail
