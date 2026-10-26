#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>

namespace m2m::engine {

// Declared observation identities, not allocated Qt objects or proof of native
// liveness. Address reuse requires a different lifetime in the supplying trace.
struct PropertyRefreshObjectIdentity {
    std::uint64_t address = 0, lifetime = 0;
    bool operator==(const PropertyRefreshObjectIdentity&) const = default;
};

struct PropertyRefreshNotifierObservation {
    PropertyRefreshObjectIdentity notifier;
    std::uint64_t occurrence = 0;
    std::optional<std::uint8_t> suppression;
    // Disengaged is unknown; engaged zero is an observed null payload.
    std::optional<std::uint64_t> payload;
    std::string qualification;
};

struct PropertyRefreshNotifierReport;
class PropertyRefreshEmissionRequest final {
public:
    PropertyRefreshEmissionRequest(const PropertyRefreshEmissionRequest&) = default;
    PropertyRefreshEmissionRequest& operator=(const PropertyRefreshEmissionRequest&) = default;
    const PropertyRefreshObjectIdentity& notifier() const noexcept { return notifier_; }
    std::uint64_t occurrence() const noexcept { return occurrence_; }
    std::uint64_t payload() const noexcept { return payload_; }
    static constexpr unsigned signalLocalIndex = 3;
private:
    friend PropertyRefreshNotifierReport planPropertyRefreshNotifier(
        const PropertyRefreshNotifierObservation&);
    friend class PropertyRefreshSlotPrefix;
    PropertyRefreshEmissionRequest(PropertyRefreshObjectIdentity, std::uint64_t, std::uint64_t);
    PropertyRefreshObjectIdentity notifier_;
    std::uint64_t occurrence_, payload_;
    std::shared_ptr<const void> identity_ = std::make_shared<const char>(0);
};

enum class PropertyRefreshNotifierStage { Admission, Suppressed, EmissionPending };
struct PropertyRefreshNotifierReport {
    PropertyRefreshNotifierStage stage = PropertyRefreshNotifierStage::Admission;
    bool completed = false;
    std::optional<PropertyRefreshEmissionRequest> emission;
    std::string issue;
};

// Pure projection of 1402ce9c0: no Qt activation, connection or delivery occurs.
// A returned local3 request is not a slot-entry observation. Raw connection3
// is normalized to0 by this Qt4 QObject::connect, not proof of synchronous use.
PropertyRefreshNotifierReport planPropertyRefreshNotifier(const PropertyRefreshNotifierObservation&);

struct PropertyRefreshSlotOwner {
    PropertyRefreshObjectIdentity receiver, interfaceObject, implementation, browser;
    bool operator==(const PropertyRefreshSlotOwner&) const = default;
};

class PropertyRefreshReceiverState final {
public:
    static std::shared_ptr<PropertyRefreshReceiverState> createQualified(
        const PropertyRefreshSlotOwner&, std::optional<std::uint64_t> properties,
        const std::string& qualification);
    PropertyRefreshReceiverState(const PropertyRefreshReceiverState&) = delete;
    PropertyRefreshReceiverState& operator=(const PropertyRefreshReceiverState&) = delete;
    const PropertyRefreshSlotOwner& owner() const noexcept { return owner_; }
    std::optional<std::uint64_t> properties() const noexcept { return properties_; }
    std::uint64_t prefixStoreCount() const noexcept { return stores_; }
    // Allows an observed external/reentrant field change; it does not execute
    // that callback or count it as this prefix's payload store.
    void observeProperties(std::uint64_t, const std::string& qualification);
    void releaseAdmissionScope() noexcept { admitted_ = false; }
private:
    friend class PropertyRefreshSlotPrefix;
    PropertyRefreshReceiverState(PropertyRefreshSlotOwner, std::optional<std::uint64_t>);
    PropertyRefreshSlotOwner owner_;
    std::optional<std::uint64_t> properties_;
    std::uint64_t stores_ = 0;
    bool admitted_ = true;
    std::set<std::uint64_t> deliveryOrdinals_;
};

struct PropertyRefreshSlotObservation {
    bool slotEntryObserved = false;
    PropertyRefreshSlotOwner owner;
    PropertyRefreshObjectIdentity notifier;
    std::uint64_t emissionOccurrence = 0;
    // Unique actual slot-entry ordinal within this receiver admission scope.
    // Multiple deliveries of one emission may have different ordinals.
    std::uint64_t deliveryOrdinal = 0;
    std::optional<std::uint64_t> payload;
    std::string qualification;
};

struct PropertyRefreshBrowserClearRequest {
    PropertyRefreshSlotOwner owner;
    std::uint64_t deliveryOrdinal = 0;
    std::weak_ptr<const void> invocation;
};
enum class PropertyRefreshClearReturn { Unavailable, Returned, Interrupted };
struct PropertyRefreshClearObservation {
    PropertyRefreshBrowserClearRequest request;
    PropertyRefreshClearReturn result = PropertyRefreshClearReturn::Unavailable;
    std::string qualification;
};

enum class PropertyRefreshSlotStage {
    Admission, BrowserClearPending, BrowserClearInterrupted, PropertyListPending, Complete
};
struct PropertyRefreshSlotReport {
    PropertyRefreshSlotStage stage = PropertyRefreshSlotStage::Admission;
    bool completed = false, payloadStored = false;
    std::optional<PropertyRefreshBrowserClearRequest> browserClear;
    std::string issue;
};

// One explicitly admitted slot invocation. It owns no Qt callback/Session and
// cannot infer Boolean E, OPI, Apply, timer readiness or first-Open delivery.
// 141375979 clear must return before 14137598c writes implementation+18.
// Nonnull payload stops before 1413759a4 property enumeration/refresh.
class PropertyRefreshSlotPrefix final {
public:
    PropertyRefreshSlotPrefix() = default;
    PropertyRefreshSlotPrefix(const PropertyRefreshSlotPrefix&) = delete;
    PropertyRefreshSlotPrefix& operator=(const PropertyRefreshSlotPrefix&) = delete;
    const PropertyRefreshSlotReport& resume(const std::shared_ptr<PropertyRefreshReceiverState>&,
        const PropertyRefreshEmissionRequest&, const PropertyRefreshSlotObservation&,
        const PropertyRefreshClearObservation& = {});
    const PropertyRefreshSlotReport& report() const noexcept { return report_; }
private:
    bool bound_ = false;
    std::weak_ptr<PropertyRefreshReceiverState> state_;
    std::shared_ptr<const void> emissionIdentity_;
    std::shared_ptr<const void> invocation_ = std::make_shared<const char>(0);
    std::uint64_t deliveryOrdinal_ = 0;
    PropertyRefreshClearReturn clearReturn_ = PropertyRefreshClearReturn::Unavailable;
    PropertyRefreshSlotReport report_;
};

} // namespace m2m::engine
