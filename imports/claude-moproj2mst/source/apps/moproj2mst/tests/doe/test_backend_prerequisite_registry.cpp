// ctest-labels: m2m-doe
// Explicit profiles and diagnostic stage hooks are adapter evidence only.
// They do not certify native map history, QObject startup or public B121 parity.
#include <testkit/check.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe/backend_prerequisite_registry.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/doe/flow_receiver_witness.hpp"

using namespace m2m;
using namespace m2m::doe;
namespace {
struct Rig {
    engine::DiagramEffectDriver diagram;
    engine::ConversionSession session{diagram};
    Rig() { initProject(session.project(), qt::L("C:/PROBLEM"), qt::L("backend_registry")); }
    std::shared_ptr<FlowReceiverWitness> first() const
    { return session.flowReceiverWitnessRegistryOwner()->orderedWitnesses().front(); }
    std::shared_ptr<FlowReceiverWitness> addSub()
    {
        auto& project = session.project();
        project.subs.push_back(makeSubProject(project, 0, 1));
        return session.flowReceiverWitnessRegistryOwner()->orderedWitnesses().back();
    }
};
template<class Work> bool unsupported(const Work& work)
{
    try { work(); }
    catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}
BackendPrerequisiteMapProfile emptyProfile(bool present)
{ return {{present, {}, "declared isolated same-key profile; not native closure"}, {}}; }
BackendPrerequisiteRecord record(std::uint64_t id, const std::shared_ptr<FlowReceiverWitness>& receiver,
                                 const char* method = "slotDiagModelCanAddItem")
{ return {id, receiver, method}; }
}

TEST_CASE(backend_registry_allocates_before_unavailable_map_and_removal_does_not)
{
    Rig rig;
    BackendPrerequisiteRegistry registry(rig.session);
    BackendCommitPrefix prefix(rig.session, rig.first());
    int backend;
    CHECK(unsupported([&] { prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, registry); }));
    REQUIRE(registry.retainedRecords().size() == 1);
    CHECK(registry.retainedRecords()[0].origin == BackendPrerequisiteRecordOrigin::RegistrationPrefix);
    CHECK(registry.retainedRecords()[0].bareMethod == "slotDiagModelCanAddItem");
    CHECK(registry.attempts().back().allocatedRecord.has_value());
    CHECK(registry.attempts().back().stop == BackendPrerequisiteStop::MapState);
    CHECK(!registry.observedMap() && !prefix.current(rig.session));
    CHECK(unsupported([&] { prefix.apply(rig.session, {2, BackendSetterSite::OldModelDetach, nullptr}, registry); }));
    CHECK(registry.retainedRecords().size() == 1);
    CHECK(!registry.attempts().back().allocatedRecord && !registry.attempts().back().keyInserted);
    registry.bindInitialMap(emptyProfile(false)); // later explicit evidence, no retry of occurrence1
    prefix.apply(rig.session, {3, BackendSetterSite::FirstNewModelAttach, &backend}, registry);
    CHECK(registry.retainedRecords().size() == 2);
    CHECK(registry.observedMap()->orderedRecordIds.size() == 1);
}

TEST_CASE(backend_registry_duplicate_attach_retains_two_allocations_and_one_entry)
{
    Rig rig;
    BackendPrerequisiteRegistry registry(rig.session);
    registry.bindInitialMap(emptyProfile(false));
    BackendCommitPrefix prefix(rig.session, rig.first());
    int backend;
    const auto owner = rig.session.sessionNotificationStateOwner();
    owner->suppressed = true;
    owner->flag90 = true; // registration is not dispatcher suppression/reset
    prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, registry);
    prefix.apply(rig.session, {1, BackendSetterSite::SecondNewModelAttach, &backend}, registry);
    REQUIRE(registry.retainedRecords().size() == 2);
    REQUIRE(registry.observedMap()->orderedRecordIds.size() == 1);
    CHECK(registry.observedMap()->orderedRecordIds[0] == registry.retainedRecords()[0].identity);
    CHECK(registry.attempts()[0].keyInserted && !registry.attempts()[0].duplicate);
    CHECK(registry.attempts()[1].duplicate && registry.attempts()[1].mapWritten);
    CHECK(owner->suppressed && owner->flag90 && owner->emptyKeys.empty());
    CHECK(prefix.current(rig.session).value() == &backend);
    CHECK(!rig.first()->backendWitness());
}

TEST_CASE(backend_registry_absent_removal_inserts_before_local_copy_and_commits_null)
{
    Rig rig;
    BackendPrerequisiteRegistry registry(rig.session);
    registry.bindInitialMap(emptyProfile(false));
    BackendCommitPrefix prefix(rig.session, rig.first());
    bool sawInsertion = false;
    registry.setDiagnosticObserver([&](auto stage, const auto& attempt) {
        if (stage == BackendPrerequisiteStage::KeyInserted) {
            sawInsertion = true;
            CHECK(registry.observedMap()->keyPresent && !attempt.snapshotTaken);
            CHECK(!prefix.current(rig.session));
        }
    });
    prefix.apply(rig.session, {1, BackendSetterSite::OldModelDetach, nullptr}, registry);
    CHECK(sawInsertion && registry.retainedRecords().empty());
    CHECK(registry.observedMap()->keyPresent && registry.observedMap()->orderedRecordIds.empty());
    CHECK(registry.attempts().back().mapWritten && registry.attempts().back().completed);
    CHECK(prefix.current(rig.session).has_value() && prefix.current(rig.session).value() == nullptr);
}

TEST_CASE(backend_registry_removes_aliases_of_first_record_not_distinct_equivalent_record)
{
    Rig rig;
    const auto other = rig.addSub();
    BackendPrerequisiteRegistry registry(rig.session);
    BackendPrerequisiteMapProfile profile{{true, {40, 10, 20, 10, 30}, "declared aliased pointer-list profile"},
        {record(10, rig.first()), record(20, rig.first()), record(30, other), record(40, rig.first(), "otherSlot")}};
    registry.bindInitialMap(std::move(profile));
    BackendCommitPrefix prefix(rig.session, rig.first());
    prefix.apply(rig.session, {1, BackendSetterSite::OldModelDetach, nullptr}, registry);
    CHECK(registry.observedMap()->orderedRecordIds == std::vector<std::uint64_t>({40, 20, 30}));
    CHECK(registry.attempts().back().removedRecord == 10);
    CHECK(registry.attempts().back().aliasesRemoved == 2);
    CHECK(registry.retainedRecords().size() == 4); // removed native pointer slots do not delete records
    prefix.apply(rig.session, {2, BackendSetterSite::OldModelDetach, nullptr}, registry);
    CHECK(registry.observedMap()->orderedRecordIds == std::vector<std::uint64_t>({40, 30}));
    prefix.apply(rig.session, {3, BackendSetterSite::OldModelDetach, nullptr}, registry);
    CHECK(!registry.attempts().back().removedRecord && registry.attempts().back().mapWritten);
}

TEST_CASE(backend_registry_retains_dead_guards_and_survivor_order)
{
    Rig rig;
    const auto dead = rig.first();
    rig.session.project().subs.clear();
    const auto live = rig.addSub();
    BackendPrerequisiteRegistry registry(rig.session);
    registry.bindInitialMap({{true, {1, 2, 1}, "declared dead-guard history"},
                             {record(1, dead), record(2, live)}});
    BackendCommitPrefix prefix(rig.session, live);
    prefix.apply(rig.session, {1, BackendSetterSite::OldModelDetach, nullptr}, registry);
    CHECK(registry.observedMap()->orderedRecordIds == std::vector<std::uint64_t>({1, 1}));
    CHECK(registry.retainedRecords().size() == 2 && !dead->nativeReceiverAlive());
}

TEST_CASE(backend_registry_guards_profile_owner_identity_and_replacement)
{
    Rig rig;
    Rig other;
    BackendPrerequisiteRegistry registry(rig.session);
    CHECK(unsupported([&] { registry.bindInitialMap({{true, {1}, "foreign"}, {record(1, other.first())}}); }));
    CHECK(!registry.observedMap() && registry.retainedRecords().empty());
    auto real = rig.first();
    auto foreignControl = std::shared_ptr<FlowReceiverWitness>(real.get(), [](FlowReceiverWitness*) {});
    CHECK(unsupported([&] { registry.bindInitialMap({{true, {1}, "foreign control"}, {record(1, foreignControl)}}); }));
    CHECK(unsupported([&] { registry.bindInitialMap({{false, {1}, "contradictory absent key"}, {record(1, real)}}); }));
    CHECK(unsupported([&] { registry.bindInitialMap({{true, {9}, "unavailable record"}, {}}); }));
    registry.bindInitialMap(emptyProfile(true));
    CHECK(unsupported([&] { registry.bindInitialMap(emptyProfile(false)); }));
    BackendCommitPrefix otherPrefix(other.session, other.first());
    int backend;
    CHECK(unsupported([&] { otherPrefix.apply(other.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, registry); }));
    CHECK(registry.attempts().back().stop == BackendPrerequisiteStop::Binding);
    CHECK(registry.retainedRecords().empty());
}

TEST_CASE(backend_registry_release_after_record_allocation_preserves_prefix)
{
    Rig rig;
    BackendPrerequisiteRegistry registry(rig.session);
    BackendCommitPrefix prefix(rig.session, rig.first());
    registry.setDiagnosticObserver([&](auto stage, const auto&) {
        if (stage == BackendPrerequisiteStage::RecordReady) rig.session.releaseProjectAfterCloseEffects();
    });
    int backend;
    CHECK(unsupported([&] { prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, registry); }));
    CHECK(registry.retainedRecords().size() == 1 && !registry.observedMap());
    CHECK(registry.attempts().back().stop == BackendPrerequisiteStop::GenerationChanged);
    CHECK(!prefix.current(rig.session) && !prefix.attempts().back().committed);
    CHECK(unsupported([&] { registry.bindInitialMap(emptyProfile(false)); }));
}

TEST_CASE(backend_registry_release_after_remove_insertion_keeps_present_empty_key)
{
    Rig rig;
    BackendPrerequisiteRegistry registry(rig.session);
    registry.bindInitialMap(emptyProfile(false));
    BackendCommitPrefix prefix(rig.session, rig.first());
    registry.setDiagnosticObserver([&](auto stage, const auto&) {
        if (stage == BackendPrerequisiteStage::KeyInserted) rig.session.releaseProjectAfterCloseEffects();
    });
    CHECK(unsupported([&] { prefix.apply(rig.session, {1, BackendSetterSite::OldModelDetach, nullptr}, registry); }));
    CHECK(registry.observedMap()->keyPresent && registry.observedMap()->orderedRecordIds.empty());
    CHECK(registry.attempts().back().keyInserted && !registry.attempts().back().snapshotTaken);
    CHECK(!registry.attempts().back().mapWritten && !prefix.attempts().back().committed);
}

TEST_CASE(backend_registry_release_after_writeback_keeps_map_and_refuses_backend_store)
{
    Rig rig;
    const auto escaped = rig.first();
    BackendPrerequisiteRegistry registry(rig.session);
    registry.bindInitialMap(emptyProfile(false));
    BackendCommitPrefix prefix(rig.session, escaped);
    registry.setDiagnosticObserver([&](auto stage, const auto&) {
        if (stage == BackendPrerequisiteStage::MapWritten) rig.session.releaseProjectAfterCloseEffects();
    });
    int backend;
    CHECK(unsupported([&] { prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, registry); }));
    CHECK(registry.observedMap()->keyPresent && registry.observedMap()->orderedRecordIds.size() == 1);
    CHECK(registry.attempts().back().mapWritten && registry.attempts().back().completed);
    CHECK(prefix.attempts().back().prerequisiteReturned);
    CHECK(prefix.attempts().back().stop == BackendCommitStop::GenerationChanged);
    CHECK(!escaped->nativeReceiverAlive() && !prefix.current(rig.session));
}

TEST_CASE(backend_registry_failure_after_local_removal_retains_old_backend_and_allocation)
{
    Rig rig;
    BackendPrerequisiteRegistry registry(rig.session);
    registry.bindInitialMap(emptyProfile(false));
    BackendCommitPrefix prefix(rig.session, rig.first());
    int backend;
    prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, registry);
    registry.setDiagnosticObserver([&](auto stage, const auto&) {
        if (stage == BackendPrerequisiteStage::LocalListReady) {
            rig.session.sessionNotificationStateOwner()->flag90 = true;
            throw Error(M2M_E_UNSUPPORTED, "declared pre-writeback effect refusal");
        }
    });
    CHECK(unsupported([&] { prefix.apply(rig.session, {2, BackendSetterSite::OldModelDetach, nullptr}, registry); }));
    CHECK(prefix.current(rig.session).value() == &backend);
    CHECK(registry.observedMap()->orderedRecordIds.size() == 1);
    CHECK(registry.attempts().back().removedRecord.has_value() && !registry.attempts().back().mapWritten);
    CHECK(registry.attempts().back().stop == BackendPrerequisiteStop::Effect);
    CHECK(rig.session.sessionNotificationStateOwner()->flag90);
}

TEST_CASE(backend_registry_guards_unrecovered_context_before_allocation)
{
    Rig rig;
    BackendPrerequisiteRegistry registry(rig.session);
    const auto receiver = rig.first();
    auto& project = rig.session.project();
    int backend;
    BackendSetterPrerequisiteContext context{*rig.session.sessionNotificationStateOwner(), *receiver,
        project, *project.subs[0], {1, BackendSetterSite::FirstNewModelAttach, &backend}};
    context.prepend = true;
    CHECK(unsupported([&] { registry.registerCanAddItem(context); }));
    context.prepend = false;
    context.slot = "1otherSlot()";
    CHECK(unsupported([&] { registry.registerCanAddItem(context); }));
    context.slot = "1slotDiagModelCanAddItem()";
    context.signal = "2otherSignal()";
    CHECK(unsupported([&] { registry.registerCanAddItem(context); }));
    context.signal = "2signalDiagModelCanAddItem()";
    CHECK(unsupported([&] { registry.removeCanAddItem(context); }));
    CHECK(registry.retainedRecords().empty() && !registry.observedMap());
    for (const auto& attempt : registry.attempts()) CHECK(attempt.stop == BackendPrerequisiteStop::Context);
}

TEST_CASE(backend_registry_does_not_qualify_set_opr_population)
{
    Rig rig;
    auto& project = rig.session.project();
    project.doeIo = makeRecordingStudyIo();
    project.doeIo->bindSessionNotificationState(rig.session.sessionNotificationStateOwner());
    BackendPrerequisiteRegistry registry(rig.session);
    registry.bindInitialMap(emptyProfile(false));
    BackendCommitPrefix prefix(rig.session, rig.first());
    prefix.apply(rig.session, {1, BackendSetterSite::OldModelDetach, nullptr}, registry);
    auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    CHECK(!binding->notifier().registrations && !rig.first()->backendWitness());
    rig.session.sessionNotificationStateOwner()->flag90 = true;
    CHECK(unsupported([&] { binding->dispatch(rig.session, 1, true); }));
    CHECK(!rig.session.sessionNotificationStateOwner()->flag90);
    CHECK(binding->notifier().attempts.back().stop == SetOprHasDoeStop::Population);
}

int main() { return testkit::run_all(); }
