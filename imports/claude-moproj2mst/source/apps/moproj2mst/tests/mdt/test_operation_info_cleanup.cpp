// ctest-labels: m2m-mdt operation-info-cleanup
#include <testkit/check.hpp>

#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/operation_info_cleanup.hpp"
#include "m2m/engine/operation_info_model_cache.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

using namespace m2m;
using engine::OperationInfoCleanupOutcome;
using engine::OperationInfoCleanupProgress;
using engine::OperationInfoCleanupStage;
using engine::OperationInfoExpiry;
using engine::attemptOperationInfoPropsCleanup;
using engine::attemptOperationInfoEngineCacheCleanup;
using engine::attemptOperationInfoObjectListCleanup;
using engine::attemptOperationInfoPositionListCleanup;
using engine::attemptOperationInfoRecoveryWrapperCleanup;
using engine::attemptOperationInfoRecoveryFlagsCleanup;
namespace {
template<class F> bool refuses(F&& body, const char* detail)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED && std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}

struct CleanupProbe final : Props {
    explicit CleanupProbe(std::function<void()> callback) : callback_(std::move(callback)) {}
    ~CleanupProbe() override { callback_(); }
private:
    std::function<void()> callback_;
};

struct BorrowedClass final : mst::OpClass {
    explicit BorrowedClass(engine::GuiModel& model) : OpClass(qt::L("BorrowedCacheFixture")), model_(&model) {}
    engine::GuiModel* classModel(Project&, Operation&) override { ++lookups; return model_; }
    std::shared_ptr<engine::GuiModel> createModel(Project&, Operation&) override { ++creates; return {}; }
    int lookups = 0, creates = 0;
private:
    engine::GuiModel* model_;
};

struct TwoModelOwner final {
    engine::GuiModel first{nullptr, 3};
    engine::GuiModel second{nullptr, 3};
};
}

static_assert(!std::is_copy_constructible_v<OperationInfoCleanupProgress>);
static_assert(!std::is_move_constructible_v<OperationInfoCleanupProgress>);

TEST_CASE(opi_cleanup_null_props_advances_only_to_guarded_engine_model_boundary)
{
    Operation operation;
    mst::OpClass cls(qt::L("NullProps"));
    cls.current = &operation;
    auto recovery = operation.recoveryB;
    OperationInfoCleanupProgress progress(operation);
    CHECK(progress.stage() == OperationInfoCleanupStage::InspectProps);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(progress.stage() == OperationInfoCleanupStage::EngineModelBoundary);
    CHECK(progress.observedPropsIdentity() == nullptr);
    CHECK(operation.recoveryB == recovery);
    CHECK(cls.current == &operation);
    CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::EngineModelCleanupUnavailable);
    CHECK(cls.current == &operation);
}

TEST_CASE(opi_cleanup_nonnull_virtual_payload_is_guarded_without_reset_or_extra_ownership)
{
    bool payloadDestroyed = false;
    Operation operation;
    operation.props = std::make_shared<CleanupProbe>([&] { payloadDestroyed = true; });
    mst::OpClass cls(qt::L("NonnullProps"));
    cls.current = &operation;
    const auto* actualProps = operation.props.get();
    const auto ownershipCount = operation.props.use_count();
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NonNullPropsVirtualDeleteUnavailable);
    CHECK(progress.stage() == OperationInfoCleanupStage::PropsVirtualDeleteBlocked);
    CHECK(progress.observedPropsIdentity() == actualProps);
    CHECK(operation.props.get() == actualProps);
    CHECK(operation.props.use_count() == ownershipCount);
    CHECK(!payloadDestroyed);
    CHECK(cls.current == &operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NonNullPropsVirtualDeleteUnavailable);
    CHECK(!payloadDestroyed);
    CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
}

TEST_CASE(opi_cleanup_does_not_reinterpret_externally_deleted_props_as_original_null_skip)
{
    Operation operation;
    operation.props = std::make_shared<Props>();
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NonNullPropsVirtualDeleteUnavailable);
    operation.props.reset(); // external fixture mutation, not helper execution
    CHECK(refuses([&] { attemptOperationInfoPropsCleanup(operation, progress); }, "Props changed"));
    CHECK(progress.stage() == OperationInfoCleanupStage::PropsVirtualDeleteBlocked);
    CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
}

TEST_CASE(opi_cleanup_rejects_props_appearance_after_completed_null_branch)
{
    Operation operation;
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    operation.props = std::make_shared<Props>();
    CHECK(refuses([&] { attemptOperationInfoPropsCleanup(operation, progress); }, "Props appeared"));
    CHECK(progress.stage() == OperationInfoCleanupStage::EngineModelBoundary);
    CHECK(operation.props != nullptr);
}

TEST_CASE(opi_cleanup_progress_cannot_be_used_for_another_live_operation)
{
    Operation first, second;
    OperationInfoCleanupProgress progress(first);
    CHECK(refuses([&] { attemptOperationInfoPropsCleanup(second, progress); }, "does not bind"));
    CHECK(progress.stage() == OperationInfoCleanupStage::InspectProps);
    CHECK(progress.identitySnapshot().get() == &first);
}

TEST_CASE(opi_cleanup_never_commits_qobject_expiry_and_rejects_an_explicitly_expired_identity)
{
    Operation operation;
    mst::OpClass cls(qt::L("ExplicitExpiry"));
    cls.current = &operation;
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(cls.current == &operation);
    CHECK(operation.commitOperationInfoQObjectBoundary("isolated external expiry fixture; no native acceptance"));
    CHECK(cls.current == nullptr);
    CHECK(refuses([&] { attemptOperationInfoPropsCleanup(operation, progress); }, "does not bind"));
    CHECK(refuses([&] { OperationInfoCleanupProgress expired(operation); }, "requires a live"));
    CHECK(progress.stage() == OperationInfoCleanupStage::EngineModelBoundary);
    CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::QObjectBoundary);
}

TEST_CASE(opi_cleanup_progress_does_not_retain_target_or_props_allocation)
{
    bool payloadDestroyed = false;
    auto operation = std::make_unique<Operation>();
    operation->props = std::make_shared<CleanupProbe>([&] { payloadDestroyed = true; });
    auto progress = std::make_unique<OperationInfoCleanupProgress>(*operation);
    CHECK(attemptOperationInfoPropsCleanup(*operation, *progress) ==
          OperationInfoCleanupOutcome::NonNullPropsVirtualDeleteUnavailable);
    operation.reset();
    CHECK(payloadDestroyed);
    CHECK(progress->identitySnapshot().get() == nullptr);
    CHECK(progress->identitySnapshot().expiry() == OperationInfoExpiry::AdapterIdentityLifetime);
    Operation later;
    CHECK(refuses([&] { attemptOperationInfoPropsCleanup(later, *progress); }, "does not bind"));
}

TEST_CASE(opi_cache_cleanup_requires_the_completed_props_null_step)
{
    Operation operation;
    OperationInfoCleanupProgress progress(operation);
    CHECK(refuses([&] { attemptOperationInfoEngineCacheCleanup(operation, progress); }, "completed Props-null"));
    CHECK(progress.stage() == OperationInfoCleanupStage::InspectProps);
    operation.props = std::make_shared<Props>();
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NonNullPropsVirtualDeleteUnavailable);
    CHECK(refuses([&] { attemptOperationInfoEngineCacheCleanup(operation, progress); }, "completed Props-null"));
    CHECK(progress.stage() == OperationInfoCleanupStage::PropsVirtualDeleteBlocked);
    CHECK(!operation.engineModel);
    CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
}

TEST_CASE(opi_cache_null_skip_stops_before_object_cleanup_and_keeps_current_live)
{
    Operation operation;
    operation.objs.push_back(std::make_unique<ObjEntry>());
    const auto* object = operation.objs.front().get();
    const auto recovery = operation.recoveryB;
    mst::OpClass cls(qt::L("NullOwnedCache")); cls.current = &operation;
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK(progress.stage() == OperationInfoCleanupStage::ObjectCleanupBoundary);
    CHECK_EQ(progress.observedEngineCacheIdentityToken(), std::uintptr_t{0});
    CHECK(!operation.engineModel);
    REQUIRE(operation.objs.size() == 1);
    CHECK(operation.objs.front().get() == object);
    CHECK(operation.recoveryB == recovery);
    CHECK(cls.current == &operation);
    CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::ObjectCleanupUnavailable);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::ObjectCleanupUnavailable);
    CHECK(operation.objs.front().get() == object);
}

TEST_CASE(opi_cache_nonnull_refuses_without_retaining_or_resetting_the_model)
{
    Operation operation;
    operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    std::weak_ptr<engine::GuiModel> model = operation.engineModel;
    const auto* actual = operation.engineModel.get();
    const auto owners = operation.engineModel.use_count();
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NonNullEngineCacheVirtualDeleteUnavailable);
    CHECK(progress.stage() == OperationInfoCleanupStage::EngineModelVirtualDeleteBlocked);
    CHECK(operation.engineModel.get() == actual);
    CHECK_EQ(operation.engineModel.use_count(), owners);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NonNullEngineCacheVirtualDeleteUnavailable);
    CHECK_EQ(operation.engineModel.use_count(), owners);
    operation.engineModel.reset(); // external fixture mutation, NOT native delete
    CHECK(model.expired());
    CHECK(refuses([&] { attemptOperationInfoEngineCacheCleanup(operation, progress); }, "Engine cache changed"));
    CHECK(refuses([&] { attemptOperationInfoPropsCleanup(operation, progress); }, "Engine cache changed"));
    CHECK(progress.stage() == OperationInfoCleanupStage::EngineModelVirtualDeleteBlocked);
    CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
}

TEST_CASE(opi_cache_null_skip_rejects_later_cache_or_props_appearance)
{
    Operation operation;
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    CHECK(refuses([&] { attemptOperationInfoEngineCacheCleanup(operation, progress); }, "Engine cache appeared"));
    CHECK(progress.stage() == OperationInfoCleanupStage::ObjectCleanupBoundary);
    operation.engineModel.reset(); // restore this declared fixture's null value
    operation.props = std::make_shared<Props>();
    CHECK(refuses([&] { attemptOperationInfoEngineCacheCleanup(operation, progress); }, "Props appeared"));
    CHECK(progress.stage() == OperationInfoCleanupStage::ObjectCleanupBoundary);
}

TEST_CASE(opi_cache_continuation_rejects_foreign_or_expired_opi_before_cache_read)
{
    Operation operation, other;
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(refuses([&] { attemptOperationInfoEngineCacheCleanup(other, progress); }, "does not bind"));
    CHECK(operation.commitOperationInfoQObjectBoundary("declared cache fixture expiry; not native cleanup"));
    CHECK(refuses([&] { attemptOperationInfoEngineCacheCleanup(operation, progress); }, "does not bind"));
    CHECK(progress.stage() == OperationInfoCleanupStage::EngineModelBoundary);
}

TEST_CASE(opi_cache_cleanup_uses_owned_slot_without_querying_a_borrowed_class_model)
{
    Project project; Operation operation;
    auto borrowed = std::make_shared<engine::GuiModel>(nullptr, 3);
    auto ownedClass = std::make_unique<BorrowedClass>(*borrowed);
    auto* const cls = ownedClass.get();
    const auto key = qt::L("BorrowedCacheFixture");
    project.registry.emplace(key, std::move(ownedClass));
    operation.project = &project; operation.classKey = key; operation.opClass = cls;
    cls->current = &operation;
    CHECK(engine::opModel(project, operation) == borrowed.get());
    CHECK(!operation.engineModel);
    CHECK_EQ(cls->lookups, 1); CHECK_EQ(cls->creates, 0);
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK_EQ(cls->lookups, 1); CHECK_EQ(cls->creates, 0);
    CHECK(cls->current == &operation);
    CHECK(borrowed != nullptr);
    CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
}

TEST_CASE(opi_cache_blocked_retry_rejects_another_control_block_at_the_same_address)
{
    Operation operation;
    const auto original = std::make_shared<engine::GuiModel>(nullptr, 3);
    const std::weak_ptr<engine::GuiModel> observedOwner = original;
    operation.engineModel = original;
    CHECK(operation.sameEngineModelOwner(observedOwner));
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NonNullEngineCacheVirtualDeleteUnavailable);
    operation.engineModel = std::shared_ptr<engine::GuiModel>(original.get(), [](engine::GuiModel*) {});
    CHECK(operation.engineModel.get() == original.get());
    CHECK(!operation.sameEngineModelOwner(observedOwner));
    CHECK(refuses([&] { attemptOperationInfoEngineCacheCleanup(operation, progress); }, "Engine cache changed"));
    operation.engineModel.reset(); // discard only this no-op-deleter fixture alias
    CHECK(progress.stage() == OperationInfoCleanupStage::EngineModelVirtualDeleteBlocked);
}

TEST_CASE(opi_cache_blocked_retry_rejects_another_address_before_same_owner_comparison)
{
    auto owner = std::make_shared<TwoModelOwner>();
    const std::shared_ptr<engine::GuiModel> first(owner, &owner->first);
    const std::shared_ptr<engine::GuiModel> second(owner, &owner->second);
    const std::weak_ptr<engine::GuiModel> observedOwner = first;
    Operation operation;
    operation.engineModel = first;
    OperationInfoCleanupProgress progress(operation);
    REQUIRE(attemptOperationInfoPropsCleanup(operation, progress) ==
            OperationInfoCleanupOutcome::NullPropsSkipReached);
    REQUIRE(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
            OperationInfoCleanupOutcome::NonNullEngineCacheVirtualDeleteUnavailable);

    operation.engineModel = second;
    CHECK(operation.engineModel.get() != first.get());
    CHECK(operation.sameEngineModelOwner(observedOwner)); // same control block, different pointee address
    CHECK(refuses([&] { attemptOperationInfoEngineCacheCleanup(operation, progress); },
                  "Engine cache changed"));
    CHECK(progress.stage() == OperationInfoCleanupStage::EngineModelVirtualDeleteBlocked);
}

TEST_CASE(opi_cache_blocked_retry_rejects_replacement_after_observed_weak_owner_expires)
{
    engine::GuiModel model(nullptr, 3);
    auto original = std::shared_ptr<engine::GuiModel>(&model, [](engine::GuiModel*) {});
    const std::weak_ptr<engine::GuiModel> observedOwner = original;
    Operation operation;
    operation.engineModel = original;
    OperationInfoCleanupProgress progress(operation);
    REQUIRE(attemptOperationInfoPropsCleanup(operation, progress) ==
            OperationInfoCleanupOutcome::NullPropsSkipReached);
    REQUIRE(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
            OperationInfoCleanupOutcome::NonNullEngineCacheVirtualDeleteUnavailable);

    operation.engineModel.reset();
    original.reset();
    REQUIRE(observedOwner.expired());
    operation.engineModel = std::shared_ptr<engine::GuiModel>(&model, [](engine::GuiModel*) {});
    CHECK_EQ(operation.engineModel.get(), &model); // the stored address token still matches
    CHECK(!operation.sameEngineModelOwner(observedOwner));
    CHECK(refuses([&] { attemptOperationInfoEngineCacheCleanup(operation, progress); },
                  "Engine cache changed"));
    CHECK(progress.stage() == OperationInfoCleanupStage::EngineModelVirtualDeleteBlocked);
}

TEST_CASE(opi_objects_require_the_explicit_props_and_cache_null_steps)
{
    Operation operation;
    OperationInfoCleanupProgress progress(operation);
    CHECK(refuses([&] { attemptOperationInfoObjectListCleanup(operation, progress); }, "Props/cache-null"));
    CHECK(progress.stage() == OperationInfoCleanupStage::InspectProps);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(refuses([&] { attemptOperationInfoObjectListCleanup(operation, progress); }, "Props/cache-null"));
    CHECK(progress.stage() == OperationInfoCleanupStage::EngineModelBoundary);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::ObjectCleanupUnavailable);
    CHECK(progress.stage() == OperationInfoCleanupStage::ObjectCleanupBoundary);
}

TEST_CASE(opi_empty_object_list_advances_only_to_position_cleanup)
{
    Operation operation;
    operation.objs.reserve(4);
    const auto* const objectStorage = operation.objs.data();
    const auto objectCapacity = operation.objs.capacity();
    operation.objpos.push_back(nullptr);
    const auto recovery = operation.recoveryB;
    mst::OpClass cls(qt::L("EmptyObjects")); cls.current = &operation;
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::EmptyObjectsSkipReached);
    CHECK(progress.stage() == OperationInfoCleanupStage::PositionCleanupBoundary);
    CHECK_EQ(progress.observedObjectCount(), std::size_t{0});
    CHECK(operation.objs.empty()); CHECK_EQ(operation.objpos.size(), std::size_t{1});
    CHECK(operation.objs.data() == objectStorage);
    CHECK_EQ(operation.objs.capacity(), objectCapacity);
    CHECK(operation.recoveryB == recovery);
    CHECK(cls.current == &operation);
    CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::PositionCleanupUnavailable);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::PositionCleanupUnavailable);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::PositionCleanupUnavailable);
    CHECK_EQ(operation.objpos.size(), std::size_t{1});
}

TEST_CASE(opi_nonempty_object_list_including_null_rows_stays_before_element_effects)
{
    for (const bool nullRow : {false, true}) {
        Operation operation;
        if (nullRow) operation.objs.push_back(nullptr);
        else operation.objs.push_back(std::make_unique<ObjEntry>());
        const auto* const first = operation.objs.front().get();
        OperationInfoCleanupProgress progress(operation);
        CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
        CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
              OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
        CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
              OperationInfoCleanupOutcome::NonemptyObjectsEffectsUnavailable);
        CHECK(progress.stage() == OperationInfoCleanupStage::ObjectListEffectsBlocked);
        CHECK_EQ(progress.observedObjectCount(), std::size_t{1});
        REQUIRE(operation.objs.size() == 1);
        CHECK(operation.objs.front().get() == first);
        CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
              OperationInfoCleanupOutcome::NonemptyObjectsEffectsUnavailable);
        operation.objs.clear(); // external fixture mutation, not native removal
        CHECK(refuses([&] { attemptOperationInfoObjectListCleanup(operation, progress); }, "Object list changed"));
        CHECK(refuses([&] { attemptOperationInfoPropsCleanup(operation, progress); }, "Object list changed"));
        CHECK(progress.stage() == OperationInfoCleanupStage::ObjectListEffectsBlocked);
        CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    }
}

TEST_CASE(opi_objects_retry_rejects_objects_or_prior_payload_appearance)
{
    Operation operation;
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::EmptyObjectsSkipReached);
    operation.objs.push_back(nullptr);
    CHECK(refuses([&] { attemptOperationInfoObjectListCleanup(operation, progress); }, "Objects appeared"));
    CHECK(progress.stage() == OperationInfoCleanupStage::PositionCleanupBoundary);
    operation.objs.clear();
    operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    CHECK(refuses([&] { attemptOperationInfoObjectListCleanup(operation, progress); }, "Engine cache appeared"));
    operation.engineModel.reset();
    operation.props = std::make_shared<Props>();
    CHECK(refuses([&] { attemptOperationInfoObjectListCleanup(operation, progress); }, "Props appeared"));
    CHECK(progress.stage() == OperationInfoCleanupStage::PositionCleanupBoundary);
}

TEST_CASE(opi_objects_rejects_foreign_and_expired_identity_before_list_read)
{
    Operation operation, other;
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK(refuses([&] { attemptOperationInfoObjectListCleanup(other, progress); }, "does not bind"));
    CHECK(operation.commitOperationInfoQObjectBoundary("declared object-prefix fixture expiry; not native cleanup"));
    CHECK(refuses([&] { attemptOperationInfoObjectListCleanup(operation, progress); }, "does not bind"));
    CHECK(progress.stage() == OperationInfoCleanupStage::ObjectCleanupBoundary);
}

TEST_CASE(opi_positions_require_all_explicit_earlier_null_and_empty_steps)
{
    Operation operation;
    OperationInfoCleanupProgress progress(operation);
    CHECK(refuses([&] { attemptOperationInfoPositionListCleanup(operation, progress); }, "empty-object steps"));
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK(refuses([&] { attemptOperationInfoPositionListCleanup(operation, progress); }, "empty-object steps"));
    CHECK(progress.stage() == OperationInfoCleanupStage::ObjectCleanupBoundary);
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::EmptyObjectsSkipReached);
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::PositionCleanupUnavailable);
    CHECK(progress.stage() == OperationInfoCleanupStage::PositionCleanupBoundary);
}

TEST_CASE(opi_empty_positions_preserve_storage_recovery_and_live_current)
{
    Operation operation;
    operation.objpos.reserve(4);
    const auto* const positionStorage = operation.objpos.data();
    const auto capacity = operation.objpos.capacity();
    const auto recovery = operation.recoveryB;
    operation.recoveryFlags = std::make_shared<std::vector<bool>>(2, true);
    const auto flags = operation.recoveryFlags;
    mst::OpClass cls(qt::L("EmptyPositions")); cls.current = &operation;
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::EmptyObjectsSkipReached);
    CHECK(attemptOperationInfoPositionListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::EmptyPositionsSkipReached);
    CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryCleanupBoundary);
    CHECK_EQ(progress.observedPositionCount(), std::size_t{0});
    CHECK(operation.objpos.data() == positionStorage); CHECK_EQ(operation.objpos.capacity(), capacity);
    CHECK(operation.recoveryB == recovery); CHECK(operation.recoveryFlags == flags);
    CHECK_EQ(flags->size(), std::size_t{2});
    CHECK(cls.current == &operation);
    CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    CHECK(attemptOperationInfoPositionListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::RecoveryCleanupUnavailable);
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::RecoveryCleanupUnavailable);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::RecoveryCleanupUnavailable);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::RecoveryCleanupUnavailable);
    CHECK(operation.recoveryB == recovery); CHECK(operation.recoveryFlags == flags);
}

TEST_CASE(opi_nonempty_positions_and_null_rows_remain_before_removal_effects)
{
    for (const bool nullRow : {false, true}) {
        Operation operation;
        if (nullRow) operation.objpos.push_back(nullptr);
        else operation.objpos.push_back(std::make_unique<ObjPos>());
        const auto* const first = operation.objpos.front().get();
        OperationInfoCleanupProgress progress(operation);
        CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
        CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
              OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
        CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
              OperationInfoCleanupOutcome::EmptyObjectsSkipReached);
        CHECK(attemptOperationInfoPositionListCleanup(operation, progress) ==
              OperationInfoCleanupOutcome::NonemptyPositionsEffectsUnavailable);
        CHECK(progress.stage() == OperationInfoCleanupStage::PositionListEffectsBlocked);
        CHECK_EQ(progress.observedPositionCount(), std::size_t{1});
        REQUIRE(operation.objpos.size() == 1);
        CHECK(operation.objpos.front().get() == first);
        CHECK(attemptOperationInfoPositionListCleanup(operation, progress) ==
              OperationInfoCleanupOutcome::NonemptyPositionsEffectsUnavailable);
        operation.objpos.clear(); // external fixture mutation, not native removal
        CHECK(refuses([&] { attemptOperationInfoPositionListCleanup(operation, progress); }, "Position list changed"));
        CHECK(refuses([&] { attemptOperationInfoPropsCleanup(operation, progress); }, "Position list changed"));
        CHECK(progress.stage() == OperationInfoCleanupStage::PositionListEffectsBlocked);
        CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    }
}

TEST_CASE(opi_positions_retry_rejects_new_positions_and_earlier_payloads)
{
    Operation operation;
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::EmptyObjectsSkipReached);
    CHECK(attemptOperationInfoPositionListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::EmptyPositionsSkipReached);
    operation.objpos.push_back(nullptr);
    CHECK(refuses([&] { attemptOperationInfoPositionListCleanup(operation, progress); }, "Positions appeared"));
    operation.objpos.clear(); operation.objs.push_back(nullptr);
    CHECK(refuses([&] { attemptOperationInfoPositionListCleanup(operation, progress); }, "Objects appeared"));
    operation.objs.clear(); operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    CHECK(refuses([&] { attemptOperationInfoPositionListCleanup(operation, progress); }, "Engine cache appeared"));
    operation.engineModel.reset(); operation.props = std::make_shared<Props>();
    CHECK(refuses([&] { attemptOperationInfoPositionListCleanup(operation, progress); }, "Props appeared"));
    CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryCleanupBoundary);
}

TEST_CASE(opi_positions_rejects_foreign_and_expired_opi_before_list_read)
{
    Operation operation, other;
    OperationInfoCleanupProgress progress(operation);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) ==
          OperationInfoCleanupOutcome::EmptyObjectsSkipReached);
    CHECK(refuses([&] { attemptOperationInfoPositionListCleanup(other, progress); }, "does not bind"));
    CHECK(operation.commitOperationInfoQObjectBoundary("declared position-prefix expiry; not native cleanup"));
    CHECK(refuses([&] { attemptOperationInfoPositionListCleanup(operation, progress); }, "does not bind"));
    CHECK(progress.stage() == OperationInfoCleanupStage::PositionCleanupBoundary);
}

namespace {
void reachRecoveryBoundary(Operation& operation, OperationInfoCleanupProgress& progress)
{
    REQUIRE(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    REQUIRE(attemptOperationInfoEngineCacheCleanup(operation, progress) == OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    REQUIRE(attemptOperationInfoObjectListCleanup(operation, progress) == OperationInfoCleanupOutcome::EmptyObjectsSkipReached);
    REQUIRE(attemptOperationInfoPositionListCleanup(operation, progress) == OperationInfoCleanupOutcome::EmptyPositionsSkipReached);
}
}

TEST_CASE(opi_recovery_requires_all_explicit_predecessors_even_for_null_wrapper)
{
    Operation operation;
    operation.recoveryB.reset(); // declared conditional fixture; ordinary construction allocates B
    OperationInfoCleanupProgress progress(operation);
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "all explicit preceding"));
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "all explicit preceding"));
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) == OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "all explicit preceding"));
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) == OperationInfoCleanupOutcome::EmptyObjectsSkipReached);
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "all explicit preceding"));
    CHECK(progress.stage() == OperationInfoCleanupStage::PositionCleanupBoundary);
    CHECK(attemptOperationInfoPositionListCleanup(operation, progress) == OperationInfoCleanupOutcome::EmptyPositionsSkipReached);
    CHECK(attemptOperationInfoPositionListCleanup(operation, progress) == OperationInfoCleanupOutcome::RecoveryCleanupUnavailable);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::RecoveryCleanupUnavailable);
    CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryCleanupBoundary);
}

TEST_CASE(opi_null_recovery_skip_preserves_flags_storage_and_current_at_next_condition)
{
    Operation operation;
    operation.recoveryB.reset(); // preexisting-null branch fixture, not an Open witness
    operation.objs.reserve(3); operation.objpos.reserve(4);
    const auto* objects = operation.objs.data(); const auto objectCapacity = operation.objs.capacity();
    const auto* positions = operation.objpos.data(); const auto positionCapacity = operation.objpos.capacity();
    operation.recoveryFlags = std::make_shared<std::vector<bool>>(2, true);
    const auto flags = operation.recoveryFlags;
    const auto flagOwners = flags.use_count();
    mst::OpClass cls(qt::L("NullRecovery")); cls.current = &operation;
    OperationInfoCleanupProgress progress(operation);
    reachRecoveryBoundary(operation, progress);
    CHECK(attemptOperationInfoRecoveryWrapperCleanup(operation, progress) == OperationInfoCleanupOutcome::NullRecoveryWrapperSkipReached);
    CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary);
    CHECK_EQ(progress.observedRecoveryWrapperIdentityToken(), std::uintptr_t{0});
    CHECK(!operation.recoveryB); CHECK(operation.recoveryFlags == flags);
    CHECK_EQ(flags.use_count(), flagOwners); REQUIRE(flags->size() == 2); CHECK((*flags)[0]); CHECK((*flags)[1]);
    CHECK(operation.objs.data() == objects); CHECK_EQ(operation.objs.capacity(), objectCapacity);
    CHECK(operation.objpos.data() == positions); CHECK_EQ(operation.objpos.capacity(), positionCapacity);
    CHECK(cls.current == &operation); CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    CHECK(attemptOperationInfoRecoveryWrapperCleanup(operation, progress) == OperationInfoCleanupOutcome::RecoveryFlagsCleanupUnavailable);
    CHECK(attemptOperationInfoPositionListCleanup(operation, progress) == OperationInfoCleanupOutcome::RecoveryFlagsCleanupUnavailable);
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) == OperationInfoCleanupOutcome::RecoveryFlagsCleanupUnavailable);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) == OperationInfoCleanupOutcome::RecoveryFlagsCleanupUnavailable);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::RecoveryFlagsCleanupUnavailable);
    CHECK(operation.recoveryFlags == flags); CHECK(cls.current == &operation);
}

TEST_CASE(opi_nonnull_recovery_including_default_empty_never_enters_null_skip)
{
    for (const bool populated : {false, true}) {
        Operation operation;
        REQUIRE(operation.recoveryB != nullptr);
        if (populated) { RecoveryEntry entry; entry.type = 7; entry.value = 9.5; operation.recoveryB->push_back(entry); }
        const auto* wrapper = operation.recoveryB.get(); const auto owners = operation.recoveryB.use_count();
        const auto size = operation.recoveryB->size();
        OperationInfoCleanupProgress progress(operation);
        reachRecoveryBoundary(operation, progress);
        CHECK(attemptOperationInfoRecoveryWrapperCleanup(operation, progress) == OperationInfoCleanupOutcome::NonNullRecoveryWrapperDeleteUnavailable);
        CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryWrapperDeleteBlocked);
        CHECK_EQ(progress.observedRecoveryWrapperIdentityToken(), reinterpret_cast<std::uintptr_t>(wrapper));
        CHECK(operation.recoveryB.get() == wrapper); CHECK_EQ(operation.recoveryB.use_count(), owners);
        CHECK_EQ(operation.recoveryB->size(), size);
        if (populated) { CHECK_EQ(operation.recoveryB->front().type, 7); CHECK_EQ(operation.recoveryB->front().value, 9.5); }
        CHECK(attemptOperationInfoRecoveryWrapperCleanup(operation, progress) == OperationInfoCleanupOutcome::NonNullRecoveryWrapperDeleteUnavailable);
        CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NonNullRecoveryWrapperDeleteUnavailable);
        CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    }
}

TEST_CASE(opi_blocked_recovery_reset_or_replacement_cannot_manufacture_null_skip)
{
    for (const bool replace : {false, true}) {
        Operation operation;
        OperationInfoCleanupProgress progress(operation);
        reachRecoveryBoundary(operation, progress);
        CHECK(attemptOperationInfoRecoveryWrapperCleanup(operation, progress) == OperationInfoCleanupOutcome::NonNullRecoveryWrapperDeleteUnavailable);
        const auto token = progress.observedRecoveryWrapperIdentityToken();
        if (replace) operation.recoveryB = std::make_shared<std::vector<RecoveryEntry>>();
        else operation.recoveryB.reset(); // external fixture change, no native deletion is certified
        CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "Recovery wrapper changed"));
        CHECK(refuses([&] { attemptOperationInfoPropsCleanup(operation, progress); }, "Recovery wrapper changed"));
        CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryWrapperDeleteBlocked);
        CHECK_EQ(progress.observedRecoveryWrapperIdentityToken(), token);
    }
}

TEST_CASE(opi_blocked_recovery_rejects_same_pointer_with_another_control_block)
{
    Operation operation;
    const auto original = operation.recoveryB;
    OperationInfoCleanupProgress progress(operation);
    reachRecoveryBoundary(operation, progress);
    CHECK(attemptOperationInfoRecoveryWrapperCleanup(operation, progress) == OperationInfoCleanupOutcome::NonNullRecoveryWrapperDeleteUnavailable);
    operation.recoveryB = std::shared_ptr<std::vector<RecoveryEntry>>(original.get(), [](std::vector<RecoveryEntry>*) {});
    CHECK(operation.recoveryB.get() == original.get());
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "Recovery wrapper changed"));
    operation.recoveryB.reset(); // discard only the no-op-deleter fixture alias
}

TEST_CASE(opi_recovery_progress_uses_no_strong_payload_lease)
{
    auto operation = std::make_unique<Operation>();
    std::weak_ptr<std::vector<RecoveryEntry>> wrapper = operation->recoveryB;
    auto progress = std::make_unique<OperationInfoCleanupProgress>(*operation);
    reachRecoveryBoundary(*operation, *progress);
    CHECK(attemptOperationInfoRecoveryWrapperCleanup(*operation, *progress) == OperationInfoCleanupOutcome::NonNullRecoveryWrapperDeleteUnavailable);
    const auto token = progress->observedRecoveryWrapperIdentityToken();
    operation.reset();
    CHECK(wrapper.expired()); CHECK_EQ(progress->observedRecoveryWrapperIdentityToken(), token);
    CHECK(progress->identitySnapshot().get() == nullptr);
    Operation later;
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(later, *progress); }, "does not bind"));
}

TEST_CASE(opi_recovery_null_skip_rejects_reappearing_wrapper_or_predecessor_owners)
{
    Operation operation;
    operation.recoveryB.reset();
    OperationInfoCleanupProgress progress(operation);
    reachRecoveryBoundary(operation, progress);
    CHECK(attemptOperationInfoRecoveryWrapperCleanup(operation, progress) == OperationInfoCleanupOutcome::NullRecoveryWrapperSkipReached);
    operation.recoveryB = std::make_shared<std::vector<RecoveryEntry>>();
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "Recovery wrapper appeared"));
    CHECK(refuses([&] { attemptOperationInfoPropsCleanup(operation, progress); }, "Recovery wrapper appeared"));
    operation.recoveryB.reset(); operation.objpos.push_back(nullptr);
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "Positions appeared"));
    operation.objpos.clear(); operation.objs.push_back(nullptr);
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "Objects appeared"));
    operation.objs.clear(); operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "Engine cache appeared"));
    operation.engineModel.reset(); operation.props = std::make_shared<Props>();
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "Props appeared"));
    CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary);
}

TEST_CASE(opi_recovery_rejects_foreign_or_expired_opi_before_wrapper_read)
{
    Operation operation, other;
    OperationInfoCleanupProgress progress(operation);
    reachRecoveryBoundary(operation, progress);
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(other, progress); }, "does not bind"));
    CHECK(operation.commitOperationInfoQObjectBoundary("declared recovery-prefix expiry fixture; not native cleanup"));
    CHECK(refuses([&] { attemptOperationInfoRecoveryWrapperCleanup(operation, progress); }, "does not bind"));
    CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryCleanupBoundary);
}

namespace {
void reachFlagsBoundary(Operation& operation, OperationInfoCleanupProgress& progress)
{
    reachRecoveryBoundary(operation, progress);
    REQUIRE(attemptOperationInfoRecoveryWrapperCleanup(operation, progress) == OperationInfoCleanupOutcome::NullRecoveryWrapperSkipReached);
}
}

TEST_CASE(opi_flags_require_every_explicit_predecessor_including_null_recovery)
{
    Operation operation;
    operation.recoveryB.reset(); // declared conditional null-B fixture, not normal Open
    OperationInfoCleanupProgress progress(operation);
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "including null recovery wrapper"));
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "including null recovery wrapper"));
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) == OperationInfoCleanupOutcome::NullEngineCacheSkipReached);
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "including null recovery wrapper"));
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) == OperationInfoCleanupOutcome::EmptyObjectsSkipReached);
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "including null recovery wrapper"));
    CHECK(attemptOperationInfoPositionListCleanup(operation, progress) == OperationInfoCleanupOutcome::EmptyPositionsSkipReached);
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "including null recovery wrapper"));
    CHECK(attemptOperationInfoRecoveryWrapperCleanup(operation, progress) == OperationInfoCleanupOutcome::NullRecoveryWrapperSkipReached);
    CHECK(attemptOperationInfoRecoveryWrapperCleanup(operation, progress) == OperationInfoCleanupOutcome::RecoveryFlagsCleanupUnavailable);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::RecoveryFlagsCleanupUnavailable);
    CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary);
}

TEST_CASE(opi_null_flags_skip_preserves_storage_current_and_unknown_item_cache)
{
    Operation operation;
    operation.recoveryB.reset(); // conditional null-B/null-flags fixture
    operation.objs.reserve(3); operation.objpos.reserve(4);
    const auto* objects = operation.objs.data(); const auto objectCapacity = operation.objs.capacity();
    const auto* positions = operation.objpos.data(); const auto positionCapacity = operation.objpos.capacity();
    engine::OperationInfoModelCache itemCache(operation); // separate diagnostic; no defaults event
    mst::OpClass cls(qt::L("NullFlags")); cls.current = &operation;
    OperationInfoCleanupProgress progress(operation);
    reachFlagsBoundary(operation, progress);
    CHECK(attemptOperationInfoRecoveryFlagsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullRecoveryFlagsSkipReached);
    CHECK(progress.stage() == OperationInfoCleanupStage::ItemModelCacheBoundary);
    CHECK_EQ(progress.observedRecoveryFlagsIdentityToken(), std::uintptr_t{0});
    CHECK(!operation.recoveryB); CHECK(!operation.recoveryFlags);
    CHECK(operation.objs.data() == objects); CHECK_EQ(operation.objs.capacity(), objectCapacity);
    CHECK(operation.objpos.data() == positions); CHECK_EQ(operation.objpos.capacity(), positionCapacity);
    CHECK(itemCache.knowledge() == engine::OperationInfoModelCacheKnowledge::Unknown);
    CHECK(cls.current == &operation); CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    CHECK(attemptOperationInfoRecoveryFlagsCleanup(operation, progress) == OperationInfoCleanupOutcome::ItemModelCacheCleanupUnavailable);
    CHECK(attemptOperationInfoRecoveryWrapperCleanup(operation, progress) == OperationInfoCleanupOutcome::ItemModelCacheCleanupUnavailable);
    CHECK(attemptOperationInfoPositionListCleanup(operation, progress) == OperationInfoCleanupOutcome::ItemModelCacheCleanupUnavailable);
    CHECK(attemptOperationInfoObjectListCleanup(operation, progress) == OperationInfoCleanupOutcome::ItemModelCacheCleanupUnavailable);
    CHECK(attemptOperationInfoEngineCacheCleanup(operation, progress) == OperationInfoCleanupOutcome::ItemModelCacheCleanupUnavailable);
    CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::ItemModelCacheCleanupUnavailable);
    CHECK(itemCache.knowledge() == engine::OperationInfoModelCacheKnowledge::Unknown);
    CHECK(cls.current == &operation);
}

TEST_CASE(opi_nonnull_flags_including_allocated_empty_are_preserved_at_delete_guard)
{
    for (const bool populated : {false, true}) {
        Operation operation;
        operation.recoveryB.reset();
        operation.recoveryFlags = std::make_shared<std::vector<bool>>();
        if (populated) { operation.recoveryFlags->push_back(true); operation.recoveryFlags->push_back(false); }
        const auto* flags = operation.recoveryFlags.get(); const auto owners = operation.recoveryFlags.use_count();
        const auto size = operation.recoveryFlags->size();
        OperationInfoCleanupProgress progress(operation);
        reachFlagsBoundary(operation, progress);
        CHECK(attemptOperationInfoRecoveryFlagsCleanup(operation, progress) == OperationInfoCleanupOutcome::NonNullRecoveryFlagsDeleteUnavailable);
        CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked);
        CHECK_EQ(progress.observedRecoveryFlagsIdentityToken(), reinterpret_cast<std::uintptr_t>(flags));
        CHECK(operation.recoveryFlags.get() == flags); CHECK_EQ(operation.recoveryFlags.use_count(), owners);
        CHECK_EQ(operation.recoveryFlags->size(), size);
        if (populated) { CHECK((*operation.recoveryFlags)[0]); CHECK(!(*operation.recoveryFlags)[1]); }
        CHECK(attemptOperationInfoRecoveryFlagsCleanup(operation, progress) == OperationInfoCleanupOutcome::NonNullRecoveryFlagsDeleteUnavailable);
        CHECK(attemptOperationInfoPropsCleanup(operation, progress) == OperationInfoCleanupOutcome::NonNullRecoveryFlagsDeleteUnavailable);
        CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    }
}

TEST_CASE(opi_blocked_flags_reset_or_replacement_cannot_manufacture_null_skip)
{
    for (const bool replace : {false, true}) {
        Operation operation;
        operation.recoveryB.reset(); operation.recoveryFlags = std::make_shared<std::vector<bool>>();
        OperationInfoCleanupProgress progress(operation);
        reachFlagsBoundary(operation, progress);
        CHECK(attemptOperationInfoRecoveryFlagsCleanup(operation, progress) == OperationInfoCleanupOutcome::NonNullRecoveryFlagsDeleteUnavailable);
        const auto token = progress.observedRecoveryFlagsIdentityToken();
        if (replace) operation.recoveryFlags = std::make_shared<std::vector<bool>>();
        else operation.recoveryFlags.reset(); // external fixture mutation, not completed native deletion
        CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "Recovery flags changed"));
        CHECK(refuses([&] { attemptOperationInfoPropsCleanup(operation, progress); }, "Recovery flags changed"));
        CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryFlagsDeleteBlocked);
        CHECK_EQ(progress.observedRecoveryFlagsIdentityToken(), token);
    }
}

TEST_CASE(opi_blocked_flags_reject_same_pointer_with_another_control_block)
{
    Operation operation;
    operation.recoveryB.reset(); operation.recoveryFlags = std::make_shared<std::vector<bool>>();
    const auto original = operation.recoveryFlags;
    OperationInfoCleanupProgress progress(operation);
    reachFlagsBoundary(operation, progress);
    CHECK(attemptOperationInfoRecoveryFlagsCleanup(operation, progress) == OperationInfoCleanupOutcome::NonNullRecoveryFlagsDeleteUnavailable);
    operation.recoveryFlags = std::shared_ptr<std::vector<bool>>(original.get(), [](std::vector<bool>*) {});
    CHECK(operation.recoveryFlags.get() == original.get());
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "Recovery flags changed"));
    operation.recoveryFlags.reset(); // discard only this no-op-deleter fixture alias
}

TEST_CASE(opi_flags_progress_retains_no_strong_payload_lease)
{
    auto operation = std::make_unique<Operation>();
    operation->recoveryB.reset(); operation->recoveryFlags = std::make_shared<std::vector<bool>>();
    std::weak_ptr<std::vector<bool>> flags = operation->recoveryFlags;
    auto progress = std::make_unique<OperationInfoCleanupProgress>(*operation);
    reachFlagsBoundary(*operation, *progress);
    CHECK(attemptOperationInfoRecoveryFlagsCleanup(*operation, *progress) == OperationInfoCleanupOutcome::NonNullRecoveryFlagsDeleteUnavailable);
    const auto token = progress->observedRecoveryFlagsIdentityToken();
    operation.reset();
    CHECK(flags.expired()); CHECK_EQ(progress->observedRecoveryFlagsIdentityToken(), token);
    CHECK(progress->identitySnapshot().get() == nullptr);
    Operation later;
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(later, *progress); }, "does not bind"));
}

TEST_CASE(opi_flags_null_skip_rejects_flags_or_predecessor_reappearance)
{
    Operation operation;
    operation.recoveryB.reset();
    OperationInfoCleanupProgress progress(operation);
    reachFlagsBoundary(operation, progress);
    CHECK(attemptOperationInfoRecoveryFlagsCleanup(operation, progress) == OperationInfoCleanupOutcome::NullRecoveryFlagsSkipReached);
    operation.recoveryFlags = std::make_shared<std::vector<bool>>();
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "Recovery flags appeared"));
    CHECK(refuses([&] { attemptOperationInfoPropsCleanup(operation, progress); }, "Recovery flags appeared"));
    operation.recoveryFlags.reset(); operation.recoveryB = std::make_shared<std::vector<RecoveryEntry>>();
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "Recovery wrapper appeared"));
    operation.recoveryB.reset(); operation.objpos.push_back(nullptr);
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "Positions appeared"));
    operation.objpos.clear(); operation.objs.push_back(nullptr);
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "Objects appeared"));
    operation.objs.clear(); operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "Engine cache appeared"));
    operation.engineModel.reset(); operation.props = std::make_shared<Props>();
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "Props appeared"));
    CHECK(progress.stage() == OperationInfoCleanupStage::ItemModelCacheBoundary);
}

TEST_CASE(opi_flags_reject_foreign_or_expired_opi_before_owner_read)
{
    Operation operation, other;
    operation.recoveryB.reset();
    OperationInfoCleanupProgress progress(operation);
    reachFlagsBoundary(operation, progress);
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(other, progress); }, "does not bind"));
    CHECK(operation.commitOperationInfoQObjectBoundary("declared flags-prefix expiry fixture; not native cleanup"));
    CHECK(refuses([&] { attemptOperationInfoRecoveryFlagsCleanup(operation, progress); }, "does not bind"));
    CHECK(progress.stage() == OperationInfoCleanupStage::RecoveryFlagsCleanupBoundary);
}

int main() { return testkit::run_all(); }
