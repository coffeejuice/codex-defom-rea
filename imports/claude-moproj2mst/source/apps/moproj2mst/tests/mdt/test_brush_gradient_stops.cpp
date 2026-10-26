// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <algorithm>
#include <array>
#include <limits>
#include "m2m/engine/brush_gradient_stops.hpp"

using namespace m2m::engine;
namespace {
using Stage = BrushGradientStopsStage;
using Issue = BrushGradientStopsIssue;
using Effect = BrushGradientStopsEffect;
std::size_t count(const BrushGradientStopsCall& call, Effect effect)
{ return static_cast<std::size_t>(std::count(call.effects().begin(), call.effects().end(), effect)); }
struct Fixture {
    std::shared_ptr<BrushGradientStopsSource> source = std::make_shared<BrushGradientStopsSource>();
    std::shared_ptr<BrushGradientStopDataState> data = std::make_shared<BrushGradientStopDataState>(31);
    std::shared_ptr<BrushGradientStopDataState> sharedNull = BrushGradientStopDataState::makeSharedNullSymbol();
    BrushGradientStopsCall call{source, sharedNull};
    void bind() { REQUIRE(source->observeSourceData(data)); }
    void empty() {
        bind(); BrushGradientStopDataObservation o; o.count = 0;
        REQUIRE(data->observe(o));
    }
};
}

TEST_CASE(gradient_stops_missing_source_pointer_and_count_do_not_construct_a_local_value)
{
    BrushGradientStopsCall absent{nullptr, BrushGradientStopDataState::makeSharedNullSymbol()};
    CHECK(absent.resume().issue == Issue::SourceUnavailable);
    CHECK(absent.effects().empty());
    Fixture f;
    CHECK(f.call.resume().issue == Issue::SourceDataUnavailable);
    CHECK(f.call.effects().empty());
    f.bind();
    CHECK(f.call.resume().issue == Issue::SourceCountUnavailable);
    CHECK(f.call.effects().empty());
    BrushGradientStopDataObservation o; o.count = 0;
    REQUIRE(f.data->observe(o));
    CHECK(f.call.resume().stage == Stage::Returned);
    CHECK(count(f.call, Effect::SourceCountRead) == 1u);
}

TEST_CASE(gradient_stops_exact_zero_skips_unknown_source_metadata_and_references)
{
    Fixture f; f.empty();
    CHECK(f.call.resume().stage == Stage::Returned);
    CHECK(f.source->sourceData() == f.data);
    CHECK(f.data->count() == 0);
    CHECK(!f.data->capacity()); CHECK(!f.data->flagsLow2());
    CHECK(!f.data->payload()); CHECK(!f.data->externalReferences());
    CHECK(f.data->modeledValueReferences() == 0u);
    CHECK(f.data->nativeDataAlive());
    CHECK(!f.sharedNull->externalReferences());
    CHECK(f.sharedNull->modeledValueReferences() == 0u);
    CHECK(f.sharedNull->nativeDataAlive());
}

TEST_CASE(gradient_stops_nonzero_and_negative_counts_keep_the_source_branch_guard)
{
    for (std::int32_t n : {-1, 1, std::numeric_limits<std::int32_t>::max()}) {
        Fixture f; f.bind(); BrushGradientStopDataObservation o; o.count = n;
        REQUIRE(f.data->observe(o));
        CHECK(f.call.resume().issue == Issue::NonzeroSourceUnrecovered);
        CHECK(f.call.report().stage == Stage::NonzeroSource);
        const auto effects = f.call.effects();
        CHECK(f.call.resume().issue == Issue::NonzeroSourceUnrecovered);
        CHECK(f.call.effects() == effects);
        CHECK(count(f.call, Effect::SourceCountRead) == 1u);
        CHECK(f.call.allocatedData().empty()); CHECK(!f.call.returnedData());
        CHECK(f.data->count() == n); CHECK(f.data->modeledValueReferences() == 0u);
        CHECK(f.sharedNull->modeledValueReferences() == 0u);
    }
}

TEST_CASE(gradient_stops_shared_null_dependency_follows_the_source_count_read)
{
    Fixture f; f.empty();
    BrushGradientStopsCall absent{f.source, nullptr};
    CHECK(absent.resume().issue == Issue::SharedNullUnavailable);
    CHECK(absent.report().stage == Stage::DefaultLocal);
    CHECK(absent.resume().issue == Issue::SharedNullUnavailable);
    CHECK(count(absent, Effect::SourceCountRead) == 1u);
    CHECK(count(absent, Effect::LocalValueConstructed) == 0u);
    BrushGradientStopsCall wrong{f.source, f.data};
    CHECK(wrong.resume().issue == Issue::SharedNullOwnerMismatch);
    CHECK(wrong.allocatedData().empty()); CHECK(f.data->modeledValueReferences() == 0u);
}

TEST_CASE(gradient_stops_default_uses_capacities_one_then_three_and_preserves_ordered_fields)
{
    Fixture f; f.empty(); CHECK(f.call.resume().stage == Stage::Returned);
    REQUIRE(f.call.allocatedData().size() == 2u);
    const auto first = f.call.allocatedData()[0]; const auto result = f.call.returnedData();
    REQUIRE(result); CHECK(first != result); CHECK(result != f.data); CHECK(result != f.sharedNull);
    CHECK(first->capacity() == 1); CHECK(first->count() == 1);
    CHECK(!first->nativeDataAlive()); CHECK(first->modeledValueReferences() == 0u);
    CHECK(result->capacity() == 3); CHECK(result->count() == 2);
    CHECK(result->flagsLow2() == 1); CHECK(result->nativeDataAlive());
    CHECK(result->modeledValueReferences() == 1u);
    CHECK(!result->observationAlias()); CHECK(result->externalReferences() == 0);
    REQUIRE(result->payload()); REQUIRE(result->payload()->size() == 2u);
    const auto& zero = result->payload()->at(0); const auto& one = result->payload()->at(1);
    CHECK(zero.positionBits == 0x0000000000000000ULL);
    CHECK(one.positionBits == 0x3ff0000000000000ULL);
    const std::array<std::uint8_t, 14> black{1,0,0,0,255,255,0,0,0,0,0,0,0,0};
    const std::array<std::uint8_t, 14> white{1,0,0,0,255,255,255,255,255,255,255,255,0,0};
    CHECK(zero.color == black); CHECK(one.color == white);
    CHECK(zero.alphaHighByte() == 255u); CHECK(one.alphaHighByte() == 255u);
    REQUIRE(first->payload()); CHECK(first->payload()->at(0) == zero);
}

TEST_CASE(gradient_stops_returned_value_survives_getter_local_destruction_then_releases_once)
{
    Fixture f; f.empty();
    CHECK(!f.call.destroyReturnedValue());
    CHECK(f.call.resume().stage == Stage::Returned);
    const auto data = f.call.returnedData(); REQUIRE(data);
    const auto effects = f.call.effects();
    CHECK(f.call.resume().stage == Stage::Returned); CHECK(f.call.effects() == effects);
    CHECK(count(f.call, Effect::ReturnedValueRetained) == 1u);
    CHECK(count(f.call, Effect::LocalValueDestroyed) == 1u);
    CHECK(data->modeledValueReferences() == 1u);
    CHECK(f.call.destroyReturnedValue());
    CHECK(data->modeledValueReferences() == 0u); CHECK(!data->nativeDataAlive());
    CHECK(!f.call.returnedData()); CHECK(f.call.report().stage == Stage::Released);
    CHECK(!f.call.destroyReturnedValue());
    CHECK(f.call.resume().stage == Stage::Released);
    CHECK(count(f.call, Effect::ReturnedValueDestroyed) == 1u);
    CHECK(count(f.call, Effect::CapacityOneFreed) == 1u);
    CHECK(count(f.call, Effect::CapacityThreeFreed) == 1u);
}

TEST_CASE(gradient_stops_two_calls_share_only_shared_null_and_source_owners)
{
    Fixture f; f.empty(); BrushGradientStopsCall second{f.source, f.sharedNull};
    CHECK(f.call.resume().stage == Stage::Returned);
    CHECK(second.resume().stage == Stage::Returned);
    CHECK(f.call.returnedData() != second.returnedData());
    CHECK(f.sharedNull->modeledValueReferences() == 0u);
    CHECK(f.data->modeledValueReferences() == 0u);
    CHECK(f.call.destroyReturnedValue()); CHECK(second.destroyReturnedValue());
    CHECK(f.data->nativeDataAlive()); CHECK(f.sharedNull->nativeDataAlive());
}

TEST_CASE(gradient_stops_source_may_be_the_same_actual_shared_null_symbol)
{
    auto symbol = BrushGradientStopDataState::makeSharedNullSymbol();
    auto source = std::make_shared<BrushGradientStopsSource>();
    REQUIRE(source->observeSourceData(symbol));
    BrushGradientStopsCall call{source, symbol};
    CHECK(call.resume().stage == Stage::Returned);
    CHECK(source->sourceData() == symbol); CHECK(call.returnedData() != symbol);
    CHECK(symbol->count() == 0); CHECK(symbol->capacity() == 0);
    CHECK(symbol->flagsLow2() == 1); CHECK(!symbol->externalReferences());
    CHECK(symbol->modeledValueReferences() == 0u); CHECK(symbol->nativeDataAlive());
    CHECK(call.destroyReturnedValue()); CHECK(symbol->nativeDataAlive());
}

TEST_CASE(gradient_stops_source_metadata_is_preserved_and_conflicting_observations_are_atomic)
{
    Fixture f; f.bind(); BrushGradientStopDataObservation o;
    o.count = 0; o.capacity = 97; o.flagsLow2 = 2; o.externalReferences = 19;
    REQUIRE(f.data->observe(o));
    BrushGradientStopDataObservation conflict; conflict.count = 1; conflict.payload.emplace();
    CHECK(!f.data->observe(conflict));
    CHECK(!f.data->payload()); CHECK(f.data->count() == 0);
    CHECK(f.call.resume().stage == Stage::Returned);
    CHECK(f.data->capacity() == 97); CHECK(f.data->flagsLow2() == 2);
    CHECK(f.data->externalReferences() == 19); CHECK(f.data->modeledValueReferences() == 0u);
    CHECK(f.data->count() == 0); CHECK(!f.data->payload());
    CHECK(f.call.returnedData()->flagsLow2() == 1);
}

TEST_CASE(gradient_stops_source_binding_rejects_foreign_control_blocks_and_generated_values)
{
    Fixture f; f.empty();
    std::shared_ptr<BrushGradientStopDataState> foreign(f.data.get(), [](BrushGradientStopDataState*) {});
    CHECK(!f.source->observeSourceData(foreign)); CHECK(f.source->sourceData() == f.data);
    CHECK(!f.source->observeSourceData(f.sharedNull));
    auto invalid = std::make_shared<BrushGradientStopDataState>(0);
    CHECK(!f.source->observeSourceData(invalid));
    BrushGradientStopDataObservation o; o.count = 0; CHECK(!invalid->observe(o));
    CHECK(f.call.resume().stage == Stage::Returned);
    BrushGradientStopsSource other;
    CHECK(!other.observeSourceData(f.call.allocatedData()[1]));
}

TEST_CASE(gradient_stops_diagnostic_leases_do_not_keep_returned_native_values_alive)
{
    Fixture f; f.empty(); std::shared_ptr<const BrushGradientStopDataState> escaped;
    {
        BrushGradientStopsCall call{f.source, f.sharedNull};
        CHECK(call.resume().stage == Stage::Returned); escaped = call.returnedData();
        CHECK(escaped->nativeDataAlive()); CHECK(escaped->modeledValueReferences() == 1u);
    }
    CHECK(!escaped->nativeDataAlive()); CHECK(escaped->modeledValueReferences() == 0u);
    CHECK(f.data->nativeDataAlive()); CHECK(f.sharedNull->nativeDataAlive());
}

TEST_CASE(gradient_stops_late_shared_null_binding_preserves_consumed_count_and_native_effects)
{
    Fixture f; f.empty();
    BrushGradientStopsCall call{f.source, nullptr};
    CHECK(call.resume().issue == Issue::SharedNullUnavailable);
    CHECK(call.report().stage == Stage::DefaultLocal);
    const auto effects = call.effects();
    REQUIRE(effects.size() == 1u);
    CHECK(effects.front() == Effect::SourceCountRead);
    REQUIRE(call.bindSharedNull(f.sharedNull));
    CHECK(call.report().stage == Stage::DefaultLocal);
    CHECK(call.report().issue == Issue::SharedNullUnavailable);
    CHECK(call.effects() == effects);
    CHECK(f.sharedNull->modeledValueReferences() == 0u);
    CHECK(call.allocatedData().empty());
    REQUIRE(call.bindSharedNull(f.sharedNull));
    CHECK(call.effects() == effects);
    CHECK(call.resume().stage == Stage::Returned);
    CHECK(count(call, Effect::SourceCountRead) == 1u);
    CHECK(count(call, Effect::SharedNullRetained) == 1u);
    CHECK(count(call, Effect::SharedNullReleased) == 1u);
    CHECK(call.returnedData()->modeledValueReferences() == 1u);
    CHECK(f.sharedNull->modeledValueReferences() == 0u);
    CHECK(call.destroyReturnedValue());
    CHECK(count(call, Effect::ReturnedValueDestroyed) == 1u);
}

TEST_CASE(gradient_stops_shared_null_binding_rejects_wrong_kind_without_poisoning_pending_frame)
{
    Fixture f; f.empty();
    BrushGradientStopsCall call{f.source, nullptr};
    CHECK(call.resume().issue == Issue::SharedNullUnavailable);
    const auto effects = call.effects();
    CHECK(!call.bindSharedNull(nullptr));
    CHECK(!call.bindSharedNull(f.data));
    CHECK(call.effects() == effects);
    CHECK(call.report().stage == Stage::DefaultLocal);
    CHECK(call.resume().issue == Issue::SharedNullUnavailable);
    REQUIRE(call.bindSharedNull(f.sharedNull));
    CHECK(call.resume().stage == Stage::Returned);
    const auto returned = call.returnedData(); REQUIRE(returned);
    CHECK(!call.bindSharedNull(call.allocatedData()[1]));
    CHECK(call.returnedData() == returned);
    CHECK(returned->modeledValueReferences() == 1u);
}

TEST_CASE(gradient_stops_shared_null_binding_rejects_replacement_and_foreign_control_block)
{
    Fixture f; f.empty();
    CHECK(f.call.resume().stage == Stage::Returned);
    const auto effects = f.call.effects();
    const auto result = f.call.returnedData(); REQUIRE(result);
    CHECK(!f.call.bindSharedNull(BrushGradientStopDataState::makeSharedNullSymbol()));
    std::shared_ptr<BrushGradientStopDataState> foreign(f.sharedNull.get(),
        [](BrushGradientStopDataState*) {});
    CHECK(!f.call.bindSharedNull(foreign));
    CHECK(!f.call.bindSharedNull(nullptr));
    REQUIRE(f.call.bindSharedNull(f.sharedNull));
    CHECK(f.call.effects() == effects);
    CHECK(f.call.report().stage == Stage::Returned);
    CHECK(f.call.returnedData() == result);
    CHECK(result->modeledValueReferences() == 1u);
    CHECK(f.call.destroyReturnedValue());
    const auto releasedEffects = f.call.effects();
    REQUIRE(f.call.bindSharedNull(f.sharedNull));
    CHECK(f.call.effects() == releasedEffects);
    CHECK(f.call.report().stage == Stage::Released);
    CHECK(f.sharedNull->modeledValueReferences() == 0u);
}

TEST_CASE(gradient_stops_constructor_bound_wrong_symbol_cannot_be_replaced_by_late_binding)
{
    Fixture f; f.empty();
    BrushGradientStopsCall call{f.source, f.data};
    CHECK(call.resume().issue == Issue::SharedNullOwnerMismatch);
    const auto effects = call.effects();
    CHECK(!call.bindSharedNull(f.sharedNull));
    CHECK(call.effects() == effects);
    CHECK(call.resume().issue == Issue::SharedNullOwnerMismatch);
    CHECK(count(call, Effect::SourceCountRead) == 1u);
    CHECK(call.allocatedData().empty());
}

int main() { return testkit::run_all(); }
