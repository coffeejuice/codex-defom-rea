#include "m2m/engine/lifecycle_events.hpp"

#include <algorithm>
#include <stdexcept>

namespace m2m::engine {

DiagramLifecycleEvents::ReceiverId DiagramLifecycleEvents::createReceiver()
{
    // IDs model QObject lifetime, including address reuse after destruction.
    // Normal allocation is the recovered scope; identifier exhaustion is an
    // adapter guard, not a claim about Qt's allocation-failure behavior.
    if (nextReceiver_ == 0) throw std::overflow_error("diagram receiver ID exhausted");
    const ReceiverId receiver = nextReceiver_++;
    receivers_.insert(receiver);
    return receiver;
}

DiagramLifecycleEvents::TimerId
DiagramLifecycleEvents::modelAttached(ReceiverId receiver, bool nonnullModel)
{
    if (!dispatcherAlive_ || !nonnullModel || receivers_.find(receiver) == receivers_.end()) return 0;
    if (nextTimer_ == 0) throw std::overflow_error("diagram timer ID exhausted");
    const TimerId timer = nextTimer_++;
    pending_.push_back({timer, receiver, 1000});
    return timer;
}

void DiagramLifecycleEvents::destroyReceiver(ReceiverId receiver) noexcept
{
    // QObject::~QObject disconnects the receiver; the dispatcher still owns
    // each QSingleShotTimer until its timeout or dispatcher destruction.
    receivers_.erase(receiver);
}

void DiagramLifecycleEvents::dispatcherDestroyed() noexcept
{
    // Dispatcher children are destroyed; receivers need not be its children.
    // This session has no replacement dispatcher; a fresh session uses a new
    // events owner. Original QObject::startTimer returns zero without one.
    dispatcherAlive_ = false;
    pending_.clear();
}

DiagramLifecycleEvents::Delivery
DiagramLifecycleEvents::deliver(TimerId timer, const ModelUpdated& callback)
{
    const auto found = std::find_if(pending_.begin(), pending_.end(),
        [timer](const PendingModelUpdate& event) { return event.id == timer; });
    if (found == pending_.end()) return Delivery::NotPending;
    const ReceiverId receiver = found->receiver;
    pending_.erase(found);
    if (receivers_.find(receiver) == receivers_.end()) return Delivery::Disconnected;
    callback(receiver);
    return Delivery::Delivered;
}

}  // namespace m2m::engine
