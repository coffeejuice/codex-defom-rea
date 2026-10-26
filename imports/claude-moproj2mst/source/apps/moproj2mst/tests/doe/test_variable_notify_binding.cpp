// ctest-labels: m2m-doe
// Actual ConversionSession owner integration; explicit receiver fixtures do not
// establish native registration/startup or public B121 completion.
#include <testkit/check.hpp>
#include <memory>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/doe/variable_notify.hpp"

using namespace m2m;
using namespace m2m::doe;
namespace {
struct SessionRig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    std::shared_ptr<StudyIo> io;
    explicit SessionRig(bool bindIo = true)
    {
        if (bindIo) {
            io = makeRecordingStudyIo();
            session.project().doeIo = io;
            io->bindSessionNotificationState(session.sessionNotificationStateOwner());
        }
    }
};

template<class Work> bool unsupported(Work work)
{
    try { work(); }
    catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}

class ActualFlowReceiver final : public SetOprHasDoeReceiver {
public:
    ActualFlowReceiver(Project& project, SubProject& sub) : project_(project), sub_(sub) {}
    bool nativeReceiverAlive() const noexcept override { return true; } // explicit diagnostic lifecycle witness
    const Project* actualProject() const noexcept override { return &project_; }
    const SubProject* actualSubProject() const noexcept override { return &sub_; }
    const FlowChart* actualFlowModel() const noexcept override { return &sub_.flow; }
    std::optional<const void*> backendWitness() const noexcept override { return nullptr; } // explicit fixture null
private:
    Project& project_;
    SubProject& sub_;
};
}

TEST_CASE(set_opr_has_doe_session_hook_requires_existing_io_owner)
{
    SessionRig rig(false);
    const auto owner = rig.session.sessionNotificationStateOwner();
    owner->flag90 = true;
    CHECK(unsupported([&] { (void)bindSetOprHasDoeSessionPrefix(rig.session); }));
    CHECK(!rig.session.project().doeIo);
    CHECK(owner->flag90);
    rig.session.project().doeIo = makeRecordingStudyIo();
    CHECK(unsupported([&] { (void)bindSetOprHasDoeSessionPrefix(rig.session); }));
    CHECK(rig.session.project().doeIo->observedSessionNotificationState() == nullptr);
    CHECK(owner->flag90);
    rig.session.project().doeIo->bindSessionNotificationState(std::make_shared<SessionNotificationState>());
    CHECK(unsupported([&] { (void)bindSetOprHasDoeSessionPrefix(rig.session); }));
    CHECK(owner->flag90);
}

TEST_CASE(set_opr_has_doe_session_hook_retains_unavailable_population_guard)
{
    SessionRig rig;
    const auto owner = rig.session.sessionNotificationStateOwner();
    auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    CHECK(binding->notifier().owner.lock() == owner);
    CHECK(!binding->notifier().registrations.has_value());
    owner->flag90 = true;
    CHECK(unsupported([&] { binding->dispatch(rig.session, 1, true); }));
    CHECK(!owner->flag90); // original reset precedes the unavailable population boundary
    REQUIRE(binding->notifier().attempts.size() == 1);
    CHECK(binding->notifier().attempts[0].stop == SetOprHasDoeStop::Population);
    CHECK(binding->notifier().attempts[0].flagReset);
}

TEST_CASE(set_opr_has_doe_session_hook_preserves_shared_suppression)
{
    SessionRig rig;
    auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    const auto owner = rig.session.sessionNotificationStateOwner();
    owner->suppressed = true;
    owner->flag90 = true;
    binding->dispatch(rig.session, 1, true);
    CHECK(owner->flag90);
    REQUIRE(binding->notifier().attempts.size() == 1);
    CHECK(binding->notifier().attempts[0].completion == SetOprHasDoeCompletion::SuppressedReturn);
    CHECK(!binding->notifier().registrations.has_value());
}

TEST_CASE(set_opr_has_doe_session_hook_uses_explicit_actual_receiver_order)
{
    SessionRig rig;
    auto& project = rig.session.project();
    for (int i = 0; i < 2; ++i) {
        auto sub = std::make_unique<SubProject>();
        sub->project = &project;
        project.subs.push_back(std::move(sub));
    }
    project.activeSub = 0;
    auto first = std::make_shared<ActualFlowReceiver>(project, *project.subs[1]);
    auto second = std::make_shared<ActualFlowReceiver>(project, *project.subs[0]);
    auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    binding->notifier().registrations = SetOprHasDoeRegistrationList{
        {{first}, {second}}, "Explicit actual-model diagnostic order; not native constructor completion", true};
    binding->dispatch(rig.session, 1, true);
    const auto& visits = binding->notifier().attempts.back().visits;
    REQUIRE(visits.size() == 2);
    CHECK(visits[0].model == &project.subs[1]->flow);
    CHECK(visits[1].model == &project.subs[0]->flow);
    CHECK(visits[0].outcome == SetOprHasDoeVisitOutcome::NullBackend);
    CHECK(visits[1].outcome == SetOprHasDoeVisitOutcome::NullBackend);
    CHECK(rig.io->reads().empty() && rig.io->textWrites().empty());
}

TEST_CASE(set_opr_has_doe_session_hook_rejects_replaced_io_before_native_effects)
{
    SessionRig rig;
    auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    const auto owner = rig.session.sessionNotificationStateOwner();
    owner->flag90 = true;
    rig.session.project().doeIo = makeRecordingStudyIo();
    rig.session.project().doeIo->bindSessionNotificationState(owner);
    CHECK(unsupported([&] { binding->dispatch(rig.session, 1, true); }));
    CHECK(owner->flag90);
    CHECK(binding->notifier().attempts.empty());
}

TEST_CASE(set_opr_has_doe_session_hook_rejects_a_foreign_project_receiver_in_order)
{
    SessionRig rig;
    SessionRig other;
    auto sub = std::make_unique<SubProject>();
    sub->project = &other.session.project();
    other.session.project().subs.push_back(std::move(sub));
    auto receiver = std::make_shared<ActualFlowReceiver>(other.session.project(),
                                                       *other.session.project().subs.back());
    auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    binding->notifier().registrations = SetOprHasDoeRegistrationList{
        {{receiver}}, "Explicit foreign-project receiver fixture", true};
    rig.session.sessionNotificationStateOwner()->flag90 = true;
    CHECK(unsupported([&] { binding->dispatch(rig.session, 1, true); }));
    CHECK(!rig.session.sessionNotificationStateOwner()->flag90);
    REQUIRE(binding->notifier().attempts.size() == 1);
    CHECK(binding->notifier().attempts[0].stop == SetOprHasDoeStop::ModelIdentity);
    CHECK(binding->notifier().attempts[0].snapshotTaken);
    REQUIRE(binding->notifier().attempts[0].visits.size() == 1);
    CHECK(binding->notifier().attempts[0].visits[0].outcome == SetOprHasDoeVisitOutcome::Stopped);
}

TEST_CASE(set_opr_has_doe_session_hook_rejects_another_actual_session)
{
    SessionRig rig;
    auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    SessionRig other;
    rig.session.sessionNotificationStateOwner()->flag90 = true;
    other.session.sessionNotificationStateOwner()->flag90 = true;
    CHECK(unsupported([&] { binding->dispatch(other.session, 1, true); }));
    CHECK(rig.session.sessionNotificationStateOwner()->flag90);
    CHECK(other.session.sessionNotificationStateOwner()->flag90);
    CHECK(binding->notifier().attempts.empty());
}

TEST_CASE(set_opr_has_doe_session_hook_does_not_revive_released_project)
{
    SessionRig rig;
    auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    const auto owner = rig.session.sessionNotificationStateOwner();
    owner->flag90 = true;
    rig.session.releaseProjectAfterCloseEffects(); // null-bound storage release only, not a normal Close replay
    CHECK(rig.session.liveProject() == nullptr);
    CHECK(unsupported([&] { binding->dispatch(rig.session, 1, true); }));
    CHECK(unsupported([&] { (void)bindSetOprHasDoeSessionPrefix(rig.session); }));
    CHECK(owner->flag90);
    CHECK(binding->notifier().attempts.empty());
}

TEST_CASE(set_opr_has_doe_session_hook_is_not_called_after_keyword_entry)
{
    SessionRig rig;
    auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    auto sub = makeSubProject(rig.session.project(), 2, 1);
    REQUIRE(sub->ownedDoeStudy != nullptr);
    CHECK(unsupported([&] {
        // The reader reaches the owner/header and earlier Description, queues
        // LinkTo, then stops at Keyword before the notifier/keyword path.
        (void)readInputTable("<sfDoeInputTable><DoeVariables><DoeVariable name=\"Transfer Time\" "
                            "numSamples=\"3\" varId=\"1\" dimension=\"3\" LinkTo=\"Missing\">"
                            "<Description>retained</Description>"
                            "<Keyword name=\"TLOC\" unitType=\"0\" parmType=\"double\" parameter=\"-1\" nominalValue=\"0.1\">"
                            "<KwOperations><KwOperation id=\"1\"/></KwOperations></Keyword>"
                            "</DoeVariable></DoeVariables></sfDoeInputTable>",
                            *sub->ownedDoeStudy);
    }));
    REQUIRE(sub->ownedDoeStudy->vars.size() == 1);
    REQUIRE(sub->ownedDoeStudy->headerVariableOwners.size() == 1);
    CHECK(sub->ownedDoeStudy->vars[0] == sub->ownedDoeStudy->headerVariableOwners[0].get());
    CHECK(sub->ownedDoeStudy->headerVariableOwners[0]->name == qt::L("Transfer Time"));
    CHECK(sub->ownedDoeStudy->headerVariableOwners[0]->description == qt::L("retained"));
    CHECK(sub->ownedDoeStudy->vars[0]->link == nullptr); // LinkTo was queued but the Keyword stop skips finalization.
    CHECK(binding->notifier().attempts.empty());
}

int main() { return testkit::run_all(); }
