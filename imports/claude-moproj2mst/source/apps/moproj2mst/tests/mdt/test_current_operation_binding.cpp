// ctest-labels: m2m-mdt current-operation-binding
#include <testkit/check.hpp>

#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

using namespace m2m;
using engine::OperationInfoExpiry;
namespace {
constexpr const char* fixtureBoundary =
    "isolated declared OPI fixture at050531/6712f8bd; no native Close acceptance";

// Matches the existing explicit save/call/restore sequence, deliberately not an
// exception-restoring scope. Actual master/Cycle/loader sites use these methods.
void temporarilyBind(mst::OpClass& cls, Operation& temporary, const std::function<void()>& callback)
{
    const auto saved = cls.current.snapshot();
    cls.current = &temporary;
    callback();
    cls.current.restore(saved);
}

struct CleanupProbe final : Props {
    explicit CleanupProbe(std::function<void()> callback) : callback_(std::move(callback)) {}
    ~CleanupProbe() override { callback_(); }
private:
    std::function<void()> callback_;
};
}

static_assert(!std::is_copy_constructible_v<Operation>);
static_assert(!std::is_move_constructible_v<Operation>);
static_assert(!std::is_constructible_v<engine::CurrentOperationBinding, Operation*>);

TEST_CASE(current_binding_preserves_actual_opclass_pointer_read_and_assignment_forms)
{
    mst::OpClass cls(qt::L("BindingTest"));
    Operation operation;
    operation.id = 81;
    CHECK(cls.current == nullptr);
    CHECK(!cls.current);
    cls.current = &operation;
    CHECK(cls.current == &operation);
    CHECK(&operation == cls.current);
    CHECK(cls.current != nullptr);
    CHECK(cls.current->id == 81);
    CHECK((*cls.current).id == 81);
    Operation* extracted = cls.current;
    CHECK(extracted == &operation);
    Operation* fromNullConditional = true ? cls.current : nullptr;
    CHECK(fromNullConditional == &operation);
    Operation* fromPointerConditional = false ? &operation : cls.current;
    CHECK(fromPointerConditional == &operation);
    auto* classPointer = &cls;
    Operation* fromClassConditional = classPointer ? classPointer->current : nullptr;
    CHECK(fromClassConditional == &operation);
    operation.props = std::make_shared<Props>();
    CHECK(cls.propsOf(nullptr) == operation.props.get()); // actual pointer/guard conditional in base.cpp
    const auto saved = cls.current.snapshot();
    cls.current = &operation;
    CHECK(cls.current == &operation);
    CHECK(saved.get() == &operation);
    cls.current = nullptr;
    CHECK(!cls.current);
    CHECK(cls.current.snapshot().expiry() == OperationInfoExpiry::Unbound);
    CHECK(saved.get() == &operation);
    cls.current.restore(saved);
    CHECK(cls.current == &operation);
}

TEST_CASE(current_binding_aliases_follow_actual_operation_boundary_without_affecting_another)
{
    mst::OpClass first(qt::L("First")), alias(qt::L("Alias")), other(qt::L("Other"));
    Operation operation, unrelated;
    first.current = &operation;
    alias.current = &operation;
    other.current = &unrelated;
    CHECK(operation.commitOperationInfoQObjectBoundary(fixtureBoundary));
    CHECK(!first.current);
    CHECK(!alias.current);
    CHECK(other.current == &unrelated);
    CHECK(first.current.snapshot().expiry() == OperationInfoExpiry::QObjectBoundary);
    CHECK(first.current.snapshot().boundaryQualification() == fixtureBoundary);
    first.current = &operation; // an already-expired identity cannot resurrect
    CHECK(!first.current);
}

TEST_CASE(current_binding_retains_current_during_payload_probe_then_expires_while_outer_row_lives)
{
    mst::OpClass cls(qt::L("PayloadProbe"));
    auto row = std::make_unique<Operation>();
    cls.current = row.get();
    bool sawCurrent = false;
    row->props = std::make_shared<CleanupProbe>([&] { sawCurrent = cls.current == row.get(); });
    row->props.reset(); // controlled fixture payload, not native destructor parity
    CHECK(sawCurrent);
    CHECK(row->commitOperationInfoQObjectBoundary(fixtureBoundary));
    CHECK(cls.current == nullptr); // admitted observer would now see null
    CHECK(row != nullptr); // explicit OPI boundary precedes outer item destruction
    row.reset();
    CHECK(cls.current.snapshot().expiry() == OperationInfoExpiry::QObjectBoundary);
}

TEST_CASE(current_binding_adapter_fallback_expires_without_retaining_actual_operation_storage)
{
    mst::OpClass cls(qt::L("Fallback"));
    bool payloadDeleted = false;
    {
        auto operation = std::make_unique<Operation>();
        operation->props = std::make_shared<CleanupProbe>([&] { payloadDeleted = true; });
        cls.current = operation.get();
        CHECK(cls.current == operation.get());
    }
    CHECK(payloadDeleted);
    CHECK(cls.current == nullptr);
    CHECK(cls.current.snapshot().expiry() == OperationInfoExpiry::AdapterIdentityLifetime);
}

TEST_CASE(current_binding_snapshot_restore_does_not_dereference_a_freed_saved_row)
{
    mst::OpClass cls(qt::L("RestoreExpired"));
    auto old = std::make_unique<Operation>();
    Operation temporary;
    cls.current = old.get();
    temporarilyBind(cls, temporary, [&] {
        CHECK(cls.current == &temporary);
        old.reset(); // old metadata snapshot remains nonowning and becomes expired
    });
    CHECK(!cls.current);
    CHECK(cls.current.snapshot().expiry() == OperationInfoExpiry::AdapterIdentityLifetime);
}

TEST_CASE(current_binding_nested_temporary_calls_restore_each_live_snapshot)
{
    mst::OpClass cls(qt::L("Nested"));
    Operation original, outer, inner;
    cls.current = &original;
    temporarilyBind(cls, outer, [&] {
        CHECK(cls.current == &outer);
        temporarilyBind(cls, inner, [&] { CHECK(cls.current == &inner); });
        CHECK(cls.current == &outer);
    });
    CHECK(cls.current == &original);
}

TEST_CASE(current_binding_explicit_restore_still_does_not_execute_on_inner_exception)
{
    mst::OpClass cls(qt::L("ThrowTiming"));
    Operation original, temporary;
    cls.current = &original;
    bool caught = false;
    try {
        temporarilyBind(cls, temporary, [] { throw std::runtime_error("declared fixture throw"); });
    } catch (const std::runtime_error&) { caught = true; }
    CHECK(caught);
    CHECK(cls.current == &temporary); // same existing explicit-restore timing
}

TEST_CASE(current_binding_registry_survives_project_and_observes_slot_storage_fallback_expiry)
{
    auto project = std::make_unique<Project>();
    auto slots = project->modelSlotsOwner();
    auto registry = project->registryOwner();
    auto cls = std::make_unique<mst::OpClass>(qt::L("Retained"));
    auto* actualClass = cls.get();
    registry->entries().emplace(qt::L("Retained"), std::move(cls));
    auto operation = std::make_unique<Operation>();
    auto* actualOperation = operation.get();
    operation->opClass = actualClass;
    actualClass->current = actualOperation;
    project->operations.push_back(std::move(operation));
    project.reset();
    CHECK(actualClass->current == actualOperation);
    // Capsule storage release is not native OPI/Qt destructor execution.
    slots->releaseCompletedSlotStorage(engine::MoModelSlots::Slot::Operations);
    CHECK(actualClass->current == nullptr);
    CHECK(actualClass->current.snapshot().expiry() == OperationInfoExpiry::AdapterIdentityLifetime);
    CHECK(registry->entries().size() == 1);
}

int main() { return testkit::run_all(); }
