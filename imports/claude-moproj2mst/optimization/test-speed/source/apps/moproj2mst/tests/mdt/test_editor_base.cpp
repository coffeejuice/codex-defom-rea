// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <array>
#include <memory>
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
    explicit Fixture(std::optional<bool> registry = true)
        : runtime(std::make_shared<WidgetRuntimeState>(registry, "Isolated actual editor base runtime"))
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

int main() { return testkit::run_all(); }
