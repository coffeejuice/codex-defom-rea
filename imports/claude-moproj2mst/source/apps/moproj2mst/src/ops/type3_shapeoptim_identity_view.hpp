#pragma once

#include "type3_shapeoptim_property_tail.hpp"

#include <map>
#include <optional>

namespace m2m::mst::detail {

// Stable prepared identities, scoped to their respective maps. These are not
// addresses, native handles, Qt guards, ownership, or object lifetime models.
using Type3ShapeOptimIdentity = std::uint64_t;
using Type3ShapeOptimOptionalIdentity = std::optional<Type3ShapeOptimIdentity>;

struct Type3ShapeOptimIdentityClass {
    Type3ShapeOptimPropertyValues values;
    Type3ShapeOptimOptionalIdentity currentOperation;  // OB+20
    Type3ShapeOptimOptionalIdentity linkedProperty;    // OB+b0
};

struct Type3ShapeOptimIdentityOperation {
    Type3ShapeOptimOptionalIdentity property;  // OPI+10
};

struct Type3ShapeOptimIdentityProperty {
    bool castSuccess = true;
    Type3ShapeOptimOptionalIdentity creator;  // B+40, independent of current OB
    Type3ShapeOptimOptionalIdentity data;     // B/P+38
    std::vector<Type3ShapeOptimPropertyNode> properties;
};

struct Type3ShapeOptimIdentityGraph {
    std::map<Type3ShapeOptimIdentity, Type3ShapeOptimIdentityClass> classes;
    std::map<Type3ShapeOptimIdentity, Type3ShapeOptimIdentityOperation> operations;
    std::map<Type3ShapeOptimIdentity, Type3ShapeOptimIdentityProperty> properties;
    std::map<Type3ShapeOptimIdentity, Type3ShapeOptimPropertyValues> data;
};

// Explicit caller-supplied prepared postconditions, not native setters or a
// simulation of factories/Qt callbacks. Construction supplies a fresh property
// and data identity with recovered defaults {200, 0.001}; link publication,
// operation publication and current selection remain independent choices.
// False means unknown referenced identity or an occupied fresh identity, with
// no change. Null represents supplied absence, never inferred invalidation.
bool prepareType3ShapeOptimConstruction(
    Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity creator,
    Type3ShapeOptimIdentity property, Type3ShapeOptimIdentity data,
    std::vector<Type3ShapeOptimPropertyNode> properties);
bool prepareType3ShapeOptimLinkedProperty(
    Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity currentClass,
    Type3ShapeOptimOptionalIdentity property);
bool prepareType3ShapeOptimOperationProperty(
    Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity operation,
    Type3ShapeOptimOptionalIdentity suppliedFactoryReturn);
bool prepareType3ShapeOptimCurrentOperation(
    Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity currentClass,
    Type3ShapeOptimOptionalIdentity operation);

enum class Type3ShapeOptimIdentityResult {
    Complete,
    InvalidPreparedIdentity,
    MissingPropertyData,
    MissingRefreshData,
};

// Private CP459 adapter to CP455: P follows currentClass.currentOperation ->
// operation.property; B independently follows currentClass.linkedProperty.
// B's refresh reads B.creator and writes B.data. Equal data IDs preserve aliasing.
// Validates the selected prepared graph before invoking the tail; malformed IDs
// refuse without effects. Null data is passed through for delayed CP455 refusal.
// No callbacks, mutation during projection, Qt fault/lifetime behavior, disposal,
// imported project identity history, or public Type3 admission is established.
Type3ShapeOptimIdentityResult projectType3ShapeOptimIdentityView(
    Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity currentClass,
    std::vector<Type3ShapeOptimPropertyEvent>& events);

}  // namespace m2m::mst::detail
