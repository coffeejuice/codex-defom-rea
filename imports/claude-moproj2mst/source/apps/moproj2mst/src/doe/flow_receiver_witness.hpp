#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "variable_notify.hpp"

namespace m2m::doe {

enum class FlowReceiverConstructionSource { NormalSub, Copy };
class FlowReceiverWitnessRegistry;

// Isolated lifetime witness supplied by a future actual constructor binding.
// Identity tokens are caller-qualified; they do not construct native QObjects.
class FlowReceiverWitness final : public SetOprHasDoeReceiver {
public:
    FlowReceiverConstructionSource source() const noexcept { return source_; }
    const void* identityToken() const noexcept { return identity_; }
    std::size_t ordinal() const noexcept { return ordinal_; }
    std::uint64_t registrationSite() const noexcept;
    bool nativeReceiverAlive() const noexcept override { return alive_; }
    const Project* actualProject() const noexcept override { return alive_ ? project_ : nullptr; }
    const SubProject* actualSubProject() const noexcept override { return alive_ ? sub_ : nullptr; }
    const FlowChart* actualFlowModel() const noexcept override { return alive_ ? model_ : nullptr; }
    std::optional<const void*> backendWitness() const noexcept override { return std::nullopt; }
private:
    friend class FlowReceiverWitnessRegistry;
    FlowReceiverWitness(FlowReceiverConstructionSource source, const void* identity, std::size_t ordinal);
    FlowReceiverConstructionSource source_;
    const void* identity_;
    std::size_t ordinal_;
    bool alive_ = true;
    const Project* project_ = nullptr;
    const SubProject* sub_ = nullptr;
    const FlowChart* model_ = nullptr;
};

// Records explicit constructor events, never a post-hoc Project.subs snapshot.
// Does not install a qualified population in SetOprHasDoeBindings: actual native
// constructor/population/backend binding remains required before production use.
class FlowReceiverWitnessRegistry final {
public:
    FlowReceiverWitnessRegistry(const std::shared_ptr<SessionNotificationState>& actualOwner,
                                std::string qualification);
    ~FlowReceiverWitnessRegistry();
    FlowReceiverWitnessRegistry(const FlowReceiverWitnessRegistry&) = delete;
    FlowReceiverWitnessRegistry& operator=(const FlowReceiverWitnessRegistry&) = delete;
    std::shared_ptr<FlowReceiverWitness> recordNormalSubConstruction(const void* identityToken);
    std::shared_ptr<FlowReceiverWitness> recordCopyConstruction(const void* identityToken);
    // SUB's Project backlink follows native flow-model construction. Bind it
    // explicitly later; membership/liveness is rechecked by the dispatcher.
    void bindModelContext(const std::shared_ptr<FlowReceiverWitness>&, const Project&, const SubProject&);
    void recordReceiverDeath(const std::shared_ptr<FlowReceiverWitness>&);
    void releaseAdapterScope() noexcept;
    std::shared_ptr<SessionNotificationState> owner() const noexcept { return owner_.lock(); }
    const std::vector<std::shared_ptr<FlowReceiverWitness>>& orderedWitnesses() const noexcept
    { return witnesses_; }
private:
    void requireScope() const;
    FlowReceiverWitness& member(const std::shared_ptr<FlowReceiverWitness>&) const;
    std::shared_ptr<FlowReceiverWitness> record(FlowReceiverConstructionSource, const void*);
    std::weak_ptr<SessionNotificationState> owner_;
    std::string qualification_;
    bool released_ = false;
    std::vector<std::shared_ptr<FlowReceiverWitness>> witnesses_;
};

} // namespace m2m::doe
