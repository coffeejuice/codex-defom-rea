#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/engine/view_manager.hpp"

namespace m2m::engine::simcontrol300 {

StagedEditor300::StagedEditor300(Editor300Binding binding)
    : widget_(std::shared_ptr<WidgetState>(new WidgetState(this))),
      project_(&binding.project), operation_(&binding.operation), model_(&binding.model),
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
    // Compare stored addresses before looking at any caller-owned fields. The
    // model guard may expire while the diagnostic record remains in the area.
    return project_ == &binding.project && operation_ == &binding.operation &&
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
    if (report_.state == Editor300ConstructorState::InvalidBinding || !model_.get()) {
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
    auto* model = model_.get();
    if (report_.state != Editor300ConstructorState::AwaitingNativeBoundaries ||
        !pages_ || !pages_->pageStateComplete || !model ||
        pages_->sourceModel != model || pages_->sourceControl != model->simCtl()) return nullptr;
    return pages_.get();
}

} // namespace m2m::engine::simcontrol300
