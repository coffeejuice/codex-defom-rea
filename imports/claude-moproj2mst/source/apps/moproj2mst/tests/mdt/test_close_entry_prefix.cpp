// ctest-labels: m2m-mdt close-entry-prefix
#include <testkit/check.hpp>

#include <memory>
#include <string>

#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/close_entry_prefix.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
CloseEntryBusyObservations unbusy(const ConversionSession& session)
{
    return {&session, closeEntrySourceOccurrence,
            CloseEntryByteObservation{0, "declared fixture global+9 at normal Close entry"},
            CloseEntryBoolObservation{true, "declared fixture global window belongs to this session"},
            CloseEntryBoolObservation{false, "declared fixture resolved main virtual+300 false"}};
}

template<class F> bool refusesOwner(F&& body)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find("owner binding") != std::string::npos;
    }
    return false;
}
}

TEST_CASE(close_prefix_actual_uncommitted_mo_project_allows_leaf_then_stops_before_busy_effects)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/ClosePrefix/fixture.DB"));
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    const auto facts = unbusy(session);
    REQUIRE(session.liveProject() != nullptr); // adapter storage is not native+19c0
    REQUIRE(model && model->moProject == nullptr);
    CHECK(manager->currentModel() == nullptr); // scan owned list, not just current
    const auto connections = manager->connections().size();
    const auto pending = session.pending().size();
    auto* project = session.liveProject();
    const auto result = attemptCloseEntryPrefix(session, facts);
    CHECK(result.stop == CloseEntryPrefixStop::BeforeBusyTransitionEffects);
    CHECK(result.allowedMoChildren == 1);
    CHECK(!result.pendingModelOrdinal);
    CHECK(facts.globalFlag9->value == 0);
    CHECK(manager->size() == 1);
    CHECK(manager->currentModel() == nullptr);
    CHECK(manager->connections().size() == connections);
    CHECK(session.pending().size() == pending);
    CHECK(session.liveProject() == project);
    CHECK(model->moProject == nullptr);
}

TEST_CASE(close_prefix_nonzero_global_busy_short_circuits_unknown_window_and_preserves_warning_guard)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CloseEntryBusyObservations facts;
    facts.sessionOwner = &session;
    facts.sourceOccurrence = closeEntrySourceOccurrence;
    facts.globalFlag9 = CloseEntryByteObservation{7, "declared fixture nonzero native busy byte"};
    const auto result = attemptCloseEntryPrefix(session, facts);
    CHECK(result.stop == CloseEntryPrefixStop::BusyWarningEffectsUnavailable);
    CHECK(result.allowedMoChildren == 0);
    CHECK(!facts.globalMainWindowPresent && !facts.mainBusy300);
    CHECK(facts.globalFlag9->value == 7);
    CHECK(session.liveProject() != nullptr);
}

TEST_CASE(close_prefix_missing_busy_observations_are_not_assumed_fresh_false)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto facts = unbusy(session);
    facts.globalFlag9.reset();
    CHECK(attemptCloseEntryPrefix(session, facts).stop ==
          CloseEntryPrefixStop::BusyObservationUnavailable);
    facts = unbusy(session);
    facts.mainBusy300.reset();
    CHECK(attemptCloseEntryPrefix(session, facts).stop ==
          CloseEntryPrefixStop::BusyObservationUnavailable);
    facts = unbusy(session);
    facts.mainBusy300->qualification.clear();
    CHECK(attemptCloseEntryPrefix(session, facts).stop ==
          CloseEntryPrefixStop::BusyObservationUnavailable);
    facts = unbusy(session);
    facts.sourceOccurrence = "unqualified other entry";
    CHECK(attemptCloseEntryPrefix(session, facts).stop ==
          CloseEntryPrefixStop::BusyObservationUnavailable);
}

TEST_CASE(close_prefix_qualified_null_global_window_skips_virtual_busy_query)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto facts = unbusy(session);
    facts.globalMainWindowPresent = CloseEntryBoolObservation{false, "declared fixture null global main window"};
    facts.mainBusy300.reset();
    const auto result = attemptCloseEntryPrefix(session, facts);
    CHECK(result.stop == CloseEntryPrefixStop::BeforeBusyTransitionEffects);
    CHECK(result.allowedMoChildren == 0); // actual empty owned manager
}

TEST_CASE(close_prefix_true_main_busy_also_stops_at_modal_warning_dependency)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto facts = unbusy(session);
    facts.mainBusy300->value = true;
    CHECK(attemptCloseEntryPrefix(session, facts).stop ==
          CloseEntryPrefixStop::BusyWarningEffectsUnavailable);
    CHECK(facts.globalFlag9->value == 0);
}

TEST_CASE(close_prefix_committed_actual_project_stops_before_process_type_or_question)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/ClosePrefix/committed.DB"));
    initProject(session.project(), qt::L("C:/PROBLEM/ClosePrefix"), qt::L("committed"));
    session.commitProjectModelConstruction();
    auto* project = session.liveProject();
    auto model = session.project().dataDb;
    const auto result = attemptCloseEntryPrefix(session, unbusy(session));
    CHECK(result.stop == CloseEntryPrefixStop::MoProjectCanCloseUnavailable);
    CHECK(result.allowedMoChildren == 0);
    REQUIRE(result.pendingModelOrdinal);
    CHECK(*result.pendingModelOrdinal == 0);
    CHECK(session.liveProject() == project);
    CHECK(model->moProject == project);
}

TEST_CASE(close_prefix_other_child_virtual_guard_is_not_treated_as_mo_null_project)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto manager = session.modelManagerOwner();
    auto base = std::make_shared<DbHandle>(); // declared adapter population, native base guard unavailable
    manager->appendCreatedModel(base);
    const auto result = attemptCloseEntryPrefix(session, unbusy(session));
    CHECK(result.stop == CloseEntryPrefixStop::ModelVirtualCanCloseUnavailable);
    CHECK(result.allowedMoChildren == 0);
    REQUIRE(result.pendingModelOrdinal);
    CHECK(*result.pendingModelOrdinal == 0);
    CHECK(manager->size() == 1);
}

TEST_CASE(close_prefix_observations_require_the_actual_session_manager_owner)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto facts = unbusy(session);
    facts.sessionOwner = nullptr;
    CHECK(refusesOwner([&] { attemptCloseEntryPrefix(session, facts); }));
    facts = unbusy(session);
    facts.globalFlag9->qualification = "   ";
    CHECK(attemptCloseEntryPrefix(session, facts).stop == CloseEntryPrefixStop::BusyObservationUnavailable);
}

int main() { return testkit::run_all(); }
