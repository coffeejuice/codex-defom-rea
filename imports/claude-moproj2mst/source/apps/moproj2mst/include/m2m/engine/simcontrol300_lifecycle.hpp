// Persistent initial editor300 page state. Original outer widget/view/project
// Save effects and external feature/remesh lifecycle bindings remain separate;
// this internal composer does not remove the public Save refusal.
#pragma once
#include "m2m/engine/simcontrol300_io.hpp"

namespace m2m::engine::simcontrol300 {

struct MoInitialObservations {
    // Original initial dispatch and backed-up refresh query independently.
    std::array<std::optional<MoFeatureAvailability>, 2> generalFills;
    StartupHeatingFeatures startupHeating;
};

struct InitialEditorContext {
    GeneralContext general;
    StepCountContext stepCount;
    NumericLocale numericLocale;
    bool pressureFieldsPopulated;
    // Required for actualMOprocType5. The bool above remains the explicit
    // isolatedprocType1 profile; it never supplies missingMOobservations.
    std::optional<MoInitialObservations> moObservations;
    // Supplied only from the actual setup translator qualification. Unknown
    // preserves explicitly seeded retained controls, never assumes English.
    StoppingLabelProfile stoppingLabelProfile = StoppingLabelProfile::Unknown;
};

struct InitialEditorSession {
    InitialEditorSnapshot pages{};
    Extra3DPage extra3d{};
    EnvironmentCurveTemporaries environmentCurves;
    // 140eb5630 and140ecb340 copy this even when its unvisited mode0 control
    // will not copy it back at Save. Retain real deep state, not a shallow DB copy.
    std::shared_ptr<const def::model::DYNARY> viewFactorCurve;
    int viewFactorDefinition = 0;
    GuiModel* sourceModel = nullptr;
    SimControl* sourceControl = nullptr;
    // Borrow the same process/session remesh owner used at ProjectOpen. This
    // is identity, not a snapshot; only page widgets remember their fill values.
    // The owner must outlive the editor, including any later Apply invocation.
    RemeshState* sourceRemesh = nullptr;
    bool constructionStarted = false;
    bool pageStateComplete = false;
    static constexpr bool completeForPublicSave = false;
};

// A reached-page receipt retains the partial owner instead of destroying it on
// an unsupported page. These are recovered page blocks; native callbacks between
// them remain separate dependencies, not implicitly completed constructor work.
enum class InitialOpenStage { Preflight, InitialFill, TemporaryCopy, RepeatFill,
    Backup, BackedRefresh, PageStateComplete };
enum class InitialOpenIssue { None, AlreadyStarted, InvalidModel,
    UnrecoveredNumericLocale, MissingMoObservation, UnsupportedPage };
struct InitialOpenReport {
    InitialOpenStage stage = InitialOpenStage::Preflight;
    InitialOpenIssue issue = InitialOpenIssue::None;
    std::optional<Page> reachedPage;
    std::array<bool, 9> initialPages{};
    std::array<bool, 2> repeatPages{};
    std::array<bool, 6> refreshedPages{};
    bool temporariesCopied = false;
    bool backupCreated = false;
    bool backupRestored = false;
    bool extra3DRefreshed = false;
    // Last reached stopping fill, including the current page on a refusal.
    std::optional<StoppingFillReport> stoppingFill;
    bool complete() const noexcept { return stage == InitialOpenStage::PageStateComplete; }
};

// Fresh destination only. A refusal retains completed pages, curve ownership and
// the reached original model/file effects. No replay after a partial attempt.
void openInitialEditorInto(InitialEditorSession&, GuiModel&, RemeshState&,
    Extra3DIo&, const InitialEditorContext&, InitialOpenReport&);

// 140eb26f0 ->140eb5630(page0), then140eb50f0: nine initial fills,
// repeated stopping/meshing, and140ecb340's six backed-up refills plus NBC
// overwrite. Context is explicit original widget state; capabilities are not
// guessed. Does not claim the later recovery-table UI or processEvents effects.
std::optional<InitialEditorSession> openInitialEditor(GuiModel&, RemeshState&,
    Extra3DIo&, const InitialEditorContext&);

struct InitialApplyReport {
    ApplyResult outcome = ApplyResult::Completed;
    // An absent result means that adapter refusal prevented reaching the page.
    std::array<std::optional<ApplyResult>, 9> pages;
    std::optional<ApplyResult> recoveryTail;
    static constexpr bool completeForPublicSave = false;
};

// 140eca080(page0)'s verified page state and140ed5dd0 empty recovery state.
// ValidationStopped is local to its page; later pages and the tail still run.
// UnsupportedSnapshot stops this adapter at the reached unsupported boundary,
// preserving earlier stores/I/O. Outer Save rename/root storage/view effects
// and intermediate viewer callbacks are not represented as completed here.
// The supplied remesh reference must identify the owner used at Open. Missing
// or replaced ownership refuses before any page/model/file effect.
InitialApplyReport applyInitialEditorPageState(InitialEditorSession&, GuiModel&,
    RemeshState&, Extra3DIo&, const std::vector<RecoveryEntry>* rootRecovery,
    const std::vector<RecoveryEntry>* operationRecovery);

// Private owner-bound composition. It checks Operation/model identity before
// page effects and resolves recovery holders only at the reached tail.
// This does not complete the public FormingClass Save callback.
InitialApplyReport applyInitialEditorPageState(InitialEditorSession&, GuiModel&,
    RemeshState&, Extra3DIo&, Operation&);

} // namespace m2m::engine::simcontrol300
