#include "m2m/engine/view_manager.hpp"
#include "m2m/engine/widget_events.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/model.hpp"
#include <utility>
#include <map>
#include <set>

namespace m2m::engine {
namespace {
template<class T> void mergeObserved(std::optional<T>& current, const std::optional<T>& supplied)
{
    if (!supplied) return;
    if (current && *current != *supplied)
        throw Error(M2M_E_UNSUPPORTED, "Observation conflicts with existing live widget state");
    if (!current) current = supplied;
}
void requireLive(const WidgetState& widget)
{
    if (!widget.nativeOwnerAlive()) throw Error(M2M_E_UNSUPPORTED, "Native widget owner was released");
}
}

struct WidgetRuntimeState::Values {
    std::string qualification;
    std::map<std::uint32_t, std::shared_ptr<WidgetFontDataState>> fonts;
    std::map<std::uint32_t, std::shared_ptr<const WidgetPaletteDataState>> palettes;
    std::optional<WidgetFontValue> defaultFont;
    std::optional<WidgetPaletteValue> defaultPalette;
    std::optional<bool> fontMutexPresent, fontCachePresent, paletteCachePresent;
    std::optional<std::vector<std::pair<std::string, WidgetFontValue>>> fontClasses;
    std::optional<std::vector<std::pair<std::string, WidgetPaletteValue>>> paletteClasses;
    std::optional<WidgetLocaleValue> freshLocale;
    std::optional<bool> modalState;
    std::optional<std::int32_t> applicationType, layoutDirection;
    struct PrivateInputs { std::weak_ptr<WidgetState> widget; WidgetPrivateConstructionInputs inputs; };
    std::map<const WidgetState*, PrivateInputs> privateInputs;
    std::optional<std::int32_t> applicationStylesheetLength;
    WidgetReceiverEventObservation freshWidgetEvents;
    std::optional<bool> freshSendChildEvents, freshFieldD0Present;
};

std::optional<WidgetFontValue> WidgetRuntimeState::applicationFont() const
{ return values_ ? values_->defaultFont : std::optional<WidgetFontValue>{}; }
std::optional<WidgetPaletteValue> WidgetRuntimeState::applicationPalette() const
{ return values_ ? values_->defaultPalette : std::optional<WidgetPaletteValue>{}; }

void WidgetRuntimeState::observeStartupValues(const WidgetRuntimeValueObservation& o)
{
    auto main = o.mainView.lock();
    auto manager = o.manager.lock();
    auto self = weak_from_this().lock();
    if (!self || !main || !manager || !main->nativeOwnerAlive() || !manager->nativeReceiverAlive() ||
        main->managerChild() != manager || manager->parentMainView() != main ||
        main->widgetState()->runtime_.lock() != self || manager->widgetState()->runtime_.lock() != self ||
        manager->pendingSplitView() || o.qualification.empty() || values_)
        throw Error(M2M_E_UNSUPPORTED, "Startup values require one initial observation of the same live widget runtime/owners");
    auto v = std::make_shared<Values>();
    v->qualification = o.qualification;
    for (const auto& f : o.fontData) {
        if (!f.alias || v->fonts.count(f.alias))
            throw Error(M2M_E_UNSUPPORTED, "Font data observation aliases must be positive and unique");
        v->fonts.emplace(f.alias, std::shared_ptr<WidgetFontDataState>(new WidgetFontDataState(f.alias, f.comparisonField38, f.deviceScreenField3c)));
    }
    for (const auto& p : o.paletteData) {
        if (!p.alias || v->palettes.count(p.alias))
            throw Error(M2M_E_UNSUPPORTED, "Palette data observation aliases must be positive and unique");
        v->palettes.emplace(p.alias, std::shared_ptr<const WidgetPaletteDataState>(new WidgetPaletteDataState(p.alias)));
    }
    auto font = [&](const WidgetFontValueObservation& f) {
        auto it = v->fonts.find(f.dataAlias);
        if (it == v->fonts.end()) throw Error(M2M_E_UNSUPPORTED, "Referenced font data alias is absent");
        return WidgetFontValue{it->second, f.resolveMask};
    };
    auto palette = [&](const WidgetPaletteValueObservation& p) {
        auto it = v->palettes.find(p.dataAlias);
        if (it == v->palettes.end()) throw Error(M2M_E_UNSUPPORTED, "Referenced palette data alias is absent");
        return WidgetPaletteValue{it->second, p.groupResolveWord};
    };
    if (o.applicationFont.defaultValue) v->defaultFont = font(*o.applicationFont.defaultValue);
    if (o.applicationPalette.defaultValue) v->defaultPalette = palette(*o.applicationPalette.defaultValue);
    v->fontMutexPresent = o.applicationFont.mutexPresent;
    v->fontCachePresent = o.applicationFont.classCachePresent;
    v->paletteCachePresent = o.applicationPalette.classCachePresent;
    if ((o.applicationFont.classes && v->fontCachePresent != true) ||
        (o.applicationPalette.classes && v->paletteCachePresent != true))
        throw Error(M2M_E_UNSUPPORTED, "Cache population requires a qualified present cache owner");
    if (o.applicationFont.classes) {
        v->fontClasses.emplace(); std::set<std::string> keys;
        for (const auto& e : *o.applicationFont.classes) {
            if (!keys.insert(e.classKeyBytes).second) throw Error(M2M_E_UNSUPPORTED, "Duplicate font class key");
            v->fontClasses->emplace_back(e.classKeyBytes, font(e.value));
        }
    }
    if (o.applicationPalette.classes) {
        v->paletteClasses.emplace(); std::set<std::string> keys;
        for (const auto& e : *o.applicationPalette.classes) {
            if (!keys.insert(e.classKeyBytes).second) throw Error(M2M_E_UNSUPPORTED, "Duplicate palette class key");
            v->paletteClasses->emplace_back(e.classKeyBytes, palette(e.value));
        }
    }
    struct Prepared {
        std::optional<WidgetFontValue> font;
        std::optional<WidgetPaletteValue> palette;
        std::optional<std::vector<WidgetPendingChild>> pending;
    };
    auto prepare = [&](const WidgetInitialValueObservation& w) {
        Prepared r;
        if (w.font) r.font = font(*w.font);
        if (w.palette) r.palette = palette(*w.palette);
        if (w.stylesheetLength && *w.stylesheetLength < 0)
            throw Error(M2M_E_UNSUPPORTED, "Invalid stylesheet length observation");
        if (w.pendingChildren) {
            r.pending.emplace(); std::set<std::uint32_t> aliases;
            for (const auto& entry : *w.pendingChildren) {
                auto known = entry.knownWidget.lock();
                if (entry.opaqueAlias) {
                    if (known || !aliases.insert(entry.opaqueAlias).second)
                        throw Error(M2M_E_UNSUPPORTED, "Pending native identities must be distinct from actual widget links");
                } else if (!known || !known->nativeOwnerAlive() || known->runtime_.lock() != self) {
                    throw Error(M2M_E_UNSUPPORTED, "Pending known widget must belong to this live runtime");
                }
                r.pending->push_back({entry.opaqueAlias, entry.knownWidget});
            }
        }
        return r;
    };
    auto m = prepare(o.mainViewValues), g = prepare(o.managerValues);
    v->freshLocale = o.freshPrivateLocale;
    v->modalState = o.applicationModalState;
    v->applicationStylesheetLength = o.applicationStylesheetLength;
    if (v->applicationStylesheetLength && *v->applicationStylesheetLength < 0)
        throw Error(M2M_E_UNSUPPORTED, "Invalid application stylesheet length observation");
    v->freshWidgetEvents = o.freshWidgetEvents;
    v->freshSendChildEvents = o.freshSendChildEvents; v->freshFieldD0Present = o.freshFieldD0Present;
    auto commit = [](WidgetState& w, const WidgetInitialValueObservation& a, Prepared&& p) {
        w.font_ = std::move(p.font); w.palette_ = std::move(p.palette); w.locale_ = a.locale;
        w.inheritedFontMask_ = a.inheritedFontMask; w.inheritedPaletteMask_ = a.inheritedPaletteMask;
        if (a.attributesLow) w.attributesLow_ = a.attributesLow;
        if (a.extendedAttributes) w.extendedAttributes_ = a.extendedAttributes;
        if (a.windowFlags) w.windowFlags_ = a.windowFlags;
        w.opaqueChildrenDirty_ = a.opaqueChildrenDirty; w.opaque_ = a.opaque;
        w.graphicsEffectPresent_ = a.graphicsEffectPresent; w.autoFillBackground_ = a.autoFillBackground;
        w.stylePresent_ = a.stylePresent; w.stylesheetLength_ = a.stylesheetLength;
        w.proxyPresent_ = a.proxyPresent; w.events_ = a.events; w.pendingChildren_ = std::move(p.pending);
    };
    // All owner/alias/population checks finish before any existing widget changes.
    observeApplicationEvents(o.applicationEvents, o.qualification);
    commit(*main->widgetState(), o.mainViewValues, std::move(m));
    commit(*manager->widgetState(), o.managerValues, std::move(g));
    values_ = std::move(v);
}

std::optional<std::int32_t> WidgetRuntimeState::applicationType() const noexcept
{ return values_ ? values_->applicationType : std::optional<std::int32_t>{}; }
std::optional<std::int32_t> WidgetRuntimeState::applicationLayoutDirection() const noexcept
{ return values_ ? values_->layoutDirection : std::optional<std::int32_t>{}; }
std::optional<bool> WidgetRuntimeState::applicationModalState() const noexcept
{ return values_ ? values_->modalState : std::optional<bool>{}; }

void WidgetRuntimeState::observePrivateConstructionInputs(const std::shared_ptr<WidgetState>& widget,
    const WidgetPrivateConstructionObservation& o)
{
    if (!widget || !widget->nativeOwnerAlive() || !widget->matchesComponentOwner(widget) ||
        widget->runtime_.lock().get() != this || o.qualification.empty())
        throw Error(M2M_E_UNSUPPORTED, "Private constructor inputs require this actual live runtime/widget");
    if (!values_) values_ = std::make_shared<Values>();
    auto& v = *values_;
    auto it = v.privateInputs.find(widget.get());
    if (it != v.privateInputs.end() && (it->second.widget.lock() != widget ||
        it->second.inputs.qualification != o.qualification))
        throw Error(M2M_E_UNSUPPORTED, "Private constructor inputs cannot replace an actual receiver or qualification");
    std::set<std::uint32_t> seen;
    for (const auto& f : o.fontData) {
        if (!f.alias || !seen.insert(f.alias).second)
            throw Error(M2M_E_UNSUPPORTED, "Private font aliases must be positive and unique");
        auto [pos, fresh] = v.fonts.emplace(f.alias, nullptr);
        if (fresh) pos->second = std::shared_ptr<WidgetFontDataState>(new WidgetFontDataState(f.alias, {}, {}));
        mergeObserved(pos->second->field38_, f.comparisonField38);
        mergeObserved(pos->second->field3c_, f.deviceScreenField3c);
    }
    seen.clear();
    for (const auto& p : o.paletteData) {
        if (!p.alias || !seen.insert(p.alias).second)
            throw Error(M2M_E_UNSUPPORTED, "Private palette aliases must be positive and unique");
        if (!v.palettes.count(p.alias))
            v.palettes.emplace(p.alias, std::shared_ptr<const WidgetPaletteDataState>(new WidgetPaletteDataState(p.alias)));
    }
    auto& record = v.privateInputs[widget.get()];
    record.widget = widget; auto& inputs = record.inputs;
    inputs.qualification = o.qualification;
    if (o.defaultFont) {
        auto found = v.fonts.find(o.defaultFont->dataAlias);
        if (found == v.fonts.end()) throw Error(M2M_E_UNSUPPORTED, "Private font references an absent data owner");
        WidgetFontValue next{found->second, o.defaultFont->resolveMask};
        if ((inputs.defaultFont && (inputs.defaultFont->data != next.data || inputs.defaultFont->resolveMask != next.resolveMask)) ||
            (v.defaultFont && v.defaultFont->data != next.data))
            throw Error(M2M_E_UNSUPPORTED, "Private font observation conflicts with the current shared default owner");
        inputs.defaultFont = next;
    }
    if (o.defaultPalette) {
        auto found = v.palettes.find(o.defaultPalette->dataAlias);
        if (found == v.palettes.end()) throw Error(M2M_E_UNSUPPORTED, "Private palette references an absent data owner");
        WidgetPaletteValue next{found->second, o.defaultPalette->groupResolveWord};
        if ((inputs.defaultPalette && (inputs.defaultPalette->data != next.data || inputs.defaultPalette->groupResolveWord != next.groupResolveWord)) ||
            (v.defaultPalette && v.defaultPalette->data != next.data))
            throw Error(M2M_E_UNSUPPORTED, "Private palette observation conflicts with the current shared default owner");
        inputs.defaultPalette = next;
    }
    if (o.defaultLocale) {
        if (inputs.defaultLocale && (inputs.defaultLocale->index != o.defaultLocale->index ||
            inputs.defaultLocale->numberOptions != o.defaultLocale->numberOptions))
            throw Error(M2M_E_UNSUPPORTED, "Private locale observation conflicts with existing constructor inputs");
        inputs.defaultLocale = o.defaultLocale;
    }
    mergeObserved(v.applicationType, o.applicationType);
    mergeObserved(v.layoutDirection, o.applicationLayoutDirection);
    mergeObserved(v.modalState, o.applicationModalState);
}

const WidgetPrivateConstructionInputs* WidgetRuntimeState::privateConstructionInputs(const WidgetState& widget) const noexcept
{
    if (!values_ || !widget.nativeOwnerAlive() || widget.runtime_.lock().get() != this) return nullptr;
    auto it = values_->privateInputs.find(&widget);
    return it != values_->privateInputs.end() && it->second.widget.lock().get() == &widget ? &it->second.inputs : nullptr;
}

void WidgetRuntimeState::observeConstructorTailValues(const std::shared_ptr<WidgetState>& widget,
    const WidgetConstructorTailObservation& o)
{
    if (!widget || !widget->nativeOwnerAlive() || !widget->matchesComponentOwner(widget) || widget->runtime_.lock().get() != this ||
        o.qualification.empty() || !values_ || !widget->font_ || !widget->font_->data ||
        widget->font_->data->observationAlias() != o.expectedFontAlias)
        throw Error(M2M_E_UNSUPPORTED, "Constructor tail values require the actual live widget and its existing font owner");
    auto found = values_->fonts.find(o.expectedFontAlias);
    if (found == values_->fonts.end() || found->second != widget->font_->data)
        throw Error(M2M_E_UNSUPPORTED, "Constructor font owner is not interned in this runtime");
    for (const auto& input : o.metricInputs) {
        auto target = input.widget.lock();
        if (!target || !target->nativeOwnerAlive() || !target->matchesComponentOwner(target) || target->runtime_.lock().get() != this)
            throw Error(M2M_E_UNSUPPORTED, "Metric inputs must identify actual live widgets in this runtime");
        if (target->paintDeviceKind_ != WidgetPaintDeviceKind::Unknown && input.kind != WidgetPaintDeviceKind::Unknown &&
            target->paintDeviceKind_ != input.kind)
            throw Error(M2M_E_UNSUPPORTED, "Metric virtual destination conflicts with its current binding");
        if (input.kind != WidgetPaintDeviceKind::Unknown) target->paintDeviceKind_ = input.kind;
        mergeObserved(target->dpiYOverride_, input.dpiYOverride);
        if (input.parentIsNull) {
            const bool nullParent = !target->parentWidget();
            if (nullParent != *input.parentIsNull || (nullParent && target->parentOwnsChild_))
                throw Error(M2M_E_UNSUPPORTED, "Metric parent observation conflicts with actual QObject ownership");
            target->parentBindingKnown_ = true;
        }
    }
    mergeObserved(found->second->field3c_, o.fontScreenField3c);
    mergeObserved(displayDcAlias_, o.displayDcAlias);
    mergeObserved(displayLogicalDpiY_, o.displayLogicalDpiY);
    mergeObserved(currentQThreadIsApplicationQThread_, o.currentQThreadIsApplicationQThread);
    mergeObserved(currentWidgets_, o.currentWidgets); mergeObserved(maximumWidgets_, o.maximumWidgets);
    mergeObserved(applicationCompileVersion_, o.applicationCompileVersion);
    mergeObserved(applicationAttributes_, o.applicationAttributes);
    if (o.notifyDepth) thread_->observeNotifyDepth(*o.notifyDepth, o.qualification);
}

void WidgetRuntimeState::observeQObjectCount(std::uint32_t count, const std::string& qualification)
{
    if (qualification.empty() || (qobjectCount() && qobjectCount() != count))
        throw Error(M2M_E_UNSUPPORTED, "QObject count observation conflicts with the current shared process state");
    if (!qobjectBase_) qobjectBase_ = count - qobjectDelta_;
}
std::optional<std::uint32_t> WidgetRuntimeState::qobjectCount() const noexcept
{ return qobjectBase_ ? std::optional<std::uint32_t>{*qobjectBase_ + qobjectDelta_} : std::nullopt; }
void WidgetRuntimeState::commitQObjectConstruction(QObjectConstructionRecord& object)
{
    auto self = weak_from_this().lock();
    if (!self || !object.identity_ || object.committed_)
        throw Error(M2M_E_UNSUPPORTED, "QObject constructor counter requires one uncommitted actual object record");
    object.runtime_ = self; object.committed_ = true;
    ++qobjectDelta_; // native LOCK ADD wraps, and does not consume the result
}
void WidgetRuntimeState::commitQObjectConstruction(WidgetState& widget)
{
    requireLive(widget);
    if (widget.runtime_.lock().get() != this || widget.thread_ != thread_)
        throw Error(M2M_E_UNSUPPORTED, "QObject widget counter precedes its actual thread binding");
    commitQObjectConstruction(widget.objectConstruction_);
}
void WidgetRuntimeState::commitQObjectConstruction(GridLayoutState& grid)
{
    if (!grid.nativeOwnerAlive() || grid.thread_ != thread_)
        throw Error(M2M_E_UNSUPPORTED, "QObject grid counter precedes its actual thread binding");
    commitQObjectConstruction(grid.objectConstruction_);
}

MainViewState::MainViewState() : private_(std::make_unique<MainViewPrivateState>()),
    widget_(std::shared_ptr<WidgetState>(new WidgetState(this)))
{
    widget_->componentOwner_ = widget_;
    widget_->widgetDataAssigned_ = true;
    // QTabWidget has earlier children. The known manager/splitter suffix must
    // not replace that unreconstructed population or its pending-child vector.
    widget_->childPrefixUnrecovered_ = true;
}

ViewManagerState::ViewManagerState(const std::shared_ptr<MainViewState>& parent)
    : parent_(parent), widget_(std::shared_ptr<WidgetState>(new WidgetState(this)))
{
    widget_->componentOwner_ = widget_;
    widget_->widgetDataAssigned_ = true;
    widget_->parent_ = parent->widgetState();
    widget_->parentOwnsChild_ = true;
    widget_->parentBindingKnown_ = true;
}

SplitViewState::SplitViewState()
    : widget_(std::shared_ptr<WidgetState>(new WidgetState(this)))
{
    widget_->componentOwner_ = widget_;
    // QWidgetPrivate initializes these before QWidgetPrivate::init. The
    // QWidgetData pointer and low attributes are assigned inside init later.
    widget_->extendedAttributes_ = std::array<std::uint32_t, 3>{{0, 0, 0}};
    // QWidgetPrivate6505e780 defaults, before any constructor callback.
    widget_->inheritedFontMask_ = widget_->inheritedPaletteMask_ = 0;
    widget_->opaqueChildrenDirty_ = true; widget_->opaque_ = false;
    widget_->graphicsEffectPresent_ = false;
    widget_->autoFillBackground_ = false; widget_->stylePresent_ = false;
    widget_->stylesheetLength_ = 0; widget_->proxyPresent_ = false;
    widget_->pendingChildren_.emplace();
    widget_->paintDeviceKind_ = WidgetPaintDeviceKind::QWidget;
    widget_->dpiYOverride_ = 0; // actual fresh null extra
    widget_->eventReceiverKind_ = WidgetEventReceiverKind::QWidget; // base vtable active during init
}
SplitViewState::~SplitViewState() = default;

bool WidgetState::matchesComponentOwner(const std::shared_ptr<WidgetState>& supplied) const noexcept
{
    auto actual = componentOwner_.lock();
    return actual && actual.get() == supplied.get() && !actual.owner_before(supplied) && !supplied.owner_before(actual);
}

std::shared_ptr<WidgetState> WidgetState::parentWidget() const noexcept
{
    auto parent = parent_.lock();
    return alive_ && parent && parent->nativeOwnerAlive() ? parent : nullptr;
}

void WidgetState::releaseNativeOwner() noexcept
{
    alive_ = false;
    parent_.reset();
    parentOwnsChild_ = false;
    childSuffix_.clear();
    pendingChildren_.reset();
    font_.reset(); palette_.reset(); locale_.reset();
    thread_.reset();
    installedLayout_.reset();
    layoutAliasAssigned_ = false;
}

WidgetRuntimeState::WidgetRuntimeState(std::optional<bool> present, std::string qualification)
    : registryPresent_(present), qualification_(std::move(qualification)),
      thread_(std::shared_ptr<WidgetThreadState>(new WidgetThreadState)) {}

void WidgetState::observeObjectName(const qt::String& name)
{ requireLive(*this); mergeObserved(objectName_, std::optional<qt::String>{name}); }
void WidgetState::assignObjectName(const qt::String& name)
{ requireLive(*this); objectName_ = name; }

std::optional<bool> WidgetState::attributeBit(unsigned raw) const
{
    if (raw >= 128) throw Error(M2M_E_UNSUPPORTED, "Widget attribute outside recovered storage");
    const unsigned word = raw / 32; const std::uint32_t bit = 1u << (raw % 32);
    if (!word && attributesLow_) return (*attributesLow_ & bit) != 0;
    if (word && extendedAttributes_) return ((*extendedAttributes_)[word - 1] & bit) != 0;
    if (partialAttributeKnown_[word] & bit) return (partialAttributeBits_[word] & bit) != 0;
    return {};
}
void WidgetState::observeAttributeBit(unsigned raw, bool value)
{
    requireLive(*this);
    auto old = attributeBit(raw);
    if (old && *old != value) throw Error(M2M_E_UNSUPPORTED, "Attribute observation conflicts with live state");
    if (!old) assignAttributeBit(raw, value);
}
void WidgetState::assignAttributeBit(unsigned raw, bool value)
{
    requireLive(*this);
    if (raw >= 128) throw Error(M2M_E_UNSUPPORTED, "Widget attribute outside recovered storage");
    const unsigned word = raw / 32; const std::uint32_t bit = 1u << (raw % 32);
    auto set = [bit, value](std::uint32_t& field) { if (value) field |= bit; else field &= ~bit; };
    if (!word && attributesLow_) set(*attributesLow_);
    else if (word && extendedAttributes_) set((*extendedAttributes_)[word - 1]);
    else { partialAttributeKnown_[word] |= bit; set(partialAttributeBits_[word]); }
}
void WidgetState::observeGeometryEndpoints(const std::array<int, 4>& value)
{ requireLive(*this); mergeObserved(geometry_, std::optional<std::array<int, 4>>{value}); }
void WidgetState::assignGeometryEndpoints(const std::array<int, 4>& value)
{ requireLive(*this); geometry_ = value; }
void WidgetState::observeReceivesChildEvents(bool value)
{ requireLive(*this); mergeObserved(receivesChildEvents_, std::optional<bool>{value}); }
std::shared_ptr<GridLayoutState> WidgetState::installedLayout() const noexcept
{
    auto layout = installedLayout_.lock();
    return alive_ && layout && layout->nativeOwnerAlive() ? layout : nullptr;
}
std::optional<bool> WidgetState::layoutPresence() const noexcept
{
    if (!alive_) return {};
    if (layoutAliasAssigned_) {
        if (installedLayout()) return true;
        // An expired alias while the widget remains alive does not prove native
        // QWidget+A8 was cleared by an unimplemented destructor path.
        return {};
    }
    return events_.layoutPresent;
}
void WidgetState::assignInstalledLayout(const std::shared_ptr<GridLayoutState>& layout)
{
    requireLive(*this);
    if (!layout || !layout->nativeOwnerAlive() ||
        (layoutAliasAssigned_ && installedLayout_.lock() != layout))
        throw Error(M2M_E_UNSUPPORTED, "Layout alias replacement requires native ownership evidence");
    installedLayout_ = layout;
    layoutAliasAssigned_ = true;
    events_.layoutPresent = true;
}
void WidgetState::observeEmptyDynamicBooleanProperties(bool allocated)
{
    requireLive(*this);
    auto prepared = properties_;
    mergeObserved(prepared.extraDataAllocated, std::optional<bool>{allocated});
    mergeObserved(prepared.names, std::optional<std::vector<std::string>>{std::vector<std::string>{}});
    mergeObserved(prepared.values, std::optional<std::vector<bool>>{std::vector<bool>{}});
    properties_ = std::move(prepared);
}
void WidgetState::allocateDynamicBooleanProperties()
{
    requireLive(*this);
    if (!properties_.extraDataAllocated || !properties_.names || !properties_.values)
        throw Error(M2M_E_UNSUPPORTED, "Dynamic property storage is unqualified");
    properties_.extraDataAllocated = true;
}
void WidgetState::appendDynamicPropertyName(std::string name)
{
    requireLive(*this);
    if (properties_.extraDataAllocated != true || !properties_.names || !properties_.values ||
        properties_.names->size() != properties_.values->size())
        throw Error(M2M_E_UNSUPPORTED, "Property name append needs qualified balanced storage");
    properties_.names->push_back(std::move(name));
}
void WidgetState::appendDynamicPropertyBoolean(bool value)
{
    requireLive(*this);
    if (properties_.extraDataAllocated != true || !properties_.names || !properties_.values ||
        properties_.names->size() != properties_.values->size() + 1)
        throw Error(M2M_E_UNSUPPORTED, "Property value append must follow its original name append");
    properties_.values->push_back(value);
}
void WidgetState::observeEventReceiver(const WidgetReceiverEventObservation& o,
    WidgetEventReceiverKind kind, const std::string& qualification)
{
    requireLive(*this);
    if (qualification.empty() || (eventReceiverKind_ != WidgetEventReceiverKind::Unknown &&
        kind != WidgetEventReceiverKind::Unknown && eventReceiverKind_ != kind))
        throw Error(M2M_E_UNSUPPORTED, "Receiver event observation has conflicting identity/type qualification");
    auto prepared = events_;
    if (layoutAliasAssigned_ && o.layoutPresent && layoutPresence() != o.layoutPresent)
        throw Error(M2M_E_UNSUPPORTED, "Layout observation conflicts with the actual installed owner");
    mergeObserved(prepared.objectFilterCount, o.objectFilterCount);
    mergeObserved(prepared.sameApplicationThread, o.sameApplicationThread);
    mergeObserved(prepared.layoutPresent, o.layoutPresent);
    mergeObserved(prepared.gestureContextCount, o.gestureContextCount);
    mergeObserved(prepared.gestureMapExclusive, o.gestureMapExclusive);
    if (o.childEventHandler != WidgetChildEventHandler::Unknown) {
        if (prepared.childEventHandler != WidgetChildEventHandler::Unknown && prepared.childEventHandler != o.childEventHandler)
            throw Error(M2M_E_UNSUPPORTED, "Child-event virtual destination conflicts with its current binding");
        prepared.childEventHandler = o.childEventHandler;
    }
    events_ = prepared;
    if (kind != WidgetEventReceiverKind::Unknown) eventReceiverKind_ = kind;
}
void WidgetRuntimeState::observeApplicationEvents(const WidgetApplicationEventObservation& o,
    const std::string& qualification)
{
    if (qualification.empty()) throw Error(M2M_E_UNSUPPORTED, "Application event observation needs provenance");
    auto prepared = applicationEvents_;
    mergeObserved(prepared.present, o.present); mergeObserved(prepared.closing, o.closing);
    mergeObserved(prepared.internalCallbackCount, o.internalCallbackCount);
    mergeObserved(prepared.gestureManagerPresent, o.gestureManagerPresent);
    mergeObserved(prepared.declarativeChildEventsEnabled, o.declarativeChildEventsEnabled);
    if (o.filters) {
        if (!prepared.filters) prepared.filters = o.filters;
        else {
            if (prepared.filters->size() != o.filters->size())
                throw Error(M2M_E_UNSUPPORTED, "Application filter observation cannot replace a live population");
            for (std::size_t i = 0; i < o.filters->size(); ++i) {
                auto& dst = (*prepared.filters)[i]; const auto& src = (*o.filters)[i];
                using Kind = WidgetApplicationEventObservation::Filter::Kind;
                if (dst.kind == Kind::Unknown) dst.kind = src.kind;
                else if (src.kind != Kind::Unknown && dst.kind != src.kind)
                    throw Error(M2M_E_UNSUPPORTED, "Application filter type conflicts with live entry");
                mergeObserved(dst.sameApplicationThread, src.sameApplicationThread);
            }
        }
    }
    applicationEvents_ = std::move(prepared);
}

bool WidgetRuntimeState::hasLiveRegistration(const void* identity) const noexcept
{
    for (const auto& entry : knownRegistrations_) {
        auto widget = entry.widget.lock();
        if (entry.identity == identity && widget && widget->nativeOwnerAlive()) return true;
    }
    return false;
}

void WidgetRuntimeState::bind(const std::shared_ptr<WidgetState>& widget)
{
    auto self = weak_from_this().lock();
    if (!self || qualification_.empty() || !widget || !widget->nativeOwnerAlive() || !widget->matchesComponentOwner(widget))
        throw Error(M2M_E_UNSUPPORTED, "Widget runtime requires a qualified shared owner and live widget identity");
    if (widget->runtimeWasBound_ && widget->runtime_.expired())
        throw Error(M2M_E_UNSUPPORTED, "An expired native runtime/thread binding cannot be replaced");
    if (auto current = widget->runtime_.lock(); current && current != self)
        throw Error(M2M_E_UNSUPPORTED, "A native widget cannot substitute another Qt runtime owner");
    widget->runtime_ = self;
    widget->runtimeWasBound_ = true;
    widget->thread_ = thread_;
}

void WidgetRuntimeState::bindWidgetComponent(const std::shared_ptr<WidgetState>& widget)
{ bind(widget); }

void WidgetRuntimeState::registerWidget(const std::shared_ptr<WidgetState>& widget)
{
    bind(widget);
    if (registryPresent_ != true) return;
    for (const auto& entry : knownRegistrations_)
        if (entry.identity == widget->nativeIdentity() && entry.widget.lock() == widget) return;
    knownRegistrations_.push_back({widget->nativeIdentity(), widget});
}

void MainViewState::bindWidgetRuntime(const std::shared_ptr<WidgetRuntimeState>& runtime)
{
    if (!nativeOwnerAlive() || !runtime)
        throw Error(M2M_E_UNSUPPORTED, "Main-view widget runtime binding requires live actual owners");
    runtime->registerWidget(widget_);
    if (managerChild_) runtime->registerWidget(managerChild_->widget_);
}

void MainViewState::observeInitialManagerFocusTail(std::string qualification)
{
    auto runtime = widget_->runtime_.lock();
    if (!nativeOwnerAlive() || !managerChild_ || !managerChild_->nativeReceiverAlive() ||
        !runtime || managerChild_->widget_->runtime_.lock() != runtime ||
        managerChild_->pendingSplitView_ || qualification.empty())
        throw Error(M2M_E_UNSUPPORTED, "Initial focus-tail observation requires this live manager/runtime before SplitView allocation");
    if (!initialFocusTailQualification_.empty() && initialFocusTailQualification_ != qualification)
        throw Error(M2M_E_UNSUPPORTED, "Initial focus-tail qualification cannot be replaced");
    initialFocusTailQualification_ = std::move(qualification);
    // Probe002 state1/2 and focus entry4 identify these two existing owners.
    // Do not fabricate the earlier QTabWidget children or opposite ring links.
    widget_->focusPrevious_ = managerChild_.get();
    widget_->focusPreviousKnown_ = true;
    managerChild_->widget_->focusNext_ = this;
    managerChild_->widget_->focusNextKnown_ = true;
}

MainViewState::~MainViewState()
{
    releaseNativeOwner();
}

void MainViewState::releaseNativeOwner() noexcept
{
    // 0e9ec0 releases the separate private allocation before its inherited
    // QTabWidget/QObject child cleanup. Other Qt teardown effects stay pending.
    private_.reset();
    if (managerChild_) managerChild_->releaseNativeOwner();
    managerChild_.reset();
    for (const auto& child : splitViewChildren_) child->releaseNativeOwner();
    splitViewChildren_.clear();
    widget_->releaseNativeOwner();
}

const MainViewPrivateState& MainViewState::privateState() const
{
    if (!private_) throw Error(M2M_E_UNSUPPORTED, "Main-view native private allocation was released");
    return *private_;
}

std::shared_ptr<ViewManagerState> MainViewPrivateState::manager() const noexcept
{
    auto manager = manager_.lock();
    return manager && manager->nativeReceiverAlive() ? manager : nullptr;
}

std::shared_ptr<ViewManagerState> MainViewState::constructManagerChild(doe::SessionNotificationState& state)
{
    if (!private_ || managerChild_)
        throw Error(M2M_E_UNSUPPORTED, "Native main-view manager reconstruction/replacement is unrecovered");
    auto parent = weak_from_this().lock();
    if (!parent)
        throw Error(M2M_E_UNSUPPORTED, "Main-view manager requires the actual live parent owner");
    managerChild_ = std::shared_ptr<ViewManagerState>(new ViewManagerState(parent));
    managerChild_->self_ = managerChild_;
    widget_->childSuffix_.push_back(managerChild_.get());
    if (auto runtime = widget_->runtime_.lock()) runtime->registerWidget(managerChild_->widget_);
    managerChild_->registerConstructorSignals(state);
    return managerChild_;
}

void MainViewState::commitPrivateManagerLink(const std::shared_ptr<ViewManagerState>& manager)
{
    if (!private_ || !manager || !manager->nativeReceiverAlive() ||
        manager != managerChild_ || manager->parentMainView().get() != this)
        throw Error(M2M_E_UNSUPPORTED, "Main-view private link requires the same native manager child");
    if (!manager->nativeLayoutResolved()) manager->requireLayoutForCurrentLookup();
    private_->manager_ = manager;
}

void ViewManagerState::registerConstructorSignals(doe::SessionNotificationState& state)
{
    auto self = self_.lock();
    if (!self || !nativeReceiverAlive())
        throw Error(M2M_E_UNSUPPORTED, "View-manager registration needs its actual live receiver/parent");
    // d34870 ordered 2cfbe0 registrations, raw fifth argument0. The notifier
    // strips the slot marker/signature and deduplicates receiver+bare method.
    for (const auto& binding : {
        std::pair{"2signalMakeLayoutInViewManager()", "slotMakeLayout"},
        std::pair{"2signalSetWidgetInViewManager()", "slotSetWidget"},
        std::pair{"2signalRefreshSceneInViewManager()", "slotRefreshScene"},
        std::pair{"2signalGetSceneInViewManager()", "slotGetScene"}}) {
        auto& receivers = state.viewManagerRegistrations[binding.first];
        bool present = false;
        for (const auto& receiver : receivers)
            if (receiver.receiver.lock() == self && receiver.bareMethod == binding.second) present = true;
        if (!present) receivers.push_back({self, binding.second});
    }
}

void ViewManagerState::requireLayoutForCurrentLookup() const
{
    if (!nativeReceiverAlive())
        throw Error(M2M_E_UNSUPPORTED, "View-manager receiver was destroyed with its native parent");
    if (gridParentTransferPending())
        throw Error(M2M_E_UNSUPPORTED, "Grid layout installation continuation remains pending before the manager layout cache");
    if (cachedGridLayout())
        throw Error(M2M_E_UNSUPPORTED, "Cached empty grid reaches native itemAtPosition(0,0) before View_Default/Primary construction");
    throw Error(M2M_E_UNSUPPORTED, "View-manager null layout reaches native lazy allocation/install before current child lookup");
}

GridLayoutState::GridLayoutState(GridConstructionProfile profile) : profile_(profile)
{
    if (profile == GridConstructionProfile::ParentedPending) {
        rows_ = columns_ = 0;
        layoutFlags_ = 6;
        layoutFlagsKnownMask_ = 0x0f;
        dirtyFlags_ = 4;
        dirtyFlagsKnownMask_ = 0x17;
    }
}
std::shared_ptr<GridLayoutState> GridLayoutState::beginParentedConstruction(const std::shared_ptr<WidgetState>& proposedParent)
{
    if (!proposedParent || !proposedParent->nativeOwnerAlive())
        throw Error(M2M_E_UNSUPPORTED, "Parented grid allocation requires its actual live proposed widget");
    // Proposed parent is a cursor input, not committed QObject parent/thread.
    auto grid = std::shared_ptr<GridLayoutState>(new GridLayoutState(GridConstructionProfile::ParentedPending));
    grid->allocationOwner_ = grid;
    return grid;
}
bool GridLayoutState::nativeOwnerAlive() const noexcept
{
    if (!alive_) return false;
    if (!parentWasAssigned_) return true; // pending allocation has its own owner
    auto parent = parentWidget_.lock();
    return parent && parent->nativeOwnerAlive();
}
void GridLayoutState::releaseNativeOwner() noexcept
{
    alive_ = false;
    parent_.reset(); parentWidget_.reset(); thread_.reset();
}

std::shared_ptr<GridLayoutState> ViewManagerState::installedWidgetLayout() const noexcept
{
    return nativeReceiverAlive() ? widget_->installedLayout() : nullptr;
}

std::shared_ptr<GridLayoutState> ViewManagerState::cachedGridLayout() const noexcept
{
    auto layout = cachedLayout_.lock();
    return nativeReceiverAlive() && layout && layout->nativeOwnerAlive() ? layout : nullptr;
}

std::shared_ptr<GridLayoutState> ViewManagerState::beginGridInstall()
{
    auto self = self_.lock();
    if (!self || !nativeReceiverAlive())
        throw Error(M2M_E_UNSUPPORTED, "Grid layout requires its actual live view-manager owner");
    if (gridParentTransferPending())
        throw Error(M2M_E_UNSUPPORTED, "Pending grid child-event continuation cannot replay native layout allocation");
    if (auto layout = cachedGridLayout()) return layout;
    if (widget_->layoutAliasAssigned_ || !gridChildren_.empty())
        throw Error(M2M_E_UNSUPPORTED, "Native existing-layout replacement or failed-install path is unrecovered");

    // d34920 sees the fresh null manager guard. d34da0 creates a parentless
    // concrete QGridLayout; its row/column setup leaves the item list empty.
    auto layout = std::shared_ptr<GridLayoutState>(new GridLayoutState());
    layout->allocationOwner_ = layout;
    layout->margins_.fill(0); // initial invalidate has no parent to notify
    // QWidget::setLayout commits these before QObject::setParent. On the
    // source-bound fresh same-thread path, parent membership precedes callbacks.
    layout->topLevel_ = true;
    layout->layoutFlags_ |= 1u;
    widget_->assignInstalledLayout(layout);
    layout->parent_ = self;
    layout->parentWidget_ = widget_;
    layout->parentWasAssigned_ = true;
    layout->thread_ = widget_->threadState();
    gridChildren_.push_back(layout);
    widget_->childSuffix_.push_back(layout.get());
    gridInstallStage_ = GridInstallStage::ChildAddedPending;
    return layout;
}

std::shared_ptr<GridLayoutState> ViewManagerState::ensureGridLayout()
{
    if (!nativeReceiverAlive())
        throw Error(M2M_E_UNSUPPORTED, "Grid layout requires its actual live view-manager owner");
    if (auto layout = cachedGridLayout()) return layout;
    (void)beginGridInstall();
    // The receiver's QObject::childEvent is a pure leaf, but application notify,
    // filters and the later parent-transfer hooks are not thereby eliminated.
    // cachedLayout_, View_Default name and split-view construction stay pending.
    throw Error(M2M_E_UNSUPPORTED, "Grid layout parent child-event dispatch and post-parent hooks are unrecovered");
}

std::shared_ptr<GridLayoutState> ViewManagerState::ensureGridLayout(const GridStartupObservation& observed)
{
    auto self = self_.lock();
    if (!self || !nativeReceiverAlive())
        throw Error(M2M_E_UNSUPPORTED, "Grid layout requires its actual live view-manager owner");
    // Native cached retrieval does not re-run constructor callback predicates.
    if (auto layout = cachedGridLayout()) return layout;
    if (observed.receiver.lock() != self || observed.qualification.empty())
        throw Error(M2M_E_UNSUPPORTED, "Grid continuation needs a qualified observation of this actual manager");
    if (!gridQualification_.empty() && gridQualification_ != observed.qualification)
        throw Error(M2M_E_UNSUPPORTED, "Pending grid continuation cannot substitute another startup qualification");
    // The implemented ownership prefix is the same-thread branch. A different
    // thread clears native parent before append; do not enter that branch here.
    if (observed.parentThreadMatches != true)
        throw Error(M2M_E_UNSUPPORTED, "Grid parent thread affinity is unavailable or outside the recovered startup profile");
    if (gridInstallStage_ == GridInstallStage::Absent) (void)beginGridInstall();
    auto layout = installedWidgetLayout();
    if (!layout || gridChildren_.size() != 1 || gridChildren_.front() != layout ||
        layout->parentViewManager() != self)
        throw Error(M2M_E_UNSUPPORTED, "Grid continuation lost its actual installed child identity");
    gridQualification_ = observed.qualification;

    if (gridInstallStage_ == GridInstallStage::ChildAddedPending) {
        if (observed.applicationPresent != true || observed.internalCallbackCount != 0u)
            throw Error(M2M_E_UNSUPPORTED, "Grid ChildAdded application/internal callback population is unqualified");
        if (!observed.applicationFilters || observed.applicationFilters->size() != 1 ||
            observed.applicationFilters->front().kind != GridApplicationFilter::Kind::WindowsVistaStyle)
            throw Error(M2M_E_UNSUPPORTED, "Grid ChildAdded requires the observed single Vista-style application filter");
        // The foreign-thread branch emits qWarning before skipping the filter;
        // its diagnostics are not elided by the filter's pure false body.
        if (observed.applicationFilters->front().sameApplicationThread != true)
            throw Error(M2M_E_UNSUPPORTED, "Grid application-filter thread affinity or warning path is unqualified");
        if (observed.objectFilterCount != 0u)
            throw Error(M2M_E_UNSUPPORTED, "Grid ChildAdded receiver event-filter population is unqualified");
        // Raw44: Vista/QWindowsStyle eventFilter -> QObject false; installed
        // QLayout event returns; QWidget -> QObject childEvent returns. The
        // QApplication closing-down early return also has no projected effects.
        // Parent transfer ignores the notify result. No callback stand-in runs.
        gridInstallStage_ = GridInstallStage::PostParentHooksPending;
    }
    if (gridInstallStage_ == GridInstallStage::PostParentHooksPending) {
        if (observed.declarativeChildEventsEnabled != false || observed.childHookDataPresent != false)
            throw Error(M2M_E_UNSUPPORTED, "Grid post-parent declarative bookkeeping or child hook remains unqualified");
        if (layout->menuWidget_ || !layout->items_.empty())
            throw Error(M2M_E_UNSUPPORTED, "Grid contained-widget reparenting requires the actual empty constructor population");
        // 650451c0 has no contained-widget iteration for this actual empty list.
        // Then QGridLayout::invalidate -> QLayout::invalidate mutates this owner
        // before update and before the final manager window test.
        layout->dirtyFlags_ |= 4u;
        layout->cachedSizeValue_ = -1;
        layout->geometryEndpoints_ = {{0, 0, -1, -1}};
        gridInstallStage_ = GridInstallStage::UpdatePending;
    }
    if (gridInstallStage_ == GridInstallStage::UpdatePending) {
        // This increment covers the observed inactive update branch only. A
        // contradictory current flag profile is refused before update; it is
        // not adopted as an invented external mutation of the actual layout.
        if (!observed.currentLayoutFlags || *observed.currentLayoutFlags != layout->layoutFlags_ ||
            (layout->layoutFlags_ & 4u) != 0)
            throw Error(M2M_E_UNSUPPORTED, "Grid update flags differ from the recovered inactive startup profile");
        gridInstallStage_ = GridInstallStage::WindowTailPending;
    }
    if (gridInstallStage_ == GridInstallStage::WindowTailPending) {
        if (!observed.currentWindowFlags || (*observed.currentWindowFlags & 1u) != 0)
            throw Error(M2M_E_UNSUPPORTED, "Grid final window predicate or top-level size-hint effects remain unqualified");
        // QWidget::setLayout has now returned. d34d50 commits guarded+28 only
        // here; View_Default name, splitter/Primary and aliases remain pending.
        cachedLayout_ = layout;
        gridInstallStage_ = GridInstallStage::Cached;
    }
    return layout;
}

void ViewManagerState::releaseNativeOwner() noexcept
{
    alive_ = false;
    // The native guarded layout wrapper is released before QWidget descendant
    // cleanup. This invalidates leases; it does not replay unclosed Qt callbacks.
    cachedLayout_.reset();
    for (const auto& layout : gridChildren_) {
        layout->releaseNativeOwner();
    }
    gridChildren_.clear();
    gridInstallStage_ = GridInstallStage::Absent;
    gridQualification_.clear();
    // Before parent-list insertion this is only a retained constructor lease.
    // After insertion, MainView owns the child and invalidates it in its own
    // child order, after the earlier manager. No Qt destructor tail is claimed.
    if (pendingSplitView_ && !pendingSplitView_->widget_->parentOwnsChild_)
        pendingSplitView_->releaseNativeOwner();
    pendingSplitView_.reset();
    widget_->releaseNativeOwner();
}

SplitViewConstructionReport ViewManagerState::ensureDefaultSplitView(const SplitViewStartupObservation& observed)
{
    auto self = self_.lock();
    auto parent = parent_.lock();
    if (!self || !parent || !nativeReceiverAlive() || observed.receiver.lock() != self ||
        observed.qualification.empty())
        throw Error(M2M_E_UNSUPPORTED, "Split-view continuation requires this qualified live manager/MainView");
    if (!cachedGridLayout())
        return {SplitViewConstructStage::Absent, false, "View_Default has not returned from the preceding grid getter"};
    auto runtime = widget_->runtime_.lock();
    if (!runtime || parent->widget_->runtime_.lock() != runtime)
        throw Error(M2M_E_UNSUPPORTED, "Split-view construction requires the same actual Qt widget runtime");
    if (!pendingSplitView_) {
        pendingSplitView_ = std::shared_ptr<SplitViewState>(new SplitViewState());
        pendingSplitView_->qualification_ = observed.qualification;
        runtime->bind(pendingSplitView_->widget_);
        if (auto v = runtime->values_) {
            auto& w = *pendingSplitView_->widget_;
            if (v->defaultFont) w.font_ = WidgetFontValue{v->defaultFont->data, 0};
            if (v->defaultPalette) w.palette_ = WidgetPaletteValue{v->defaultPalette->data, 0};
            w.locale_ = v->freshLocale;
            w.events_ = v->freshWidgetEvents;
            w.sendChildEvents_ = v->freshSendChildEvents; w.fieldD0Present_ = v->freshFieldD0Present;
        }
    }
    auto split = pendingSplitView_;
    auto& child = *split->widget_;
    if (!child.nativeOwnerAlive() || split->qualification_ != observed.qualification)
        throw Error(M2M_E_UNSUPPORTED, "Pending split-view cannot replace its identity or constructor qualification");
    auto pending = [&](const char* issue) {
        return SplitViewConstructionReport{split->stage_, false, issue};
    };
    if (split->stage_ == SplitViewConstructStage::WidgetRegistryPending) {
        if (!runtime->allWidgetsRegistryPresent())
            return pending("QApplication allWidgets registry presence is unqualified before QWidgetData assignment");
        // 6506e53b: same actual identity enters the existing set, before the
        // widget-data pointer, initial attributes, geometry or native parent.
        runtime->registerWidget(split->widget_);
        split->stage_ = SplitViewConstructStage::WidgetParentKindPending;
    }
    if (split->stage_ == SplitViewConstructStage::WidgetParentKindPending) {
        if (!observed.parentWindowFlags)
            return pending("Initial QWidget parent kind is unqualified");
        if ((*observed.parentWindowFlags & 0xffu) == 0x11u)
            return pending("Desktop-parent normalization needs the separate parentless constructor branch");
        child.widgetDataAssigned_ = true;
        child.windowFlags_ = 0;
        child.attributesLow_ = 1u << 16; // setAttribute(0x10,true)
        (*child.extendedAttributes_)[1] |= 1u << 12; // raw0x4c
        child.geometry_ = std::array<int, 4>{{0, 0, 99, 29}};
        child.focusNext_ = child.focusPrevious_ = child.nativeIdentity();
        child.focusNextKnown_ = child.focusPreviousKnown_ = true;
        child.inSetParent_ = true; // private+1d8 bit24 before parenting predicates
        split->stage_ = SplitViewConstructStage::ParentNativeAttributePending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentNativeAttributePending) {
        const bool nativeChildren = observed.parentExtraNativeChildren == true ||
            (observed.parentAttributesLow && (*observed.parentAttributesLow & (1u << 8)));
        if (!nativeChildren && (!observed.parentExtraNativeChildren || !observed.parentAttributesLow))
            return pending("MainView extra+5c bit5/attribute8 predicate is unqualified before native parent assignment");
        if (nativeChildren) {
            // setAttribute first writes the requested bit, then enters its
            // native-window helper. Retain that store at the exact guard.
            (*child.extendedAttributes_)[2] |= 1u << 4; // raw0x64
            split->stage_ = SplitViewConstructStage::ParentNativeAttributeEffectPending;
            return pending("Native-window attribute helper remains unresolved after the attribute store");
        }
        // Fresh private+A0/null, old-parent/null, native handle0 and visible0
        // close the pre-helper branches. QObjectPrivate sets parent before its
        // thread comparison and before parent-list insertion (6712d519).
        child.parent_ = parent->widget_;
        child.parentBindingKnown_ = true;
        split->stage_ = SplitViewConstructStage::ParentThreadPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentNativeAttributeEffectPending)
        return pending("Native-window attribute helper remains unresolved after the attribute store");
    if (split->stage_ == SplitViewConstructStage::ParentThreadPending) {
        if (observed.parentThreadMatches != true)
            return pending("QObject parent thread comparison/warning is unqualified after parent assignment");
        parent->splitViewChildren_.push_back(split);
        parent->widget_->childSuffix_.push_back(split.get());
        child.parentOwnsChild_ = true;
        // isWidget skips this helper's raw44/declarative branch. QWidget emits
        // its own ChildAdded later, beyond the current propagation boundary.
        split->stage_ = SplitViewConstructStage::ParentHookPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentHookPending) {
        if (observed.childHookDataPresent != false)
            return pending("QObject final parent hook remains unqualified after actual child insertion");
        // Platform tail stores flags, clears raw3c/0f/10, and normalizes the
        // zero input flags with the actual nonnull parent before native create.
        (*child.extendedAttributes_)[0] &= ~(1u << 28);
        *child.attributesLow_ &= ~((1u << 15) | (1u << 16));
        child.windowFlags_ = 0x0800f000;
        split->stage_ = SplitViewConstructStage::ParentNativeCreationPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentNativeCreationPending) {
        if (!observed.parentExtendedAttributes || ((*observed.parentExtendedAttributes)[0] & (1u << 28)))
            return pending("Parent-created predicate or platform native-window creation is unresolved");
        split->stage_ = SplitViewConstructStage::ParentVisibilityPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentVisibilityPending) {
        if (!observed.parentAttributesLow)
            return pending("Parent visibility is unqualified before child hidden-state update");
        if (*observed.parentAttributesLow & (1u << 15)) *child.attributesLow_ |= 1u << 16;
        // Previous explicit-hide was false; raw0x45 remains false. Child extra
        // and native handle are null, so their platform cleanup branches skip.
        (*child.extendedAttributes_)[1] &= ~(1u << 5);
        split->stage_ = SplitViewConstructStage::ParentDropSitePending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentDropSitePending) {
        if (!observed.parentExtendedAttributes)
            return pending("Parent drop-site attribute is unqualified");
        if ((*observed.parentExtendedAttributes)[1] & (1u << 15)) {
            (*child.extendedAttributes_)[1] |= 1u << 15;
            split->stage_ = SplitViewConstructStage::ParentDropSiteEffectPending;
            return pending("Drop-site attribute helper is unresolved after the attribute store");
        }
        // invalidateBuffer performs only reads before its visible0 early exit.
        split->stage_ = SplitViewConstructStage::ParentBackingStorePending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentDropSiteEffectPending)
        return pending("Drop-site attribute helper is unresolved after the attribute store");
    if (split->stage_ == SplitViewConstructStage::ParentBackingStorePending) {
        if (!observed.parentWindowFlags || (*observed.parentWindowFlags & 1u) == 0 ||
            observed.parentTopExtraPresent != false)
            return pending("Parent window/backing-store ownership is unqualified before post-parent propagation");
        // Old child extra was null; after reparenting its window is MainView.
        // With the observed null topExtra, both backing-store routes skip.
        split->stage_ = SplitViewConstructStage::ParentCompatibilityPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentCompatibilityPending) {
        if (!observed.applicationCompileVersion)
            return pending("Application compile-version predicate is unqualified before focus reparenting");
        if (*observed.applicationCompileVersion >= 0x40200 && !observed.applicationAttributes)
            return pending("Application compatibility attribute is unqualified before focus reparenting");
        if (*observed.applicationCompileVersion < 0x40200 || (*observed.applicationAttributes & 1u)) {
            split->stage_ = SplitViewConstructStage::ParentCompatibilityEffectPending;
            return pending("Compatibility branch requires native widget creation before focus reparenting");
        }
        split->stage_ = SplitViewConstructStage::ParentFocusPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentCompatibilityEffectPending)
        return pending("Compatibility native creation remains unresolved before focus reparenting");
    if (split->stage_ == SplitViewConstructStage::ParentFocusPending) {
        if (parent->initialFocusTailQualification_.empty() ||
            !parent->widget_->focusPreviousKnown_ || parent->widget_->focusPrevious_ != this ||
            !widget_->focusNextKnown_ || widget_->focusNext_ != parent.get())
            return pending("Initial MainView focus tail is not bound to this actual manager");
        if (!child.focusNextKnown_ || !child.focusPreviousKnown_ ||
            child.focusNext_ != split.get() || child.focusPrevious_ != split.get() ||
            child.parentWidget() != parent->widget_ || !child.parentOwnsChild_)
            return pending("Fresh self-ring and actual MainView ownership are required before the focus splice");
        // 6505fc40: old window is the fresh self-ring; new window is MainView.
        // No partition iterations occur. At6505fdd9..fdf7 these are the four
        // native writes, preserving the unqualified opposite preexisting links.
        parent->widget_->focusPrevious_ = split.get();
        widget_->focusNext_ = split.get();
        child.focusPrevious_ = this;
        child.focusNext_ = parent.get();
        // 6506df49 restores saved attribute0x2a (extended word0 bit10), false
        // in this fresh receiver before setParent. The setter's false leaf is closed.
        (*child.extendedAttributes_)[0] &= ~(1u << 10);
        split->stage_ = SplitViewConstructStage::ParentFontPreparationPending;
    }
    auto values = runtime->values_;
    auto& ancestor = *parent->widget_;
    // Exact QWidget base metadata applies at this constructor cursor. Native
    // lookup first checks the exact class key, then the ordered inherited keys.
    auto classValue = [](const auto& entries, const auto& fallback) {
        auto result = fallback;
        for (const auto& e : *entries) if (e.first == "QWidget") return std::optional{e.second};
        for (const auto& e : *entries) {
            if (e.first.find('\0') != std::string::npos) return decltype(result){};
            if (e.first == "QPaintDevice" || e.first == "QObject") return std::optional{e.second};
        }
        return result;
    };
    auto paletteGroup = [&](WidgetState& w) -> const char* {
        if (!w.palette_ || !w.attributesLow_) return "Actual palette value/receiver attributes are unqualified";
        auto& word = w.palette_->groupResolveWord;
        if (*w.attributesLow_ & 1u) { word = (word & 0xfffffff1u) | 1u; return nullptr; }
        if (*w.attributesLow_ & (1u << 15)) return "Visible palette receiver requires active-window resolution";
        if (!values || values->modalState != false) return "Modal palette receiver predicate is unqualified";
        word &= 0xfffffff0u;
        return nullptr;
    };
    if (split->stage_ == SplitViewConstructStage::ParentFontPreparationPending) {
        if (!values || !values->defaultFont || values->fontMutexPresent != true ||
            values->fontCachePresent != true || !values->fontClasses)
            return pending("Application font/cache ownership or complete class population is unqualified");
        if (!ancestor.extendedAttributes_ || ((*ancestor.extendedAttributes_)[2] & 2u))
            return pending("Live parent style-sheet attribute branch is outside the qualified font preparation");
        auto selected = classValue(values->fontClasses, values->defaultFont);
        if (!selected || selected->data != values->defaultFont->data)
            return pending("Class-specific font resolution requires the unrecovered different-data branch");
        if (!ancestor.font_ || !child.font_ || ancestor.font_->data != child.font_->data ||
            child.font_->resolveMask != 0)
            return pending("Font preparation requires the actual same-data, zero-mask resolve branch");
        // Local copied parent font has mask0. Same data means equality and
        // field38 equality without deep comparisons; updater makes no store.
        split->stage_ = SplitViewConstructStage::ParentPalettePreparationPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentPalettePreparationPending) {
        if (!values || !values->defaultPalette || values->paletteCachePresent != true || !values->paletteClasses)
            return pending("Application palette/cache ownership or complete class population is unqualified");
        auto selected = classValue(values->paletteClasses, values->defaultPalette);
        if (!selected) return pending("Palette class metadata query is unqualified");
        // Both native parent branches call the real QWidget::palette BEFORE
        // any optional different-class resolve. Its group write is persistent.
        if (auto issue = paletteGroup(ancestor)) return pending(issue);
        split->preparedPalette_ = *ancestor.palette_;
        split->preparedPalette_->groupResolveWord &= 0xfu;
        split->paletteClassMatchesDefault_ = selected->data == values->defaultPalette->data;
        split->stage_ = SplitViewConstructStage::ParentPaletteResolvePending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentPaletteResolvePending) {
        if (!split->paletteClassMatchesDefault_)
            return pending("Different-class palette resolve remains pending after the actual parent group write");
        if (!child.palette_ || !split->preparedPalette_ || child.palette_->data != split->preparedPalette_->data ||
            (child.palette_->groupResolveWord & 0xfffffff0u) != 0)
            return pending("Palette deep comparison/update remains pending after the parent group write");
        // Sharing resolve returns parent data/group with child's high mask.
        // 650668f0 ignores low group differences when data/high mask agree, so
        // it does NOT assign that candidate group to the child.
        split->preparedPalette_.reset();
        split->stage_ = SplitViewConstructStage::ParentDirectionPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentDirectionPending) {
        if (!ancestor.extendedAttributes_ || !child.extendedAttributes_ ||
            (((*ancestor.extendedAttributes_)[0] ^ (*child.extendedAttributes_)[0]) & (1u << 24)))
            return pending("Layout-direction change and its children/events remain unresolved");
        split->stage_ = SplitViewConstructStage::ParentLocalePending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentLocalePending) {
        if (!child.locale_ || !ancestor.locale_ || child.locale_->index != ancestor.locale_->index ||
            child.locale_->numberOptions != ancestor.locale_->numberOptions)
            return pending("Locale mutation/child notification remains unresolved for unequal or unknown inputs");
        // 670038f0 compares locale data selected by uint16 index and uint16
        // options. Equal fields prove equality; trailing object bytes are not data.
        split->stage_ = SplitViewConstructStage::ParentEnabledPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentEnabledPending) {
        if (!ancestor.attributesLow_ || (*ancestor.attributesLow_ & ((1u << 10) | 1u)))
            return pending("Enabled/updates propagation requires the qualified unchanged parent branches");
        split->stage_ = SplitViewConstructStage::ParentStylePending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentStylePending) {
        if (!values || values->applicationStylesheetLength != 0 || ancestor.stylePresent_ != false ||
            child.stylePresent_ != false || child.stylesheetLength_ != 0)
            return pending("Style inheritance requires the actual null styles and empty stylesheets");
        split->stage_ = SplitViewConstructStage::ParentChildAddedPending;
    }
    auto eventInputs = [&](WidgetState& receiver, bool parentChange) -> const char* {
        const auto& appEvents = runtime->applicationEvents_;
        if (!values || appEvents.present != true || appEvents.closing != false ||
            appEvents.internalCallbackCount != 0u)
            return "Application/QInternal event inputs are unqualified";
        const auto& filters = appEvents.filters;
        if (!filters || filters->size() != 1 ||
            filters->front().kind != WidgetApplicationEventObservation::Filter::Kind::WindowsVistaStyle ||
            filters->front().sameApplicationThread != true)
            return "Actual application-filter population/affinity is unqualified";
        if (receiver.events_.sameApplicationThread != true || receiver.events_.objectFilterCount != 0u ||
            receiver.events_.layoutPresent != false)
            return "Actual receiver filter/affinity/layout event inputs are unqualified";
        if (parentChange) {
            if (!appEvents.gestureManagerPresent) return "Application gesture-manager presence is unqualified";
            if (*appEvents.gestureManagerPresent) {
                if (receiver.events_.gestureContextCount != 0u)
                    return "Current widget gesture population is not qualified empty";
                auto current = receiver.parentWidget();
                while (current) {
                    if (current->events_.gestureContextCount != 0u || current->events_.gestureMapExclusive != true)
                        return "Ancestor gesture map population/exclusive ownership is unqualified before nonconst access";
                    if (!current->windowFlags_) return "Gesture ancestor window boundary is unqualified";
                    if (*current->windowFlags_ & 1u) break;
                    current = current->parentWidget();
                }
            }
        }
        return nullptr;
    };
    if (split->stage_ == SplitViewConstructStage::ParentChildAddedPending) {
        if (!child.sendChildEvents_) return pending("QObject send-child-events flag is unqualified");
        if (*child.sendChildEvents_) {
            if (auto issue = eventInputs(ancestor, false)) return pending(issue);
            // SfMoMainView vt+20 -> QTabWidget event. Raw44 skips its raw4c
            // layout effect, then QWidget -> QObject childEvent pure leaf.
            split->stage_ = SplitViewConstructStage::ParentPendingChildrenPending;
        } else split->stage_ = SplitViewConstructStage::ParentChangeEventPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentPendingChildrenPending) {
        if (!ancestor.pendingChildren_) return pending("Actual parent's pending-child population is unqualified");
        if (ancestor.pendingChildren_->empty())
            return pending("Empty pending-child vector reaches native posted raw43 allocation/queue ownership");
        ancestor.pendingChildren_->push_back({0, split->widget_});
        split->stage_ = SplitViewConstructStage::ParentChangeEventPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentChangeEventPending) {
        if (child.sendChildEvents_ == true && child.fieldD0Present_ != false)
            return pending("Optional child-polished raw45 delivery remains unresolved");
        if (auto issue = eventInputs(child, true)) return pending(issue);
        // Raw15 common delivery reaches QWidget's base changeEvent pure return.
        split->stage_ = SplitViewConstructStage::ParentHiddenPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentHiddenPending) {
        if (!ancestor.attributesLow_) return pending("Live parent visibility is unqualified at final hidden-state store");
        if (*ancestor.attributesLow_ & (1u << 15)) *child.attributesLow_ |= 1u << 16;
        else *child.attributesLow_ &= ~(1u << 16);
        child.opaqueChildrenDirty_ = true; // 6505ebca BEFORE graphics-effect traversal
        split->stage_ = SplitViewConstructStage::ParentDirtyEffectsPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentDirtyEffectsPending) {
        // Actual parent chain, including window ancestors; 6505eb50 scans to
        // null before the separate dirty propagation loop tests window bits.
        auto current = split->widget_;
        while (current) {
            if (current->graphicsEffectPresent_ != false)
                return pending("Graphics-effect cache update is unresolved after the child's dirty-bit write");
            current = current->parentWidget();
        }
        current = child.parentWidget();
        while (current) {
            if (!current->opaqueChildrenDirty_) return pending("Ancestor opaque-cache dirty state is unqualified");
            if (*current->opaqueChildrenDirty_) break;
            current->opaqueChildrenDirty_ = true;
            if (!current->windowFlags_) {
                split->stage_ = SplitViewConstructStage::ParentDirtyWindowPending;
                return pending("Dirty ancestor window boundary is unqualified after its bit write");
            }
            if (*current->windowFlags_ & 1u) break;
            current = current->parentWidget();
        }
        split->stage_ = SplitViewConstructStage::ParentOpacityPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentDirtyWindowPending)
        return pending("Dirty ancestor window boundary remains unqualified after its bit write");
    if (split->stage_ == SplitViewConstructStage::ParentOpacityPending) {
        if (child.graphicsEffectPresent_ != false || !child.attributesLow_)
            return pending("Current graphics-effect/opacity predicates are unqualified");
        if (*child.attributesLow_ & ((1u << 4) | (1u << 8)))
            return pending("Opaque attribute transition requires the native cache helper");
        if (auto issue = paletteGroup(child)) return pending(issue);
        if (child.autoFillBackground_ != false || !child.windowFlags_ || (*child.windowFlags_ & 1u))
            return pending("Auto-fill/window palette opacity dependencies remain unresolved after palette group write");
        if (!child.opaque_) return pending("Current opacity state is unqualified");
        if (*child.opaque_) {
            child.opaque_ = false;
            split->stage_ = SplitViewConstructStage::ParentOpacityEffectPending;
            return pending("Opacity transition cache helper remains unresolved after clearing opaque");
        }
        split->stage_ = SplitViewConstructStage::ParentProxyPending;
    }
    if (split->stage_ == SplitViewConstructStage::ParentOpacityEffectPending)
        return pending("Opacity transition cache helper remains unresolved after clearing opaque");
    if (split->stage_ == SplitViewConstructStage::ParentProxyPending) {
        // Old top-level was the fresh child itself, with no extra/proxy. The
        // new MainView proxy is not substituted for that source receiver.
        if (child.proxyPresent_ != false) return pending("Old-window graphics-proxy cleanup remains unresolved");
        child.inSetParent_ = false;
        split->stage_ = SplitViewConstructStage::WidgetFontDevicePending;
    }
    if (!split->tail_) split->tail_ = std::make_unique<WidgetConstructorTail>();
    const auto& tail = split->tail_->resume(*runtime, split->widget_);
    if (tail.stage == WidgetConstructorTailStage::Create) split->stage_ = SplitViewConstructStage::WidgetCreatePending;
    else if (tail.stage == WidgetConstructorTailStage::PostPending) split->stage_ = SplitViewConstructStage::WidgetPostPending;
    return SplitViewConstructionReport{split->stage_, false, tail.issue};
}

} // namespace m2m::engine
