#include "flow_receiver_witness.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/model.hpp"

namespace m2m::doe {

FlowReceiverWitness::FlowReceiverWitness(FlowReceiverConstructionSource source,
    const void* identity, std::size_t ordinal)
    : source_(source), identity_(identity), ordinal_(ordinal) {}

std::uint64_t FlowReceiverWitness::registrationSite() const noexcept
{
    return source_ == FlowReceiverConstructionSource::NormalSub ? 0x140071166ULL : 0x140071a7fULL;
}

FlowReceiverWitnessRegistry::FlowReceiverWitnessRegistry(
    const std::shared_ptr<SessionNotificationState>& owner, std::string qualification)
    : owner_(owner), qualification_(std::move(qualification))
{ requireScope(); }

FlowReceiverWitnessRegistry::~FlowReceiverWitnessRegistry() { releaseAdapterScope(); }

void FlowReceiverWitnessRegistry::requireScope() const
{
    if (released_ || owner_.expired() || qualification_.empty())
        throw Error(M2M_E_UNSUPPORTED, "Flow receiver witness registry scope/owner is unavailable");
}

FlowReceiverWitness& FlowReceiverWitnessRegistry::member(
    const std::shared_ptr<FlowReceiverWitness>& supplied) const
{
    for (const auto& entry : witnesses_)
        if (supplied && entry.get() == supplied.get() &&
            !entry.owner_before(supplied) && !supplied.owner_before(entry)) return *entry;
    throw Error(M2M_E_UNSUPPORTED, "Flow receiver witness belongs to another registry/owner");
}

std::shared_ptr<FlowReceiverWitness> FlowReceiverWitnessRegistry::record(
    FlowReceiverConstructionSource source, const void* identity)
{
    requireScope();
    if (!identity)
        throw Error(M2M_E_UNSUPPORTED, "Flow receiver constructor identity is unavailable");
    for (const auto& entry : witnesses_)
        if (entry->alive_ && entry->identity_ == identity)
            throw Error(M2M_E_UNSUPPORTED, "Flow receiver constructor identity is already live");
    auto result = std::shared_ptr<FlowReceiverWitness>(
        new FlowReceiverWitness(source, identity, witnesses_.size()));
    witnesses_.push_back(result);
    return result;
}

std::shared_ptr<FlowReceiverWitness> FlowReceiverWitnessRegistry::recordNormalSubConstruction(const void* identity)
{ return record(FlowReceiverConstructionSource::NormalSub, identity); }

std::shared_ptr<FlowReceiverWitness> FlowReceiverWitnessRegistry::recordCopyConstruction(const void* identity)
{ return record(FlowReceiverConstructionSource::Copy, identity); }

void FlowReceiverWitnessRegistry::bindModelContext(const std::shared_ptr<FlowReceiverWitness>& supplied,
    const Project& project, const SubProject& sub)
{
    requireScope();
    auto& entry = member(supplied);
    if (!entry.alive_ || sub.project != &project ||
        (entry.project_ && (entry.project_ != &project || entry.sub_ != &sub || entry.model_ != &sub.flow)))
        throw Error(M2M_E_UNSUPPORTED, "Flow receiver actual model context is dead, foreign or replaced");
    entry.project_ = &project;
    entry.sub_ = &sub;
    entry.model_ = &sub.flow;
}

void FlowReceiverWitnessRegistry::recordReceiverDeath(const std::shared_ptr<FlowReceiverWitness>& supplied)
{
    auto& entry = member(supplied);
    entry.alive_ = false;
    entry.project_ = nullptr; entry.sub_ = nullptr; entry.model_ = nullptr;
}

void FlowReceiverWitnessRegistry::releaseAdapterScope() noexcept
{
    if (released_) return;
    released_ = true;
    for (auto& entry : witnesses_) {
        entry->alive_ = false;
        entry->project_ = nullptr; entry->sub_ = nullptr; entry->model_ = nullptr;
    }
    // Retain ordered diagnostics. This is adapter cleanup, not native Close.
}

} // namespace m2m::doe
