#include "type3_shapeoptim_prepared_view.hpp"

namespace m2m::mst::detail {
namespace {

std::int32_t derivedValuec4(std::int32_t x) noexcept
{
    if (x >= 10000) return x % 100;
    const auto remainder = x % 1021;
    if (remainder < 3) return remainder;
    if (x == 1055 || x == 1056 || x == 1057) return 0;
    return x % 10;
}

std::int32_t clampCount(std::int32_t value) noexcept
{
    return value < 1 ? 1 : value;
}

}  // namespace

void projectType3ShapeOptimPreparedView(
    const Type3OptimizerDefOptView& view,
    Type3ShapeOptimPreparedState& state)
{
    state.values.maxSims = view.value38();
    state.valued8 = view.value0c();
    state.valued4 = view.value10();
    state.valued0 = view.value34();
    state.valuee8 = state.valued0 > 0;
    state.valuec0 = view.value30();
    state.valuec4 = derivedValuec4(state.valuec0);

    state.value98 = 0;
    state.value99 = 0;
    state.value9a = 0;
    state.value9b = 0;
    const auto rowCount = view.numConCount();
    for (std::int32_t index = 0; index < rowCount; ++index) {
        const auto& row = *view.numConAt(index);
        switch (row.value04) {
        case 0x3f2:
        case 0x3f4:
        case 0x438:
            if (row.value04 == 0x3f2) state.value98 = 1;
            else if (row.value04 == 0x3f4) state.value99 = 1;
            else state.value9a = 1;
            state.value74 = row.value0c;
            state.value9d = state.value74 > 0;
            state.value80 = clampCount(row.value08);
            break;
        case 0x439:
            state.value9b = 1;
            state.value78 = row.value0c;
            state.value9e = state.value74 > 0;
            state.value84 = clampCount(row.value08);
            break;
        default:
            state.listA0.push_back(row.value04);
            state.listA8.push_back(row.value18);
            state.value70 = row.value0c;
            state.value9c = state.value70 > 0;
            state.value7c = clampCount(row.value08);
            break;
        }
    }

    const auto regionCount = view.regionCount();
    for (std::int32_t index = 0; index < regionCount; ++index)
        state.listF0.push_back(view.regionNameAt(index));
    state.values.minObjective = view.value40();
    state.valuee9 = view.value48();
}

}  // namespace m2m::mst::detail
