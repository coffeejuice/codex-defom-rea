#include "type3_shapeoptim_registry_view.hpp"

namespace m2m::mst::detail {

Type3ShapeOptimRegistrySelection selectType3ShapeOptimRegistryOperation(
    Type3ShapeOptimIdentityGraph& graph,
    const Type3ShapeOptimPreparedRegistry& registry,
    Type3ShapeOptimIdentity operation)
{
    using Status = Type3ShapeOptimRegistryStatus;
    const auto op = graph.operations.find(operation);
    if (op == graph.operations.end())
        return {Status::InvalidPreparedIdentity, std::nullopt};
    const auto entry = registry.find(op->second.classKey);
    if (entry == registry.end())
        return {Status::MissingClassKey, std::nullopt};
    if (!entry->second)
        return {Status::NullClass, std::nullopt};
    const auto selected = graph.classes.find(*entry->second);
    if (selected == graph.classes.end())
        return {Status::InvalidPreparedIdentity, std::nullopt};
    selected->second.currentOperation = operation;
    return {Status::Selected, entry->second};
}

}  // namespace m2m::mst::detail
