#include "flow_receiver_population_prefix.hpp"

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"

namespace m2m::doe {
namespace {
template<class T>
bool sameOwner(const std::shared_ptr<T>& first, const std::shared_ptr<T>& second)
{
    return first && second && first.get() == second.get() &&
        !first.owner_before(second) && !second.owner_before(first);
}
} // namespace

FlowReceiverPopulationPrefix::FlowReceiverPopulationPrefix(const Project* project,
    const std::shared_ptr<FlowReceiverWitnessRegistry>& registry,
    const std::shared_ptr<SessionNotificationState>& owner)
    : project_(project), registry_(registry), owner_(owner) {}

void FlowReceiverPopulationPrefix::validateAgainst(engine::ConversionSession& session) const
{
    const auto* project = session.liveProject();
    const auto registry = registry_.lock();
    const auto owner = owner_.lock();
    if (!project || project != project_ ||
        !sameOwner(registry, session.flowReceiverWitnessRegistryOwner()) ||
        !sameOwner(owner, session.sessionNotificationStateOwner()) ||
        !sameOwner(registry, project->flowReceiverWitnessRegistry()) ||
        !sameOwner(owner, registry->owner()) ||
        (project->doeIo && project->doeIo->observedSessionNotificationState() != owner.get()))
        throw Error(M2M_E_UNSUPPORTED,
            "Flow receiver population prefix needs its live Session/Project/registry/notifier owners");
}

FlowReceiverPopulationPrefix projectFlowReceiverPopulationPrefix(engine::ConversionSession& session)
{
    const auto registry = session.flowReceiverWitnessRegistryOwner();
    FlowReceiverPopulationPrefix result(session.liveProject(), registry,
        session.sessionNotificationStateOwner());
    result.validateAgainst(session);
    const auto& history = registry->orderedWitnesses();
    result.entries_.reserve(history.size());
    for (std::size_t i = 0; i < history.size(); ++i) {
        const auto& witness = history[i];
        if (!witness || witness->ordinal() != i || !witness->identityToken())
            throw Error(M2M_E_UNSUPPORTED,
                "Flow receiver population prefix constructor history is unavailable");
        result.entries_.push_back({witness->ordinal(), witness->source(),
            witness->registrationSite(), witness->identityToken(),
            {witness, "slotSetOprHasDoe"}});
    }
    // No SetOprHasDoeRegistrationList is created: direct constructor evidence
    // does not qualify complete native population or current backend state.
    return result;
}

} // namespace m2m::doe
