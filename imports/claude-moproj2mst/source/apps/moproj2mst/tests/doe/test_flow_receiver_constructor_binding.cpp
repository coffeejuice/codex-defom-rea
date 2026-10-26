// ctest-labels: m2m-doe
#include <testkit/check.hpp>
#include <functional>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/doe/flow_receiver_witness.hpp"

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
    void initialize() { initProject(session.project(), qt::L("C:/PROBLEM"), qt::L("flow_witness")); }
};
class ConstructorProbeIo final : public StudyIo {
public:
    explicit ConstructorProbeIo(std::function<void()> probe) : StudyIo(IoMode::Recording), probe_(std::move(probe)) {}
    bool ensureDirectory(const qt::String&) override { probe_(); return true; }
private:
    std::function<void()> probe_;
};
}

TEST_CASE(flow_receiver_constructor_session_binds_before_sub0_and_io)
{
    Rig rig;
    auto& project = rig.session.project();
    const auto registry = rig.session.flowReceiverWitnessRegistryOwner();
    REQUIRE(registry && registry->owner() == rig.session.sessionNotificationStateOwner());
    CHECK(project.flowReceiverWitnessRegistry() == registry && !project.doeIo);
    CHECK(registry->orderedWitnesses().empty());
    rig.initialize();
    REQUIRE(project.subs.size() == 1 && registry->orderedWitnesses().size() == 1);
    const auto witness = registry->orderedWitnesses()[0];
    CHECK(witness->identityToken() == &project.subs[0]->flow && witness->actualProject() == &project);
    CHECK(witness->actualSubProject() == project.subs[0].get());
    CHECK(witness->source() == FlowReceiverConstructionSource::NormalSub);
    CHECK(witness->nativeReceiverAlive() && !witness->backendWitness());
    CHECK(!project.doeIo);
}

TEST_CASE(flow_receiver_constructor_context_precedes_type2_study_and_vector_growth_preserves_identity)
{
    Rig rig;
    rig.initialize();
    auto& project = rig.session.project();
    const auto registry = rig.session.flowReceiverWitnessRegistryOwner();
    const auto first = registry->orderedWitnesses()[0];
    const auto* firstModel = first->actualFlowModel();
    bool observedBeforeStudyCommit = false;
    auto io = std::make_shared<ConstructorProbeIo>([&] {
        const auto witness = registry->orderedWitnesses().back();
        const auto* sub = witness->actualSubProject();
        observedBeforeStudyCommit = witness->nativeReceiverAlive() && sub && sub->type == 2 &&
            witness->actualProject() == &project && witness->actualFlowModel() == &sub->flow && !sub->ownedDoeStudy;
    });
    project.doeIo = io;
    io->bindSessionNotificationState(rig.session.sessionNotificationStateOwner());
    auto studySub = makeSubProject(project, 2, 7);
    CHECK(observedBeforeStudyCommit && studySub->ownedDoeStudy);
    project.subs.push_back(std::move(studySub));
    for (int id = 2; id < 20; ++id) project.subs.push_back(makeSubProject(project, 0, id));
    CHECK(first->actualFlowModel() == firstModel && firstModel == &project.subs[0]->flow);
    REQUIRE(registry->orderedWitnesses().size() == project.subs.size());
    for (std::size_t i = 0; i < project.subs.size(); ++i) {
        CHECK(registry->orderedWitnesses()[i]->ordinal() == i);
        CHECK(registry->orderedWitnesses()[i]->actualFlowModel() == &project.subs[i]->flow);
    }
}

TEST_CASE(flow_receiver_constructor_sub_and_project_death_invalidate_escaped_witnesses)
{
    Rig rig;
    rig.initialize();
    auto& project = rig.session.project();
    const auto registry = rig.session.flowReceiverWitnessRegistryOwner();
    const auto old = registry->orderedWitnesses()[0];
    project.subs.clear();
    CHECK(!old->nativeReceiverAlive() && !old->actualSubProject());
    project.subs.push_back(makeSubProject(project, 0, 0));
    const auto replacement = registry->orderedWitnesses().back();
    CHECK(replacement != old && replacement->nativeReceiverAlive() && replacement->ordinal() == 1);
    rig.session.releaseProjectAfterCloseEffects();
    CHECK(!rig.session.liveProject() && !replacement->nativeReceiverAlive());
    CHECK(!replacement->actualProject() && registry->orderedWitnesses().size() == 2);
}

TEST_CASE(flow_receiver_constructor_binding_keeps_population_unavailable_and_rejects_replacement)
{
    Rig rig;
    rig.initialize();
    auto& project = rig.session.project();
    const auto registry = rig.session.flowReceiverWitnessRegistryOwner();
    project.bindFlowReceiverWitnessRegistry(registry);
    auto foreign = std::make_shared<FlowReceiverWitnessRegistry>(registry->owner(), "foreign diagnostic registry");
    CHECK(unsupported([&] { project.bindFlowReceiverWitnessRegistry(foreign); }));
    project.doeIo = makeRecordingStudyIo();
    project.doeIo->bindSessionNotificationState(rig.session.sessionNotificationStateOwner());
    const auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    CHECK(!binding->notifier().registrations);
    registry->owner()->flag90 = true;
    CHECK(unsupported([&] { binding->dispatch(rig.session, 1, true); }));
    CHECK(!registry->owner()->flag90);
    REQUIRE(binding->notifier().attempts.size() == 1);
    CHECK(binding->notifier().attempts[0].stop == SetOprHasDoeStop::Population);
    CHECK(!registry->orderedWitnesses()[0]->backendWitness());
}

TEST_CASE(flow_receiver_constructor_unbound_project_stays_explicit_and_expired_owner_refuses)
{
    Project project;
    CHECK(!project.flowReceiverWitnessRegistry());
    auto unbound = makeSubProject(project, 0, 0);
    CHECK(unbound->project == &project);
    auto owner = std::make_shared<SessionNotificationState>();
    auto registry = std::make_shared<FlowReceiverWitnessRegistry>(owner, "isolated expiring registry");
    CHECK(unsupported([&] { project.bindFlowReceiverWitnessRegistry(registry); }));
    Project expiringProject;
    expiringProject.bindFlowReceiverWitnessRegistry(registry);
    registry.reset();
    CHECK(unsupported([&] { (void)makeSubProject(expiringProject, 0, 1); }));
}

int main() { return testkit::run_all(); }
