// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_task_factory.hpp"

using namespace m2m;
using qt::L;

namespace {
bool refuses(const auto& action, const char* message)
{
    try { action(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find(message) != std::string::npos;
    }
    return false;
}

void qualifiedReceiver(doe::StudyIo& io, const std::shared_ptr<doe::TaskFactoryRuntime>& factory)
{
    io.bindTaskFactoryRuntime(factory);
    io.sessionNotificationState().createTaskBaseRegistrations = doe::TaskFactoryRegistrationList{
        {{factory, "slotCreateTaskBase"}}, "Explicit fixed-source receiver test profile", true};
}
} // namespace

TEST_CASE(doe_task_factory_suppression_precedes_unavailable_registration_and_runtime)
{
    auto io = doe::makeRecordingStudyIo();
    auto& notifier = io->sessionNotificationState();
    notifier.suppressed = true;
    notifier.flag90 = true;
    CHECK(doe::dispatchCreateTaskBase(*io, L("Custom")) == nullptr);
    CHECK(!io->taskFactoryRuntimeOwner() && !notifier.createTaskBaseRegistrations);
    CHECK(notifier.flag90 && notifier.emptyKeys.empty());
}

TEST_CASE(doe_task_factory_unknown_receiver_population_is_not_empty)
{
    auto io = doe::makeRecordingStudyIo();
    io->sessionNotificationState().flag90 = true;
    CHECK(refuses([&] { doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")); }, "registration population"));
    CHECK(io->sessionNotificationState().flag90 && io->sessionNotificationState().emptyKeys.empty());
}

TEST_CASE(doe_task_factory_native_prefix_commits_factory_data_and_raw_report_before_qobject_guard)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto factory = std::make_shared<doe::TaskFactoryRuntime>(store);
    auto io = doe::makeRecordingStudyIo();
    qualifiedReceiver(*io, factory);
    io->sessionNotificationState().flag90 = true;
    CHECK(factory->profile() == doe::TaskFactoryProfile::Native);
    CHECK(!factory->construction().factoryCommitted);
    CHECK(refuses([&] { doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")); }, "Report QObject(parent=null)"));
    const auto state = factory->construction();
    CHECK(state.factoryCommitted && state.dataCommitted && state.dataFactoryBacklinkMatches);
    REQUIRE(state.pendingReportAllocation.has_value());
    CHECK(*state.pendingReportAllocation == 0 && state.nextPrototypeOrdinal == 0);
    CHECK(!state.nativeInitializationComplete && state.metadataCacheCount == 0 && state.taskCacheCount == 0);
    REQUIRE(store->retainedPrototypeCount() == 1);
    const auto* pending = store->retainedPrototypeAt(0);
    CHECK(pending->kind == doe::PendingTaskPrototype::Kind::Report);
    CHECK(pending->nativeAllocationBytes == 0xc0 && !pending->qobjectBaseCompleted);
    CHECK(store->retainedTaskCount() == 0);
    CHECK(io->sessionNotificationState().flag90 && io->sessionNotificationState().emptyKeys.empty());
}

TEST_CASE(doe_task_factory_native_unknown_key_still_initializes_before_key_lookup)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    doe::TaskFactoryRuntime factory(store);
    CHECK(refuses([&] { factory.createTaskBase(L("Unregistered")); }, "Report QObject(parent=null)"));
    CHECK(factory.construction().dataCommitted && store->retainedPrototypeCount() == 1);
}

TEST_CASE(doe_task_factory_repeat_guard_does_not_replay_allocations_or_complete_cache)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    doe::TaskFactoryRuntime factory(store);
    for (int index = 0; index < 3; ++index)
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskDoe")); }, "before TaskInfo"));
    CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
    CHECK(factory.construction().taskCacheCount == 0 && factory.construction().nextPrototypeOrdinal == 0);
    //This is retained adapter suspension, not a native C++ failure/retry claim.
}

TEST_CASE(doe_task_factory_expired_allocation_owner_guards_after_factory_data_commits)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    doe::TaskFactoryRuntime factory(store);
    store.reset();
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskDoe")); }, "before first Report allocation"));
    const auto state = factory.construction();
    CHECK(state.factoryCommitted && state.dataCommitted && state.dataFactoryBacklinkMatches);
    CHECK(!state.pendingReportAllocation && state.taskCacheCount == 0);
}

TEST_CASE(doe_task_factory_explicit_projection_is_not_native_initialization)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto io = doe::makeRecordingStudyIo();
    const auto factory = doe::bindBoundedParsedDoeProjection(*io, store);
    const auto first = doe::dispatchCreateTaskBase(*io, L("SfTaskDoe"));
    const auto second = doe::dispatchCreateTaskBase(*io, L("SfTaskDoe"));
    CHECK(first && second && first.get() != second.get());
    CHECK(factory->profile() == doe::TaskFactoryProfile::BoundedParsedDoeProjection);
    CHECK(!factory->construction().factoryCommitted && store->retainedPrototypeCount() == 0);
    CHECK(!factory->construction().nativeInitializationComplete && store->retainedTaskCount() == 2);
    CHECK(doe::dispatchCreateTaskBase(*io, qt::String()) == nullptr);
    CHECK(refuses([&] { doe::dispatchCreateTaskBase(*io, L("Custom")); }, "bounded projection"));
}

TEST_CASE(doe_task_factory_observed_empty_list_inserts_key_without_resetting_flag90)
{
    auto io = doe::makeRecordingStudyIo();
    auto& notifier = io->sessionNotificationState();
    notifier.flag90 = true;
    notifier.createTaskBaseRegistrations = doe::TaskFactoryRegistrationList{
        {}, "Explicit observed empty return-receiver list", false};
    CHECK(doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")) == nullptr);
    CHECK(notifier.createTaskBaseRegistrations->mapKeyPresent && notifier.flag90);
    CHECK(notifier.emptyKeys.count("2signalCreateTaskBase()") == 1);
}

TEST_CASE(doe_task_factory_expired_receiver_is_skipped_without_pruning_persistent_list)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto io = doe::makeRecordingStudyIo();
    auto factory = doe::bindBoundedParsedDoeProjection(*io, store);
    factory.reset();
    CHECK(!io->taskFactoryRuntimeOwner());
    CHECK(doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")) == nullptr);
    CHECK(io->sessionNotificationState().createTaskBaseRegistrations->entries.size() == 1);
    CHECK(io->sessionNotificationState().emptyKeys.empty() && store->retainedTaskCount() == 0);
}

TEST_CASE(doe_task_factory_receiver_order_retains_first_effect_before_second_receiver_guard)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto first = doe::TaskFactoryRuntime::boundedParsedDoeProjection(store);
    auto second = std::make_shared<doe::TaskFactoryRuntime>(store);
    auto io = doe::makeRecordingStudyIo();
    io->sessionNotificationState().createTaskBaseRegistrations = doe::TaskFactoryRegistrationList{
        {{first, "slotCreateTaskBase"}, {second, "slotCreateTaskBase"}},
        "Explicit ordered two-receiver fixture, not fixed startup singleton", true};
    CHECK(refuses([&] { doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")); }, "Report QObject(parent=null)"));
    CHECK(store->retainedTaskCount() == 1 && store->retainedPrototypeCount() == 1);
    CHECK(!first->construction().factoryCommitted && second->construction().dataCommitted);
}

TEST_CASE(doe_task_factory_binding_rejects_replacement_and_released_receiver_is_skipped)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto io = doe::makeRecordingStudyIo();
    const auto factory = doe::bindBoundedParsedDoeProjection(*io, store);
    io->bindTaskFactoryRuntime(factory);
    auto other = std::make_shared<doe::TaskFactoryRuntime>(store);
    CHECK(refuses([&] { io->bindTaskFactoryRuntime(other); }, "without replacement"));
    factory->releaseAdapterScope();
    factory->releaseAdapterScope();
    CHECK(!factory->nativeReceiverAlive() && io->taskFactoryRuntimeOwner() == factory);
    CHECK(doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")) == nullptr);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, "scope has been released"));
    CHECK(store->retainedTaskCount() == 0);
}

TEST_CASE(doe_task_factory_released_holder_does_not_delete_store_owned_pending_prototype)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto factory = std::make_shared<doe::TaskFactoryRuntime>(store);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, "before TaskInfo"));
    factory->releaseAdapterScope();
    CHECK(!factory->construction().factoryCommitted && store->retainedPrototypeCount() == 1);
    CHECK(!store->retainedPrototypeAt(0)->qobjectBaseCompleted);
    store->releaseAdapterScope();
    CHECK(store->retainedPrototypeCount() == 0);
    //Both releases are adapter scope cleanup, not native normal-Close proof.
}

TEST_CASE(doe_task_factory_binding_cannot_replace_expired_actual_owner)
{
    auto io = doe::makeRecordingStudyIo();
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto original = std::make_shared<doe::TaskFactoryRuntime>(store);
    io->bindTaskFactoryRuntime(original);
    original.reset();
    CHECK(!io->taskFactoryRuntimeOwner());
    auto replacement = std::make_shared<doe::TaskFactoryRuntime>(store);
    CHECK(refuses([&] { io->bindTaskFactoryRuntime(replacement); }, "without replacement"));
    CHECK(refuses([&] { io->bindTaskFactoryRuntime({}); }, "without replacement"));
    CHECK(!io->taskFactoryRuntimeOwner());
}

TEST_CASE(doe_task_factory_return_slot_is_overwritten_in_receiver_order)
{
    auto firstStore = std::make_shared<doe::TaskAllocationStore>();
    auto secondStore = std::make_shared<doe::TaskAllocationStore>();
    auto first = doe::TaskFactoryRuntime::boundedParsedDoeProjection(firstStore);
    auto second = doe::TaskFactoryRuntime::boundedParsedDoeProjection(secondStore);
    auto io = doe::makeRecordingStudyIo();
    io->sessionNotificationState().createTaskBaseRegistrations = doe::TaskFactoryRegistrationList{
        {{first, "slotCreateTaskBase"}, {second, "slotCreateTaskBase"}},
        "Explicit ordered multi-receiver fixture; not native startup population", true};
    const auto result = doe::dispatchCreateTaskBase(*io, L("SfTaskDoe"));
    REQUIRE(result);
    CHECK(firstStore->retainedTaskCount() == 1 && secondStore->retainedTaskCount() == 1);
    CHECK(result.get() == secondStore->retainedTaskAt(0).get());
    CHECK(result.get() != firstStore->retainedTaskAt(0).get());
}

int main() { return testkit::run_all(); }
