// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>

#include <defmodel/layout/db_step_data.hpp>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_statevar.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using qt::L;

namespace {
xml::NodePtr parse(const std::string& text)
{
    xml::Document doc;
    qt::String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(doc, text, &error, &line, &column));
    return doc.root;
}

bool refuses(const auto& action, const char* contains)
{
    try { action(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find(contains) != std::string::npos;
    }
    return false;
}

doe::StateVarConstructorContext profile()
{
    doe::StateVarConstructorContext context;
    context.profileIdentity = "declared-unit-profile; not a captured original profile";
    context.globals.emplace();
    context.globals->colorbarNo = 7;
    context.globals->title = L("Explicit title");
    context.globals->deflectionReferenceRgba = {0.1f, 0.2f, 0.3f, 0.4f};
    context.globals->principalMode = 2;
    context.globals->paletteMode = 3;
    context.globals->paletteFlag0 = true;
    context.globals->iso = {1, 2, 3, 4, 5, 6, 7};
    for (int index = 0; index < 25; ++index) {
        context.globals->palette.codes[index] = 100 + index;
        context.globals->palette.alternateCodes[index] = 200 + index;
        context.globals->palette.rgba[index] = {float(index), 0.5f, 0.75f, 1.0f};
    }
    for (int index = 0; index < 12; ++index) context.globals->principalRgba[index] = float(index);
    context.deflectionScaleType = {doe::SettingObservation::Value, 2};
    context.deflectionScale = {doe::SettingObservation::Value, 17};
    context.deflectionLineThickness = {doe::SettingObservation::Value, -3};
    context.showDeflectionReference = {doe::SettingObservation::Value, true};
    context.transparentDeflectionReference = {doe::SettingObservation::Value, false};
    return context;
}

std::shared_ptr<doe::StateVarState> state(engine::GuiModel& model)
{
    REQUIRE(model.doeStepData != nullptr);
    REQUIRE(model.doeStepData->stateVar != nullptr);
    return model.doeStepData->stateVar;
}

doe::StateVarColorbarSnapshot colorbar(int id, int firstCount, int secondCount)
{
    doe::StateVarColorbarSnapshot source;
    source.field00 = id;
    source.title = L("Declared colorbar");
    source.field10Bits = 0x8000000000000000ULL;
    source.field18Bits = 0x7ff8123456789abcULL;
    source.flag20 = true;
    for (int index = 0; index < firstCount; ++index)
        source.first.push_back({{0x7fc12345u, 0x80000000u, std::uint32_t(id + index)},
            0x7ff8000000000042ULL, -100 - index});
    for (int index = 0; index < secondCount; ++index)
        source.second.push_back({{0x3f800000u, std::uint32_t(id + index), 0x7f800000u},
            0x8000000000000000ULL, 100 + index});
    return source;
}
doe::StateVarConstructorContext zeroProfile()
{
    auto context = profile();
    context.translatedNoneTitle = L("Explicit translated None");
    return context;
}

doe::StateVarConstructorContext sceneProfile(Project& project, engine::GuiModel& model,
                                               double width = 121, double height = 100)
{
    auto context = zeroProfile();
    engine::displayShow(project, &model);
    auto& display = engine::projectDisplay(project);
    context.displayContext.emplace();
    auto& input = *context.displayContext;
    input.boundDisplay = &display;
    input.mainWindowAvailable = true;
    input.backend = doe::StateVarDisplayBackend::Scene;
    input.legendSlot = doe::StateVarPresence::Absent;
    input.propertyTable = doe::StateVarPresence::Absent;
    input.sceneSize = std::array<double, 2>{width, height};
    input.legendGlobals = doe::StateVarLegendGlobals{0x81, -3, 12, 27, 0x82};
    return context;
}
} // namespace

TEST_CASE(doe_statevar_unavailable_inputs_guard_before_constructor_or_runtime_effects)
{
    for (int scenario = 0; scenario < 5; ++scenario) {
        engine::GuiModel model(nullptr, 2);
        model.step->u1b4c = 7;
        auto io = doe::makeRecordingStudyIo();
        auto context = profile();
        if (scenario == 1) context.profileIdentity.clear();
        if (scenario == 2) context.globals.reset();
        if (scenario == 3) context.deflectionScale.observation = doe::SettingObservation::Unavailable;
        CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"),
            scenario == 0 ? nullptr : &context, scenario == 4 ? nullptr : io.get()); }, "StateVar"));
        CHECK(!model.doeStepData && model.step->u1b4c == 7);
        CHECK(io->observedStateVarRuntime() == nullptr);
        CHECK(io->reads().empty() && io->textWrites().empty());
    }
}

TEST_CASE(doe_statevar_observed_absent_settings_override_provisional_defaults)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile();
    context.deflectionScaleType = {doe::SettingObservation::Absent, 99};
    context.deflectionScale = {doe::SettingObservation::Absent, 99};
    context.deflectionLineThickness = {doe::SettingObservation::Absent, 99};
    context.showDeflectionReference = {doe::SettingObservation::Absent, true};
    context.transparentDeflectionReference = {doe::SettingObservation::Absent, true};
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"),
        &context, io.get()); }, "d15b40 postparse"));
    const auto current = state(model);
    CHECK(current->deflectionScaleType == 0 && current->deflectionScale == 0);
    CHECK(current->deflectionLineThickness == 0 && !current->showDeflectionReference);
    CHECK(!current->transparentDeflectionReference);
    CHECK(current->colorbarNo == 7 && current->plotType == 2 && current->type == 2);
    CHECK(current->plotVar == 0 && current->rangeType == 0 && current->rangeMax == 0);
    CHECK(current->postparsePending && !current->auxiliaryColorbarPending);
    CHECK(!model.doeStepData->propertyTableCreated() && !current->nestedQtPlotBlocksRecovered);
}

TEST_CASE(doe_statevar_explicit_constructor_palette_copy_precedes_deferred_histogram_registration)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    const auto context = profile();
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"),
        &context, io.get()); }, "d15b40 postparse"));
    const auto current = state(model);
    CHECK(current->constructorProfile == context.profileIdentity);
    CHECK(current->constructorGlobals.title == L("Explicit title"));
    CHECK(current->constructorGlobals.iso == context.globals->iso);
    CHECK(current->deflectionScaleType == 2 && current->deflectionScale == 17);
    CHECK(current->deflectionLineThickness == -3 && current->showDeflectionReference);
    REQUIRE(io->observedStateVarRuntime() != nullptr);
    const auto& runtime = *io->observedStateVarRuntime();
    REQUIRE(runtime.palette.has_value());
    CHECK(*runtime.palette == context.globals->palette);
    CHECK(runtime.events == std::vector<std::string>({
        "setting:displayOption/dispDeflectionScaleType", "setting:displayOption/dispDeflectionScale",
        "setting:displayOption/dispDeflectionLineThkType", "setting:displayOption/dispDeflectionRefShow",
        "setting:displayOption/dispDeflectionRefTransparent", "globalPaletteCopy",
        "register:signalChangeHistoGraph", "register:signalAssignHistoGraph",
        "register:signalHistoGraphOff", "register:signalGetHistoGraphOn", "histogramReset"}));
    REQUIRE(runtime.histogramRegistrations.size() == 4);
    for (const auto& registration : runtime.histogramRegistrations)
        CHECK(registration.receiver.lock() == current->histogram);
    CHECK(runtime.histogramRegistrations[0].slot == "slotChangeHistoGraph");
    CHECK(runtime.histogramRegistrations[3].slot == "slotGetHistoGraphOn");
    CHECK(io->qtWarnings().empty() && io->textWrites().empty() && io->reads().empty());
}

TEST_CASE(doe_statevar_scalar_encounter_order_uses_int_conversion_even_for_double_ranges)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    const auto context = profile();
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar>raw<Unknown/>"
        "<SVType>-2</SVType><SVPlotVar>1000</SVPlotVar><SVPlotVar>bad</SVPlotVar>"
        "<SVPlotCompNo>2147483647</SVPlotCompNo><SVPlotType>7</SVPlotType>"
        "<SVRangeType>1</SVRangeType><SVRangeMin>1.75</SVRangeMin><SVRangeMax>-23</SVRangeMax>"
        "<SVMin>2147483648</SVMin><SVMax>+4</SVMax></StateVar></StepData>"),
        &context, io.get()); }, "d15b40 postparse"));
    const auto current = state(model);
    CHECK(current->type == 2 && current->plotVar == 0 && current->plotCompNo == 2147483647);
    CHECK(current->plotType == 7 && current->rangeType == 1);
    CHECK(current->rangeMin == 0 && current->rangeMax == -23);
    CHECK(current->minimum == 0 && current->maximum == 4);
}

TEST_CASE(doe_statevar_changed_colorbar_assigns_before_guard_and_skips_later_children)
{
    engine::GuiModel model(nullptr, 2);
    model.step->u1b4c = 7;
    auto io = doe::makeRecordingStudyIo();
    const auto context = profile();
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar><SVType>3</SVType>"
        "<SVColorBarNo>9</SVColorBarNo><SVPlotVar>800</SVPlotVar></StateVar></StepData>"),
        &context, io.get()); }, "colorbar auxiliary"));
    const auto current = state(model);
    CHECK(current->type == 3 && current->colorbarNo == 9 && current->plotVar == 0);
    CHECK(current->auxiliaryColorbarPending && !current->postparsePending);
    REQUIRE(current->histogram != nullptr);
    CHECK(current->histogram->binWidth == 10 && !current->histogram->graphOn);
    CHECK(model.step->u1b4c == 7 && !model.doeStepData->propertyTableCreated());
    CHECK(io->observedStateVarRuntime()->palette.has_value());
}

TEST_CASE(doe_statevar_unchanged_colorbar_continues_to_later_children)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    const auto context = profile();
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar>"
        "<SVColorBarNo>7</SVColorBarNo><SVPlotVar>800</SVPlotVar></StateVar></StepData>"),
        &context, io.get()); }, "d15b40 postparse"));
    const auto current = state(model);
    CHECK(current->colorbarNo == 7 && current->plotVar == 800);
    CHECK(!current->auxiliaryColorbarPending && current->postparsePending);
}

TEST_CASE(doe_statevar_histogram_repeated_payloads_retain_absent_fields_then_reset_on_next_read)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    const auto context = profile();
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar>"
        "<SVHistoGraph><BinWidth>5</BinWidth><GraphOn>-1</GraphOn></SVHistoGraph>"
        "<SVHistoGraph><BinWidth>bad</BinWidth><Unknown/></SVHistoGraph>"
        "</StateVar></StepData>"), &context, io.get()); }, "d15b40 postparse"));
    const auto current = state(model);
    const auto histogram = current->histogram;
    REQUIRE(histogram != nullptr);
    CHECK(histogram->binWidth == 0 && histogram->graphOn && !histogram->qtPayloadRecovered);
    histogram->flag30 = true;
    // Reinvoke only the recovered prefix; this is not a claim that execution can
    // resume through the guarded postparse work after an unsupported conversion.
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"),
        nullptr, io.get()); }, "d15b40 postparse"));
    CHECK(current == state(model) && current->histogram == histogram);
    CHECK(histogram->binWidth == 10 && !histogram->graphOn && !histogram->flag30);
    CHECK(io->observedStateVarRuntime()->histogramRegistrations.size() == 4);
    CHECK(io->observedStateVarRuntime()->events.size() == 12);
}

TEST_CASE(doe_statevar_reset_preserves_owned_state_and_runtime_palette_but_close_dispatch_is_unclaimed)
{
    auto io = doe::makeRecordingStudyIo();
    std::weak_ptr<doe::HistogramState> weakHistogram;
    {
        engine::GuiModel model(nullptr, 2);
        const auto context = profile();
        CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"),
            &context, io.get()); }, "d15b40 postparse"));
        const auto current = state(model);
        weakHistogram = current->histogram;
        model.doeStepData->propertyTable = std::make_unique<doe::PropertyTableState>(model);
        model.reset();
        CHECK(state(model) == current && !model.doeStepData->propertyTableCreated());
        CHECK(!weakHistogram.expired() && io->observedStateVarRuntime()->palette.has_value());
    }
    CHECK(weakHistogram.expired());
    CHECK(io->observedStateVarRuntime()->palette.has_value());
    // Historical weak registration observations remain; this does not implement
    // original process-global QObject unregister/Close behavior.
    CHECK(io->observedStateVarRuntime()->histogramRegistrations.size() == 4);
}

TEST_CASE(doe_colorbar_clone_copies_proven_fields_and_uses_second_list_count)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile();
    context.colorbars = std::vector<doe::StateVarColorbarSnapshot>{colorbar(10, 0, 0), colorbar(-17, 3, 2)};
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar>"
        "<SVColorBarNo>1</SVColorBarNo><SVPlotVar>800</SVPlotVar></StateVar></StepData>"),
        &context, io.get()); }, "d15b40 postparse"));
    const auto current = state(model);
    REQUIRE(current->auxiliaryColorbar != nullptr);
    const auto& auxiliary = *current->auxiliaryColorbar;
    const auto& source = (*context.colorbars)[1];
    CHECK(auxiliary.field00 == -17 && auxiliary.title == source.title && auxiliary.flag20);
    CHECK(auxiliary.field10Bits == source.field10Bits && auxiliary.field18Bits == source.field18Bits);
    CHECK(auxiliary.firstAllocations == std::vector<std::size_t>({0, 2}));
    CHECK(auxiliary.secondAllocations == std::vector<std::size_t>({1, 3}));
    auto& allocations = io->stateVarRuntime().colorbarPointAllocations;
    REQUIRE(allocations.size() == 4); //surplus first[2] is ignored
    CHECK(allocations[0].value == source.first[0] && allocations[1].value == source.second[0]);
    CHECK(allocations[2].value == source.first[1] && allocations[3].value == source.second[1]);
    CHECK(!allocations[0].orphanedByRecoveredDelete && !allocations[3].orphanedByRecoveredDelete);
    allocations[0].value.floatBits[0] = 0;
    CHECK(source.first[0].floatBits[0] == 0x7fc12345u);
    CHECK(allocations[1].value == source.second[0]); //independent allocated copy
    CHECK(current->plotVar == 800 && !current->auxiliaryColorbarPending);
    CHECK(!current->auxiliaryPointerDangling && current->postparsePending);
    const auto& events = io->stateVarRuntime().events;
    REQUIRE(events.size() == 17);
    CHECK(std::vector<std::string>(events.begin() + 11, events.end()) == std::vector<std::string>({
        "colorbarLookup:1", "colorbarConstruct", "colorbarFirst:0", "colorbarSecond:0",
        "colorbarFirst:1", "colorbarSecond:1"}));
}

TEST_CASE(doe_colorbar_replacement_retains_original_orphan_payloads_in_both_io_modes)
{
    for (const bool native : {false, true}) {
        auto io = native ? doe::makeNativeStudyIo() : doe::makeRecordingStudyIo();
        {
            engine::GuiModel model(nullptr, 2);
            auto context = profile();
            context.colorbars = std::vector<doe::StateVarColorbarSnapshot>{
                colorbar(0, 1, 1), colorbar(1, 1, 1), colorbar(2, 1, 1)};
            CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar>"
                "<SVColorBarNo>1</SVColorBarNo><SVColorBarNo>2</SVColorBarNo>"
                "</StateVar></StepData>"), &context, io.get()); }, "d15b40 postparse"));
            const auto current = state(model);
            REQUIRE(current->auxiliaryColorbar != nullptr);
            CHECK(current->auxiliaryColorbar->field00 == 2);
            CHECK(current->auxiliaryColorbar->firstAllocations == std::vector<std::size_t>({2}));
            CHECK(current->auxiliaryColorbar->secondAllocations == std::vector<std::size_t>({3}));
            const auto& allocations = io->stateVarRuntime().colorbarPointAllocations;
            REQUIRE(allocations.size() == 4);
            CHECK(allocations[0].orphanedByRecoveredDelete && allocations[1].orphanedByRecoveredDelete);
            CHECK(!allocations[2].orphanedByRecoveredDelete && !allocations[3].orphanedByRecoveredDelete);
            CHECK(allocations[0].value == (*context.colorbars)[1].first[0]);
            CHECK(allocations[1].value == (*context.colorbars)[1].second[0]);
            const auto& events = io->stateVarRuntime().events;
            REQUIRE(events.size() == 20);
            CHECK(std::vector<std::string>(events.begin() + 11, events.end()) == std::vector<std::string>({
                "colorbarLookup:1", "colorbarConstruct", "colorbarFirst:0", "colorbarSecond:0",
                "colorbarDelete", "colorbarLookup:2", "colorbarConstruct", "colorbarFirst:0", "colorbarSecond:0"}));
        }
        // Arena data survives model lifetime within this conversion scope; no
        // original process-global Close/unregister equivalence is asserted.
        CHECK(io->stateVarRuntime().colorbarPointAllocations.size() == 4);
        CHECK(io->textWrites().empty() && io->reads().empty() && io->directories().empty());
    }
}

TEST_CASE(doe_colorbar_uint_out_of_range_selection_clamps_to_zero_without_changing_stored_no)
{
    for (const auto& text : {std::string("-1"), std::string("99")}) {
        engine::GuiModel model(nullptr, 2);
        auto io = doe::makeRecordingStudyIo();
        auto context = profile();
        context.colorbars = std::vector<doe::StateVarColorbarSnapshot>{colorbar(42, 0, 0), colorbar(99, 1, 1)};
        CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar><SVColorBarNo>" +
            text + "</SVColorBarNo></StateVar></StepData>"), &context, io.get()); }, "d15b40 postparse"));
        const auto current = state(model);
        REQUIRE(current->auxiliaryColorbar != nullptr);
        CHECK(current->colorbarNo == (text == "-1" ? -1 : 99));
        CHECK(current->auxiliaryColorbar->field00 == 42);
        CHECK(current->auxiliaryColorbar->firstAllocations.empty());
        CHECK(io->stateVarRuntime().colorbarPointAllocations.empty());
        CHECK(io->stateVarRuntime().events[11] == "colorbarLookup:0");
    }
}

TEST_CASE(doe_colorbar_short_first_list_guards_after_prior_pairs_and_metadata_without_rollback)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile();
    context.colorbars = std::vector<doe::StateVarColorbarSnapshot>{colorbar(0, 0, 0), colorbar(51, 1, 2)};
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar>"
        "<SVColorBarNo>1</SVColorBarNo><SVPlotVar>800</SVPlotVar></StateVar></StepData>"),
        &context, io.get()); }, "short first list"));
    const auto current = state(model);
    REQUIRE(current->auxiliaryColorbar != nullptr);
    CHECK(current->auxiliaryColorbar->field00 == 51 && current->auxiliaryColorbar->flag20);
    CHECK(current->auxiliaryColorbar->firstAllocations == std::vector<std::size_t>({0}));
    CHECK(current->auxiliaryColorbar->secondAllocations == std::vector<std::size_t>({1}));
    const auto& allocations = io->stateVarRuntime().colorbarPointAllocations;
    REQUIRE(allocations.size() == 2);
    CHECK(allocations[0].value == (*context.colorbars)[1].first[0]);
    CHECK(allocations[1].value == (*context.colorbars)[1].second[0]);
    CHECK(!allocations[0].orphanedByRecoveredDelete && !allocations[1].orphanedByRecoveredDelete);
    CHECK(current->colorbarNo == 1 && current->plotVar == 0 && current->auxiliaryColorbarPending);
    CHECK(!current->auxiliaryPointerDangling && !current->postparsePending);
    CHECK(!model.doeStepData->propertyTableCreated());
}

TEST_CASE(doe_colorbar_unavailable_and_observed_empty_snapshots_guard_before_auxiliary_allocation)
{
    for (const bool observedEmpty : {false, true}) {
        engine::GuiModel model(nullptr, 2);
        auto io = doe::makeRecordingStudyIo();
        auto context = profile();
        if (observedEmpty) context.colorbars.emplace();
        CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar>"
            "<SVColorBarNo>1</SVColorBarNo></StateVar></StepData>"), &context, io.get()); },
            observedEmpty ? "empty list" : "explicit list snapshot"));
        const auto current = state(model);
        CHECK(current->colorbarNo == 1 && current->auxiliaryColorbarPending);
        CHECK(!current->auxiliaryColorbar && !current->auxiliaryPointerDangling);
        CHECK(io->stateVarRuntime().colorbarPointAllocations.empty());
        CHECK(io->stateVarRuntime().events.size() == 11);
    }
}

TEST_CASE(doe_colorbar_old_auxiliary_delete_precedes_unavailable_source_and_retains_stale_pointer_state)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile(); //colorbar-list input unavailable
    // Declared helper-entry state, not a fabricated original after-state fixture.
    model.doeStepData = std::make_shared<doe::StepDataState>();
    model.doeStepData->stateVar = std::make_shared<doe::StateVarState>();
    auto current = state(model);
    current->auxiliaryColorbar = std::make_unique<doe::StateVarColorbar>();
    current->auxiliaryColorbar->firstAllocations = {0};
    current->auxiliaryColorbar->secondAllocations = {1};
    const auto oldSource = colorbar(12, 1, 1);
    io->stateVarRuntime().colorbarPointAllocations = {{oldSource.first[0], false}, {oldSource.second[0], false}};
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar>"
        "<SVColorBarNo>1</SVColorBarNo><SVType>8</SVType></StateVar></StepData>"),
        &context, io.get()); }, "explicit list snapshot"));
    CHECK(!current->auxiliaryColorbar && current->auxiliaryPointerDangling);
    CHECK(current->colorbarNo == 1 && current->type == 1 && current->auxiliaryColorbarPending);
    const auto& allocations = io->stateVarRuntime().colorbarPointAllocations;
    REQUIRE(allocations.size() == 2);
    CHECK(allocations[0].orphanedByRecoveredDelete && allocations[1].orphanedByRecoveredDelete);
    CHECK(allocations[0].value == oldSource.first[0] && allocations[1].value == oldSource.second[0]);
    CHECK(io->stateVarRuntime().events.back() == "colorbarDelete");
}

TEST_CASE(doe_colorbar_unsupported_partial_prefix_cannot_silently_resume_as_complete)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile();
    context.colorbars = std::vector<doe::StateVarColorbarSnapshot>{colorbar(0, 0, 0), colorbar(1, 1, 2)};
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar>"
        "<SVColorBarNo>1</SVColorBarNo></StateVar></StepData>"), &context, io.get()); }, "short first list"));
    const auto current = state(model);
    const auto events = io->stateVarRuntime().events;
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar>"
        "<SVColorBarNo>0</SVColorBarNo></StateVar></StepData>"), &context, io.get()); }, "cannot resume"));
    CHECK(current->colorbarNo == 1 && current->auxiliaryColorbarPending);
    CHECK(io->stateVarRuntime().events == events);
    CHECK(io->stateVarRuntime().colorbarPointAllocations.size() == 2);
}

TEST_CASE(doe_statevar_zero_var_finishes_before_later_stepdata_child_and_current_refresh)
{
    for (const bool laterFailure : {false, true}) {
        engine::GuiModel model(nullptr, 2);
        model.step->u1b4c = 7;
        auto context = zeroProfile();
        auto io = doe::makeRecordingStudyIo();
        const auto node = parse(std::string("<StepData><StateVar><SVType>99</SVType><SVPlotVar>0</SVPlotVar>") +
            "<SVPlotCompNo>900</SVPlotCompNo><SVPlotType>7</SVPlotType><SVMin>-4</SVMin><SVMax>23</SVMax>"
            "</StateVar><Object No='" + (laterFailure ? "-1" : "99") + "'/></StepData>");
        if (laterFailure)
            CHECK(refuses([&] { doe::readStepData(model, *node, &context, io.get()); }, "negative Object"));
        else doe::readStepData(model, *node, &context, io.get());
        const auto current = state(model);
        CHECK(current->type == 2 && current->plotCompNo == 900 && current->plotType == 7);
        CHECK(current->plotTitle == *context.translatedNoneTitle);
        CHECK(current->minimum == 0 && current->maximum == 1);
        CHECK(!current->postparsePending && !current->plotApplyInProgress);
        CHECK(model.doeStepData->propertyTableCreated() == !laterFailure);
        CHECK(model.step->u1b4c == (laterFailure ? 7 : 0));
        CHECK(io->stateVarRuntime().legends.empty());
        CHECK(!model.viewer && io->reads().empty() && io->textWrites().empty());
    }
}

TEST_CASE(doe_statevar_missing_translation_keeps_exact_type_prefix_before_range_assignment)
{
    engine::GuiModel model(nullptr, 2);
    auto context = profile();
    auto io = doe::makeRecordingStudyIo();
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar>"
        "<SVType>99</SVType><SVMin>-4</SVMin><SVMax>23</SVMax>"
        "</StateVar></StepData>"), &context, io.get()); }, "translated None title"));
    const auto current = state(model);
    CHECK(current->type == 2 && current->plotTitle.isNull());
    CHECK(current->minimum == -4 && current->maximum == 23);
    CHECK(current->postparsePending && current->plotApplyInProgress);
    CHECK(!model.doeStepData->propertyTableCreated() && !model.viewer);
}

TEST_CASE(doe_statevar_nonzero_guard_clears_different_cached_variable_before_type_resolution)
{
    engine::GuiModel model(nullptr, 2);
    auto context = zeroProfile();
    auto io = doe::makeRecordingStudyIo();
    doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"), &context, io.get());
    const auto current = state(model);
    current->cachedVariable = 9; //declared helper-entry state, not a captured fixture
    current->cachedDataPresent = true;
    CHECK(refuses([&] { doe::readStateVar(model, *parse("<StateVar><SVType>19</SVType>"
        "<SVPlotVar>1000</SVPlotVar></StateVar>"), &context, io.get()); }, "nonzero variable rendering"));
    CHECK(!current->cachedDataPresent && current->cachedVariable == -1);
    CHECK(current->type == 19 && current->plotVar == 1000);
    CHECK(current->plotApplyInProgress && current->postparsePending);
}

TEST_CASE(doe_statevar_populated_view_guard_precedes_cache_clear_and_zero_type_assignment)
{
    engine::GuiModel model(nullptr, 2);
    auto context = zeroProfile();
    auto io = doe::makeRecordingStudyIo();
    doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"), &context, io.get());
    const auto current = state(model);
    current->cachedVariable = 9;
    current->cachedDataPresent = true;
    float minimum[3]{}, maximum[3]{};
    CHECK(engine::viewerBox(model, 1, minimum, maximum));
    REQUIRE(engine::viewerItemCount(model) == 1);
    CHECK(refuses([&] { doe::readStateVar(model, *parse("<StateVar><SVType>19</SVType></StateVar>"),
        &context, io.get()); }, "view normalization"));
    CHECK(current->cachedDataPresent && current->cachedVariable == 9 && current->type == 19);
    CHECK(current->plotApplyInProgress && current->postparsePending);
}

TEST_CASE(doe_statevar_scene_geometry_preserves_width_threshold_and_negative_position)
{
    for (const bool narrow : {true, false}) {
        Project project;
        ProjectScope scope(project);
        engine::GuiModel model(nullptr, 2);
        auto context = sceneProfile(project, model, narrow ? 120.9 : 121.9, 101.9);
        auto io = doe::makeRecordingStudyIo();
        doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"), &context, io.get());
        const auto& runtime = io->stateVarRuntime();
        REQUIRE(runtime.legends.size() == 1);
        const auto& legend = runtime.legends[0];
        CHECK(legend.display == model.display && legend.width == (narrow ? 24 : 120));
        CHECK(legend.height == 50 && legend.x == (narrow ? 91 : -4) && legend.y == 25);
        CHECK(legend.aspect == 50.0 / legend.width);
        CHECK((legend.constraintFields == std::array<double, 4>({82, 200, 40, 512})));
        CHECK(legend.globals == *context.displayContext->legendGlobals);
        CHECK(legend.globals.fieldD8 == 0x81 && legend.globals.fieldE8 == 0x82);
        CHECK(legend.smallFontFamily == L("Arial") && legend.largeFontFamily == L("Arial"));
        CHECK(legend.smallFontPoints == 8 && legend.largeFontPoints == 10 && legend.fontWeight == 50);
        CHECK(!legend.italic && legend.smallColor == 2 && legend.largeColor == 2);
        CHECK(legend.graphicsFlags == 7 && legend.acceptHoverEvents);
        CHECK(legend.insertedIntoScene && !legend.visible && legend.cacheMode == 1);
        CHECK(legend.cacheInvalidations == 1 && !legend.qtPayloadRecovered);
        REQUIRE(runtime.events.size() == 16);
        CHECK(std::vector<std::string>(runtime.events.begin() + 11, runtime.events.end()) ==
            std::vector<std::string>({"legendConstruct", "legendInitialHide", "legendAddItem",
                "legendCacheUpdate", "legendHide"}));
        CHECK(!state(model)->plotApplyInProgress && model.doeStepData->propertyTableCreated());
        CHECK(!model.viewer && io->reads().empty() && io->textWrites().empty());
    }
}

TEST_CASE(doe_statevar_unavailable_display_inputs_guard_after_range_before_allocation)
{
    for (int scenario = 0; scenario < 7; ++scenario) {
        Project project;
        ProjectScope scope(project);
        engine::GuiModel model(nullptr, 2);
        auto context = sceneProfile(project, model);
        auto& input = *context.displayContext;
        if (scenario == 0) input.boundDisplay = nullptr;
        if (scenario == 1) input.mainWindowAvailable.reset();
        if (scenario == 2) input.backend = doe::StateVarDisplayBackend::Alternative;
        if (scenario == 3) input.legendSlot = doe::StateVarPresence::Present;
        if (scenario == 4) input.sceneSize.reset();
        if (scenario == 5) input.legendGlobals.reset();
        if (scenario == 6) input.sceneSize = std::array<double, 2>{4, 100};
        auto io = doe::makeRecordingStudyIo();
        CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"),
            &context, io.get()); }, "d15b40 postparse"));
        const auto current = state(model);
        CHECK(current->type == 2 && current->minimum == 0 && current->maximum == 1);
        CHECK(current->plotApplyInProgress && current->postparsePending);
        CHECK(io->stateVarRuntime().legends.empty() && io->stateVarRuntime().events.size() == 11);
        CHECK(!model.doeStepData->propertyTableCreated());
    }
}

TEST_CASE(doe_statevar_property_table_refusal_retains_legend_construction_and_cache_prefix)
{
    for (const auto table : {doe::StateVarPresence::Unavailable, doe::StateVarPresence::Present}) {
        Project project;
        ProjectScope scope(project);
        engine::GuiModel model(nullptr, 2);
        auto context = sceneProfile(project, model);
        context.displayContext->propertyTable = table;
        auto io = doe::makeRecordingStudyIo();
        CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"),
            &context, io.get()); }, "active model requires observed absent property table"));
        const auto& runtime = io->stateVarRuntime();
        REQUIRE(runtime.legends.size() == 1);
        CHECK(runtime.legends[0].insertedIntoScene && runtime.legends[0].cacheInvalidations == 1);
        CHECK(!runtime.legends[0].visible && state(model)->plotApplyInProgress);
        REQUIRE(runtime.events.size() == 15);
        CHECK(runtime.events.back() == "legendCacheUpdate");
        CHECK(!model.doeStepData->propertyTableCreated());
    }
}

TEST_CASE(doe_statevar_named_model_guard_occurs_after_legend_cache_and_before_visibility_scan)
{
    Project project;
    ProjectScope scope(project);
    engine::GuiModel model(nullptr, 2);
    auto context = sceneProfile(project, model);
    engine::displayModels(*model.display).named.emplace_back(); //explicit unknown named holder
    auto io = doe::makeRecordingStudyIo();
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"),
        &context, io.get()); }, "named-model plot scan"));
    REQUIRE(io->stateVarRuntime().legends.size() == 1);
    CHECK(io->stateVarRuntime().events.back() == "legendCacheUpdate");
    CHECK(state(model)->plotApplyInProgress && !model.doeStepData->propertyTableCreated());
}

TEST_CASE(doe_statevar_reuses_scene_legend_without_reapplying_constructor_globals_in_both_io_modes)
{
    for (const bool native : {false, true}) {
        Project project;
        ProjectScope scope(project);
        engine::GuiModel model(nullptr, 2);
        auto context = sceneProfile(project, model);
        auto io = native ? doe::makeNativeStudyIo() : doe::makeRecordingStudyIo();
        doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"), &context, io.get());
        const auto globals = io->stateVarRuntime().legends[0].globals;
        context.displayContext->legendSlot = doe::StateVarPresence::Present;
        context.displayContext->legendGlobals.reset();
        context.displayContext->sceneSize.reset();
        doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"), &context, io.get());
        const auto& legends = io->stateVarRuntime().legends;
        REQUIRE(legends.size() == 1);
        CHECK(legends[0].globals == globals && legends[0].width == 120);
        CHECK(legends[0].cacheInvalidations == 2 && !legends[0].visible);
        CHECK(io->reads().empty() && io->textWrites().empty() && io->directories().empty());
    }
}

TEST_CASE(doe_statevar_observed_null_main_window_skips_backend_and_legend_inputs)
{
    Project project;
    ProjectScope scope(project);
    engine::GuiModel model(nullptr, 2);
    auto context = sceneProfile(project, model);
    auto& input = *context.displayContext;
    input.mainWindowAvailable = false;
    input.backend = doe::StateVarDisplayBackend::Unavailable;
    input.legendSlot = doe::StateVarPresence::Unavailable;
    input.sceneSize.reset();
    input.legendGlobals.reset();
    auto io = doe::makeRecordingStudyIo();
    doe::readStepData(model, *parse("<StepData><StateVar/></StepData>"), &context, io.get());
    CHECK(io->stateVarRuntime().legends.empty() && io->stateVarRuntime().events.size() == 11);
    CHECK(model.doeStepData->propertyTableCreated() && !state(model)->plotApplyInProgress);
}

int main() { return testkit::run_all(); }
