// ctest-labels: m2m-model feature-context ownership
// Passive host epoch ownership; no original CRT heap or native Close execution.
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include "../../src/model/project_feature_context_anchor.hpp"
#include "../../src/model/project_feature_storage.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/qt4path.hpp"
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <new>
#include <type_traits>

namespace allocation_probe {
// Only factory calls are armed. Testkit and other model allocations are normal.
bool armed = false;
std::size_t calls = 0;
std::size_t failAt = 0;
void* first = nullptr;
std::size_t firstDeletes = 0;
void begin(std::size_t failure) noexcept
{
    calls = 0; failAt = failure; first = nullptr; firstDeletes = 0; armed = true;
}
struct Armed final {
    explicit Armed(std::size_t failure) noexcept { begin(failure); }
    ~Armed() { armed = false; }
};
}

void* operator new(std::size_t bytes)
{
    using namespace allocation_probe;
    if (armed && ++calls == failAt) throw std::bad_alloc();
    if (void* value = std::malloc(bytes ? bytes : 1)) {
        if (armed && calls == 1) first = value;
        return value;
    }
    throw std::bad_alloc();
}
void operator delete(void* value) noexcept
{
    if (value && value == allocation_probe::first) {
        ++allocation_probe::firstDeletes;
        allocation_probe::first = nullptr;
    }
    std::free(value);
}
void operator delete(void* value, std::size_t) noexcept { ::operator delete(value); }

using namespace m2m;
using Epoch = engine::ProjectStorageLifetime;
using Access = detail::FeatureContextAccess;
static_assert(std::is_empty_v<Epoch> && std::is_final_v<Epoch>);
static_assert(!Project::FeatureContextAnchor::nativeHeapEffectsCommitted);
static_assert(!Project::FeatureContextAnchor::completeForPublicClose);

TEST_CASE(feature_epoch_last_strong_owner_deletes_bundle_once_while_weak_control_block_survives)
{
    std::shared_ptr<const Epoch> epoch;
    {
        allocation_probe::Armed probe(0);
        epoch = detail::makeFeatureEpoch();
    }
    REQUIRE(epoch);
    REQUIRE(detail::featureEpoch(epoch));
    auto copy = epoch;
    CHECK(detail::featureEpoch(copy) == detail::featureEpoch(epoch));
    CHECK(!copy.owner_before(epoch) && !epoch.owner_before(copy));
    std::weak_ptr<const Epoch> weak = epoch;
    epoch.reset();
    CHECK(!weak.expired() && allocation_probe::firstDeletes == 0);
    copy.reset();
    CHECK(weak.expired() && allocation_probe::firstDeletes == 1);
    CHECK(allocation_probe::calls == 2); // Separate host bundle and control block.
    weak.reset();
    CHECK(allocation_probe::firstDeletes == 1);
}

TEST_CASE(feature_epoch_rejects_foreign_and_wrong_marker_aliases)
{
    CHECK(detail::featureEpoch({}) == nullptr);
    const auto foreign = std::make_shared<const Epoch>();
    CHECK(detail::featureEpoch(foreign) == nullptr);
    const auto first = detail::makeFeatureEpoch();
    const auto second = detail::makeFeatureEpoch();
    CHECK(detail::featureEpoch(first) != detail::featureEpoch(second));
    CHECK(first.get() != second.get());
    const std::shared_ptr<const Epoch> wrongMarker(first, second.get());
    CHECK(detail::featureEpoch(wrongMarker) == nullptr);
    const std::shared_ptr<const Epoch> nullAlias(first, nullptr);
    CHECK(detail::featureEpoch(nullAlias) == nullptr);
    CHECK(detail::featureEpoch(first) != nullptr); // Rejection does not consume its real owner.
}

namespace {
void checkHostFailure(std::size_t failure)
{
    bool caught = false;
    try {
        allocation_probe::Armed probe(failure);
        const auto epoch = detail::makeFeatureEpoch();
    }
    catch (const std::bad_alloc&) { caught = true; }
    CHECK(caught);
    CHECK(allocation_probe::calls == failure);
    CHECK(allocation_probe::firstDeletes == (failure == 2 ? 1u : 0u));
    CHECK(allocation_probe::first == nullptr);
    // The next unrelated construction has its own valid epoch, with no retry
    // of a native allocation or resurrection of the failed host storage.
    const auto next = detail::makeFeatureEpoch();
    CHECK(detail::featureEpoch(next) != nullptr);
}
}

TEST_CASE(feature_epoch_bundle_host_allocation_failure_has_no_owner)
{ checkHostFailure(1); }

TEST_CASE(feature_epoch_control_block_failure_destroys_host_bundle_once)
{ checkHostFailure(2); }

TEST_CASE(actual_project_shell_uses_typed_epoch_without_starting_native_constructor)
{
    std::shared_ptr<const Project::FeatureContextAnchor> escaped;
    {
        Project project;
        escaped = project.featureContextAnchor(); REQUIRE(escaped);
        CHECK(Access::hasTypedEpoch(*escaped));
        CHECK(escaped->sourceOwnerAlive() && escaped->storageEpochAlive());
        CHECK(escaped->stage() == Project::FeatureContextStage::PassiveShell);
        CHECK(escaped->preparedModelIdentity() == nullptr && !escaped->adapterF10AtAttempt());
        CHECK(project.featureContextAttemptReport().stop == Project::FeatureContextStop::PreparedMoUnavailable);
    }
    CHECK(!escaped->sourceOwnerAlive() && !escaped->storageEpochAlive());
    CHECK(!Access::hasTypedEpoch(*escaped));
}

TEST_CASE(actual_session_epoch_expires_before_registered_mo_slots_and_escaped_anchor)
{
    engine::DiagramEffectDriver driver;
    std::shared_ptr<const Project::FeatureContextAnchor> anchor;
    std::shared_ptr<engine::MoModelSlots> slots;
    std::shared_ptr<engine::DbHandle> child;
    std::shared_ptr<engine::ModelManager> manager;
    {
        engine::ConversionSession session(driver);
        const qt::String db((m2m_test::scratchRoot() / "feature-storage-epoch.DB").u16string());
        session.prepareProjectModel(db);
        auto& project = session.project();
        anchor = project.featureContextAnchor();
        slots = project.modelSlotsOwner();
        child = project.dataDb; REQUIRE(child);
        manager = session.modelManagerOwner(); REQUIRE(manager);
        REQUIRE(Access::hasTypedEpoch(*anchor));
        initProject(project, qt::absolutePath(db), qt::completeBaseName(db));
        session.commitProjectModelConstruction();
        CHECK(anchor->stage() == Project::FeatureContextStage::ProxyAllocationPending);
        CHECK(anchor->preparedModelIdentity() == child.get() && child->moProject == &project);
    } // Actual Session destruction releases adapter storage; not native Close.
    CHECK(child->moProject == nullptr);
    CHECK(!Access::hasTypedEpoch(*anchor));
    CHECK(!anchor->sourceOwnerAlive() && !anchor->storageEpochAlive());
    CHECK(!slots->projectStorageAlive());
    CHECK(slots->nextStorageBoundary() == engine::MoModelSlots::Slot::Operations);
    CHECK(manager->size() == 1 && child->moModelSlotsOwner() == slots);
    CHECK(anchor->stage() == Project::FeatureContextStage::ProxyAllocationPending);
}

TEST_CASE(actual_project_address_reuse_does_not_rebind_old_typed_epoch)
{
    alignas(Project) std::byte storage[sizeof(Project)];
    auto destroy = [](Project* value) { value->~Project(); };
    std::unique_ptr<Project, decltype(destroy)> first(new (storage) Project(), destroy);
    const auto old = first->featureContextAnchor();
    REQUIRE(Access::hasTypedEpoch(*old));
    first.reset();
    CHECK(!Access::hasTypedEpoch(*old) && !old->storageEpochAlive());
    std::unique_ptr<Project, decltype(destroy)> second(new (storage) Project(), destroy);
    const auto fresh = second->featureContextAnchor();
    CHECK(old->projectIdentity() == fresh->projectIdentity()); // Identity comparison only.
    CHECK(fresh != old && !fresh->sharesStorageEpoch(*old));
    CHECK(Access::hasTypedEpoch(*fresh) && !Access::hasTypedEpoch(*old));
}

int main() { return testkit::run_all(); }
