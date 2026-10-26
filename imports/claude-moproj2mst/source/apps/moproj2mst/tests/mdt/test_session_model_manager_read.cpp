// ctest-labels: m2m-mdt session-model-manager-read
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/engine/mo_model_slots.hpp"
#include "m2m/engine/session_model_manager_read.hpp"
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
static_assert(!std::is_copy_constructible_v<SessionModelManagerReadReceipt>);
static_assert(!std::is_move_constructible_v<SessionModelManagerReadReceipt>);
static_assert(!std::is_constructible_v<SessionModelManagerReadReceipt,
    const ConversionSession&, const std::shared_ptr<ModelManager>&>);
static_assert(!SessionModelManagerReadReceipt::nativeFactoryCompleted);
static_assert(!SessionModelManagerReadReceipt::nativeConcreteMoClassObserved);
static_assert(!SessionModelManagerReadReceipt::nativeWrapperQualified);
static_assert(!SessionModelManagerReadReceipt::mainWindowReady);
static_assert(!SessionModelManagerReadReceipt::completeForPublicClose);
static_assert(std::is_same_v<decltype(std::declval<const SessionModelManagerReadReceipt&>()
    .observedManagerIdentityToken()), std::uintptr_t>);

TEST_CASE(session_manager_open_read_is_absent_before_actual_open_edge)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    REQUIRE(session.modelManagerOwner());
    CHECK(!session.modelManagerOpenReadReceipt());
    CHECK(session.modelManagerOwner()->size() == 0);
    CHECK(session.modelManagerOwner()->ownerSession() == &session);
    CHECK(refuses([&] { session.currentViewManagerOwner(); }, "View_Default/Primary"));
}

TEST_CASE(session_manager_open_read_binds_same_actual_manager_before_child_registration)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    const auto manager = session.modelManagerOwner();
    session.prepareProjectModel(qt::L("C:/PROBLEM/C78/read.DB"));
    const auto receipt = session.modelManagerOpenReadReceipt();
    REQUIRE(receipt);
    CHECK(receipt->origin() == SessionModelManagerReadOrigin::CandidateExistingOwnerRead);
    CHECK(std::string(receipt->sourceSite).find("0x140c9b7ca") != std::string::npos);
    CHECK(std::string(receipt->qualification).find("candidate") != std::string::npos);
    CHECK(receipt->observedManagerIdentityToken() == reinterpret_cast<std::uintptr_t>(manager.get()));
    CHECK(receipt->requireManagerFor(session) == manager);
    CHECK(receipt->sessionScopeAvailable());
    CHECK(receipt->modelCountAtRead() == 0 && receipt->connectionCountAtRead() == 0);
    CHECK(session.modelManagerOwner() == manager && manager->ownerSession() == &session);
    REQUIRE(manager->size() == 1 && manager->connections().size() == 2);
    auto model = session.project().dataDb;
    REQUIRE(model);
    CHECK(manager->modelAt(0) == model.get() && model->modelId == 1);
    CHECK(model->kind == DbHandle::Kind::Mo && model->moProject == nullptr);
    CHECK(manager->currentModel() == nullptr && manager->currentModelId() == -1);
    for (const auto& connection : manager->connections()) {
        CHECK(connection.sender.lock() == model);
        CHECK(connection.receiver == &session && connection.rawConnectionType == 3);
    }
    CHECK(refuses([&] { session.currentViewManagerOwner(); }, "View_Default/Primary"));
}

TEST_CASE(session_manager_open_read_survives_later_project_and_io_binding_without_selecting_early)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C78/selected.DB"));
    auto receipt = session.modelManagerOpenReadReceipt();
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    initProject(session.project(), qt::L("C:/PROBLEM/C78"), qt::L("selected"));
    session.commitProjectModelConstruction();
    auto io = doe::makeRecordingStudyIo();
    io->bindModelManager(manager);
    CHECK(io->modelManager() == receipt->requireManagerFor(session).get());
    CHECK(model->moProject == session.liveProject());
    CHECK(manager->currentModel() == nullptr);
    session.commitProjectModelSelection();
    CHECK(manager->currentModel() == model.get());
    CHECK(session.modelManagerOpenReadReceipt() == receipt);
    CHECK(receipt->modelCountAtRead() == 0); // historical read, not current population
    CHECK(receipt->requireManagerFor(session)->size() == 1);
}

TEST_CASE(session_manager_open_read_rejected_second_prepare_retains_receipt_and_original_child)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C78/first.DB"));
    auto receipt = session.modelManagerOpenReadReceipt();
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    CHECK(refuses([&] {
        session.prepareProjectModel(qt::L("C:/PROBLEM/C78/second.DB"));
    }, "fresh Open"));
    CHECK(session.modelManagerOpenReadReceipt() == receipt);
    CHECK(receipt->requireManagerFor(session) == manager);
    CHECK(manager->size() == 1 && manager->modelAt(0) == model.get());
    CHECK(manager->connections().size() == 2 && manager->currentModel() == nullptr);
    CHECK(session.project().dataDb == model && model->moProject == nullptr);
}

TEST_CASE(session_manager_open_read_later_selection_guard_does_not_rewrite_factory_evidence)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C78/unbound.DB"));
    auto receipt = session.modelManagerOpenReadReceipt();
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    CHECK(refuses([&] { session.commitProjectModelSelection(); }, "completed actual Project binding"));
    CHECK(session.modelManagerOpenReadReceipt() == receipt && receipt->sessionScopeAvailable());
    CHECK(receipt->requireManagerFor(session) == manager);
    CHECK(model->moProject == nullptr && manager->currentModel() == nullptr);
    CHECK(manager->size() == 1 && manager->connections().size() == 2);
}

TEST_CASE(session_manager_open_read_foreign_session_use_preserves_both_actual_owners)
{
    DiagramEffectDriver effects;
    ConversionSession first(effects), second(effects);
    first.prepareProjectModel(qt::L("C:/PROBLEM/C78/owner.DB"));
    auto receipt = first.modelManagerOpenReadReceipt();
    CHECK(refuses([&] { receipt->requireManagerFor(second); }, "identity"));
    CHECK(receipt->requireManagerFor(first) == first.modelManagerOwner());
    CHECK(first.modelManagerOwner()->size() == 1 && second.modelManagerOwner()->size() == 0);
    CHECK(!second.modelManagerOpenReadReceipt());
}

TEST_CASE(session_manager_open_read_revokes_before_escaped_cpp_leases_can_authorize_new_session)
{
    DiagramEffectDriver effects;
    std::shared_ptr<const SessionModelManagerReadReceipt> receipt;
    std::shared_ptr<ModelManager> retained;
    {
        ConversionSession first(effects);
        first.prepareProjectModel(qt::L("C:/PROBLEM/C78/old.DB"));
        receipt = first.modelManagerOpenReadReceipt();
        retained = first.modelManagerOwner();
        REQUIRE(receipt->sessionScopeAvailable());
    }
    CHECK(!receipt->sessionScopeAvailable());
    CHECK(receipt->observedManagerIdentityToken() == reinterpret_cast<std::uintptr_t>(retained.get()));
    ConversionSession replacement(effects);
    CHECK(refuses([&] { receipt->requireManagerFor(replacement); }, "scope was revoked"));
    CHECK(!replacement.modelManagerOpenReadReceipt());
    CHECK(replacement.modelManagerOwner() != retained);
}

TEST_CASE(session_manager_open_read_escaped_receipt_has_only_an_opaque_token_after_manager_destruction)
{
    DiagramEffectDriver effects;
    std::shared_ptr<const SessionModelManagerReadReceipt> receipt;
    std::weak_ptr<ModelManager> manager;
    std::uintptr_t token = 0;
    {
        ConversionSession first(effects);
        first.prepareProjectModel(qt::L("C:/PROBLEM/C78/expired.DB"));
        receipt = first.modelManagerOpenReadReceipt();
        manager = first.modelManagerOwner();
        token = receipt->observedManagerIdentityToken();
        REQUIRE(token != 0);
    }
    CHECK(manager.expired()); // no retained strong manager lease in this case
    CHECK(!receipt->sessionScopeAvailable());
    CHECK(receipt->observedManagerIdentityToken() == token);
    ConversionSession replacement(effects);
    CHECK(refuses([&] { receipt->requireManagerFor(replacement); }, "scope was revoked"));
    CHECK(!replacement.modelManagerOpenReadReceipt());
}

TEST_CASE(session_manager_open_read_is_not_a_frozen_population_or_project_absence_witness)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C78/clear.DB"));
    auto receipt = session.modelManagerOpenReadReceipt();
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    manager->clear(); // declared adapter mutation, not native Close evidence
    CHECK(receipt->requireManagerFor(session) == manager);
    CHECK(receipt->sessionScopeAvailable() && manager->size() == 0);
    CHECK(session.project().dataDb == model && session.liveProject() != nullptr);
    CHECK(!receipt->nativeFactoryCompleted && !receipt->nativeWrapperQualified);
}

TEST_CASE(session_manager_open_read_preserves_unrelated_session_owners_and_pending_events)
{
    DiagramEffectDriver effects; // callbacks refuse; the read must deliver none
    ConversionSession session(effects);
    auto* project = session.liveProject();
    auto slots = project->modelSlotsOwner();
    auto main = session.mainViewOwner();
    auto runtime = session.widgetRuntimeOwner();
    auto notifications = session.sessionNotificationStateOwner();
    notifications->flag90 = true;
    notifications->suppressed = true;
    const auto attempts = notifications->attempts.size();
    session.remeshState().enabled = true;
    session.remeshState().filterDistance = 17.0;
    // Candidate-only diagram fixture; no native timer schedule is inferred.
    auto sub = std::make_unique<SubProject>();
    sub->project = project;
    auto* attached = sub.get();
    project->subs.push_back(std::move(sub));
    session.commitNonNullDiagramBinding(*attached);
    const auto pending = session.pending();
    session.prepareProjectModel(qt::L("C:/PROBLEM/C78/retained.DB"));
    REQUIRE(session.modelManagerOpenReadReceipt());
    CHECK(session.liveProject() == project && project->modelSlotsOwner() == slots);
    CHECK(slots->projectStorageAlive() && slots->nextStorageBoundary() == MoModelSlots::Slot::Operations);
    CHECK(session.mainViewOwner() == main && session.widgetRuntimeOwner() == runtime);
    CHECK(main->widgetState()->nativeOwnerAlive());
    CHECK(notifications->flag90 && notifications->suppressed);
    CHECK(notifications->attempts.size() == attempts);
    CHECK(session.remeshState().enabled && session.remeshState().filterDistance == 17.0);
    REQUIRE(session.pending().size() == pending.size());
    CHECK(session.pending()[0].id == pending[0].id);
    CHECK(session.pending()[0].receiver == pending[0].receiver);
    CHECK(refuses([&] { session.currentViewManagerOwner(); }, "View_Default/Primary"));
}

int main() { return testkit::run_all(); }
