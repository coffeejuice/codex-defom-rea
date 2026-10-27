#pragma once

#include "type3_shapeoptim_identity_view.hpp"

namespace m2m::mst::detail {

// One explicitly supplied stable registry. Exact UTF-16 key equality is a
// prepared convention, not QString comparison or native QMap behavior.
using Type3ShapeOptimPreparedRegistry = std::map<
    std::u16string, Type3ShapeOptimOptionalIdentity>;

enum class Type3ShapeOptimRegistryStatus {
    Selected, MissingClassKey, NullClass, InvalidPreparedIdentity,
};

struct Type3ShapeOptimRegistrySelection {
    Type3ShapeOptimRegistryStatus status;
    Type3ShapeOptimOptionalIdentity selectedClass;
};

// Private CP460 registry/selection postcondition: resolve the operation-owned
// key in this const registry and set only that class's currentOperation.
// Missing keys, present null values and malformed identities have no effects.
// Linked B, operation P, all data and scalar/list state remain independent.
// Requires stable prepared inputs; no manager identity, QMap detach/insertion,
// Qt guards, lifetime, native wrapper's external work/two selections, callbacks
// or public Type3 admission is modelled.
Type3ShapeOptimRegistrySelection selectType3ShapeOptimRegistryOperation(
    Type3ShapeOptimIdentityGraph& graph,
    const Type3ShapeOptimPreparedRegistry& registry,
    Type3ShapeOptimIdentity operation);

}  // namespace m2m::mst::detail
