#include "variable_notify.hpp"

#include <algorithm>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "input_table_reader.hpp"

namespace m2m::doe {
namespace {
constexpr const char* kSignal = "2signalSetOprHasDoe()";

[[noreturn]] void blocked(SetOprHasDoeBindings& binding, std::size_t attemptIndex,
                         SetOprHasDoeStop stage, const char* reason)
{
    auto& attempt = binding.attempts.at(attemptIndex);
    attempt.stop = stage;
    attempt.error = reason;
    if (!attempt.visits.empty()) attempt.visits.back().outcome = SetOprHasDoeVisitOutcome::Stopped;
    throw Error(M2M_E_UNSUPPORTED, std::string(kSignal) + " prefix: " + reason);
}
} // namespace

SetOprHasDoeEntryOutcome enterSetOprHasDoeNotifierPrefix(
    const InputTableNotifierContext& context, Study& study, int operationId, bool hasDoe)
{
    auto refuseEntry = [](const char* reason) -> void {
        throw Error(M2M_E_UNSUPPORTED, std::string("SetOprHasDoe notifier entry: ") + reason);
    };
    if (!context.session || operationId != 1 || !hasDoe)
        refuseEntry("actual TLOC caller and int1/booltrue arguments are required");
    auto& session = *context.session; // Borrowed live Session for this synchronous call.
    auto* project = session.liveProject();
    // Compare/find owner membership before following saved SUB/I/O aliases.
    if (!project || project != context.project || !context.sub || context.study != &study ||
        std::none_of(project->subs.begin(), project->subs.end(),
            [&context](const auto& sub) { return sub.get() == context.sub; }))
        refuseEntry("actual live Project and owned SUB/Study context are required");
    const auto& sub = *context.sub;
    const auto io = project->doeIo;
    const auto expectedIo = context.ioOwner.lock();
    const auto notifier = session.sessionNotificationStateOwner();
    const auto manager = session.modelManagerOwner();
    if (sub.type != 2 || sub.project != project || !sub.ownedDoeStudy ||
        sub.ownedDoeStudy.get() != &study || sub.doe != &study || study.sub != &sub ||
        !io || !expectedIo || io.get() != context.io || io.get() != expectedIo.get() ||
        io.owner_before(expectedIo) || expectedIo.owner_before(io) ||
        !manager || io->modelManager() != manager.get() ||
        !notifier || io->observedSessionNotificationState() != notifier.get())
        refuseEntry("matching actual Study/I/O/manager/notifier owners are required");
    if (notifier->suppressed) // 2cf3f1: before flag, key, map or receiver work.
        return SetOprHasDoeEntryOutcome::SuppressedCalleeReturned;
    notifier->flag90 = false; // 2cf408, retained at the following key boundary.
    return SetOprHasDoeEntryOutcome::KeyConstructionRequired;
}

SetOprHasDoeBindings::SetOprHasDoeBindings(
    const std::shared_ptr<SessionNotificationState>& actualOwner, std::string qualification)
    : owner(actualOwner), ownerQualification(std::move(qualification))
{}

SetOprHasDoeSessionBinding::SetOprHasDoeSessionBinding(
    Project& project, const std::shared_ptr<StudyIo>& io,
    const std::shared_ptr<SessionNotificationState>& owner)
    : project_(&project), io_(io),
      notifier_(owner, "Actual ConversionSession notifier already bound to Project.doeIo; receiver population unavailable")
{
    notifier_.requiredProject = &project;
}

std::unique_ptr<SetOprHasDoeSessionBinding>
bindSetOprHasDoeSessionPrefix(engine::ConversionSession& session)
{
    auto* project = session.liveProject();
    const auto owner = session.sessionNotificationStateOwner();
    if (!project || !project->doeIo || !owner ||
        project->doeIo->observedSessionNotificationState() != owner.get())
        throw Error(M2M_E_UNSUPPORTED,
                    "SetOprHasDoe session binding requires the live session Project and its actual bound DOE notifier");
    return std::unique_ptr<SetOprHasDoeSessionBinding>(
        new SetOprHasDoeSessionBinding(*project, project->doeIo, owner));
}

void SetOprHasDoeSessionBinding::dispatch(engine::ConversionSession& session,
                                         int operationId, bool hasDoe)
{
    auto* project = session.liveProject();
    const auto io = io_.lock();
    const auto owner = notifier_.owner.lock();
    const auto sessionOwner = session.sessionNotificationStateOwner();
    // Compare identity before dereferencing the saved Project pointer. External
    // I/O/notifier leases do not keep a released session Project alive.
    if (!project || project != project_ || notifier_.requiredProject != project ||
        !io || project->doeIo != io ||
        !owner || owner != sessionOwner || io->observedSessionNotificationState() != owner.get())
        throw Error(M2M_E_UNSUPPORTED,
                    "SetOprHasDoe session binding is stale or does not match the actual Project/I/O/notifier owner");
    dispatchSetOprHasDoePrefix(*io, notifier_, operationId, hasDoe);
}

void dispatchSetOprHasDoePrefix(StudyIo& io, SetOprHasDoeBindings& binding,
                               int operationId, bool hasDoe)
{
    const auto attemptIndex = binding.attempts.size();
    binding.attempts.emplace_back();
    binding.attempts[attemptIndex].operationId = operationId;
    binding.attempts[attemptIndex].hasDoe = hasDoe;
    const auto owner = binding.owner.lock();
    // The observed accessor does not create the notifier. Qualify identity
    // before representing any native dispatcher effect.
    if (!owner || binding.ownerQualification.empty() || io.observedSessionNotificationState() != owner.get())
        blocked(binding, attemptIndex, SetOprHasDoeStop::Owner, "actual shared notifier owner is unavailable");
    binding.attempts[attemptIndex].ownerVerified = true;
    binding.attempts[attemptIndex].suppressed = owner->suppressed;
    if (owner->suppressed) {                            // 1402cf3f1, before any map/list read
        binding.attempts[attemptIndex].completion = SetOprHasDoeCompletion::SuppressedReturn;
        return;
    }
    owner->flag90 = false;                              // 1402cf408, retained on later refusal
    binding.attempts[attemptIndex].flagReset = true;
    if (!binding.registrations || binding.registrations->qualification.empty())
        blocked(binding, attemptIndex, SetOprHasDoeStop::Population, "ordered receiver population is unavailable");
    auto& list = *binding.registrations;
    if (!list.mapKeyPresent && !list.entries.empty())
        blocked(binding, attemptIndex, SetOprHasDoeStop::Population, "absent map key contradicts nonempty registrations");
    if (!list.mapKeyPresent) {
        list.mapKeyPresent = true;                      // 1402d03f0 default insertion
        owner->emptyKeys.insert(kSignal);
        binding.attempts[attemptIndex].keyInserted = true;
    }
    const auto snapshot = list.entries;                 // 1402cf461: local traversal copy
    binding.attempts[attemptIndex].snapshotTaken = true;
    for (std::size_t i = 0; i < snapshot.size(); ++i) {
        const auto visitIndex = binding.attempts[attemptIndex].visits.size();
        binding.attempts[attemptIndex].visits.push_back({i});
        const auto receiver = snapshot[i].receiver.lock();
        if (!receiver) {
            binding.attempts[attemptIndex].visits[visitIndex].outcome = SetOprHasDoeVisitOutcome::ExpiredGuard;
            continue;
        }
        if (!receiver->nativeReceiverAlive()) {
            binding.attempts[attemptIndex].visits[visitIndex].outcome = SetOprHasDoeVisitOutcome::NativeDead;
            continue;
        }
        if (snapshot[i].bareMethod != "slotSetOprHasDoe")
            blocked(binding, attemptIndex, SetOprHasDoeStop::Method, "live receiver method is outside the recovered slot");
        if (!receiver->nativeReceiverAlive())
            blocked(binding, attemptIndex, SetOprHasDoeStop::ReceiverLifetime, "live QObject guard changed at invocation");
        const auto* project = receiver->actualProject();
        const auto* sub = receiver->actualSubProject();
        // Validate ownership before dereferencing a possibly stale sub alias.
        if (!project || (binding.requiredProject && project != binding.requiredProject) ||
            !sub || std::none_of(project->subs.begin(), project->subs.end(),
                [sub](const auto& owned) { return owned.get() == sub; }))
            blocked(binding, attemptIndex, SetOprHasDoeStop::ModelIdentity, "actual flow model owner is unavailable");
        const auto* model = receiver->actualFlowModel();
        if (sub->project != project || model != &sub->flow)
            blocked(binding, attemptIndex, SetOprHasDoeStop::ModelIdentity, "flow model is not the actual owned sub-project model");
        binding.attempts[attemptIndex].visits[visitIndex].model = model;
        const auto backend = receiver->backendWitness();
        if (!backend.has_value())
            blocked(binding, attemptIndex, SetOprHasDoeStop::BackendWitness, "actual diagram backend state is unavailable");
        binding.attempts[attemptIndex].visits[visitIndex].backend = *backend;
        if (*backend == nullptr) {                     // 140086cc7: before logical item lookup
            binding.attempts[attemptIndex].visits[visitIndex].outcome = SetOprHasDoeVisitOutcome::NullBackend;
            continue;
        }
        const FlowItem* match = nullptr;
        for (const auto& item : model->items) {          // 140086820: first matching vt+28 value
            if (!item)
                blocked(binding, attemptIndex, SetOprHasDoeStop::LogicalItem, "logical item pointer is unavailable");
            if (item->opId == operationId) { match = item.get(); break; }
        }
        if (!match) {
            binding.attempts[attemptIndex].visits[visitIndex].outcome = SetOprHasDoeVisitOutcome::MissingItem;
            continue;
        }
        binding.attempts[attemptIndex].visits[visitIndex].item = match;
        if (hasDoe)
            blocked(binding, attemptIndex, SetOprHasDoeStop::IconAllocation,
                    "matched live backend requires QIcon allocation/resource ownership (140086d0d)");
        blocked(binding, attemptIndex, SetOprHasDoeStop::IconBackend,
                "matched live backend requires primary-icon clear ownership (140086e03)");
    }
    // Native invokeMethod uses DirectConnection1 and ignores its Boolean. No
    // callback, aggregate false result or parser-success claim is invented here.
    binding.attempts[attemptIndex].completion = SetOprHasDoeCompletion::ReceiverListReturned;
}

} // namespace m2m::doe
