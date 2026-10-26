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
constexpr auto qualification = "Isolated actual editor texture image binding";
using Issue = BrushTextureImageIssue;
template<class F> void refused(F&& f)
{
    bool caught = false;
    try { f(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
WidgetBrushTextureImageObservation imageSource(WidgetBrushAlias brush,
    std::optional<WidgetTextureImageDataAlias> data = std::nullopt)
{
    WidgetBrushTextureImageObservation o;
    o.brushAlias = brush; o.pixmapSelected = false;
    o.imagePaintingActiveWord = 0; o.imageDataAlias = data;
    return o;
}
struct Fixture {
    Project project;
    Operation operation;
    RemeshState remesh;
    std::shared_ptr<WidgetRuntimeState> runtime;
    std::unique_ptr<StagedEditor300> factory;
    explicit Fixture(std::shared_ptr<WidgetRuntimeState> shared = {}, bool qualify = true,
        WidgetPaletteAlias palette = 17)
        : runtime(shared ? std::move(shared) : std::make_shared<WidgetRuntimeState>(true, qualification))
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
        o.brushData = {{23, 24, 255}, {24, 24, 0}, {25, 1, 255}, {26, 16, 255}, {27, 24, 255}};
        o.paletteBrushes = {{17, 0, 10, 23}, {17, 1, 10, 24}, {17, 0, 9, 25},
            {17, 2, 10, 26}, {17, 0, 8, 27}, {18, 0, 10, 23}};
        runtime->observePrivateConstructionInputs(factory->widgetState(), o);
        WidgetApplicationEventObservation app; app.present = true;
        runtime->observeApplicationEvents(app, qualification);
        const auto pending = factory->continueNativeBase(binding(),
            {factory.get(), qualification, runtime->threadState()});
        CHECK(pending.stage == EditorBaseStage::TextureImage);
        const auto* call = factory->nativeBaseConstruction()->textureImageCall();
        if (brush()->textureImageSource()) {
            // Every prebound payload used by this fixture has unknown format.
            CHECK(pending.issue == EditorBaseIssue::TextureImagePending);
            REQUIRE(call);
            CHECK(call->report().issue == Issue::ImageFormatUnavailable);
        } else {
            CHECK(pending.issue == EditorBaseIssue::TextureImageSourceUnavailable);
            CHECK(!call);
        }
    }
    Editor300Binding binding() { return {project, operation, *operation.engineModel, remesh}; }
    WidgetTextureImageObservation observation() const
    {
        WidgetTextureImageObservation o; o.receiver = factory->widgetState(); o.qualification = qualification;
        return o;
    }
    void observe(const WidgetTextureImageObservation& o)
    { runtime->observeTextureImageInputs(factory->widgetState(), o); }
    std::shared_ptr<const BrushTextureImageDataState> data(WidgetTextureImageDataAlias alias) const
    { return runtime->textureImageDataFor(*factory->widgetState(), alias); }
    std::shared_ptr<const WidgetBrushDataState> brush(std::uint32_t group = 0, std::uint32_t role = 10) const
    { return factory->widgetState()->palette()->data->brushAt(group, role); }
};
}

TEST_CASE(widget_texture_image_requires_actual_private_input_receiver_and_qualification)
{
    Fixture f;
    auto o = f.observation(); o.imageData = {{31, {5, std::nullopt}}}; o.brushes = {imageSource(23, 31)};
    o.qualification.clear(); refused([&] { f.observe(o); });
    o.qualification = "Other stage"; refused([&] { f.observe(o); });
    o.qualification = qualification; o.receiver.reset(); refused([&] { f.observe(o); });
    Fixture unqualified({}, false);
    o = unqualified.observation(); o.imageData = {{31, {5, std::nullopt}}};
    refused([&] { unqualified.observe(o); });
    CHECK(!f.data(31)); CHECK(!f.brush()->textureImageSource());
}

TEST_CASE(widget_texture_image_rejects_foreign_runtime_and_receiver_control_blocks)
{
    Fixture f, foreign;
    auto o = f.observation(); o.receiver = foreign.factory->widgetState();
    refused([&] { f.observe(o); });
    o = foreign.observation();
    refused([&] { f.runtime->observeTextureImageInputs(foreign.factory->widgetState(), o); });
    const auto real = f.factory->widgetState();
    std::shared_ptr<WidgetState> alias(real.get(), [](WidgetState*) {});
    o = f.observation(); o.receiver = alias;
    refused([&] { f.observe(o); });
    refused([&] { f.runtime->observeTextureImageInputs(alias, o); });
    CHECK(!f.brush()->textureImageSource());
}

TEST_CASE(widget_texture_image_interns_new_data_and_refines_same_payload_without_native_effects)
{
    Fixture f; auto o = f.observation(); o.imageData = {{31, {}}};
    o.brushes = {imageSource(23, 31)}; f.observe(o);
    const auto data = f.data(31); REQUIRE(data);
    const auto source = f.brush()->textureImageSource(); REQUIRE(source);
    CHECK(data->observationAlias() == 31u); CHECK(data->temporaryValueReferences() == 0u);
    BrushTextureImageOpacityCall call{source};
    CHECK(call.resume().issue == Issue::ImageFormatUnavailable);
    CHECK(data->temporaryValueReferences() == 1u);
    const auto effects = call.effects();
    o = f.observation(); o.imageData = {{31, {5, std::nullopt}}}; f.observe(o);
    CHECK(f.data(31) == data); CHECK(f.brush()->textureImageSource() == source);
    CHECK(data->temporaryValueReferences() == 1u); CHECK(call.effects() == effects);
    CHECK(call.resume().opaque == false); CHECK(data->temporaryValueReferences() == 0u);
    f.observe(o); CHECK(f.data(31) == data); CHECK(f.brush()->textureImageSource() == source);
}

TEST_CASE(widget_texture_image_distinguishes_unknown_image_pointer_from_known_null)
{
    Fixture f; auto o = f.observation(); auto image = imageSource(23);
    image.cachedPixmapPresent = false; o.brushes = {image}; f.observe(o);
    const auto source = f.brush()->textureImageSource(); REQUIRE(source);
    BrushTextureImageOpacityCall call{source};
    CHECK(call.resume().issue == Issue::ImageDataUnavailable);
    o = f.observation(); o.brushes = {imageSource(23, 0)}; f.observe(o);
    CHECK(f.brush()->textureImageSource() == source);
    CHECK(call.resume().opaque == true); CHECK(!f.data(0));
    o = f.observation(); o.imageData = {{31, {5, std::nullopt}}}; o.brushes = {imageSource(23, 31)};
    refused([&] { f.observe(o); });
    CHECK(!f.data(31)); CHECK(f.brush()->textureImageSource() == source);
}

TEST_CASE(widget_texture_image_rejects_data_owner_replacement_before_interning_new_alias)
{
    Fixture f; auto o = f.observation(); o.imageData = {{31, {5, std::nullopt}}};
    o.brushes = {imageSource(23, 31)}; f.observe(o);
    const auto data = f.data(31); const auto source = f.brush()->textureImageSource();
    o = f.observation(); o.imageData = {{32, {6, true}}}; o.brushes = {imageSource(23, 32)};
    refused([&] { f.observe(o); });
    CHECK(!f.data(32)); CHECK(f.data(31) == data); CHECK(f.brush()->textureImageSource() == source);
    o = f.observation(); o.brushes = {imageSource(23, 0)};
    refused([&] { f.observe(o); });
    BrushTextureImageOpacityCall call{source}; CHECK(call.resume().opaque == false);
}

TEST_CASE(widget_texture_image_late_bad_link_preserves_metadata_aliases_payloads_and_receiver_grants)
{
    Fixture first; auto o = first.observation(); o.imageData = {{31, {}}};
    o.brushes = {imageSource(23, 31)}; first.observe(o);
    const auto data = first.data(31); REQUIRE(data);
    BrushTextureImageOpacityCall call{first.brush()->textureImageSource()};
    CHECK(call.resume().issue == Issue::ImageFormatUnavailable);
    Fixture second(first.runtime, true, 18);
    // The second editor has its own pending copied QImage as well as call.
    CHECK(data->temporaryValueReferences() == 2u);
    o = second.observation(); o.imageData = {{31, {5, true}}, {32, {6, true}}};
    o.brushes = {imageSource(27, 32), imageSource(24, 99)};
    refused([&] { second.observe(o); });
    CHECK(!second.data(31)); CHECK(!second.data(32));
    CHECK(!first.data(32)); CHECK(first.data(31) == data);
    CHECK(!first.brush(0, 8)->textureImageSource()); CHECK(!first.brush(1)->textureImageSource());
    CHECK(data->temporaryValueReferences() == 2u);
    CHECK(call.resume().issue == Issue::ImageFormatUnavailable);
    // Declaring32 afterwards proves the rejected batch never interned it with6.
    o = first.observation(); o.imageData = {{31, {4, false}}, {32, {4, false}}}; first.observe(o);
    CHECK(call.resume().opaque == true); CHECK(first.data(32));
    CHECK(data->temporaryValueReferences() == 1u);
    CHECK(second.factory->nativeBaseConstruction()->textureImageCall()->report().issue == Issue::ImageFormatUnavailable);
    CHECK(!second.data(31)); CHECK(!second.data(32));
}

TEST_CASE(widget_texture_image_late_bad_link_does_not_refine_an_earlier_existing_source)
{
    Fixture f; auto o = f.observation(); o.brushes = {imageSource(23)}; f.observe(o);
    const auto source = f.brush()->textureImageSource(); REQUIRE(source);
    BrushTextureImageOpacityCall call{source}; CHECK(call.resume().issue == Issue::ImageDataUnavailable);
    o = f.observation(); o.imageData = {{31, {5, false}}};
    o.brushes = {imageSource(23, 31), imageSource(24, 99)};
    refused([&] { f.observe(o); });
    CHECK(!f.data(31)); CHECK(f.brush()->textureImageSource() == source);
    CHECK(call.resume().issue == Issue::ImageDataUnavailable);
    o = f.observation(); auto image = imageSource(23, 0); image.cachedPixmapPresent = false;
    o.brushes = {image}; f.observe(o);
    CHECK(call.resume().opaque == true);
}

TEST_CASE(widget_texture_image_conflicting_metadata_rejects_the_entire_envelope)
{
    Fixture f; auto o = f.observation(); o.imageData = {{31, {4, std::nullopt}}};
    o.brushes = {imageSource(23, 31)}; f.observe(o);
    const auto data = f.data(31); REQUIRE(data);
    BrushTextureImageOpacityCall call{f.brush()->textureImageSource()};
    CHECK(call.resume().issue == Issue::ImageColorTableAlphaUnavailable);
    o = f.observation(); o.imageData = {{32, {6, true}}, {31, {5, true}}};
    o.brushes = {imageSource(24, 32)};
    refused([&] { f.observe(o); });
    CHECK(!f.data(32)); CHECK(!f.brush(1)->textureImageSource());
    CHECK(call.resume().issue == Issue::ImageColorTableAlphaUnavailable);
    CHECK(data->temporaryValueReferences() == 1u);
    o = f.observation(); o.imageData = {{31, {4, false}}}; f.observe(o);
    CHECK(call.resume().opaque == true); CHECK(data->temporaryValueReferences() == 0u);
}

TEST_CASE(widget_texture_image_rejects_zero_duplicate_and_missing_data_aliases)
{
    Fixture f;
    auto o = f.observation(); o.imageData = {{0, {5, true}}};
    refused([&] { f.observe(o); }); CHECK(!f.data(0));
    o = f.observation(); o.imageData = {{31, {}}, {31, {5, true}}};
    refused([&] { f.observe(o); }); CHECK(!f.data(31));
    o = f.observation(); o.brushes = {imageSource(23, 31)};
    refused([&] { f.observe(o); }); CHECK(!f.brush()->textureImageSource());
}

TEST_CASE(widget_texture_image_rejects_duplicate_absent_and_nontexture_brush_links)
{
    Fixture f;
    for (const WidgetBrushAlias alias : {0u, 25u, 26u, 99u}) {
        auto o = f.observation(); o.imageData = {{31, {5, true}}};
        o.brushes = {imageSource(alias, 31)};
        refused([&] { f.observe(o); }); CHECK(!f.data(31));
    }
    auto o = f.observation(); o.imageData = {{31, {5, true}}};
    o.brushes = {imageSource(23, 31), imageSource(23, 31)};
    refused([&] { f.observe(o); });
    CHECK(!f.data(31)); CHECK(!f.brush()->textureImageSource());
}

TEST_CASE(widget_texture_image_conflicting_source_fields_leave_other_refinements_uncommitted)
{
    Fixture f; auto o = f.observation(); o.brushes = {imageSource(23)}; f.observe(o);
    const auto source = f.brush()->textureImageSource(); REQUIRE(source);
    o = f.observation(); o.imageData = {{31, {5, true}}};
    auto conflict = imageSource(23, 31); conflict.pixmapSelected = true;
    o.brushes = {imageSource(24, 31), conflict};
    refused([&] { f.observe(o); });
    CHECK(!f.data(31)); CHECK(!f.brush(1)->textureImageSource());
    BrushTextureImageOpacityCall call{source}; CHECK(call.resume().issue == Issue::ImageDataUnavailable);
}

TEST_CASE(widget_texture_image_same_brush_shares_payload_distinct_brushes_share_only_image_data)
{
    Fixture f; auto o = f.observation(); o.imageData = {{31, {}}};
    o.brushes = {imageSource(23, 31), imageSource(24, 31)}; f.observe(o);
    const auto first = f.brush()->textureImageSource(); const auto other = f.brush(1)->textureImageSource();
    REQUIRE(first); REQUIRE(other); CHECK(first != other);
    const auto data = f.data(31); REQUIRE(data);
    Fixture shared(f.runtime, true, 18);
    CHECK(shared.brush() == f.brush()); CHECK(shared.brush()->textureImageSource() == first);
    CHECK(!shared.data(31));
    CHECK(data->temporaryValueReferences() == 1u); // shared editor's pending copy
    BrushTextureImageOpacityCall a{first}, b{other};
    CHECK(a.resume().issue == Issue::ImageFormatUnavailable);
    CHECK(b.resume().issue == Issue::ImageFormatUnavailable);
    CHECK(data->temporaryValueReferences() == 3u);
    o = f.observation(); o.imageData = {{31, {5, std::nullopt}}}; f.observe(o);
    CHECK(data->temporaryValueReferences() == 3u);
    CHECK(a.resume().opaque == false); CHECK(data->temporaryValueReferences() == 2u);
    CHECK(b.resume().opaque == false); CHECK(data->temporaryValueReferences() == 1u);
    CHECK(shared.factory->nativeBaseConstruction()->textureImageCall()->report().issue == Issue::ImageFormatUnavailable);
}

TEST_CASE(widget_texture_image_receiver_alias_grants_require_explicit_data_or_source_links)
{
    Fixture first; auto o = first.observation(); o.imageData = {{31, {}}, {32, {}}};
    o.brushes = {imageSource(23, 31)}; first.observe(o);
    const auto data = first.data(31); REQUIRE(data); REQUIRE(first.data(32));
    Fixture second(first.runtime, true, 18);
    CHECK(!second.data(31)); CHECK(!second.data(32));
    second.observe(second.observation());
    CHECK(!second.data(31)); CHECK(!second.data(32));
    o = second.observation(); o.brushes = {imageSource(23, 31)}; second.observe(o);
    CHECK(second.data(31) == data); CHECK(!second.data(32));
    o = second.observation(); o.imageData = {{32, {}}}; second.observe(o);
    CHECK(second.data(31) == data); CHECK(second.data(32) == first.data(32));
    Fixture foreign; CHECK(!foreign.runtime->textureImageDataFor(*second.factory->widgetState(), 31));
}

TEST_CASE(widget_texture_image_observation_does_not_resume_editor_or_change_image_reference_ledger)
{
    Fixture f; const auto* base = f.factory->nativeBaseConstruction();
    const auto before = base->report(); const auto effects = base->effects();
    CHECK(before.stage == EditorBaseStage::TextureImage);
    CHECK(before.issue == EditorBaseIssue::TextureImageSourceUnavailable);
    CHECK(!base->textureImageCall());
    const auto qobjects = f.runtime->retainedQObjectConstructionDelta();
    auto o = f.observation(); o.imageData = {{31, {5, std::nullopt}}};
    o.brushes = {imageSource(23, 31)}; f.observe(o);
    CHECK(base->report().stage == before.stage); CHECK(base->report().issue == before.issue);
    CHECK(!base->textureImageCall());
    CHECK(base->effects() == effects); CHECK(f.runtime->retainedQObjectConstructionDelta() == qobjects);
    CHECK(f.data(31)->temporaryValueReferences() == 0u); CHECK(f.factory->widgetState()->opaque() == false);
    CHECK(!base->report().completeForPublicSave); CHECK(!base->report().applicationWrappersReturned);
    EditorSetupState setup; CHECK(!f.factory->bindSetupOwner(setup));
}

TEST_CASE(widget_texture_image_runtime_state_preserves_pixmap_conversion_and_active_copy_guards)
{
    Fixture f;
    auto o = f.observation(); auto pixmap = imageSource(23); pixmap.pixmapSelected = true;
    o.brushes = {pixmap}; f.observe(o);
    BrushTextureImageOpacityCall pixmapCall{f.brush()->textureImageSource()};
    CHECK(pixmapCall.resume().issue == Issue::PixmapPathUnrecovered);
    o = f.observation(); auto conversion = imageSource(24, 0); conversion.cachedPixmapPresent = true;
    o.brushes = {conversion}; f.observe(o);
    BrushTextureImageOpacityCall conversionCall{f.brush(1)->textureImageSource()};
    CHECK(conversionCall.resume().issue == Issue::ImageConversionUnrecovered);
    o = f.observation(); o.imageData = {{31, {5, false}}};
    auto active = imageSource(27, 31); active.imagePaintingActiveWord = 1;
    o.brushes = {active}; f.observe(o);
    BrushTextureImageOpacityCall activeCall{f.brush(0, 8)->textureImageSource()};
    CHECK(activeCall.resume().issue == Issue::ActiveImageCopyUnrecovered);
    CHECK(f.data(31)->temporaryValueReferences() == 0u);
}

TEST_CASE(widget_texture_image_dead_widget_cannot_observe_or_expose_qualified_aliases)
{
    Fixture f; auto o = f.observation(); o.imageData = {{31, {5, true}}};
    o.brushes = {imageSource(23, 31)}; f.observe(o);
    const auto widget = f.factory->widgetState(); const auto data = f.data(31); REQUIRE(data);
    f.factory.reset();
    refused([&] { f.runtime->observeTextureImageInputs(widget, o); });
    CHECK(!f.runtime->textureImageDataFor(*widget, 31));
    CHECK(data->temporaryValueReferences() == 0u);
}

int main() { return testkit::run_all(); }
