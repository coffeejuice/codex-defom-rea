#pragma once
#include <memory>
#include <string>
#include <vector>
#include "m2m/engine/view_manager.hpp"
#include "m2m/engine/widget_events.hpp"

namespace m2m::engine::simcontrol300 {

class StagedEditor300;

struct EditorGridContext {
    const StagedEditor300* receiver = nullptr;
    std::string qualification;
    // Actual current native thread owner, not an assertion that a callback
    // succeeded. Missing/different identities remain source-stage guards.
    std::shared_ptr<WidgetThreadState> currentThread;
};

enum class EditorGridStage {
    NotStarted, ThreadSelection, ParentMembership, ChildEventPredicate,
    ChildAddedConstruction, ChildAddedNotification, PostParentBookkeeping,
    QObjectCounter, LayoutConstruction
};
enum class EditorGridIssue {
    None, IdentityMismatch, MissingQualification, SetupOwnerUnavailable,
    HistoricalState, WrongStage, PropertyNotReturned, WidgetOwnerUnavailable,
    RuntimeOwnerUnavailable, CurrentThreadUnavailable, ParentThreadUnavailable,
    ParentThreadMismatch, CurrentThreadChanged, UnknownReceiveChildEvents,
    ChildAddedUnrecovered, QObjectCounterUnrecovered,
    ApplicationPresenceUnavailable, ChildAddedNotificationPending,
    DeclarativeContextUnavailable, DeclarativeChildrenUnrecovered,
    LayoutConstructionUnrecovered
};
enum class EditorGridEffect {
    AllocationAndPrivateConstruction, QObjectBaseInstalled, ThreadDataRetained,
    ParentAssigned, ChildMembershipAppended, ChildEventConstructed,
    ChildNotificationReturned, ChildEventDestroyed, QObjectCountIncremented
};
struct EditorGridReport {
    EditorGridStage stage = EditorGridStage::NotStarted;
    EditorGridIssue issue = EditorGridIssue::None;
    bool attempted = false;
    // The current prefix never reaches QLayout/QGridLayout/UI publication.
    static constexpr bool constructorReturned = false;
    static constexpr bool completeForNativeSetup = false;
};

// This is the retained original first-grid constructor call frame. The same
// concrete GridLayoutState moves from pending allocation to actual editor child;
// neither diagnostic leases nor the later UI pointer become QObject owners.
class EditorGridConstruction final {
public:
    ~EditorGridConstruction();
    EditorGridConstruction(const EditorGridConstruction&) = delete;
    EditorGridConstruction& operator=(const EditorGridConstruction&) = delete;
    const EditorGridReport& report() const noexcept { return report_; }
    const std::vector<EditorGridEffect>& effects() const noexcept { return effects_; }
    std::shared_ptr<const GridLayoutState> pendingAllocation() const noexcept { return pending_; }
    std::shared_ptr<const GridLayoutState> ownedChild() const noexcept { return child_; }
    std::shared_ptr<const GridLayoutState> actualGrid() const noexcept { return pending_ ? pending_ : child_; }
    const std::string& qualification() const noexcept { return qualification_; }
    const WidgetEventState* childEvent() const noexcept { return childEvent_.get(); }
    const WidgetDispatchReport& childNotification() const noexcept { return childDispatch_.report(); }
private:
    friend class StagedEditor300;
    explicit EditorGridConstruction(const StagedEditor300*, const std::shared_ptr<WidgetState>&);
    EditorGridReport resume(const EditorGridContext&);
    void releaseNativeOwner() noexcept;
    const StagedEditor300* factoryIdentity_;
    std::weak_ptr<WidgetState> parent_;
    std::weak_ptr<WidgetThreadState> currentThread_;
    std::shared_ptr<GridLayoutState> pending_, child_;
    std::unique_ptr<WidgetEventState> childEvent_;
    WidgetEventDispatch childDispatch_;
    EditorGridReport report_;
    std::vector<EditorGridEffect> effects_;
    std::string qualification_;
};

} // namespace m2m::engine::simcontrol300
