// Stable private values and refusal prefixes only; no native Qt fault parity.
#include <testkit/check.hpp>

#include "../../src/ops/type3_shapeoptim_initializer.hpp"

using namespace m2m;
using namespace m2m::mst::detail;

namespace {
using Event = Type3ShapeOptimInitEvent;
using Kind = Type3ShapeOptimInitEventKind;
using Result = Type3ShapeOptimInitResult;
using List = Type3ShapeOptimInitList;

Type3ShapeOptimPreparedState seeded()
{
    Type3ShapeOptimPreparedState state;
    state.value6c = -6;
    state.value70 = 70; state.value74 = 74; state.value78 = 78;
    state.value7c = 7; state.value80 = 80; state.value84 = 84;
    state.value88 = -0.0; state.value90 = -90.5;
    state.value98 = 98; state.value99 = 99; state.value9a = 100;
    state.value9b = 101; state.value9c = 102; state.value9d = 103;
    state.value9e = 104;
    state.listA0 = {1, -2}; state.listA8 = {-0.0, 2.5};
    state.valuec0 = -1; state.valuec4 = -2; state.valuec8 = -3;
    state.values = {-4, -5.5}; state.valued0 = -6;
    state.valued4 = -7; state.valued8 = -8;
    state.valuee8 = 108; state.valuee9 = 109;
    state.listF0 = {qt::L("seed"), qt::L("other")};
    return state;
}

void checkScalars(const Type3ShapeOptimPreparedState& state)
{
    CHECK_EQ(state.value6c, 1); CHECK_EQ(state.valuec4, 1);
    CHECK_EQ(state.valuec0, 1022); CHECK_EQ(state.valuec8, 0);
    CHECK_EQ(state.values.maxSims, 200); CHECK_BITS(state.values.minObjective, 0.001);
    CHECK_EQ(state.valuee8, 0); CHECK_EQ(state.valuee9, 0); CHECK_EQ(state.valued0, 1);
    CHECK_BITS(state.value88, 0.0); CHECK_BITS(state.value90, 0.0);
    CHECK_EQ(state.value98, 0); CHECK_EQ(state.value99, 0); CHECK_EQ(state.value9a, 0);
    CHECK_EQ(state.value9b, 0); CHECK_EQ(state.value9c, 0); CHECK_EQ(state.value9d, 0);
    CHECK_EQ(state.value70, 1); CHECK_EQ(state.value74, 1); CHECK_EQ(state.value78, 1);
    CHECK_EQ(state.value7c, 0); CHECK_EQ(state.value80, 0); CHECK_EQ(state.value84, 0);
    CHECK_EQ(state.value9e, 104); CHECK_EQ(state.valued4, -7); CHECK_EQ(state.valued8, -8);
}

std::vector<Event> expected()
{
    const auto i = [](std::uint16_t offset, std::int32_t value) {
        return Event{Kind::Store, offset, value};
    };
    const auto b = [](std::uint16_t offset) {
        return Event{Kind::Store, offset, std::uint8_t{0}};
    };
    const auto d = [](std::uint16_t offset, double value) {
        return Event{Kind::Store, offset, value};
    };
    const auto clear = [](std::uint16_t offset) {
        return Event{Kind::ListClear, offset, std::monostate{}};
    };
    return {i(0x6c, 1), i(0xc4, 1), i(0xc0, 1022), i(0xc8, 0),
        i(0xcc, 200), d(0xe0, 0.001), b(0xe8), b(0xe9), i(0xd0, 1),
        d(0x88, 0.0), d(0x90, 0.0), b(0x98), b(0x99), b(0x9a),
        b(0x9b), b(0x9c), b(0x9d), i(0x70, 1), i(0x74, 1), i(0x78, 1),
        i(0x7c, 0), i(0x80, 0), i(0x84, 0), clear(0xa0), clear(0xa8), clear(0xf0)};
}

void checkEvents(const std::vector<Event>& actual, std::size_t start, std::size_t count)
{
    const auto wanted = expected();
    REQUIRE(actual.size() == start + count);
    for (std::size_t index = 0; index < count; ++index) {
        const auto& event = actual[start + index];
        CHECK(event.kind == wanted[index].kind);
        CHECK_EQ(event.offset, wanted[index].offset);
        CHECK(event.value == wanted[index].value);
    }
    CHECK_BITS(std::get<double>(actual[start + 9].value), 0.0);
    CHECK_BITS(std::get<double>(actual[start + 10].value), 0.0);
}
}  // namespace

TEST_CASE(type3_initializer_exact_order_and_typed_values_preserve_unmentioned_fields)
{
    auto state = seeded();
    std::vector<Event> events{{Kind::ListClear, 0xffff, std::monostate{}}};
    CHECK(initializeType3ShapeOptimPreparedState(state, events) == Result::Complete);
    checkScalars(state); checkEvents(events, 1, 26);
    CHECK_EQ(events[0].offset, 0xffff);
    CHECK(state.listA0.empty()); CHECK(state.listA8.empty()); CHECK(state.listF0.empty());
    // Empty destinations still produce three ordered completed-clear events.
    events.clear();
    CHECK(initializeType3ShapeOptimPreparedState(state, events) == Result::Complete);
    checkScalars(state); checkEvents(events, 0, 26);
}

TEST_CASE(type3_initializer_refusal_retains_exact_completed_prefix)
{
    const List boundaries[] = {List::A0, List::A8, List::F0};
    const Result results[] = {Result::RefusedBeforeA0, Result::RefusedBeforeA8,
                              Result::RefusedBeforeF0};
    for (std::size_t index = 0; index < 3; ++index) {
        auto state = seeded(); const auto original = state;
        std::vector<Event> events;
        CHECK(initializeType3ShapeOptimPreparedState(state, events, boundaries[index])
              == results[index]);
        checkScalars(state); checkEvents(events, 0, 23 + index);
        if (index == 0) CHECK(state.listA0 == original.listA0);
        else CHECK(state.listA0.empty());
        if (index < 2) {
            CHECK(state.listA8 == original.listA8);
            CHECK_BITS(state.listA8[0], -0.0);
        } else CHECK(state.listA8.empty());
        CHECK(state.listF0 == original.listF0);
    }
}

int main() { return testkit::run_all(); }
