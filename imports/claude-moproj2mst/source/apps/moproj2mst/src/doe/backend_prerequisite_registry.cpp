#include "m2m/doe/backend_prerequisite_registry.hpp"

#include <algorithm>
#include <cstring>
#include <exception>
#include <limits>
#include <utility>

#include "flow_receiver_witness.hpp"
#include "m2m/common.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"

namespace m2m::doe {
namespace {
constexpr const char* kSignal = "2signalDiagModelCanAddItem()";
constexpr const char* kSlot = "1slotDiagModelCanAddItem()";
constexpr const char* kBareMethod = "slotDiagModelCanAddItem";

[[noreturn]] void unsupported(const char* reason)
{ throw Error(M2M_E_UNSUPPORTED, std::string("DOE backend prerequisite: ") + reason); }

bool sameReceiver(const std::weak_ptr<FlowReceiverWitness>& a,
                  const std::weak_ptr<FlowReceiverWitness>& b)
{
    return !a.owner_before(b) && !b.owner_before(a) && a.lock().get() == b.lock().get();
}

bool bareAscii(const std::string& text)
{
    return !text.empty() && std::all_of(text.begin(), text.end(), [](unsigned char c) {
        return c > 0 && c < 128 && c != '(' && c != ')';
    });
}
} // namespace

struct BackendPrerequisiteRegistry::State {
    engine::ConversionSession* session = nullptr; // caller-owned; must outlive calls
    const Project* project = nullptr; // identity only until live Session check
    std::weak_ptr<FlowReceiverWitnessRegistry> registry;
    std::weak_ptr<SessionNotificationState> notifier;
    std::optional<BackendPrerequisiteMapState> map;
    std::vector<BackendPrerequisiteRecord> records; // retained, including orphan/removed records
    std::vector<BackendPrerequisiteAttempt> attempts;
    DiagnosticObserver observer;
    bool inFlight = false;

    bool scopeMatches() const noexcept
    {
        if (!session || !session->liveProject() || session->liveProject() != project) return false;
        const auto actualRegistry = registry.lock();
        const auto actualNotifier = notifier.lock();
        return actualRegistry && actualNotifier && actualRegistry == session->flowReceiverWitnessRegistryOwner() &&
            actualNotifier == session->sessionNotificationStateOwner() && actualRegistry->owner() == actualNotifier;
    }

    std::shared_ptr<FlowReceiverWitness> member(const std::weak_ptr<FlowReceiverWitness>& supplied) const
    {
        const auto actualRegistry = registry.lock();
        const auto receiver = supplied.lock();
        if (!actualRegistry || !receiver) return {};
        for (const auto& entry : actualRegistry->orderedWitnesses())
            if (entry.get() == receiver.get() && !entry.owner_before(receiver) &&
                !receiver.owner_before(entry)) return receiver;
        return {};
    }

    bool ownedLive(const FlowReceiverWitness& receiver) const noexcept
    {
        if (!receiver.nativeReceiverAlive() || receiver.actualProject() != project) return false;
        const auto* sub = receiver.actualSubProject();
        if (!sub || std::none_of(project->subs.begin(), project->subs.end(),
                [sub](const auto& entry) { return entry.get() == sub; })) return false;
        return sub->project == project && receiver.actualFlowModel() == &sub->flow &&
            receiver.identityToken() == &sub->flow;
    }

    bool contextMatches(const BackendSetterPrerequisiteContext& context) const
    {
        if (!scopeMatches() || &context.notifier != notifier.lock().get() || &context.project != project) return false;
        const auto actualRegistry = registry.lock();
        for (const auto& entry : actualRegistry->orderedWitnesses())
            if (entry.get() == &context.receiver)
                return ownedLive(*entry) && entry->actualSubProject() == &context.sub;
        return false;
    }

    const BackendPrerequisiteRecord* record(std::uint64_t identity) const noexcept
    {
        const auto found = std::find_if(records.begin(), records.end(),
            [identity](const auto& entry) { return entry.identity == identity; });
        return found == records.end() ? nullptr : &*found;
    }

    std::uint64_t nextIdentity() const
    {
        std::uint64_t highest = 0;
        for (const auto& entry : records) highest = std::max(highest, entry.identity);
        if (highest == std::numeric_limits<std::uint64_t>::max())
            unsupported("registration identity space is unavailable");
        return highest + 1;
    }

    void observe(BackendPrerequisiteStage stage, std::size_t index)
    {
        const auto diagnostic = observer;
        if (diagnostic) diagnostic(stage, attempts[index]);
    }
};

BackendPrerequisiteRegistry::BackendPrerequisiteRegistry(engine::ConversionSession& session)
    : state_(std::make_unique<State>())
{
    state_->session = &session;
    state_->project = session.liveProject();
    state_->registry = session.flowReceiverWitnessRegistryOwner();
    state_->notifier = session.sessionNotificationStateOwner();
    if (!state_->scopeMatches()) unsupported("actual Session/Project/registry/notifier owner is unavailable");
}

BackendPrerequisiteRegistry::~BackendPrerequisiteRegistry() = default;

void BackendPrerequisiteRegistry::bindInitialMap(BackendPrerequisiteMapProfile profile)
{
    if (state_->inFlight || !state_->scopeMatches()) unsupported("initial map binding has no live idle actual owner");
    if (state_->map) unsupported("initial map binding cannot replace existing same-key state");
    if (profile.map.qualification.empty() || (!profile.map.keyPresent && !profile.map.orderedRecordIds.empty()))
        unsupported("same-key map profile is unavailable or contradictory");
    auto proposed = state_->records; // adapter evidence admission, not a native map operation
    std::vector<std::uint64_t> importedIds;
    for (auto& record : profile.records) {
        if (!record.identity || !bareAscii(record.bareMethod) ||
            std::find(importedIds.begin(), importedIds.end(), record.identity) != importedIds.end())
            unsupported("initial record identity/method is unavailable or duplicated");
        const auto receiver = state_->member(record.receiver);
        if (!receiver || (receiver->nativeReceiverAlive() && !state_->ownedLive(*receiver)))
            unsupported("initial record has no actual same-registry receiver generation");
        importedIds.push_back(record.identity);
        const auto found = std::find_if(proposed.begin(), proposed.end(),
            [&](const auto& existing) { return existing.identity == record.identity; });
        if (found != proposed.end()) {
            if (found->bareMethod != record.bareMethod || !sameReceiver(found->receiver, record.receiver))
                unsupported("initial record identity contradicts a retained record");
        } else {
            record.origin = BackendPrerequisiteRecordOrigin::ImportedProfile;
            proposed.push_back(std::move(record));
        }
    }
    for (const auto id : profile.map.orderedRecordIds)
        if (std::none_of(proposed.begin(), proposed.end(),
                [id](const auto& record) { return record.identity == id; }))
            unsupported("initial ordered map references an unavailable record identity");
    state_->records = std::move(proposed);
    state_->map = std::move(profile.map);
}

std::string BackendPrerequisiteRegistry::qualification() const
{ return "Actual Session owner; isolated DiagModelCanAddItem prefix; map history requires explicit binding"; }

void BackendPrerequisiteRegistry::registerCanAddItem(const BackendSetterPrerequisiteContext& context)
{ execute(context, BackendPrerequisiteAction::Register); }

void BackendPrerequisiteRegistry::removeCanAddItem(const BackendSetterPrerequisiteContext& context)
{ execute(context, BackendPrerequisiteAction::Remove); }

void BackendPrerequisiteRegistry::execute(const BackendSetterPrerequisiteContext& context,
                                         BackendPrerequisiteAction action)
{
    if (state_->inFlight) unsupported("a prerequisite is already in flight");
    const auto index = state_->attempts.size();
    state_->attempts.push_back({action, context.occurrence});
    auto refuse = [&](BackendPrerequisiteStop stop, const char* reason) {
        state_->attempts[index].stop = stop;
        state_->attempts[index].error = reason;
        unsupported(reason);
    };
    if (!state_->contextMatches(context))
        refuse(BackendPrerequisiteStop::Binding, "actual notifier/model generation is unavailable");
    if (!context.signal || !context.slot || std::strcmp(context.signal, kSignal) ||
        std::strcmp(context.slot, kSlot) || context.prepend ||
        ((action == BackendPrerequisiteAction::Register) != (context.occurrence.backend != nullptr)))
        refuse(BackendPrerequisiteStop::Context, "only the exact flag0 backend-setter key/slot/branch is recovered");
    state_->inFlight = true;
    struct EndScope { bool& flag; ~EndScope() { flag = false; } } endScope{state_->inFlight};
    auto requireGeneration = [&] {
        if (!state_->contextMatches(context))
            refuse(BackendPrerequisiteStop::GenerationChanged, "actual model generation ended during prerequisite");
    };
    try {
        if (action == BackendPrerequisiteAction::Register) {
            const auto id = state_->nextIdentity();
            const auto registry = state_->registry.lock();
            const auto& history = registry->orderedWitnesses();
            const auto receiver = *std::find_if(history.begin(), history.end(),
                [&](const auto& entry) { return entry.get() == &context.receiver; });
            // 2cfc17 allocates0x10; normalize/assign receiver+method BEFORE map
            // membership2cfd8b. Retain this record through later refusal/dedup.
            state_->records.push_back({id, receiver, kBareMethod,
                                      BackendPrerequisiteRecordOrigin::RegistrationPrefix});
            state_->attempts[index].allocatedRecord = id;
            state_->observe(BackendPrerequisiteStage::RecordReady, index);
            requireGeneration();
        }
        if (!state_->map)
            refuse(BackendPrerequisiteStop::MapState, "actual same-key map state is unavailable");
        auto& map = *state_->map;
        if (action == BackendPrerequisiteAction::Remove && !map.keyPresent) {
            // Removal2d002a operator[] happens before method normalization.
            map.keyPresent = true;
            state_->attempts[index].keyInserted = true;
            state_->observe(BackendPrerequisiteStage::KeyInserted, index);
            requireGeneration();
        }
        state_->attempts[index].localRecordIds = map.orderedRecordIds;
        state_->attempts[index].snapshotTaken = true;
        std::optional<std::uint64_t> firstMatch;
        for (const auto id : state_->attempts[index].localRecordIds) {
            const auto* record = state_->record(id);
            if (!record) refuse(BackendPrerequisiteStop::Record, "ordered record identity is unavailable");
            const auto receiver = state_->member(record->receiver);
            // Native non-null receiver comparison fails for a dead QObject
            // guard. Do not prune expired entries or dereference stale context.
            if (receiver && receiver->nativeReceiverAlive() && receiver.get() == &context.receiver &&
                record->bareMethod == kBareMethod) { firstMatch = id; break; }
        }
        if (action == BackendPrerequisiteAction::Register) {
            state_->attempts[index].duplicate = firstMatch.has_value();
            if (!firstMatch) state_->attempts[index].localRecordIds.push_back(
                *state_->attempts[index].allocatedRecord);
        } else if (firstMatch) {
            auto& local = state_->attempts[index].localRecordIds;
            const auto oldCount = local.size();
            local.erase(std::remove(local.begin(), local.end(), *firstMatch), local.end());
            state_->attempts[index].removedRecord = firstMatch;
            state_->attempts[index].aliasesRemoved = oldCount - local.size();
        }
        state_->observe(BackendPrerequisiteStage::LocalListReady, index);
        requireGeneration();
        // Registration2cff24 / removal2d01e1: write back even on duplicate/miss.
        if (!map.keyPresent) {
            map.keyPresent = true;
            state_->attempts[index].keyInserted = true;
        }
        map.orderedRecordIds = state_->attempts[index].localRecordIds;
        state_->attempts[index].mapWritten = true;
        state_->observe(BackendPrerequisiteStage::MapWritten, index);
        state_->attempts[index].completed = true;
    } catch (const std::exception& error) {
        if (!state_->attempts[index].stop) {
            state_->attempts[index].stop = BackendPrerequisiteStop::Effect;
            state_->attempts[index].error = error.what();
        }
        throw;
    } catch (...) {
        state_->attempts[index].stop = BackendPrerequisiteStop::Effect;
        state_->attempts[index].error = "prerequisite effect did not return";
        throw;
    }
}

const std::optional<BackendPrerequisiteMapState>& BackendPrerequisiteRegistry::observedMap() const noexcept
{ return state_->map; }

const std::vector<BackendPrerequisiteRecord>& BackendPrerequisiteRegistry::retainedRecords() const noexcept
{ return state_->records; }

const std::vector<BackendPrerequisiteAttempt>& BackendPrerequisiteRegistry::attempts() const noexcept
{ return state_->attempts; }

void BackendPrerequisiteRegistry::setDiagnosticObserver(DiagnosticObserver observer)
{ state_->observer = std::move(observer); }

} // namespace m2m::doe
