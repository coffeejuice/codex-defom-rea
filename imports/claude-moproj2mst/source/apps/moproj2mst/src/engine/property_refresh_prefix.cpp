#include "m2m/engine/property_refresh_prefix.hpp"
#include "m2m/common.hpp"
#include <utility>

namespace m2m::engine {
namespace {
bool valid(const PropertyRefreshObjectIdentity& o) { return o.address && o.lifetime; }
bool valid(const PropertyRefreshSlotOwner& o)
{
    return valid(o.receiver) && valid(o.interfaceObject) && valid(o.implementation) && valid(o.browser);
}
void require(bool condition, const char* issue)
{
    if (!condition) throw Error(M2M_E_UNSUPPORTED, issue);
}
bool sameToken(const std::shared_ptr<const void>& a, const std::shared_ptr<const void>& b)
{
    return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a);
}
} // namespace

PropertyRefreshEmissionRequest::PropertyRefreshEmissionRequest(
    PropertyRefreshObjectIdentity notifier, std::uint64_t occurrence, std::uint64_t payload)
    : notifier_(notifier), occurrence_(occurrence), payload_(payload) {}

PropertyRefreshNotifierReport planPropertyRefreshNotifier(const PropertyRefreshNotifierObservation& o)
{
    PropertyRefreshNotifierReport r;
    if (!valid(o.notifier) || !o.occurrence || o.qualification.empty()) {
        r.issue = "Actual notifier owner/occurrence qualification is unavailable";
        return r;
    }
    if (!o.suppression) {
        r.issue = "Notifier+141 suppression read is unknown";
        return r;
    }
    if (*o.suppression) {
        r.stage = PropertyRefreshNotifierStage::Suppressed;
        r.completed = true;
        return r;
    }
    if (!o.payload) {
        r.issue = "Nullable property payload is unknown";
        return r;
    }
    r.emission = PropertyRefreshEmissionRequest(o.notifier, o.occurrence, *o.payload);
    r.stage = PropertyRefreshNotifierStage::EmissionPending;
    r.issue = "Local3 emission request requires actual Qt activation/delivery";
    return r;
}

PropertyRefreshReceiverState::PropertyRefreshReceiverState(
    PropertyRefreshSlotOwner owner, std::optional<std::uint64_t> properties)
    : owner_(owner), properties_(properties) {}

std::shared_ptr<PropertyRefreshReceiverState> PropertyRefreshReceiverState::createQualified(
    const PropertyRefreshSlotOwner& owner, std::optional<std::uint64_t> properties,
    const std::string& qualification)
{
    require(valid(owner) && !qualification.empty(), "Property receiver mapping is unqualified");
    return std::shared_ptr<PropertyRefreshReceiverState>(new PropertyRefreshReceiverState(owner, properties));
}

void PropertyRefreshReceiverState::observeProperties(std::uint64_t value, const std::string& qualification)
{
    require(admitted_ && !qualification.empty(), "Property receiver field observation is unqualified/expired");
    properties_ = value;
}

const PropertyRefreshSlotReport& PropertyRefreshSlotPrefix::resume(
    const std::shared_ptr<PropertyRefreshReceiverState>& state,
    const PropertyRefreshEmissionRequest& emission, const PropertyRefreshSlotObservation& delivery,
    const PropertyRefreshClearObservation& clear)
{
    auto pending = [&](const char* issue) -> const PropertyRefreshSlotReport& {
        report_.issue = issue;
        return report_;
    };
    if (!state) {
        require(!bound_, "Property slot frame lost its bound receiver state");
        return pending("Actual property receiver state is unavailable");
    }
    require(state->admitted_, "Property receiver admission scope expired");
    if (!bound_) {
        state_ = state;
        emissionIdentity_ = emission.identity_;
        bound_ = true;
    } else {
        const auto retained = state_.lock();
        require(retained == state && !retained.owner_before(state) && !state.owner_before(retained) &&
                sameToken(emissionIdentity_, emission.identity_),
            "Property slot frame cannot rebind its owner or emission");
    }

    if (delivery.slotEntryObserved) {
        require(!delivery.qualification.empty() && delivery.owner == state->owner_ &&
                delivery.notifier == emission.notifier_ &&
                delivery.emissionOccurrence == emission.occurrence_ && delivery.deliveryOrdinal,
            "Property slot entry belongs to another owner/emission or is unqualified");
        if (!delivery.payload && !deliveryOrdinal_) return pending("Actual delivered payload is unknown");
        if (delivery.payload)
            require(*delivery.payload == emission.payload_, "Property slot delivered a different payload");
        if (deliveryOrdinal_)
            require(deliveryOrdinal_ == delivery.deliveryOrdinal, "Property slot entry ordinal changed");
    }

    if (report_.stage == PropertyRefreshSlotStage::Admission) {
        require(clear.result == PropertyRefreshClearReturn::Unavailable,
            "Browser-clear completion precedes this frame's request");
        if (!delivery.slotEntryObserved) return pending("Emission request does not admit slot delivery");
        require(state->deliveryOrdinals_.insert(delivery.deliveryOrdinal).second,
            "Property slot entry ordinal was already admitted by another frame");
        deliveryOrdinal_ = delivery.deliveryOrdinal;
        report_.browserClear = PropertyRefreshBrowserClearRequest{state->owner_, deliveryOrdinal_, invocation_};
        report_.stage = PropertyRefreshSlotStage::BrowserClearPending;
        return pending("Browser clear1413dd960 must return before the payload store");
    }

    if (clear.result != PropertyRefreshClearReturn::Unavailable) {
        require(!clear.qualification.empty() && clear.request.owner == state->owner_ &&
                clear.request.deliveryOrdinal == deliveryOrdinal_ &&
                sameToken(clear.request.invocation.lock(), invocation_),
            "Browser-clear completion belongs to another owner/invocation or is unqualified");
        require(clearReturn_ == PropertyRefreshClearReturn::Unavailable || clearReturn_ == clear.result,
            "Browser-clear return observation contradicts the retained frame");
    }
    if (report_.stage != PropertyRefreshSlotStage::BrowserClearPending) return report_;
    if (clear.result == PropertyRefreshClearReturn::Unavailable)
        return pending("Browser-clear normal return remains unobserved");
    clearReturn_ = clear.result;
    if (clear.result == PropertyRefreshClearReturn::Interrupted) {
        report_.stage = PropertyRefreshSlotStage::BrowserClearInterrupted;
        return pending("Interrupted browser clear has no admitted payload-store continuation");
    }

    // @src 0x14137598c. No callback or revision check is inserted here: an
    // external/reentrant observation may have changed the cell during clear.
    state->properties_ = emission.payload_;
    ++state->stores_;
    report_.payloadStored = true;
    if (!emission.payload_) {
        report_.stage = PropertyRefreshSlotStage::Complete;
        report_.completed = true;
        report_.issue.clear();
    } else {
        report_.stage = PropertyRefreshSlotStage::PropertyListPending;
        report_.issue = "Property enumeration1413759a4/refresh1413759dc remains guarded";
    }
    return report_;
}

} // namespace m2m::engine
