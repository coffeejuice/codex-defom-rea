// CP222 migration groundwork: adapter observations, not native access grants.
#include <optional>
#include <type_traits>

#include <defmodel/layout/db_step_data.hpp>
#include <testkit/check.hpp>

#include "m2m/engine.hpp"

using namespace m2m::engine;

static_assert(!std::is_constructible_v<StepIdentity,
    std::shared_ptr<GuiModelLifetime>, std::uint64_t>);
static_assert(!std::is_convertible_v<StepIdentity, DB_STEP_DATA*>);
static_assert(!std::is_convertible_v<StepIdentity, const DB_STEP_DATA*>);
static_assert(!std::is_convertible_v<StepIdentity, std::uintptr_t>);

TEST_CASE(step_identity_constructor_current_and_owner_equality)
{
    const StepIdentity empty;
    CHECK(!empty.isCurrent());
    CHECK(empty == StepIdentity{});
    GuiModel model(nullptr, 3);
    GuiModel other(nullptr, 3);
    const auto identity = model.stepIdentity();
    CHECK(identity.isCurrent());
    CHECK(identity == model.stepIdentity());
    CHECK(identity != empty);
    CHECK(identity != other.stepIdentity());
    CHECK(model.isCurrentStepIdentity(identity));
    CHECK(!other.isCurrentStepIdentity(identity));
    CHECK(!model.isCurrentStepIdentity(empty));
}

TEST_CASE(step_identity_reset_invalidates_even_with_unchanged_native_address)
{
    GuiModel model(nullptr, 3);
    const auto* address = model.step.get();
    const auto before = model.stepIdentity();
    model.step->sim_ctrl.nstep = 47;
    model.state100 = 9;
    model.reset();
    CHECK(model.step.get() == address);
    CHECK_EQ(model.step->sim_ctrl.nstep, 100);
    CHECK_EQ(model.state100, 0);
    CHECK(!before.isCurrent());
    CHECK(!model.isCurrentStepIdentity(before));
    const auto after = model.stepIdentity();
    CHECK(after.isCurrent());
    CHECK(before != after);
    model.reset();
    CHECK(!after.isCurrent());
    CHECK(after != model.stepIdentity());
}

TEST_CASE(step_identity_expired_owner_does_not_keep_model_alive)
{
    StepIdentity identity;
    GuiModelGuard guard;
    {
        GuiModel model(nullptr, 2);
        guard.reset(&model);
        identity = model.stepIdentity();
        CHECK(identity.isCurrent());
    }
    CHECK(guard.get() == nullptr);
    CHECK(!identity.isCurrent());
    // A token retains its weak control-block identity after owner expiry.
    CHECK(identity == identity);
    CHECK(identity != StepIdentity{});
}

TEST_CASE(step_identity_reused_model_address_has_distinct_owner)
{
    std::optional<GuiModel> storage;
    storage.emplace(nullptr, 3);
    const auto* address = &*storage;
    const auto old = storage->stepIdentity();
    storage.reset();
    CHECK(!old.isCurrent());
    storage.emplace(nullptr, 3);
    CHECK(&*storage == address);
    const auto current = storage->stepIdentity();
    CHECK(current.isCurrent());
    CHECK(old != current);
    CHECK(!storage->isCurrentStepIdentity(old));
    CHECK(!old.isCurrent());
}

TEST_CASE(step_identity_legacy_public_step_replacement_is_not_instrumented)
{
    GuiModel model(nullptr, 3);
    const auto before = model.stepIdentity();
    model.step.reset();
    // Deliberate staging limitation, never authority for native access.
    CHECK(before.isCurrent());
    CHECK(model.isCurrentStepIdentity(before));
    model.reset(); // Existing null-step no-op is preserved.
    CHECK(before == model.stepIdentity());
}

int main() { return testkit::run_all(); }
