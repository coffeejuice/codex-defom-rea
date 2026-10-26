#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

#include "m2m/doe_task_lists.hpp"

namespace m2m::doe {

struct SessionRecord;
struct SessionRecordIdentity {
    SessionRecord* current = nullptr; //adapter lifetime observation, not native field
};

class TaskAllocationStore;
class TaskObserver;

// A retained native allocation at a constructor boundary, not a completed
// QObject/SessionTask. No uninitialized native bytes are assigned values here.
struct PendingTaskPrototype {
    enum class Kind { Report };
    Kind kind = Kind::Report;
    std::size_t nativeAllocationBytes = 0xc0;
    bool qobjectBaseCompleted = false;
};

// Recovered list-state of the actual parentless Task QObject. Constructor
// preferences and unrelated fields remain opaque; this is not a task runner.
struct SessionTask {
    SessionTask() = default;
    SessionTask(const SessionTask&) = delete;
    SessionTask& operator=(const SessionTask&) = delete;
    TaskLists lists{{}, {0}};
    bool executionPreferencesRecovered = false;

    // Native Task+30 is a RAW Session backlink. Initial null is distinct from a
    // formerly bound, now destroyed record; the latter guards at consumption.
    SessionRecord* sessionBacklink() const;
    bool hasSessionBacklink() const noexcept { return session_.has_value(); }

private:
    friend class TaskAllocationStore;
    friend void bindSessionTask(SessionRecord&, const TaskObserver&);
    std::shared_ptr<const unsigned char> lifetime_ = std::make_shared<const unsigned char>(0);
    std::optional<std::weak_ptr<SessionRecordIdentity>> session_;
};

// QPointer-style observation: copies do not own the Task. This is distinct from
// the native factory's raw cache, whose initialization remains guarded.
class TaskObserver {
public:
    TaskObserver() = default;
    SessionTask* get() const noexcept { return lifetime_.expired() ? nullptr : pointer_; }
    SessionTask* operator->() const noexcept { return get(); }
    SessionTask& operator*() const { return *get(); }
    explicit operator bool() const noexcept { return get() != nullptr; }
    void reset() noexcept { lifetime_.reset(); pointer_ = nullptr; }
    friend bool operator==(const TaskObserver& task, std::nullptr_t) noexcept
    { return task.get() == nullptr; }

private:
    friend class TaskAllocationStore;
    TaskObserver(SessionTask* pointer, const std::shared_ptr<const unsigned char>& lifetime)
        : pointer_(pointer), lifetime_(lifetime) {}
    SessionTask* pointer_ = nullptr;
    std::weak_ptr<const unsigned char> lifetime_;
};

// Adapter allocation retention for one original fresh-process/session scope.
// Type2 clear and concrete Mo factory clear do not delete these allocations.
// This does not model an initialized 17-prototype registry or all native Close.
class TaskAllocationStore {
public:
    TaskAllocationStore();
    ~TaskAllocationStore();
    TaskAllocationStore(const TaskAllocationStore&) = delete;
    TaskAllocationStore& operator=(const TaskAllocationStore&) = delete;

    TaskObserver createParsedDoeTask();
    const PendingTaskPrototype& allocatePendingReportPrototype();
    SessionRecord& createSessionRecord();
    // Adapter fresh-process boundary, NOT native normal Close deletion. A
    // retained external shared lease cannot keep semantic objects live past it.
    void releaseAdapterScope() noexcept;
    bool adapterScopeLive() const noexcept { return adapterScopeLive_; }
    std::size_t retainedTaskCount() const noexcept { return tasks_.size(); }
    std::size_t retainedSessionCount() const noexcept { return sessions_.size(); }
    std::size_t retainedPrototypeCount() const noexcept { return prototypes_.size(); }
    const PendingTaskPrototype* retainedPrototypeAt(std::size_t index) const;
    TaskObserver retainedTaskAt(std::size_t index) const;
    const SessionRecord* retainedSessionAt(std::size_t index) const;

private:
    void requireAdapterScope() const;
    bool adapterScopeLive_ = true;
    std::vector<std::unique_ptr<SessionTask>> tasks_;
    std::vector<std::unique_ptr<SessionRecord>> sessions_;
    std::vector<std::unique_ptr<PendingTaskPrototype>> prototypes_;
};

//658520: assign Session+58, then Task+30 only if the observed Task is live.
void bindSessionTask(SessionRecord& record, const TaskObserver& task);

} // namespace m2m::doe
