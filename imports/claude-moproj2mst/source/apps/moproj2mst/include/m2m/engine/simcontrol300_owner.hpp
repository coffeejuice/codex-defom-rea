// Parentless editor factory record, retained by the mainwindow widget-area
// owner. FormingClass only requests construction; it does not own this editor.
#pragma once
#include "m2m/engine.hpp"
#include "m2m/engine/editor_setup.hpp"
#include "m2m/engine/editor_grid.hpp"
#include "m2m/engine/editor_base.hpp"
#include "m2m/engine/operation_info_guard.hpp"
#include "m2m/engine/simcontrol300_lifecycle.hpp"

namespace m2m::engine { class WidgetState; class WidgetRuntimeState; }

namespace m2m::engine::simcontrol300 {

// Each call supplies the actual current identities. No absent runtime context
// is converted into a default model, operation, project or remesh copy.
struct Editor300Binding {
    Project& project;
    Operation& operation;
    GuiModel& model;
    RemeshState& remesh;
};

enum class Editor300ConstructorState { InvalidBinding, AwaitingContext,
    PageConstruction, PageRefused, AwaitingNativeBoundaries };
enum class Editor300BindingIssue { None, IdentityMismatch, ContextMismatch,
    SetupOwnerExpired, SetupPrerequisite, NativeSetupPrerequisite };
enum class Editor300SetupBindIssue { None, InvalidFactory, AlreadyBound,
    LateBinding, SetupAlreadyOwned, NativeBaseIncomplete };
struct Editor300ConstructionReport {
    Editor300ConstructorState state = Editor300ConstructorState::AwaitingContext;
    Editor300BindingIssue bindingIssue = Editor300BindingIssue::None;
    Editor300SetupBindIssue setupBindIssue = Editor300SetupBindIssue::None;
    std::optional<SetupPageHandoffReport> setupHandoff;
    std::optional<InitialOpenReport> pages;
    bool repeatedConstructionRejected = false;
    bool exceptionInterrupted = false;
    // Completing recovered page blocks does not implement the intervening
    // constructor callbacks, final native tail, area attachment or Save receiver.
    static constexpr bool completeForPublicSave = false;
};

enum class StepCountToggleStage { Preflight, OperationFlagStored, Complete };
struct StepCountToggleReport {
    ApplyResult outcome = ApplyResult::UnsupportedSnapshot;
    StepCountToggleStage stage = StepCountToggleStage::Preflight;
    bool operationFlagStored = false, pageRefilled = false;
};

enum class EnvironmentReactionStage { Preflight, ControlsProjected, Complete };
struct EnvironmentReactionReport {
    ApplyResult outcome = ApplyResult::UnsupportedSnapshot;
    EnvironmentReactionStage stage = EnvironmentReactionStage::Preflight;
    bool controlsProjected = false, modelUpdated = false;
};

class StagedEditor300 final {
public:
    explicit StagedEditor300(Editor300Binding);
    ~StagedEditor300();
    // One native editor component; a diagnostic lease does not prolong native
    // ownership. Allocation/binding alone proves no QWidget constructor stage.
    std::shared_ptr<WidgetState> widgetState() const noexcept { return widget_; }
    bool bindWidgetRuntime(Editor300Binding, const std::shared_ptr<WidgetRuntimeState>&);
    EditorBaseReport continueNativeBase(Editor300Binding, const EditorBaseContext&);
    const EditorBaseConstruction* nativeBaseConstruction() const noexcept { return base_.get(); }
    StagedEditor300(const StagedEditor300&) = delete;
    StagedEditor300& operator=(const StagedEditor300&) = delete;
    bool identityMatches(Editor300Binding) const noexcept;
    const Editor300ConstructionReport& construction() const noexcept { return report_; }
    // Preserve native allocation order: create factory, create setup, then bind.
    // One shot; a destroyed setup is not replaced and never becomes an unbound
    // isolated helper. Setup must remain alive until its handoff is attempted.
    bool bindSetupOwner(EditorSetupState&) noexcept;
    const EditorSetupState* setupOwner() const noexcept { return setup_.get(); }
    bool setupWasBound() const noexcept { return setupBound_; }
    // Original setup prefix on this exact factory/setup owner. The caller must
    // establish that the earlier QWidget/base constructor has returned. Unknown
    // current context stops at the reached operation and retains prior writes.
    NativeSetupPrefixReport beginNativeSetupPrefix(Editor300Binding,
        const NativeSetupObservation&);
    // Resume only the same pending stack event/runtime call frame. This never
    // reruns name/resize/property storage or certifies earlier base construction.
    NativeSetupPrefixReport continueNativeSetupPropertyNotification(Editor300Binding);
    EditorGridReport continueNativeSetupFirstGrid(Editor300Binding, const EditorGridContext&);
    const EditorGridConstruction* firstGridConstruction() const noexcept { return firstGrid_.get(); }
    const NativeSetupPrefixState* nativeSetupPrefix() const noexcept;
    std::optional<NativeSetupPropertyEvent> pendingNativeSetupEvent(
        Editor300Binding) const;
    // Explicit recovered page projection. One attempt only: a partial refusal
    // must never replay earlier stores/I/O on a subsequent call. An unbound
    // factory retains the explicitly isolated page-helper profile; production
    // Session binds setup and still gates earlier unresolved native stages.
    const Editor300ConstructionReport& constructPageState(Editor300Binding,
        Extra3DIo&, const InitialEditorContext&);
    InitialEditorSession* pageState() noexcept;
    const InitialEditorSession* partialPageState() const noexcept { return pages_.get(); }
    // Explicit private activated(bool) callback on the same bound editor.
    // The delivered value must already be present in the cached checkbox.
    StepCountToggleReport toggleNstart(
        Editor300Binding, const StepCountContext& currentContext, bool checked);
    // Explicit private title textChanged delivery on the same bound page.
    ApplyResult titleChanged(Editor300Binding, const qt::String& deliveredText);
    // Explicit private reaction-rate clicked delivery on the bound page.
    // Programmatic radio state projection does not invoke this callback.
    EnvironmentReactionReport reactionRateClicked(Editor300Binding, int selection);
    // Explicit private Environment constant/table clicked delivery. It
    // projects controls only; model curve writes belong to later Apply.
    ApplyResult environmentCurveClicked(Editor300Binding, int curveIndex,
        int selection);
    // Explicit private Advanced textEdited delivery for one bound frequency
    // row; the page's latest cached text must match the delivered value.
    ApplyResult frequencyTextEdited(Editor300Binding, int row,
        const qt::String& deliveredText, NumericLocale);
    // Explicit private Advanced currentIndexChanged delivery; cached combo
    // selection must already equal the delivered value.
    ApplyResult frequencyIndexChanged(Editor300Binding, int row, int selection);
    // Explicit private ViewFactor stateChanged delivery; projection of the
    // definition control is distinct from the later Apply model writes.
    ApplyResult viewFactorStateChanged(Editor300Binding, int state);
    // Explicit private solver radio toggled delivery; projection only.
    ApplyResult explicitSolverToggled(Editor300Binding, bool checked);
    static constexpr bool completeForPublicSave = false;
private:
    // Diagnostic identity snapshots do not own Operation/GuiModel allocations.
    // The OPI generation is independent of the possibly borrowed model guard.
    // Capturing it does not execute native130857's editor+38 assignment or
    // certify the preceding QWidget/base construction. Project/Operation raw
    // addresses remain tokens, dereferenced only through a live current binding.
    std::shared_ptr<WidgetState> widget_;
    Project* project_;
    Operation* operation_;
    OperationInfoGuard operationIdentity_;
    GuiModelGuard model_;
    RemeshState* remesh_;
    EditorSetupGuard setup_;
    bool setupBound_ = false;
    std::unique_ptr<InitialEditorSession> pages_;
    std::unique_ptr<EditorBaseConstruction> base_;
    std::unique_ptr<EditorGridConstruction> firstGrid_;
    Editor300ConstructionReport report_;
};

// Root integration retains this record at the original factory point before
// area delivery. Native delivery is 1405f4756 -> 1405f499f -> SfWidgetArea;
// replacing/deleting current widgets uses delayed ownership, not immediate reset.
// This class intentionally does not claim to implement those area effects.
} // namespace m2m::engine::simcontrol300
