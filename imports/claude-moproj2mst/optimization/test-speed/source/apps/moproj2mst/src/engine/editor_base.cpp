#include "m2m/engine/editor_base.hpp"
#include "m2m/engine/widget_events.hpp"

namespace m2m::engine::simcontrol300 {
namespace {
template<class T> bool sameOwner(const std::shared_ptr<T>& left,
    const std::shared_ptr<T>& right) noexcept
{
    return left && right && left.get() == right.get() &&
        !left.owner_before(right) && !right.owner_before(left);
}
}

EditorBaseConstruction::EditorBaseConstruction(const StagedEditor300* factory,
    const std::shared_ptr<WidgetState>& widget) : factory_(factory), widget_(widget) {}

EditorBaseReport EditorBaseConstruction::resume(const EditorBaseContext& context)
{
    if (context.receiver != factory_)
        return {report_.stage, EditorBaseIssue::IdentityMismatch};
    if (context.qualification.empty())
        return {report_.stage, EditorBaseIssue::MissingQualification};
    auto widget = widget_.lock();
    if (!widget || !widget->nativeOwnerAlive() || widget->nativeIdentity() != factory_)
        return {report_.stage, EditorBaseIssue::WidgetOwnerUnavailable};
    auto runtime = widget->runtimeOwner();
    if (!runtime) return {report_.stage, EditorBaseIssue::RuntimeOwnerUnavailable};
    const auto pending = [&](EditorBaseIssue issue) {
        report_.issue = issue;
        return report_;
    };
    if (report_.attempted && !sameOwner(runtime, runtime_.lock()))
        return pending(EditorBaseIssue::RuntimeOwnerChanged);
    if (auto thread = currentThread_.lock(); thread && !sameOwner(thread, context.currentThread))
        return pending(EditorBaseIssue::CurrentThreadChanged);
    if (!report_.attempted) {
        qualification_ = context.qualification;
        runtime_ = runtime;
        report_.attempted = true;
        effects_.push_back(EditorBaseEffect::PrivateAllocation);
        // QObjectPrivate67131160: the native thread field is null until the
        // later QObject ctor. Runtime affinity is already bound separately.
        widget->thread_.reset();
        widget->parent_.reset();
        widget->parentOwnsChild_ = false;
        widget->objectName_ = qt::String();
        widget->childSuffix_.clear();
        widget->childPrefixUnrecovered_ = false;
        widget->pendingChildren_.emplace();
        widget->events_.objectFilterCount = 0;
        widget->sendChildEvents_ = true;
        widget->receivesChildEvents_ = true;
        widget->observeEmptyDynamicBooleanProperties(false);
        effects_.push_back(EditorBaseEffect::QObjectPrivateConstructed);
        // QWidgetPrivate+88..e8 are cleared before the first palette/font
        // constructors. Later extended-attribute stores must not move here.
        widget->graphicsEffectPresent_ = false;
        widget->fieldD0Present_ = false;
        widget->installedLayout_.reset();
        widget->layoutAliasAssigned_ = false;
        widget->events_.layoutPresent = false;
        widget->autoFillBackground_ = false;
        widget->stylePresent_ = false;
        widget->stylesheetLength_ = 0;
        widget->proxyPresent_ = false;
        effects_.push_back(EditorBaseEffect::WidgetPrivatePrefix);
        report_.stage = EditorBaseStage::PrivatePalette;
    }
    const auto* inputs = runtime->privateConstructionInputs(*widget);
    if (report_.stage == EditorBaseStage::PrivatePalette) {
        if (!inputs || !inputs->defaultPalette)
            return pending(EditorBaseIssue::PrivatePaletteUnavailable);
        widget->palette_ = WidgetPaletteValue{inputs->defaultPalette->data, 0};
        effects_.push_back(EditorBaseEffect::PaletteConstructed);
        report_.stage = EditorBaseStage::PrivateFont;
    }
    if (report_.stage == EditorBaseStage::PrivateFont) {
        if (!inputs || !inputs->defaultFont)
            return pending(EditorBaseIssue::PrivateFontUnavailable);
        widget->font_ = WidgetFontValue{inputs->defaultFont->data, 0};
        effects_.push_back(EditorBaseEffect::FontConstructed);
        report_.stage = EditorBaseStage::PrivateLocale;
    }
    if (report_.stage == EditorBaseStage::PrivateLocale) {
        if (!inputs || !inputs->defaultLocale)
            return pending(EditorBaseIssue::PrivateLocaleUnavailable);
        widget->locale_ = inputs->defaultLocale;
        effects_.push_back(EditorBaseEffect::LocaleConstructed);
        // Source known bits at6505e94f..96c, not a zero-filled private block.
        widget->opaqueChildrenDirty_ = true;
        widget->opaque_ = false;
        effects_.push_back(EditorBaseEffect::WidgetPrivateTail);
        report_.stage = EditorBaseStage::PrivateApplication;
    }
    if (report_.stage == EditorBaseStage::PrivateApplication) {
        const auto present = runtime->applicationEvents().present;
        if (!present) return pending(EditorBaseIssue::ApplicationPresenceUnavailable);
        if (!*present) return pending(EditorBaseIssue::ApplicationMissing);
        // Native caller passes exactly40804. Source isWidget and extended
        // attribute stores follow its app/version checks at6505e9a5..bd.
        widget->extendedAttributes_ = std::array<std::uint32_t, 3>{{0, 0, 0}};
        effects_.push_back(EditorBaseEffect::ExtendedAttributesInitialized);
        report_.stage = EditorBaseStage::ThreadSelection;
    }
    if (report_.stage == EditorBaseStage::ThreadSelection) {
        if (!context.currentThread) return pending(EditorBaseIssue::CurrentThreadUnavailable);
        if (!sameOwner(context.currentThread, runtime->threadState()))
            return pending(EditorBaseIssue::CurrentThreadMismatch);
        widget->thread_ = context.currentThread;
        currentThread_ = context.currentThread;
        effects_.push_back(EditorBaseEffect::QObjectThreadRetained);
        report_.stage = EditorBaseStage::QObjectCounter;
    }
    if (report_.stage == EditorBaseStage::QObjectCounter) {
        runtime->commitQObjectConstruction(*widget);
        effects_.push_back(EditorBaseEffect::QObjectCountIncremented);
        // QPaintDevice and QWidget native class install occur after QObject
        // returns, before privateinit; no extra receiver object is allocated.
        effects_.push_back(EditorBaseEffect::PaintDeviceConstructed);
        widget->paintDeviceKind_ = WidgetPaintDeviceKind::QWidget;
        widget->eventReceiverKind_ = WidgetEventReceiverKind::QWidget;
        effects_.push_back(EditorBaseEffect::QWidgetClassInstalled);
        report_.stage = EditorBaseStage::InitApplication;
    }
    if (report_.stage == EditorBaseStage::InitApplication) {
        const auto type = runtime->applicationType();
        if (!type) return pending(EditorBaseIssue::ApplicationTypeUnavailable);
        if (*type == 0) return pending(EditorBaseIssue::NonGuiApplication);
        report_.stage = EditorBaseStage::InitRegistry;
    }
    if (report_.stage == EditorBaseStage::InitRegistry) {
        const auto registry = runtime->allWidgetsRegistryPresent();
        if (!registry) return pending(EditorBaseIssue::RegistryPresenceUnavailable);
        if (*registry) runtime->registerWidget(widget);
        effects_.push_back(EditorBaseEffect::RegistryStagePassed);
        report_.stage = EditorBaseStage::InitData;
    }
    if (report_.stage == EditorBaseStage::InitData) {
        widget->widgetDataAssigned_ = true;
        widget->windowFlags_ = 0;
        widget->attributesLow_ = 0; // explicit6506e56c store
        effects_.push_back(EditorBaseEffect::WidgetDataAssigned);
        // Fixed flags0 does not take raw100 path. raw76 stays set for initial
        // type0; source setters below have no reached callback for these bits.
        widget->assignAttributeBit(76, true);
        widget->assignAttributeBit(16, true);
        effects_.push_back(EditorBaseEffect::InitialAttributesAssigned);
        widget->geometry_ = std::array<int, 4>{{0, 0, 639, 479}};
        effects_.push_back(EditorBaseEffect::ParentlessGeometryAssigned);
        widget->focusNext_ = widget->focusPrevious_ = factory_;
        widget->focusNextKnown_ = widget->focusPreviousKnown_ = true;
        effects_.push_back(EditorBaseEffect::SelfFocusAssigned);
        widget->windowFlags_ = 0x0800f001u;
        effects_.push_back(EditorBaseEffect::WindowFlagsAdjusted);
        report_.stage = EditorBaseStage::Direction;
    }
    if (report_.stage == EditorBaseStage::Direction) {
        const auto direction = runtime->applicationLayoutDirection();
        if (!direction) return pending(EditorBaseIssue::DirectionUnavailable);
        const bool desired = *direction == 1;
        const auto current = widget->attributeBit(56);
        if (!current) return pending(EditorBaseIssue::DirectionUnavailable);
        if (desired != *current) {
            //6506a55d writes raw56 before child propagation and raw5a. Keep
            // the reached store. Attribute56 maps directly to setter return;
            // the outer helper's following event/children tail stays pending.
            widget->assignAttributeBit(56, desired);
            effects_.push_back(EditorBaseEffect::DirectionAttributeAssigned);
            report_.stage = EditorBaseStage::DirectionPropagation;
            return pending(EditorBaseIssue::DirectionChangeUnrecovered);
        }
        effects_.push_back(EditorBaseEffect::DirectionResolved);
        report_.stage = EditorBaseStage::PaletteGroup;
    }
    if (report_.stage == EditorBaseStage::DirectionPropagation)
        return pending(EditorBaseIssue::DirectionChangeUnrecovered);
    if (report_.stage == EditorBaseStage::PaletteGroup) {
        auto& group = widget->palette_->groupResolveWord;
        if (widget->attributeBit(0) == true) {
            group = (group & 0xfffffff1u) | 1u;
        } else {
            if (widget->attributeBit(15) == true)
                return pending(EditorBaseIssue::ActivePaletteUnrecovered);
            const auto modal = runtime->applicationModalState();
            if (!modal) return pending(EditorBaseIssue::ModalStateUnavailable);
            if (*modal) return pending(EditorBaseIssue::ModalPaletteUnrecovered);
            group &= 0xfffffff0u;
        }
        effects_.push_back(EditorBaseEffect::PaletteGroupAssigned);
        report_.stage = EditorBaseStage::BrushOpacity;
    }
    // The shared palette owner presently records identity/group, not the
    // source-consumed role10 brush/style/QColor. Keep that exact reached guard;
    // do not treat the observed opaque result as invented color storage.
    return pending(EditorBaseIssue::PaletteBrushUnrecovered);
}

} // namespace m2m::engine::simcontrol300
