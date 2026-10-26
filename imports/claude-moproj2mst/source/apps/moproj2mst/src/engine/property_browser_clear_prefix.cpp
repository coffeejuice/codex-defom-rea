#include "m2m/engine/property_browser_clear_prefix.hpp"
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

PropertyBrowserClearState::PropertyBrowserClearState(
    PropertyRefreshBrowserClearRequest parent, PropertyRefreshObjectIdentity browserPrivate)
    : parent_(std::move(parent)), owner_{parent_.owner, browserPrivate} {}

std::shared_ptr<PropertyBrowserClearState> PropertyBrowserClearState::createQualified(
    const PropertyRefreshBrowserClearRequest& parent, PropertyRefreshObjectIdentity browserPrivate,
    const std::string& qualification)
{
    require(valid(parent.owner) && valid(browserPrivate) && parent.deliveryOrdinal &&
            !parent.invocation.expired() && !qualification.empty(),
        "Browser-clear owner/parent request is unqualified or expired");
    return std::shared_ptr<PropertyBrowserClearState>(new PropertyBrowserClearState(parent, browserPrivate));
}

void PropertyBrowserClearPrefix::requestNext(const PropertyBrowserClearState& state)
{
    if (report_.normalRemovalReturns == snapshot_.size()) {
        report_.removal.reset();
        removal_.reset();
        report_.stage = PropertyBrowserClearStage::CleanupPending;
        report_.issue = "Temporary cleanups1413dd9d5/1413dd9e0 and normal clear return remain guarded";
        return;
    }
    // @src 0x1413dd997 sets end; 0x1413df312 predecrements before dereference.
    const auto index = snapshot_.size() - 1 - report_.normalRemovalReturns;
    removal_ = std::make_shared<const char>(0);
    report_.removal = PropertyBrowserRemovalRequest{
        state.owner_, state.parent_.deliveryOrdinal, index, snapshot_[index], removal_};
    report_.stage = PropertyBrowserClearStage::RemovalPending;
    report_.issue = "Actual removal1413ddbc0 must return before advancing the snapshot cursor";
}

const PropertyBrowserClearReport& PropertyBrowserClearPrefix::resume(
    const std::shared_ptr<PropertyBrowserClearState>& state,
    const PropertyBrowserSnapshotObservation& snapshot, const PropertyBrowserRemovalObservation& removal)
{
    auto pending = [&](const char* issue) -> const PropertyBrowserClearReport& {
        report_.issue = issue;
        return report_;
    };
    if (!state) {
        require(!bound_, "Browser traversal lost its bound owner");
        return pending("Actual browser-clear owner is unavailable");
    }
    require(state->admitted_ && !state->parent_.invocation.expired(),
        "Browser-clear owner or parent request admission expired");
    if (!bound_) {
        require(!state->claimed_, "Browser-clear admission already belongs to another traversal");
        state_ = state;
        state->claimed_ = true;
        state->traversal_ = traversal_;
        bound_ = true;
    } else {
        const auto retained = state_.lock();
        require(retained == state && !retained.owner_before(state) && !state.owner_before(retained) &&
                sameToken(state->traversal_.lock(), traversal_),
            "Browser traversal cannot rebind its owner/admission scope");
    }

    if (snapshot.copiesReturned) {
        require(snapshot.owner == state->owner_ && !snapshot.qualification.empty(),
            "Browser snapshot belongs to another owner or is unqualified");
        if (snapshot.entries && report_.snapshotRetained)
            require(*snapshot.entries == snapshot_, "Browser traversal cannot replace its retained snapshot");
    }
    if (!report_.snapshotRetained) {
        require(removal.result == PropertyBrowserRemovalReturn::Unavailable,
            "Removal return precedes the admitted snapshot/request");
        if (!snapshot.copiesReturned || !snapshot.entries)
            return pending("Actual returned browser snapshot is unknown");
        snapshot_ = *snapshot.entries;
        report_.snapshotRetained = true;
        report_.snapshotSize = snapshot_.size();
        requestNext(*state);
        return report_;
    }

    if (removal.result == PropertyBrowserRemovalReturn::Unavailable) return report_;
    require(report_.removal.has_value(), "No pending removal accepts this return observation");
    const auto& request = *report_.removal;
    require(!removal.qualification.empty() && removal.request.owner == request.owner &&
            removal.request.parentDeliveryOrdinal == request.parentDeliveryOrdinal &&
            removal.request.snapshotIndex == request.snapshotIndex && removal.request.property == request.property &&
            sameToken(removal.request.invocation.lock(), removal_),
        "Removal return belongs to another owner/snapshot entry/invocation or is unqualified");
    if (report_.stage == PropertyBrowserClearStage::RemovalInterrupted) {
        require(removal.result == PropertyBrowserRemovalReturn::Interrupted,
            "Interrupted browser removal cannot later return normally in this frame");
        return report_;
    }
    if (removal.result == PropertyBrowserRemovalReturn::Interrupted) {
        report_.stage = PropertyBrowserClearStage::RemovalInterrupted;
        return pending("Interrupted removal has no admitted cursor-advance/cleanup continuation");
    }
    ++report_.normalRemovalReturns;
    requestNext(*state);
    return report_;
}

} // namespace m2m::engine
