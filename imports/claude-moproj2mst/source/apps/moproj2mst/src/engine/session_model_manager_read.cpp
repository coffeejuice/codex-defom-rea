#include "m2m/engine/session_model_manager_read.hpp"

#include "m2m/common.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Session manager Open read: ") + detail);
}
bool sameManager(const std::shared_ptr<ModelManager>& left, const std::shared_ptr<ModelManager>& right)
{
    return left && right && left.get() == right.get() &&
        !left.owner_before(right) && !right.owner_before(left);
}
}

SessionModelManagerReadReceipt::SessionModelManagerReadReceipt(const ConversionSession& session,
    const std::shared_ptr<ModelManager>& manager)
    : session_(&session), manager_(manager),
      managerIdentity_(reinterpret_cast<std::uintptr_t>(manager.get())),
      modelsAtRead_(0), connectionsAtRead_(0)
{
    if (!sameManager(manager, session.modelManagerOwner()) || manager->ownerSession() != &session)
        unavailable("actual candidate field/backlink identity is unavailable");
    modelsAtRead_ = manager->size();
    connectionsAtRead_ = manager->connections().size();
}

std::shared_ptr<ModelManager> SessionModelManagerReadReceipt::requireManagerFor(
    const ConversionSession& session) const
{
    // Revocation precedes any access through the historical Session identity.
    if (!admitted_) unavailable("adapter Session observation scope was revoked");
    const auto actual = session.modelManagerOwner();
    const auto recorded = manager_.lock();
    if (session_ != &session || !sameManager(actual, recorded) ||
        reinterpret_cast<std::uintptr_t>(actual.get()) != managerIdentity_ || actual->ownerSession() != &session)
        unavailable("receipt does not bind this actual Session manager identity");
    return actual;
}
} // namespace m2m::engine
