#include "type3_shapeoptim_property_tail.hpp"

namespace m2m::mst::detail {

Type3ShapeOptimPropertyTailResult projectType3ShapeOptimPropertyTail(
    const Type3ShapeOptimPropertyValues& currentOb,
    const Type3ShapeOptimPropertyCast* castProperties,
    const Type3ShapeOptimPropertyRefresh* refresh,
    std::vector<Type3ShapeOptimPropertyEvent>& events)
{
    if (castProperties) {
        for (const auto& top : castProperties->properties) {
            events.push_back({Type3ShapeOptimPropertyEventKind::DebugTopDisplayName,
                              top.displayName});
            if (top.displayName != "StopCritGroup") continue;
            for (const auto& child : top.children) {
                if (child.displayName == "MaxSims") {
                    if (!castProperties->data)
                        return Type3ShapeOptimPropertyTailResult::MissingPropertyData;
                    const auto value = currentOb.maxSims;
                    castProperties->data->maxSims = value;
                    events.push_back({Type3ShapeOptimPropertyEventKind::SetMaxSims,
                                      {}, value});
                } else if (child.displayName == "MinObjFuncVal") {
                    if (!castProperties->data)
                        return Type3ShapeOptimPropertyTailResult::MissingPropertyData;
                    const auto value = currentOb.minObjective;
                    castProperties->data->minObjective = value;
                    events.push_back({Type3ShapeOptimPropertyEventKind::SetMinObjective,
                                      {}, 0, value});
                }
            }
        }
    }
    if (refresh && refresh->linkedClass) {
        if (!refresh->data)
            return Type3ShapeOptimPropertyTailResult::MissingRefreshData;
        const auto maxSims = refresh->linkedClass->maxSims;
        refresh->data->maxSims = maxSims;
        events.push_back({Type3ShapeOptimPropertyEventKind::RefreshMaxSims,
                          {}, maxSims});
        const auto minObjective = refresh->linkedClass->minObjective;
        refresh->data->minObjective = minObjective;
        events.push_back({Type3ShapeOptimPropertyEventKind::RefreshMinObjective,
                          {}, 0, minObjective});
    }
    return Type3ShapeOptimPropertyTailResult::Complete;
}

}  // namespace m2m::mst::detail
