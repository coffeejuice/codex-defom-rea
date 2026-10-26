#pragma once
#include <utility>
#include "m2m/engine/view_manager.hpp"

namespace m2m::engine {

class WidgetEventState final {
public:
    // C++ owns the retained record. These represented native events belong to
    // synchronous stack frames and are never transferred to a queue.
    static std::unique_ptr<WidgetEventState> dynamicPropertyChange(std::string nameBytes);
    static std::unique_ptr<WidgetEventState> create();
    static std::unique_ptr<WidgetEventState> childAdded(const std::shared_ptr<GridLayoutState>&);
    WidgetEventState(const WidgetEventState&) = delete;
    WidgetEventState& operator=(const WidgetEventState&) = delete;
    std::uint16_t rawType() const noexcept { return type_; }
    const std::string& propertyName() const noexcept { return propertyName_; }
    bool accepted() const noexcept { return accepted_; }
    bool spontaneous() const noexcept { return spontaneous_; }
    bool posted() const noexcept { return posted_; }
    bool nativeAlive() const noexcept { return alive_; }
    bool nativePrivateDataNull() const noexcept { return true; }
    std::shared_ptr<GridLayoutState> child() const noexcept { return child_.lock(); }
    void destroyStackEvent();
private:
    friend class WidgetEventDispatch;
    struct Lifetime {};
    explicit WidgetEventState(std::string name) : propertyName_(std::move(name)) {}
    std::uint16_t type_ = 0xaa;
    std::string propertyName_;
    std::weak_ptr<GridLayoutState> child_;
    bool accepted_ = true, spontaneous_ = false, posted_ = false;
    bool alive_ = true, activeDispatch_ = false;
    std::shared_ptr<Lifetime> lifetime_ = std::make_shared<Lifetime>();
};

// Separate heap-event/record observations; stack WidgetEventState objects never
// enter this queue. Aliases are local native identities, not vector ordinals.
enum class WidgetPostedDeletingClass { Unknown, PlainQtGuiQEvent };
struct WidgetPostedEventObservation {
    std::uint32_t alias = 0;
    std::uint16_t rawType = 0, flags = 0;
    WidgetPostedDeletingClass deletingClass = WidgetPostedDeletingClass::Unknown;
};
struct WidgetPostRecordObservation {
    std::uint32_t alias = 0;
    std::shared_ptr<WidgetState> receiver;
    std::optional<WidgetPostedEventObservation> event;
    std::int32_t priority = 0;
};
struct WidgetPostedReceiverObservation {
    std::shared_ptr<WidgetState> receiver;
    std::uint32_t postedCount = 0;
};
struct WidgetPostedQueueObservation {
    bool completePopulation = false, uniqueBacking = false;
    std::int32_t begin = 0, capacity = 0;
    std::optional<std::uint32_t> cleanupControl38;
    std::vector<WidgetPostedReceiverObservation> receivers;
    std::vector<WidgetPostRecordObservation> records;
};
class WidgetPostedEventState final {
public:
    std::uint32_t observationAlias() const noexcept { return alias_; }
    std::uint16_t rawType() const noexcept { return type_; }
    std::uint16_t flags() const noexcept { return flags_; }
    bool posted() const noexcept { return (flags_ & 1u) != 0; }
    bool nativeAlive() const noexcept { return alive_; }
private:
    friend class WidgetPostEventQueue;
    explicit WidgetPostedEventState(const WidgetPostedEventObservation& o)
        : alias_(o.alias), type_(o.rawType), flags_(o.flags), deletingClass_(o.deletingClass) {}
    std::uint32_t alias_;
    std::uint16_t type_, flags_;
    WidgetPostedDeletingClass deletingClass_;
    bool alive_ = true;
};
class WidgetPostEventRecordState final {
public:
    std::uint32_t observationAlias() const noexcept { return alias_; }
    std::shared_ptr<WidgetState> receiver() const noexcept { return receiver_.lock(); }
    std::shared_ptr<const WidgetPostedEventState> event() const noexcept { return event_; }
    std::int32_t priority() const noexcept { return priority_; }
    bool nativeAlive() const noexcept { return alive_; }
private:
    friend class WidgetPostEventQueue;
    WidgetPostEventRecordState() = default;
    std::uint32_t alias_ = 0;
    std::weak_ptr<WidgetState> receiver_;
    std::shared_ptr<WidgetPostedEventState> event_;
    std::int32_t priority_ = 0;
    bool alive_ = true;
};
enum class WidgetQueueRemovalEffectKind {
    Lock, ReceiverCountDecrement, EventPostedCleared, EventTombstoned,
    RecordSwap, RecordReleased, Unlock, EventDeleted
};
struct WidgetQueueRemovalEffect {
    WidgetQueueRemovalEffectKind kind;
    std::uint32_t alias = 0;
};
struct WidgetQueueRemovalReport {
    std::uint32_t matched = 0;
    std::vector<WidgetQueueRemovalEffect> effects;
};
// Unknown until a complete qualified observation is bound. This helper closes
// selected removal only; it does not post, pump, wake or finish a constructor.
class WidgetPostEventQueue final {
public:
    WidgetPostEventQueue(const WidgetPostEventQueue&) = delete;
    WidgetPostEventQueue& operator=(const WidgetPostEventQueue&) = delete;
    bool populationKnown() const noexcept { return known_; }
    std::optional<std::int32_t> begin() const noexcept;
    std::optional<std::int32_t> end() const noexcept;
    bool lockHeld() const noexcept { return lockHeld_; }
    std::vector<std::shared_ptr<const WidgetPostEventRecordState>> records() const;
    void observe(const WidgetPostedQueueObservation&, const std::string& qualification);
    WidgetQueueRemovalReport removePostedEvents(WidgetThreadState& currentThread,
        const std::shared_ptr<WidgetState>& receiver, std::int32_t requestedType = 0);
private:
    friend class WidgetThreadState;
    explicit WidgetPostEventQueue(WidgetThreadState* owner) : owner_(owner) {}
    WidgetThreadState* owner_;
    bool known_ = false, uniqueBacking_ = false, lockHeld_ = false;
    std::int32_t begin_ = 0, capacity_ = 0;
    std::optional<std::uint32_t> cleanupControl38_;
    std::vector<std::weak_ptr<WidgetState>> receivers_;
    std::vector<std::shared_ptr<WidgetPostEventRecordState>> records_;
};

class WidgetThreadState final {
public:
    std::optional<std::uint32_t> notifyDepth() const noexcept;
    std::uint32_t retainedNotifyDelta() const noexcept { return notifyDelta_; }
    void observeNotifyDepth(std::uint32_t, const std::string& qualification);
    const WidgetPostEventQueue& postedEvents() const noexcept { return queue_; }
    WidgetPostEventQueue& postedEvents() noexcept { return queue_; }
private:
    friend class WidgetRuntimeState;
    friend class WidgetEventDispatch;
    WidgetThreadState() = default;
    std::optional<std::uint32_t> depthBase_;
    std::uint32_t notifyDelta_ = 0;
    WidgetPostEventQueue queue_{this};
};

enum class WidgetDispatchStage {
    NotStarted, InternalCallbacks, Application, ApplicationGestures, ApplicationFilters,
    ReceiverLayout, ReceiverFilters, Receiver, Complete
};
struct WidgetDispatchReport {
    WidgetDispatchStage stage = WidgetDispatchStage::NotStarted;
    bool completed = false;
    std::optional<bool> originalResult;
    std::string issue;
};

// One retained native call frame. A guarded call keeps its +1 depth mutation;
// retry resumes the same frame/event/receiver and never increments twice.
// Destroying/abandoning this diagnostic frame does not simulate native unwind
// or repair its retained thread delta; that exception path remains unclosed.
class WidgetEventDispatch final {
public:
    const WidgetDispatchReport& resume(WidgetRuntimeState&, const std::shared_ptr<WidgetState>&,
                                       WidgetEventState&);
    const WidgetDispatchReport& report() const noexcept { return report_; }
private:
    WidgetDispatchReport report_;
    WidgetRuntimeState* runtime_ = nullptr;
    std::weak_ptr<WidgetRuntimeState> runtimeOwner_;
    std::weak_ptr<WidgetState> receiver_;
    std::weak_ptr<WidgetThreadState> thread_;
    WidgetEventState* event_ = nullptr;
    std::weak_ptr<WidgetEventState::Lifetime> eventLifetime_;
    bool depthEntered_ = false;
};

enum class WidgetConstructorTailStage {
    FontDevice, Attributes, WidgetCounts, Compatibility, Create, PostPending, Complete
};
struct WidgetConstructorTailReport {
    WidgetConstructorTailStage stage = WidgetConstructorTailStage::FontDevice;
    bool completed = false;
    std::string issue;
};
// One actual QWidgetPrivate::init call tail, shared by splitter/editor cursors.
// Stack Create remains owned here until native post-return/dtor order closes.
class WidgetConstructorTail final {
public:
    const WidgetConstructorTailReport& resume(WidgetRuntimeState&, const std::shared_ptr<WidgetState>&);
    const WidgetConstructorTailReport& report() const noexcept { return report_; }
    const WidgetEventState* createEvent() const noexcept { return create_.get(); }
private:
    WidgetConstructorTailReport report_;
    std::weak_ptr<WidgetRuntimeState> runtime_;
    std::weak_ptr<WidgetState> receiver_;
    bool bound_ = false;
    std::unique_ptr<WidgetEventState> create_;
    WidgetEventDispatch dispatch_;
};

} // namespace m2m::engine
