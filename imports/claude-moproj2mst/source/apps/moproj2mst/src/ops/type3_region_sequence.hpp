#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include "type3_optimizer_scalar_state.hpp"

namespace m2m::mst::detail {

enum class Type3RegionSequenceStop {
    CountSatisfied,
    InputEnded,
    RowCountUnrepresentable,
};

struct Type3RegionSequenceResult {
    Type3RegionSequenceStop stop = Type3RegionSequenceStop::CountSatisfied;
    std::size_t rowsConsumed = 0;
};

// Appends up to count already-read raw REGIONS row QStrings in order. Count is
// already converted; this does not read/split files or handle parser ownership.
Type3RegionSequenceResult appendType3RegionRows(
    Type3OptimizerScalarState& state,
    std::int32_t count,
    std::span<const qt::String> rawRows);

}  // namespace m2m::mst::detail
