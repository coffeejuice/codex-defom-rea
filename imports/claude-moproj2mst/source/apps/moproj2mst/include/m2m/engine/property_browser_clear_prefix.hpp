#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "m2m/engine/property_refresh_prefix.hpp"

namespace m2m::engine {

struct PropertyBrowserClearOwner {
    PropertyRefreshSlotOwner slot;
    PropertyRefreshObjectIdentity browserPrivate;
    bool operator==(const PropertyBrowserClearOwner&) const = default;
};

// One declared browser-clear admission scope. Its parent is the still-live
// CP136 clear request, not a new Qt delivery or a completed clear callback.
class PropertyBrowserClearState final {
public:
    static std::shared_ptr<PropertyBrowserClearState> createQualified(
        const PropertyRefreshBrowserClearRequest&, PropertyRefreshObjectIdentity browserPrivate,
        const std::string& qualification);
    PropertyBrowserClearState(const PropertyBrowserClearState&) = delete;
    PropertyBrowserClearState& operator=(const PropertyBrowserClearState&) = delete;
    const PropertyBrowserClearOwner& owner() const noexcept { return owner_; }
    void releaseAdmissionScope() noexcept { admitted_ = false; }
private:
    friend class PropertyBrowserClearPrefix;
    PropertyBrowserClearState(PropertyRefreshBrowserClearRequest, PropertyRefreshObjectIdentity);
    PropertyRefreshBrowserClearRequest parent_;
    PropertyBrowserClearOwner owner_;
    bool admitted_ = true, claimed_ = false;
    std::weak_ptr<const void> traversal_;
};

struct PropertyBrowserSnapshotObservation {
    PropertyBrowserClearOwner owner;
    // Observation of the two list-copy return boundaries, not a source request.
    bool copiesReturned = false;
    // Unknown and an observed empty snapshot are distinct. Raw pointer values,
    // including null/duplicates, are retained without dereferencing properties.
    std::optional<std::vector<std::uint64_t>> entries;
    std::string qualification;
};

struct PropertyBrowserRemovalRequest {
    PropertyBrowserClearOwner owner;
    std::uint64_t parentDeliveryOrdinal = 0;
    std::size_t snapshotIndex = 0;
    std::uint64_t property = 0;
    std::weak_ptr<const void> invocation;
};
enum class PropertyBrowserRemovalReturn { Unavailable, Returned, Interrupted };
struct PropertyBrowserRemovalObservation {
    PropertyBrowserRemovalRequest request;
    PropertyBrowserRemovalReturn result = PropertyBrowserRemovalReturn::Unavailable;
    std::string qualification;
};

enum class PropertyBrowserClearStage { SnapshotPending, RemovalPending, RemovalInterrupted, CleanupPending };
struct PropertyBrowserClearReport {
    PropertyBrowserClearStage stage = PropertyBrowserClearStage::SnapshotPending;
    bool snapshotRetained = false;
    std::size_t snapshotSize = 0, normalRemovalReturns = 0;
    std::optional<PropertyBrowserRemovalRequest> removal;
    std::string issue;
};

// Native1413dd960's reverse snapshot cursor only. No list/Qt mutation or browser
// callback runs here. Normal removal return does not mean a property was found,
// removed or destroyed. CleanupPending never synthesizes CP136 clear completion.
class PropertyBrowserClearPrefix final {
public:
    PropertyBrowserClearPrefix() = default;
    PropertyBrowserClearPrefix(const PropertyBrowserClearPrefix&) = delete;
    PropertyBrowserClearPrefix& operator=(const PropertyBrowserClearPrefix&) = delete;
    const PropertyBrowserClearReport& resume(const std::shared_ptr<PropertyBrowserClearState>&,
        const PropertyBrowserSnapshotObservation&, const PropertyBrowserRemovalObservation& = {});
    const PropertyBrowserClearReport& report() const noexcept { return report_; }
private:
    void requestNext(const PropertyBrowserClearState&);
    bool bound_ = false;
    std::weak_ptr<PropertyBrowserClearState> state_;
    std::shared_ptr<const void> traversal_ = std::make_shared<const char>(0);
    std::shared_ptr<const void> removal_;
    std::vector<std::uint64_t> snapshot_;
    PropertyBrowserClearReport report_;
};

} // namespace m2m::engine
