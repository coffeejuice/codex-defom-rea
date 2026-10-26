// ctest-labels: m2m-mdt close-postclear-project-lookup
#include <testkit/check.hpp>

#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/close_postclear_project_lookup.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/mo_model_slots.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/engine/view_manager.hpp"
#include "m2m/model.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
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
static_assert(!std::is_copy_constructible_v<ClosePostClearProjectProgress>);
static_assert(!std::is_move_constructible_v<ClosePostClearProjectProgress>);
static_assert(!ClosePostClearProjectResult::nativeWrapperQualified);
static_assert(!ClosePostClearProjectResult::completeForPublicClose);

TEST_CASE(close_postclear_project_leaf_is_null_while_adapter_project_stays_live)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto* project = session.liveProject();
    auto* scope = currentProject();
    REQUIRE(project);
    CloseEmptyManagerTailProgress clear(session, closeEmptyManagerSourceOccurrence,
        "declared empty-manager occurrence");
    REQUIRE(attemptCloseEmptyManagerTail(session, clear).clearCommittedThisAttempt);
    ClosePostClearProjectProgress lookup(session, clear, closePostClearProjectSourceOccurrence,
        "declared conditional inner null-current leaf; wrapper binding unavailable");
    CHECK(lookup.stage() == ClosePostClearProjectStage::AwaitNullCurrentLeaf);
    const auto result = attemptClosePostClearProjectLookup(session, lookup);
    CHECK(result.lookupCommittedThisAttempt);
    CHECK(result.project == nullptr);
    CHECK(result.stop == ClosePostClearProjectStop::NativeWrapperAndMainWindowEffectsUnavailable);
    CHECK(std::string(result.nextSourceSite).find("0x1400133c6") == 0);
    CHECK(std::string(result.nextSourceSite).find("0x140029a93") != std::string::npos);
    CHECK(lookup.stage() == ClosePostClearProjectStage::NullProjectLeafReached);
    CHECK(session.liveProject() == project && currentProject() == scope);
    const auto retry = attemptClosePostClearProjectLookup(session, lookup);
    CHECK(!retry.lookupCommittedThisAttempt && retry.project == nullptr);
    CHECK(retry.stop == result.stop);
    CHECK(!attemptCloseEmptyManagerTail(session, clear).clearCommittedThisAttempt);
}

TEST_CASE(close_postclear_project_admission_cannot_advance_unstarted_empty_clear)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseEmptyManagerTailProgress clear(session, closeEmptyManagerSourceOccurrence, "declared entry");
    CHECK(refuses([&] {
        ClosePostClearProjectProgress lookup(session, clear, closePostClearProjectSourceOccurrence,
            "declared lookup without completed prerequisite");
    }, "completed empty-manager boundary"));
    CHECK(clear.stage() == CloseEmptyManagerTailStage::InspectPopulation);
    CHECK(session.modelManagerOwner()->size() == 0);
    CHECK(session.liveProject() != nullptr);
}

TEST_CASE(close_postclear_project_admission_preserves_registered_child_at_blocked_clear)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C76/blocked.DB"));
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    REQUIRE(model && model->moProject == nullptr);
    CloseEmptyManagerTailProgress clear(session, closeEmptyManagerSourceOccurrence,
        "declared failed-Open registered child");
    REQUIRE(attemptCloseEmptyManagerTail(session, clear).stop ==
        CloseEmptyManagerTailStop::OwnedChildTeardownUnavailable);
    CHECK(refuses([&] {
        ClosePostClearProjectProgress lookup(session, clear, closePostClearProjectSourceOccurrence,
            "declared lookup after blocked child teardown");
    }, "completed empty-manager boundary"));
    CHECK(manager->size() == 1 && manager->modelAt(0) == model.get());
    CHECK(manager->connections().size() == 2 && session.project().dataDb == model);
    CHECK(clear.stage() == CloseEmptyManagerTailStage::OwnedChildTeardownBlocked);
}

TEST_CASE(close_postclear_project_admission_revalidates_population_after_completed_clear)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseEmptyManagerTailProgress clear(session, closeEmptyManagerSourceOccurrence, "declared entry");
    REQUIRE(attemptCloseEmptyManagerTail(session, clear).clearCommittedThisAttempt);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C76/arrived.DB"));
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    REQUIRE(manager->selectModel(model.get()));
    CHECK(refuses([&] {
        ClosePostClearProjectProgress lookup(session, clear, closePostClearProjectSourceOccurrence,
            "declared stale empty boundary");
    }, "population changed"));
    CHECK(manager->size() == 1 && manager->currentModel() == model.get());
    CHECK(manager->connections().size() == 2);
}

TEST_CASE(close_postclear_project_first_attempt_does_not_clear_arrival_after_admission)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseEmptyManagerTailProgress clear(session, closeEmptyManagerSourceOccurrence, "declared entry");
    attemptCloseEmptyManagerTail(session, clear);
    ClosePostClearProjectProgress lookup(session, clear, closePostClearProjectSourceOccurrence,
        "declared inner leaf admission");
    session.prepareProjectModel(qt::L("C:/PROBLEM/C76/before.DB"));
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    session.commitProjectModelConstruction();
    session.commitProjectModelSelection();
    CHECK(refuses([&] { attemptClosePostClearProjectLookup(session, lookup); }, "population changed"));
    CHECK(lookup.stage() == ClosePostClearProjectStage::AwaitNullCurrentLeaf);
    CHECK(manager->currentModel() == model.get() && model->moProject == session.liveProject());
    CHECK(manager->size() == 1 && manager->connections().size() == 2);
}

TEST_CASE(close_postclear_project_retry_retains_null_leaf_and_preserves_later_current_child)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseEmptyManagerTailProgress clear(session, closeEmptyManagerSourceOccurrence, "declared entry");
    attemptCloseEmptyManagerTail(session, clear);
    ClosePostClearProjectProgress lookup(session, clear, closePostClearProjectSourceOccurrence,
        "declared inner leaf occurrence");
    REQUIRE(attemptClosePostClearProjectLookup(session, lookup).project == nullptr);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C76/after.DB"));
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    REQUIRE(manager->selectModel(model.get()));
    CHECK(refuses([&] { attemptClosePostClearProjectLookup(session, lookup); }, "population changed"));
    CHECK(lookup.stage() == ClosePostClearProjectStage::NullProjectLeafReached);
    CHECK(manager->currentModel() == model.get() && manager->connections().size() == 2);
    CHECK(session.project().dataDb == model);
}

TEST_CASE(close_postclear_project_rejects_foreign_clear_and_foreign_attempt_without_effects)
{
    DiagramEffectDriver effects;
    ConversionSession first(effects), second(effects);
    CloseEmptyManagerTailProgress clear(first, closeEmptyManagerSourceOccurrence, "declared first entry");
    attemptCloseEmptyManagerTail(first, clear);
    CHECK(refuses([&] {
        ClosePostClearProjectProgress lookup(second, clear, closePostClearProjectSourceOccurrence,
            "declared foreign prerequisite");
    }, "identity"));
    ClosePostClearProjectProgress lookup(first, clear, closePostClearProjectSourceOccurrence,
        "declared first inner leaf");
    CHECK(refuses([&] { attemptClosePostClearProjectLookup(second, lookup); }, "identity"));
    CHECK(lookup.stage() == ClosePostClearProjectStage::AwaitNullCurrentLeaf);
    CHECK(first.modelManagerOwner()->size() == 0 && second.modelManagerOwner()->size() == 0);
}

TEST_CASE(close_postclear_project_rejects_missing_provenance_and_wrong_source_occurrence)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseEmptyManagerTailProgress clear(session, closeEmptyManagerSourceOccurrence, "declared entry");
    attemptCloseEmptyManagerTail(session, clear);
    CHECK(refuses([&] {
        ClosePostClearProjectProgress lookup(session, clear, "some other site", "fixture");
    }, "occurrence/qualification"));
    CHECK(refuses([&] {
        ClosePostClearProjectProgress lookup(session, clear, closePostClearProjectSourceOccurrence,
            " \r\n\t");
    }, "occurrence/qualification"));
    CHECK(clear.stage() == CloseEmptyManagerTailStage::BeforePostClearProjectLookup);
}

TEST_CASE(close_postclear_project_owns_its_binding_after_predecessor_progress_storage_ends)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    std::optional<ClosePostClearProjectProgress> lookup;
    {
        CloseEmptyManagerTailProgress clear(session, closeEmptyManagerSourceOccurrence, "declared entry");
        attemptCloseEmptyManagerTail(session, clear);
        lookup.emplace(session, clear, closePostClearProjectSourceOccurrence,
            "declared admitted inner leaf, independent diagnostic progress storage");
    }
    const auto result = attemptClosePostClearProjectLookup(session, *lookup);
    CHECK(result.lookupCommittedThisAttempt && result.project == nullptr);
    CHECK(session.liveProject() != nullptr);
}

TEST_CASE(close_postclear_project_escaped_lookup_cannot_authorize_replacement_session)
{
    DiagramEffectDriver effects;
    std::optional<ClosePostClearProjectProgress> lookup;
    std::shared_ptr<ModelManager> oldManager;
    {
        ConversionSession first(effects);
        oldManager = first.modelManagerOwner();
        CloseEmptyManagerTailProgress clear(first, closeEmptyManagerSourceOccurrence, "declared entry");
        attemptCloseEmptyManagerTail(first, clear);
        lookup.emplace(first, clear, closePostClearProjectSourceOccurrence, "declared old inner leaf");
    }
    ConversionSession replacement(effects);
    REQUIRE(replacement.modelManagerOwner() != oldManager);
    CHECK(refuses([&] { attemptClosePostClearProjectLookup(replacement, *lookup); }, "identity"));
    CHECK(lookup->stage() == ClosePostClearProjectStage::AwaitNullCurrentLeaf);
    CHECK(replacement.modelManagerOwner()->size() == 0);
}

TEST_CASE(close_postclear_project_leaf_preserves_runtime_notifications_remesh_and_attached_timers)
{
    DiagramEffectDriver effects; // its callbacks refuse; helper must invoke none
    ConversionSession session(effects);
    auto* project = session.liveProject();
    auto slots = project->modelSlotsOwner();
    auto primary = session.primarySceneOwner();
    auto secondary = session.secondarySceneOwner();
    auto main = session.mainViewOwner();
    auto runtime = session.widgetRuntimeOwner();
    auto qobjects = runtime->qobjectCount();
    auto widgets = runtime->currentWidgetCount();
    auto notifications = session.sessionNotificationStateOwner();
    notifications->suppressed = true;
    notifications->flag90 = true;
    const auto attempts = notifications->attempts.size();
    session.remeshState().enabled = true;
    session.remeshState().filterDistance = 12.0;
    // Declared adapter binding fixture, not an original timer/cast observation.
    auto sub = std::make_unique<SubProject>();
    sub->project = project;
    auto* attached = sub.get();
    project->subs.push_back(std::move(sub));
    session.commitNonNullDiagramBinding(*attached);
    const auto pending = session.pending();
    REQUIRE(pending.size() == 1);
    CloseEmptyManagerTailProgress clear(session, closeEmptyManagerSourceOccurrence, "declared entry");
    attemptCloseEmptyManagerTail(session, clear);
    ClosePostClearProjectProgress lookup(session, clear, closePostClearProjectSourceOccurrence,
        "declared isolated null leaf with independent application owners");
    CHECK(attemptClosePostClearProjectLookup(session, lookup).project == nullptr);
    CHECK(session.liveProject() == project && project->modelSlotsOwner() == slots);
    CHECK(slots->projectStorageAlive() && slots->nextStorageBoundary() == MoModelSlots::Slot::Operations);
    CHECK(session.primarySceneOwner() == primary && session.secondarySceneOwner() == secondary);
    CHECK(session.mainViewOwner() == main && main->widgetState()->nativeOwnerAlive());
    CHECK(session.widgetRuntimeOwner() == runtime && runtime->qobjectCount() == qobjects);
    CHECK(runtime->currentWidgetCount() == widgets);
    CHECK(notifications->suppressed && notifications->flag90);
    CHECK(notifications->attempts.size() == attempts);
    CHECK(session.remeshState().enabled && session.remeshState().filterDistance == 12.0);
    REQUIRE(session.pending().size() == pending.size());
    CHECK(session.pending()[0].id == pending[0].id);
    CHECK(session.pending()[0].receiver == pending[0].receiver);
    CHECK(session.pending()[0].requestedIntervalMs == pending[0].requestedIntervalMs);
    CHECK(refuses([&] { session.releaseProjectAfterCloseEffects(); }, "diagram null binding"));
}

int main() { return testkit::run_all(); }
