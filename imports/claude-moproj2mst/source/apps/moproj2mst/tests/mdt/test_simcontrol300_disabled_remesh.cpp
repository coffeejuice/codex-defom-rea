// ctest-labels: m2m-mdt private-editor300
// Actual model/Session remesh storage plus private page Apply; public palette stays guarded.
#include <testkit/check.hpp>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <initializer_list>
#include "m2m/engine/simcontrol300.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine.hpp"
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/gui_twins_delete.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
template<class T> T& field(def::model::LLOBJ_DATA* object, std::size_t offset)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(object) + offset);
}
void seedObject(def::model::LLOBJ_DATA* object, const std::array<double, 4>& values)
{
    REQUIRE(object);
    field<int>(object, 0x87c) = 1;
    field<double>(object, 0x2dc8) = values[0];
    field<double>(object, 0x2dd0) = values[1];
    field<double>(object, 0x2de0) = values[2];
    field<double>(object, 0x2dd8) = values[3];
}
std::array<double, 4> scalars(def::model::LLOBJ_DATA* object)
{
    return {field<double>(object, 0x2dc8), field<double>(object, 0x2dd0),
        field<double>(object, 0x2de0), field<double>(object, 0x2dd8)};
}
void checkScalarBits(const std::array<double, 4>& actual, const std::array<double, 4>& expected)
{
    for (std::size_t i = 0; i < actual.size(); ++i)
        CHECK_EQ(std::bit_cast<std::uint64_t>(actual[i]), std::bit_cast<std::uint64_t>(expected[i]));
}
void seedRemesh(RemeshState& state)
{
    state.method = -9; state.goodInternalChangeable = true;
    state.filterDistance = 44.25; state.uniformInternalEdge = -0.0;
    state.improveContact = 7; state.goodSurfaceChangeable = 9;
    for (std::size_t i = 0; i < state.contactToScalingBytes.size(); ++i)
        state.contactToScalingBytes[i] = static_cast<std::byte>(i + 1);
    state.scalingFactor = std::bit_cast<double>(std::uint64_t{0x7ff8000000004321ULL});
    state.enabled = 1;
}
void checkOtherRemeshFields(const RemeshState& actual, const RemeshState& before)
{
    CHECK_EQ(actual.method, before.method);
    CHECK(actual.goodInternalChangeable == before.goodInternalChangeable);
    CHECK_EQ(std::bit_cast<std::uint64_t>(actual.filterDistance), std::bit_cast<std::uint64_t>(before.filterDistance));
    CHECK_EQ(std::bit_cast<std::uint64_t>(actual.uniformInternalEdge), std::bit_cast<std::uint64_t>(before.uniformInternalEdge));
    CHECK_EQ(actual.improveContact, before.improveContact);
    CHECK(actual.contactToScalingBytes == before.contactToScalingBytes);
    CHECK_EQ(std::bit_cast<std::uint64_t>(actual.scalingFactor), std::bit_cast<std::uint64_t>(before.scalingFactor));
    CHECK_EQ(actual.goodSurfaceChangeable, before.goodSurfaceChangeable);
}
}

TEST_CASE(disabled_remesh_ignores_method_projection_but_commits_objects_before_shared_enabled_byte)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession owner(driver);
    auto& state = owner.remeshState(); // Actual Session-owned member, supplied to private Apply.
    engine::GuiModel model(nullptr, 3);
    auto* first = model.objectWrap(1, true, false);
    auto* second = model.objectWrap(2, true, false);
    seedObject(first, {-6.12345678, 7.12345678, -8.12345678, 9.75});
    const std::array unvisited{std::bit_cast<double>(std::uint64_t{0x7ff8000000001234ULL}), 3.12345678, -0.0, 5.75};
    seedObject(second, unvisited);
    auto form = fillMeshing3D(model, state); REQUIRE(form); REQUIRE(form->objects.size() == 2);
    CHECK_EQ(form->objects[0].number, 1); CHECK_EQ(form->objects[1].number, 2);
    form->remeshEnabled = false; form->remeshScalingFactor = qt::L("not-a-number");
    form->goodInternalChangeableChecked = true;
    field<double>(first, 0x2d20) = 123.25; field<double>(second, 0x2d20) = 456.5;
    for (int unusedMethod : {0, 99, (std::numeric_limits<int>::min)()}) {
        form->remeshMethodRadio = unusedMethod;
        seedObject(first, {99.0, 99.0, 99.0, 99.0}); seedObject(second, {88.0, 88.0, 88.0, 88.0});
        seedRemesh(state); const auto before = state;
        CHECK(applyMeshing3D(model, state, *form) == ApplyResult::Completed);
        checkScalarBits(scalars(first), {-6.12346, 7.12346, 8.12346, 9.0});
        checkScalarBits(scalars(second), unvisited);
        CHECK_EQ(state.enabled, 0); checkOtherRemeshFields(state, before);
        CHECK(&state == &owner.remeshState());
        CHECK(field<double>(first, 0x2d20) == 123.25); CHECK(field<double>(second, 0x2d20) == 456.5);
        CHECK_EQ(form->remeshMethodRadio, unusedMethod); CHECK(form->remeshScalingFactor == qt::L("not-a-number"));
    }
}

TEST_CASE(disabled_remesh_missing_remembered_object_does_not_stop_later_object_or_enabled_store)
{
    engine::GuiModel model(nullptr, 3);
    auto* first = model.objectWrap(1, true, false);
    auto* missing = model.objectWrap(2, true, false);
    auto* last = model.objectWrap(3, true, false);
    seedObject(first, {1.23456789, 2.34567891, -3.45678912, 4.75});
    seedObject(missing, {5.0, 6.0, 7.0, 8.0});
    const std::array lastValues{-9.12345678, 10.12345678, -11.12345678, 12.75};
    seedObject(last, lastValues);
    RemeshState state; auto form = fillMeshing3D(model, state);
    REQUIRE(form); REQUIRE(form->objects.size() == 3);
    REQUIRE(def::model::object_list_remove(&model.step->objects, missing)); // No dangling object use below.
    CHECK(model.object(2) == nullptr);
    seedObject(first, {99.0, 99.0, 99.0, 99.0}); seedObject(last, {88.0, 88.0, 88.0, 88.0});
    form->remeshEnabled = false; form->remeshMethodRadio = 99;
    seedRemesh(state); const auto before = state;
    CHECK(applyMeshing3D(model, state, *form) == ApplyResult::Completed);
    checkScalarBits(scalars(first), {1.23457, 2.34568, 3.45679, 4.0});
    checkScalarBits(scalars(last), lastValues);
    CHECK_EQ(state.enabled, 0); checkOtherRemeshFields(state, before);
    CHECK(model.object(2) == nullptr);
}

TEST_CASE(enabled_invalid_method_remains_preflight_refusal_before_object_or_shared_state_effects)
{
    engine::GuiModel model(nullptr, 3); auto* object = model.objectWrap(1, true, false);
    seedObject(object, {1.25, 2.5, 3.75, 4.0});
    RemeshState state; auto form = fillMeshing3D(model, state); REQUIRE(form);
    form->remeshEnabled = true; form->remeshMethodRadio = 99;
    seedObject(object, {99.0, 99.0, 99.0, 99.0});
    seedRemesh(state); state.enabled = 0; const auto before = state;
    CHECK(applyMeshing3D(model, state, *form) == ApplyResult::UnsupportedSnapshot);
    checkScalarBits(scalars(object), {99.0, 99.0, 99.0, 99.0});
    CHECK_EQ(state.enabled, before.enabled); checkOtherRemeshFields(state, before);
}

TEST_CASE(disabled_unused_method_cannot_bypass_wrong_dimension_or_missing_step)
{
    engine::GuiModel source(nullptr, 3); auto* original = source.objectWrap(1, true, false);
    seedObject(original, {1.25, 2.5, 3.75, 4.0});
    RemeshState state; auto form = fillMeshing3D(source, state); REQUIRE(form);
    form->remeshEnabled = false; form->remeshMethodRadio = 99;
    engine::GuiModel wrong(nullptr, 2); auto* object = wrong.objectWrap(1, true, false);
    seedObject(object, {99.0, 99.0, 99.0, 99.0});
    seedRemesh(state); const auto before = state;
    CHECK(applyMeshing3D(wrong, state, *form) == ApplyResult::UnsupportedSnapshot);
    checkScalarBits(scalars(object), {99.0, 99.0, 99.0, 99.0});
    CHECK_EQ(state.enabled, before.enabled); checkOtherRemeshFields(state, before);
    engine::GuiModel absent(nullptr, 3); absent.step.reset();
    CHECK(applyMeshing3D(absent, state, *form) == ApplyResult::UnsupportedSnapshot);
    CHECK_EQ(state.enabled, before.enabled); checkOtherRemeshFields(state, before);
}

int main() { return testkit::run_all(); }
