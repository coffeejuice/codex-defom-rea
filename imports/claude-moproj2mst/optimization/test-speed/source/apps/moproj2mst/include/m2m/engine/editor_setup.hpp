// Recovered setupUi control identities and bounded state. This is not a Qt
// replacement or permission to pass the native constructor's setupUi boundary.
#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>
#include "m2m/qt4.hpp"
#include "m2m/engine/widget_events.hpp"

namespace m2m::engine { class WidgetState; }

namespace m2m::engine::simcontrol300 {

class EditorSetupState;
class StagedEditor300;
struct InitialEditorSession;
struct EditorSetupLifetime;
enum class StoppingLabelProfile;

// Observes setup lifetime without extending the native owner's lifetime.
class EditorSetupGuard final {
public:
    void reset(EditorSetupState* = nullptr) noexcept;
    EditorSetupState* get() const noexcept;
private:
    EditorSetupState* identity_ = nullptr;
    std::weak_ptr<EditorSetupLifetime> lifetime_;
};

using SetupControlId = std::uint16_t; // offset in original editor+140 UI record

struct SetupControlIdentity {
    SetupControlId id;
    std::string_view objectName;
};

enum class SetupProperty {
    Enabled, Checked, Checkable, Minimum, Maximum, Value, SingleStep,
    ColumnCount, CurrentIndex
};

struct SetupScalarCall {
    SetupControlId control;
    SetupProperty property;
    int value;
    bool conditionalColumnMinimum;
    std::uint64_t nativeReturnAddress;
};

struct SetupInternalConnection {
    SetupControlId sender, receiver;
};

struct SetupAutomaticSlot {
    SetupControlId sender;
    std::string_view signal, slot;
};

std::span<const SetupControlIdentity> editorSetupIdentities() noexcept;
std::span<const SetupScalarCall> editorSetupPrefixCalls() noexcept;
std::span<const SetupScalarCall> editorSetupFinalIndexCalls() noexcept;
std::span<const SetupInternalConnection> editorSetupInternalConnections() noexcept;
std::span<const SetupAutomaticSlot> editorSetupAutomaticSlots() noexcept;

struct SetupSpinState {
    int minimum, maximum, value, singleStep;
};

struct SetupControlState {
    SetupControlIdentity identity;
    // Requested local enable state, NOT effective state through the parent
    // tree. An unset value is unknown, not the Qt default true/false/zero.
    std::optional<bool> requestedEnabled, requestedChecked, checkable;
    // Source-resolved checked state; MDS groups require their explicit profile.
    std::optional<bool> checked;
    // Only the bound fresh MDS parent tree resolves these consumed enable
    // values. Generic requestedEnabled does not establish inherited state.
    std::optional<bool> enabled;
    std::optional<int> minimum, maximum, value, singleStep, columnCount;
    // Actual numeric state under the fresh native QSpinBox profile. The scalar
    // fields above retain explicit arguments and do not imply effective values.
    std::optional<SetupSpinState> spin;
    int columnCountLowerBound = 0;
    std::optional<qt::String> text;
    std::optional<std::vector<qt::String>> comboItems;
    // Actual selection only for the explicitly bound fresh default Qt model.
    // -1 is an invalid persistent model index; unset means unqualified model.
    std::optional<int> comboCurrentIndex;
    // An explicit setter argument; not a claim about an unmodeled widget's
    // effective selection or signal emission.
    std::optional<int> requestedCurrentIndex;
};

enum class SetupTranslationProfile { Unknown, NoActiveTranslator };
enum class SetupComboModelProfile { Unknown, FreshDefaultQtModel, CustomModel };
enum class SetupSpinProfile { Unknown, FreshNative, CustomControl };
enum class SetupStoppingProfile { Unknown, FreshNativeTree, CustomControls };
enum class SetupProgress {
    Prefix, TranslatingConsumedControls, ConsumedControlsTranslated,
    InternalConnections, FinalIndicesRecorded, AutomaticSlotsRecorded
};
enum class SetupAdvanceResult {
    Applied, UnknownTranslation, UnknownComboModel, UnsupportedComboModel,
    UnknownSpinControl, UnsupportedSpinControl, UnknownStoppingControl,
    UnsupportedStoppingControl, HistoricalState, WrongStage
};

enum class SetupPageHandoffIssue { None, AlreadySealed, WrongFactory, WrongStage,
    WrongDestination, DestinationStarted, UnknownComboModel, UnknownSpinControl,
    UnknownStoppingControl, UnknownTranslation, UnknownLabelProfile,
    UnresolvedControl };
struct SetupPageHandoffReport {
    SetupPageHandoffIssue issue = SetupPageHandoffIssue::None;
    SetupControlId control = 0;
    bool complete() const noexcept { return issue == SetupPageHandoffIssue::None; }
};

struct SetupEnableDelivery {
    SetupControlId sender, receiver;
    bool requestedEnabled;
};

// Current native editor context, qualified at setup entry. These are not Qt
// defaults: a fresh label or a startup observation does not establish them.
enum class NativeSetupWidgetExtra { Unknown, Absent, Present };
enum class NativeSetupMetaObject { Unknown, NativeStatic, DynamicOverride };
enum class NativeSetupPropertyTable { Unknown, AbsentExtraData, EmptyNativeTable,
    NonemptyNativeTable };
struct NativeSetupSizeLimits {
    std::int32_t minimumWidth, minimumHeight, maximumWidth, maximumHeight;
};
struct NativeSetupObservation {
    const StagedEditor300* receiver = nullptr;
    std::string qualification;
    std::optional<qt::String> objectName;
    std::optional<bool> declarativeHookDataPresent;
    std::optional<bool> widgetCreated;
    NativeSetupWidgetExtra widgetExtra = NativeSetupWidgetExtra::Unknown;
    std::optional<NativeSetupSizeLimits> sizeLimits;
    // Native QRect endpoints x1,y1,x2,y2, not x,y,width,height.
    std::optional<std::array<std::int32_t, 4>> geometryEndpoints;
    NativeSetupMetaObject metaObject = NativeSetupMetaObject::Unknown;
    NativeSetupPropertyTable propertyTable = NativeSetupPropertyTable::Unknown;
};
enum class NativeSetupPrefixStage { NotStarted, ObjectName, ObjectNameHook,
    ResizeCreatedState, ResizeWindow, ResizeConstraints, ResizeGeometry,
    PropertyMetaObject, PropertyTable, PropertyNotification, PropertyEventDestruction,
    PropertySetReturned };
enum class NativeSetupPrefixIssue { None, IdentityMismatch, MissingQualification,
    SetupOwnerUnavailable, WrongStage, HistoricalState, UnknownObjectName,
    UnknownNameHook, DeclarativeNameHook, UnknownCreatedState, CreatedWidget,
    UnknownConstraints, UnknownGeometry, UnknownMetaObject, DynamicMetaObject,
    UnknownPropertyTable, NonemptyPropertyTable, NotificationContext,
    WidgetOwnerUnavailable, ObservationMismatch, RuntimeOwnerUnavailable };
enum class NativeSetupPrefixEffect { ObjectNameAssigned, ResizeAttribute42Set,
    GeometrySizeAssigned, ResizeAttribute35Set, FalseVariantConstructed,
    DynamicExtraDataAllocated, PropertyNameAppended, PropertyValueAppended,
    PropertyEventConstructed, PropertyNotificationReturned, PropertyEventDestroyed,
    DynamicPropertySetterReturned, CallerFalseVariantDestroyed };
struct NativeSetupPrefixReport {
    NativeSetupPrefixStage stage = NativeSetupPrefixStage::NotStarted;
    NativeSetupPrefixIssue issue = NativeSetupPrefixIssue::None;
    bool attempted = false;
    bool repeatedAttemptRejected = false;
    bool repeatedNotificationRejected = false;
    static constexpr bool completeForNativeSetup = false;
};
struct NativeSetupPropertyEvent {
    // Opaque identity tokens, not dispatchable native pointers. Consumers must
    // revalidate the live factory binding; copying this description keeps no
    // QObject or event alive and does not certify successful delivery.
    const StagedEditor300* receiver = nullptr;
    const EditorSetupState* owner = nullptr;
    std::uint16_t type = 0xaa;
    std::string name;
    bool accepted = true;
    bool spontaneous = false;
    bool posted = false;
    bool privateDataNull = true;
    // Other native flag bits are uninitialized; no zero-filled raw flags or
    // synthetic event return value is exposed by this description.
    std::uint64_t notifyCallAddress = 0x6712e36f;
};
struct NativeSetupPrefixState {
    NativeSetupPrefixReport report;
    std::string qualification;
    // The actual factory component owns every live QObject/QWidget field.
    // This read-only view may retain historical values after native death;
    // nativeOwnerAlive() and the factory binding govern dispatchability.
    std::shared_ptr<const WidgetState> widgetState() const noexcept { return widget_.lock(); }
    bool falseVariantConstructed = false;
    bool falseVariantDestroyed = false;
    const WidgetEventState* eventState() const noexcept { return event_.get(); }
    const WidgetDispatchReport& notification() const noexcept { return dispatch_.report(); }
    std::optional<bool> originalSetterResult;
    // Attribute entries record source setters and the known resulting bits;
    // they do not assert the old bits were false or that a native OR was needed.
    std::vector<NativeSetupPrefixEffect> effects;
private:
    friend class EditorSetupState;
    std::weak_ptr<WidgetState> widget_;
    std::unique_ptr<WidgetEventState> event_;
    WidgetEventDispatch dispatch_;
};

class EditorSetupState final {
public:
    // Explicit scalar prefix before140f21eb0 retranslation. In particular the
    // two remesh radio groups retain source-proven initial checked state.
    EditorSetupState();
    ~EditorSetupState();
    EditorSetupState(const EditorSetupState&) = delete;
    EditorSetupState& operator=(const EditorSetupState&) = delete;
    // Constructor evidence only. After handoff these observations remain
    // historical; live widgets are exclusively in the retained page owner.
    const SetupControlState* constructorControl(SetupControlId) const noexcept;
    // Historical alias retained for existing diagnostic consumers.
    const SetupControlState* control(SetupControlId) const noexcept;

    // Qualify the21 plain QComboBox controls constructed by native setupUi.
    // Fresh default models have zero rows and invalid current indices. Custom
    // models and unqualified application-filter mutations are not substituted.
    // Binding is allowed once at Prefix, before any consumed translation.
    SetupAdvanceResult bindComboModels(SetupComboModelProfile);
    SetupComboModelProfile comboModelProfile() const noexcept;

    // Reconstruct the11 plain native QSpinBox numeric states at Prefix from
    // constructor defaults and ordered setup setters. Range changes clamp the
    // stored value before the next setter; no editor callback is fabricated.
    // Custom subclasses, wrapping and edited controls are outside this profile.
    SetupAdvanceResult bindSpinControls(SetupSpinProfile);
    SetupSpinProfile spinProfile() const noexcept;

    // Exact plain native MDS radio parents and their initially enabled tree;
    // custom controls or unqualified ancestor/filter changes stay unsupported.
    SetupAdvanceResult bindStoppingControls(SetupStoppingProfile);
    SetupStoppingProfile stoppingProfile() const noexcept;

    // The caller must qualify actual startup translator state. This profile
    // copies the13 recovered combo lists and16 numeric text operands only;
    // remaining labels/resources/defaults do not become supported implicitly.
    SetupAdvanceResult translateConsumedControls(SetupTranslationProfile);
    SetupProgress progress() const noexcept;
    std::size_t consumedTranslationActionsCompleted() const noexcept;

    // Isolated projection of the later internal connection stage, allowed only
    // after the explicit consumed-translation profile. It does not certify full
    // native retranslation, final indices, automatic
    // slots, resources, inherited parent enable state, or constructor completion.
    // A second installation is rejected; the original calls this stage once.
    bool installInternalConnectionProjection();
    bool internalConnectionProjectionInstalled() const noexcept;
    SetupAdvanceResult recordFinalIndexProjection();
    SetupAdvanceResult recordAutomaticSlotProjection();

    // Receive an already-emitted toggled(bool); do not invent its emission or
    // invoke other editor callbacks. Duplicate connections deliver twice, in
    // registration order. Calling before installation has no deliveries.
    // After handoff this throws logic_error: live events belong to page state.
    std::vector<SetupEnableDelivery> receiveInternalToggled(SetupControlId, bool);

    // One-way handoff of source-closed consumed stopping widgets. Validate all
    // prerequisites first; on success setup becomes immutable historical
    // evidence. This does not certify the earlier native constructor stages.
    SetupPageHandoffReport handoffToPages(const StagedEditor300&,
        InitialEditorSession&, StoppingLabelProfile);
    bool sealedForPages() const noexcept { return pageOwnerIdentity_ != nullptr; }
    const void* factoryOwnerIdentity() const noexcept { return factoryIdentity_; }
    const void* pageOwnerIdentity() const noexcept { return pageOwnerIdentity_; }
    // The sole native-prefix state for the bound editor. This is distinct from
    // the isolated control projections above and never certifies full setup.
    const NativeSetupPrefixState& nativePrefix() const noexcept { return nativePrefix_; }
    // UI[0] is a source alias, separate from QObject ownership and QWidget's
    // installed-layout alias. It stays null before the native grid returns.
    std::shared_ptr<const GridLayoutState> firstNativeGridUi() const noexcept { return firstNativeGridUi_.lock(); }

    static constexpr bool completeForNativeSetup = false;
    static constexpr bool completeForPublicSave = false;
private:
    friend class EditorSetupGuard;
    friend class StagedEditor300;
    bool bindFactoryOwner(const StagedEditor300&) noexcept;
    NativeSetupPrefixReport beginNativePrefix(const StagedEditor300&,
        const NativeSetupObservation&);
    NativeSetupPrefixReport continueNativePropertyNotification(const StagedEditor300&);
    bool destroyCallerFalseVariant(const StagedEditor300&);
    SetupControlState* mutableControl(SetupControlId) noexcept;
    std::shared_ptr<EditorSetupLifetime> lifetime_;
    // Identity tokens only; never dereferenced or used to extend ownership.
    const StagedEditor300* factoryIdentity_ = nullptr;
    const InitialEditorSession* pageOwnerIdentity_ = nullptr;
    std::vector<SetupControlState> controls_;
    bool internalConnectionsInstalled_ = false;
    SetupComboModelProfile comboModelProfile_ = SetupComboModelProfile::Unknown;
    SetupSpinProfile spinProfile_ = SetupSpinProfile::Unknown;
    SetupStoppingProfile stoppingProfile_ = SetupStoppingProfile::Unknown;
    SetupTranslationProfile translationProfile_ = SetupTranslationProfile::Unknown;
    SetupProgress progress_ = SetupProgress::Prefix;
    std::size_t translatedActions_ = 0;
    NativeSetupPrefixState nativePrefix_;
    std::weak_ptr<GridLayoutState> firstNativeGridUi_;
};

} // namespace m2m::engine::simcontrol300
