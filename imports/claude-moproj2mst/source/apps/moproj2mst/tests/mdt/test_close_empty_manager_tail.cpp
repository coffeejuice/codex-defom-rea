// ctest-labels: m2m-mdt close-empty-manager-tail
#include <testkit/check.hpp>

#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/close_empty_manager_tail.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/mo_model_slots.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/engine/view_manager.hpp"
#include "m2m/model.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
struct CountingDriver final : DiagramEffectDriver {
    unsigned fits = 0, clicks = 0;
    void fitAndNotify(ConversionSession&) override { ++fits; }
    void clickAndUpdateMainView(ConversionSession&, const FlowItem&, FileEffects&) override {
        ++clicks;
    }
};
template<class F> bool refuses(F&& body, const char* detail)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}
}

static_assert(!std::is_copy_constructible_v<CloseEmptyManagerTailProgress>);
static_assert(!std::is_move_constructible_v<CloseEmptyManagerTailProgress>);

TEST_CASE(close_empty_manager_tail_uses_actual_session_manager_and_retains_post_clear_guard)
{
    CountingDriver driver;
    ConversionSession session(driver);
    auto manager = session.modelManagerOwner();
    REQUIRE(manager && manager->ownerSession() == &session);
    REQUIRE(manager->size() == 0);
    REQUIRE(manager->connections().empty());
    CloseEmptyManagerTailProgress progress(session, closeEmptyManagerSourceOccurrence,
        "declared isolated empty manager entry; not accepted Close evidence");
    CHECK(progress.stage() == CloseEmptyManagerTailStage::InspectPopulation);
    CHECK(progress.blockedModelIdentity() == nullptr);
    const auto first = attemptCloseEmptyManagerTail(session, progress);
    CHECK(first.stop == CloseEmptyManagerTailStop::BeforePostClearProjectLookup);
    CHECK(first.clearCommittedThisAttempt);
    CHECK(first.observedModelCount == 0);
    CHECK(std::string(first.nextSourceSite).find("0x140029a83") == 0);
    CHECK(progress.stage() == CloseEmptyManagerTailStage::BeforePostClearProjectLookup);
    CHECK(manager->currentModel() == nullptr);
    CHECK(manager->currentModelId() == -1);
    CHECK(session.modelManagerOwner() == manager);
    const auto retry = attemptCloseEmptyManagerTail(session, progress);
    CHECK(retry.stop == first.stop);
    CHECK(!retry.clearCommittedThisAttempt);
    CHECK(retry.observedModelCount == 0);
    CHECK(driver.fits == 0 && driver.clicks == 0);
}

TEST_CASE(close_empty_manager_tail_preserves_project_slots_runtime_and_process_projections)
{
    CountingDriver driver;
    ConversionSession session(driver);
    auto* project = session.liveProject();
    REQUIRE(project);
    const auto slots = project->modelSlotsOwner();
    const auto registry = project->registryOwner();
    const auto display = project->display;
    const auto primary = session.primarySceneOwner();
    const auto secondary = session.secondarySceneOwner();
    const auto mainView = session.mainViewOwner();
    const auto runtime = session.widgetRuntimeOwner();
    const auto qobjects = runtime->qobjectCount();
    const auto widgets = runtime->currentWidgetCount();
    auto notifications = session.sessionNotificationStateOwner();
    notifications->flag90 = true;
    notifications->suppressed = true;
    const auto attempts = notifications->attempts.size();
    const auto keys = notifications->emptyKeys;
    auto* remesh = &session.remeshState();
    remesh->enabled = true;
    remesh->method = 4;
    remesh->filterDistance = 7.0;
    auto* scope = currentProject();
    CloseEmptyManagerTailProgress progress(session, closeEmptyManagerSourceOccurrence,
        "declared empty-owner fixture; no Qt teardown");
    attemptCloseEmptyManagerTail(session, progress);
    CHECK(session.liveProject() == project);
    CHECK(currentProject() == scope);
    CHECK(project->modelSlotsOwner() == slots && project->registryOwner() == registry);
    CHECK(slots->projectStorageAlive());
    CHECK(slots->nextStorageBoundary() == MoModelSlots::Slot::Operations);
    CHECK(project->display == display);
    CHECK(session.primarySceneOwner() == primary && session.secondarySceneOwner() == secondary);
    CHECK(session.mainViewOwner() == mainView && session.widgetRuntimeOwner() == runtime);
    CHECK(mainView->widgetState()->nativeOwnerAlive());
    CHECK(runtime->qobjectCount() == qobjects && runtime->currentWidgetCount() == widgets);
    CHECK(session.sessionNotificationStateOwner() == notifications);
    CHECK(notifications->flag90 && notifications->suppressed);
    CHECK(notifications->attempts.size() == attempts && notifications->emptyKeys == keys);
    CHECK(&session.remeshState() == remesh);
    CHECK(remesh->enabled && remesh->method == 4 && remesh->filterDistance == 7.0);
    CHECK(driver.fits == 0 && driver.clicks == 0);
}

TEST_CASE(close_empty_manager_tail_refuses_registered_child_before_project_binding)
{
    CountingDriver driver;
    ConversionSession session(driver);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C75/prebind.DB"));
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    auto* project = session.liveProject();
    const auto slots = project->modelSlotsOwner();
    const auto connections = manager->connections().size();
    REQUIRE(model && model->moProject == nullptr);
    REQUIRE(connections == 2);
    CloseEmptyManagerTailProgress progress(session, closeEmptyManagerSourceOccurrence,
        "declared failed-Open boundary after registration before native Project binding");
    const auto result = attemptCloseEmptyManagerTail(session, progress);
    CHECK(result.stop == CloseEmptyManagerTailStop::OwnedChildTeardownUnavailable);
    CHECK(!result.clearCommittedThisAttempt && result.observedModelCount == 1);
    CHECK(progress.stage() == CloseEmptyManagerTailStage::OwnedChildTeardownBlocked);
    CHECK(progress.blockedModelIdentity() == model.get());
    CHECK(std::string(result.nextSourceSite).find("0x140c784f2") == 0);
    CHECK(manager->size() == 1 && manager->modelAt(0) == model.get());
    CHECK(manager->currentModel() == nullptr);
    CHECK(manager->connections().size() == connections);
    CHECK(session.liveProject() == project && project->dataDb == model);
    CHECK(model->moProject == nullptr && slots->projectStorageAlive());
    CHECK(slots->nextStorageBoundary() == MoModelSlots::Slot::Operations);
    const auto retry = attemptCloseEmptyManagerTail(session, progress);
    CHECK(retry.stop == result.stop && !retry.clearCommittedThisAttempt);
    CHECK(manager->connections().size() == connections);
    CHECK(driver.fits == 0 && driver.clicks == 0);
}

TEST_CASE(close_empty_manager_tail_keeps_current_and_project_backlink_before_child_teardown)
{
    CountingDriver driver;
    ConversionSession session(driver);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C75/current.DB"));
    session.commitProjectModelConstruction();
    session.commitProjectModelSelection();
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    auto* project = session.liveProject();
    REQUIRE(manager->currentModel() == model.get() && model->moProject == project);
    CloseEmptyManagerTailProgress progress(session, closeEmptyManagerSourceOccurrence,
        "declared nonempty current child; no accepted-Close shortcut");
    const auto result = attemptCloseEmptyManagerTail(session, progress);
    CHECK(result.stop == CloseEmptyManagerTailStop::OwnedChildTeardownUnavailable);
    CHECK(manager->currentModel() == model.get());
    CHECK(manager->size() == 1 && manager->connections().size() == 2);
    CHECK(session.liveProject() == project && model->moProject == project);
}

TEST_CASE(close_empty_manager_tail_blocked_retry_cannot_use_external_clear_as_completed_child_effects)
{
    CountingDriver driver;
    ConversionSession session(driver);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C75/blocked.DB"));
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    CloseEmptyManagerTailProgress progress(session, closeEmptyManagerSourceOccurrence,
        "declared blocked child occurrence");
    attemptCloseEmptyManagerTail(session, progress);
    manager->clear(); // fixture mutation, not native child teardown evidence
    CHECK(refuses([&] { attemptCloseEmptyManagerTail(session, progress); }, "population changed"));
    CHECK(progress.stage() == CloseEmptyManagerTailStage::OwnedChildTeardownBlocked);
    CHECK(progress.blockedModelIdentity() == model.get());
    CHECK(progress.observedModelCount() == 1);
    CHECK(manager->size() == 0 && manager->currentModel() == nullptr);
    CHECK(session.project().dataDb == model);
}

TEST_CASE(close_empty_manager_tail_blocked_retry_preserves_replacement_with_same_count)
{
    CountingDriver driver;
    ConversionSession session(driver);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C75/old.DB"));
    auto manager = session.modelManagerOwner();
    auto original = session.project().dataDb; // retain old address for unambiguous fixture
    CloseEmptyManagerTailProgress progress(session, closeEmptyManagerSourceOccurrence,
        "declared blocked child before external replacement");
    attemptCloseEmptyManagerTail(session, progress);
    manager->clear();
    auto replacement = std::make_shared<DbHandle>();
    manager->appendCreatedModel(replacement);
    manager->registerModelSignals(replacement);
    REQUIRE(manager->selectModel(replacement.get()));
    CHECK(refuses([&] { attemptCloseEmptyManagerTail(session, progress); }, "population changed"));
    CHECK(manager->size() == 1 && manager->currentModel() == replacement.get());
    CHECK(manager->connections().size() == 2);
    CHECK(progress.blockedModelIdentity() == original.get());
}

TEST_CASE(close_empty_manager_tail_committed_retry_does_not_clear_later_arrival)
{
    CountingDriver driver;
    ConversionSession session(driver);
    CloseEmptyManagerTailProgress progress(session, closeEmptyManagerSourceOccurrence,
        "declared empty occurrence before later Open registration");
    REQUIRE(attemptCloseEmptyManagerTail(session, progress).clearCommittedThisAttempt);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C75/later.DB"));
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    REQUIRE(manager->selectModel(model.get()));
    CHECK(refuses([&] { attemptCloseEmptyManagerTail(session, progress); }, "population changed"));
    CHECK(progress.stage() == CloseEmptyManagerTailStage::BeforePostClearProjectLookup);
    CHECK(manager->size() == 1 && manager->currentModel() == model.get());
    CHECK(manager->connections().size() == 2 && session.project().dataDb == model);
}

TEST_CASE(close_empty_manager_tail_rejects_foreign_session_and_unqualified_occurrence)
{
    CountingDriver driver;
    ConversionSession first(driver), second(driver);
    CloseEmptyManagerTailProgress progress(first, closeEmptyManagerSourceOccurrence,
        "declared first Session cleanup occurrence");
    CHECK(refuses([&] { attemptCloseEmptyManagerTail(second, progress); }, "identity"));
    CHECK(progress.stage() == CloseEmptyManagerTailStage::InspectPopulation);
    CHECK(refuses([&] {
        CloseEmptyManagerTailProgress invalid(first, "another occurrence", "fixture");
    }, "occurrence/qualification"));
    CHECK(refuses([&] {
        CloseEmptyManagerTailProgress invalid(first, closeEmptyManagerSourceOccurrence, " \t\r\n");
    }, "occurrence/qualification"));
    CHECK(first.modelManagerOwner()->size() == 0 && second.modelManagerOwner()->size() == 0);
}

TEST_CASE(close_empty_manager_tail_escaped_progress_cannot_authorize_replacement_session)
{
    CountingDriver driver;
    std::optional<CloseEmptyManagerTailProgress> progress;
    std::shared_ptr<ModelManager> retainedManager;
    {
        ConversionSession first(driver);
        retainedManager = first.modelManagerOwner();
        progress.emplace(first, closeEmptyManagerSourceOccurrence,
            "declared old Session occurrence with retained diagnostic manager");
    }
    ConversionSession replacement(driver);
    REQUIRE(replacement.modelManagerOwner() != retainedManager);
    CHECK(refuses([&] { attemptCloseEmptyManagerTail(replacement, *progress); }, "identity"));
    CHECK(progress->stage() == CloseEmptyManagerTailStage::InspectPopulation);
    CHECK(replacement.modelManagerOwner()->size() == 0);
}

TEST_CASE(close_empty_manager_tail_preserves_pending_timer_identity_and_receiver)
{
    CountingDriver driver;
    ConversionSession session(driver);
    // Declared adapter binding fixture, not a captured original model lifecycle.
    auto sub = std::make_unique<SubProject>();
    sub->project = &session.project();
    auto* attached = sub.get();
    session.project().subs.push_back(std::move(sub));
    const auto first = session.commitNonNullDiagramBinding(*attached);
    const auto second = session.commitNonNullDiagramBinding(*attached);
    const auto pending = session.pending();
    REQUIRE(first != second && pending.size() == 2);
    CloseEmptyManagerTailProgress progress(session, closeEmptyManagerSourceOccurrence,
        "declared empty manager with independent pending receiver timers");
    REQUIRE(attemptCloseEmptyManagerTail(session, progress).clearCommittedThisAttempt);
    REQUIRE(session.pending().size() == pending.size());
    for (std::size_t index = 0; index < pending.size(); ++index) {
        CHECK(session.pending()[index].id == pending[index].id);
        CHECK(session.pending()[index].receiver == pending[index].receiver);
        CHECK(session.pending()[index].requestedIntervalMs == pending[index].requestedIntervalMs);
    }
    CHECK(driver.fits == 0 && driver.clicks == 0);
    // Still bound: the helper must not sneak in diagram detach/Project release.
    CHECK(refuses([&] { session.releaseProjectAfterCloseEffects(); }, "diagram null binding"));
    CHECK(session.liveProject() != nullptr);
}

int main() { return testkit::run_all(); }
