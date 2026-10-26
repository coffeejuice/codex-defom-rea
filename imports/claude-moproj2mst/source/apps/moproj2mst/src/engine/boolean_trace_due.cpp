#include "m2m/engine/boolean_trace_due.hpp"

#include <algorithm>
#include <set>
#include <sstream>

namespace m2m::engine {
namespace {
bool hashValid(const std::string& s)
{
    return s.size() == 64 && std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    });
}
bool identityValid(const BooleanTraceIdentity& i)
{
    return hashValid(i.executableSha256) && hashValid(i.qtCoreSha256) && hashValid(i.qtGuiSha256) &&
        hashValid(i.scriptSha256) && hashValid(i.fixtureSha256) &&
        !i.actionProfile.empty() && !i.nativeSession.empty();
}
bool keyValid(const BooleanNativeTimerKey& k)
{
    return k.receiverAddress && k.receiverLifetime && k.timerAddress && k.timerLifetime && k.registrationOrdinal;
}
bool samePoint(const PumpPoint& a, const PumpPoint& b)
{ return a.sourceSite == b.sourceSite && a.occurrence == b.occurrence; }
}

BooleanTraceDueResult bindBooleanTraceDue(const DiagramLifecycleEvents& owner,
    const BooleanTraceBindingContext& context, const BooleanPumpTrace& trace)
{
    auto refused = [](const char* issue) { return BooleanTraceDueResult{std::nullopt, issue}; };
    if (context.logicalOwner != &owner || !context.ownerScope || context.ownerScope->owner_ != &owner)
        return refused("Trace mapping belongs to a different logical event owner/admission scope");
    if (!identityValid(context.expectedIdentity) || !identityValid(trace.identity) ||
        trace.identity != context.expectedIdentity)
        return refused("Trace binary/script/fixture/action/session identity is missing or mismatched");
    if (context.expectedPoint.sourceSite.empty() || !samePoint(context.expectedPoint, trace.point) ||
        !context.expectedNativeThread || trace.nativeThread != context.expectedNativeThread)
        return refused("Trace belongs to another pump occurrence or native thread");
    if (!trace.complete || !trace.balanced || !trace.registrationsComplete || trace.truncated ||
        trace.readErrors || trace.observerErrors || trace.unmatchedFrames || trace.evidence.empty() ||
        !trace.enterOrdinal || trace.returnOrdinal <= trace.enterOrdinal)
        return refused("Native trace interval is unavailable, incomplete or unbalanced");
    // Explicit diagnostic bounds, not original Qt timer population limits.
    if (context.registrations.size() > 1024 || trace.deliveries.size() > 4096)
        return refused("Trace exceeds the bounded binder capacity");

    std::set<DiagramLifecycleEvents::TimerId> logicalTimers;
    std::set<std::pair<std::uint64_t, std::uint64_t>> timerLifetimes;
    std::set<std::uint64_t> registrationOrdinals;
    std::vector<DiagramLifecycleEvents::PendingModelUpdate> registrations;
    for (const auto& mapping : context.registrations) {
        if (!keyValid(mapping.native) || mapping.nativeTimerId <= 0 || mapping.requestedIntervalMs != 1000 ||
            mapping.nativeThread != trace.nativeThread ||
            !mapping.logicalReceiver || !mapping.logicalTimer || mapping.evidence.empty())
            return refused("Timer registration lacks actual native lifetime/positive ID/interval evidence");
        if (!logicalTimers.insert(mapping.logicalTimer).second ||
            !timerLifetimes.emplace(mapping.native.timerAddress, mapping.native.timerLifetime).second ||
            !registrationOrdinals.insert(mapping.native.registrationOrdinal).second)
            return refused("Native-to-logical timer registration mapping is ambiguous");
        // A receiver address may have several generations only with distinct
        // native lifetime IDs. Same-generation logical receiver aliases agree.
        for (const auto& peer : context.registrations) {
            if (peer.native.receiverAddress == mapping.native.receiverAddress &&
                peer.native.receiverLifetime == mapping.native.receiverLifetime &&
                peer.logicalReceiver != mapping.logicalReceiver)
                return refused("Native receiver lifetime maps to conflicting logical receivers");
            if (peer.logicalReceiver == mapping.logicalReceiver &&
                (peer.native.receiverAddress != mapping.native.receiverAddress ||
                 peer.native.receiverLifetime != mapping.native.receiverLifetime))
                return refused("Logical receiver substitutes another native receiver lifetime");
        }
        const auto pending = std::find_if(owner.pending().begin(), owner.pending().end(), [&](const auto& p) {
            return p.id == mapping.logicalTimer && p.receiver == mapping.logicalReceiver &&
                p.requestedIntervalMs == mapping.requestedIntervalMs;
        });
        if (pending == owner.pending().end())
            return refused("Trace registration does not map to the same currently pending logical timer");
        registrations.push_back(*pending);
    }

    std::vector<DiagramLifecycleEvents::TimerId> due;
    std::set<DiagramLifecycleEvents::TimerId> delivered;
    std::uint64_t previousEnter = 0;
    std::uint64_t previousReturn = 0;
    for (const auto& row : trace.deliveries) {
        if (!keyValid(row.native) || !row.timerEventObserved || !row.modelUpdatedObserved ||
            !row.matchedCallbackReceiver || !row.balanced || row.evidence.empty() ||
            row.nativeThread != trace.nativeThread || row.enterOrdinal <= trace.enterOrdinal ||
            row.returnOrdinal >= trace.returnOrdinal || row.returnOrdinal <= row.enterOrdinal ||
            row.enterOrdinal <= previousEnter)
            return refused("Delivery lacks complete ordered timerEvent/callback evidence in this pump interval");
        previousEnter = row.enterOrdinal;
        if (row.enterOrdinal <= previousReturn)
            return refused("Nested/overlapping native callbacks have no flat dispatch representation");
        previousReturn = row.returnOrdinal;
        const auto mapping = std::find_if(context.registrations.begin(), context.registrations.end(),
            [&](const auto& m) { return m.native == row.native && m.nativeTimerId == row.nativeTimerId; });
        if (mapping == context.registrations.end())
            return refused("Observed delivery has no exact native registration/lifetime mapping");
        if (!delivered.insert(mapping->logicalTimer).second)
            return refused("One logical single-shot timer is delivered twice in this interval");
        due.push_back(mapping->logicalTimer);
    }

    std::ostringstream qualification;
    qualification << "OriginalTrace " << trace.evidence << "; action=" << trace.identity.actionProfile
        << "; session=" << trace.identity.nativeSession << "; thread=" << trace.nativeThread
        << "; interval=" << trace.enterOrdinal << ':' << trace.returnOrdinal
        << "; exe=" << trace.identity.executableSha256 << "; core=" << trace.identity.qtCoreSha256
        << "; gui=" << trace.identity.qtGuiSha256 << "; script=" << trace.identity.scriptSha256
        << "; fixture=" << trace.identity.fixtureSha256;
    DueObservation observation{trace.point, DueObservation::Origin::OriginalTrace, qualification.str(), std::move(due)};
    BooleanBoundTraceDue binding(&owner, *context.ownerScope, std::move(observation), std::move(registrations));
    return {std::move(binding), {}};
}

BooleanTraceDueValidation validateBooleanTraceDueForOwner(const DiagramLifecycleEvents& owner,
    const BooleanTraceOwnerScope& scope, const BooleanBoundTraceDue& binding)
{
    auto refused = [](const char* issue) { return BooleanTraceDueValidation{std::nullopt, issue}; };
    const auto lifetime = binding.lifetime_.lock();
    if (!lifetime || lifetime != scope.lifetime_ || binding.scope_ != &scope ||
        binding.owner_ != &owner || scope.owner_ != &owner)
        return refused("Bound trace owner/admission scope expired, changed or was rebound");
    for (const auto& registered : binding.registrations_) {
        const auto pending = std::find_if(owner.pending().begin(), owner.pending().end(), [&](const auto& p) {
            return p.id == registered.id && p.receiver == registered.receiver &&
                p.requestedIntervalMs == registered.requestedIntervalMs;
        });
        if (pending == owner.pending().end())
            return refused("Bound trace registrations changed before same-owner admission");
    }
    return {binding.observation_, {}};
}

} // namespace m2m::engine
