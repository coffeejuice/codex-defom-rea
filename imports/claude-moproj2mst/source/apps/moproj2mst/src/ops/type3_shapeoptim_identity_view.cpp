#include "type3_shapeoptim_identity_view.hpp"

#include <utility>

namespace m2m::mst::detail {
namespace {

template <class Map>
bool validOptional(const Map& map, Type3ShapeOptimOptionalIdentity id)
{
    return !id || map.find(*id) != map.end();
}

bool validProperty(const Type3ShapeOptimIdentityGraph& graph,
                   const Type3ShapeOptimIdentityProperty& property)
{
    return validOptional(graph.classes, property.creator)
        && validOptional(graph.data, property.data);
}

}  // namespace

bool prepareType3ShapeOptimConstruction(
    Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity creator,
    Type3ShapeOptimIdentity property, Type3ShapeOptimIdentity data,
    std::vector<Type3ShapeOptimPropertyNode> properties)
{
    if (graph.classes.find(creator) == graph.classes.end()
        || graph.properties.find(property) != graph.properties.end()
        || graph.data.find(data) != graph.data.end()) return false;
    graph.data.emplace(data, Type3ShapeOptimPropertyValues{200, 0.001});
    graph.properties.emplace(property, Type3ShapeOptimIdentityProperty{
        true, creator, data, std::move(properties)});
    return true;
}

bool prepareType3ShapeOptimLinkedProperty(
    Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity currentClass,
    Type3ShapeOptimOptionalIdentity property)
{
    const auto current = graph.classes.find(currentClass);
    if (current == graph.classes.end() || !validOptional(graph.properties, property))
        return false;
    current->second.linkedProperty = property;
    return true;
}

bool prepareType3ShapeOptimOperationProperty(
    Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity operation,
    Type3ShapeOptimOptionalIdentity suppliedFactoryReturn)
{
    const auto current = graph.operations.find(operation);
    if (current == graph.operations.end()
        || !validOptional(graph.properties, suppliedFactoryReturn)) return false;
    current->second.property = suppliedFactoryReturn;
    return true;
}

bool prepareType3ShapeOptimCurrentOperation(
    Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity currentClass,
    Type3ShapeOptimOptionalIdentity operation)
{
    const auto current = graph.classes.find(currentClass);
    if (current == graph.classes.end() || !validOptional(graph.operations, operation))
        return false;
    current->second.currentOperation = operation;
    return true;
}

Type3ShapeOptimIdentityResult projectType3ShapeOptimIdentityView(
    Type3ShapeOptimIdentityGraph& graph, Type3ShapeOptimIdentity currentClass,
    std::vector<Type3ShapeOptimPropertyEvent>& events)
{
    using Result = Type3ShapeOptimIdentityResult;
    const auto current = graph.classes.find(currentClass);
    if (current == graph.classes.end()) return Result::InvalidPreparedIdentity;
    const auto& ob = current->second;
    if (!validOptional(graph.operations, ob.currentOperation)
        || !validOptional(graph.properties, ob.linkedProperty))
        return Result::InvalidPreparedIdentity;

    const Type3ShapeOptimIdentityProperty* p = nullptr;
    if (ob.currentOperation) {
        const auto property = graph.operations.at(*ob.currentOperation).property;
        if (!validOptional(graph.properties, property))
            return Result::InvalidPreparedIdentity;
        if (property) p = &graph.properties.at(*property);
    }
    const auto* b = ob.linkedProperty
        ? &graph.properties.at(*ob.linkedProperty) : nullptr;
    if ((p && !validProperty(graph, *p)) || (b && !validProperty(graph, *b)))
        return Result::InvalidPreparedIdentity;

    // Temporary pointer views exist only during this call. Map entries retain
    // stable identities; callers cannot supply dangling raw pointers.
    Type3ShapeOptimPropertyCast cast;
    if (p && p->castSuccess) {
        cast.properties = p->properties;
        cast.data = p->data ? &graph.data.at(*p->data) : nullptr;
    }
    Type3ShapeOptimPropertyRefresh refresh;
    if (b) {
        refresh.linkedClass = b->creator
            ? &graph.classes.at(*b->creator).values : nullptr;
        refresh.data = b->data ? &graph.data.at(*b->data) : nullptr;
    }
    const auto result = projectType3ShapeOptimPropertyTail(
        ob.values, p && p->castSuccess ? &cast : nullptr,
        b ? &refresh : nullptr, events);
    switch (result) {
    case Type3ShapeOptimPropertyTailResult::Complete: return Result::Complete;
    case Type3ShapeOptimPropertyTailResult::MissingPropertyData:
        return Result::MissingPropertyData;
    case Type3ShapeOptimPropertyTailResult::MissingRefreshData:
        return Result::MissingRefreshData;
    }
    return Result::InvalidPreparedIdentity;
}

}  // namespace m2m::mst::detail
