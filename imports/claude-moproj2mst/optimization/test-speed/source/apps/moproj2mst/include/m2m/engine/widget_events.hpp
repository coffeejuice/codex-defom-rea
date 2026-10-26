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

// Queue population is deliberately unknown until an ordered native observation
// and its insertion/removal effects are implemented. It is not an empty queue.
class WidgetPostEventQueue final {
public:
    bool populationKnown() const noexcept { return false; }
};

class WidgetThreadState final {
public:
    std::optional<std::uint32_t> notifyDepth() const noexcept;
    std::uint32_t retainedNotifyDelta() const noexcept { return notifyDelta_; }
    void observeNotifyDepth(std::uint32_t, const std::string& qualification);
    const WidgetPostEventQueue& postedEvents() const noexcept { return queue_; }
private:
    friend class WidgetRuntimeState;
    friend class WidgetEventDispatch;
    WidgetThreadState() = default;
    std::optional<std::uint32_t> depthBase_;
    std::uint32_t notifyDelta_ = 0;
    WidgetPostEventQueue queue_;
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
