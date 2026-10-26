#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "m2m/qt4.hpp"

namespace m2m::doe { struct SessionNotificationState; }
namespace m2m::engine::simcontrol300 { class StagedEditor300; class EditorGridConstruction; class EditorBaseConstruction; }
namespace m2m::engine {
class MainViewState;
class ViewManagerState;
class SplitViewState;
class WidgetRuntimeState;
class WidgetState;
class WidgetThreadState;
class WidgetEventDispatch;
class WidgetEventState;
class WidgetConstructorTail;
class GridLayoutState;
enum class WidgetEventReceiverKind { Unknown, QWidget };
enum class WidgetChildEventHandler { Unknown, QObjectLeaf };

// The native names and QVariant lists have distinct append points. This
// bounded projection supports boolean values; unknown population is not empty.
struct WidgetDynamicBooleanProperties {
    std::optional<bool> extraDataAllocated;
    std::optional<std::vector<std::string>> names;
    std::optional<std::vector<bool>> values;
};

// Observation-local aliases are NOT original pointer values or global IDs.
// A receipt maps each positive alias to a native shared-data identity. Runtime
// interning creates one actual C++ data owner per alias/type/runtime.
using WidgetFontAlias = std::uint32_t;
using WidgetPaletteAlias = std::uint32_t;

struct WidgetFontValueObservation {
    WidgetFontAlias dataAlias = 0;
    std::uint32_t resolveMask = 0;
};
struct WidgetPaletteValueObservation {
    WidgetPaletteAlias dataAlias = 0;
    std::uint32_t groupResolveWord = 0;
};
struct WidgetLocaleValue {
    std::uint16_t index = 0;
    std::uint16_t numberOptions = 0;
};
struct WidgetFontDataObservation {
    WidgetFontAlias alias = 0;
    std::optional<std::uint32_t> comparisonField38;
    std::optional<std::uint32_t> deviceScreenField3c;
};
struct WidgetPaletteDataObservation { WidgetPaletteAlias alias = 0; };

struct WidgetFontClassObservation {
    std::string classKeyBytes;
    WidgetFontValueObservation value;
};
struct WidgetPaletteClassObservation {
    std::string classKeyBytes;
    WidgetPaletteValueObservation value;
};
struct WidgetApplicationFontObservation {
    std::optional<WidgetFontValueObservation> defaultValue;
    std::optional<bool> mutexPresent;
    std::optional<bool> classCachePresent;
    // nullopt means incomplete/unqualified population, not an empty cache.
    // Present entries retain native firstNode/nextNode iteration order.
    std::optional<std::vector<WidgetFontClassObservation>> classes;
};
struct WidgetApplicationPaletteObservation {
    std::optional<WidgetPaletteValueObservation> defaultValue;
    std::optional<bool> classCachePresent;
    std::optional<std::vector<WidgetPaletteClassObservation>> classes;
};

struct WidgetReceiverEventObservation {
    WidgetChildEventHandler childEventHandler = WidgetChildEventHandler::Unknown;
    std::optional<std::size_t> objectFilterCount;
    std::optional<bool> sameApplicationThread;
    std::optional<bool> layoutPresent;
    std::optional<std::size_t> gestureContextCount;
    std::optional<bool> gestureMapExclusive; // ancestor nonconst begin/end may detach
};
struct WidgetApplicationEventObservation {
    std::optional<bool> present;
    std::optional<bool> closing;
    std::optional<std::size_t> internalCallbackCount;
    // Each element is one actual observed filter, in native order.
    struct Filter { enum class Kind { Unknown, WindowsVistaStyle }; Kind kind = Kind::Unknown;
        std::optional<bool> sameApplicationThread; };
    std::optional<std::vector<Filter>> filters;
    std::optional<bool> gestureManagerPresent;
    std::optional<bool> declarativeChildEventsEnabled;
};
struct WidgetPendingChildObservation {
    std::uint32_t opaqueAlias = 0;
    std::weak_ptr<WidgetState> knownWidget;
};
struct WidgetInitialValueObservation {
    std::optional<WidgetFontValueObservation> font;
    std::optional<WidgetPaletteValueObservation> palette;
    std::optional<WidgetLocaleValue> locale;
    std::optional<std::uint32_t> inheritedFontMask;
    std::optional<std::uint32_t> inheritedPaletteMask;
    // Bind these to the existing actual WidgetState fields; later helpers read
    // the live state rather than an independent copy of the parent flags.
    std::optional<std::uint32_t> attributesLow;
    std::optional<std::array<std::uint32_t, 3>> extendedAttributes;
    std::optional<std::uint32_t> windowFlags;
    std::optional<bool> opaqueChildrenDirty; // private+1d8 bit16
    std::optional<bool> opaque;              // private+1d8 bit17
    std::optional<bool> graphicsEffectPresent; // private+d8; true remains guarded
    std::optional<bool> autoFillBackground;    // extra+5c bit4
    std::optional<bool> stylePresent;          // extra+20
    std::optional<std::int32_t> stylesheetLength;
    WidgetReceiverEventObservation events;
    std::optional<std::vector<WidgetPendingChildObservation>> pendingChildren;
    std::optional<bool> proxyPresent;          // extra+10, distinct from topExtra
};

struct WidgetRuntimeValueObservation {
    std::weak_ptr<MainViewState> mainView;
    std::weak_ptr<ViewManagerState> manager;
    std::string qualification; // exact accepted receipt/role/state binding
    std::vector<WidgetFontDataObservation> fontData;
    std::vector<WidgetPaletteDataObservation> paletteData;
    WidgetApplicationFontObservation applicationFont;
    WidgetApplicationPaletteObservation applicationPalette;
    WidgetInitialValueObservation mainViewValues;
    WidgetInitialValueObservation managerValues;
    // This is the QLocale value observed in the fresh QWidgetPrivate constructor
    // role; it is not inferred from the current MainView locale.
    std::optional<WidgetLocaleValue> freshPrivateLocale;
    WidgetApplicationEventObservation applicationEvents;
    WidgetReceiverEventObservation freshWidgetEvents;
    std::optional<bool> freshSendChildEvents;
    std::optional<bool> freshFieldD0Present;
    std::optional<bool> applicationModalState;
    std::optional<std::int32_t> applicationStylesheetLength;
};

class WidgetFontDataState final {
public:
    std::uint32_t observationAlias() const noexcept { return alias_; }
    std::optional<std::uint32_t> comparisonField38() const noexcept { return field38_; }
    std::optional<std::uint32_t> deviceScreenField3c() const noexcept { return field3c_; }
private:
    friend class WidgetRuntimeState;
    WidgetFontDataState(std::uint32_t a, std::optional<std::uint32_t> f,
                       std::optional<std::uint32_t> s = {}) : alias_(a), field38_(f), field3c_(s) {}
    std::uint32_t alias_;
    std::optional<std::uint32_t> field38_;
    std::optional<std::uint32_t> field3c_;
};
class WidgetPaletteDataState final {
public:
    std::uint32_t observationAlias() const noexcept { return alias_; }
private:
    friend class WidgetRuntimeState;
    explicit WidgetPaletteDataState(std::uint32_t a) : alias_(a) {}
    std::uint32_t alias_;
};
struct WidgetFontValue {
    std::shared_ptr<const WidgetFontDataState> data;
    std::uint32_t resolveMask = 0;
};
struct WidgetPaletteValue {
    std::shared_ptr<const WidgetPaletteDataState> data;
    std::uint32_t groupResolveWord = 0;
};
struct WidgetPrivateConstructionObservation {
    std::string qualification;
    std::vector<WidgetFontDataObservation> fontData;
    std::vector<WidgetPaletteDataObservation> paletteData;
    std::optional<WidgetFontValueObservation> defaultFont;
    std::optional<WidgetPaletteValueObservation> defaultPalette;
    std::optional<WidgetLocaleValue> defaultLocale;
    std::optional<std::int32_t> applicationType, applicationLayoutDirection;
    std::optional<bool> applicationModalState;
};
struct WidgetPrivateConstructionInputs {
    std::string qualification;
    std::optional<WidgetFontValue> defaultFont;
    std::optional<WidgetPaletteValue> defaultPalette;
    std::optional<WidgetLocaleValue> defaultLocale;
};
enum class WidgetPaintDeviceKind { Unknown, QWidget };
struct WidgetMetricInputObservation {
    std::weak_ptr<WidgetState> widget;
    WidgetPaintDeviceKind kind = WidgetPaintDeviceKind::Unknown;
    std::optional<std::uint16_t> dpiYOverride;
    std::optional<bool> parentIsNull;
};
struct WidgetConstructorTailObservation {
    std::string qualification;
    WidgetFontAlias expectedFontAlias = 0;
    std::optional<std::uint32_t> fontScreenField3c, displayDcAlias;
    std::optional<std::int32_t> displayLogicalDpiY;
    std::optional<bool> currentQThreadIsApplicationQThread;
    std::vector<WidgetMetricInputObservation> metricInputs;
    std::optional<std::uint32_t> currentWidgets, maximumWidgets, notifyDepth;
    std::optional<std::int32_t> applicationCompileVersion;
    std::optional<std::uint32_t> applicationAttributes;
};
// This is the counter effect record for one actual native QObject. It does not
// stand in for QObject construction, thread binding, parenthood or destruction.
class QObjectConstructionRecord final {
public:
    explicit QObjectConstructionRecord(const void* nativeIdentity) : identity_(nativeIdentity) {}
    QObjectConstructionRecord(const QObjectConstructionRecord&) = delete;
    QObjectConstructionRecord& operator=(const QObjectConstructionRecord&) = delete;
    const void* nativeIdentity() const noexcept { return identity_; }
    bool committed() const noexcept { return committed_; }
private:
    friend class WidgetRuntimeState;
    const void* identity_;
    bool committed_ = false;
    std::weak_ptr<WidgetRuntimeState> runtime_;
};
// Opaque earlier native children remain evidence identities only; known entries
// are weak links to actual components. Neither entry owns a QObject child.
struct WidgetPendingChild {
    std::uint32_t opaqueAlias = 0;
    std::weak_ptr<WidgetState> knownWidget;
};


// A component of its native owner, not a separately parented QObject. The
// identity is the actual MainView/ViewManager/SplitView owner address. Existing
// widgets have unknown Qt data until qualified; fresh fields are committed in
// the recovered constructor order.
class WidgetState final {
public:
    WidgetState(const WidgetState&) = delete;
    WidgetState& operator=(const WidgetState&) = delete;
    const void* nativeIdentity() const noexcept { return identity_; }
    bool nativeOwnerAlive() const noexcept { return alive_; }
    std::shared_ptr<WidgetRuntimeState> runtimeOwner() const noexcept { return runtime_.lock(); }
    std::shared_ptr<WidgetThreadState> threadState() const noexcept { return thread_; }
    std::shared_ptr<GridLayoutState> installedLayout() const noexcept;
    std::optional<bool> layoutPresence() const noexcept;
    std::optional<bool> receivesChildEvents() const noexcept { return receivesChildEvents_; }
    void observeReceivesChildEvents(bool);
    const std::optional<qt::String>& objectName() const noexcept { return objectName_; }
    void observeObjectName(const qt::String&);
    void assignObjectName(const qt::String&);
    std::optional<bool> attributeBit(unsigned raw) const;
    void observeAttributeBit(unsigned raw, bool);
    void assignAttributeBit(unsigned raw, bool);
    void observeGeometryEndpoints(const std::array<int, 4>&);
    void assignGeometryEndpoints(const std::array<int, 4>&);
    const WidgetDynamicBooleanProperties& dynamicBooleanProperties() const noexcept { return properties_; }
    void observeEmptyDynamicBooleanProperties(bool extraDataAllocated);
    void allocateDynamicBooleanProperties();
    void appendDynamicPropertyName(std::string);
    void appendDynamicPropertyBoolean(bool);
    void observeEventReceiver(const WidgetReceiverEventObservation&, WidgetEventReceiverKind,
                              const std::string& qualification);
    std::shared_ptr<WidgetState> parentWidget() const noexcept;
    std::optional<std::uint32_t> attributesLow() const noexcept { return attributesLow_; }
    const std::optional<std::array<std::uint32_t, 3>>& extendedAttributes() const noexcept { return extendedAttributes_; }
    std::optional<std::uint32_t> windowFlags() const noexcept { return windowFlags_; }
    const std::optional<std::array<int, 4>>& geometryEndpoints() const noexcept { return geometry_; }
    bool widgetDataAssigned() const noexcept { return widgetDataAssigned_; }
    bool inSetParent() const noexcept { return inSetParent_; }
    bool parentOwnsChild() const noexcept { return parentOwnsChild_; }
    const void* focusNextIdentity() const noexcept { return focusNext_; }
    const void* focusPreviousIdentity() const noexcept { return focusPrevious_; }
    bool focusNextKnown() const noexcept { return focusNextKnown_; }
    bool focusPreviousKnown() const noexcept { return focusPreviousKnown_; }
    // This known suffix is not the whole QObject list when prefixUnrecovered
    // is true. It is separate from splitter records and pending-child events.
    const std::optional<WidgetFontValue>& font() const noexcept { return font_; }
    const std::optional<WidgetPaletteValue>& palette() const noexcept { return palette_; }
    const std::optional<WidgetLocaleValue>& locale() const noexcept { return locale_; }
    std::optional<bool> opaqueChildrenDirty() const noexcept { return opaqueChildrenDirty_; }
    std::optional<bool> opaque() const noexcept { return opaque_; }
    const std::optional<std::vector<WidgetPendingChild>>& pendingChildren() const noexcept { return pendingChildren_; }
    bool childPrefixUnrecovered() const noexcept { return childPrefixUnrecovered_; }
    const std::vector<const void*>& knownChildSuffix() const noexcept { return childSuffix_; }
private:
    friend class simcontrol300::StagedEditor300;
    friend class simcontrol300::EditorGridConstruction;
    friend class simcontrol300::EditorBaseConstruction;
    friend class WidgetEventDispatch;
    friend class WidgetConstructorTail;
    friend class MainViewState;
    friend class ViewManagerState;
    friend class SplitViewState;
    friend class WidgetRuntimeState;
    explicit WidgetState(const void* identity) : identity_(identity), objectConstruction_(identity) {}
    bool matchesComponentOwner(const std::shared_ptr<WidgetState>&) const noexcept;
    void releaseNativeOwner() noexcept;
    void assignInstalledLayout(const std::shared_ptr<GridLayoutState>&);
    const void* identity_;
    QObjectConstructionRecord objectConstruction_;
    std::weak_ptr<WidgetState> componentOwner_;
    bool alive_ = true;
    bool widgetDataAssigned_ = false;
    bool inSetParent_ = false;
    bool parentOwnsChild_ = false;
    bool parentBindingKnown_ = false;
    bool childPrefixUnrecovered_ = false;
    std::weak_ptr<WidgetState> parent_;
    std::weak_ptr<WidgetRuntimeState> runtime_;
    bool runtimeWasBound_ = false;
    std::shared_ptr<WidgetThreadState> thread_;
    std::weak_ptr<GridLayoutState> installedLayout_;
    bool layoutAliasAssigned_ = false;
    std::optional<bool> receivesChildEvents_;
    WidgetPaintDeviceKind paintDeviceKind_ = WidgetPaintDeviceKind::Unknown;
    std::optional<std::uint16_t> dpiYOverride_;
    std::optional<qt::String> objectName_;
    // Sparse bits are used only when the corresponding full word is unknown.
    std::array<std::uint32_t, 4> partialAttributeKnown_{}, partialAttributeBits_{};
    WidgetDynamicBooleanProperties properties_;
    WidgetEventReceiverKind eventReceiverKind_ = WidgetEventReceiverKind::Unknown;
    std::optional<std::uint32_t> attributesLow_;
    std::optional<std::array<std::uint32_t, 3>> extendedAttributes_;
    std::optional<std::uint32_t> windowFlags_;
    std::optional<std::array<int, 4>> geometry_;
    const void* focusNext_ = nullptr;
    const void* focusPrevious_ = nullptr;
    bool focusNextKnown_ = false;
    bool focusPreviousKnown_ = false;
    std::vector<const void*> childSuffix_;
    std::optional<WidgetFontValue> font_;
    std::optional<WidgetPaletteValue> palette_;
    std::optional<WidgetLocaleValue> locale_;
    std::optional<std::uint32_t> inheritedFontMask_, inheritedPaletteMask_;
    std::optional<bool> opaqueChildrenDirty_, opaque_, graphicsEffectPresent_;
    std::optional<bool> autoFillBackground_, stylePresent_, proxyPresent_;
    std::optional<std::int32_t> stylesheetLength_;
    WidgetReceiverEventObservation events_;
    std::optional<bool> sendChildEvents_, fieldD0Present_;
    std::optional<std::vector<WidgetPendingChild>> pendingChildren_;

};

// Partial membership projection of QApplication's actual allWidgets set. No
// total population/count or iteration order is asserted. Invalid diagnostic
// leases cannot remain live registrations; physical Qt cleanup is unclosed.
class WidgetRuntimeState final : public std::enable_shared_from_this<WidgetRuntimeState> {
public:
    WidgetRuntimeState(std::optional<bool> allWidgetsRegistryPresent, std::string qualification);
    WidgetRuntimeState(const WidgetRuntimeState&) = delete;
    WidgetRuntimeState& operator=(const WidgetRuntimeState&) = delete;
    std::optional<bool> allWidgetsRegistryPresent() const noexcept { return registryPresent_; }
    const std::string& qualification() const noexcept { return qualification_; }
    bool hasLiveRegistration(const void* nativeIdentity) const noexcept;
    bool populationComplete() const noexcept { return false; }
    void observeStartupValues(const WidgetRuntimeValueObservation&);
    void observePrivateConstructionInputs(const std::shared_ptr<WidgetState>&,
                                         const WidgetPrivateConstructionObservation&);
    const WidgetPrivateConstructionInputs* privateConstructionInputs(const WidgetState&) const noexcept;
    void observeConstructorTailValues(const std::shared_ptr<WidgetState>&,
                                      const WidgetConstructorTailObservation&);
    std::optional<std::int32_t> applicationType() const noexcept;
    std::optional<std::int32_t> applicationLayoutDirection() const noexcept;
    std::optional<bool> applicationModalState() const noexcept;
    void observeQObjectCount(std::uint32_t, const std::string& qualification);
    std::optional<std::uint32_t> qobjectCount() const noexcept;
    std::uint32_t retainedQObjectConstructionDelta() const noexcept { return qobjectDelta_; }
    void commitQObjectConstruction(QObjectConstructionRecord&);
    void commitQObjectConstruction(WidgetState&);
    void commitQObjectConstruction(GridLayoutState&);
    std::optional<std::uint32_t> currentWidgetCount() const noexcept { return currentWidgets_; }
    std::optional<std::uint32_t> maximumWidgetCount() const noexcept { return maximumWidgets_; }
    std::optional<WidgetFontValue> applicationFont() const;
    std::optional<WidgetPaletteValue> applicationPalette() const;
    // This binding does not register a widget or certify any constructor stage.
    void bindWidgetComponent(const std::shared_ptr<WidgetState>&);
    std::shared_ptr<WidgetThreadState> threadState() const noexcept { return thread_; }
    void observeApplicationEvents(const WidgetApplicationEventObservation&, const std::string& qualification);
    const WidgetApplicationEventObservation& applicationEvents() const noexcept { return applicationEvents_; }

private:
    friend class MainViewState;
    friend class ViewManagerState;
    friend class WidgetEventDispatch;
    friend class WidgetConstructorTail;
    friend class simcontrol300::EditorBaseConstruction;
    friend class simcontrol300::EditorGridConstruction;
    void bind(const std::shared_ptr<WidgetState>&);
    void registerWidget(const std::shared_ptr<WidgetState>&);
    std::optional<bool> registryPresent_;
    std::string qualification_;
    struct Registration { const void* identity; std::weak_ptr<WidgetState> widget; };
    std::vector<Registration> knownRegistrations_;
    struct Values;
    std::shared_ptr<Values> values_;
    // One application event context and one actual qualified thread owner are
    // shared by startup widgets and the later native editor component.
    WidgetApplicationEventObservation applicationEvents_;
    std::shared_ptr<WidgetThreadState> thread_;
    std::optional<std::uint32_t> qobjectBase_, currentWidgets_, maximumWidgets_;
    std::uint32_t qobjectDelta_ = 0;
    std::optional<std::uint32_t> displayDcAlias_;
    std::optional<std::int32_t> displayLogicalDpiY_;
    std::optional<bool> currentQThreadIsApplicationQThread_;
    std::optional<std::int32_t> applicationCompileVersion_;
    std::optional<std::uint32_t> applicationAttributes_;

};

enum class SplitViewConstructStage {
    Absent, WidgetRegistryPending, WidgetParentKindPending,
    ParentNativeAttributePending, ParentNativeAttributeEffectPending,
    ParentThreadPending, ParentHookPending, ParentNativeCreationPending,
    ParentVisibilityPending, ParentDropSitePending, ParentDropSiteEffectPending,
    ParentBackingStorePending, ParentCompatibilityPending,
    ParentCompatibilityEffectPending, ParentFocusPending, ParentFontPreparationPending,
    ParentPalettePreparationPending, ParentPaletteResolvePending, ParentDirectionPending,
    ParentLocalePending, ParentEnabledPending, ParentStylePending, ParentChildAddedPending,
    ParentPendingChildrenPending, ParentChangeEventPending, ParentHiddenPending,
    ParentDirtyEffectsPending, ParentDirtyWindowPending, ParentOpacityPending, ParentOpacityEffectPending,
    ParentProxyPending, WidgetFontDevicePending, WidgetCreatePending, WidgetPostPending
};

struct SplitViewConstructionReport {
    SplitViewConstructStage stage = SplitViewConstructStage::Absent;
    bool completed = false;
    std::string issue;
};

struct SplitViewStartupObservation {
    std::weak_ptr<ViewManagerState> receiver;
    std::string qualification;
    // MainView extra+5c bit5; absent extra would establish false, but the
    // captured MainView has extra and this particular bit was not recorded.
    std::optional<bool> parentExtraNativeChildren;
    std::optional<std::uint32_t> parentAttributesLow;
    std::optional<std::array<std::uint32_t, 3>> parentExtendedAttributes;
    std::optional<std::uint32_t> parentWindowFlags;
    std::optional<bool> parentThreadMatches;
    std::optional<bool> childHookDataPresent;
    std::optional<bool> parentTopExtraPresent;
    std::optional<std::int32_t> applicationCompileVersion;
    std::optional<std::uint32_t> applicationAttributes;
};

class SplitViewState final {
public:
    ~SplitViewState();
    SplitViewState(const SplitViewState&) = delete;
    SplitViewState& operator=(const SplitViewState&) = delete;
    std::shared_ptr<WidgetState> widgetState() const noexcept { return widget_; }
    bool nativeOwnerAlive() const noexcept { return widget_->nativeOwnerAlive(); }
    const WidgetConstructorTail* constructorTail() const noexcept { return tail_.get(); }
    SplitViewConstructStage constructionStage() const noexcept { return stage_; }
    // Assigned only after the QWidget/QFrame constructor returns.
    std::optional<int> orientation() const noexcept { return orientation_; }
    // Dedicated SfSplitView QString+28, constructed after QSplitter returns;
    // this is distinct from QObject's objectName and absent during its base.
    const std::optional<qt::String>& splitViewName() const noexcept { return name_; }
private:
    friend class MainViewState;
    friend class ViewManagerState;
    SplitViewState();
    void releaseNativeOwner() noexcept { widget_->releaseNativeOwner(); }
    std::shared_ptr<WidgetState> widget_;
    SplitViewConstructStage stage_ = SplitViewConstructStage::WidgetRegistryPending;
    std::optional<int> orientation_;
    std::unique_ptr<WidgetConstructorTail> tail_;
    std::optional<qt::String> name_;
    std::string qualification_;
    std::optional<WidgetPaletteValue> preparedPalette_;
    bool paletteClassMatchesDefault_ = false;
};

// Explicit current observations for the recovered fresh-startup role. These
// qualify pure native branches; they are not another event-filter registry.
// Missing fields remain unknown. In particular, an empty application-filter
// list must not replace the one live Vista-style filter actually observed.
struct GridApplicationFilter {
    enum class Kind { Unknown, WindowsVistaStyle };
    Kind kind = Kind::Unknown;
    std::optional<bool> sameApplicationThread;
};

struct GridStartupObservation {
    std::weak_ptr<ViewManagerState> receiver;
    std::string qualification;
    std::optional<bool> applicationPresent;
    std::optional<bool> parentThreadMatches;
    std::optional<std::size_t> internalCallbackCount;
    std::optional<std::vector<GridApplicationFilter>> applicationFilters;
    std::optional<std::size_t> objectFilterCount;
    std::optional<bool> declarativeChildEventsEnabled;
    std::optional<bool> childHookDataPresent;
    std::optional<std::uint32_t> currentLayoutFlags;
    std::optional<std::uint32_t> currentWindowFlags;
};

// Adapter suspension stages, not extra native state or notification counters.
enum class GridInstallStage {
    Absent, ChildAddedPending, PostParentHooksPending, UpdatePending,
    WindowTailPending, Cached
};

// Only the empty constructor population is recovered here. Later concrete
// layout items must enter this actual owner collection when their effects close.
class GridLayoutItemState {
public:
    virtual ~GridLayoutItemState() = default;
protected:
    GridLayoutItemState() = default;
};

enum class GridConstructionProfile { ManagerReady, ParentedPending };
class GridLayoutState final {
public:
    static std::shared_ptr<GridLayoutState> beginParentedConstruction(const std::shared_ptr<WidgetState>& proposedParent);
    GridConstructionProfile constructionProfile() const noexcept { return profile_; }
    bool nativeOwnerAlive() const noexcept;
    std::shared_ptr<WidgetState> parentWidget() const noexcept { return parentWidget_.lock(); }
    std::shared_ptr<WidgetThreadState> threadState() const noexcept { return thread_; }
    std::shared_ptr<ViewManagerState> parentViewManager() const noexcept { return parent_.lock(); }
    const std::array<int, 4>& contentsMargins() const noexcept { return margins_; }
    int rowCount() const noexcept { return rows_; }
    int columnCount() const noexcept { return columns_; }
    bool isTopLevelLayout() const noexcept { return topLevel_; }
    std::uint32_t layoutFlags() const noexcept { return layoutFlags_; }
    std::uint32_t dirtyFlags() const noexcept { return dirtyFlags_; }
    // The pending parented ctor initializes only these masks. Returned values
    // must not be interpreted as complete native words when a mask is partial.
    std::uint32_t layoutFlagsKnownMask() const noexcept { return layoutFlagsKnownMask_; }
    std::uint32_t dirtyFlagsKnownMask() const noexcept { return dirtyFlagsKnownMask_; }
    int cachedSizeValue() const noexcept { return cachedSizeValue_; }
    // QRect stores endpoints here, not width and height. This is layout
    // geometry only; it does not establish any scene or viewport rectangle.
    const std::array<int, 4>& geometryEndpoints() const noexcept { return geometryEndpoints_; }
    const std::vector<std::unique_ptr<GridLayoutItemState>>& items() const noexcept { return items_; }
private:
    friend class ViewManagerState;
    friend class WidgetRuntimeState;
    friend class simcontrol300::EditorGridConstruction;
    friend class WidgetEventState;
    explicit GridLayoutState(GridConstructionProfile = GridConstructionProfile::ManagerReady);
    void releaseNativeOwner() noexcept;
    GridConstructionProfile profile_;
    std::weak_ptr<GridLayoutState> allocationOwner_;
    QObjectConstructionRecord objectConstruction_{this};
    std::weak_ptr<ViewManagerState> parent_;
    std::weak_ptr<WidgetState> parentWidget_;
    bool parentWasAssigned_ = false;
    std::shared_ptr<WidgetThreadState> thread_;
    bool alive_ = true;
    bool topLevel_ = false;
    std::array<int, 4> margins_{{-1, -1, -1, -1}};
    int rows_ = 1;
    int columns_ = 1;
    // Parentless zero-margin invalidate establishes these before installation.
    std::uint32_t layoutFlags_ = 2; // QLayoutPrivate+9c, distinct from dirtyFlags
    std::uint32_t dirtyFlags_ = 4; // QGridLayoutPrivate+134
    std::uint32_t layoutFlagsKnownMask_ = 0xffffffffu;
    std::uint32_t dirtyFlagsKnownMask_ = 0xffffffffu;
    int cachedSizeValue_ = -1; // QGridLayoutPrivate+108
    std::array<int, 4> geometryEndpoints_{{0, 0, -1, -1}};
    const void* menuWidget_ = nullptr; // QLayoutPrivate+b8, fresh empty menu
    std::vector<std::unique_ptr<GridLayoutItemState>> items_;
};

// The separate private allocation addressed through SfMoMainView+30. Its own
// +30 pointer refers to the manager; it is not a second manager or QObject owner.
class MainViewPrivateState final {
public:
    std::shared_ptr<ViewManagerState> manager() const noexcept;
private:
    friend class MainViewState;
    std::weak_ptr<ViewManagerState> manager_;
};

// SfViewManager d34730: QWidget child of the actual SfMoMainView. This prefix
// establishes identity, empty layout name/null layout guard and registrations.
// It does not claim the later View_Default layout/children were constructed.
class ViewManagerState final {
public:
    ViewManagerState(const ViewManagerState&) = delete;
    ViewManagerState& operator=(const ViewManagerState&) = delete;
    const qt::String& layoutName() const noexcept { return layoutName_; }
    std::shared_ptr<MainViewState> parentMainView() const noexcept { return parent_.lock(); }
    // A diagnostic shared_ptr may outlive the native QObject parent. It must
    // never make the destroyed native receiver dispatchable.
    bool nativeReceiverAlive() const noexcept { return alive_ && !parent_.expired(); }
    bool nativeLayoutResolved() const noexcept { return false; }
    std::shared_ptr<WidgetState> widgetState() const noexcept { return widget_; }
    SplitViewConstructionReport ensureDefaultSplitView(const SplitViewStartupObservation&);
    std::shared_ptr<SplitViewState> pendingSplitView() const noexcept { return pendingSplitView_; }
    void registerConstructorSignals(doe::SessionNotificationState&);
    // d34d50/d34da0 fresh path. Commits the QWidget layout pointer and actual
    // QObject child ownership, then refuses the unresolved parent-event tail.
    // The separate manager+28 guard is assigned only after native return.
    std::shared_ptr<GridLayoutState> ensureGridLayout();
    // Continue the same installed/owned grid through explicitly qualified
    // normal callbacks and the setLayout tail. A later guard retains its stage.
    std::shared_ptr<GridLayoutState> ensureGridLayout(const GridStartupObservation&);
    std::shared_ptr<GridLayoutState> installedWidgetLayout() const noexcept;
    std::shared_ptr<GridLayoutState> cachedGridLayout() const noexcept;
    const std::vector<std::shared_ptr<GridLayoutState>>& ownedGridLayouts() const noexcept { return gridChildren_; }
    GridInstallStage gridInstallStage() const noexcept { return gridInstallStage_; }
    bool gridParentTransferPending() const noexcept {
        return gridInstallStage_ != GridInstallStage::Absent && gridInstallStage_ != GridInstallStage::Cached;
    }
    const std::string& gridContinuationQualification() const noexcept { return gridQualification_; }
    // d34d50 on the source-bound fresh null layout enters allocation, not an
    // empty split-view result. Keep this exact pending dependency explicit.
    void requireLayoutForCurrentLookup() const;
private:
    friend class MainViewState;
    explicit ViewManagerState(const std::shared_ptr<MainViewState>& parent);
    std::shared_ptr<GridLayoutState> beginGridInstall();
    void releaseNativeOwner() noexcept;
    std::weak_ptr<MainViewState> parent_;
    std::weak_ptr<ViewManagerState> self_;
    bool alive_ = true;
    qt::String layoutName_; // native QString default (null/empty)
    std::weak_ptr<GridLayoutState> cachedLayout_; // SfViewManager guarded+28
    std::vector<std::shared_ptr<GridLayoutState>> gridChildren_; // actual QObject grid children
    GridInstallStage gridInstallStage_ = GridInstallStage::Absent;
    std::string gridQualification_;
    std::shared_ptr<WidgetState> widget_;
    std::shared_ptr<SplitViewState> pendingSplitView_; // retained constructor lease, not QObject parent
};

class MainViewState final : public std::enable_shared_from_this<MainViewState> {
public:
    MainViewState();
    ~MainViewState();
    void releaseNativeOwner() noexcept;
    bool nativeOwnerAlive() const noexcept { return bool(private_); }
    MainViewState(const MainViewState&) = delete;
    MainViewState& operator=(const MainViewState&) = delete;
    // 0e9400 allocation/d34730 occurs before mainwindow+148 binding and before
    // writing the same manager into the separate private allocation.
    std::shared_ptr<ViewManagerState> constructManagerChild(doe::SessionNotificationState&);
    void commitPrivateManagerLink(const std::shared_ptr<ViewManagerState>&);
    const MainViewPrivateState& privateState() const;
    std::shared_ptr<ViewManagerState> managerChild() const noexcept { return managerChild_; }
    std::shared_ptr<WidgetState> widgetState() const noexcept { return widget_; }
    void bindWidgetRuntime(const std::shared_ptr<WidgetRuntimeState>&);
    // Observed initial tail only: this window.previous is its actual manager,
    // and manager.next is this window. The other links remain unqualified.
    // Must bind before any pending SplitView is allocated.
    void observeInitialManagerFocusTail(std::string qualification);
    const std::vector<std::shared_ptr<SplitViewState>>& ownedSplitViews() const noexcept { return splitViewChildren_; }
private:
    friend class ViewManagerState;
    std::unique_ptr<MainViewPrivateState> private_;
    std::shared_ptr<ViewManagerState> managerChild_; // QObject child ownership projection
    std::shared_ptr<WidgetState> widget_;
    std::vector<std::shared_ptr<SplitViewState>> splitViewChildren_;
    std::string initialFocusTailQualification_;
};
} // namespace m2m::engine
