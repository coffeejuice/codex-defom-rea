#include "type3_region_sequence.hpp"

#include <limits>

#include "type3_region_value.hpp"

namespace m2m::mst::detail {

Type3RegionSequenceResult appendType3RegionRows(
    Type3OptimizerScalarState& state,
    std::int32_t count,
    std::span<const qt::String> rawRows)
{
    std::size_t consumed = 0;
    while (count > 0 && consumed < rawRows.size() &&
           consumed < static_cast<std::size_t>(count)) {
        if (state.regionRows.size() >=
            static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max())) {
            return {Type3RegionSequenceStop::RowCountUnrepresentable, consumed};
        }

        const qt::String value = deriveType3RegionValue(rawRows[consumed]);
        (void)appendType3RegionPreparedRow(state, value);
        ++consumed;
    }

    if (count > 0 && consumed < static_cast<std::size_t>(count))
        return {Type3RegionSequenceStop::InputEnded, consumed};
    return {Type3RegionSequenceStop::CountSatisfied, consumed};
}

}  // namespace m2m::mst::detail
