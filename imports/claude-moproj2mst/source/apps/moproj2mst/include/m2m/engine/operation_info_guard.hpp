// Isolated OPI/QObject guard identity. No OpClass or accepted-Close integration.
#pragma once

#include <memory>
#include <string>

namespace m2m {
struct Operation;
namespace engine {
namespace detail { struct OperationInfoGuardState; }
class OperationInfoIdentity;

enum class OperationInfoExpiry {
    Unbound,
    Live,
    QObjectBoundary,
    AdapterIdentityLifetime
};

// Shares only liveness metadata, never Operation ownership. Guard copies remain
// separate bindings: rebinding/resetting one does not alter any other guard.
// Binding/expiry use the caller's serialized session; no concurrent-Qt claim.
class OperationInfoGuard final {
public:
    OperationInfoGuard() = default;
    explicit OperationInfoGuard(const OperationInfoIdentity&);
    void reset(const OperationInfoIdentity* = nullptr) noexcept;
    Operation* get() const noexcept;
    explicit operator bool() const noexcept { return get() != nullptr; }
    OperationInfoExpiry expiry() const noexcept;
    const std::string& boundaryQualification() const noexcept;
private:
    std::shared_ptr<detail::OperationInfoGuardState> state_;
};

// One identity must be owned by the actual OPI lifecycle, independently of its
// outer SfMoOprItem (both are currently folded into Operation). The address is
// borrowed; this helper cannot infer that an arbitrary Operation was deleted.
// Root must bind the identity lifetime to the actual owner before production use.
class OperationInfoIdentity final {
public:
    explicit OperationInfoIdentity(Operation&);
    ~OperationInfoIdentity();
    OperationInfoIdentity(const OperationInfoIdentity&) = delete;
    OperationInfoIdentity& operator=(const OperationInfoIdentity&) = delete;
    OperationInfoIdentity(OperationInfoIdentity&&) = delete;
    OperationInfoIdentity& operator=(OperationInfoIdentity&&) = delete;

    OperationInfoGuard guard() const;
    Operation* get() const noexcept;
    OperationInfoExpiry expiry() const noexcept;

    // 050531 -> QObject::~QObject ->6712f8bd/clearGuards. Caller must already
    // have completed qualified OPI payload/member cleanup. This does not run it,
    // deliver destroyed signals, or certify the supplied evidence string.
    // Returns true once; repeated commits retain the first receipt and return
    // false. A missing qualification refuses before mutation.
    bool commitQObjectBoundary(const std::string& sourceQualification);
private:
    friend class OperationInfoGuard;
    std::shared_ptr<detail::OperationInfoGuardState> state_;
};
} // namespace engine
} // namespace m2m
