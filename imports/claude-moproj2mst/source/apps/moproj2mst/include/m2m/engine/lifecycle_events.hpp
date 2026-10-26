#pragma once

#include <cstdint>
#include <functional>
#include <unordered_set>
#include <vector>

namespace m2m::engine {

// Session/dispatcher state, not Project state. Original setModel 0x140170430
// requests a separate QTimer::singleShot(1000, receiver, modelUpdated()) for
// every nonnull attachment. See analysis/wave3-boolean-timer-contract.json.
// This records logical timers; it neither reads a clock nor infers readiness.
class DiagramLifecycleEvents final {
public:
    using ReceiverId = std::uint64_t;
    using TimerId = std::uint64_t;

    struct PendingModelUpdate {
        TimerId id;
        ReceiverId receiver;
        unsigned requestedIntervalMs;
    };

    enum class Delivery { NotPending, Disconnected, Delivered };
    using ModelUpdated = std::function<void(ReceiverId)>;

    DiagramLifecycleEvents() = default;
    DiagramLifecycleEvents(const DiagramLifecycleEvents&) = delete;
    DiagramLifecycleEvents& operator=(const DiagramLifecycleEvents&) = delete;

    ReceiverId createReceiver();
    // Returns zero for a null binding, destroyed/unknown receiver, or a session
    // whose dispatcher has been destroyed (startTimer then returns zero).
    // A null rebind leaves older timers pending. Model/item are not captured.
    TimerId modelAttached(ReceiverId receiver, bool nonnullModel);
    void destroyReceiver(ReceiverId receiver) noexcept;
    void dispatcherDestroyed() noexcept;

    // The caller supplies an actually due timer at a qualified dispatch point.
    // Consume it before callback, matching kill/emit/delete single-shot order.
    // The live-receiver callback must resolve its THEN-current model/item and
    // run the existing replacement-selection chain (including editor refill).
    // A live null-bound receiver still receives modelUpdated; its model guard
    // belongs to that callback, after the original fit-view step.
    Delivery deliver(TimerId timer, const ModelUpdated& callback);
    const std::vector<PendingModelUpdate>& pending() const noexcept { return pending_; }

private:
    ReceiverId nextReceiver_ = 1;
    TimerId nextTimer_ = 1;
    bool dispatcherAlive_ = true;
    std::unordered_set<ReceiverId> receivers_;
    std::vector<PendingModelUpdate> pending_;
};

}  // namespace m2m::engine
