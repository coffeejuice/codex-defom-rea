// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <memory>
#include <utility>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using namespace m2m::engine;
using namespace m2m::engine::simcontrol300;
namespace {
constexpr auto qualification = "Isolated actual editor gradient binding";
template<class F> void refused(F&& f) {
    bool caught = false;
    try { f(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
WidgetGradientSharedNullObservation symbolProfile() {
    WidgetGradientSharedNullObservation o;
    o.moduleSha256 = "bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160";
    o.symbolRva = 0x2f0bb8; o.count = 0; o.capacity = 0; o.flagsLow2 = 1;
    return o;
}
BrushGradientStopDataObservation countOnly(std::int32_t n) {
    BrushGradientStopDataObservation o; o.count = n; return o;
}
struct Fixture {
    Project project;
    Operation operation;
    RemeshState remesh;
    std::shared_ptr<WidgetRuntimeState> runtime;
    std::unique_ptr<StagedEditor300> factory;
    explicit Fixture(std::shared_ptr<WidgetRuntimeState> shared = {}, bool qualify = true,
        WidgetPaletteAlias palette = 17)
        : runtime(shared ? std::move(shared) : std::make_shared<WidgetRuntimeState>(true, "Isolated actual runtime"))
    {
        operation.project = &project;
        operation.engineModel = std::make_shared<GuiModel>(nullptr, 3);
        factory = std::make_unique<StagedEditor300>(binding());
        REQUIRE(factory->bindWidgetRuntime(binding(), runtime));
        if (!qualify) return;
        WidgetPrivateConstructionObservation o;
        o.qualification = qualification;
        o.fontData = {{11, 168, 0}}; o.paletteData = {{17}, {18}};
        o.defaultFont = WidgetFontValueObservation{11, 0};
        o.defaultPalette = WidgetPaletteValueObservation{palette, 0};
        o.defaultLocale = WidgetLocaleValue{0x187, 0};
        o.applicationType = 1; o.applicationLayoutDirection = 0; o.applicationModalState = false;
        o.brushData = {{23, 15, 255}, {24, 17, 0}, {25, 1, 255}};
        o.paletteBrushes = {{17, 0, 10, 23}, {17, 1, 10, 24}, {17, 0, 9, 25}, {18, 0, 10, 23}};
        runtime->observePrivateConstructionInputs(factory->widgetState(), o);
        WidgetApplicationEventObservation app; app.present = true;
        runtime->observeApplicationEvents(app, qualification);
        const auto pending = resume();
        CHECK(pending.stage == EditorBaseStage::GradientStops);
        CHECK(pending.issue == (brush()->gradientStopsSource()
            ? EditorBaseIssue::GradientStopsPending : EditorBaseIssue::GradientSourceUnavailable));
    }
    Editor300Binding binding() { return {project, operation, *operation.engineModel, remesh}; }
    EditorBaseReport resume() {
        return factory->continueNativeBase(binding(), {factory.get(), qualification, runtime->threadState()});
    }
    WidgetGradientObservation observation() const {
        WidgetGradientObservation o; o.receiver = factory->widgetState(); o.qualification = qualification;
        return o;
    }
    void observe(const WidgetGradientObservation& o) { runtime->observeGradientInputs(factory->widgetState(), o); }
    std::shared_ptr<const WidgetBrushDataState> brush(std::uint32_t group = 0, std::uint32_t role = 10) const {
        return factory->widgetState()->palette()->data->brushAt(group, role);
    }
};
}

TEST_CASE(widget_gradient_requires_this_actual_private_input_widget_and_qualification)
{
    Fixture f;
    auto o = f.observation(); o.qualification.clear();
    refused([&] { f.observe(o); });
    o = f.observation(); o.qualification = "Other stage";
    refused([&] { f.observe(o); });
    o = f.observation(); o.receiver.reset();
    refused([&] { f.observe(o); });
    Fixture unqualified({}, false);
    o = unqualified.observation();
    refused([&] { unqualified.observe(o); });
    CHECK(!f.runtime->gradientSharedNull(*f.factory->widgetState()));
    CHECK(!f.brush()->gradientStopsSource());
}

TEST_CASE(widget_gradient_rejects_foreign_runtime_receiver_and_widget_control_blocks)
{
    Fixture f, foreign;
    auto o = f.observation(); o.receiver = foreign.factory->widgetState();
    refused([&] { f.observe(o); });
    o = foreign.observation();
    refused([&] { f.runtime->observeGradientInputs(foreign.factory->widgetState(), o); });
    auto real = f.factory->widgetState();
    std::shared_ptr<WidgetState> alias(real.get(), [](WidgetState*) {});
    o = f.observation(); o.receiver = alias;
    refused([&] { f.runtime->observeGradientInputs(alias, o); });
    CHECK(!f.brush()->gradientStopsSource());
}

TEST_CASE(widget_gradient_interns_unknown_then_known_count_without_replacing_data_owner)
{
    Fixture f; auto o = f.observation(); o.stopData = {{31, {}}}; f.observe(o);
    const auto data = f.runtime->gradientStopDataFor(*f.factory->widgetState(), 31);
    REQUIRE(data); CHECK(!data->count()); CHECK(!data->externalReferences());
    CHECK(!f.brush()->gradientStopsSource());
    o = f.observation(); o.stopData = {{31, countOnly(0)}};
    o.brushSources = {{23, WidgetGradientSourceKind::DataAlias, 31}}; f.observe(o);
    REQUIRE(f.brush()->gradientStopsSource());
    CHECK(f.brush()->gradientStopsSource()->sourceData() == data);
    CHECK(f.runtime->gradientStopDataFor(*f.factory->widgetState(), 31) == data);
    CHECK(data->count() == 0); CHECK(!data->capacity()); CHECK(!data->flagsLow2());
    CHECK(!data->payload()); CHECK(!data->externalReferences());
    CHECK(data->modeledValueReferences() == 0u);
    const auto source = f.brush()->gradientStopsSource(); f.observe(o);
    CHECK(f.brush()->gradientStopsSource() == source);
}

TEST_CASE(widget_gradient_distinct_brushes_share_one_source_stop_owner)
{
    Fixture f; auto o = f.observation(); o.stopData = {{31, countOnly(0)}};
    o.brushSources = {{23, WidgetGradientSourceKind::DataAlias, 31},
                     {24, WidgetGradientSourceKind::DataAlias, 31}}; f.observe(o);
    const auto a = f.brush()->gradientStopsSource(); const auto b = f.brush(1)->gradientStopsSource();
    REQUIRE(a); REQUIRE(b); CHECK(a != b); CHECK(a->sourceData() == b->sourceData());
    CHECK(a->sourceData()->modeledValueReferences() == 0u);
    Fixture sameRuntime{f.runtime, true, 18};
    CHECK(sameRuntime.brush() == f.brush());
    CHECK(sameRuntime.brush()->gradientStopsSource() == a);
    const auto* base = sameRuntime.factory->nativeBaseConstruction();
    CHECK(base->report().stage == EditorBaseStage::GradientStops);
    CHECK(base->report().issue == EditorBaseIssue::GradientStopsPending);
    REQUIRE(base->gradientStopsCall());
    CHECK(base->gradientStopsCall()->report().issue == BrushGradientStopsIssue::SharedNullUnavailable);
    CHECK(!sameRuntime.runtime->gradientStopDataFor(*sameRuntime.factory->widgetState(), 31));
}

TEST_CASE(widget_gradient_symbol_link_unifies_source_with_one_actual_runtime_symbol)
{
    Fixture f; auto o = f.observation(); o.sharedNull = symbolProfile();
    o.brushSources = {{23, WidgetGradientSourceKind::SharedNullSymbol, 0}}; f.observe(o);
    const auto symbol = f.runtime->gradientSharedNull(*f.factory->widgetState()); REQUIRE(symbol);
    REQUIRE(f.brush()->gradientStopsSource());
    CHECK(f.brush()->gradientStopsSource()->sourceData() == symbol);
    CHECK(symbol->kind() == BrushGradientStopOwnerKind::SharedNullSymbol);
    CHECK(!symbol->observationAlias()); CHECK(!symbol->externalReferences());
    CHECK(symbol->modeledValueReferences() == 0u);
    f.observe(o); CHECK(f.runtime->gradientSharedNull(*f.factory->widgetState()) == symbol);
}

TEST_CASE(widget_gradient_shared_null_requires_explicit_active_receiver_qualification)
{
    Fixture first; auto o = first.observation(); o.sharedNull = symbolProfile(); first.observe(o);
    const auto symbol = first.runtime->gradientSharedNull(*first.factory->widgetState()); REQUIRE(symbol);
    Fixture second{first.runtime, true, 18};
    CHECK(!second.runtime->gradientSharedNull(*second.factory->widgetState()));
    second.observe(second.observation());
    CHECK(!second.runtime->gradientSharedNull(*second.factory->widgetState()));
    o = second.observation(); o.brushSources = {{23, WidgetGradientSourceKind::SharedNullSymbol, 0}};
    refused([&] { second.observe(o); });
    o.sharedNull = symbolProfile(); second.observe(o);
    CHECK(second.runtime->gradientSharedNull(*second.factory->widgetState()) == symbol);
}

TEST_CASE(widget_gradient_incomplete_or_wrong_symbol_profile_commits_nothing)
{
    Fixture f;
    for (int changed = 0; changed < 5; ++changed) {
        auto o = f.observation(); o.sharedNull = symbolProfile();
        if (changed == 0) o.sharedNull->moduleSha256 = "Other module";
        if (changed == 1) o.sharedNull->symbolRva = 0;
        if (changed == 2) o.sharedNull->count.reset();
        if (changed == 3) o.sharedNull->capacity = 1;
        if (changed == 4) o.sharedNull->flagsLow2 = 3;
        o.stopData = {{31, countOnly(0)}};
        o.brushSources = {{23, WidgetGradientSourceKind::DataAlias, 31}};
        refused([&] { f.observe(o); });
        CHECK(!f.runtime->gradientSharedNull(*f.factory->widgetState()));
        CHECK(!f.runtime->gradientStopDataFor(*f.factory->widgetState(), 31));
        CHECK(!f.brush()->gradientStopsSource());
    }
}

TEST_CASE(widget_gradient_late_bad_link_cannot_refine_or_intern_earlier_batch_data)
{
    Fixture f; auto o = f.observation(); o.stopData = {{31, {}}}; f.observe(o);
    const auto old = f.runtime->gradientStopDataFor(*f.factory->widgetState(), 31); REQUIRE(old);
    o = f.observation(); o.stopData = {{31, countOnly(0)}, {32, countOnly(0)}};
    o.sharedNull = symbolProfile(); o.brushSources = {{23, WidgetGradientSourceKind::DataAlias, 99}};
    refused([&] { f.observe(o); });
    CHECK(!old->count()); CHECK(!f.runtime->gradientStopDataFor(*f.factory->widgetState(), 32));
    CHECK(!f.runtime->gradientSharedNull(*f.factory->widgetState()));
    CHECK(!f.brush()->gradientStopsSource());
    o = f.observation(); o.stopData = {{31, countOnly(1)}, {32, countOnly(1)}};
    o.brushSources = {{23, WidgetGradientSourceKind::DataAlias, 31}}; f.observe(o);
    CHECK(old->count() == 1); CHECK(f.brush()->gradientStopsSource()->sourceData() == old);
}

TEST_CASE(widget_gradient_conflicting_source_link_and_duplicate_aliases_preserve_old_binding)
{
    Fixture f; auto o = f.observation(); o.stopData = {{31, countOnly(0)}};
    o.brushSources = {{23, WidgetGradientSourceKind::DataAlias, 31}}; f.observe(o);
    const auto source = f.brush()->gradientStopsSource(); const auto data = source->sourceData();
    o = f.observation(); o.stopData = {{32, countOnly(0)}};
    o.brushSources = {{23, WidgetGradientSourceKind::DataAlias, 32}};
    refused([&] { f.observe(o); });
    CHECK(!f.runtime->gradientStopDataFor(*f.factory->widgetState(), 32));
    CHECK(f.brush()->gradientStopsSource() == source); CHECK(source->sourceData() == data);
    o = f.observation(); o.stopData = {{33, countOnly(0)}, {33, countOnly(0)}};
    refused([&] { f.observe(o); });
    CHECK(!f.runtime->gradientStopDataFor(*f.factory->widgetState(), 33));
    o = f.observation(); o.brushSources = {{23, WidgetGradientSourceKind::DataAlias, 31},
                                         {23, WidgetGradientSourceKind::DataAlias, 31}};
    refused([&] { f.observe(o); }); CHECK(f.brush()->gradientStopsSource() == source);
}

TEST_CASE(widget_gradient_rejects_non_gradient_or_absent_brushes_and_malformed_data)
{
    Fixture f;
    for (auto brush : {25u, 99u}) {
        auto o = f.observation(); o.stopData = {{31, countOnly(0)}};
        o.brushSources = {{brush, WidgetGradientSourceKind::DataAlias, 31}};
        refused([&] { f.observe(o); });
        CHECK(!f.runtime->gradientStopDataFor(*f.factory->widgetState(), 31));
    }
    auto o = f.observation(); BrushGradientStopDataObservation mismatch;
    mismatch.count = 1; mismatch.payload.emplace(); o.stopData = {{31, mismatch}};
    refused([&] { f.observe(o); });
    CHECK(!f.runtime->gradientStopDataFor(*f.factory->widgetState(), 31));
    o = f.observation(); o.stopData = {{0, countOnly(0)}};
    refused([&] { f.observe(o); });
}

TEST_CASE(widget_gradient_observation_never_advances_editor_dispatch_or_constructor_effects)
{
    Fixture f; const auto effects = f.factory->nativeBaseConstruction()->effects();
    const auto qobject = f.runtime->retainedQObjectConstructionDelta();
    auto o = f.observation(); o.stopData = {{31, countOnly(0)}}; o.sharedNull = symbolProfile();
    o.brushSources = {{23, WidgetGradientSourceKind::DataAlias, 31}}; f.observe(o);
    const auto result = f.factory->nativeBaseConstruction()->report();
    CHECK(result.issue == EditorBaseIssue::GradientSourceUnavailable);
    CHECK(result.stage == EditorBaseStage::GradientStops);
    CHECK(!f.factory->nativeBaseConstruction()->gradientStopsCall());
    CHECK(f.factory->nativeBaseConstruction()->effects() == effects);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == qobject);
    CHECK(f.factory->widgetState()->opaque() == false);
    CHECK(!f.factory->nativeBaseConstruction()->constructorTail());
    CHECK(!result.completeForPublicSave); CHECK(!result.applicationWrappersReturned);
    EditorSetupState setup; CHECK(!f.factory->bindSetupOwner(setup));
}

TEST_CASE(widget_gradient_dead_widget_cannot_observe_or_expose_runtime_owners)
{
    Fixture f; auto o = f.observation(); o.stopData = {{31, countOnly(0)}};
    o.sharedNull = symbolProfile(); f.observe(o);
    const auto widget = f.factory->widgetState(); f.factory.reset();
    refused([&] { f.runtime->observeGradientInputs(widget, o); });
    CHECK(!f.runtime->gradientSharedNull(*widget)); CHECK(!f.runtime->gradientStopDataFor(*widget, 31));
}

int main() { return testkit::run_all(); }
