// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <array>
#include <algorithm>
#include <memory>
#include <limits>
#include <utility>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/engine/widget_events.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using namespace m2m::engine;
using namespace m2m::engine::simcontrol300;

namespace {
struct Fixture {
    Project project;
    Operation operation;
    RemeshState remesh;
    std::shared_ptr<WidgetRuntimeState> runtime;
    std::unique_ptr<StagedEditor300> factory;
    explicit Fixture(std::optional<bool> registry = true, std::shared_ptr<WidgetRuntimeState> shared = {})
        : runtime(shared ? std::move(shared) : std::make_shared<WidgetRuntimeState>(registry, "Isolated actual editor base runtime"))
    {
        operation.project = &project;
        operation.engineModel = std::make_shared<GuiModel>(nullptr, 3);
        factory = std::make_unique<StagedEditor300>(binding());
        REQUIRE(factory->bindWidgetRuntime(binding(), runtime));
    }
    Editor300Binding binding() { return {project, operation, *operation.engineModel, remesh}; }
    EditorBaseContext context()
    { return {factory.get(), "Isolated source-ordered editor base", runtime->threadState()}; }
    WidgetPrivateConstructionObservation inputs()
    {
        WidgetPrivateConstructionObservation o;
        o.qualification = "Isolated current private constructor inputs";
        return o;
    }
    void observe(const WidgetPrivateConstructionObservation& o)
    { runtime->observePrivateConstructionInputs(factory->widgetState(), o); }
    void defaults(std::optional<int> appType = 1, std::optional<int> direction = 0,
        std::optional<bool> modal = false)
    {
        auto o = inputs();
        o.fontData = {{11, 168, 0}};
        o.paletteData = {{17}};
        o.defaultPalette = WidgetPaletteValueObservation{17, 0};
        o.defaultFont = WidgetFontValueObservation{11, 0};
        o.defaultLocale = WidgetLocaleValue{0x187, 0};
        o.applicationType = appType;
        o.applicationLayoutDirection = direction;
        o.applicationModalState = modal;
        observe(o);
    }
    void appPresent(bool present = true)
    {
        WidgetApplicationEventObservation o; o.present = present;
        runtime->observeApplicationEvents(o, "Isolated current application presence");
    }
    EditorBaseReport resume() { return factory->continueNativeBase(binding(), context()); }
    void brush(std::optional<std::int32_t> style, std::optional<std::uint8_t> alpha,
        std::uint32_t group = 0)
    {
        auto o = inputs(); o.brushData = {{23, style, alpha}};
        o.paletteBrushes = {{17, group, 10, 23}}; observe(o);
    }
    WidgetGradientObservation gradientInputs() const
    {
        WidgetGradientObservation o;
        o.receiver = factory->widgetState();
        o.qualification = "Isolated current private constructor inputs";
        return o;
    }
    WidgetGradientSharedNullObservation symbolProfile() const
    {
        WidgetGradientSharedNullObservation o;
        o.moduleSha256 = "bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160";
        o.symbolRva = 0x2f0bb8; o.count = 0; o.capacity = 0; o.flagsLow2 = 1;
        return o;
    }
    void gradientCount(std::optional<std::int32_t> count, bool withSymbol,
        WidgetBrushAlias brushAlias = 23)
    {
        auto o = gradientInputs(); BrushGradientStopDataObservation data; data.count = count;
        o.stopData = {{31, data}};
        o.brushSources = {{brushAlias, WidgetGradientSourceKind::DataAlias, 31}};
        if (withSymbol) o.sharedNull = symbolProfile();
        runtime->observeGradientInputs(factory->widgetState(), o);
    }
    void qualifyGradientSymbol()
    {
        auto o = gradientInputs(); o.sharedNull = symbolProfile();
        runtime->observeGradientInputs(factory->widgetState(), o);
    }
    WidgetRadialObservation radialInputs() const
    {
        WidgetRadialObservation o; o.receiver = factory->widgetState();
        o.qualification = "Isolated current private constructor inputs";
        return o;
    }
    void radialWord(std::optional<std::uint32_t> word, WidgetBrushAlias alias = 23)
    {
        auto o = radialInputs(); o.brushes = {{alias, word}};
        runtime->observeRadialInputs(factory->widgetState(), o);
    }
    WidgetTextureImageObservation textureInputs() const
    {
        WidgetTextureImageObservation o; o.receiver = factory->widgetState();
        o.qualification = "Isolated current private constructor inputs";
        return o;
    }
    void bindImage(std::optional<WidgetTextureImageDataAlias> data,
        std::optional<std::int32_t> format = {}, std::optional<bool> alphaFlag = {},
        std::optional<bool> cachedPixmap = {}, WidgetBrushAlias brushAlias = 23)
    {
        auto o = textureInputs(); WidgetBrushTextureImageObservation source;
        source.brushAlias = brushAlias; source.pixmapSelected = false;
        source.imagePaintingActiveWord = 0; source.imageDataAlias = data;
        source.cachedPixmapPresent = cachedPixmap;
        o.brushes = {source};
        if (data && *data) o.imageData = {{*data, {format, alphaFlag}}};
        runtime->observeTextureImageInputs(factory->widgetState(), o);
    }
    WidgetPaintDeviceCleanupObservation cleanupInputs() const
    {
        WidgetPaintDeviceCleanupObservation o; o.receiver = factory->widgetState();
        o.qualification = "Isolated current private constructor inputs";
        o.moduleSha256 = "c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887";
        o.pointerRva = 0x9cb858u;
        return o;
    }
    void bindActiveImage(std::optional<WidgetTextureImageDataAlias> data,
        std::optional<std::uint16_t> word = 1, std::optional<bool> cachedPixmap = false,
        WidgetBrushAlias alias = 23)
    {
        auto o = textureInputs(); WidgetBrushTextureImageObservation source;
        source.brushAlias = alias; source.pixmapSelected = false; source.imagePaintingActiveWord = word;
        source.imageDataAlias = data; source.cachedPixmapPresent = cachedPixmap; o.brushes = {source};
        if (data && *data) o.imageData = {{*data, {5, std::nullopt}}};
        runtime->observeTextureImageInputs(factory->widgetState(), o);
    }
    void cleanupCounter(std::optional<std::int32_t> value, std::optional<std::uint32_t> alias = 31)
    {
        auto o = cleanupInputs(); o.currentCounterAlias = alias;
        if (alias && *alias) o.counters = {{*alias, value}};
        runtime->observePaintDeviceCleanupInputs(factory->widgetState(), o);
    }
    void tailInputs()
    {
        const auto widget = factory->widgetState();
        WidgetConstructorTailObservation o;
        o.qualification = "Isolated actual parentless editor tail inputs";
        o.expectedFontAlias = 11; o.fontScreenField3c = 0;
        o.displayDcAlias = 1; o.displayLogicalDpiY = 168;
        o.currentQThreadIsApplicationQThread = true;
        o.metricInputs = {{widget, WidgetPaintDeviceKind::QWidget, 0, true}};
        o.currentWidgets = 760; o.maximumWidgets = 761; o.notifyDepth = 0;
        o.applicationCompileVersion = 0x40804; o.applicationAttributes = 0;
        runtime->observeConstructorTailValues(widget, o);
        WidgetReceiverEventObservation receiver;
        receiver.sameApplicationThread = true; receiver.objectFilterCount = 0;
        receiver.layoutPresent = false; receiver.gestureContextCount = 0;
        widget->observeEventReceiver(receiver, WidgetEventReceiverKind::QWidget,
            "Isolated actual editor Create receiver");
        WidgetApplicationEventObservation app;
        app.present = true; app.closing = false; app.internalCallbackCount = 0;
        app.filters = std::vector<WidgetApplicationEventObservation::Filter>{
            {WidgetApplicationEventObservation::Filter::Kind::WindowsVistaStyle, true}};
        app.gestureManagerPresent = true;
        runtime->observeApplicationEvents(app, "Isolated editor Create application population");
    }
    std::size_t effectCount(EditorBaseEffect effect) const
    {
        const auto& effects = factory->nativeBaseConstruction()->effects();
        return static_cast<std::size_t>(std::count(effects.begin(), effects.end(), effect));
    }
    std::size_t imageEffectCount(BrushTextureImageEffect effect) const
    {
        const auto* call = factory->nativeBaseConstruction()->textureImageCall();
        if (!call) return 0;
        return static_cast<std::size_t>(std::count(call->effects().begin(), call->effects().end(), effect));
    }
};
}

TEST_CASE(editor_base_rejects_foreign_binding_and_empty_evidence_before_private_allocation)
{
    Fixture f;
    auto c = f.context(); c.receiver = nullptr;
    CHECK(f.factory->continueNativeBase(f.binding(), c).issue == EditorBaseIssue::IdentityMismatch);
    c = f.context(); c.qualification.clear();
    CHECK(f.factory->continueNativeBase(f.binding(), c).issue == EditorBaseIssue::MissingQualification);
    CHECK(!f.factory->nativeBaseConstruction());
    CHECK(!f.factory->widgetState()->objectName());
    CHECK(!f.runtime->hasLiveRegistration(f.factory.get()));
}

TEST_CASE(editor_base_unknown_palette_retains_only_reached_private_prefix)
{
    Fixture f;
    const auto r = f.resume(); const auto w = f.factory->widgetState();
    CHECK(r.attempted); CHECK(r.stage == EditorBaseStage::PrivatePalette);
    CHECK(r.issue == EditorBaseIssue::PrivatePaletteUnavailable);
    REQUIRE(w->objectName()); CHECK(*w->objectName() == qt::String());
    CHECK(!w->threadState()); CHECK(w->runtimeOwner() == f.runtime);
    CHECK(w->dynamicBooleanProperties().extraDataAllocated == false);
    CHECK(w->receivesChildEvents() == true); CHECK(!w->childPrefixUnrecovered());
    CHECK(w->knownChildSuffix().empty()); CHECK(w->pendingChildren()->empty());
    CHECK(!w->palette()); CHECK(!w->font()); CHECK(!w->locale());
    CHECK(!w->extendedAttributes()); CHECK(!w->widgetDataAssigned());
    CHECK(!w->opaque()); CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
    CHECK(!f.runtime->hasLiveRegistration(f.factory.get()));
    CHECK(!r.qwidgetReturned); CHECK(!r.applicationWrappersReturned); CHECK(!r.completeForPublicSave);
}

TEST_CASE(editor_base_resumes_palette_font_locale_in_original_order_on_same_widget)
{
    Fixture f; const auto w = f.factory->widgetState();
    auto o = f.inputs(); o.paletteData = {{17}}; o.defaultPalette = WidgetPaletteValueObservation{17, 7};
    f.observe(o);
    CHECK(f.resume().issue == EditorBaseIssue::PrivateFontUnavailable);
    REQUIRE(w->palette()); const auto data = w->palette()->data;
    CHECK(w->palette()->groupResolveWord == 0u); CHECK(!w->font()); CHECK(!w->locale());
    o = f.inputs(); o.fontData = {{11, 168, 0}}; o.defaultFont = WidgetFontValueObservation{11, 4};
    f.observe(o);
    CHECK(f.resume().issue == EditorBaseIssue::PrivateLocaleUnavailable);
    REQUIRE(w->font()); CHECK(w->font()->resolveMask == 0u);
    CHECK(w->palette()->data == data); CHECK(!w->opaque()); CHECK(!w->threadState());
    o = f.inputs(); o.defaultLocale = WidgetLocaleValue{0x187, 0}; f.observe(o);
    CHECK(f.resume().issue == EditorBaseIssue::ApplicationPresenceUnavailable);
    REQUIRE(w->locale()); CHECK(w->locale()->index == 0x187);
    CHECK(w->opaque() == false); CHECK(w->opaqueChildrenDirty() == true);
    CHECK(!w->extendedAttributes()); CHECK(!w->threadState());
    CHECK(f.factory->widgetState() == w);
}

TEST_CASE(editor_base_missing_application_stops_before_extended_attributes_and_qobject_count)
{
    Fixture f; f.defaults(); f.appPresent(false);
    CHECK(f.resume().issue == EditorBaseIssue::ApplicationMissing);
    const auto effects = f.factory->nativeBaseConstruction()->effects().size();
    CHECK(f.resume().issue == EditorBaseIssue::ApplicationMissing);
    CHECK(f.factory->nativeBaseConstruction()->effects().size() == effects);
    CHECK(!f.factory->widgetState()->extendedAttributes());
    CHECK(!f.factory->widgetState()->threadState());
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
}

TEST_CASE(editor_base_foreign_thread_control_block_cannot_become_native_retaining_owner)
{
    Fixture f; f.defaults(); f.appPresent();
    const auto real = f.runtime->threadState();
    std::shared_ptr<WidgetThreadState> foreign(real.get(), [](WidgetThreadState*) {});
    auto c = f.context(); c.currentThread = foreign;
    const auto r = f.factory->continueNativeBase(f.binding(), c);
    CHECK(r.issue == EditorBaseIssue::CurrentThreadMismatch);
    CHECK(r.stage == EditorBaseStage::ThreadSelection);
    REQUIRE(f.factory->widgetState()->extendedAttributes());
    CHECK(!f.factory->widgetState()->threadState());
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 0u);
    CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushUnrecovered);
    CHECK(f.factory->widgetState()->threadState() == real);
    CHECK(f.factory->continueNativeBase(f.binding(), c).issue == EditorBaseIssue::CurrentThreadChanged);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
}

TEST_CASE(editor_base_unknown_gui_type_preserves_one_thread_and_qobject_increment)
{
    Fixture f; f.defaults(std::nullopt); f.appPresent();
    f.runtime->observeQObjectCount(1891, "Isolated pre-editor QObject count");
    CHECK(f.resume().issue == EditorBaseIssue::ApplicationTypeUnavailable);
    CHECK(f.runtime->qobjectCount() == 1892u);
    CHECK(!f.factory->widgetState()->widgetDataAssigned());
    CHECK(!f.runtime->hasLiveRegistration(f.factory.get()));
    const auto effects = f.factory->nativeBaseConstruction()->effects().size();
    CHECK(f.resume().issue == EditorBaseIssue::ApplicationTypeUnavailable);
    CHECK(f.runtime->qobjectCount() == 1892u);
    CHECK(f.factory->nativeBaseConstruction()->effects().size() == effects);
}

TEST_CASE(editor_base_non_gui_application_does_not_register_or_initialize_widget_data)
{
    Fixture f; f.defaults(0); f.appPresent();
    CHECK(f.resume().issue == EditorBaseIssue::NonGuiApplication);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
    CHECK(!f.runtime->hasLiveRegistration(f.factory.get()));
    CHECK(!f.factory->widgetState()->widgetDataAssigned());
}

TEST_CASE(editor_base_unknown_registry_stops_after_qobject_before_widget_data)
{
    Fixture f(std::nullopt); f.defaults(); f.appPresent();
    CHECK(f.resume().issue == EditorBaseIssue::RegistryPresenceUnavailable);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
    CHECK(!f.factory->widgetState()->widgetDataAssigned());
    CHECK(!f.factory->widgetState()->geometryEndpoints());
}

TEST_CASE(editor_base_absent_registry_skips_registration_but_keeps_parentless_data_and_focus)
{
    Fixture f(false); f.defaults(1, std::nullopt); f.appPresent();
    CHECK(f.resume().issue == EditorBaseIssue::DirectionUnavailable);
    const auto w = f.factory->widgetState();
    CHECK(!f.runtime->hasLiveRegistration(f.factory.get())); CHECK(w->widgetDataAssigned());
    CHECK(!w->parentWidget()); CHECK(!w->parentOwnsChild()); CHECK(w->knownChildSuffix().empty());
    const std::array<int, 4> geometry{0, 0, 639, 479};
    CHECK(w->geometryEndpoints() == geometry); CHECK(w->windowFlags() == 0x0800f001u);
    CHECK(w->attributeBit(76) == true); CHECK(w->attributeBit(16) == true);
    CHECK(w->attributeBit(34) == false); CHECK(w->attributeBit(35) == false);
    CHECK(w->focusNextKnown()); CHECK(w->focusPreviousKnown());
    CHECK(w->focusNextIdentity() == f.factory.get()); CHECK(w->focusPreviousIdentity() == f.factory.get());
}

TEST_CASE(editor_base_changed_direction_retains_bit_store_without_replaying_or_skipping_event_tail)
{
    Fixture f; f.defaults(1, 1); f.appPresent();
    auto r = f.resume(); CHECK(r.issue == EditorBaseIssue::DirectionChangeUnrecovered);
    CHECK(r.stage == EditorBaseStage::DirectionPropagation);
    CHECK(f.factory->widgetState()->attributeBit(56) == true);
    const auto effects = f.factory->nativeBaseConstruction()->effects().size();
    CHECK(f.resume().issue == EditorBaseIssue::DirectionChangeUnrecovered);
    CHECK(f.factory->nativeBaseConstruction()->effects().size() == effects);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
}

TEST_CASE(editor_base_unknown_modal_state_is_not_inferred_from_capture_palette_outcome)
{
    Fixture f; f.defaults(1, 0, std::nullopt); f.appPresent();
    CHECK(f.resume().issue == EditorBaseIssue::ModalStateUnavailable);
    CHECK(f.factory->nativeBaseConstruction()->report().stage == EditorBaseStage::PaletteGroup);
    CHECK(f.runtime->hasLiveRegistration(f.factory.get()));
    CHECK(f.factory->widgetState()->opaque() == false);
    CHECK(!f.factory->nativeBaseConstruction()->report().qwidgetReturned);
}

TEST_CASE(editor_base_known_modal_context_keeps_unrecovered_modal_palette_branch)
{
    Fixture f; f.defaults(1, 0, true); f.appPresent();
    CHECK(f.resume().issue == EditorBaseIssue::ModalPaletteUnrecovered);
    CHECK(f.factory->nativeBaseConstruction()->report().stage == EditorBaseStage::PaletteGroup);
    CHECK(f.factory->widgetState()->opaque() == false);
}

TEST_CASE(editor_base_current_palette_group_is_committed_before_unrecovered_brush_read)
{
    Fixture f; f.defaults(); f.appPresent();
    CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushUnrecovered);
    const auto w = f.factory->widgetState(); REQUIRE(w->palette());
    CHECK(w->palette()->data == f.runtime->privateConstructionInputs(*w)->defaultPalette->data);
    CHECK(w->palette()->groupResolveWord == 0u); CHECK(w->opaque() == false);
    CHECK(!f.factory->bindWidgetRuntime(f.binding(), f.runtime));
    EditorSetupState setup;
    CHECK(!f.factory->bindSetupOwner(setup));
    CHECK(f.factory->construction().setupBindIssue == Editor300SetupBindIssue::NativeBaseIncomplete);
    CHECK(!f.factory->setupWasBound());
}

TEST_CASE(editor_base_cannot_relabel_historical_setup_projection_as_native_fresh_constructor)
{
    Fixture f; EditorSetupState setup;
    REQUIRE(f.factory->bindSetupOwner(setup));
    CHECK(f.resume().issue == EditorBaseIssue::WrongStage);
    CHECK(!f.factory->nativeBaseConstruction());
    CHECK(!f.factory->widgetState()->objectName());
}

TEST_CASE(editor_base_destruction_invalidates_escaped_component_and_registration)
{
    Fixture f; f.defaults(); f.appPresent();
    CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushUnrecovered);
    const auto w = f.factory->widgetState(); const auto id = f.factory.get();
    REQUIRE(f.runtime->hasLiveRegistration(id));
    f.factory.reset();
    CHECK(!w->nativeOwnerAlive()); CHECK(!w->threadState());
    CHECK(!f.runtime->hasLiveRegistration(id));
    // This is adapter liveness invalidation, not the still-unrecovered native
    // destructor decrement/last-QObject cleanup branch.
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
}

TEST_CASE(editor_base_sparse_brush_requires_actual_current_palette_slot)
{
    Fixture f; f.defaults(); f.appPresent();
    auto o = f.inputs(); o.paletteData = {{18}};
    o.brushData = {{23, 1, 255}}; o.paletteBrushes = {{18, 0, 10, 23}}; f.observe(o);
    CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushUnrecovered);
    CHECK(f.factory->widgetState()->opaque() == false);
    CHECK(!f.factory->nativeBaseConstruction()->constructorTail());
    // The same interned brush may be referenced by the actual palette slot.
    f.brush(1, 255);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->widgetState()->opaque() == true);
}

TEST_CASE(editor_base_unknown_brush_fields_resume_without_repeating_palette_group)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(std::nullopt, std::nullopt);
    CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushUnrecovered);
    CHECK(f.effectCount(EditorBaseEffect::PaletteGroupAssigned) == 1u);
    f.brush(1, std::nullopt);
    CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushAlphaUnavailable);
    CHECK(f.factory->widgetState()->opaque() == false);
    CHECK(!f.factory->nativeBaseConstruction()->constructorTail());
    f.brush(1, 255);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->widgetState()->opaque() == true);
    CHECK(f.effectCount(EditorBaseEffect::PaletteGroupAssigned) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::OpacityAssigned) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 1u);
}

TEST_CASE(editor_base_solid_brush_compares_consumed_alpha_byte_exactly)
{
    for (std::uint8_t alpha : {std::uint8_t{254}, std::uint8_t{255}}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(1, alpha);
        CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
        CHECK(f.factory->widgetState()->opaque() == (alpha == 255u));
        CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
        CHECK(f.effectCount(EditorBaseEffect::OpacityAssigned) == (alpha == 255u ? 1u : 0u));
        CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == (alpha == 255u ? 1u : 0u));
        CHECK(!f.factory->nativeBaseConstruction()->report().qwidgetReturned);
    }
}

TEST_CASE(editor_base_no_brush_skips_alpha_read_and_opacity_helper)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(0, std::nullopt);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->widgetState()->opaque() == false);
    CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 0u);
    CHECK(f.effectCount(EditorBaseEffect::OpacityAssigned) == 0u);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 0u);
}

TEST_CASE(editor_base_unknown_texture_payload_does_not_infer_opacity_from_brush_alpha)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageSourceUnavailable);
    CHECK(f.factory->widgetState()->opaque() == false);
    CHECK(f.effectCount(EditorBaseEffect::PaletteGroupAssigned) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u);
    CHECK(!f.factory->nativeBaseConstruction()->constructorTail());
}

TEST_CASE(editor_base_simple_pattern_brushes_consume_alpha_but_never_become_opaque)
{
    for (std::int32_t style = 2; style <= 14; ++style) {
        for (std::uint8_t alpha : {std::uint8_t{0}, std::uint8_t{255}}) {
            Fixture f; f.defaults(); f.appPresent(); f.brush(style, alpha);
            CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
            CHECK(f.factory->widgetState()->opaque() == false);
            CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
            CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 1u);
            CHECK(f.effectCount(EditorBaseEffect::OpacityAssigned) == 0u);
            CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 0u);
            CHECK(!f.factory->nativeBaseConstruction()->report().qwidgetReturned);
        }
    }
}

TEST_CASE(editor_base_pattern_alpha_dependency_resumes_without_replaying_group_or_private_prefix)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(2, std::nullopt);
    CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushAlphaUnavailable);
    CHECK(f.factory->nativeBaseConstruction()->report().stage == EditorBaseStage::BrushOpacity);
    CHECK(!f.factory->nativeBaseConstruction()->constructorTail());
    CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u);
    CHECK(f.effectCount(EditorBaseEffect::PaletteGroupAssigned) == 1u);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
    f.brush(2, 255);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.effectCount(EditorBaseEffect::PaletteGroupAssigned) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 1u);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
}

TEST_CASE(editor_base_missing_texture_payload_retains_its_source_guard)
{
    for (std::int32_t style : {24}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(style, 255);
        CHECK(f.resume().issue == EditorBaseIssue::TextureImageSourceUnavailable);
        CHECK(!f.factory->nativeBaseConstruction()->constructorTail());
        CHECK(f.factory->widgetState()->opaque() == false);
        CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u);
    }
}

TEST_CASE(editor_base_texture_bound_image_still_waits_for_brush_alpha)
{
    for (std::uint8_t alpha : {std::uint8_t{0}, std::uint8_t{255}}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(24, std::nullopt);
        f.bindImage(0, {}, {}, false); f.cleanupCounter(0);
        CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushAlphaUnavailable);
        CHECK(!f.factory->nativeBaseConstruction()->textureImageCall());
        CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 0u);
        f.brush(24, alpha);
        CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
        CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
        CHECK(f.factory->widgetState()->opaque() == true);
        CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueCopied) == 1u);
        CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataRetained) == 0u);
        CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageFormatRead) == 0u);
        CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueDestroyed) == 1u);
        CHECK(!f.factory->nativeBaseConstruction()->gradientStopsCall());
        CHECK(!f.factory->nativeBaseConstruction()->report().completeForPublicSave);
    }
}

TEST_CASE(editor_base_texture_late_payload_and_null_image_dependencies_keep_one_call)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255); f.cleanupCounter(0);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageSourceUnavailable);
    auto o = f.textureInputs(); WidgetBrushTextureImageObservation source; source.brushAlias = 23;
    o.brushes = {source}; f.runtime->observeTextureImageInputs(f.factory->widgetState(), o);
    CHECK(f.factory->nativeBaseConstruction()->report().issue == EditorBaseIssue::TextureImageSourceUnavailable);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImagePending);
    const auto* call = f.factory->nativeBaseConstruction()->textureImageCall(); REQUIRE(call);
    CHECK(call->report().issue == BrushTextureImageIssue::SelectionUnavailable);
    source.pixmapSelected = false; o.brushes = {source};
    f.runtime->observeTextureImageInputs(f.factory->widgetState(), o);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImagePending);
    CHECK(call->report().issue == BrushTextureImageIssue::ImageDataUnavailable);
    source.imageDataAlias = 0; o.brushes = {source};
    f.runtime->observeTextureImageInputs(f.factory->widgetState(), o);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImagePending);
    CHECK(call->report().issue == BrushTextureImageIssue::PixmapCachePresenceUnavailable);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageNullnessRead) == 1u);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImagePending);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageNullnessRead) == 1u);
    source.cachedPixmapPresent = false; o.brushes = {source};
    f.runtime->observeTextureImageInputs(f.factory->widgetState(), o);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImagePending);
    CHECK(call->report().issue == BrushTextureImageIssue::PaintingActiveUnavailable);
    source.imagePaintingActiveWord = 0; o.brushes = {source};
    f.runtime->observeTextureImageInputs(f.factory->widgetState(), o);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->nativeBaseConstruction()->textureImageCall() == call);
    CHECK(call->report().stage == BrushTextureImageStage::Returned);
    CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueCopied) == 1u);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueDestroyed) == 1u);
}

TEST_CASE(editor_base_texture_metadata_refinement_holds_and_releases_the_same_image_value)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255); f.bindImage(31); f.cleanupCounter(0);
    const auto data = f.runtime->textureImageDataFor(*f.factory->widgetState(), 31); REQUIRE(data);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImagePending);
    const auto* call = f.factory->nativeBaseConstruction()->textureImageCall(); REQUIRE(call);
    CHECK(call->report().issue == BrushTextureImageIssue::ImageFormatUnavailable);
    CHECK(data->temporaryValueReferences() == 1u);
    const auto before = call->effects();
    f.bindImage(31, 7); CHECK(call->effects() == before);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImagePending);
    CHECK(call->report().issue == BrushTextureImageIssue::ImageColorTableAlphaUnavailable);
    CHECK(data->temporaryValueReferences() == 1u);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueCopied) == 1u);
    f.bindImage(31, 7, true);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->nativeBaseConstruction()->textureImageCall() == call);
    CHECK(data->temporaryValueReferences() == 0u); CHECK(f.factory->widgetState()->opaque() == true);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageNullnessRead) == 1u);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageColorTableAlphaRead) == 1u);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataReleased) == 1u);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueDestroyed) == 1u);
}

TEST_CASE(editor_base_texture_image_result_uses_metadata_instead_of_consumed_brush_alpha)
{
    struct Case { std::int32_t format; std::optional<bool> alpha; bool opaque; };
    for (const auto item : {Case{5, std::nullopt, false}, Case{3, true, false}, Case{3, false, true}}) {
        for (std::uint8_t alpha : {std::uint8_t{0}, std::uint8_t{255}}) {
            Fixture f; f.defaults(); f.appPresent(); f.brush(24, alpha); f.bindImage(31, item.format, item.alpha); f.cleanupCounter(0);
            const auto data = f.runtime->textureImageDataFor(*f.factory->widgetState(), 31); REQUIRE(data);
            CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
            CHECK(f.factory->widgetState()->opaque() == item.opaque);
            CHECK(data->temporaryValueReferences() == 0u);
            CHECK(f.effectCount(EditorBaseEffect::TextureImageOpacityResolved) == 1u);
            CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueCopied) == 1u);
            CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataReleased) == 1u);
            CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueDestroyed) == 1u);
            CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageColorTableAlphaRead) == (item.format == 5 ? 0u : 1u));
            const auto effects = f.factory->nativeBaseConstruction()->effects();
            CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
            CHECK(f.factory->nativeBaseConstruction()->effects() == effects);
            EditorSetupState setup; CHECK(!f.factory->bindSetupOwner(setup));
        }
    }
}

TEST_CASE(editor_base_texture_pixmap_selection_and_conversion_cache_paths_remain_guarded)
{
    Fixture pixmap; pixmap.defaults(); pixmap.appPresent(); pixmap.brush(24, 255);
    auto o = pixmap.textureInputs(); WidgetBrushTextureImageObservation source;
    source.brushAlias = 23; source.pixmapSelected = true; o.brushes = {source};
    pixmap.runtime->observeTextureImageInputs(pixmap.factory->widgetState(), o);
    CHECK(pixmap.resume().issue == EditorBaseIssue::TextureImagePending);
    const auto* call = pixmap.factory->nativeBaseConstruction()->textureImageCall(); REQUIRE(call);
    CHECK(call->report().issue == BrushTextureImageIssue::PixmapPathUnrecovered);
    CHECK(call->effects().empty()); CHECK(pixmap.resume().issue == EditorBaseIssue::TextureImagePending);
    CHECK(call->effects().empty()); CHECK(!pixmap.factory->nativeBaseConstruction()->constructorTail());
    Fixture conversion; conversion.defaults(); conversion.appPresent(); conversion.brush(24, 255);
    conversion.bindImage(0, {}, {}, true);
    CHECK(conversion.resume().issue == EditorBaseIssue::TextureImagePending);
    const auto* cached = conversion.factory->nativeBaseConstruction()->textureImageCall(); REQUIRE(cached);
    CHECK(cached->report().issue == BrushTextureImageIssue::ImageConversionUnrecovered);
    CHECK(conversion.imageEffectCount(BrushTextureImageEffect::ImageNullnessRead) == 1u);
    CHECK(conversion.imageEffectCount(BrushTextureImageEffect::ImageValueCopied) == 0u);
    CHECK(conversion.factory->widgetState()->opaque() == false);
}

TEST_CASE(editor_base_texture_active_painting_waits_for_actual_module_before_copy)
{
    for (std::uint32_t alias : {0u, 31u}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255);
        auto o = f.textureInputs(); WidgetBrushTextureImageObservation source;
        source.brushAlias = 23; source.pixmapSelected = false; source.imageDataAlias = alias;
        source.imagePaintingActiveWord = 1; source.cachedPixmapPresent = false; o.brushes = {source};
        if (alias) o.imageData = {{alias, {5, std::nullopt}}};
        f.runtime->observeTextureImageInputs(f.factory->widgetState(), o);
        CHECK(f.resume().issue == EditorBaseIssue::TextureImageActiveSourceUnavailable);
        const auto* call = f.factory->nativeBaseConstruction()->textureImageCall(); REQUIRE(call);
        CHECK(call->report().issue == BrushTextureImageIssue::ActiveImageCopyUnrecovered);
        CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueCopied) == 0u);
        CHECK(f.effectCount(EditorBaseEffect::TextureImageOpacityResolved) == 0u);
        if (alias) CHECK(f.runtime->textureImageDataFor(*f.factory->widgetState(), alias)->temporaryValueReferences() == 0u);
    }
}

TEST_CASE(editor_base_texture_image_is_destroyed_before_raw120_true_clear_flag_return)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageSourceUnavailable);
    f.factory->widgetState()->assignAttributeBit(120, true); f.bindImage(31, 7, false); f.cleanupCounter(0);
    const auto data = f.runtime->textureImageDataFor(*f.factory->widgetState(), 31); REQUIRE(data);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->nativeBaseConstruction()->nativeOpacityRaw120Read() == true);
    CHECK(f.factory->nativeBaseConstruction()->nativeOpacityFlag11Read() == false);
    const auto* call = f.factory->nativeBaseConstruction()->textureImageCall(); REQUIRE(call);
    CHECK(call->report().stage == BrushTextureImageStage::Returned);
    CHECK(data->temporaryValueReferences() == 0u);
    REQUIRE(!call->effects().empty()); CHECK(call->effects().back() == BrushTextureImageEffect::ImageValueDestroyed);
    const auto* tail = f.factory->nativeBaseConstruction()->textureImageCleanupCall(); REQUIRE(tail);
    CHECK(tail->report().stage == PaintDeviceCleanupStage::Returned);
    const auto cleanup = tail->effects();
    const auto effects = f.factory->nativeBaseConstruction()->effects(); const auto image = call->effects();
    const auto resolved = std::find(effects.begin(), effects.end(), EditorBaseEffect::TextureImageOpacityResolved);
    const auto palette = std::find(effects.begin(), effects.end(), EditorBaseEffect::PaletteOpacityResolved);
    const auto assigned = std::find(effects.begin(), effects.end(), EditorBaseEffect::OpacityAssigned);
    CHECK(resolved < palette); CHECK(palette < assigned);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->nativeBaseConstruction()->effects() == effects); CHECK(call->effects() == image); CHECK(tail->effects() == cleanup);
    f.factory->widgetState()->assignAttributeBit(120, false);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending); CHECK(call->effects() == image); CHECK(tail->effects() == cleanup);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityRaw120Read) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityFlag11Read) == 1u);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataReleased) == 1u);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueDestroyed) == 1u);
}

TEST_CASE(editor_base_texture_two_actual_widget_calls_hold_distinct_refs_to_one_image_owner)
{
    Fixture first; first.defaults(); first.appPresent(); first.brush(24, 255); first.bindImage(31); first.cleanupCounter(0);
    CHECK(first.resume().issue == EditorBaseIssue::TextureImagePending);
    const auto data = first.runtime->textureImageDataFor(*first.factory->widgetState(), 31); REQUIRE(data);
    CHECK(data->temporaryValueReferences() == 1u);
    Fixture second(true, first.runtime); second.defaults(); second.appPresent(); second.brush(24, 255); second.cleanupCounter(0);
    CHECK(second.resume().issue == EditorBaseIssue::TextureImagePending);
    CHECK(data->temporaryValueReferences() == 2u);
    CHECK(first.factory->nativeBaseConstruction()->textureImageCall() != second.factory->nativeBaseConstruction()->textureImageCall());
    first.bindImage(31, 5);
    CHECK(first.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(data->temporaryValueReferences() == 1u);
    CHECK(second.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(data->temporaryValueReferences() == 0u);
    CHECK(first.imageEffectCount(BrushTextureImageEffect::ImageValueCopied) == 1u);
    CHECK(second.imageEffectCount(BrushTextureImageEffect::ImageValueCopied) == 1u);
}

TEST_CASE(editor_base_texture_source_is_selected_from_actual_current_palette_brush)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255);
    auto p = f.inputs(); p.paletteData = {{18}}; p.brushData = {{24, 24, 255}};
    p.paletteBrushes = {{18, 0, 10, 24}}; f.observe(p); f.bindImage(31, 5, {}, {}, 24);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageSourceUnavailable);
    CHECK(!f.factory->nativeBaseConstruction()->textureImageCall());
    Fixture disabled; disabled.defaults(1, std::nullopt); disabled.appPresent();
    CHECK(disabled.resume().issue == EditorBaseIssue::DirectionUnavailable);
    disabled.factory->widgetState()->assignAttributeBit(0, true);
    p = disabled.inputs(); p.applicationLayoutDirection = 0;
    p.brushData = {{23, 24, 255}, {24, 24, 0}};
    p.paletteBrushes = {{17, 0, 10, 23}, {17, 1, 10, 24}}; disabled.observe(p);
    disabled.bindImage(0, {}, {}, false, 24); disabled.cleanupCounter(0);
    CHECK(disabled.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(disabled.factory->widgetState()->palette()->groupResolveWord == 1u);
    CHECK(disabled.factory->widgetState()->opaque() == true);
}

TEST_CASE(editor_base_texture_pending_adapter_destruction_drops_its_temporary_image_ref)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255); f.bindImage(31);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImagePending);
    const auto data = f.runtime->textureImageDataFor(*f.factory->widgetState(), 31); REQUIRE(data);
    CHECK(data->temporaryValueReferences() == 1u);
    f.factory.reset(); CHECK(data->temporaryValueReferences() == 0u);
    // Adapter call-frame cleanup is not a claim of full original Close parity.
}

TEST_CASE(editor_base_texture_cleanup_late_globals_and_value_never_replay_image_release)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255); f.bindImage(31, 5);
    const auto data = f.runtime->textureImageDataFor(*f.factory->widgetState(), 31); REQUIRE(data);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageCleanupGlobalsUnavailable);
    const auto* base = f.factory->nativeBaseConstruction(); const auto* image = base->textureImageCall(); REQUIRE(image);
    CHECK(base->report().stage == EditorBaseStage::TextureImageCleanup);
    CHECK(base->textureImageReturnedPaintingWord() == 0); CHECK(!base->textureImageCleanupCall());
    CHECK(data->temporaryValueReferences() == 0u);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataReleased) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::TextureImagePaintingWordRead) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::TextureImagePaintDeviceClassInstalled) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::TextureImageOpacityResolved) == 0u);
    CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u); CHECK(!base->constructorTail());
    const auto imageEffects = image->effects(); const auto baseEffects = base->effects();
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageCleanupGlobalsUnavailable);
    CHECK(image->effects() == imageEffects); CHECK(base->effects() == baseEffects);
    f.cleanupCounter(std::nullopt);
    CHECK(!base->textureImageCleanupCall()); CHECK(base->effects() == baseEffects);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageCleanupPending);
    const auto* cleanup = base->textureImageCleanupCall(); REQUIRE(cleanup);
    CHECK(cleanup->report().issue == PaintDeviceCleanupIssue::CounterValueUnavailable);
    CHECK(cleanup->report().stage == PaintDeviceCleanupStage::CounterComparison);
    const auto getterEffects = cleanup->effects(); REQUIRE(getterEffects.size() == 2u);
    CHECK(getterEffects[0] == PaintDeviceCleanupEffect::FirstGetterReturned);
    CHECK(getterEffects[1] == PaintDeviceCleanupEffect::SecondGetterReturned);
    f.cleanupCounter(0); CHECK(cleanup->effects() == getterEffects); CHECK(image->effects() == imageEffects);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(base->textureImageCleanupCall() == cleanup); CHECK(cleanup->report().stage == PaintDeviceCleanupStage::Returned);
    const std::vector expected{PaintDeviceCleanupEffect::FirstGetterReturned,
        PaintDeviceCleanupEffect::SecondGetterReturned, PaintDeviceCleanupEffect::CounterValueRead};
    CHECK(cleanup->effects() == expected); CHECK(image->effects() == imageEffects);
    CHECK(f.effectCount(EditorBaseEffect::TextureImageCleanupReturned) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::TextureImageOpacityResolved) == 1u);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataReleased) == 1u);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending); CHECK(cleanup->effects() == expected);
}

TEST_CASE(editor_base_null_texture_image_still_waits_for_unknown_or_null_cleanup_global)
{
    for (const auto pointer : {std::optional<std::uint32_t>{}, std::optional<std::uint32_t>{0}}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(24, 0); f.bindImage(0, {}, {}, false);
        f.cleanupCounter(std::nullopt, pointer);
        CHECK(f.resume().issue == EditorBaseIssue::TextureImageCleanupPending);
        const auto* base = f.factory->nativeBaseConstruction(); const auto* tail = base->textureImageCleanupCall(); REQUIRE(tail);
        CHECK(tail->report().issue == (pointer ? PaintDeviceCleanupIssue::LazyGlobalUnrecovered
                                             : PaintDeviceCleanupIssue::CounterPointerUnavailable));
        CHECK(tail->effects().empty()); CHECK(base->textureImageReturnedPaintingWord() == 0);
        CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueCopied) == 1u);
        CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataRetained) == 0u);
        CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataReleased) == 0u);
        CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageFormatRead) == 0u);
        CHECK(f.factory->widgetState()->opaque() == false); CHECK(!base->constructorTail());
        CHECK(f.effectCount(EditorBaseEffect::TextureImageOpacityResolved) == 0u);
        const auto effects = base->effects();
        CHECK(f.resume().issue == EditorBaseIssue::TextureImageCleanupPending); CHECK(base->effects() == effects);
    }
}

TEST_CASE(editor_base_texture_true_and_false_predicates_share_the_same_cleanup_boundary)
{
    for (bool opaque : {false, true}) {
        for (std::uint8_t alpha : {std::uint8_t{0}, std::uint8_t{255}}) {
            Fixture f; f.defaults(); f.appPresent(); f.brush(24, alpha);
            f.bindImage(31, opaque ? 7 : 5, opaque ? std::optional<bool>{false} : std::nullopt);
            CHECK(f.resume().issue == EditorBaseIssue::TextureImageCleanupGlobalsUnavailable);
            CHECK(f.factory->widgetState()->opaque() == false);
            CHECK(f.effectCount(EditorBaseEffect::TextureImageOpacityResolved) == 0u);
            CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u);
            f.cleanupCounter(0);
            CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
            CHECK(f.factory->widgetState()->opaque() == opaque);
            const auto& effects = f.factory->nativeBaseConstruction()->effects();
            const auto data = std::find(effects.begin(), effects.end(), EditorBaseEffect::TextureImageDataBoundaryReached);
            const auto word = std::find(effects.begin(), effects.end(), EditorBaseEffect::TextureImagePaintingWordRead);
            const auto baseClass = std::find(effects.begin(), effects.end(), EditorBaseEffect::TextureImagePaintDeviceClassInstalled);
            const auto cleanup = std::find(effects.begin(), effects.end(), EditorBaseEffect::TextureImageCleanupReturned);
            const auto result = std::find(effects.begin(), effects.end(), EditorBaseEffect::TextureImageOpacityResolved);
            const auto palette = std::find(effects.begin(), effects.end(), EditorBaseEffect::PaletteOpacityResolved);
            CHECK(data < word); CHECK(word < baseClass); CHECK(baseClass < cleanup);
            CHECK(cleanup < result); CHECK(result < palette);
            CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataReleased) == 1u);
            CHECK(!f.factory->nativeBaseConstruction()->report().applicationWrappersReturned);
            EditorSetupState setup; CHECK(!f.factory->bindSetupOwner(setup));
        }
    }
}

TEST_CASE(editor_base_texture_nonzero_cleanup_counter_is_independent_of_zero_image_refs)
{
    for (std::int32_t value : {-1, 1}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255); f.bindImage(0, {}, {}, false); f.cleanupCounter(value);
        CHECK(f.resume().issue == EditorBaseIssue::TextureImageCleanupPending);
        const auto* base = f.factory->nativeBaseConstruction(); const auto* tail = base->textureImageCleanupCall(); REQUIRE(tail);
        CHECK(tail->report().issue == PaintDeviceCleanupIssue::NonzeroCleanupUnrecovered);
        CHECK(tail->counterOwner()->value() == value); const auto effects = tail->effects();
        CHECK(f.resume().issue == EditorBaseIssue::TextureImageCleanupPending); CHECK(tail->effects() == effects);
        CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataRetained) == 0u);
        CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataReleased) == 0u);
        CHECK(f.effectCount(EditorBaseEffect::TextureImageCleanupReturned) == 0u);
        CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u);
        CHECK(f.factory->widgetState()->opaque() == false); CHECK(!base->constructorTail());
    }
}

TEST_CASE(editor_base_texture_shared_image_data_uses_separate_cleanup_frames_on_one_runtime_counter)
{
    Fixture first; first.defaults(); first.appPresent(); first.brush(24, 255); first.bindImage(31);
    CHECK(first.resume().issue == EditorBaseIssue::TextureImagePending);
    const auto image = first.runtime->textureImageDataFor(*first.factory->widgetState(), 31); REQUIRE(image);
    Fixture second(true, first.runtime); second.defaults(); second.appPresent(); second.brush(24, 255);
    CHECK(second.resume().issue == EditorBaseIssue::TextureImagePending); CHECK(image->temporaryValueReferences() == 2u);
    first.bindImage(31, 5);
    CHECK(first.resume().issue == EditorBaseIssue::TextureImageCleanupGlobalsUnavailable); CHECK(image->temporaryValueReferences() == 1u);
    CHECK(second.resume().issue == EditorBaseIssue::TextureImageCleanupGlobalsUnavailable); CHECK(image->temporaryValueReferences() == 0u);
    first.cleanupCounter(std::nullopt); second.cleanupCounter(std::nullopt);
    CHECK(first.resume().issue == EditorBaseIssue::TextureImageCleanupPending);
    CHECK(second.resume().issue == EditorBaseIssue::TextureImageCleanupPending);
    const auto* a = first.factory->nativeBaseConstruction()->textureImageCleanupCall();
    const auto* b = second.factory->nativeBaseConstruction()->textureImageCleanupCall(); REQUIRE(a); REQUIRE(b); CHECK(a != b);
    CHECK(a->counterOwner() == b->counterOwner()); CHECK(a->counterOwner()->observationAlias() == image->observationAlias());
    CHECK(static_cast<const void*>(a->counterOwner().get()) != static_cast<const void*>(image.get()));
    const auto prior = b->effects(); first.cleanupCounter(0);
    CHECK(first.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(b->report().stage == PaintDeviceCleanupStage::CounterComparison); CHECK(b->effects() == prior);
    CHECK(second.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(a->report().stage == PaintDeviceCleanupStage::Returned); CHECK(b->report().stage == PaintDeviceCleanupStage::Returned);
    CHECK(a->counterOwner()->value() == 0); CHECK(image->temporaryValueReferences() == 0u);
    CHECK(first.imageEffectCount(BrushTextureImageEffect::ImageDataReleased) == 1u);
    CHECK(second.imageEffectCount(BrushTextureImageEffect::ImageDataReleased) == 1u);
}

TEST_CASE(editor_base_texture_cleanup_owner_loss_or_rejected_rebinding_does_not_release_twice)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255); f.bindImage(31, 5); f.cleanupCounter(std::nullopt);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageCleanupPending);
    const auto* base = f.factory->nativeBaseConstruction(); const auto* image = base->textureImageCall();
    const auto* tail = base->textureImageCleanupCall(); REQUIRE(image); REQUIRE(tail);
    const auto data = f.runtime->textureImageDataFor(*f.factory->widgetState(), 31); REQUIRE(data);
    const auto globals = f.runtime->paintDeviceCleanupGlobals(*f.factory->widgetState()); REQUIRE(globals);
    auto o = f.cleanupInputs(); o.counters = {{32, 0}}; o.currentCounterAlias = 32;
    bool caught = false;
    try { f.runtime->observePaintDeviceCleanupInputs(f.factory->widgetState(), o); }
    catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught); CHECK(!globals->counterFor(32));
    const auto imageEffects = image->effects(); const auto cleanupEffects = tail->effects();
    globals->releaseNativeOwner();
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageCleanupPending);
    CHECK(tail->report().issue == PaintDeviceCleanupIssue::GlobalsUnavailable);
    CHECK(tail->effects() == cleanupEffects); CHECK(image->effects() == imageEffects);
    CHECK(data->temporaryValueReferences() == 0u); CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataReleased) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::TextureImageOpacityResolved) == 0u); CHECK(!base->constructorTail());
}

TEST_CASE(editor_base_texture_abandoning_after_data_release_does_not_retry_native_cleanup)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255); f.bindImage(31, 5); f.cleanupCounter(std::nullopt);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageCleanupPending);
    const auto data = f.runtime->textureImageDataFor(*f.factory->widgetState(), 31); REQUIRE(data);
    const auto globals = f.runtime->paintDeviceCleanupGlobals(*f.factory->widgetState()); REQUIRE(globals);
    const auto counter = globals->counterFor(31); REQUIRE(counter); CHECK(!counter->value());
    CHECK(data->temporaryValueReferences() == 0u);
    f.factory.reset(); CHECK(data->temporaryValueReferences() == 0u);
    CHECK(!counter->value()); CHECK(globals->nativeOwnerAlive());
    // Host abandonment is not a completed native destructor/Close receipt.
}

TEST_CASE(editor_base_radial_word_follows_brush_alpha_and_late_observation_does_not_advance)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(16, std::nullopt);
    f.radialWord(std::nullopt);
    CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushAlphaUnavailable);
    CHECK(f.effectCount(EditorBaseEffect::RadialFocalRadiusRead) == 0u);
    f.brush(16, 255);
    CHECK(f.resume().issue == EditorBaseIssue::RadialFocalRadiusUnavailable);
    CHECK(f.factory->nativeBaseConstruction()->report().stage == EditorBaseStage::RadialFocalRadius);
    CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
    const auto effects = f.factory->nativeBaseConstruction()->effects();
    f.radialWord(0x3f800000u);
    CHECK(f.factory->nativeBaseConstruction()->report().issue == EditorBaseIssue::RadialFocalRadiusUnavailable);
    CHECK(f.factory->nativeBaseConstruction()->effects() == effects);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::RadialFocalRadiusRead) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::RadialExtendedResolved) == 1u);
}

TEST_CASE(editor_base_radial_finite_large_magnitudes_mask_low_bits_and_ignore_brush_alpha)
{
    for (std::uint32_t base : {0x3f800000u, 0xbf800000u}) {
        for (std::uint32_t low = 0; low != 8; ++low) {
            for (std::uint8_t alpha : {std::uint8_t{0}, std::uint8_t{255}}) {
                Fixture f; f.defaults(); f.appPresent(); f.brush(16, alpha); f.radialWord(base | low);
                CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
                const auto brush = f.factory->widgetState()->palette()->data->brushAt(0, 10);
                CHECK(brush->radialFocalRadiusWord() == (base | low));
                CHECK(f.factory->widgetState()->opaque() == false);
                CHECK(f.effectCount(EditorBaseEffect::RadialFocalRadiusRead) == 1u);
                CHECK(f.effectCount(EditorBaseEffect::RadialExtendedResolved) == 1u);
                CHECK(f.effectCount(EditorBaseEffect::OpacityAssigned) == 0u);
                CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 0u);
                CHECK(!f.factory->nativeBaseConstruction()->gradientStopsCall());
                CHECK(!f.runtime->gradientSharedNull(*f.factory->widgetState()));
                const auto effects = f.factory->nativeBaseConstruction()->effects();
                CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
                CHECK(f.factory->nativeBaseConstruction()->effects() == effects);
                CHECK(!f.factory->nativeBaseConstruction()->report().completeForPublicSave);
            }
        }
    }
}

TEST_CASE(editor_base_radial_packed_float_threshold_is_strict_and_uses_double_epsilon)
{
    struct Case { std::uint32_t word; bool extended; };
    // Masked float magnitudes9.999995623233282e-13 and1.0000004296850662e-12.
    for (const auto item : {Case{0x2b8cbcc8u, false}, Case{0x2b8cbccfu, false},
             Case{0xab8cbcc8u, false}, Case{0xab8cbccfu, false},
             Case{0x2b8cbcd0u, true}, Case{0x2b8cbcd7u, true},
             Case{0xab8cbcd0u, true}, Case{0xab8cbcd7u, true}}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(16, 255); f.radialWord(item.word);
        const auto report = f.resume();
        CHECK(report.issue == (item.extended ? EditorBaseIssue::ConstructorTailPending
                                            : EditorBaseIssue::RadialGeometryUnrecovered));
        CHECK(f.effectCount(EditorBaseEffect::RadialFocalRadiusRead) == 1u);
        CHECK(f.effectCount(EditorBaseEffect::RadialExtendedResolved) == (item.extended ? 1u : 0u));
        CHECK(!f.factory->nativeBaseConstruction()->gradientStopsCall());
    }
}

TEST_CASE(editor_base_radial_zero_and_small_finite_words_retain_geometry_cursor)
{
    for (std::uint32_t word : {0u, 0x80000000u, 1u, 7u, 8u, 0x80000008u, 0x007fffffu, 0x10000000u}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(16, 255); f.radialWord(word);
        CHECK(f.resume().issue == EditorBaseIssue::RadialGeometryUnrecovered);
        CHECK(f.factory->nativeBaseConstruction()->report().stage == EditorBaseStage::RadialGeometry);
        const auto effects = f.factory->nativeBaseConstruction()->effects();
        CHECK(f.resume().issue == EditorBaseIssue::RadialGeometryUnrecovered);
        CHECK(f.factory->nativeBaseConstruction()->effects() == effects);
        CHECK(f.effectCount(EditorBaseEffect::RadialFocalRadiusRead) == 1u);
        CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u);
        CHECK(!f.factory->nativeBaseConstruction()->gradientStopsCall());
        CHECK(!f.factory->nativeBaseConstruction()->constructorTail());
    }
}

TEST_CASE(editor_base_radial_nonfinite_words_keep_floating_point_guard_without_replay)
{
    for (std::uint32_t word : {0x7f800000u, 0xff800000u, 0x7fc00000u, 0xffc00000u, 0x7f800001u, 0x7fa00008u}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(16, 255); f.radialWord(word);
        CHECK(f.resume().issue == EditorBaseIssue::RadialFloatingPointUnrecovered);
        CHECK(f.factory->nativeBaseConstruction()->report().stage == EditorBaseStage::RadialNonfinite);
        const auto effects = f.factory->nativeBaseConstruction()->effects();
        CHECK(f.resume().issue == EditorBaseIssue::RadialFloatingPointUnrecovered);
        CHECK(f.factory->nativeBaseConstruction()->effects() == effects);
        CHECK(f.effectCount(EditorBaseEffect::RadialFocalRadiusRead) == 1u);
        CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u);
        CHECK(!f.factory->nativeBaseConstruction()->gradientStopsCall());
    }
}

TEST_CASE(editor_base_radial_unchanged_nonopaque_bit_skips_native_helper_even_with_raw120)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(16, 255);
    CHECK(f.resume().issue == EditorBaseIssue::RadialFocalRadiusUnavailable);
    f.factory->widgetState()->assignAttributeBit(120, true); f.radialWord(0x3f800000u);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->widgetState()->opaque() == false);
    CHECK(f.effectCount(EditorBaseEffect::OpacityAssigned) == 0u);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 0u);
    CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 1u);
    CHECK(!f.factory->nativeBaseConstruction()->nativeOpacityRaw120Read());
    CHECK(!f.factory->nativeBaseConstruction()->nativeOpacityFlag11Read());
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityRaw120Read) == 0u);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityFlag11Read) == 0u);
}

TEST_CASE(editor_base_radial_observation_rejects_other_runtime_qualification_and_dead_owner)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(16, 255);
    CHECK(f.resume().issue == EditorBaseIssue::RadialFocalRadiusUnavailable);
    const auto brush = f.factory->widgetState()->palette()->data->brushAt(0, 10);
    const auto rejected = [&](const auto& widget, const WidgetRadialObservation& o) {
        bool caught = false;
        try { f.runtime->observeRadialInputs(widget, o); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
        CHECK(caught);
    };
    auto o = f.radialInputs(); o.brushes = {{23, 0x3f800000u}}; o.qualification.clear();
    rejected(f.factory->widgetState(), o);
    o = f.radialInputs(); o.brushes = {{23, 0x3f800000u}}; o.qualification = "Other stage";
    rejected(f.factory->widgetState(), o);
    Fixture other; other.defaults(); other.brush(16, 255);
    o = other.radialInputs(); o.brushes = {{23, 0x3f800000u}};
    rejected(other.factory->widgetState(), o);
    const auto widget = f.factory->widgetState();
    std::shared_ptr<WidgetState> foreign(widget.get(), [](WidgetState*) {});
    o = f.radialInputs(); o.receiver = foreign; o.brushes = {{23, 0x3f800000u}};
    rejected(foreign, o); CHECK(!brush->radialFocalRadiusWord());
    o = f.radialInputs(); o.brushes = {{23, 0x3f800000u}}; f.factory.reset();
    rejected(widget, o); CHECK(!brush->radialFocalRadiusWord());
}

TEST_CASE(editor_base_radial_observation_preflights_entire_batch_and_preserves_raw_bits)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(16, 255);
    auto p = f.inputs(); p.brushData = {{24, 1, 255}, {25, 16, 255}};
    p.paletteBrushes = {{17, 0, 9, 24}, {17, 1, 10, 25}}; f.observe(p);
    CHECK(f.resume().issue == EditorBaseIssue::RadialFocalRadiusUnavailable);
    const auto palette = f.factory->widgetState()->palette()->data;
    const auto actual = palette->brushAt(0, 10); const auto second = palette->brushAt(1, 10);
    const auto reject = [&](const WidgetRadialObservation& o) {
        bool caught = false;
        try { f.runtime->observeRadialInputs(f.factory->widgetState(), o); }
        catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
        CHECK(caught);
    };
    for (std::uint32_t badAlias : {0u, 24u, 99u}) {
        auto o = f.radialInputs(); o.brushes = {{23, 0x3f800000u}, {badAlias, 0x3f800000u}};
        reject(o); CHECK(!actual->radialFocalRadiusWord());
    }
    auto o = f.radialInputs(); o.brushes = {{23, 0x3f800000u}, {23, 0x3f800000u}};
    reject(o); CHECK(!actual->radialFocalRadiusWord());
    f.radialWord(0x3f800007u); CHECK(actual->radialFocalRadiusWord() == 0x3f800007u);
    o = f.radialInputs(); o.brushes = {{25, 0x3f800000u}, {23, 0x3f800000u}};
    reject(o); CHECK(!second->radialFocalRadiusWord());
    CHECK(actual->radialFocalRadiusWord() == 0x3f800007u);
    f.radialWord(0x3f800007u); CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
}

TEST_CASE(editor_base_radial_shared_brush_metadata_is_visible_to_actual_second_widget)
{
    Fixture first; first.defaults(); first.appPresent(); first.brush(16, 255); first.radialWord(0x3f800000u);
    CHECK(first.resume().issue == EditorBaseIssue::ConstructorTailPending);
    Fixture second(true, first.runtime); second.defaults(); second.appPresent(); second.brush(16, 255);
    CHECK(second.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(first.factory->widgetState()->palette()->data->brushAt(0, 10) ==
          second.factory->widgetState()->palette()->data->brushAt(0, 10));
    CHECK(second.effectCount(EditorBaseEffect::RadialExtendedResolved) == 1u);
    CHECK(!second.factory->nativeBaseConstruction()->gradientStopsCall());
}

TEST_CASE(editor_base_radial_cannot_borrow_another_brush_word_and_respects_current_group)
{
    Fixture f; f.defaults(1, std::nullopt); f.appPresent();
    CHECK(f.resume().issue == EditorBaseIssue::DirectionUnavailable);
    auto o = f.inputs(); o.applicationLayoutDirection = 0;
    o.brushData = {{23, 16, 255}, {24, 16, 255}};
    o.paletteBrushes = {{17, 0, 10, 23}, {17, 1, 10, 24}}; f.observe(o); f.radialWord(0x3f800000u, 24);
    CHECK(f.resume().issue == EditorBaseIssue::RadialFocalRadiusUnavailable);
    CHECK(f.effectCount(EditorBaseEffect::RadialFocalRadiusRead) == 0u);
    Fixture disabled; disabled.defaults(1, std::nullopt); disabled.appPresent();
    CHECK(disabled.resume().issue == EditorBaseIssue::DirectionUnavailable);
    disabled.factory->widgetState()->assignAttributeBit(0, true);
    o = disabled.inputs(); o.applicationLayoutDirection = 0;
    o.brushData = {{23, 16, 255}, {24, 16, 255}};
    o.paletteBrushes = {{17, 0, 10, 23}, {17, 1, 10, 24}}; disabled.observe(o); disabled.radialWord(0x3f800000u, 24);
    CHECK(disabled.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(disabled.factory->widgetState()->palette()->groupResolveWord == 1u);
    CHECK(disabled.effectCount(EditorBaseEffect::RadialExtendedResolved) == 1u);
}

TEST_CASE(editor_base_nonstandard_raw_styles_follow_original_constant_false_branch)
{
    for (std::int32_t style : {std::numeric_limits<std::int32_t>::min(), -1,
             18, 19, 20, 21, 22, 23, 25, std::numeric_limits<std::int32_t>::max()}) {
        for (std::uint8_t alpha : {std::uint8_t{0}, std::uint8_t{255}}) {
            Fixture f; f.defaults(); f.appPresent(); f.brush(style, std::nullopt);
            CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushAlphaUnavailable);
            CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 0u);
            f.brush(style, alpha);
            CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
            CHECK(f.factory->widgetState()->opaque() == false);
            CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
            CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 1u);
            CHECK(f.effectCount(EditorBaseEffect::OpacityAssigned) == 0u);
            CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 0u);
            CHECK(f.effectCount(EditorBaseEffect::PaletteGroupAssigned) == 1u);
            CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
            CHECK(!f.factory->nativeBaseConstruction()->report().qwidgetReturned);
        }
    }
}

TEST_CASE(editor_base_gradient_source_guard_follows_consumed_alpha_without_replay)
{
    for (std::int32_t style : {15, 17}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(style, std::nullopt);
        CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushAlphaUnavailable);
        CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 0u);
        f.brush(style, 255);
        CHECK(f.resume().issue == EditorBaseIssue::GradientSourceUnavailable);
        CHECK(f.factory->nativeBaseConstruction()->report().stage == EditorBaseStage::GradientStops);
        CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
        CHECK(!f.factory->nativeBaseConstruction()->gradientStopsCall());
        const auto effects = f.factory->nativeBaseConstruction()->effects();
        CHECK(f.resume().issue == EditorBaseIssue::GradientSourceUnavailable);
        CHECK(f.factory->nativeBaseConstruction()->effects() == effects);
        CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u);
        CHECK(!f.factory->nativeBaseConstruction()->constructorTail());
    }
}

TEST_CASE(editor_base_gradient_unknown_count_then_late_symbol_resumes_one_getter_frame)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(15, 255);
    f.gradientCount(std::nullopt, false);
    CHECK(f.resume().issue == EditorBaseIssue::GradientStopsPending);
    const auto* call = f.factory->nativeBaseConstruction()->gradientStopsCall(); REQUIRE(call);
    CHECK(call->report().issue == BrushGradientStopsIssue::SourceCountUnavailable);
    CHECK(call->effects().empty());
    CHECK(f.resume().issue == EditorBaseIssue::GradientStopsPending);
    CHECK(call->effects().empty());
    f.gradientCount(0, false);
    CHECK(f.resume().issue == EditorBaseIssue::GradientStopsPending);
    CHECK(call->report().issue == BrushGradientStopsIssue::SharedNullUnavailable);
    CHECK(call->report().stage == BrushGradientStopsStage::DefaultLocal);
    const auto reached = call->effects();
    CHECK(f.resume().issue == EditorBaseIssue::GradientStopsPending);
    CHECK(call->effects() == reached); CHECK(call->allocatedData().empty());
    f.qualifyGradientSymbol();
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->nativeBaseConstruction()->gradientStopsCall() == call);
    CHECK(call->report().stage == BrushGradientStopsStage::Released);
    CHECK(std::count(call->effects().begin(), call->effects().end(), BrushGradientStopsEffect::SourceCountRead) == 1);
    CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::GradientStopAlphaRead) == 2u);
    CHECK(f.effectCount(EditorBaseEffect::GradientReturnedValueDestroyed) == 1u);
}

TEST_CASE(editor_base_empty_gradient_uses_returned_stops_and_releases_before_opacity)
{
    for (std::int32_t style : {15, 17}) {
        for (std::uint8_t alpha : {std::uint8_t{0}, std::uint8_t{255}}) {
            Fixture f; f.defaults(); f.appPresent(); f.brush(style, alpha); f.gradientCount(0, true);
            const auto source = f.runtime->gradientStopDataFor(*f.factory->widgetState(), 31);
            CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
            const auto* base = f.factory->nativeBaseConstruction(); const auto* call = base->gradientStopsCall();
            REQUIRE(call); CHECK(call->report().stage == BrushGradientStopsStage::Released);
            CHECK(!call->returnedData()); REQUIRE(call->allocatedData().size() == 2u);
            CHECK(call->allocatedData()[0]->capacity() == 1); CHECK(call->allocatedData()[1]->capacity() == 3);
            for (const auto& data : call->allocatedData()) {
                CHECK(!data->nativeDataAlive()); CHECK(data->modeledValueReferences() == 0u);
            }
            CHECK(f.factory->widgetState()->opaque() == true);
            CHECK(f.effectCount(EditorBaseEffect::GradientStopAlphaRead) == 2u);
            CHECK(f.effectCount(EditorBaseEffect::GradientReturnedValueDestroyed) == 1u);
            CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 1u);
            const auto& effects = base->effects();
            const auto alphaRead = std::find(effects.begin(), effects.end(), EditorBaseEffect::BrushAlphaConsumed);
            const auto returned = std::find(effects.begin(), effects.end(), EditorBaseEffect::GradientStopsReturned);
            const auto stopRead = std::find(effects.begin(), effects.end(), EditorBaseEffect::GradientStopAlphaRead);
            const auto destroyed = std::find(effects.begin(), effects.end(), EditorBaseEffect::GradientReturnedValueDestroyed);
            const auto resolved = std::find(effects.begin(), effects.end(), EditorBaseEffect::PaletteOpacityResolved);
            const auto assigned = std::find(effects.begin(), effects.end(), EditorBaseEffect::OpacityAssigned);
            CHECK(alphaRead < returned); CHECK(returned < stopRead); CHECK(stopRead < destroyed);
            CHECK(destroyed < resolved); CHECK(resolved < assigned);
            REQUIRE(source); CHECK(source->count() == 0); CHECK(!source->capacity());
            CHECK(!source->externalReferences()); CHECK(!source->flagsLow2()); CHECK(!source->payload());
            CHECK(source->modeledValueReferences() == 0u);
            const auto retained = call->effects();
            CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending); CHECK(call->effects() == retained);
            CHECK(!base->report().qwidgetReturned); CHECK(!base->report().completeForPublicSave);
            EditorSetupState setup; CHECK(!f.factory->bindSetupOwner(setup));
        }
    }
}

TEST_CASE(editor_base_nonzero_gradient_source_preserves_getter_guard_and_earlier_alpha)
{
    for (std::int32_t n : {-1, 1}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(17, 255); f.gradientCount(n, true);
        CHECK(f.resume().issue == EditorBaseIssue::GradientStopsPending);
        const auto* call = f.factory->nativeBaseConstruction()->gradientStopsCall(); REQUIRE(call);
        CHECK(call->report().issue == BrushGradientStopsIssue::NonzeroSourceUnrecovered);
        const auto effects = call->effects();
        CHECK(f.resume().issue == EditorBaseIssue::GradientStopsPending); CHECK(call->effects() == effects);
        CHECK(call->allocatedData().empty()); CHECK(!call->returnedData());
        CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
        CHECK(f.effectCount(EditorBaseEffect::GradientStopsReturned) == 0u);
        CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u);
        CHECK(f.factory->widgetState()->opaque() == false);
    }
}

TEST_CASE(editor_base_gradient_source_can_be_the_same_qualified_shared_null_symbol)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(17, 0);
    auto o = f.gradientInputs(); o.sharedNull = f.symbolProfile();
    o.brushSources = {{23, WidgetGradientSourceKind::SharedNullSymbol, 0}};
    f.runtime->observeGradientInputs(f.factory->widgetState(), o);
    const auto symbol = f.runtime->gradientSharedNull(*f.factory->widgetState()); REQUIRE(symbol);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->widgetState()->palette()->data->brushAt(0, 10)->gradientStopsSource()->sourceData() == symbol);
    CHECK(symbol->modeledValueReferences() == 0u); CHECK(!symbol->externalReferences());
    CHECK(symbol->count() == 0); CHECK(symbol->nativeDataAlive());
    CHECK(f.factory->widgetState()->opaque() == true);
}

TEST_CASE(editor_base_gradient_value_is_destroyed_before_raw120_true_clear_flag_return)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(15, 255);
    CHECK(f.resume().issue == EditorBaseIssue::GradientSourceUnavailable);
    f.factory->widgetState()->assignAttributeBit(120, true); f.gradientCount(0, true);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    const auto* call = f.factory->nativeBaseConstruction()->gradientStopsCall(); REQUIRE(call);
    CHECK(call->report().stage == BrushGradientStopsStage::Released); CHECK(!call->returnedData());
    for (const auto& data : call->allocatedData()) CHECK(!data->nativeDataAlive());
    const auto effects = f.factory->nativeBaseConstruction()->effects(); const auto calls = call->effects();
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->nativeBaseConstruction()->effects() == effects); CHECK(call->effects() == calls);
    CHECK(f.factory->widgetState()->opaque() == true);
    CHECK(f.factory->nativeBaseConstruction()->nativeOpacityRaw120Read() == true);
    CHECK(f.factory->nativeBaseConstruction()->nativeOpacityFlag11Read() == false);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityFlag11Read) == 1u);
    f.factory->widgetState()->assignAttributeBit(120, false);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending); CHECK(call->effects() == calls);
    CHECK(f.effectCount(EditorBaseEffect::GradientStopAlphaRead) == 2u);
    CHECK(f.effectCount(EditorBaseEffect::GradientReturnedValueDestroyed) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 1u);
}

TEST_CASE(editor_base_gradient_binding_on_another_brush_cannot_supply_the_actual_palette_slot)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(15, 255);
    auto inputs = f.inputs(); inputs.paletteData = {{18}}; inputs.brushData = {{24, 15, 255}};
    inputs.paletteBrushes = {{18, 0, 10, 24}}; f.observe(inputs); f.gradientCount(0, true, 24);
    CHECK(f.resume().issue == EditorBaseIssue::GradientSourceUnavailable);
    CHECK(!f.factory->nativeBaseConstruction()->gradientStopsCall());
    CHECK(f.factory->widgetState()->opaque() == false);
}

TEST_CASE(editor_base_disabled_current_palette_group_selects_its_actual_gradient_source)
{
    Fixture f; f.defaults(1, std::nullopt); f.appPresent();
    CHECK(f.resume().issue == EditorBaseIssue::DirectionUnavailable);
    f.factory->widgetState()->assignAttributeBit(0, true);
    auto o = f.inputs(); o.applicationLayoutDirection = 0;
    o.brushData = {{23, 1, 0}, {24, 15, 255}};
    o.paletteBrushes = {{17, 0, 10, 23}, {17, 1, 10, 24}}; f.observe(o); f.gradientCount(0, true, 24);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->widgetState()->palette()->groupResolveWord == 1u);
    CHECK(f.factory->widgetState()->opaque() == true);
    CHECK(!f.factory->widgetState()->palette()->data->brushAt(0, 10)->gradientStopsSource());
    CHECK(f.effectCount(EditorBaseEffect::GradientStopAlphaRead) == 2u);
}

TEST_CASE(editor_base_texture_consumes_alpha_before_retaining_payload_guard)
{
    for (std::int32_t style : {24}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(style, std::nullopt);
        auto r = f.resume();
        CHECK(r.issue == EditorBaseIssue::PaletteBrushAlphaUnavailable);
        CHECK(r.stage == EditorBaseStage::BrushOpacity);
        CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 0u);
        CHECK(f.effectCount(EditorBaseEffect::PaletteGroupAssigned) == 1u);
        CHECK(!f.factory->nativeBaseConstruction()->constructorTail());
        f.brush(style, 255);
        r = f.resume();
        CHECK(r.issue == EditorBaseIssue::TextureImageSourceUnavailable);
        CHECK(r.stage == EditorBaseStage::TextureImage);
        CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
        const auto effects = f.factory->nativeBaseConstruction()->effects().size();
        CHECK(f.resume().issue == EditorBaseIssue::TextureImageSourceUnavailable);
        CHECK(f.factory->nativeBaseConstruction()->effects().size() == effects);
        CHECK(f.effectCount(EditorBaseEffect::PaletteGroupAssigned) == 1u);
        CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u);
        CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
        CHECK(f.factory->widgetState()->opaque() == false);
        CHECK(!f.factory->nativeBaseConstruction()->constructorTail());
        CHECK(!f.factory->nativeBaseConstruction()->report().qwidgetReturned);
    }
}

TEST_CASE(editor_base_palette_uses_resolved_current_group_instead_of_group_zero)
{
    Fixture f; f.defaults(1, std::nullopt); f.appPresent();
    CHECK(f.resume().issue == EditorBaseIssue::DirectionUnavailable);
    // Isolated changed live widget input: disabled selects group1. This is not
    // a claim that the fixed fresh original editor starts disabled.
    f.factory->widgetState()->assignAttributeBit(0, true);
    auto o = f.inputs(); o.applicationLayoutDirection = 0;
    o.brushData = {{23, 1, 255}, {24, 0, std::nullopt}};
    o.paletteBrushes = {{17, 0, 10, 23}, {17, 1, 10, 24}}; f.observe(o);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK((f.factory->widgetState()->palette()->groupResolveWord & 0xfu) == 1u);
    CHECK(f.factory->widgetState()->opaque() == false);
}

TEST_CASE(editor_base_native_opacity_raw120_true_clear_flag_returns_once)
{
    Fixture f; f.defaults(); f.appPresent();
    CHECK(f.resume().issue == EditorBaseIssue::PaletteBrushUnrecovered);
    // Inject an actual live native-attribute dependency before the brush read.
    // The fixed original constructor's zeroed extended word has raw120=false.
    f.factory->widgetState()->assignAttributeBit(120, true);
    f.brush(1, 255);
    auto r = f.resume();
    CHECK(r.issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(r.stage == EditorBaseStage::SharedTail);
    CHECK(f.factory->widgetState()->opaque() == true);
    REQUIRE(f.factory->nativeBaseConstruction()->constructorTail());
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.effectCount(EditorBaseEffect::OpacityAssigned) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 1u);
    f.factory->widgetState()->assignAttributeBit(120, false);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
    CHECK(f.factory->nativeBaseConstruction()->nativeOpacityRaw120Read() == true);
    CHECK(f.factory->nativeBaseConstruction()->nativeOpacityFlag11Read() == false);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityRaw120Read) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityFlag11Read) == 1u);
}

TEST_CASE(editor_base_shared_tail_retains_actual_create_and_counts_at_post_guard)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(1, 255);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    const auto* tail = f.factory->nativeBaseConstruction()->constructorTail(); REQUIRE(tail);
    CHECK(tail->report().stage == WidgetConstructorTailStage::FontDevice);
    CHECK(!tail->createEvent()); CHECK(!f.runtime->currentWidgetCount());
    f.tailInputs();
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(tail->report().stage == WidgetConstructorTailStage::PostPending);
    CHECK(f.runtime->currentWidgetCount() == 761u); CHECK(f.runtime->maximumWidgetCount() == 761u);
    CHECK(f.runtime->retainedQObjectConstructionDelta() == 1u);
    CHECK(f.runtime->threadState()->notifyDepth() == 0u);
    const auto* event = tail->createEvent(); REQUIRE(event);
    CHECK(event->rawType() == 0x0f && event->nativeAlive() && !event->posted());
    CHECK(f.factory->widgetState()->attributeBit(34) == true);
    CHECK(f.factory->widgetState()->attributeBit(35) == true);
    const auto count = f.factory->nativeBaseConstruction()->effects().size();
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->nativeBaseConstruction()->constructorTail() == tail);
    CHECK(tail->createEvent() == event && event->nativeAlive());
    CHECK(f.runtime->currentWidgetCount() == 761u);
    CHECK(f.factory->nativeBaseConstruction()->effects().size() == count);
    CHECK(!f.runtime->threadState()->postedEvents().populationKnown());
    CHECK(!f.factory->nativeBaseConstruction()->report().qwidgetReturned);
    EditorSetupState setup; CHECK(!f.factory->bindSetupOwner(setup));
    CHECK(f.factory->construction().setupBindIssue == Editor300SetupBindIssue::NativeBaseIncomplete);
}

TEST_CASE(editor_base_active_texture_finishes_actual_outer_before_opacity)
{
    for (std::uint8_t alpha : {std::uint8_t{0}, std::uint8_t{255}}) {
        for (std::uint16_t word : {std::uint16_t{1}, std::uint16_t{0x8000}, std::uint16_t{0xffff}}) {
            Fixture f; f.defaults(); f.appPresent(); f.brush(24, alpha); f.bindActiveImage(0, word);
            CHECK(f.resume().issue == EditorBaseIssue::TextureImageActiveSourceUnavailable);
            const auto* base = f.factory->nativeBaseConstruction(); REQUIRE(base);
            CHECK(!base->textureImageActiveCopyCall()); CHECK(!base->textureImageActiveFinishCall());
            const auto getterEffects = base->textureImageCall()->effects();
            f.cleanupCounter(0); CHECK(base->textureImageCall()->effects() == getterEffects);
            CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
            const auto* copy = base->textureImageActiveCopyCall(); const auto* finish = base->textureImageActiveFinishCall();
            REQUIRE(copy); REQUIRE(finish); CHECK(copy->report().stage == PaintDeviceActiveCopyStage::Returned);
            CHECK(finish->report().stage == PaintDeviceNullImageFinishStage::Returned); CHECK(finish->report().opaque == true);
            CHECK(!base->textureImageActiveReturnedValue()); auto prefix = finish->valuePrefixState(); REQUIRE(prefix);
            CHECK(prefix == copy->returnedImagePrefixState()); CHECK(prefix != copy->innerImagePrefixState());
            CHECK(!prefix->nativeOwnerAlive()); CHECK(prefix->paintingWord() == 0);
            CHECK(prefix->classIdentity() == PaintDevicePrefixClass::QPaintDevice);
            CHECK(finish->cleanupGlobalsOwner().lock() == f.runtime->paintDeviceCleanupGlobals(*f.factory->widgetState()));
            REQUIRE(finish->cleanupCall()); REQUIRE(copy->innerCleanupCall());
            CHECK(finish->cleanupCall() != copy->innerCleanupCall());
            CHECK(finish->cleanupCall()->counterOwner() == copy->innerCleanupCall()->counterOwner());
            CHECK(finish->cleanupCall()->effects().size() == 3u); CHECK(copy->innerCleanupCall()->effects().size() == 3u);
            const std::vector finishedEffects{PaintDeviceNullImageFinishEffect::PredicateImageDataRead,
                PaintDeviceNullImageFinishEffect::AlphaPredicateReturnedFalse, PaintDeviceNullImageFinishEffect::OpacityNegated,
                PaintDeviceNullImageFinishEffect::QImageClassInstalled, PaintDeviceNullImageFinishEffect::QImageDataRead,
                PaintDeviceNullImageFinishEffect::PaintDevicePrefixTailReached, PaintDeviceNullImageFinishEffect::CleanupReturned,
                PaintDeviceNullImageFinishEffect::OuterImageDestroyed, PaintDeviceNullImageFinishEffect::OpacityReturned};
            CHECK(finish->effects() == finishedEffects); CHECK(!base->textureImageCleanupCall()); CHECK(!base->textureImageReturnedPaintingWord());
            CHECK(base->textureImageCall()->effects() == getterEffects); CHECK(!base->textureImageCall()->report().opaque);
            CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
            CHECK(f.effectCount(EditorBaseEffect::TextureImageActiveSourceBound) == 1u);
            CHECK(f.effectCount(EditorBaseEffect::TextureImageActiveCopyReturned) == 1u);
            CHECK(f.effectCount(EditorBaseEffect::TextureImageActiveFinishReturned) == 1u);
            CHECK(f.effectCount(EditorBaseEffect::TextureImageDataBoundaryReached) == 0u);
            CHECK(f.effectCount(EditorBaseEffect::TextureImagePaintingWordRead) == 0u);
            CHECK(f.effectCount(EditorBaseEffect::TextureImagePaintDeviceClassInstalled) == 0u);
            CHECK(f.effectCount(EditorBaseEffect::TextureImageCleanupReturned) == 0u);
            CHECK(f.effectCount(EditorBaseEffect::TextureImageOpacityResolved) == 1u);
            CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 1u); CHECK(f.factory->widgetState()->opaque() == true);
            const auto effects = base->effects();
            const auto done = std::find(effects.begin(), effects.end(), EditorBaseEffect::TextureImageActiveFinishReturned);
            const auto resolved = std::find(effects.begin(), effects.end(), EditorBaseEffect::TextureImageOpacityResolved);
            const auto palette = std::find(effects.begin(), effects.end(), EditorBaseEffect::PaletteOpacityResolved);
            const auto assigned = std::find(effects.begin(), effects.end(), EditorBaseEffect::OpacityAssigned);
            CHECK(done < resolved); CHECK(resolved < palette); CHECK(palette < assigned);
            const auto copyEffects = copy->effects(); CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
            CHECK(base->effects() == effects); CHECK(copy->effects() == copyEffects); CHECK(finish->effects() == finishedEffects);
            REQUIRE(base->constructorTail()); CHECK(!base->report().qwidgetReturned); CHECK(!base->report().completeForPublicSave);
        }
    }
}

TEST_CASE(editor_base_active_texture_late_counter_preserves_producer_and_alpha_cursors)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255); f.bindActiveImage(0);
    f.runtime->observePaintDeviceCleanupInputs(f.factory->widgetState(), f.cleanupInputs());
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageActiveCopyPending);
    const auto* base = f.factory->nativeBaseConstruction(); const auto* copy = base->textureImageActiveCopyCall(); REQUIRE(copy);
    CHECK(copy->innerCleanupCall()->report().issue == PaintDeviceCleanupIssue::CounterPointerUnavailable);
    CHECK(!base->textureImageActiveFinishCall()); const auto getterEffects = base->textureImageCall()->effects(); const auto before = base->effects();
    f.cleanupCounter(std::nullopt); CHECK(base->effects() == before);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageActiveCopyPending);
    const auto* cleanup = copy->innerCleanupCall(); REQUIRE(cleanup);
    CHECK(cleanup->report().issue == PaintDeviceCleanupIssue::CounterValueUnavailable); CHECK(cleanup->effects().size() == 2u);
    f.cleanupCounter(0); CHECK(base->textureImageActiveCopyCall() == copy); CHECK(copy->innerCleanupCall() == cleanup);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending); CHECK(cleanup->effects().size() == 3u);
    const auto* finish = base->textureImageActiveFinishCall(); REQUIRE(finish); CHECK(finish->report().opaque == true);
    CHECK(base->textureImageCall()->effects() == getterEffects); CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::TextureImageActiveSourceBound) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::TextureImageActiveCopyReturned) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::TextureImageActiveFinishReturned) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::OpacityAssigned) == 1u); CHECK(!base->textureImageCleanupCall());
}

TEST_CASE(editor_base_active_nonnull_texture_keeps_pixel_copy_guard_after_native_prefix)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255); f.bindActiveImage(31); f.cleanupCounter(0);
    CHECK(f.resume().issue == EditorBaseIssue::TextureImageActiveCopyPending);
    const auto* base = f.factory->nativeBaseConstruction(); const auto* copy = base->textureImageActiveCopyCall(); REQUIRE(copy);
    CHECK(copy->report().issue == PaintDeviceActiveCopyIssue::NonNullActiveCopyUnrecovered);
    REQUIRE(copy->defaultRectangle()); CHECK(!copy->innerImagePrefixState()); CHECK(!copy->innerCleanupCall());
    CHECK(!base->textureImageActiveReturnedValue()); CHECK(!base->textureImageActiveFinishCall());
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageValueCopied) == 0u);
    CHECK(f.imageEffectCount(BrushTextureImageEffect::ImageDataRetained) == 0u);
    CHECK(f.runtime->textureImageDataFor(*f.factory->widgetState(), 31)->temporaryValueReferences() == 0u);
    const auto effects = copy->effects(); CHECK(f.resume().issue == EditorBaseIssue::TextureImageActiveCopyPending);
    CHECK(copy->effects() == effects); CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u);
}

TEST_CASE(editor_base_active_texture_uses_captured_current_palette_brush_not_other_alias)
{
    Fixture f; f.defaults(1, std::nullopt); f.appPresent(); CHECK(f.resume().issue == EditorBaseIssue::DirectionUnavailable);
    f.factory->widgetState()->assignAttributeBit(0, true);
    auto p = f.inputs(); p.applicationLayoutDirection = 0; p.brushData = {{23, 24, 255}, {24, 24, 0}};
    p.paletteBrushes = {{17, 0, 10, 23}, {17, 1, 10, 24}}; f.observe(p);
    f.bindImage(31, 5, {}, {}, 23); f.bindActiveImage(0, 0xffff, false, 24); f.cleanupCounter(0);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(f.factory->widgetState()->palette()->groupResolveWord == 1u);
    const auto view = f.runtime->bindTextureImageCopySource(f.factory->widgetState(), 24);
    const auto selected = f.factory->widgetState()->palette()->data->brushAt(1, 10)->textureImageSource();
    const auto decoy = f.factory->widgetState()->palette()->data->brushAt(0, 10)->textureImageSource();
    CHECK(view->textureImageSource() == selected); CHECK(view->textureImageSource() != decoy);
    CHECK(f.factory->nativeBaseConstruction()->textureImageActiveCopyCall()->report().readSourcePaintingWord == 0xffff);
    CHECK(f.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u); CHECK(f.factory->widgetState()->opaque() == true);
}

TEST_CASE(editor_base_active_texture_preserves_getter_cache_and_unknown_word_guards)
{
    Fixture unknown; unknown.defaults(); unknown.appPresent(); unknown.brush(24, 255);
    unknown.bindActiveImage(0, std::nullopt); unknown.cleanupCounter(0);
    CHECK(unknown.resume().issue == EditorBaseIssue::TextureImagePending);
    CHECK(unknown.factory->nativeBaseConstruction()->textureImageCall()->report().issue == BrushTextureImageIssue::PaintingActiveUnavailable);
    CHECK(!unknown.factory->nativeBaseConstruction()->textureImageActiveCopyCall());
    unknown.bindActiveImage(0, 1); CHECK(unknown.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(unknown.effectCount(EditorBaseEffect::BrushAlphaConsumed) == 1u);
    Fixture cache; cache.defaults(); cache.appPresent(); cache.brush(24, 255); cache.bindActiveImage(0, 1, true); cache.cleanupCounter(0);
    CHECK(cache.resume().issue == EditorBaseIssue::TextureImagePending);
    CHECK(cache.factory->nativeBaseConstruction()->textureImageCall()->report().issue == BrushTextureImageIssue::ImageConversionUnrecovered);
    CHECK(!cache.factory->nativeBaseConstruction()->textureImageActiveCopyCall());
    CHECK(cache.effectCount(EditorBaseEffect::TextureImageActiveSourceBound) == 0u);
}

TEST_CASE(editor_base_active_texture_completed_finish_does_not_replay_after_source_release)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255); f.bindActiveImage(0); f.cleanupCounter(0);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    const auto* base = f.factory->nativeBaseConstruction(); const auto* finish = base->textureImageActiveFinishCall(); REQUIRE(finish);
    auto prefix = finish->valuePrefixState(); REQUIRE(prefix); CHECK(!prefix->nativeOwnerAlive());
    auto view = f.runtime->bindTextureImageCopySource(f.factory->widgetState(), 23); view->releaseNativeOwner();
    const auto effects = base->effects(); const auto completed = finish->effects();
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(base->effects() == effects); CHECK(finish->effects() == completed); CHECK(finish->report().opaque == true);
    f.factory.reset(); CHECK(!prefix->nativeOwnerAlive());
}

TEST_CASE(editor_base_active_texture_shared_source_requires_current_receiver_module_grant)
{
    Fixture first; first.defaults(); first.appPresent(); first.brush(24, 255); first.bindActiveImage(0); first.cleanupCounter(0);
    CHECK(first.resume().issue == EditorBaseIssue::ConstructorTailPending);
    const auto* firstFinish = first.factory->nativeBaseConstruction()->textureImageActiveFinishCall(); REQUIRE(firstFinish);
    auto firstPrefix = firstFinish->valuePrefixState();
    Fixture second(true, first.runtime); second.defaults(); second.appPresent(); second.brush(24, 255);
    CHECK(second.resume().issue == EditorBaseIssue::TextureImageActiveSourceUnavailable);
    CHECK(!second.factory->nativeBaseConstruction()->textureImageActiveCopyCall()); second.cleanupCounter(0);
    CHECK(second.resume().issue == EditorBaseIssue::ConstructorTailPending);
    const auto* secondFinish = second.factory->nativeBaseConstruction()->textureImageActiveFinishCall(); REQUIRE(secondFinish);
    auto secondPrefix = secondFinish->valuePrefixState(); CHECK(firstPrefix != secondPrefix);
    CHECK(firstFinish != secondFinish); CHECK(firstFinish->cleanupCall() != secondFinish->cleanupCall());
    CHECK(firstFinish->cleanupCall()->counterOwner() == secondFinish->cleanupCall()->counterOwner());
    CHECK(first.runtime->bindTextureImageCopySource(first.factory->widgetState(), 23) ==
        second.runtime->bindTextureImageCopySource(second.factory->widgetState(), 23));
    first.factory.reset(); CHECK(!firstPrefix->nativeOwnerAlive()); CHECK(!secondPrefix->nativeOwnerAlive());
    CHECK(second.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(second.effectCount(EditorBaseEffect::TextureImageActiveCopyReturned) == 1u);
    CHECK(second.effectCount(EditorBaseEffect::TextureImageActiveFinishReturned) == 1u);
    CHECK(second.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 1u);
}

TEST_CASE(editor_base_active_texture_finish_precedes_raw120_true_clear_flag_return)
{
    Fixture f; f.defaults(1, std::nullopt); f.appPresent(); f.brush(24, 255);
    CHECK(f.resume().issue == EditorBaseIssue::DirectionUnavailable);
    f.factory->widgetState()->assignAttributeBit(120, true);
    auto p = f.inputs(); p.applicationLayoutDirection = 0; f.observe(p); f.bindActiveImage(0); f.cleanupCounter(0);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    const auto* base = f.factory->nativeBaseConstruction(); const auto* copy = base->textureImageActiveCopyCall();
    const auto* finish = base->textureImageActiveFinishCall(); REQUIRE(copy); REQUIRE(finish);
    CHECK(finish->report().stage == PaintDeviceNullImageFinishStage::Returned); CHECK(finish->report().opaque == true);
    CHECK(!finish->valuePrefixState()->nativeOwnerAlive()); CHECK(f.factory->widgetState()->opaque() == true);
    CHECK(base->report().stage == EditorBaseStage::SharedTail); REQUIRE(base->constructorTail());
    CHECK(f.effectCount(EditorBaseEffect::OpacityAssigned) == 1u); CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 1u);
    CHECK(base->nativeOpacityRaw120Read() == true); CHECK(base->nativeOpacityFlag11Read() == false);
    const auto effects = base->effects(); const auto producer = copy->effects(); const auto consumer = finish->effects();
    const auto outerCleanup = finish->cleanupCall()->effects(); const auto innerCleanup = copy->innerCleanupCall()->effects();
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending); CHECK(base->effects() == effects);
    CHECK(copy->effects() == producer); CHECK(finish->effects() == consumer);
    f.factory->widgetState()->assignAttributeBit(120, false);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    CHECK(copy->effects() == producer); CHECK(finish->effects() == consumer);
    CHECK(finish->cleanupCall()->effects() == outerCleanup); CHECK(copy->innerCleanupCall()->effects() == innerCleanup);
    CHECK(f.effectCount(EditorBaseEffect::TextureImageActiveFinishReturned) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::TextureImageOpacityResolved) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::OpacityAssigned) == 1u); CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityRaw120Read) == 1u); CHECK(f.effectCount(EditorBaseEffect::NativeOpacityFlag11Read) == 1u);
    const auto assigned = std::find(effects.begin(), effects.end(), EditorBaseEffect::OpacityAssigned);
    const auto raw = std::find(effects.begin(), effects.end(), EditorBaseEffect::NativeOpacityRaw120Read);
    const auto flag = std::find(effects.begin(), effects.end(), EditorBaseEffect::NativeOpacityFlag11Read);
    const auto returned = std::find(effects.begin(), effects.end(), EditorBaseEffect::NativeOpacityHelperReturned);
    CHECK(assigned < raw); CHECK(raw < flag); CHECK(flag < returned);
}

TEST_CASE(editor_base_active_texture_pending_origin_loss_never_hands_off_or_publishes)
{
    for (bool releaseModule : {false, true}) {
        Fixture f; f.defaults(); f.appPresent(); f.brush(24, 255); f.bindActiveImage(0); f.cleanupCounter(std::nullopt);
        CHECK(f.resume().issue == EditorBaseIssue::TextureImageActiveCopyPending);
        const auto* base = f.factory->nativeBaseConstruction(); const auto* copy = base->textureImageActiveCopyCall(); REQUIRE(copy);
        auto outer = copy->returnedImagePrefixState(), inner = copy->innerImagePrefixState(); REQUIRE(outer); REQUIRE(inner);
        CHECK(outer->nativeOwnerAlive()); CHECK(inner->nativeOwnerAlive());
        const auto effects = base->effects(); const auto producer = copy->effects();
        auto view = f.runtime->bindTextureImageCopySource(f.factory->widgetState(), 23);
        if (releaseModule) view->cleanupGlobalsOwner().lock()->releaseNativeOwner(); else view->releaseNativeOwner();
        CHECK(f.resume().issue == EditorBaseIssue::TextureImageActiveSourceUnavailable);
        CHECK(base->effects() == effects); CHECK(copy->effects() == producer);
        CHECK(!base->textureImageActiveFinishCall()); CHECK(!base->textureImageActiveReturnedValue());
        CHECK(f.effectCount(EditorBaseEffect::TextureImageOpacityResolved) == 0u);
        CHECK(f.effectCount(EditorBaseEffect::PaletteOpacityResolved) == 0u); CHECK(f.factory->widgetState()->opaque() == false);
        f.factory.reset(); CHECK(!outer->nativeOwnerAlive()); CHECK(!inner->nativeOwnerAlive());
    }
}

TEST_CASE(editor_base_native_opacity_raw120_false_skips_flag11_and_retains_read_receipt)
{
    Fixture f; f.defaults(); f.appPresent(); f.brush(1, 255);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending);
    const auto* base = f.factory->nativeBaseConstruction(); REQUIRE(base);
    REQUIRE(f.factory->widgetState()->windowFlags()); CHECK(*f.factory->widgetState()->windowFlags() == 0x0800f001u);
    CHECK(base->nativeOpacityRaw120Read() == false); CHECK(!base->nativeOpacityFlag11Read());
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityRaw120Read) == 1u);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityFlag11Read) == 0u);
    CHECK(f.effectCount(EditorBaseEffect::NativeOpacityHelperReturned) == 1u);
    const auto effects = base->effects(); f.factory->widgetState()->assignAttributeBit(120, true);
    CHECK(f.resume().issue == EditorBaseIssue::ConstructorTailPending); CHECK(base->effects() == effects);
    CHECK(base->nativeOpacityRaw120Read() == false); CHECK(!base->nativeOpacityFlag11Read());
    CHECK(f.factory->widgetState()->opaque() == true); CHECK(!base->report().qwidgetReturned);
    CHECK(!base->report().completeForPublicSave);
}

int main() { return testkit::run_all(); }
