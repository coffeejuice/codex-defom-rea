#pragma once

#include "type3_shapeoptim_prepared_view.hpp"

#include <optional>
#include <variant>

namespace m2m::mst::detail {

enum class Type3ShapeOptimInitEventKind { Store, ListClear };

struct Type3ShapeOptimInitEvent {
    Type3ShapeOptimInitEventKind kind;
    std::uint16_t offset;
    // monostate is used only for a completed list clear.
    std::variant<std::monostate, std::int32_t, double, std::uint8_t> value;
};

enum class Type3ShapeOptimInitList { A0, A8, F0 };
enum class Type3ShapeOptimInitResult {
    Complete, RefusedBeforeA0, RefusedBeforeA8, RefusedBeforeF0,
};

// CP461's stable private value projection of 14013f5a0. Appends completed
// effects in native store/clear order, preserving every unmentioned field.
// Optional refusal happens before the selected list clear, after all scalar
// stores and earlier clears. It is a private boundary, not a native error.
// Requires valid prepared destinations and no callbacks/concurrent mutation.
// Qt storage/refcounts, allocator/cache faults, unwind and public Type3
// admission are excluded; standard-container exceptions do not model them.
Type3ShapeOptimInitResult initializeType3ShapeOptimPreparedState(
    Type3ShapeOptimPreparedState& state,
    std::vector<Type3ShapeOptimInitEvent>& events,
    std::optional<Type3ShapeOptimInitList> refuseBefore = std::nullopt);

}  // namespace m2m::mst::detail
