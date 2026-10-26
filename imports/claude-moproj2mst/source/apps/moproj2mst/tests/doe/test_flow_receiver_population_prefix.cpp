// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <optional>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/doe/flow_receiver_population_prefix.hpp"

using namespace m2m;
using namespace m2m::doe;
namespace {
bool unsupported(const auto& work)
{
    try { work(); }
    catch (const Error& e) { return e.code == M2M_E_UNSUPPORTED; }
    return false;
}
struct Rig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    void initialize() { initProject(session.project(), qt::L("C:/PROBLEM"), qt::L("flow_prefix")); }
    void bindIo()
    {
        session.project().doeIo = makeRecordingStudyIo();
        session.project().doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
    }
};
}

TEST_CASE(flow_receiver_population_prefix_pre_sub0_is_empty_and_incomplete_without_effects)
{
    Rig rig;
    const auto owner = rig.session.sessionNotificationStateOwner();
    REQUIRE(owner->createTaskBaseRegistrations);
    const auto registrationCount = owner->createTaskBaseRegistrations->entries.size();
    owner->flag90 = true;
    const auto prefix = projectFlowReceiverPopulationPrefix(rig.session);
    CHECK(prefix.entries().empty() && !prefix.completeNativePopulation());
    CHECK(prefix.owner() == owner && !rig.session.project().doeIo);
    CHECK(owner->flag90 && owner->emptyKeys.empty() && owner->attempts.empty());
    CHECK(owner->createTaskBaseRegistrations &&
          owner->createTaskBaseRegistrations->entries.size() == registrationCount);
    prefix.validateAgainst(rig.session);
}

TEST_CASE(flow_receiver_population_prefix_retains_actual_sub_order_including_inactive_models)
{
    Rig rig;
    rig.initialize();
    rig.bindIo();
    auto& project = rig.session.project();
    project.subs.push_back(makeSubProject(project, 2, 8));
    project.subs.push_back(makeSubProject(project, 0, 9));
    project.activeSub = 1;
    const auto prefix = projectFlowReceiverPopulationPrefix(rig.session);
    REQUIRE(prefix.entries().size() == 3);
    CHECK(project.subs[1]->ownedDoeStudy);
    for (std::size_t i = 0; i < project.subs.size(); ++i) {
        const auto& entry = prefix.entries()[i];
        const auto receiver = entry.registration.receiver.lock();
        REQUIRE(receiver);
        CHECK(entry.constructorOrdinal == i);
        CHECK(entry.source == FlowReceiverConstructionSource::NormalSub);
        CHECK(entry.registrationSite == 0x140071166ULL);
        CHECK(entry.identityToken == &project.subs[i]->flow);
        CHECK(entry.registration.bareMethod == "slotSetOprHasDoe");
        CHECK(receiver->actualSubProject() == project.subs[i].get());
        CHECK(receiver->actualFlowModel() == &project.subs[i]->flow);
        CHECK(receiver->nativeReceiverAlive() && !receiver->backendWitness().has_value());
    }
    CHECK(project.activeSub == 1 && !prefix.completeNativePopulation());
}

TEST_CASE(flow_receiver_population_prefix_keeps_death_copy_source_and_reused_identity_generations)
{
    Rig rig;
    rig.initialize();
    const auto registry = rig.session.flowReceiverWitnessRegistryOwner();
    const auto before = projectFlowReceiverPopulationPrefix(rig.session);
    rig.session.project().subs.clear();
    const auto dead = before.entries()[0].registration.receiver.lock();
    REQUIRE(dead);
    CHECK(!dead->nativeReceiverAlive());
    // Explicit source-event fixture: not a native copy constructor binding.
    int copyIdentity = 0;
    const auto copy = registry->recordCopyConstruction(&copyIdentity);
    registry->recordReceiverDeath(copy);
    const auto replacement = registry->recordNormalSubConstruction(&copyIdentity);
    const auto after = projectFlowReceiverPopulationPrefix(rig.session);
    REQUIRE(after.entries().size() == 3);
    CHECK(before.entries().size() == 1); // Snapshot is not a live mutable list.
    CHECK(after.entries()[0].registration.receiver.lock() == dead);
    CHECK(after.entries()[1].source == FlowReceiverConstructionSource::Copy);
    CHECK(after.entries()[1].registrationSite == 0x140071a7fULL);
    CHECK(after.entries()[2].source == FlowReceiverConstructionSource::NormalSub);
    CHECK(after.entries()[2].registrationSite == 0x140071166ULL);
    CHECK(after.entries()[1].identityToken == after.entries()[2].identityToken);
    CHECK(after.entries()[1].registration.receiver.lock() != after.entries()[2].registration.receiver.lock());
    CHECK(!copy->nativeReceiverAlive() && replacement->nativeReceiverAlive());
}

TEST_CASE(flow_receiver_population_prefix_rejects_foreign_session_unbound_io_and_wrong_notifier)
{
    Rig first, second;
    first.initialize();
    const auto prefix = projectFlowReceiverPopulationPrefix(first.session);
    CHECK(unsupported([&] { prefix.validateAgainst(second.session); }));
    auto& project = first.session.project();
    project.doeIo = makeRecordingStudyIo();
    CHECK(unsupported([&] { (void)projectFlowReceiverPopulationPrefix(first.session); }));
    CHECK(!project.doeIo->observedSessionNotificationState()); // No lazy owner creation.
    project.doeIo->bindSessionNotificationState(second.session.sessionNotificationStateOwner());
    CHECK(unsupported([&] { prefix.validateAgainst(first.session); }));
    project.doeIo.reset();
    prefix.validateAgainst(first.session);
    first.session.releaseProjectAfterCloseEffects();
    CHECK(unsupported([&] { prefix.validateAgainst(first.session); }));
    CHECK(unsupported([&] { (void)projectFlowReceiverPopulationPrefix(first.session); }));
}

TEST_CASE(flow_receiver_population_prefix_holds_no_session_registry_or_receiver_lifetime)
{
    std::optional<FlowReceiverPopulationPrefix> escaped;
    std::weak_ptr<SessionNotificationState> owner;
    std::weak_ptr<FlowReceiverWitnessRegistry> registry;
    std::weak_ptr<SetOprHasDoeReceiver> receiver;
    {
        Rig rig;
        rig.initialize();
        escaped = projectFlowReceiverPopulationPrefix(rig.session);
        owner = escaped->owner();
        registry = rig.session.flowReceiverWitnessRegistryOwner();
        receiver = escaped->entries()[0].registration.receiver;
        CHECK(!owner.expired() && !registry.expired() && !receiver.expired());
    }
    CHECK(owner.expired() && registry.expired() && receiver.expired());
    CHECK(!escaped->owner() && !escaped->completeNativePopulation());
    Rig another;
    CHECK(unsupported([&] { escaped->validateAgainst(another.session); }));
}

TEST_CASE(flow_receiver_population_prefix_exposes_only_observable_released_registry_history)
{
    Rig rig;
    rig.initialize();
    const auto registry = rig.session.flowReceiverWitnessRegistryOwner();
    registry->releaseAdapterScope();
    const auto prefix = projectFlowReceiverPopulationPrefix(rig.session);
    REQUIRE(prefix.entries().size() == 1);
    const auto receiver = prefix.entries()[0].registration.receiver.lock();
    REQUIRE(receiver);
    CHECK(!receiver->nativeReceiverAlive() && !receiver->actualProject());
    CHECK(!prefix.completeNativePopulation());
    // Current registry API cannot distinguish scope release from all-dead
    // history. This diagnostic view makes no unreleased-scope assertion.
    prefix.validateAgainst(rig.session);
}

TEST_CASE(flow_receiver_population_prefix_does_not_install_population_or_bypass_guard)
{
    Rig rig;
    rig.initialize();
    rig.bindIo();
    const auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    const auto owner = rig.session.sessionNotificationStateOwner();
    REQUIRE(owner->createTaskBaseRegistrations);
    const auto registrationCount = owner->createTaskBaseRegistrations->entries.size();
    owner->flag90 = true;
    const auto prefix = projectFlowReceiverPopulationPrefix(rig.session);
    CHECK(!prefix.entries().empty() && !prefix.completeNativePopulation());
    CHECK(!binding->notifier().registrations && owner->flag90);
    CHECK(owner->emptyKeys.empty() && owner->attempts.empty());
    CHECK(unsupported([&] { binding->dispatch(rig.session, 7, true); }));
    CHECK(!owner->flag90 && !binding->notifier().registrations);
    REQUIRE(binding->notifier().attempts.size() == 1);
    CHECK(binding->notifier().attempts[0].stop == SetOprHasDoeStop::Population);
    CHECK(!binding->notifier().attempts[0].snapshotTaken);
    CHECK(owner->emptyKeys.empty() && owner->createTaskBaseRegistrations &&
          owner->createTaskBaseRegistrations->entries.size() == registrationCount);
}

int main() { return testkit::run_all(); }
