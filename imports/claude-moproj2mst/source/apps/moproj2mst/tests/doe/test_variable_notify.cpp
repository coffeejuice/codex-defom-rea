// ctest-labels: m2m-doe
// Isolated source-qualified projection from resume51-doe-variable-notify.md.
// Explicit fixture witnesses do not establish original startup or live delivery.
#include <testkit/check.hpp>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/doe/variable_notify.hpp"

using namespace m2m;
using namespace m2m::doe;
namespace {
class Receiver final : public SetOprHasDoeReceiver {
public:
    Receiver(Project& project, SubProject& sub) : model_(&sub.flow), project_(&project), sub_(&sub) {}
    bool alive = true; // explicit diagnostic QObject witness, not a default production profile
    std::optional<const void*> backend; // unknown until fixture qualifies it
    const FlowChart* model_;
    bool nativeReceiverAlive() const noexcept override { return alive; }
    const Project* actualProject() const noexcept override { return project_; }
    const SubProject* actualSubProject() const noexcept override { return sub_; }
    const FlowChart* actualFlowModel() const noexcept override { return model_; }
    std::optional<const void*> backendWitness() const noexcept override { return backend; }
private:
    Project* project_;
    SubProject* sub_;
};

struct Rig {
    Project project;
    std::shared_ptr<StudyIo> io = makeRecordingStudyIo();
    std::shared_ptr<SessionNotificationState> state = std::make_shared<SessionNotificationState>();
    SetOprHasDoeBindings bindings{state, "explicit shared diagnostic notifier owner"};
    int backendToken = 0;
    Rig()
    {
        io->bindSessionNotificationState(state);
        project.doeIo = io;
        project.activeSub = 0;
        for (int i = 0; i < 3; ++i) {
            auto sub = std::make_unique<SubProject>();
            sub->project = &project;
            sub->id = i;
            project.subs.push_back(std::move(sub));
        }
        bindings.registrations.emplace();
        bindings.registrations->qualification = "complete ordered explicit fixture population";
        bindings.registrations->mapKeyPresent = true;
    }
    std::shared_ptr<Receiver> receiver(int subIndex)
    {
        return std::make_shared<Receiver>(project, *project.subs[static_cast<std::size_t>(subIndex)]);
    }
    FlowItem* item(int subIndex, int operationId)
    {
        auto item = std::make_unique<FlowItem>();
        item->opId = operationId;
        auto* result = item.get();
        project.subs[static_cast<std::size_t>(subIndex)]->flow.items.push_back(std::move(item));
        return result;
    }
};

SetOprHasDoeStop refused(StudyIo& io, SetOprHasDoeBindings& bindings, int operationId = 1, bool value = true)
{
    try { dispatchSetOprHasDoePrefix(io, bindings, operationId, value); }
    catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        REQUIRE(bindings.attempts.back().stop.has_value());
        return *bindings.attempts.back().stop;
    }
    REQUIRE(false);
    return SetOprHasDoeStop::Owner;
}
}

TEST_CASE(set_opr_has_doe_requires_the_existing_shared_owner)
{
    auto io = makeRecordingStudyIo();
    auto state = std::make_shared<SessionNotificationState>();
    state->flag90 = true;
    SetOprHasDoeBindings binding(state, "explicit owner");
    CHECK(refused(*io, binding) == SetOprHasDoeStop::Owner);
    CHECK(io->observedSessionNotificationState() == nullptr);
    CHECK(state->flag90);
    io->bindSessionNotificationState(std::make_shared<SessionNotificationState>());
    CHECK(refused(*io, binding) == SetOprHasDoeStop::Owner);
    CHECK(state->flag90);
    CHECK(state->emptyKeys.empty());
}

TEST_CASE(set_opr_has_doe_suppression_precedes_population_and_reset)
{
    Rig rig;
    rig.state->suppressed = true;
    rig.state->flag90 = true;
    rig.bindings.registrations.reset();
    dispatchSetOprHasDoePrefix(*rig.io, rig.bindings, 1, true);
    const auto& attempt = rig.bindings.attempts.back();
    CHECK(attempt.completion == SetOprHasDoeCompletion::SuppressedReturn);
    CHECK(!attempt.flagReset && !attempt.keyInserted && !attempt.snapshotTaken);
    CHECK(attempt.visits.empty());
    CHECK(rig.state->flag90 && rig.state->emptyKeys.empty());
}

TEST_CASE(set_opr_has_doe_population_guards_retain_flag_reset)
{
    Rig rig;
    rig.state->flag90 = true;
    rig.bindings.registrations.reset();
    CHECK(refused(*rig.io, rig.bindings) == SetOprHasDoeStop::Population);
    CHECK(!rig.state->flag90);
    CHECK(rig.bindings.attempts.back().flagReset);
    rig.bindings.registrations.emplace();
    CHECK(refused(*rig.io, rig.bindings) == SetOprHasDoeStop::Population); // unqualified empty is unavailable
    auto receiver = rig.receiver(0);
    rig.bindings.registrations->qualification = "fixture population";
    rig.bindings.registrations->entries.push_back({receiver});
    CHECK(refused(*rig.io, rig.bindings) == SetOprHasDoeStop::Population); // absent/nonempty contradiction
    CHECK(!rig.bindings.registrations->mapKeyPresent);
    CHECK(rig.state->emptyKeys.empty());
}

TEST_CASE(set_opr_has_doe_known_empty_lookup_inserts_only_the_missing_key)
{
    Rig rig;
    rig.bindings.registrations->mapKeyPresent = false;
    dispatchSetOprHasDoePrefix(*rig.io, rig.bindings, 1, true);
    CHECK(rig.bindings.registrations->mapKeyPresent);
    CHECK(rig.state->emptyKeys.contains("2signalSetOprHasDoe()"));
    CHECK(rig.bindings.attempts.back().keyInserted);
    dispatchSetOprHasDoePrefix(*rig.io, rig.bindings, 1, true);
    CHECK(!rig.bindings.attempts.back().keyInserted);
    CHECK(rig.bindings.attempts.back().completion == SetOprHasDoeCompletion::ReceiverListReturned);
    CHECK_EQ(rig.bindings.attempts.size(), 2U);
}

TEST_CASE(set_opr_has_doe_uses_all_actual_flow_models_in_registration_order)
{
    Rig rig;
    auto inactive = rig.receiver(2);
    auto active = rig.receiver(0);
    inactive->backend = nullptr;
    active->backend = nullptr;
    rig.project.subs[2]->flow.items.push_back(nullptr); // null-backend branch must not inspect items
    rig.bindings.registrations->entries = {{inactive}, {active}};
    dispatchSetOprHasDoePrefix(*rig.io, rig.bindings, 1, true);
    const auto& attempt = rig.bindings.attempts.back();
    REQUIRE(attempt.visits.size() == 2);
    CHECK(attempt.visits[0].model == &rig.project.subs[2]->flow);
    CHECK(attempt.visits[1].model == &rig.project.subs[0]->flow);
    for (const auto& visit : attempt.visits) CHECK(visit.outcome == SetOprHasDoeVisitOutcome::NullBackend);
    CHECK(attempt.completion == SetOprHasDoeCompletion::ReceiverListReturned);
    CHECK(rig.io->textWrites().empty() && rig.io->reads().empty());
}

TEST_CASE(set_opr_has_doe_expired_guards_do_not_prune_shared_registrations)
{
    Rig rig;
    auto dead = rig.receiver(0);
    dead->alive = false;
    auto live = rig.receiver(1);
    live->backend = nullptr;
    rig.bindings.registrations->entries = {SetOprHasDoeRegistration{}, {dead}, {live}};
    dispatchSetOprHasDoePrefix(*rig.io, rig.bindings, 1, true);
    const auto& visits = rig.bindings.attempts.back().visits;
    REQUIRE(visits.size() == 3);
    CHECK(visits[0].outcome == SetOprHasDoeVisitOutcome::ExpiredGuard);
    CHECK(visits[1].outcome == SetOprHasDoeVisitOutcome::NativeDead);
    CHECK(visits[2].outcome == SetOprHasDoeVisitOutcome::NullBackend);
    CHECK_EQ(rig.bindings.registrations->entries.size(), 3U);
}

TEST_CASE(set_opr_has_doe_preserves_signed_id_and_missing_item_return)
{
    Rig rig;
    auto receiver = rig.receiver(0);
    receiver->backend = &rig.backendToken;
    rig.item(0, 1);
    rig.bindings.registrations->entries = {{receiver}};
    dispatchSetOprHasDoePrefix(*rig.io, rig.bindings, -7, false);
    const auto& attempt = rig.bindings.attempts.back();
    CHECK_EQ(attempt.operationId, -7);
    CHECK(!attempt.hasDoe);
    CHECK(attempt.visits[0].outcome == SetOprHasDoeVisitOutcome::MissingItem);
    CHECK(attempt.completion == SetOprHasDoeCompletion::ReceiverListReturned);
}

TEST_CASE(set_opr_has_doe_refuses_icon_branch_after_earlier_receivers)
{
    Rig rig;
    auto nullBackend = rig.receiver(1);
    nullBackend->backend = nullptr;
    auto matched = rig.receiver(0);
    matched->backend = &rig.backendToken;
    auto later = rig.receiver(2);
    later->backend = nullptr;
    const auto* first = rig.item(0, 1);
    rig.item(0, 1); // native first-match lookup, not last match
    rig.bindings.registrations->entries = {{nullBackend}, {matched}, {later}};
    rig.state->flag90 = true;
    CHECK(refused(*rig.io, rig.bindings) == SetOprHasDoeStop::IconAllocation);
    const auto& attempt = rig.bindings.attempts.back();
    REQUIRE(attempt.visits.size() == 2);
    CHECK(attempt.visits[0].outcome == SetOprHasDoeVisitOutcome::NullBackend);
    CHECK(attempt.visits[1].outcome == SetOprHasDoeVisitOutcome::Stopped);
    CHECK(attempt.visits[1].item == first);
    CHECK(attempt.visits[1].backend == &rig.backendToken);
    CHECK(!rig.state->flag90 && attempt.flagReset);
    CHECK(!attempt.completion.has_value());
    CHECK(rig.io->textWrites().empty() && rig.io->reads().empty());
}

TEST_CASE(set_opr_has_doe_guards_unknown_backend_method_and_model_identity)
{
    Rig rig;
    auto receiver = rig.receiver(0);
    rig.bindings.registrations->entries = {{receiver}};
    CHECK(refused(*rig.io, rig.bindings) == SetOprHasDoeStop::BackendWitness);
    rig.bindings.registrations->entries[0].bareMethod = "unrecoveredMethod";
    CHECK(refused(*rig.io, rig.bindings) == SetOprHasDoeStop::Method);
    rig.bindings.registrations->entries[0].bareMethod = "slotSetOprHasDoe";
    FlowChart copiedModel;
    receiver->model_ = &copiedModel;
    CHECK(refused(*rig.io, rig.bindings) == SetOprHasDoeStop::ModelIdentity);
}

TEST_CASE(set_opr_has_doe_false_branch_stops_before_backend_clear)
{
    Rig rig;
    auto receiver = rig.receiver(0);
    receiver->backend = &rig.backendToken;
    rig.item(0, 1);
    rig.bindings.registrations->entries = {{receiver}};
    CHECK(refused(*rig.io, rig.bindings, 1, false) == SetOprHasDoeStop::IconBackend);
    CHECK(!rig.bindings.attempts.back().hasDoe);
    CHECK(!rig.bindings.attempts.back().completion.has_value());
}

TEST_CASE(set_opr_has_doe_repeated_calls_use_the_current_shared_order)
{
    Rig rig;
    auto first = rig.receiver(0);
    auto second = rig.receiver(1);
    first->backend = nullptr;
    second->backend = nullptr;
    rig.bindings.registrations->entries = {{first}, {second}};
    dispatchSetOprHasDoePrefix(*rig.io, rig.bindings, 1, true);
    rig.bindings.registrations->entries = {{second}, {first}};
    dispatchSetOprHasDoePrefix(*rig.io, rig.bindings, 1, true);
    REQUIRE(rig.bindings.attempts.size() == 2);
    CHECK(rig.bindings.attempts[0].visits[0].model == &rig.project.subs[0]->flow);
    CHECK(rig.bindings.attempts[1].visits[0].model == &rig.project.subs[1]->flow);
}

int main() { return testkit::run_all(); }
