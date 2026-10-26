// ctest-labels: m2m-doe
// Declared prerequisite fixtures test this isolated prefix only. They do not
// implement original notifier registration/removal or close B121 population.
#include <testkit/check.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe/backend_commit_prefix.hpp"
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
    Rig() { initProject(session.project(), qt::L("C:/PROBLEM"), qt::L("backend_prefix")); }
    std::shared_ptr<FlowReceiverWitness> receiver() const
    { return session.flowReceiverWitnessRegistryOwner()->orderedWitnesses().front(); }
};

template<class Work> bool unsupported(const Work& work)
{
    try { work(); }
    catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}

struct DiagnosticPrerequisites : BackendSetterPrerequisiteDriver {
    std::vector<std::string> calls;
    std::function<void(const BackendSetterPrerequisiteContext&)> during;
    bool fail = false;
    std::string qualification() const override { return "declared isolated adapter fixture; not native registration proof"; }
    void run(const BackendSetterPrerequisiteContext& context, const char* call)
    {
        calls.emplace_back(call);
        if (during) during(context);
        if (fail) throw Error(M2M_E_UNSUPPORTED, "declared prerequisite refusal after partial effects");
    }
    void registerCanAddItem(const BackendSetterPrerequisiteContext& context) override { run(context, "register"); }
    void removeCanAddItem(const BackendSetterPrerequisiteContext& context) override { run(context, "remove"); }
};
}

TEST_CASE(backend_commit_defaults_guard_registration_and_removal)
{
    Rig rig;
    BackendCommitPrefix prefix(rig.session, rig.receiver());
    BackendSetterPrerequisiteDriver unavailable;
    int backend;
    CHECK(!prefix.current(rig.session));
    CHECK(unsupported([&] { prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, unavailable); }));
    CHECK(unsupported([&] { prefix.apply(rig.session, {2, BackendSetterSite::OldModelDetach, nullptr}, unavailable); }));
    REQUIRE(prefix.attempts().size() == 2);
    CHECK(prefix.attempts()[0].stop == BackendCommitStop::Prerequisite);
    CHECK(prefix.attempts()[1].stop == BackendCommitStop::Prerequisite);
    CHECK(prefix.attempts()[0].error.find("registration") != std::string::npos);
    CHECK(prefix.attempts()[1].error.find("removal") != std::string::npos);
    CHECK(!prefix.attempts()[0].committed && !prefix.attempts()[1].committed);
    CHECK(!prefix.current(rig.session));
    CHECK(!rig.receiver()->backendWitness());
}

TEST_CASE(backend_commit_follows_prerequisite_with_actual_context_and_distinct_occurrences)
{
    Rig rig;
    const auto receiver = rig.receiver();
    BackendCommitPrefix prefix(rig.session, receiver);
    DiagnosticPrerequisites driver;
    int backend;
    unsigned calls = 0;
    driver.during = [&](const BackendSetterPrerequisiteContext& context) {
        CHECK(&context.notifier == rig.session.sessionNotificationStateOwner().get());
        CHECK(&context.receiver == receiver.get());
        CHECK(&context.project == &rig.session.project());
        CHECK(&context.sub == rig.session.project().subs[0].get());
        CHECK(std::string(context.signal) == "2signalDiagModelCanAddItem()");
        CHECK(std::string(context.slot) == "1slotDiagModelCanAddItem()");
        CHECK(!context.prepend && context.occurrence.backend == &backend);
        if (++calls == 1) CHECK(!prefix.current(rig.session));
        else CHECK(prefix.current(rig.session).value() == &backend);
    };
    prefix.apply(rig.session, {7, BackendSetterSite::FirstNewModelAttach, &backend}, driver);
    prefix.apply(rig.session, {7, BackendSetterSite::SecondNewModelAttach, &backend}, driver);
    prefix.apply(rig.session, {8, BackendSetterSite::FirstNewModelAttach, &backend}, driver);
    REQUIRE(prefix.attempts().size() == 3);
    CHECK(prefix.attempts()[0].callerSite == 0x140170581ULL);
    CHECK(prefix.attempts()[1].callerSite == 0x1401705f4ULL);
    CHECK(prefix.attempts()[2].callerSite == 0x140170581ULL);
    for (const auto& attempt : prefix.attempts())
        CHECK(attempt.prerequisiteStarted && attempt.prerequisiteReturned && attempt.committed && !attempt.stop);
    CHECK(calls == 3);
    CHECK(!receiver->backendWitness()); // isolated projection never installs a production backend
}

TEST_CASE(backend_commit_null_detach_runs_removal_before_storing_null)
{
    Rig rig;
    BackendCommitPrefix prefix(rig.session, rig.receiver());
    DiagnosticPrerequisites driver;
    int backend;
    prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, driver);
    driver.during = [&](const BackendSetterPrerequisiteContext& context) {
        CHECK(prefix.current(rig.session).value() == &backend);
        CHECK(context.occurrence.backend == nullptr);
    };
    prefix.apply(rig.session, {2, BackendSetterSite::OldModelDetach, nullptr}, driver);
    REQUIRE(prefix.current(rig.session).has_value());
    CHECK(prefix.current(rig.session).value() == nullptr);
    CHECK(driver.calls == std::vector<std::string>({"register", "remove"}));
    CHECK(prefix.attempts().back().callerSite == 0x140170540ULL);
}

TEST_CASE(backend_commit_prerequisite_refusal_retains_prior_commit_and_partial_effects)
{
    Rig rig;
    BackendCommitPrefix prefix(rig.session, rig.receiver());
    DiagnosticPrerequisites driver;
    int backend;
    prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, driver);
    driver.fail = true;
    driver.during = [](const BackendSetterPrerequisiteContext& context) { context.notifier.flag90 = true; };
    CHECK(unsupported([&] { prefix.apply(rig.session, {2, BackendSetterSite::OldModelDetach, nullptr}, driver); }));
    CHECK(prefix.current(rig.session).value() == &backend);
    CHECK(rig.session.sessionNotificationStateOwner()->flag90);
    CHECK(prefix.attempts().back().prerequisiteStarted && !prefix.attempts().back().prerequisiteReturned);
    CHECK(!prefix.attempts().back().committed);
    // A refused source occurrence cannot be replayed as if the native setter resumed.
    driver.fail = false;
    CHECK(unsupported([&] { prefix.apply(rig.session, {2, BackendSetterSite::OldModelDetach, nullptr}, driver); }));
    CHECK(prefix.attempts().back().stop == BackendCommitStop::Occurrence);
    CHECK(driver.calls.size() == 2);
}

TEST_CASE(backend_commit_rejects_foreign_generation_and_control_block)
{
    Rig rig;
    Rig other;
    CHECK(unsupported([&] { BackendCommitPrefix prefix(rig.session, other.receiver()); }));
    auto receiver = rig.receiver();
    auto foreignControl = std::shared_ptr<FlowReceiverWitness>(receiver.get(), [](FlowReceiverWitness*) {});
    CHECK(unsupported([&] { BackendCommitPrefix prefix(rig.session, foreignControl); }));
    BackendCommitPrefix prefix(rig.session, receiver);
    DiagnosticPrerequisites driver;
    int backend;
    CHECK(unsupported([&] { prefix.apply(other.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, driver); }));
    CHECK(prefix.attempts().back().stop == BackendCommitStop::Binding);
    CHECK(driver.calls.empty() && !prefix.current(other.session));
}

TEST_CASE(backend_commit_rejects_released_project_before_prerequisite)
{
    Rig rig;
    const auto escaped = rig.receiver();
    BackendCommitPrefix prefix(rig.session, escaped);
    DiagnosticPrerequisites driver;
    int backend;
    prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, driver);
    rig.session.releaseProjectAfterCloseEffects(); // actual storage release, not native Close parity
    CHECK(!escaped->nativeReceiverAlive());
    CHECK(!prefix.current(rig.session));
    CHECK(unsupported([&] { prefix.apply(rig.session, {2, BackendSetterSite::OldModelDetach, nullptr}, driver); }));
    CHECK(prefix.attempts().back().stop == BackendCommitStop::Binding);
    CHECK(driver.calls.size() == 1);
}

TEST_CASE(backend_commit_revalidates_after_release_during_active_prerequisite)
{
    Rig rig;
    const auto escaped = rig.receiver();
    BackendCommitPrefix prefix(rig.session, escaped);
    DiagnosticPrerequisites driver;
    int backend;
    driver.during = [&](const BackendSetterPrerequisiteContext& context) {
        context.notifier.flag90 = true; // retained partial prerequisite effect
        rig.session.releaseProjectAfterCloseEffects();
    };
    CHECK(unsupported([&] { prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, driver); }));
    CHECK(!escaped->nativeReceiverAlive() && !prefix.current(rig.session));
    CHECK(rig.session.sessionNotificationStateOwner()->flag90);
    CHECK(prefix.attempts().back().prerequisiteReturned);
    CHECK(prefix.attempts().back().stop == BackendCommitStop::GenerationChanged);
    CHECK(!prefix.attempts().back().committed);
}

TEST_CASE(backend_commit_sub_death_never_transfers_state_to_replacement)
{
    Rig rig;
    const auto old = rig.receiver();
    BackendCommitPrefix prefix(rig.session, old);
    DiagnosticPrerequisites driver;
    int backend;
    prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, driver);
    auto& project = rig.session.project();
    project.subs.clear();
    project.subs.push_back(makeSubProject(project, 0, 1));
    const auto replacement = rig.session.flowReceiverWitnessRegistryOwner()->orderedWitnesses().back();
    CHECK(!old->nativeReceiverAlive() && replacement->nativeReceiverAlive());
    CHECK(replacement->ordinal() != old->ordinal());
    CHECK(!prefix.current(rig.session));
    BackendCommitPrefix fresh(rig.session, replacement);
    CHECK(!fresh.current(rig.session));
    CHECK(unsupported([&] { prefix.apply(rig.session, {2, BackendSetterSite::OldModelDetach, nullptr}, driver); }));
    CHECK(driver.calls.size() == 1);
}

TEST_CASE(backend_commit_unknown_interval_invalidates_without_erasing_history)
{
    Rig rig;
    BackendCommitPrefix prefix(rig.session, rig.receiver());
    DiagnosticPrerequisites driver;
    int backend;
    prefix.apply(rig.session, {1, BackendSetterSite::FirstNewModelAttach, &backend}, driver);
    prefix.invalidateCurrentEvidence("unqualified publication/write interval");
    CHECK(!prefix.current(rig.session));
    REQUIRE(prefix.invalidations().size() == 1);
    CHECK(prefix.attempts().size() == 1 && prefix.attempts()[0].committed);
    CHECK(unsupported([&] { prefix.invalidateCurrentEvidence(""); }));
    prefix.apply(rig.session, {2, BackendSetterSite::OldModelDetach, nullptr}, driver);
    CHECK(prefix.current(rig.session).has_value() && prefix.current(rig.session).value() == nullptr);
}

TEST_CASE(backend_commit_validates_occurrences_before_prerequisite_effects)
{
    Rig rig;
    BackendCommitPrefix prefix(rig.session, rig.receiver());
    DiagnosticPrerequisites driver;
    int backend;
    CHECK(unsupported([&] { prefix.apply(rig.session, {0, BackendSetterSite::FirstNewModelAttach, &backend}, driver); }));
    CHECK(unsupported([&] { prefix.apply(rig.session, {1, BackendSetterSite::OldModelDetach, &backend}, driver); }));
    CHECK(unsupported([&] { prefix.apply(rig.session, {2, BackendSetterSite::SecondNewModelAttach, nullptr}, driver); }));
    CHECK(unsupported([&] { prefix.apply(rig.session, {3, static_cast<BackendSetterSite>(99), &backend}, driver); }));
    CHECK(driver.calls.empty() && !prefix.current(rig.session));
    for (const auto& attempt : prefix.attempts()) CHECK(attempt.stop == BackendCommitStop::Occurrence);
}

TEST_CASE(backend_commit_does_not_qualify_set_opr_has_doe_population)
{
    Rig rig;
    auto& project = rig.session.project();
    project.doeIo = makeRecordingStudyIo();
    project.doeIo->bindSessionNotificationState(rig.session.sessionNotificationStateOwner());
    BackendCommitPrefix prefix(rig.session, rig.receiver());
    DiagnosticPrerequisites driver;
    prefix.apply(rig.session, {1, BackendSetterSite::OldModelDetach, nullptr}, driver);
    auto binding = bindSetOprHasDoeSessionPrefix(rig.session);
    CHECK(!binding->notifier().registrations);
    rig.session.sessionNotificationStateOwner()->flag90 = true;
    CHECK(unsupported([&] { binding->dispatch(rig.session, 1, true); }));
    CHECK(!rig.session.sessionNotificationStateOwner()->flag90);
    CHECK(binding->notifier().attempts.back().stop == SetOprHasDoeStop::Population);
    CHECK(!rig.receiver()->backendWitness());
}

int main() { return testkit::run_all(); }
