#include "type3_shapeoptim_initializer.hpp"

namespace m2m::mst::detail {

Type3ShapeOptimInitResult initializeType3ShapeOptimPreparedState(
    Type3ShapeOptimPreparedState& state,
    std::vector<Type3ShapeOptimInitEvent>& events,
    std::optional<Type3ShapeOptimInitList> refuseBefore)
{
    using Kind = Type3ShapeOptimInitEventKind;
    using Result = Type3ShapeOptimInitResult;
    using List = Type3ShapeOptimInitList;
    const auto store = [&events](auto& destination, auto value, std::uint16_t offset) {
        destination = value;
        events.push_back({Kind::Store, offset, destination});
    };
    store(state.value6c, std::int32_t{1}, 0x6c);
    store(state.valuec4, std::int32_t{1}, 0xc4);
    store(state.valuec0, std::int32_t{1022}, 0xc0);
    store(state.valuec8, std::int32_t{0}, 0xc8);
    store(state.values.maxSims, std::int32_t{200}, 0xcc);
    store(state.values.minObjective, 0.001, 0xe0);
    store(state.valuee8, std::uint8_t{0}, 0xe8);
    store(state.valuee9, std::uint8_t{0}, 0xe9);
    store(state.valued0, std::int32_t{1}, 0xd0);
    store(state.value88, 0.0, 0x88);
    store(state.value90, 0.0, 0x90);
    store(state.value98, std::uint8_t{0}, 0x98);
    store(state.value99, std::uint8_t{0}, 0x99);
    store(state.value9a, std::uint8_t{0}, 0x9a);
    store(state.value9b, std::uint8_t{0}, 0x9b);
    store(state.value9c, std::uint8_t{0}, 0x9c);
    store(state.value9d, std::uint8_t{0}, 0x9d);
    store(state.value70, std::int32_t{1}, 0x70);
    store(state.value74, std::int32_t{1}, 0x74);
    store(state.value78, std::int32_t{1}, 0x78);
    store(state.value7c, std::int32_t{0}, 0x7c);
    store(state.value80, std::int32_t{0}, 0x80);
    store(state.value84, std::int32_t{0}, 0x84);

    if (refuseBefore == List::A0) return Result::RefusedBeforeA0;
    state.listA0.clear();
    events.push_back({Kind::ListClear, 0xa0, std::monostate{}});
    if (refuseBefore == List::A8) return Result::RefusedBeforeA8;
    state.listA8.clear();
    events.push_back({Kind::ListClear, 0xa8, std::monostate{}});
    if (refuseBefore == List::F0) return Result::RefusedBeforeF0;
    state.listF0.clear();
    events.push_back({Kind::ListClear, 0xf0, std::monostate{}});
    return Result::Complete;
}

}  // namespace m2m::mst::detail
