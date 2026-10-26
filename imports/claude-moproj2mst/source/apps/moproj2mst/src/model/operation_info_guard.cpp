// OPI identity/guard metadata only; no native payload or signal delivery.
#include "m2m/engine/operation_info_guard.hpp"

#include "m2m/common.hpp"

namespace m2m::engine {
namespace detail {
struct OperationInfoGuardState {
    explicit OperationInfoGuardState(Operation& operation) : value(&operation) {}
    Operation* value;
    OperationInfoExpiry expiry = OperationInfoExpiry::Live;
    std::string qualification;
};
}
namespace {
const std::string emptyQualification;
}

OperationInfoGuard::OperationInfoGuard(const OperationInfoIdentity& identity)
    : state_(identity.state_) {}

void OperationInfoGuard::reset(const OperationInfoIdentity* identity) noexcept
{
    // Same-identity binding is unchanged, as cbc920 skips an equal pointer.
    const auto replacement = identity ? identity->state_ : nullptr;
    if (state_ != replacement) state_ = replacement;
}

Operation* OperationInfoGuard::get() const noexcept
{
    return state_ && state_->expiry == OperationInfoExpiry::Live ? state_->value : nullptr;
}

OperationInfoExpiry OperationInfoGuard::expiry() const noexcept
{
    return state_ ? state_->expiry : OperationInfoExpiry::Unbound;
}

const std::string& OperationInfoGuard::boundaryQualification() const noexcept
{
    return state_ ? state_->qualification : emptyQualification;
}

OperationInfoIdentity::OperationInfoIdentity(Operation& operation)
    : state_(std::make_shared<detail::OperationInfoGuardState>(operation)) {}

OperationInfoIdentity::~OperationInfoIdentity()
{
    if (state_->expiry == OperationInfoExpiry::Live) {
        state_->value = nullptr;
        state_->expiry = OperationInfoExpiry::AdapterIdentityLifetime;
    }
    // Preserve a committed QObject-boundary receipt for diagnostic guards.
}

OperationInfoGuard OperationInfoIdentity::guard() const { return OperationInfoGuard(*this); }

Operation* OperationInfoIdentity::get() const noexcept
{
    return state_->expiry == OperationInfoExpiry::Live ? state_->value : nullptr;
}

OperationInfoExpiry OperationInfoIdentity::expiry() const noexcept { return state_->expiry; }

bool OperationInfoIdentity::commitQObjectBoundary(const std::string& sourceQualification)
{
    if (sourceQualification.find_first_not_of(" \t\r\n") == std::string::npos)
        throw Error(M2M_E_UNSUPPORTED, "OperationInfo QObject boundary requires a source qualification");
    if (state_->expiry != OperationInfoExpiry::Live) return false;
    // Copy may allocate: perform it before publishing the terminal transition.
    state_->qualification = sourceQualification;
    state_->value = nullptr;
    state_->expiry = OperationInfoExpiry::QObjectBoundary;
    return true;
}
} // namespace m2m::engine
