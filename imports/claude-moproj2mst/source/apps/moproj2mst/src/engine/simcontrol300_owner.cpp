#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/engine/view_manager.hpp"
#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <utility>
#include <defmodel/layout/dynary.hpp>

namespace m2m::engine::simcontrol300 {

StagedEditor300::StagedEditor300(Editor300Binding binding)
    : widget_(std::shared_ptr<WidgetState>(new WidgetState(this))),
      project_(&binding.project), operation_(&binding.operation),
      operationIdentity_(binding.operation.operationInfoIdentity()), model_(&binding.model),
      remesh_(&binding.remesh)
{
    widget_->componentOwner_ = widget_;
    if (!identityMatches(binding)) {
        report_.state = Editor300ConstructorState::InvalidBinding;
        report_.bindingIssue = Editor300BindingIssue::IdentityMismatch;
    }
}

StagedEditor300::~StagedEditor300()
{
    // Native area/factory destruction invalidates escaped diagnostic leases.
    // It does not certify the still-qualified native constructor/Close tails.
    if (firstGrid_) firstGrid_->releaseNativeOwner();
    widget_->releaseNativeOwner();
}

bool StagedEditor300::bindWidgetRuntime(Editor300Binding binding,
    const std::shared_ptr<WidgetRuntimeState>& runtime)
{
    if (!identityMatches(binding) || !runtime || !widget_->nativeOwnerAlive() ||
        report_.state != Editor300ConstructorState::AwaitingContext || pages_ || base_) return false;
    runtime->bindWidgetComponent(widget_);
    return true;
}

bool StagedEditor300::identityMatches(Editor300Binding binding) const noexcept
{
    // Check the original OPI generation before looking at caller-owned fields.
    // A live borrowed model and reused raw addresses cannot replace that OPI.
    return project_ == &binding.project && operation_ == &binding.operation &&
        operationIdentity_.get() == &binding.operation &&
        model_.get() == &binding.model && remesh_ == &binding.remesh &&
        binding.operation.project == &binding.project &&
        binding.operation.engineModel.get() == &binding.model;
}

EditorBaseReport StagedEditor300::continueNativeBase(Editor300Binding binding,
    const EditorBaseContext& context)
{
    if (!identityMatches(binding) || context.receiver != this)
        return {EditorBaseStage::NotStarted, EditorBaseIssue::IdentityMismatch};
    if (context.qualification.empty())
        return {EditorBaseStage::NotStarted, EditorBaseIssue::MissingQualification};
    // Native UI allocation follows QWidget and both application wrappers.
    // A previously bound setup is an explicit isolated/historical projection;
    // it cannot be replayed as a fresh native base constructor.
    if (setupBound_ || pages_ || firstGrid_ ||
        report_.state != Editor300ConstructorState::AwaitingContext)
        return {EditorBaseStage::NotStarted, EditorBaseIssue::WrongStage};
    if (!widget_->nativeOwnerAlive())
        return {EditorBaseStage::NotStarted, EditorBaseIssue::WidgetOwnerUnavailable};
    if (!widget_->runtimeOwner())
        return {EditorBaseStage::NotStarted, EditorBaseIssue::RuntimeOwnerUnavailable};
    if (!base_) base_.reset(new EditorBaseConstruction(this, widget_));
    return base_->resume(context);
}

bool StagedEditor300::bindSetupOwner(EditorSetupState& setup) noexcept
{
    if (setupBound_) {
        report_.setupBindIssue = Editor300SetupBindIssue::AlreadyBound;
        return false;
    }
    if (report_.state == Editor300ConstructorState::InvalidBinding ||
        operationIdentity_.get() != operation_ || !model_.get()) {
        report_.setupBindIssue = Editor300SetupBindIssue::InvalidFactory;
        return false;
    }
    if (base_ && !base_->report().applicationWrappersReturned) {
        report_.setupBindIssue = Editor300SetupBindIssue::NativeBaseIncomplete;
        return false;
    }
    if (report_.state != Editor300ConstructorState::AwaitingContext || pages_) {
        report_.setupBindIssue = Editor300SetupBindIssue::LateBinding;
        return false;
    }
    if (!setup.bindFactoryOwner(*this)) {
        report_.setupBindIssue = Editor300SetupBindIssue::SetupAlreadyOwned;
        return false;
    }
    setup_.reset(&setup);
    setupBound_ = true;
    report_.setupBindIssue = Editor300SetupBindIssue::None;
    return true;
}

NativeSetupPrefixReport StagedEditor300::beginNativeSetupPrefix(
    Editor300Binding binding, const NativeSetupObservation& context)
{
    if (!identityMatches(binding) || context.receiver != this)
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::IdentityMismatch};
    auto* setup = setup_.get();
    if (!setupBound_ || !setup)
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::SetupOwnerUnavailable};
    if (setup->sealedForPages())
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::HistoricalState};
    if (report_.state != Editor300ConstructorState::AwaitingContext || pages_)
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::WrongStage};
    return setup->beginNativePrefix(*this, context);
}

NativeSetupPrefixReport StagedEditor300::continueNativeSetupPropertyNotification(Editor300Binding binding)
{
    if (!identityMatches(binding))
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::IdentityMismatch};
    auto* setup = setup_.get();
    if (!setupBound_ || !setup)
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::SetupOwnerUnavailable};
    if (report_.state != Editor300ConstructorState::AwaitingContext || pages_)
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::WrongStage};
    return setup->continueNativePropertyNotification(*this);
}

EditorGridReport StagedEditor300::continueNativeSetupFirstGrid(
    Editor300Binding binding, const EditorGridContext& context)
{
    if (!identityMatches(binding) || context.receiver != this)
        return {EditorGridStage::NotStarted, EditorGridIssue::IdentityMismatch};
    if (context.qualification.empty())
        return {EditorGridStage::NotStarted, EditorGridIssue::MissingQualification};
    auto* setup = setup_.get();
    if (!setupBound_ || !setup)
        return {EditorGridStage::NotStarted, EditorGridIssue::SetupOwnerUnavailable};
    if (setup->sealedForPages())
        return {EditorGridStage::NotStarted, EditorGridIssue::HistoricalState};
    if (report_.state != Editor300ConstructorState::AwaitingContext || pages_)
        return {EditorGridStage::NotStarted, EditorGridIssue::WrongStage};
    if (!setup->destroyCallerFalseVariant(*this))
        return {EditorGridStage::NotStarted, EditorGridIssue::PropertyNotReturned};
    if (!firstGrid_)
        firstGrid_.reset(new EditorGridConstruction(this, widget_));
    return firstGrid_->resume(context);
}

const NativeSetupPrefixState* StagedEditor300::nativeSetupPrefix() const noexcept
{
    const auto* setup = setup_.get();
    return setup ? &setup->nativePrefix() : nullptr;
}

std::optional<NativeSetupPropertyEvent> StagedEditor300::pendingNativeSetupEvent(
    Editor300Binding binding) const
{
    if (!identityMatches(binding)) return std::nullopt;
    const auto* setup = setup_.get();
    if (!setup || setup->sealedForPages()) return std::nullopt;
    const auto* event = setup->nativePrefix().eventState();
    if (!widget_->nativeOwnerAlive() || !event || !event->nativeAlive()) return std::nullopt;
    return NativeSetupPropertyEvent{this, setup, event->rawType(), event->propertyName(),
        event->accepted(), event->spontaneous(), event->posted(), event->nativePrivateDataNull()};
}

const Editor300ConstructionReport& StagedEditor300::constructPageState(
    Editor300Binding binding, Extra3DIo& io, const InitialEditorContext& context)
{
    if (report_.state != Editor300ConstructorState::AwaitingContext) {
        // Do not erase the first reached failure or completed progress receipt.
        // The separate call rejection is observable without replaying effects.
        report_.repeatedConstructionRejected = true;
        return report_;
    }
    if (!identityMatches(binding)) {
        report_.state = Editor300ConstructorState::InvalidBinding;
        report_.bindingIssue = Editor300BindingIssue::IdentityMismatch;
        return report_;
    }
    if (base_ && !base_->report().applicationWrappersReturned) {
        report_.bindingIssue = Editor300BindingIssue::NativeSetupPrerequisite;
        return report_;
    }
    // Original procType5 is the actual MO editor. Explicit but unrelated PRE
    // context is not evidence for this operation's constructor.
    if (context.general.applicationMode != 5 || context.general.dimension != 3 ||
        context.general.moProject != project_ || context.stepCount.applicationMode != 5 ||
        !context.stepCount.mo || context.stepCount.mo->operation != operation_) {
        report_.state = Editor300ConstructorState::PageRefused;
        report_.bindingIssue = Editor300BindingIssue::ContextMismatch;
        return report_;
    }
    auto* setup = setup_.get();
    if (setupBound_ && !setup) {
        report_.state = Editor300ConstructorState::PageRefused;
        report_.bindingIssue = Editor300BindingIssue::SetupOwnerExpired;
        return report_;
    }
    if (setup && setup->nativePrefix().report.attempted) {
        // An explicit native attempt stopped before setup returned. Isolated
        // control/page projections cannot bypass that pending source stage.
        report_.state = Editor300ConstructorState::PageRefused;
        report_.bindingIssue = Editor300BindingIssue::NativeSetupPrerequisite;
        return report_;
    }
    report_.state = Editor300ConstructorState::PageConstruction;
    try {
        pages_ = std::make_unique<InitialEditorSession>();
        if (setup) {
            report_.setupHandoff = setup->handoffToPages(*this, *pages_, context.stoppingLabelProfile);
            if (!report_.setupHandoff->complete()) {
                report_.bindingIssue = Editor300BindingIssue::SetupPrerequisite;
                report_.state = Editor300ConstructorState::PageRefused;
                return report_;
            }
        }
        report_.pages.emplace();
        openInitialEditorInto(*pages_, binding.model, binding.remesh, io, context, *report_.pages);
    } catch (...) {
        // Keep the reached original state when the adapter reports an unresolved
        // I/O branch by exception. Preserve and rethrow the exact exception.
        report_.exceptionInterrupted = true;
        report_.state = Editor300ConstructorState::PageRefused;
        throw;
    }
    report_.state = report_.pages->complete()
        ? Editor300ConstructorState::AwaitingNativeBoundaries
        : Editor300ConstructorState::PageRefused;
    return report_;
}

InitialEditorSession* StagedEditor300::pageState() noexcept
{
    // Keep partial diagnostics, but do not expose the live page view after OPI
    // expiry merely because its borrowed model allocation remains available.
    if (operationIdentity_.get() != operation_) return nullptr;
    auto* model = model_.get();
    if (report_.state != Editor300ConstructorState::AwaitingNativeBoundaries ||
        !pages_ || !pages_->pageStateComplete || !model ||
        pages_->sourceModel != model || !model->matchesCurrentSimControl(pages_->sourceControl)) return nullptr;
    return pages_.get();
}

// @src 0x140ecf910: store OPI+18 before resolving current SC/refilling.
StepCountToggleReport StagedEditor300::toggleNstart(Editor300Binding binding,
    const StepCountContext& currentContext, bool checked)
{
    StepCountToggleReport report;
    // Do not use pageState() here: its simCtl lookup would move a source
    // prerequisite ahead of the operation flag store.
    if (!identityMatches(binding) ||
        report_.state != Editor300ConstructorState::AwaitingNativeBoundaries ||
        !pages_ || !pages_->pageStateComplete || pages_->sourceModel != &binding.model ||
        pages_->sourceRemesh != &binding.remesh || currentContext.applicationMode != 5 ||
        !currentContext.mo || currentContext.mo->operation != &binding.operation ||
        !pages_->pages.stepCount.mo ||
        pages_->pages.stepCount.mo->operation != &binding.operation ||
        pages_->pages.stepCount.nstartChecked != checked) return report;

    binding.operation.nstartGiven = checked ? 1 : 0;
    report.stage = StepCountToggleStage::OperationFlagStored;
    report.operationFlagStored = true;
    StepCountPage refill{};
    if (!binding.model.refillEditorStepCountIfControlMatches(
            pages_->sourceControl, currentContext, checked, refill)) return report;
    pages_->pages.stepCount = std::move(refill);
    report.stage = StepCountToggleStage::Complete;
    report.pageRefilled = true;
    report.outcome = ApplyResult::Completed;
    return report;
}

// @src 0x140ecb8e0: delivered text -> local bytes -> current SC -> title copy.
ApplyResult StagedEditor300::titleChanged(Editor300Binding binding,
    const qt::String& deliveredText)
{
    if (!identityMatches(binding) ||
        report_.state != Editor300ConstructorState::AwaitingNativeBoundaries ||
        !pages_ || !pages_->pageStateComplete || pages_->sourceModel != &binding.model ||
        pages_->sourceRemesh != &binding.remesh ||
        !(pages_->pages.general.title == deliveredText)) return ApplyResult::UnsupportedSnapshot;

    // Explicit ASCII/buffer admission, not native validation or truncation.
    // The original strcpy has no bound; the admitted title fits its 81 bytes.
    if (deliveredText.u16().size() > 80) return ApplyResult::UnsupportedSnapshot;
    for (char16_t unit : deliveredText.u16())
        if (unit < 0x20 || unit > 0x7e) return ApplyResult::UnsupportedSnapshot;
    const auto bytes = qt::toAscii(deliveredText);
    // The model command retains simCtl()'s barrier-first lookup here, after
    // conversion, and compares the cached source only as an identity token.
    if (!binding.model.copyEditorTitleIfControlMatches(pages_->sourceControl, bytes))
        return ApplyResult::UnsupportedSnapshot;
    return ApplyResult::Completed;
}

// @src 0x140eccf10, clicked wrappers 0x140ecf7c0/7e0/810/840.
EnvironmentReactionReport StagedEditor300::reactionRateClicked(Editor300Binding binding,
    int selection)
{
    EnvironmentReactionReport report;
    if (!identityMatches(binding) || selection < 0 || selection > 3 ||
        report_.state != Editor300ConstructorState::AwaitingNativeBoundaries ||
        !pages_ || !pages_->pageStateComplete || pages_->sourceModel != &binding.model ||
        pages_->sourceRemesh != &binding.remesh) return report;
    auto& owners = pages_->environmentCurves;
    auto& temporary = owners.curves[3];
    const auto& projected = pages_->pages.environment.curves[3];
    if (!temporary || projected.get() != temporary.get() ||
        projected.owner_before(temporary) || temporary.owner_before(projected)) return report;
    for (int i = 0; i < 3; ++i)
        if (owners.curves[i].get() == temporary.get()) return report;

    // Project only the existing private control values. This does not claim
    // actual QWidget enabled bits, setChecked emissions or reentrant delivery.
    pages_->pages.environment.acvcofControls =
        {selection, selection == 0, selection == 0, selection != 0};
    report.stage = EnvironmentReactionStage::ControlsProjected;
    report.controlsProjected = true;
    // Native current-SC lookup follows the radio/enabled setters. Do not use
    // pageState() above and move this prerequisite ahead of those effects.
    if (commitReactionRateSelection(binding.model, selection) != ReactionRateCommit::Complete)
        return report;
    report.stage = EnvironmentReactionStage::Complete;
    report.modelUpdated = true;
    report.outcome = ApplyResult::Completed;
    return report;
}

StagedEditor300::ReactionRateCommit StagedEditor300::commitReactionRateSelection(
    GuiModel& model, int selection)
{
    // The caller has checked page and temporary ownership and projected the
    // cached controls. This single lookup keeps the geometry barrier first.
    auto* control = model.simCtl();
    if (!control || control != pages_->sourceControl)
        return ReactionRateCommit::MissingOrReplacedControl;
    auto& temporaries = pages_->environmentCurves;
    auto* temporary = temporaries.curves[3].get();
    if (temporary == &control->envtmp || temporary == &control->cnvcof ||
        temporary == &control->envatm || temporary == &control->acvcof)
        return ReactionRateCommit::LiveCurveAlias;

    temporaries.currentCurveIndex = 3;
    constexpr int variables[]{0, 3, 12, 1};
    if (selection == 0) control->acvcof.ndim = 0;
    control->acvcof.var[0] = variables[selection];
    // Native changes the temporary variable, never its dimension or table.
    temporary->var[0] = variables[selection];
    return ReactionRateCommit::Complete;
}

// @src 0x140eccb50/ccc90/cdd0: clicked constant/table controls only.
ApplyResult StagedEditor300::environmentCurveClicked(Editor300Binding binding,
    int curveIndex, int selection)
{
    if (!identityMatches(binding) || curveIndex < 0 || curveIndex > 2 ||
        selection < 0 || selection > 1 ||
        report_.state != Editor300ConstructorState::AwaitingNativeBoundaries ||
        !pages_ || !pages_->pageStateComplete || pages_->sourceModel != &binding.model ||
        pages_->sourceRemesh != &binding.remesh) return ApplyResult::UnsupportedSnapshot;
    // These three native callbacks have no current-SC query or curve stores.
    // Keep them independent of pageState() and of the later Apply operation.
    const bool constant = selection == 0;
    pages_->pages.environment.curveControls[curveIndex] =
        {constant, !constant, constant, constant, !constant};
    return ApplyResult::Completed;
}

// @src 0x140ecfeb0/ff30/ffb0, 0x140ed0030/00b0/0130.
ApplyResult StagedEditor300::frequencyTextEdited(Editor300Binding binding, int row,
    const qt::String& deliveredText, NumericLocale locale)
{
    if (!identityMatches(binding) || row < 0 || row > 5 || locale != NumericLocale::KnownC ||
        report_.state != Editor300ConstructorState::AwaitingNativeBoundaries ||
        !pages_ || !pages_->pageStateComplete || pages_->sourceModel != &binding.model ||
        pages_->sourceRemesh != &binding.remesh) return ApplyResult::UnsupportedSnapshot;
    const auto& cached = pages_->pages.advanced.populatedFrequencyText[row];
    if (!cached || !(*cached == deliveredText)) return ApplyResult::UnsupportedSnapshot;

    return binding.model.applyEditorFrequencyTextIfControlMatches(pages_->sourceControl, row,
        deliveredText) ? ApplyResult::Completed : ApplyResult::UnsupportedSnapshot;
}

// @src 0x140ecfab0/fb90/fc30/fcd0/fd70/fe10.
ApplyResult StagedEditor300::frequencyIndexChanged(Editor300Binding binding,
    int row, int selection)
{
    if (!identityMatches(binding) || row < 0 || row > 5 || selection < 0 ||
        selection > (row == 0 ? 2 : 1) ||
        report_.state != Editor300ConstructorState::AwaitingNativeBoundaries ||
        !pages_ || !pages_->pageStateComplete || pages_->sourceModel != &binding.model ||
        pages_->sourceRemesh != &binding.remesh ||
        pages_->pages.advanced.frequencyCombo[row] != selection)
        return ApplyResult::UnsupportedSnapshot;
    if (!binding.model.applyEditorFrequencyIndexIfControlMatches(pages_->sourceControl,
        row, selection)) return ApplyResult::UnsupportedSnapshot;
    auto& enabled = pages_->pages.advanced.frequencyInputEnabled[row];
    if (selection == (row == 0 ? 2 : 1)) {
        enabled = true;
        return ApplyResult::Completed;
    }
    enabled = false; // Native model store precedes the requested disable.
    return ApplyResult::Completed;
}

// @src 0x140ed0820: stateChanged only requests definition-button enablement.
ApplyResult StagedEditor300::viewFactorStateChanged(Editor300Binding binding, int state)
{
    if (!identityMatches(binding) || (state != 0 && state != 2) ||
        report_.state != Editor300ConstructorState::AwaitingNativeBoundaries ||
        !pages_ || !pages_->pageStateComplete || pages_->sourceModel != &binding.model ||
        pages_->sourceRemesh != &binding.remesh ||
        pages_->pages.environment.viewFactorChecked != (state != 0))
        return ApplyResult::UnsupportedSnapshot;
    // No SC/dimension/curve query: those belong to later Apply, not this slot.
    pages_->pages.environment.viewFactorDefinitionEnabled = state != 0;
    return ApplyResult::Completed;
}

// @src 0x140ed0660: eight show/hide requests, no solver/model access.
ApplyResult StagedEditor300::explicitSolverToggled(Editor300Binding binding, bool checked)
{
    if (!identityMatches(binding) ||
        report_.state != Editor300ConstructorState::AwaitingNativeBoundaries ||
        !pages_ || !pages_->pageStateComplete || pages_->sourceModel != &binding.model ||
        pages_->sourceRemesh != &binding.remesh) return ApplyResult::UnsupportedSnapshot;
    auto& form = pages_->pages.iteration;
    if ((form.solver != 0 && form.solver != 1 && form.solver != 2 && form.solver != 4 &&
            form.solver != 5 && form.solver != 6) || (form.solver == 6) != checked)
        return ApplyResult::UnsupportedSnapshot;
    // Common argument for UI+b18,b28,b38,b48,b10,b20,b30,b40 only. Do not
    // query pageState/SC, populate edit text, normalize parameters or Apply.
    form.explicitControlsVisible = checked;
    return ApplyResult::Completed;
}

} // namespace m2m::engine::simcontrol300
