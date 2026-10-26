#include "m2m/engine/editor_grid.hpp"
#include <utility>

namespace m2m::engine::simcontrol300 {

namespace {
bool sameThreadOwner(const std::shared_ptr<WidgetThreadState>& left,
    const std::shared_ptr<WidgetThreadState>& right) noexcept
{
    // Equal raw addresses do not establish the lifetime owner. A foreign
    // control block must never replace QObject's actual retained threadData.
    return left && right && left.get() == right.get() &&
        !left.owner_before(right) && !right.owner_before(left);
}
}

EditorGridConstruction::EditorGridConstruction(const StagedEditor300* factory,
    const std::shared_ptr<WidgetState>& parent)
    : factoryIdentity_(factory), parent_(parent) {}

EditorGridConstruction::~EditorGridConstruction() { releaseNativeOwner(); }

void EditorGridConstruction::releaseNativeOwner() noexcept
{
    if (pending_) pending_->releaseNativeOwner();
    if (child_) child_->releaseNativeOwner();
    currentThread_.reset();
}

EditorGridReport EditorGridConstruction::resume(const EditorGridContext& context)
{
    if (context.receiver != factoryIdentity_)
        return {report_.stage, EditorGridIssue::IdentityMismatch};
    if (context.qualification.empty())
        return {report_.stage, EditorGridIssue::MissingQualification};
    auto parent = parent_.lock();
    if (!parent || !parent->nativeOwnerAlive() || parent->nativeIdentity() != factoryIdentity_)
        return {report_.stage, EditorGridIssue::WidgetOwnerUnavailable};
    const auto pending = [&](EditorGridIssue issue) {
        report_.issue = issue;
        return report_;
    };
    if (!report_.attempted) {
        qualification_ = context.qualification;
        // Successful original allocation/private-constructor profile only.
        // Null allocation followed by native dereference is not repaired here.
        pending_ = GridLayoutState::beginParentedConstruction(parent);
        effects_.push_back(EditorGridEffect::AllocationAndPrivateConstruction);
        // 6712e64b..661 installs QObject base identity and private backlinks
        // before selecting/refcounting the actual threadData owner.
        effects_.push_back(EditorGridEffect::QObjectBaseInstalled);
        report_.attempted = true;
        report_.stage = EditorGridStage::ThreadSelection;
    }
    if (report_.stage == EditorGridStage::ThreadSelection) {
        auto runtime = parent->runtimeOwner();
        if (!runtime) return pending(EditorGridIssue::RuntimeOwnerUnavailable);
        if (!context.currentThread) return pending(EditorGridIssue::CurrentThreadUnavailable);
        auto parentThread = parent->threadState();
        if (!parentThread) return pending(EditorGridIssue::ParentThreadUnavailable);
        // 6712e66a..67e selects either the parent's threadData (when its QThread
        // is null) or current threadData. Equal actual owners close both arms.
        // The unequal-owner selection/warning path remains explicitly guarded.
        if (!sameThreadOwner(context.currentThread, parentThread) ||
            !sameThreadOwner(context.currentThread, runtime->threadState()))
            return pending(EditorGridIssue::ParentThreadMismatch);
        pending_->thread_ = context.currentThread; // native ref at6712e685
        currentThread_ = context.currentThread;
        effects_.push_back(EditorGridEffect::ThreadDataRetained);
        report_.stage = EditorGridStage::ParentMembership;
    } else if (!sameThreadOwner(context.currentThread, currentThread_.lock())) {
        return pending(EditorGridIssue::CurrentThreadChanged);
    }
    if (report_.stage == EditorGridStage::ParentMembership) {
        // QObject constructor's parent-thread precheck precedes this helper.
        // On the recovered equal-owner path the repeated helper test also
        // succeeds. Parent pointer is stored before list insertion/callbacks.
        pending_->parentWidget_ = parent;
        pending_->parentWasAssigned_ = true;
        effects_.push_back(EditorGridEffect::ParentAssigned);
        // Existing editor QObject children have not been qualified by this
        // prefix. Record the known append without asserting an empty prefix.
        parent->childPrefixUnrecovered_ = true;
        parent->childSuffix_.push_back(pending_.get());
        child_ = std::move(pending_);
        effects_.push_back(EditorGridEffect::ChildMembershipAppended);
        report_.stage = EditorGridStage::ChildEventPredicate;
    }
    if (report_.stage == EditorGridStage::ChildEventPredicate) {
        // Fresh QObjectPrivate sets sendChildEvents bit5 and clears isWidget.
        // The actual parent's receiveChildEvents bit6 is consumed only after
        // committed membership, at6712d571; it is not inferred from freshness.
        const auto receives = parent->receivesChildEvents();
        if (!receives) return pending(EditorGridIssue::UnknownReceiveChildEvents);
        report_.stage = *receives ? EditorGridStage::ChildAddedConstruction : EditorGridStage::QObjectCounter;
    }
    if (report_.stage == EditorGridStage::ChildAddedConstruction) {
        childEvent_ = WidgetEventState::childAdded(child_);
        effects_.push_back(EditorGridEffect::ChildEventConstructed);
        report_.stage = EditorGridStage::ChildAddedNotification;
    }
    if (report_.stage == EditorGridStage::ChildAddedNotification) {
        auto runtime = parent->runtimeOwner();
        if (!runtime) return pending(EditorGridIssue::RuntimeOwnerUnavailable);
        const auto present = runtime->applicationEvents().present;
        if (!present) return pending(EditorGridIssue::ApplicationPresenceUnavailable);
        // Native helper tests app pointer AFTER constructing the stack event.
        // Null skips notify altogether; it is not a false callback result.
        if (*present) {
            const auto& notified = childDispatch_.resume(*runtime, parent, *childEvent_);
            if (!notified.completed)
                return pending(EditorGridIssue::ChildAddedNotificationPending);
            effects_.push_back(EditorGridEffect::ChildNotificationReturned);
        }
        report_.stage = EditorGridStage::PostParentBookkeeping;
    }
    if (report_.stage == EditorGridStage::PostParentBookkeeping) {
        auto runtime = parent->runtimeOwner();
        if (!runtime) return pending(EditorGridIssue::RuntimeOwnerUnavailable);
        const auto declarative = runtime->applicationEvents().declarativeChildEventsEnabled;
        if (!declarative) return pending(EditorGridIssue::DeclarativeContextUnavailable);
        if (*declarative) return pending(EditorGridIssue::DeclarativeChildrenUnrecovered);
        //6712d677: native event destruction follows post-parent bookkeeping,
        // not notify return. The event borrows the actual child throughout.
        childEvent_->destroyStackEvent();
        effects_.push_back(EditorGridEffect::ChildEventDestroyed);
        report_.stage = EditorGridStage::QObjectCounter;
    }
    // Parent receive=false jumps directly to6712d681. Fresh child hookdata+78
    // is null and the closed child callback cannot change it. Shared runtime
    // owns this process count; no editor-only counter or replay on resume.
    if (report_.stage == EditorGridStage::QObjectCounter) {
        auto runtime = parent->runtimeOwner();
        if (!runtime) return pending(EditorGridIssue::RuntimeOwnerUnavailable);
        runtime->commitQObjectConstruction(*child_);
        effects_.push_back(EditorGridEffect::QObjectCountIncremented);
        report_.stage = EditorGridStage::LayoutConstruction;
    }
    return pending(EditorGridIssue::LayoutConstructionUnrecovered);
}

} // namespace m2m::engine::simcontrol300
