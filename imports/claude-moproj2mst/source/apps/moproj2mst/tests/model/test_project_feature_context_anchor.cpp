// ctest-labels: m2m-model feature-context ownership
// Actual Project/Session binding only; no native constructor, feature provider or Close completion.
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include "m2m/model.hpp"
#include "../../src/model/project_feature_context_anchor.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/qt4path.hpp"
#include <cstddef>
#include <memory>
#include <new>
#include <string>
#include <type_traits>

using namespace m2m;
using Stage = Project::FeatureContextStage;
using Stop = Project::FeatureContextStop;
using Anchor = Project::FeatureContextAnchor;
namespace {
qt::String database(const char* name)
{
    return qt::String((m2m_test::scratchRoot() / (std::string("feature-context-") + name + ".DB")).u16string());
}
void initialize(Project& project, const qt::String& db)
{ initProject(project, qt::absolutePath(db), qt::completeBaseName(db)); }
}

static_assert(!std::is_default_constructible_v<Anchor>);
static_assert(!std::is_copy_constructible_v<Anchor>);
static_assert(!std::is_copy_assignable_v<Anchor>);
static_assert(!std::is_copy_constructible_v<Project> && !std::is_move_constructible_v<Project>);
static_assert(!Anchor::nativeConstructorCompleted && !Anchor::nativeHeapEffectsCommitted);
static_assert(!Anchor::nativeFeatureValuesAvailable && !Anchor::nativeRegistrationCompleted && !Anchor::completeForPublicClose);

TEST_CASE(project_feature_context_shell_is_passive_and_unprepared_legacy_init_stays_supported)
{
    Project project;
    const auto anchor = project.featureContextAnchor(); REQUIRE(anchor);
    CHECK(anchor->projectIdentity() == &project);
    CHECK(anchor->storageEpochAlive() && anchor->sourceOwnerAlive());
    CHECK(anchor->stage() == Stage::PassiveShell);
    CHECK(anchor->preparedModelIdentity() == nullptr && !anchor->adapterF10AtAttempt().has_value());
    const auto before = project.featureContextAttemptReport();
    CHECK(before.stop == Stop::PreparedMoUnavailable && before.sourceOccurrence == nullptr && before.nextSourceSite == nullptr);
    project.f10 = 77;
    initialize(project, database("legacy"));
    CHECK(project.f10 == 5 && project.subs.size() == 1); // Existing adapter behavior.
    CHECK(project.featureContextAnchor() == anchor && anchor->stage() == Stage::PassiveShell);
    CHECK(!anchor->adapterF10AtAttempt().has_value() && !anchor->projectBacklinkWasNullAtAttempt());
}

TEST_CASE(project_feature_context_logical_attempt_requires_prepared_mo_and_precedes_later_adapter_fields)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    auto& project = session.project(); const auto anchor = project.featureContextAnchor();
    const auto db = database("timing");
    session.prepareProjectModel(db);
    const auto model = project.dataDb; REQUIRE(model);
    CHECK(session.modelManagerOwner()->size() == 1 && model->moProject == nullptr);
    CHECK(project.featureContextAttemptReport().stop == Stop::ConstructorPhaseNotEntered);
    CHECK(anchor->stage() == Stage::PassiveShell && !anchor->adapterF10AtAttempt());
    project.f10 = 77;
    initialize(project, db);
    const auto pending = project.featureContextAttemptReport();
    CHECK(pending.stage == Stage::ProxyAllocationPending && pending.stop == Stop::HeapProfileUnavailable);
    CHECK(std::string(pending.sourceOccurrence) == "0x140089917:0x1412f10d0");
    CHECK(std::string(pending.nextSourceSite) == "0x1412f107b:8-byte proxy allocation");
    REQUIRE(anchor->adapterF10AtAttempt()); CHECK(*anchor->adapterF10AtAttempt() == 77);
    CHECK(project.f10 == 5); // The captured value proves the attempt preceded this legacy write.
    CHECK(anchor->projectBacklinkWasNullAtAttempt() && anchor->preparedModelIdentity() == model.get());
    CHECK(model->moProject == nullptr && session.modelManagerOwner()->size() == 1);
    session.commitProjectModelConstruction();
    CHECK(model->moProject == &project);
    CHECK(project.featureContextAttemptReport().stop == Stop::HeapProfileUnavailable);
    CHECK(!Anchor::nativeConstructorCompleted); // Adapter backlink is not native completion.
    project.f10 = 88;
    initialize(project, db);
    CHECK(project.featureContextAnchor() == anchor);
    CHECK(*anchor->adapterF10AtAttempt() == 77); // Pending occurrence is never restarted.
}

TEST_CASE(project_feature_context_distinct_projects_have_distinct_storage_epochs)
{
    Project first, second;
    const auto a = first.featureContextAnchor(), b = second.featureContextAnchor();
    REQUIRE(a && b);
    CHECK(a != b && a->projectIdentity() != b->projectIdentity());
    CHECK(!a->sharesStorageEpoch(*b) && a->sharesStorageEpoch(*a));
    CHECK(first.modelSlotsOwner()->projectIdentity() == &first);
    CHECK(second.modelSlotsOwner()->projectIdentity() == &second);
}

TEST_CASE(project_feature_context_rejects_base_or_foreign_mo_slots_without_native_attempt)
{
    Project project, foreign;
    project.dataDb = std::make_shared<engine::DbHandle>();
    project.dataDb->modelId = 1; engine::dbSetPath(*project.dataDb, database("base"), false);
    initialize(project, database("base"));
    CHECK(project.featureContextAttemptReport().stop == Stop::PreparedMoUnavailable);
    CHECK(project.featureContextAnchor()->stage() == Stage::PassiveShell);
    auto foreignModel = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    foreignModel->adoptMoModelSlots(foreign.modelSlotsOwner());
    foreignModel->modelId = 1; engine::dbSetPath(*foreignModel, database("foreign"), false);
    project.dataDb = foreignModel;
    initialize(project, database("foreign"));
    CHECK(project.featureContextAnchor()->stage() == Stage::PassiveShell);
    CHECK(project.featureContextAttemptReport().stop == Stop::PreparedMoUnavailable);
    CHECK(project.dataDb == foreignModel && !project.featureContextAnchor()->adapterF10AtAttempt());
}

TEST_CASE(project_feature_context_same_pointer_different_control_block_cannot_rebind_pending_attempt)
{
    engine::DiagramEffectDriver driver; engine::ConversionSession session(driver);
    const auto db = database("identity"); session.prepareProjectModel(db);
    auto& project = session.project(); initialize(project, db);
    const auto anchor = project.featureContextAnchor(); const auto original = project.dataDb;
    project.dataDb = std::shared_ptr<engine::DbHandle>(original.get(), [](engine::DbHandle*) {});
    CHECK(project.dataDb.get() == original.get());
    CHECK(project.featureContextAttemptReport().stop == Stop::OwnerBindingChanged);
    initialize(project, db); // Observed mismatch consumes no native effects and permanently refuses rebinding.
    project.dataDb = original;
    CHECK(project.featureContextAttemptReport().stop == Stop::OwnerBindingChanged);
    CHECK(anchor->preparedModelIdentity() == original.get() && anchor->stage() == Stage::ProxyAllocationPending);
    CHECK(session.modelManagerOwner()->size() == 1 && original->moProject == nullptr);
}

TEST_CASE(project_feature_context_does_not_start_constructor_after_an_early_adapter_backlink_commit)
{
    engine::DiagramEffectDriver driver; engine::ConversionSession session(driver);
    const auto db = database("early-commit"); session.prepareProjectModel(db);
    session.commitProjectModelConstruction(); // Deliberately invalid source ordering in a declared adapter fixture.
    auto& project = session.project(); initialize(project, db);
    CHECK(project.dataDb->moProject == &project);
    CHECK(project.featureContextAnchor()->stage() == Stage::PassiveShell);
    CHECK(project.featureContextAttemptReport().stop == Stop::PreparedMoUnavailable);
    CHECK(!project.featureContextAnchor()->adapterF10AtAttempt());
}

TEST_CASE(project_feature_context_metadata_cannot_extend_actual_project_storage_lifetime)
{
    engine::DiagramEffectDriver driver; engine::ConversionSession session(driver);
    const auto db = database("release"); session.prepareProjectModel(db);
    auto& project = session.project(); initialize(project, db); session.commitProjectModelConstruction();
    const auto anchor = project.featureContextAnchor(); const auto slots = project.modelSlotsOwner();
    const auto child = project.dataDb; const auto* identity = &project;
    REQUIRE(anchor->sourceOwnerAlive() && slots->projectStorageAlive());
    session.commitNullDiagramBinding(); session.releaseProjectAfterCloseEffects(); // Adapter storage only.
    CHECK(session.liveProject() == nullptr && child->moProject == nullptr);
    CHECK(!anchor->sourceOwnerAlive() && !anchor->storageEpochAlive());
    CHECK(!slots->projectStorageAlive() && slots->nextStorageBoundary() == engine::MoModelSlots::Slot::Operations);
    CHECK(anchor->projectIdentity() == identity && anchor->preparedModelIdentity() == child.get()); // Compare only.
    CHECK(anchor->stage() == Stage::ProxyAllocationPending); // Expiry did not fake completion/reset.
    CHECK(session.modelManagerOwner()->size() == 1 && child->moModelSlotsOwner() == slots);
}

TEST_CASE(project_feature_context_address_reuse_does_not_reuse_the_expired_epoch)
{
    alignas(Project) std::byte storage[sizeof(Project)];
    auto destroy = [](Project* value) { value->~Project(); };
    std::unique_ptr<Project, decltype(destroy)> first(new (storage) Project(), destroy);
    const auto old = first->featureContextAnchor(); const auto* address = first.get();
    first.reset(); CHECK(!old->sourceOwnerAlive() && !old->storageEpochAlive());
    std::unique_ptr<Project, decltype(destroy)> second(new (storage) Project(), destroy);
    const auto fresh = second->featureContextAnchor();
    CHECK(second.get() == address && old->projectIdentity() == fresh->projectIdentity());
    CHECK(fresh != old && !fresh->sharesStorageEpoch(*old));
    CHECK(fresh->sourceOwnerAlive() && !old->sourceOwnerAlive());
    CHECK(fresh->stage() == Stage::PassiveShell && !fresh->adapterF10AtAttempt());
}

int main() { return testkit::run_all(); }
