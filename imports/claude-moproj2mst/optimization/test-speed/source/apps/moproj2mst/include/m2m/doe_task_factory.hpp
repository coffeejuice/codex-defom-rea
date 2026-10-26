#pragma once

#include <cstddef>
#include <memory>
#include <optional>

#include "m2m/doe_task_runtime.hpp"

namespace m2m::engine { class WidgetRuntimeState; }

namespace m2m::doe {
class StudyIo;

enum class TaskFactoryProfile { Native, BoundedParsedDoeProjection };

struct TaskFactoryConstructionSnapshot {
    bool factoryCommitted = false;
    bool dataCommitted = false;
    bool dataFactoryBacklinkMatches = false;
    std::size_t nextPrototypeOrdinal = 0;
    std::size_t metadataCacheCount = 0;
    std::size_t taskCacheCount = 0;
    std::optional<std::size_t> pendingReportAllocation;
    bool nativeInitializationComplete = false;
};

// The holder belongs to the actual main-window adapter, not to a Project or
// SessionRecord. Constructing it does not construct native mainwindow+4a0.
class TaskFactoryRuntime final {
public:
    explicit TaskFactoryRuntime(const std::shared_ptr<TaskAllocationStore>& allocations,
        const std::shared_ptr<engine::WidgetRuntimeState>& widgets = {});
    ~TaskFactoryRuntime();
    TaskFactoryRuntime(const TaskFactoryRuntime&) = delete;
    TaskFactoryRuntime& operator=(const TaskFactoryRuntime&) = delete;

    // Explicit isolated-fixture projection only; never the production default.
    static std::shared_ptr<TaskFactoryRuntime> boundedParsedDoeProjection(
        const std::shared_ptr<TaskAllocationStore>& allocations);
    TaskFactoryProfile profile() const noexcept { return profile_; }
    bool nativeReceiverAlive() const noexcept { return adapterScopeLive_; }
    void releaseAdapterScope() noexcept;
    TaskObserver createTaskBase(const qt::String& type);
    TaskFactoryConstructionSnapshot construction() const noexcept;
    std::shared_ptr<engine::WidgetRuntimeState> widgetRuntimeOwner() const noexcept
    { return widgets_.lock(); }

private:
    struct Factory;
    void initializeNativePrefix();
    TaskFactoryProfile profile_ = TaskFactoryProfile::Native;
    bool adapterScopeLive_ = true;
    std::weak_ptr<TaskAllocationStore> allocations_;
    std::weak_ptr<engine::WidgetRuntimeState> widgets_;
    std::unique_ptr<Factory> factory_;
    std::optional<std::size_t> pendingReportAllocation_;
};

// Return-value notifier2cf740: suppression precedes lookup and does not reset
// flag90. Uses the actual shared notifier's qualified ordered receiver list.
TaskObserver dispatchCreateTaskBase(StudyIo& io, const qt::String& type);

// The caller retains the returned runtime lease. Explicit fixture setup only;
// production must bind its actual main-window receiver and native runtime.
std::shared_ptr<TaskFactoryRuntime> bindBoundedParsedDoeProjection(StudyIo& io,
    const std::shared_ptr<TaskAllocationStore>& allocations);

} // namespace m2m::doe
