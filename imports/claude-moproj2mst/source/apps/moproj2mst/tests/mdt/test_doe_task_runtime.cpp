// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_session_records.hpp"
#include "m2m/doe_task_runtime.hpp"

using namespace m2m;

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
} // namespace

TEST_CASE(doe_task_runtime_distinct_allocations_have_nonowning_observers)
{
    doe::TaskAllocationStore store;
    auto first = store.createParsedDoeTask();
    const auto second = store.createParsedDoeTask();
    REQUIRE(first && second);
    CHECK(first.get() != second.get() && store.retainedTaskCount() == 2);
    CHECK(first->lists.operations.empty() && first->lists.objects == std::vector<int>({0}));
    CHECK(!first->executionPreferencesRecovered && !first->hasSessionBacklink());
    CHECK(first->sessionBacklink() == nullptr);
    first->lists.objects = {9, 9};
    first.reset();
    CHECK(first == nullptr && store.retainedTaskCount() == 2);
    CHECK(store.retainedTaskAt(0)->lists.objects == std::vector<int>({9, 9}));
}

TEST_CASE(doe_task_runtime_heap_record_identity_survives_pointer_list_growth)
{
    static_assert(!std::is_move_constructible_v<doe::SessionRecord>);
    doe::TaskAllocationStore store;
    const auto task = store.createParsedDoeTask();
    auto& initial = store.createSessionRecord();
    CHECK(initial.taskType == qt::L("Custom") && initial.stepNumber == -1);
    initial.title = qt::L("first");
    const auto identity = initial.identity().lock();
    doe::bindSessionTask(initial, task);
    CHECK(task->sessionBacklink() == &initial);
    std::vector<doe::SessionRecord*> records;
    records.reserve(1);
    records.push_back(&initial);
    records.push_back(&store.createSessionRecord()); //relocates pointer cells only
    CHECK(task->sessionBacklink() == records[0] && records[0] == &initial);
    CHECK(records[0]->identity().lock() == identity && records[0]->title == qt::L("first"));
    CHECK(store.retainedSessionCount() == 2);
}

TEST_CASE(doe_task_runtime_rebinding_retains_displaced_task_and_shared_identity)
{
    doe::TaskAllocationStore store;
    const auto first = store.createParsedDoeTask();
    const auto second = store.createParsedDoeTask();
    auto& a = store.createSessionRecord();
    auto& b = store.createSessionRecord();
    doe::bindSessionTask(a, first);
    doe::bindSessionTask(b, second);
    second->lists.objects = {21};
    //Declared658520 helper entry, not a fabricated initialized factory lookup.
    doe::bindSessionTask(a, second);
    CHECK(a.task.get() == b.task.get() && a.task.get() == second.get());
    CHECK(second->sessionBacklink() == &a && first->sessionBacklink() == &a);
    CHECK(a.task->lists.objects == std::vector<int>({21}));
    CHECK(first && store.retainedTaskCount() == 2);
}

TEST_CASE(doe_task_runtime_cleared_session_retention_preserves_raw_backlink)
{
    doe::TaskAllocationStore store;
    const auto task = store.createParsedDoeTask();
    auto& record = store.createSessionRecord();
    record.title = qt::L("retained");
    doe::bindSessionTask(record, task);
    std::vector<doe::SessionRecord*> records{&record};
    records.clear(); //pointer cells only; allocation is already in the same store
    CHECK(store.retainedSessionCount() == 1 && store.retainedTaskCount() == 1);
    CHECK(task->sessionBacklink() == store.retainedSessionAt(0));
    CHECK(task->sessionBacklink()->title == qt::L("retained"));
    CHECK(task->sessionBacklink()->task.get() == task.get());
}

TEST_CASE(doe_task_runtime_actual_record_destruction_is_distinct_from_type2_list_clear)
{
    doe::TaskAllocationStore store;
    const auto task = store.createParsedDoeTask();
    {
        doe::SessionRecord record;
        doe::bindSessionTask(record, task);
    } //explicit adapter record destruction; NOT the native Type2 list-clear path
    CHECK(task && task->hasSessionBacklink());
    CHECK(refuses([&] { (void)task->sessionBacklink(); }, "destroyed record"));
    CHECK(store.retainedTaskCount() == 1);
}

TEST_CASE(doe_task_runtime_null_binding_does_not_delete_old_task_or_rewrite_its_backlink)
{
    doe::TaskAllocationStore store;
    const auto task = store.createParsedDoeTask();
    auto& record = store.createSessionRecord();
    doe::bindSessionTask(record, task);
    doe::bindSessionTask(record, {});
    CHECK(record.task == nullptr && task && task->sessionBacklink() == &record);
    CHECK(store.retainedTaskCount() == 1);
}

TEST_CASE(doe_task_runtime_scope_release_invalidates_escaped_store_and_task_observers)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    const auto escaped = store;
    const auto task = store->createParsedDoeTask();
    auto& record = store->createSessionRecord();
    doe::bindSessionTask(record, task);
    const auto identity = record.identity();
    store->releaseAdapterScope();
    CHECK(!escaped->adapterScopeLive() && task == nullptr && identity.expired());
    CHECK(escaped->retainedTaskCount() == 0 && escaped->retainedSessionCount() == 0);
    CHECK(refuses([&] { (void)escaped->createParsedDoeTask(); }, "scope has been released"));
    CHECK(refuses([&] { (void)escaped->createSessionRecord(); }, "scope has been released"));
    CHECK(escaped->retainedSessionCount() == 0); //refusal precedes allocation
    escaped->releaseAdapterScope();
    CHECK(!escaped->adapterScopeLive() && task == nullptr);
}

TEST_CASE(doe_task_runtime_removing_pointer_cells_does_not_destroy_either_record)
{
    doe::TaskAllocationStore store;
    const auto first = store.createParsedDoeTask();
    const auto second = store.createParsedDoeTask();
    auto& a = store.createSessionRecord();
    auto& b = store.createSessionRecord();
    doe::bindSessionTask(a, first);
    doe::bindSessionTask(b, second);
    std::vector<doe::SessionRecord*> records{&a, &b};
    records.erase(records.begin());
    CHECK(second->sessionBacklink() == records[0] && records[0] == &b);
    CHECK(first && first->sessionBacklink() == &a);
    CHECK(store.retainedSessionCount() == 2 && store.retainedTaskCount() == 2);
}

int main() { return testkit::run_all(); }
