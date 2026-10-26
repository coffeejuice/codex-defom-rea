#include "m2m/doe_task_factory.hpp"

#include <map>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"

namespace m2m::doe {
struct TaskFactoryRuntime::Factory {
    struct Data {
        explicit Data(Factory* value) : owner(value) {}
        Factory* owner; //base7c7f70 writes data+18 to actual factory
        // Native maps are nonowning. Both are empty before prototype1 returns;
        // no unavailable TaskInfo payload or QObject is manufactured here.
        std::map<qt::String, const void*> metadata;
        std::map<qt::String, SessionTask*> tasks;
    };
    std::unique_ptr<Data> data;
};

TaskFactoryRuntime::TaskFactoryRuntime(const std::shared_ptr<TaskAllocationStore>& allocations,
    const std::shared_ptr<engine::WidgetRuntimeState>& widgets)
    : allocations_(allocations), widgets_(widgets) {}

TaskFactoryRuntime::~TaskFactoryRuntime() { releaseAdapterScope(); }

std::shared_ptr<TaskFactoryRuntime> TaskFactoryRuntime::boundedParsedDoeProjection(
    const std::shared_ptr<TaskAllocationStore>& allocations)
{
    auto runtime = std::make_shared<TaskFactoryRuntime>(allocations);
    runtime->profile_ = TaskFactoryProfile::BoundedParsedDoeProjection;
    return runtime;
}

void TaskFactoryRuntime::releaseAdapterScope() noexcept
{
    if (!adapterScopeLive_) return;
    adapterScopeLive_ = false;
    factory_.reset();
    pendingReportAllocation_.reset();
    // The actual allocation store owns pending Task allocations. Destroying a
    // factory is not an invented native Task deletion; Session releases its
    // adapter allocation scope independently at the fresh-process boundary.
}

TaskFactoryConstructionSnapshot TaskFactoryRuntime::construction() const noexcept
{
    TaskFactoryConstructionSnapshot result;
    result.factoryCommitted = factory_ != nullptr;
    result.dataCommitted = factory_ && factory_->data;
    if (result.dataCommitted) {
        result.dataFactoryBacklinkMatches = factory_->data->owner == factory_.get();
        result.metadataCacheCount = factory_->data->metadata.size();
        result.taskCacheCount = factory_->data->tasks.size();
    }
    result.pendingReportAllocation = pendingReportAllocation_;
    return result;
}

void TaskFactoryRuntime::initializeNativePrefix()
{
    if (!factory_) {
        factory_ = std::make_unique<Factory>(); //01d960 commits mainwindow+4a0
        factory_->data = std::make_unique<Factory::Data>(factory_.get()); //before137610
    }
    if (!pendingReportAllocation_) {
        const auto allocations = allocations_.lock();
        if (!allocations || !allocations->adapterScopeLive())
            throw Error(M2M_E_UNSUPPORTED,
                "DOE Task factory allocation owner unavailable before first Report allocation");
        const auto index = allocations->retainedPrototypeCount();
        allocations->allocatePendingReportPrototype(); //137610 allocates0xc0
        pendingReportAllocation_ = index;
    }
    //Report7b89e0->157ead0->1403210 first invokes QObject(parent=null).
    //Public671313d0 private/thread initialization precedes the shared counter. It is
    //not closed by a counter-only seam. In particular TaskInfo+20, current-font
    //reads, metadata, preferences and cache registration have NOT occurred.
    throw Error(M2M_E_UNSUPPORTED,
        "DOE Task factory first Report QObject(parent=null) construction remains unsupported before TaskInfo initialization");
}

TaskObserver TaskFactoryRuntime::createTaskBase(const qt::String& type)
{
    if (!adapterScopeLive_)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory adapter scope has been released");
    if (profile_ == TaskFactoryProfile::Native) {
        initializeNativePrefix(); //initializes before requested-key lookup
        return {}; //unreachable until the full17-prototype initialization closes
    }
    // Old bounded parsed list-state is retained only by this explicitly named
    // fixture profile. It is not native factory/cache/constructor completion.
    if (type == qt::L("SfTaskDoe")) {
        const auto allocations = allocations_.lock();
        if (!allocations || !allocations->adapterScopeLive())
            throw Error(M2M_E_UNSUPPORTED, "DOE bounded parsed Task projection allocation owner is unavailable");
        return allocations->createParsedDoeTask();
    }
    for (const char* known : {"SfTaskReport", "SfTaskDeformSummary", "SfTaskDeformGraph",
            "SfTaskDeformStateVar", "SfTaskDeformPointTrack", "SfTaskDeformFlownet",
            "SfTaskUsrVarTrack", "SfTaskDeformCoupons", "SfTaskDeformRegions",
            "SfTaskDeformMetalFlow", "SfTaskReportGenerate", "SfTaskDoeGlobalMinMax",
            "SfTaskDoeObjMinMax", "SfTaskDoeSVMinMax", "SfTaskDoeAllNodeValue", "Custom"})
        if (type == qt::L(known))
            throw Error(M2M_E_UNSUPPORTED, "DOE session registered non-DOE Task construction remains unsupported in bounded projection");
    return {};
}

std::shared_ptr<TaskFactoryRuntime> bindBoundedParsedDoeProjection(StudyIo& io,
    const std::shared_ptr<TaskAllocationStore>& allocations)
{
    auto runtime = TaskFactoryRuntime::boundedParsedDoeProjection(allocations);
    auto& notifier = io.sessionNotificationState();
    if (notifier.createTaskBaseRegistrations)
        throw Error(M2M_E_UNSUPPORTED, "DOE bounded parsed projection cannot replace existing receiver population");
    io.bindTaskFactoryRuntime(runtime);
    notifier.createTaskBaseRegistrations = TaskFactoryRegistrationList{
        {{runtime, "slotCreateTaskBase"}},
        "Explicit bounded parsed DOE fixture; not native factory completion", true};
    return runtime;
}
} // namespace m2m::doe
