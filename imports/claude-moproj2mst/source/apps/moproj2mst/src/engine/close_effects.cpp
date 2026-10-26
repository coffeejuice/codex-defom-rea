#include "m2m/engine/close_effects.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_statevar.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_auxiliary.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unsupported(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Close ChangePtrkGraph: ") + detail);
}
[[noreturn]] void unsupportedPostGraph(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Close post graph manager: ") + detail);
}
}

PostGraphSceneState::PostGraphSceneState(std::shared_ptr<Display> display)
    : display_(std::move(display))
{
    if (!display_) unsupportedPostGraph("present scene requires its actual embedded Display");
}

SceneTextItemState::SceneTextItemState(const std::shared_ptr<Display>& display,
                                     const qt::String& text)
    : textTemplate(text), displayedText(text), display_(display)
{}

void PostGraphSceneState::commitInitialTextItems()
{
    if (textCommitStarted_)
        unsupportedPostGraph("initial scene text construction is a one-shot boundary");
    textCommitStarted_ = true;
    // Native cca680 commits the first allocation/add before starting the next.
    // C++ container allocation failure is not claimed to reproduce Qt failure.
    auto databaseName = std::unique_ptr<SceneTextItemState>(
        new SceneTextItemState(display_, qt::L("$DBNAME")));
    auto* first = databaseName.get();
    items_.push_back(std::move(databaseName));
    databaseNameItem_ = first;
    auto stepNumber = std::unique_ptr<SceneTextItemState>(
        new SceneTextItemState(display_, qt::L("Step $STEPNO")));
    auto* second = stepNumber.get();
    items_.push_back(std::move(stepNumber));
    stepNumberItem_ = second;
    // ccab89/ccaba3 hide these actual objects; hiding does not erase membership.
    databaseNameItem_->hidden = true;
    stepNumberItem_->hidden = true;
    textCommitFinished_ = true;
}

void PostGraphSceneState::qualifyCurrentItemPopulation(std::string qualification)
{
    if (!textCommitFinished_)
        unsupportedPostGraph("scene membership qualification requires its initial text construction prefix");
    itemQualification_ = std::move(qualification);
}

void PostGraphSceneState::invalidateItemPopulation() noexcept
{
    itemQualification_.clear();
}

void PostGraphSceneState::observeCurrentSelection(
    std::unordered_set<const PostGraphSceneItemState*> selected,
    std::uint32_t depth, std::string qualification)
{
    if (clearSelectionPending_)
        unsupportedPostGraph("clearSelection continuation is pending before rebinding selection");
    for (const auto* identity : selected) {
        bool owned = false;
        for (const auto& item : items_) {
            if (item.get() == identity) { owned = true; break; }
        }
        if (!owned)
            unsupportedPostGraph("selection identity is not represented in this actual scene");
    }
    selectedItems_ = std::move(selected);
    selectionDepth_ = depth;
    selectionQualification_ = std::move(qualification);
}

void PostGraphSceneState::invalidateCurrentSelection() noexcept
{
    // Losing coverage does not delete selected identities or reset native depth.
    selectionQualification_.clear();
}

void PostGraphSceneState::clearSelection()
{
    if (clearSelectionPending_)
        unsupportedPostGraph("clearSelection continuation is pending before another invocation");
    if (!selectionDepth_)
        unsupportedPostGraph("clearSelection native selection-change depth is unavailable");
    ++*selectionDepth_; //6557174a, raw 32-bit INC (including wrap)
    clearSelectionPending_ = true;
    if (selectionQualification_.empty())
        unsupportedPostGraph("clearSelection current selected-item coverage is unavailable");
    if (!selectedItems_.empty())
        unsupportedPostGraph("clearSelection nonempty item setSelected callbacks are unavailable");
    // A coherent empty Qt hash snapshot has begin==end, including allocated
    // empty buckets. It invokes no item/node callbacks;655662d0 clears its set.
    // Container allocation failure parity is outside this qualified branch.
    selectedItems_.clear();
    --*selectionDepth_; //65571800, after the actual set clear
    clearSelectionPending_ = false;
    // oldNonempty=false: native65571809 skips signal activation at every depth.
}

bool PostGraphSceneState::hasHistogramChart() const
{
    if (itemQualification_.empty())
        unsupportedPostGraph("c0d7e0 current secondary item population coverage is unavailable");
    for (const auto& item : items_) {
        // SfGraphicsTextItem's complete native RTTI hierarchy matches neither
        // SfGraphItem nor SfHistoChart. Its text content and visibility do not
        // change that concrete type. No graph constructor/insertion is exposed
        // until its actual ownership and destructor effects are recovered.
        if (dynamic_cast<const SceneTextItemState*>(item.get())) continue;
        unsupportedPostGraph("c0d7e0 concrete graph item classification is unavailable");
    }
    return false;
}

void PostGraphSceneState::removeGraphItems(PostGraphManagerState&)
{
    if (itemQualification_.empty())
        unsupportedPostGraph("c0c650 current item population coverage is unavailable");
    for (const auto& item : items_) {
        // Native c0c650 tests seven graph/chart RTTI types. SfGraphicsTextItem
        // has none of those bases, regardless of text, visibility or parent.
        // Preserve both scene-owned text objects and their direct scene fields.
        if (dynamic_cast<const SceneTextItemState*>(item.get())) continue;
        unsupportedPostGraph("c0c650 concrete graph classification/deletion is unavailable");
    }
}

PostGraphFactoryState::PostGraphFactoryState(Kind kind, const PostGraphManagerState* manager,
                                           const std::shared_ptr<DbHandle>& model)
    : kind_(kind), manager_(manager), modelOwner_(model)
{}

PostGraphManagerState::PostGraphManagerState(const std::shared_ptr<DbHandle>& model,
                                           std::shared_ptr<PostGraphSceneState> scene)
    : modelOwner_(model), scene_(std::move(scene))
{
    // c15440 -> c15540 -> base virtual+58=c15880 creates the base factory.
    factory_.reset(new PostGraphFactoryState(PostGraphFactoryState::Kind::Base, this, model));
    // Derived c0b980 -> c0b9f0 creates the new child BEFORE c157f0 deletes the
    // previous factory and commits the replacement. Both have actual ownership.
    auto derived = std::unique_ptr<PostGraphFactoryState>(
        new PostGraphFactoryState(PostGraphFactoryState::Kind::Post, this, model));
    factory_.reset();
    factory_ = std::move(derived);
    field30 = field34 = 10; // c0de20, after factory replacement
    flag38 = false; // c0dec0, before receiver registration
}

PostGraphManagerState::~PostGraphManagerState()
{
    // c155d0 deletes the factory before graph traversal. This class has no
    // supported graph insertion path, so its actual collection is still empty.
    factory_.reset();
    graphItems_.clear();
}

std::shared_ptr<PostGraphManagerState> ensurePostGraphManager(
    const std::shared_ptr<DbHandle>& model, const std::optional<PostGraphSceneBinding>& scene,
    doe::SessionNotificationState& notifications)
{
    if (!model) unsupportedPostGraph("actual model/DB child is unavailable");
    if (model->postGraphManager) {
        if (model->postGraphManager->modelOwner().lock().get() != model.get())
            unsupportedPostGraph("cached manager belongs to a different model/DB child");
        return model->postGraphManager;
    }
    if (model->kind != DbHandle::Kind::Mo)
        unsupportedPostGraph("base/type4 model factory cdd930 is unrecovered");
    if (!scene || scene->qualification.empty())
        unsupportedPostGraph("045260 global scene binding is unavailable before allocation");
    auto created = std::shared_ptr<PostGraphManagerState>(
        new PostGraphManagerState(model, scene->owner));
    // c0b980 registers last. An unavailable population is not a known empty
    // global key. On adapter refusal, local ownership cleans this uncommitted
    // constructor result; cdd8e0 has not stored the model's cache yet.
    if (!notifications.removeGraphItemsRegistrations ||
        notifications.removeGraphItemsQualification.empty())
        unsupportedPostGraph("RemoveGraphItems registration population is unavailable");
    notifications.removeGraphItemsRegistrations->push_back(created);
    model->postGraphManager = created; // cdd8e0 stores only after factory returns
    return created;
}

void removeGraphItems(doe::SessionNotificationState& notifications)
{
    removeGraphItems(notifications, std::nullopt);
}

void removeGraphItems(doe::SessionNotificationState& notifications,
                      const std::optional<PostGraphWindowBinding>& window)
{
    // 2cf3c0 skips every map/receiver effect when global+141 is set. Otherwise
    // it clears +90 before the map lookup, including before an adapter guard.
    if (notifications.suppressed) return;
    notifications.flag90 = false;
    if (!notifications.removeGraphItemsRegistrations ||
        notifications.removeGraphItemsQualification.empty())
        unsupportedPostGraph("RemoveGraphItems dispatch population is unavailable");

    const auto snapshot = *notifications.removeGraphItemsRegistrations;
    if (snapshot.empty()) {
        // A qualified empty population allows the original map's default key
        // insertion. A registered nonempty list, even if expired, already has
        // its key and must not be replaced by an empty map entry.
        notifications.emptyKeys.insert("2signalRemoveGraphItems()");
        return;
    }
    for (const auto& weak : snapshot) {
        const auto receiver = weak.lock();
        if (!receiver) continue; // only the native local copy is pruned
        const auto owner = receiver->modelOwner().lock();
        if (!owner)
            unsupportedPostGraph("live RemoveGraphItems receiver has lost its model owner");
        if (owner->postGraphManager.get() != receiver.get())
            unsupportedPostGraph("live RemoveGraphItems receiver is not its model's cached manager");
        if (!window || window->qualification.empty())
            unsupportedPostGraph("c0cfe0 current global mainwindow binding is unavailable");
        if (!window->mainWindowPresent)
            unsupportedPostGraph("c0cfe0 null global mainwindow would be dereferenced");
        if (!window->primaryScene || window->primaryScene->qualification.empty())
            unsupportedPostGraph("c0cfe0 current primary scene binding is unavailable");
        const auto primary = window->primaryScene->owner;
        if (!window->secondaryScene || window->secondaryScene->qualification.empty())
            unsupportedPostGraph("c0cfe0 current secondary scene binding is unavailable");
        const auto secondary = window->secondaryScene->owner;
        // Both native getters have returned before either selective cleanup.
        // The constructor cache and empty manager list cannot substitute for
        // the actual current global scenes or their qualified collections.
        if (primary) primary->removeGraphItems(*receiver);
        if (secondary) secondary->removeGraphItems(*receiver);
        // c0cfe0 emits even after two null scenes or all-text no-match lists.
        // Actual splitter/widget owners and ordered receiver coverage remain
        // session dependencies; no counter or invented empty population here.
        unsupportedPostGraph("c0cfe0 unconditional HideSecondary dispatch effects are unavailable");
    }
}

PtrkGraphManagerState::PtrkGraphManagerState(const std::shared_ptr<DbHandle>& owner)
    : owner_(owner)
{
    if (!owner) unsupported("SfPtrkManager requires its actual nonnull data-root owner");
}

PtrkGraphManagerState::~PtrkGraphManagerState()
{
    clearCachedGraph(); // 309fb0 ->321520, before other manager-owned cleanup
}

void PtrkGraphManagerState::clearCachedGraph() noexcept
{
    // Supported graphs have no populated native render child. unique ownership
    // mirrors321520's virtual delete followed by cache=null, including null skip.
    graph_.reset();
}

void CloseEffects::setPtrkGraphRegistrations(std::optional<PtrkGraphRegistrations> registrations)
{
    ptrkGraphRegistrations_ = std::move(registrations);
}

void CloseEffects::setPtrkGraphDisplayBinding(std::optional<PtrkGraphDisplayBinding> binding)
{
    ptrkGraphDisplayBinding_ = std::move(binding);
}

void CloseEffects::changePtrkGraph(GuiModel& model, doe::StateVarState& state)
{
    // Adapter identity qualification, not an additional original branch.
    if (!model.doeStepData || model.doeStepData->stateVar.get() != &state)
        unsupported("application must pass the StateVar owned by the actual GuiModel");
    if (!ptrkGraphRegistrations_ || ptrkGraphRegistrations_->qualification.empty())
        unsupported("ordered receiver population is unavailable");

    // 2cf3c0 traverses a local QList copy. Expired QPointers are removed only
    // from that copy; it does not clean the original global registration list.
    const auto snapshot = ptrkGraphRegistrations_->receivers;
    for (const auto& weak : snapshot) {
        const auto receiver = weak.lock();
        if (!receiver) continue;
        // An externally retained receiver whose original parent has died is an
        // incomplete ownership projection, not a proof of a native live object.
        const auto owner = receiver->owner().lock();
        if (!owner) unsupported("live receiver has lost its original data-root owner");

        // 322420 ->3215c0 is a pure +4d0 load. False returns before model lookup,
        // graph creation, plot-variable reads or any graph mutation.
        if (!receiver->graphOn) continue;

        // 321660 ->31fab0 first allocates/stores SfPtrk if +4c8 is null. This
        // mutation persists when a later effect is unavailable. Cached objects
        // retain their identity and +f1 flag; neither is reset on another call.
        if (!receiver->graph_) receiver->graph_ = std::make_unique<PtrkGraphState>();

        // 31fab0 resolves the global Display and registers even a cached graph
        // with +f1=false. Missing binding is not a source-observed null scene.
        const auto selectedModel = [&]() -> GuiModel* {
            if (!ptrkGraphDisplayBinding_ || ptrkGraphDisplayBinding_->qualification.empty())
                unsupported("31fab0 global Display binding for model+250 registration is unavailable after cache allocation");
            const auto display = ptrkGraphDisplayBinding_->owner;
            return display ? displaySelectedModel(*display) : nullptr;
        };
        if (auto* selected = selectedModel())
            registerAuxiliaryDrawing(*selected, receiver->graph_.get());

        // 321660 reads graph+f1 only after the registration prefix. A new graph
        // returns here without needing any StateVar or rendering payload.
        if (!receiver->graph_->graphOn) continue;

        // 322420 calls30a2b0 again; do not reuse the registration-time selection
        // or read the emitting model's variable as a substitute.
        auto* selected = selectedModel();
        if (!selected) continue;
        if (!selected->doeStepData || !selected->doeStepData->stateVar)
            unsupported("c905b0 selected-model StateVar constructor requires resolved context and runtime");
        if (selected->doeStepData->stateVar->plotVar == 0) continue;
        unsupported("30ad90 selected nonzero-variable graph update is unavailable after registration");
    }
}
void StateVarPlotEffects::changeHistoGraph(GuiModel&, doe::StateVarState&)
{
    throw Error(M2M_E_UNSUPPORTED, "Close ChangeHistoGraph: receiver effect is unavailable");
}

void CloseEffects::setHistogramRegistrations(std::optional<HistogramRegistrations> registrations)
{
    histogramRegistrations_ = std::move(registrations);
}

void CloseEffects::setHistogramGraphBinding(std::optional<HistogramGraphBinding> binding)
{
    histogramGraphBinding_ = std::move(binding);
}

void CloseEffects::changeHistoGraph(GuiModel& model, doe::StateVarState& state)
{
    const auto refuse = [](const char* detail) {
        throw Error(M2M_E_UNSUPPORTED, std::string("Close ChangeHistoGraph: ") + detail);
    };
    if (!model.doeStepData || model.doeStepData->stateVar.get() != &state)
        refuse("application must pass the StateVar owned by the actual GuiModel");
    if (!histogramRegistrations_ || !histogramRegistrations_->owner ||
        histogramRegistrations_->qualification.empty())
        refuse("actual runtime and ordered receiver coverage are unavailable");

    // Bind the actual runtime and take a new local snapshot at each emit. New
    // helpers registered since binding are included. Flags are never copied.
    const auto lease = histogramRegistrations_->owner;
    const auto snapshot = lease->stateVarRuntime().histogramRegistrations;
    for (const auto& registration : snapshot) {
        if (registration.signal != "signalChangeHistoGraph") continue;
        const auto receiver = registration.receiver.lock();
        if (!receiver) continue; // do not prune persistent runtime registrations
        if (registration.slot != "slotChangeHistoGraph")
            refuse("live receiver uses an unsupported slot");
        // 413f00 ->4117e0 reads the existing helper's actual +14 field.
        if (!receiver->graphOn) continue;
        // 8e0550 is a pure helper+38+7c load. Its result is only used after the
        // still-guarded nonzero-variable virtual+78 effect; it can be elided
        // for this prefix. It must never be replaced by binWidth at+10.
        const bool requestedOn = receiver->graphOn; //413f00 rereads +14
        if (!histogramGraphBinding_ || histogramGraphBinding_->qualification.empty())
            refuse("enabled receiver global binding is unavailable after pure +0xb4 read");
        const auto& binding = *histogramGraphBinding_;
        if (!binding.mainWindowPresent) continue; //4122f0 initial null-global return
        if (!binding.modelManager || lease->modelManager() != binding.modelManager.get())
            refuse("global ModelManager must be the actual StudyIo-bound owner");
        if (!lease->observedSessionNotificationState())
            refuse("actual shared notifier owner is unavailable before graph-manager lookup");
        auto& notifications = lease->sessionNotificationState();

        auto current = currentSubProjectDbOwner(*binding.modelManager);
        if (!current)
            refuse("4122f0 null current model reaches native cdd8e0 dereference");
        // This real cache/registration mutation precedes selected StateVar
        // lookup, and survives any later adapter refusal.
        (void)ensurePostGraphManager(current, binding.primaryScene, notifications);

        const auto selectedState = [&]() -> doe::StateVarState* {
            if (!binding.primaryScene || binding.primaryScene->qualification.empty())
                refuse("414030 actual primary scene binding is unavailable");
            const auto& scene = binding.primaryScene->owner;
            auto* selected = scene ? displaySelectedModel(*scene->displayOwner()) : nullptr;
            if (!selected) return nullptr;
            if (!selected->doeStepData || !selected->doeStepData->stateVar)
                refuse("c905b0 selected-model StateVar constructor is unavailable");
            return selected->doeStepData->stateVar.get();
        };
        int plotVar = 0;
        if (selectedState()) {
            // Original414030 is called again for the actual value read.
            auto* selected = selectedState();
            if (!selected) refuse("4122f0 repeated StateVar lookup reached native null dereference");
            plotVar = selected->plotVar;
        }
        if (requestedOn && plotVar != 0)
            refuse("4122f0 nonzero-variable virtual+78 graph mutation is unavailable");

        //412430 clears the actual receiver before its own global/current
        // checks. It re-resolves current twice; the earlier cache is not reused.
        receiver->graphOn = false;
        if (!binding.mainWindowPresent) continue;
        if (!currentSubProjectDbOwner(*binding.modelManager)) continue;
        current = currentSubProjectDbOwner(*binding.modelManager);
        if (!current)
            refuse("412430 repeated current-model lookup reaches native cdd8e0 dereference");
        (void)ensurePostGraphManager(current, binding.primaryScene, notifications);
        if (!binding.secondaryScene || binding.secondaryScene->qualification.empty())
            refuse("c0d7e0 current secondary scene binding is unavailable after graphOn clear");
        const auto& secondary = binding.secondaryScene->owner;
        if (!secondary)
            refuse("c0d7e0 null secondary scene reaches native items() dereference");
        if (secondary->hasHistogramChart())
            refuse("c0d7e0 matching graph requires actual HideSecondary receiver effects");
        // No match: native c0d7e0 returns without emitting Hide or deleting any
        // item. Continue ordered histogram receiver delivery.
    }
}
} // namespace m2m::engine
