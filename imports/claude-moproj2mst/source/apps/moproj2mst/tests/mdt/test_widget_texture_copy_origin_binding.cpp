// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <memory>
#include <string>
#include <utility>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/engine/paint_device_active_copy.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using namespace m2m::engine;
using namespace m2m::engine::simcontrol300;
namespace {
constexpr auto moduleHash = "c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887";
template<class F> void refused(F&& f) {
    bool caught = false; try { f(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
struct Fixture {
    Project project;
    Operation operation;
    RemeshState remesh;
    std::string qualification;
    std::shared_ptr<WidgetRuntimeState> runtime;
    std::unique_ptr<StagedEditor300> factory;
    explicit Fixture(std::shared_ptr<WidgetRuntimeState> shared = {}, bool qualify = true,
        std::string receipt = "Isolated texture-copy receiver")
        : qualification(std::move(receipt)), runtime(shared ? std::move(shared)
            : std::make_shared<WidgetRuntimeState>(true, "Isolated actual copy-source runtime"))
    {
        operation.project = &project; operation.engineModel = std::make_shared<GuiModel>(nullptr, 3);
        factory = std::make_unique<StagedEditor300>(binding()); REQUIRE(factory->bindWidgetRuntime(binding(), runtime));
        if (!qualify) return;
        WidgetPrivateConstructionObservation o; o.qualification = qualification;
        o.fontData = {{11, 168, 0}}; o.paletteData = {{17}};
        o.defaultFont = WidgetFontValueObservation{11, 0}; o.defaultPalette = WidgetPaletteValueObservation{17, 0};
        o.defaultLocale = WidgetLocaleValue{0x187, 0}; o.applicationType = 1; o.applicationLayoutDirection = 0;
        o.applicationModalState = false; o.brushData = {{23, 24, 255}, {24, 24, 0}, {25, 1, 255}};
        o.paletteBrushes = {{17, 0, 10, 23}, {17, 0, 9, 24}, {17, 0, 8, 25}};
        runtime->observePrivateConstructionInputs(factory->widgetState(), o);
    }
    Editor300Binding binding() { return {project, operation, *operation.engineModel, remesh}; }
    WidgetTextureImageObservation textureObservation() const {
        WidgetTextureImageObservation o; o.receiver = factory->widgetState(); o.qualification = qualification; return o;
    }
    void source(WidgetBrushAlias alias = 23, std::optional<std::uint16_t> word = {},
        std::optional<WidgetTextureImageDataAlias> data = {}) {
        auto o = textureObservation(); WidgetBrushTextureImageObservation b; b.brushAlias = alias;
        b.imagePaintingActiveWord = word; b.imageDataAlias = data; o.brushes = {b};
        runtime->observeTextureImageInputs(factory->widgetState(), o);
    }
    WidgetPaintDeviceCleanupObservation moduleObservation() const {
        WidgetPaintDeviceCleanupObservation o; o.receiver = factory->widgetState(); o.qualification = qualification;
        o.moduleSha256 = moduleHash; o.pointerRva = 0x9cb858u; return o;
    }
    void module() { runtime->observePaintDeviceCleanupInputs(factory->widgetState(), moduleObservation()); }
    void existing(std::optional<std::int32_t> value) {
        auto o = moduleObservation(); o.counters = {{31, value}}; o.currentCounterAlias = 31;
        runtime->observePaintDeviceCleanupInputs(factory->widgetState(), o);
    }
    std::shared_ptr<PaintDeviceActiveCopySourceState> bind(WidgetBrushAlias alias = 23) {
        return runtime->bindTextureImageCopySource(factory->widgetState(), alias);
    }
    std::shared_ptr<const BrushTextureImageState> payload(std::uint32_t role = 10) const {
        const auto* inputs = runtime->privateConstructionInputs(*factory->widgetState());
        REQUIRE(inputs); REQUIRE(inputs->defaultPalette);
        const auto brush = inputs->defaultPalette->data->brushAt(0, role); REQUIRE(brush); return brush->textureImageSource();
    }
};
}

TEST_CASE(texture_copy_binding_requires_actual_private_receiver_and_module_grant)
{
    Fixture f; f.source(); refused([&] { f.bind(); });
    Fixture unqualified({}, false); refused([&] { unqualified.bind(); });
    refused([&] { f.runtime->bindTextureImageCopySource(nullptr, 23); });
    Fixture foreign; foreign.source(); foreign.module();
    refused([&] { f.runtime->bindTextureImageCopySource(foreign.factory->widgetState(), 23); });
    auto real = f.factory->widgetState(); std::shared_ptr<WidgetState> rewrapped(real.get(), [](WidgetState*) {});
    f.module(); refused([&] { f.runtime->bindTextureImageCopySource(rewrapped, 23); });
    auto source = f.bind(); REQUIRE(source); CHECK(source->textureImageSource() == f.payload());
}

TEST_CASE(texture_copy_binding_requires_positive_existing_style24_payload)
{
    Fixture f; f.module();
    refused([&] { f.bind(0); }); refused([&] { f.bind(777); });
    refused([&] { f.bind(25); }); refused([&] { f.bind(23); });
    f.source(); const auto first = f.bind(); REQUIRE(first);
    refused([&] { f.bind(24); }); CHECK(f.bind() == first);
}

TEST_CASE(texture_copy_binding_interns_exact_payload_without_defaults_or_execution)
{
    Fixture f; f.source(); f.module(); CHECK(!f.factory->nativeBaseConstruction());
    auto source = f.bind(); REQUIRE(source); const auto payload = f.payload(); REQUIRE(payload);
    CHECK(source->textureImageBound()); CHECK(source->textureImageSource().get() == payload.get());
    CHECK(!source->textureImageSource().owner_before(payload)); CHECK(!payload.owner_before(source->textureImageSource()));
    CHECK(source->cleanupGlobalsOwner().lock() == f.runtime->paintDeviceCleanupGlobals(*f.factory->widgetState()));
    CHECK(!source->paintingWord()); CHECK(!source->imageData()); CHECK(f.bind() == source);
    CHECK(!f.factory->nativeBaseConstruction());
    PaintDeviceActiveCopyCall copy{source, "Actual bound copy"}; CHECK(copy.effects().empty());
    CHECK(copy.resume().issue == PaintDeviceActiveCopyIssue::SourcePaintingWordUnavailable);
    CHECK(copy.effects().size() == 1u); CHECK(!f.factory->nativeBaseConstruction());
}

TEST_CASE(texture_copy_binding_reuses_source_across_widgets_only_after_current_module_grant)
{
    Fixture first; Fixture second(first.runtime, true, "Second private receiver");
    first.source(23, 1, 0); first.existing(0); auto view = first.bind(); REQUIRE(view);
    refused([&] { second.bind(); }); second.module(); CHECK(second.bind() == view);
    CHECK(second.payload() == first.payload()); CHECK(view->paintingWord() == 1);
    auto releasedWidget = first.factory->widgetState(); first.factory.reset(); CHECK(!releasedWidget->nativeOwnerAlive());
    refused([&] { first.runtime->bindTextureImageCopySource(releasedWidget, 23); });
    CHECK(second.bind() == view); CHECK(view->nativeOwnerAlive()); CHECK(!second.factory->nativeBaseConstruction());
}

TEST_CASE(texture_copy_binding_keeps_distinct_values_despite_shared_image_data)
{
    Fixture f; auto o = f.textureObservation(); o.imageData = {{31, {}}};
    WidgetBrushTextureImageObservation a; a.brushAlias = 23; a.imageDataAlias = 31; a.imagePaintingActiveWord = 1;
    auto b = a; b.brushAlias = 24; o.brushes = {a, b};
    f.runtime->observeTextureImageInputs(f.factory->widgetState(), o); f.module();
    auto first = f.bind(23), second = f.bind(24); REQUIRE(first); REQUIRE(second);
    CHECK(first != second); CHECK(first->textureImageSource() != second->textureImageSource());
    REQUIRE(first->imageData()); REQUIRE(second->imageData()); CHECK(*first->imageData() == *second->imageData());
    CHECK(first->cleanupGlobalsOwner().lock() == second->cleanupGlobalsOwner().lock());
    CHECK(f.bind(23) == first); CHECK(f.bind(24) == second);
}

TEST_CASE(texture_copy_binding_late_observations_refine_same_source_without_continuation)
{
    Fixture f; f.source(); f.existing(0); auto view = f.bind(); REQUIRE(view);
    PaintDeviceActiveCopyCall copy{view, "Actual bound copy"};
    CHECK(copy.resume().issue == PaintDeviceActiveCopyIssue::SourcePaintingWordUnavailable);
    const auto effects = copy.effects(); f.source(23, 1, 0);
    CHECK(f.bind() == view); CHECK(copy.effects() == effects); CHECK(view->paintingWord() == 1);
    CHECK(&view->imageData() == &f.payload()->imageDataOwner()); CHECK(!f.factory->nativeBaseConstruction());
    CHECK(copy.resume().stage == PaintDeviceActiveCopyStage::Returned);
    auto value = copy.takeReturnedImage(); REQUIRE(value);
    CHECK(value->cleanupGlobalsOwner().lock() == f.runtime->paintDeviceCleanupGlobals(*f.factory->widgetState()));
    PaintDeviceNullImageFinishCall finish{std::move(value)};
    CHECK(finish.resume().stage == PaintDeviceNullImageFinishStage::Returned); CHECK(finish.report().opaque == true);
    CHECK(!finish.report().completeForPublicSave); CHECK(!f.factory->nativeBaseConstruction());
}

TEST_CASE(texture_copy_binding_failed_metadata_envelope_preserves_view_and_actual_fields)
{
    Fixture f; f.source(); f.module(); auto view = f.bind(); REQUIRE(view);
    auto o = f.textureObservation(); WidgetBrushTextureImageObservation a; a.brushAlias = 23; a.imagePaintingActiveWord = 1;
    auto bad = a; bad.brushAlias = 24; bad.imageDataAlias = 999; o.brushes = {a, bad};
    refused([&] { f.runtime->observeTextureImageInputs(f.factory->widgetState(), o); });
    CHECK(!view->paintingWord()); CHECK(!view->imageData()); CHECK(f.bind() == view); CHECK(!f.payload(9));
    PaintDeviceActiveCopySourceObservation duplicate; duplicate.receiver = view; duplicate.qualification = view->qualification();
    duplicate.paintingWord = 1; duplicate.imageData = std::shared_ptr<BrushTextureImageDataState>{};
    CHECK(!view->observe(duplicate)); CHECK(!view->paintingWord()); CHECK(!view->imageData());
}

TEST_CASE(texture_copy_binding_module_grant_does_not_infer_existing_counter_zero)
{
    Fixture f; f.source(23, 1, 0); f.module(); auto view = f.bind(); REQUIRE(view);
    const auto globals = view->cleanupGlobalsOwner().lock(); REQUIRE(globals); CHECK(!globals->counterFor(31));
    PaintDeviceActiveCopyCall copy{view, "Unknown global pointer"};
    CHECK(copy.resume().issue == PaintDeviceActiveCopyIssue::InnerCleanupPending); REQUIRE(copy.innerCleanupCall());
    CHECK(copy.innerCleanupCall()->report().issue == PaintDeviceCleanupIssue::CounterPointerUnavailable);
    const auto effects = copy.effects(); CHECK(!copy.takeReturnedImage());
    f.existing(0); CHECK(copy.effects() == effects); CHECK(f.bind() == view);
    CHECK(copy.resume().stage == PaintDeviceActiveCopyStage::Returned);
    auto value = copy.takeReturnedImage(); REQUIRE(value); CHECK(value->cleanupGlobalsOwner().lock() == globals);
}

TEST_CASE(texture_copy_binding_released_view_is_not_replaced_or_revived)
{
    Fixture f; f.source(23, 1, 0); f.existing(0); auto view = f.bind(); REQUIRE(view);
    view->releaseNativeOwner(); CHECK(!view->nativeOwnerAlive());
    refused([&] { f.bind(); }); f.source(23, 1, 0); refused([&] { f.bind(); });
    PaintDeviceActiveCopyCall copy{view, "Released source"};
    CHECK(copy.resume().issue == PaintDeviceActiveCopyIssue::SourceUnavailable); CHECK(copy.effects().empty());
}

TEST_CASE(texture_copy_binding_released_module_does_not_accept_rebind)
{
    Fixture f; f.source(23, 1, 0); f.existing(0); auto view = f.bind(); REQUIRE(view);
    auto globals = view->cleanupGlobalsOwner().lock(); REQUIRE(globals); globals->releaseNativeOwner();
    CHECK(!view->nativeOwnerAlive()); refused([&] { f.bind(); });
    refused([&] { f.module(); }); CHECK(!f.runtime->paintDeviceCleanupGlobals(*f.factory->widgetState()));
    Fixture other; other.module(); PaintDeviceActiveCopyCall copy{view, "Released module source"};
    CHECK(!copy.bindCleanupGlobals(other.runtime->paintDeviceCleanupGlobals(*other.factory->widgetState())));
    CHECK(copy.resume().issue == PaintDeviceActiveCopyIssue::SourceUnavailable); CHECK(copy.effects().empty());
}

TEST_CASE(texture_copy_binding_runtime_death_invalidates_escaped_metadata_and_module_leases)
{
    Fixture f; f.source(23, 1, 0); f.existing(0); auto view = f.bind(); REQUIRE(view);
    auto payload = view->textureImageSource(); auto module = view->cleanupGlobalsOwner().lock(); REQUIRE(module);
    std::weak_ptr<WidgetRuntimeState> runtime = f.runtime; f.runtime.reset(); CHECK(runtime.expired());
    CHECK(!module->nativeOwnerAlive()); CHECK(!view->nativeOwnerAlive()); CHECK(payload->imagePaintingActiveWord() == 1);
    PaintDeviceActiveCopyCall copy{view, "Expired runtime"};
    CHECK(copy.resume().issue == PaintDeviceActiveCopyIssue::SourceUnavailable); CHECK(copy.effects().empty());
}

TEST_CASE(texture_copy_binding_identical_aliases_in_distinct_runtimes_are_distinct_origins)
{
    Fixture first, second; first.source(23, 1, 0); second.source(23, 1, 0); first.existing(0); second.existing(0);
    auto a = first.bind(), b = second.bind(); REQUIRE(a); REQUIRE(b);
    CHECK(a != b); CHECK(a->textureImageSource() != b->textureImageSource());
    CHECK(a->cleanupGlobalsOwner().lock() != b->cleanupGlobalsOwner().lock());
    PaintDeviceActiveCopyCall copy{a, "First runtime copy"}; CHECK(!copy.bindCleanupGlobals(b->cleanupGlobalsOwner().lock()));
    CHECK(copy.resume().stage == PaintDeviceActiveCopyStage::Returned);
    auto value = copy.takeReturnedImage(); REQUIRE(value); CHECK(value->cleanupGlobalsOwner().lock() == a->cleanupGlobalsOwner().lock());
}

int main() { return testkit::run_all(); }
