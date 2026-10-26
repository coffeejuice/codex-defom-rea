// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <memory>
#include <string>
#include <utility>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using namespace m2m::engine;
using namespace m2m::engine::simcontrol300;
namespace {
using Stage = PaintDeviceCleanupStage;
using Issue = PaintDeviceCleanupIssue;
constexpr auto moduleHash = "c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887";
template<class F> void refused(F&& f)
{
    bool caught = false;
    try { f(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
struct Fixture {
    Project project;
    Operation operation;
    RemeshState remesh;
    std::string privateQualification;
    std::shared_ptr<WidgetRuntimeState> runtime;
    std::unique_ptr<StagedEditor300> factory;
    explicit Fixture(std::shared_ptr<WidgetRuntimeState> shared = {}, bool qualify = true,
        std::string receipt = "Isolated actual cleanup widget")
        : privateQualification(std::move(receipt)), runtime(shared ? std::move(shared)
            : std::make_shared<WidgetRuntimeState>(true, "Isolated actual Qt runtime module"))
    {
        operation.project = &project;
        operation.engineModel = std::make_shared<GuiModel>(nullptr, 3);
        factory = std::make_unique<StagedEditor300>(binding());
        REQUIRE(factory->bindWidgetRuntime(binding(), runtime));
        if (!qualify) return;
        WidgetPrivateConstructionObservation o; o.qualification = privateQualification;
        o.fontData = {{11, 168, 0}}; o.paletteData = {{17}};
        o.defaultFont = WidgetFontValueObservation{11, 0};
        o.defaultPalette = WidgetPaletteValueObservation{17, 0};
        o.defaultLocale = WidgetLocaleValue{0x187, 0};
        o.applicationType = 1; o.applicationLayoutDirection = 0; o.applicationModalState = false;
        o.brushData = {{23, 24, 255}}; o.paletteBrushes = {{17, 0, 10, 23}};
        runtime->observePrivateConstructionInputs(factory->widgetState(), o);
        WidgetApplicationEventObservation app; app.present = true;
        runtime->observeApplicationEvents(app, privateQualification);
        const auto report = factory->continueNativeBase(binding(),
            {factory.get(), privateQualification, runtime->threadState()});
        CHECK(report.stage == EditorBaseStage::TextureImage);
        CHECK(report.issue == EditorBaseIssue::TextureImageSourceUnavailable);
    }
    Editor300Binding binding() { return {project, operation, *operation.engineModel, remesh}; }
    WidgetPaintDeviceCleanupObservation observation() const
    {
        WidgetPaintDeviceCleanupObservation o;
        o.receiver = factory->widgetState(); o.qualification = privateQualification;
        o.moduleSha256 = moduleHash; o.pointerRva = 0x9cb858;
        return o;
    }
    void observe(const WidgetPaintDeviceCleanupObservation& o)
    { runtime->observePaintDeviceCleanupInputs(factory->widgetState(), o); }
    std::shared_ptr<PaintDeviceCleanupGlobals> globals() const
    { return runtime->paintDeviceCleanupGlobals(*factory->widgetState()); }
    void existing(std::optional<std::int32_t> value)
    {
        auto o = observation(); o.counters = {{31, value}}; o.currentCounterAlias = 31;
        observe(o);
    }
};
}

TEST_CASE(widget_paint_cleanup_has_no_owner_or_counter_defaults)
{
    Fixture f; CHECK(!f.globals());
    f.observe(f.observation());
    const auto globals = f.globals(); REQUIRE(globals);
    CHECK(globals->qualified()); CHECK(globals->qualification() == f.runtime->qualification());
    CHECK(!globals->counterFor(31));
    PaintDeviceCleanupCall call{globals};
    CHECK(call.resume().issue == Issue::CounterPointerUnavailable); CHECK(call.effects().empty());
}

TEST_CASE(widget_paint_cleanup_requires_same_live_private_input_receiver_and_receipt)
{
    Fixture f; auto o = f.observation(); o.counters = {{31, 0}}; o.currentCounterAlias = 31;
    o.qualification.clear(); refused([&] { f.observe(o); });
    o.qualification = "Other private stage"; refused([&] { f.observe(o); });
    o.qualification = f.privateQualification; o.receiver.reset(); refused([&] { f.observe(o); });
    Fixture unqualified({}, false); o = unqualified.observation();
    refused([&] { unqualified.observe(o); });
    CHECK(!f.globals()); CHECK(!unqualified.globals());
}

TEST_CASE(widget_paint_cleanup_rejects_foreign_runtime_and_rewrapped_widget_receivers)
{
    Fixture f, other; auto o = f.observation(); o.receiver = other.factory->widgetState();
    refused([&] { f.observe(o); });
    o = other.observation();
    refused([&] { f.runtime->observePaintDeviceCleanupInputs(other.factory->widgetState(), o); });
    const auto actual = f.factory->widgetState();
    std::shared_ptr<WidgetState> foreign(actual.get(), [](WidgetState*) {});
    o = f.observation(); o.receiver = foreign;
    refused([&] { f.observe(o); });
    refused([&] { f.runtime->observePaintDeviceCleanupInputs(foreign, o); });
    CHECK(!f.globals()); CHECK(!other.globals());
}

TEST_CASE(widget_paint_cleanup_wrong_module_profile_never_publishes_an_owner)
{
    Fixture f;
    for (int changed = 0; changed != 4; ++changed) {
        auto o = f.observation(); o.counters = {{31, 0}}; o.currentCounterAlias = 31;
        if (changed == 0) o.moduleSha256 = "Other module";
        if (changed == 1) o.moduleSha256.clear();
        if (changed == 2) o.pointerRva = 0;
        if (changed == 3) o.pointerRva.reset();
        refused([&] { f.observe(o); }); CHECK(!f.globals());
    }
    f.existing(std::nullopt); REQUIRE(f.globals());
    CHECK(!f.globals()->counterFor(31)->value());
}

TEST_CASE(widget_paint_cleanup_known_null_remains_a_lazy_global_guard)
{
    Fixture f; auto o = f.observation(); o.currentCounterAlias = 0; f.observe(o);
    const auto globals = f.globals(); REQUIRE(globals);
    PaintDeviceCleanupCall call{globals};
    CHECK(call.resume().issue == Issue::LazyGlobalUnrecovered); CHECK(call.effects().empty());
    o = f.observation(); o.counters = {{31, 0}}; o.currentCounterAlias = 31;
    refused([&] { f.observe(o); });
    CHECK(f.globals() == globals); CHECK(!globals->counterFor(31));
    CHECK(call.resume().issue == Issue::LazyGlobalUnrecovered);
}

TEST_CASE(widget_paint_cleanup_late_value_refines_the_same_owner_without_replaying_getters)
{
    Fixture f; f.existing(std::nullopt); const auto globals = f.globals(); REQUIRE(globals);
    const auto counter = globals->counterFor(31); REQUIRE(counter);
    PaintDeviceCleanupCall call{globals};
    CHECK(call.resume().issue == Issue::CounterValueUnavailable);
    CHECK(call.report().stage == Stage::CounterComparison);
    const auto effects = call.effects(); REQUIRE(effects.size() == 2u);
    f.existing(0); CHECK(f.globals() == globals); CHECK(globals->counterFor(31) == counter);
    CHECK(counter->value() == 0); CHECK(call.effects() == effects);
    CHECK(call.resume().stage == Stage::Returned); CHECK(call.effects().size() == 3u);
    const auto returned = call.effects(); CHECK(call.resume().stage == Stage::Returned);
    CHECK(call.effects() == returned);
}

TEST_CASE(widget_paint_cleanup_unknown_pointer_can_bind_to_explicit_existing_counter)
{
    Fixture f; auto o = f.observation(); o.counters = {{31, 0}}; f.observe(o);
    const auto globals = f.globals(); REQUIRE(globals); REQUIRE(globals->counterFor(31));
    PaintDeviceCleanupCall call{globals};
    CHECK(call.resume().issue == Issue::CounterPointerUnavailable); CHECK(call.effects().empty());
    o = f.observation(); o.currentCounterAlias = 31; f.observe(o);
    CHECK(f.globals() == globals); CHECK(call.effects().empty());
    CHECK(call.resume().stage == Stage::Returned);
}

TEST_CASE(widget_paint_cleanup_same_runtime_shares_owner_after_each_receiver_qualifies)
{
    Fixture first; first.existing(0); const auto globals = first.globals(); REQUIRE(globals);
    Fixture second(first.runtime, true, "Different actual private widget receipt");
    CHECK(!second.globals());
    second.observe(second.observation());
    CHECK(second.globals() == globals); CHECK(globals->qualification() == first.runtime->qualification());
    PaintDeviceCleanupCall a{globals}, b{second.globals()};
    CHECK(a.resume().stage == Stage::Returned); CHECK(b.effects().empty());
    CHECK(b.resume().stage == Stage::Returned); CHECK(a.counterOwner() == b.counterOwner());
    CHECK(globals->counterFor(31)->value() == 0);
}

TEST_CASE(widget_paint_cleanup_late_invalid_link_preserves_values_aliases_and_receiver_access)
{
    Fixture first; first.existing(std::nullopt); const auto globals = first.globals(); REQUIRE(globals);
    const auto counter = globals->counterFor(31); REQUIRE(counter);
    PaintDeviceCleanupCall call{globals}; CHECK(call.resume().issue == Issue::CounterValueUnavailable);
    Fixture second(first.runtime); CHECK(!second.globals());
    auto o = second.observation(); o.counters = {{31, 0}, {32, 0}}; o.currentCounterAlias = 99;
    refused([&] { second.observe(o); });
    CHECK(!second.globals()); CHECK(first.globals() == globals);
    CHECK(!counter->value()); CHECK(!globals->counterFor(32));
    CHECK(call.resume().issue == Issue::CounterValueUnavailable); CHECK(call.effects().size() == 2u);
    o = second.observation(); o.counters = {{31, 0}}; o.currentCounterAlias = 31; second.observe(o);
    CHECK(second.globals() == globals); CHECK(call.resume().stage == Stage::Returned);
}

TEST_CASE(widget_paint_cleanup_duplicate_alias_and_pointer_replacement_reject_without_refinement)
{
    Fixture f; f.existing(std::nullopt); const auto globals = f.globals(); REQUIRE(globals);
    const auto counter = globals->counterFor(31); REQUIRE(counter);
    auto o = f.observation(); o.counters = {{31, 0}, {31, 0}};
    refused([&] { f.observe(o); }); CHECK(!counter->value());
    o = f.observation(); o.counters = {{31, 0}, {0, 0}};
    refused([&] { f.observe(o); }); CHECK(!counter->value());
    o = f.observation(); o.counters = {{31, 0}, {32, 0}}; o.currentCounterAlias = 32;
    refused([&] { f.observe(o); }); CHECK(!counter->value()); CHECK(!globals->counterFor(32));
    o = f.observation(); o.currentCounterAlias = 0;
    refused([&] { f.observe(o); }); CHECK(f.globals() == globals);
}

TEST_CASE(widget_paint_cleanup_different_runtimes_and_released_owner_remain_distinct)
{
    Fixture first, second; first.existing(0); second.existing(0);
    const auto a = first.globals(), b = second.globals(); REQUIRE(a); REQUIRE(b);
    CHECK(a != b); CHECK(a->counterFor(31) != b->counterFor(31));
    a->releaseNativeOwner(); CHECK(!first.globals()); CHECK(second.globals() == b);
    refused([&] { first.existing(0); }); CHECK(!first.globals());
    CHECK(b->nativeOwnerAlive()); CHECK(b->counterFor(31)->value() == 0);
}

TEST_CASE(widget_paint_cleanup_dead_widget_cannot_use_owner_while_other_widget_can)
{
    Fixture first; first.existing(0); const auto globals = first.globals(); REQUIRE(globals);
    Fixture second(first.runtime); second.observe(second.observation());
    const auto widget = first.factory->widgetState(); const auto o = first.observation();
    first.factory.reset();
    CHECK(!first.runtime->paintDeviceCleanupGlobals(*widget));
    refused([&] { first.runtime->observePaintDeviceCleanupInputs(widget, o); });
    CHECK(second.globals() == globals); CHECK(globals->nativeOwnerAlive());
}

TEST_CASE(widget_paint_cleanup_runtime_end_invalidates_escaped_global_and_counter_leases)
{
    std::shared_ptr<PaintDeviceCleanupGlobals> escaped;
    std::shared_ptr<const PaintDeviceCleanupCounterState> counter;
    std::unique_ptr<PaintDeviceCleanupCall> call;
    {
        Fixture f; f.existing(std::nullopt); escaped = f.globals(); REQUIRE(escaped);
        counter = escaped->counterFor(31); REQUIRE(counter);
        call = std::make_unique<PaintDeviceCleanupCall>(escaped);
        CHECK(call->resume().issue == Issue::CounterValueUnavailable);
    }
    CHECK(!escaped->nativeOwnerAlive()); CHECK(!escaped->counterFor(31)); CHECK(!counter->value());
    const auto effects = call->effects();
    CHECK(call->resume().issue == Issue::GlobalsUnavailable); CHECK(call->effects() == effects);
}

TEST_CASE(widget_paint_cleanup_observation_does_not_advance_editor_or_infer_global_from_image_refs)
{
    Fixture f; const auto* base = f.factory->nativeBaseConstruction();
    const auto report = base->report(); const auto effects = base->effects();
    const auto qobjects = f.runtime->retainedQObjectConstructionDelta();
    WidgetTextureImageObservation image;
    image.receiver = f.factory->widgetState(); image.qualification = f.privateQualification;
    image.imageData = {{31, {5, std::nullopt}}};
    WidgetBrushTextureImageObservation source;
    source.brushAlias = 23; source.pixmapSelected = false;
    source.imageDataAlias = 31; source.imagePaintingActiveWord = 0; image.brushes = {source};
    f.runtime->observeTextureImageInputs(f.factory->widgetState(), image);
    const auto data = f.runtime->textureImageDataFor(*f.factory->widgetState(), 31); REQUIRE(data);
    CHECK(data->temporaryValueReferences() == 0u);
    f.existing(-1); const auto globals = f.globals(); REQUIRE(globals);
    PaintDeviceCleanupCall call{globals}; CHECK(call.resume().issue == Issue::NonzeroCleanupUnrecovered);
    CHECK(globals->counterFor(31)->value() == -1); CHECK(data->temporaryValueReferences() == 0u);
    CHECK(base->report().stage == report.stage); CHECK(base->report().issue == report.issue);
    CHECK(base->effects() == effects); CHECK(f.runtime->retainedQObjectConstructionDelta() == qobjects);
    CHECK(!base->report().completeForPublicSave); CHECK(!base->report().applicationWrappersReturned);
    EditorSetupState setup; CHECK(!f.factory->bindSetupOwner(setup));
}

int main() { return testkit::run_all(); }
