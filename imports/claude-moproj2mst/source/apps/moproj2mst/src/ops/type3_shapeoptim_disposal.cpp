#include "type3_shapeoptim_disposal.hpp"

namespace m2m::mst::detail {

Type3ShapeOptimDisposalProjection projectType3ShapeOptimDisposal(
    const Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity operation,
    const Type3ShapeOptimDisposalAssumptions& assumptions,
    std::optional<Type3ShapeOptimDisposalEvent> refuseBefore)
{
    using Event = Type3ShapeOptimDisposalEvent;
    Type3ShapeOptimDisposalProjection out;
    const auto op = graph.operations.find(operation);
    if (op == graph.operations.end()) {
        out.result = Type3ShapeOptimDisposalResult::InvalidPreparedIdentity;
        return out;
    }
    out.operationProperty = op->second.property;
    if (!out.operationProperty) return out;
    const auto property = graph.properties.find(*out.operationProperty);
    if (property == graph.properties.end() ||
        (property->second.data && graph.data.find(*property->second.data) == graph.data.end()) ||
        (!assumptions.qobjectHasGuards && !assumptions.guardDestinations.empty())) {
        out.result = Type3ShapeOptimDisposalResult::InvalidPreparedIdentity;
        return out;
    }
    out.property = property->first;
    out.data = property->second.data;
    auto emit = [&](Event event) {
        if (refuseBefore == event) {
            out.result = Type3ShapeOptimDisposalResult::Refused;
            return false;
        }
        out.events.push_back(event);
        return true;
    };
    if (!emit(Event::DerivedVtables)) return out;
    if (assumptions.oldTreeRequiresRelease && !emit(Event::TreeRelease)) return out;
    if (!emit(Event::DerivedListPublished)) return out;
    out.listPublishedEmpty = true;
    if (!emit(Event::BaseVtables)) return out;
    if (!emit(Event::BaseListReset)) return out;
    if (!emit(Event::OperationGuardRemoved)) return out;
    out.operationGuardRemoved = true;
    if (!emit(Event::SecondaryListReset)) return out;
    if (!emit(Event::SecondaryListDestroyed)) return out;
    out.secondaryListDestroyed = true;
    if (assumptions.qobjectHasGuards) {
        if (!emit(Event::QObjectGuardClear)) return out;
        out.clearedGuards = assumptions.guardDestinations;
    }
    if (assumptions.qobjectHasChildren && !emit(Event::QObjectChildCleanup)) return out;
    if (!emit(Event::QObjectCompletion)) return out;
    out.qobjectCompleted = true;
    if (!emit(Event::PropertyReleased)) return out;
    out.propertyReleased = true;
    if (!emit(Event::OperationPropertyCleared)) return out;
    out.operationProperty.reset();
    return out;
}

}  // namespace m2m::mst::detail
