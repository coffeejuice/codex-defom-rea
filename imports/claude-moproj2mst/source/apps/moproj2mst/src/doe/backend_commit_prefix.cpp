#include "m2m/doe/backend_commit_prefix.hpp"

#include <algorithm>
#include <exception>
#include <utility>

#include "flow_receiver_witness.hpp"
#include "m2m/common.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"

namespace m2m::doe {
namespace {
[[noreturn]] void unsupported(const char* reason)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("DOE backend setter prefix: ") + reason);
}

std::uint64_t callerSite(BackendSetterSite site)
{
    switch (site) {
    case BackendSetterSite::OldModelDetach: return 0x140170540ULL;
    case BackendSetterSite::FirstNewModelAttach: return 0x140170581ULL;
    case BackendSetterSite::SecondNewModelAttach: return 0x1401705f4ULL;
    }
    return 0;
}
} // namespace

void BackendSetterPrerequisiteDriver::registerCanAddItem(const BackendSetterPrerequisiteContext&)
{ unsupported("DiagModelCanAddItem registration prefix is unavailable (140071b69)"); }

void BackendSetterPrerequisiteDriver::removeCanAddItem(const BackendSetterPrerequisiteContext&)
{ unsupported("DiagModelCanAddItem removal prefix is unavailable (140071b8e)"); }

struct BackendCommitPrefix::State {
    const Project* project = nullptr; // comparison only until live Session validation
    std::weak_ptr<FlowReceiverWitnessRegistry> registry;
    std::weak_ptr<FlowReceiverWitness> receiver;
    std::weak_ptr<SessionNotificationState> notifier;
    std::optional<const void*> value;
    std::vector<BackendCommitAttempt> attempts;
    std::vector<std::string> invalidations;
    bool inFlight = false;

    bool matches(engine::ConversionSession& session) const noexcept
    {
        const auto* actualProject = session.liveProject();
        if (!actualProject || actualProject != project) return false;
        const auto actualRegistry = registry.lock();
        const auto actualReceiver = receiver.lock();
        const auto actualNotifier = notifier.lock();
        if (!actualRegistry || !actualReceiver || !actualNotifier ||
            actualRegistry != session.flowReceiverWitnessRegistryOwner() ||
            actualNotifier != session.sessionNotificationStateOwner() ||
            actualRegistry->owner() != actualNotifier) return false;
        const auto& history = actualRegistry->orderedWitnesses();
        const bool member = std::any_of(history.begin(), history.end(), [&](const auto& entry) {
            return entry.get() == actualReceiver.get() && !entry.owner_before(actualReceiver) &&
                   !actualReceiver.owner_before(entry);
        });
        if (!member || !actualReceiver->nativeReceiverAlive() ||
            actualReceiver->actualProject() != actualProject) return false;
        const auto* sub = actualReceiver->actualSubProject();
        if (!sub || std::none_of(actualProject->subs.begin(), actualProject->subs.end(),
                [sub](const auto& entry) { return entry.get() == sub; })) return false;
        return sub->project == actualProject && actualReceiver->actualFlowModel() == &sub->flow &&
               actualReceiver->identityToken() == &sub->flow;
    }
};

BackendCommitPrefix::BackendCommitPrefix(engine::ConversionSession& session,
    const std::shared_ptr<FlowReceiverWitness>& receiver) : state_(std::make_unique<State>())
{
    state_->project = session.liveProject();
    state_->registry = session.flowReceiverWitnessRegistryOwner();
    state_->receiver = receiver;
    state_->notifier = session.sessionNotificationStateOwner();
    if (!state_->matches(session))
        unsupported("actual live Session/Project/notifier/model generation is unavailable");
}

BackendCommitPrefix::~BackendCommitPrefix() = default;

void BackendCommitPrefix::apply(engine::ConversionSession& session, BackendSetterOccurrence occurrence,
                                BackendSetterPrerequisiteDriver& driver)
{
    // No nested adapter call is authorized by this selected registration prefix.
    // This is an adapter scope guard, not invented native event-loop behavior.
    if (state_->inFlight) unsupported("a setter occurrence is already in flight");
    const auto index = state_->attempts.size();
    state_->attempts.push_back({occurrence, callerSite(occurrence.site)});
    auto refuse = [&](BackendCommitStop stage, const char* reason) {
        state_->attempts[index].stop = stage;
        state_->attempts[index].error = reason;
        unsupported(reason);
    };
    if (!state_->matches(session))
        refuse(BackendCommitStop::Binding, "actual live Session/Project/notifier/model generation is unavailable");
    if (!occurrence.diagramCallId || !callerSite(occurrence.site) ||
        ((occurrence.site == BackendSetterSite::OldModelDetach) != (occurrence.backend == nullptr)))
        refuse(BackendCommitStop::Occurrence, "source setter occurrence or backend argument is unavailable");
    for (std::size_t i = 0; i < index; ++i)
        if (state_->attempts[i].occurrence.diagramCallId == occurrence.diagramCallId &&
            state_->attempts[i].occurrence.site == occurrence.site)
            refuse(BackendCommitStop::Occurrence, "source setter occurrence was already attempted; retry is unavailable");

    const auto receiver = state_->receiver.lock();
    const auto notifier = state_->notifier.lock();
    const auto* sub = receiver->actualSubProject();
    BackendSetterPrerequisiteContext context{*notifier, *receiver, *state_->project, *sub, occurrence};
    state_->inFlight = true;
    struct EndScope { bool& flag; ~EndScope() { flag = false; } } endScope{state_->inFlight};
    try {
        state_->attempts[index].prerequisiteQualification = driver.qualification();
        if (state_->attempts[index].prerequisiteQualification.empty())
            refuse(BackendCommitStop::Prerequisite, occurrence.backend
                ? "DiagModelCanAddItem registration prefix is unavailable (140071b69)"
                : "DiagModelCanAddItem removal prefix is unavailable (140071b8e)");
        state_->attempts[index].prerequisiteStarted = true;
        if (occurrence.backend) driver.registerCanAddItem(context); // BEFORE14014c404
        else driver.removeCanAddItem(context);
        state_->attempts[index].prerequisiteReturned = true;
    } catch (const std::exception& error) {
        state_->attempts[index].stop = BackendCommitStop::Prerequisite;
        state_->attempts[index].error = error.what();
        throw;
    } catch (...) {
        state_->attempts[index].stop = BackendCommitStop::Prerequisite;
        state_->attempts[index].error = "prerequisite driver did not return";
        throw;
    }
    if (!state_->matches(session)) {
        state_->value.reset();
        refuse(BackendCommitStop::GenerationChanged, "actual model generation changed before backend store");
    }
    // Leaf14014c3f0, store14014c404: interface+8 / whole-model+18. This
    // projection does not publish a backend witness to the receiver registry.
    state_->value = occurrence.backend;
    state_->attempts[index].committed = true;
}

std::optional<const void*> BackendCommitPrefix::current(engine::ConversionSession& session) const noexcept
{ return state_->matches(session) ? state_->value : std::nullopt; }

void BackendCommitPrefix::invalidateCurrentEvidence(std::string reason)
{
    if (reason.empty()) unsupported("backend evidence invalidation requires a source reason");
    state_->invalidations.push_back(std::move(reason));
    state_->value.reset();
}

const std::vector<BackendCommitAttempt>& BackendCommitPrefix::attempts() const noexcept
{ return state_->attempts; }

const std::vector<std::string>& BackendCommitPrefix::invalidations() const noexcept
{ return state_->invalidations; }

} // namespace m2m::doe
