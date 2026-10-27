#pragma once

#include "type3_optimizer_defopt_view.hpp"
#include "type3_shapeoptim_property_tail.hpp"

namespace m2m::mst::detail {

// Prepared OB destinations shared by the private initializer, optimizer-present
// normal prefix of 0x14013d740, and property-tail identity adapter.
// Offset names identify recovered destinations, not this structure's memory layout.
struct Type3ShapeOptimPreparedState {
    std::int32_t value6c = 0;
    std::int32_t value70 = 0;
    std::int32_t value74 = 0;
    std::int32_t value78 = 0;
    std::int32_t value7c = 0;
    std::int32_t value80 = 0;
    std::int32_t value84 = 0;
    double value88 = 0;
    double value90 = 0;
    std::uint8_t value98 = 0;
    std::uint8_t value99 = 0;
    std::uint8_t value9a = 0;
    std::uint8_t value9b = 0;
    std::uint8_t value9c = 0;
    std::uint8_t value9d = 0;
    std::uint8_t value9e = 0;
    std::vector<std::int32_t> listA0;
    std::vector<double> listA8;
    std::int32_t valuec0 = 0;
    std::int32_t valuec4 = 0;
    std::int32_t valuec8 = 0;
    // Canonical OB+cc/+e0 values, also read by the private property tail.
    Type3ShapeOptimPropertyValues values;
    std::int32_t valued0 = 0;
    std::int32_t valued4 = 0;
    std::int32_t valued8 = 0;
    std::uint8_t valuee8 = 0;
    std::uint8_t valuee9 = 0;
    std::vector<qt::String> listF0;
};

// Mutates seeded state in recovered store/append order; never clears the lists.
// Requires stable prepared vectors with counts representable as signed int32_t.
// Excludes Qt storage/allocation/exception parity, owner lifetime, project/SUB
// admission, the property tail and GEO I/O. It is not a public Type3 load route.
void projectType3ShapeOptimPreparedView(
    const Type3OptimizerDefOptView& view,
    Type3ShapeOptimPreparedState& state);

}  // namespace m2m::mst::detail
