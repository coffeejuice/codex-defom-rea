// Mo-owned manager storage. Native Close effects remain a separate guarded driver.
#pragma once

#include <map>
#include <memory>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m {
struct Project;
struct Operation;
struct ItemManager;
namespace mst { class OpClass; }

namespace engine {
struct DbHandle;

class OperationClassRegistry final {
public:
    using Entries = std::map<qt::String, std::unique_ptr<mst::OpClass>>;
    OperationClassRegistry();
    ~OperationClassRegistry();
    OperationClassRegistry(const OperationClassRegistry&) = delete;
    OperationClassRegistry& operator=(const OperationClassRegistry&) = delete;
    Entries& entries() noexcept;
    const Entries& entries() const noexcept;
private:
    Entries entries_;
};

// Adapter Project storage marker. Its weak lifetime expires only after all
// later-declared Project members, not at entry to the Project destructor body.
// It does not represent native QObject construction/destruction.
struct ProjectStorageLifetime final {};

class MoModelSlots final {
public:
    using OperationRows = std::vector<std::unique_ptr<Operation>>;
    enum class Slot { Operations, Objects, Materials, Equipment, Lubricants, Done };

    MoModelSlots(); // storage defaults; not a native lazy-constructor receipt
    ~MoModelSlots();
    MoModelSlots(const MoModelSlots&) = delete;
    MoModelSlots& operator=(const MoModelSlots&) = delete;
    MoModelSlots(MoModelSlots&&) = delete;
    MoModelSlots& operator=(MoModelSlots&&) = delete;

    OperationRows& operations();
    const OperationRows& operations() const;
    ItemManager& objects();
    const ItemManager& objects() const;
    ItemManager& materials();
    const ItemManager& materials() const;
    ItemManager& equipment();
    const ItemManager& equipment() const;
    ItemManager& lubricants();
    const ItemManager& lubricants() const;

    Slot nextStorageBoundary() const noexcept { return next_; }
    bool projectStorageAlive() const noexcept { return !projectStorage_.expired(); }
    const Project* projectIdentity() const noexcept { return projectIdentity_; }
    const DbHandle* modelIdentity() const noexcept { return modelIdentity_; }

    // Storage-only continuation. The caller must separately complete/qualify
    // native effects before calling. This API does not decide or execute Close.
    // Rejects out-of-order/repeated release and any still-live Project borrow.
    void releaseCompletedSlotStorage(Slot);

private:
    friend struct m2m::Project;
    friend struct DbHandle;
    void bindProjectStorage(const Project&, const std::shared_ptr<const ProjectStorageLifetime>&,
                            const std::shared_ptr<OperationClassRegistry>&);
    void bindModelIdentity(const DbHandle&);
    void releaseAdapterStorage() noexcept;

    // Keep the exact registry objects available through late operation release.
    // This is a lease, not a claim that the registry is a native Mo child.
    std::shared_ptr<OperationClassRegistry> registryLease_;
    std::unique_ptr<OperationRows> operations_;
    std::unique_ptr<ItemManager> objects_, materials_, equipment_, lubricants_;
    std::weak_ptr<const ProjectStorageLifetime> projectStorage_;
    const Project* projectIdentity_ = nullptr; // identity only; never dereferenced after expiry
    const DbHandle* modelIdentity_ = nullptr; // same native model identity, never a second child
    Slot next_ = Slot::Operations;
};
} // namespace engine
} // namespace m2m
