#include "m2m/doe_task_runtime.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_session_records.hpp"

namespace m2m::doe {

SessionRecord* SessionTask::sessionBacklink() const
{
    if (!session_) return nullptr;
    const auto identity = session_->lock();
    if (!identity || !identity->current)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task raw Session backlink reaches destroyed record");
    return identity->current;
}

TaskAllocationStore::TaskAllocationStore() = default;
TaskAllocationStore::~TaskAllocationStore() { releaseAdapterScope(); }

void TaskAllocationStore::requireAdapterScope() const
{
    if (!adapterScopeLive_)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task allocation adapter scope has been released");
}

void TaskAllocationStore::releaseAdapterScope() noexcept
{
    if (!adapterScopeLive_) return;
    adapterScopeLive_ = false;
    sessions_.clear();
    tasks_.clear();
    prototypes_.clear();
}

const PendingTaskPrototype& TaskAllocationStore::allocatePendingReportPrototype()
{
    requireAdapterScope();
    auto pending = std::make_unique<PendingTaskPrototype>();
    const auto& result = *pending;
    prototypes_.push_back(std::move(pending));
    return result;
}

const PendingTaskPrototype* TaskAllocationStore::retainedPrototypeAt(std::size_t index) const
{
    requireAdapterScope();
    if (index >= prototypes_.size())
        throw Error(M2M_E_UNSUPPORTED, "DOE pending Task prototype diagnostic index is unavailable");
    return prototypes_[index].get();
}

TaskObserver TaskAllocationStore::createParsedDoeTask()
{
    requireAdapterScope();
    auto task = std::make_unique<SessionTask>();
    TaskObserver observer(task.get(), task->lifetime_);
    tasks_.push_back(std::move(task));
    return observer;
}

SessionRecord& TaskAllocationStore::createSessionRecord()
{
    requireAdapterScope();
    auto record = std::make_unique<SessionRecord>();
    auto& result = *record;
    sessions_.push_back(std::move(record));
    return result;
}

TaskObserver TaskAllocationStore::retainedTaskAt(std::size_t index) const
{
    requireAdapterScope();
    if (index >= tasks_.size())
        throw Error(M2M_E_UNSUPPORTED, "DOE retained Task diagnostic index is unavailable");
    const auto& task = tasks_[index];
    return TaskObserver(task.get(), task->lifetime_);
}

const SessionRecord* TaskAllocationStore::retainedSessionAt(std::size_t index) const
{
    requireAdapterScope();
    if (index >= sessions_.size())
        throw Error(M2M_E_UNSUPPORTED, "DOE retained Session diagnostic index is unavailable");
    return sessions_[index].get();
}

void bindSessionTask(SessionRecord& record, const TaskObserver& task)
{
    record.task = task;
    if (auto* live = record.task.get()) live->session_ = record.identity();
}

SessionRecord::SessionRecord()
    : identity_(std::make_shared<SessionRecordIdentity>())
{
    identity_->current = this;
}

SessionRecord::~SessionRecord()
{
    if (identity_) identity_->current = nullptr;
}

} // namespace m2m::doe
