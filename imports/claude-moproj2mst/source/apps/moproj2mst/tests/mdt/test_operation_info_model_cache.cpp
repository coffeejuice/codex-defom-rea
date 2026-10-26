// ctest-labels: m2m-mdt operation-info-model-cache
#include <testkit/check.hpp>

#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/engine/operation_info_cleanup.hpp"
#include "m2m/engine/operation_info_model_cache.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using engine::OperationInfoExpiry;
using engine::OperationInfoModelCache;
using engine::OperationInfoModelCacheKnowledge;
namespace {
constexpr const char* defaultsReceipt =
    "declared isolated native-defaults fixture:050550@05067b cbc920(+160,null); no native acceptance";

template<class F> bool refuses(F&& body, const char* detail)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED && std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}
}

static_assert(!std::is_copy_constructible_v<OperationInfoModelCache>);
static_assert(!std::is_move_constructible_v<OperationInfoModelCache>);
static_assert(std::is_same_v<decltype(std::declval<Operation&>().operationInfoModelCache()),
                             const OperationInfoModelCache&>);
static_assert(std::is_same_v<decltype(std::declval<const Operation&>().operationInfoModelCache()),
                             const OperationInfoModelCache&>);
static_assert(!std::is_invocable_v<decltype(&OperationInfoModelCache::commitNativeDefaultNull),
                                  const OperationInfoModelCache&, Operation&, const std::string&>);

TEST_CASE(opi_model_cache_unknown_noncreating_lookup_is_not_invented_null)
{
    Operation operation;
    mst::OpClass cls(qt::L("UnknownCache"));
    cls.current = &operation;
    OperationInfoModelCache cache(operation);
    CHECK(cache.knowledge() == OperationInfoModelCacheKnowledge::Unknown);
    CHECK(cache.nullQualification().empty());
    CHECK(refuses([&] { cache.lookup(operation, false); }, "guard result is unavailable"));
    CHECK(refuses([&] { cache.lookup(operation, true); }, "guard result is unavailable"));
    CHECK(cache.knowledge() == OperationInfoModelCacheKnowledge::Unknown);
    CHECK(cls.current == &operation);
}

TEST_CASE(opi_model_cache_native_default_null_allows_repeated_noncreating_lookup_without_rebinding)
{
    Operation operation;
    mst::OpClass cls(qt::L("KnownNullCache"));
    cls.current = &operation;
    OperationInfoModelCache cache(operation);
    const auto opiIdentity = cache.identitySnapshot();
    CHECK(cache.commitNativeDefaultNull(operation, defaultsReceipt));
    CHECK(cache.knowledge() == OperationInfoModelCacheKnowledge::KnownNull);
    CHECK(cache.lookup(operation, false) == nullptr);
    CHECK(cache.lookup(operation, false) == nullptr);
    CHECK(cache.identitySnapshot().get() == opiIdentity.get());
    CHECK(cache.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    CHECK(cache.nullQualification() == defaultsReceipt);
    CHECK(cls.current == &operation);
}

TEST_CASE(opi_model_cache_create_path_remains_guarded_without_changing_known_null_or_current)
{
    Operation operation;
    mst::OpClass cls(qt::L("CreateGuard"));
    cls.current = &operation;
    OperationInfoModelCache cache(operation);
    CHECK(cache.commitNativeDefaultNull(operation, defaultsReceipt));
    CHECK(refuses([&] { cache.lookup(operation, true); }, "construction/Qt effects are unavailable"));
    CHECK(cache.knowledge() == OperationInfoModelCacheKnowledge::KnownNull);
    CHECK(cache.nullQualification() == defaultsReceipt);
    CHECK(cache.lookup(operation, false) == nullptr);
    CHECK(cls.current == &operation);
}

TEST_CASE(opi_model_cache_default_null_receipt_is_required_and_first_commit_is_preserved)
{
    Operation operation;
    OperationInfoModelCache cache(operation);
    CHECK(refuses([&] { cache.commitNativeDefaultNull(operation, ""); }, "requires a source qualification"));
    CHECK(refuses([&] { cache.commitNativeDefaultNull(operation, " \t\r\n"); }, "requires a source qualification"));
    CHECK(cache.knowledge() == OperationInfoModelCacheKnowledge::Unknown);
    CHECK(cache.nullQualification().empty());
    CHECK(cache.commitNativeDefaultNull(operation, defaultsReceipt));
    CHECK(!cache.commitNativeDefaultNull(operation, "different later defaults receipt"));
    CHECK(cache.nullQualification() == defaultsReceipt);
}

TEST_CASE(opi_model_cache_cannot_be_committed_or_looked_up_for_another_operation)
{
    Operation first, second;
    OperationInfoModelCache cache(first);
    CHECK(refuses([&] { cache.commitNativeDefaultNull(second, defaultsReceipt); }, "does not bind"));
    CHECK(cache.knowledge() == OperationInfoModelCacheKnowledge::Unknown);
    CHECK(cache.commitNativeDefaultNull(first, defaultsReceipt));
    CHECK(refuses([&] { cache.lookup(second, false); }, "does not bind"));
    CHECK(cache.lookup(first, false) == nullptr);
}

TEST_CASE(opi_model_cache_lookup_does_not_advance_props_cleanup_or_expire_opi)
{
    Operation operation;
    mst::OpClass cls(qt::L("PropsProgress"));
    cls.current = &operation;
    engine::OperationInfoCleanupProgress progress(operation);
    OperationInfoModelCache cache(operation);
    CHECK(cache.commitNativeDefaultNull(operation, defaultsReceipt));
    CHECK(cache.lookup(operation, false) == nullptr);
    CHECK(progress.stage() == engine::OperationInfoCleanupStage::InspectProps);
    CHECK(engine::attemptOperationInfoPropsCleanup(operation, progress) ==
          engine::OperationInfoCleanupOutcome::NullPropsSkipReached);
    const auto stage = progress.stage();
    CHECK(cache.lookup(operation, false) == nullptr);
    CHECK(refuses([&] { cache.lookup(operation, true); }, "construction/Qt effects are unavailable"));
    CHECK(progress.stage() == stage);
    CHECK(progress.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    CHECK(cls.current == &operation);
}

TEST_CASE(opi_model_cache_rejects_explicit_opi_expiry_without_relabelling_cache_knowledge)
{
    Operation operation;
    OperationInfoModelCache cache(operation);
    CHECK(cache.commitNativeDefaultNull(operation, defaultsReceipt));
    CHECK(operation.commitOperationInfoQObjectBoundary("isolated external expiry fixture; no native acceptance"));
    CHECK(refuses([&] { cache.lookup(operation, false); }, "does not bind"));
    CHECK(refuses([&] { cache.commitNativeDefaultNull(operation, defaultsReceipt); }, "does not bind"));
    CHECK(refuses([&] { OperationInfoModelCache expired(operation); }, "does not bind"));
    CHECK(cache.knowledge() == OperationInfoModelCacheKnowledge::KnownNull);
    CHECK(cache.identitySnapshot().expiry() == OperationInfoExpiry::QObjectBoundary);
}

TEST_CASE(opi_model_cache_metadata_does_not_retain_target_allocation)
{
    auto operation = std::make_unique<Operation>();
    auto cache = std::make_unique<OperationInfoModelCache>(*operation);
    CHECK(cache->commitNativeDefaultNull(*operation, defaultsReceipt));
    operation.reset();
    CHECK(cache->identitySnapshot().get() == nullptr);
    CHECK(cache->identitySnapshot().expiry() == OperationInfoExpiry::AdapterIdentityLifetime);
    Operation later;
    CHECK(refuses([&] { cache->lookup(later, false); }, "does not bind"));
}

TEST_CASE(opi_canonical_cache_has_stable_identity_and_unknown_state_without_a_producer)
{
    Operation operation;
    const Operation& constOperation = operation;
    const auto& canonical = operation.operationInfoModelCache();
    CHECK(&canonical == &operation.operationInfoModelCache());
    CHECK(&canonical == &constOperation.operationInfoModelCache());
    CHECK(canonical.identitySnapshot().get() == &operation);
    CHECK(canonical.identitySnapshot().expiry() == OperationInfoExpiry::Live);
    CHECK(canonical.knowledge() == OperationInfoModelCacheKnowledge::Unknown);
    CHECK(canonical.nullQualification().empty());
    CHECK(refuses([&] { canonical.lookup(operation, false); }, "guard result is unavailable"));
    CHECK(refuses([&] { canonical.lookup(operation, true); }, "guard result is unavailable"));
    CHECK(!operation.engineModel);
    CHECK(canonical.knowledge() == OperationInfoModelCacheKnowledge::Unknown);
}

TEST_CASE(opi_candidate_defaults_and_init_do_not_certify_canonical_native_null)
{
    Project project;
    Operation operation;
    const auto* canonical = &operation.operationInfoModelCache();
    operationDefaults(operation); // candidate semantic defaults, no bound +160 event
    CHECK(&operation.operationInfoModelCache() == canonical);
    CHECK(canonical->knowledge() == OperationInfoModelCacheKnowledge::Unknown);
    CHECK(canonical->nullQualification().empty());
    initOperation(operation, project, 0);
    CHECK(operation.project == &project);
    CHECK(&operation.operationInfoModelCache() == canonical);
    CHECK(canonical->knowledge() == OperationInfoModelCacheKnowledge::Unknown);
    CHECK(refuses([&] { canonical->lookup(operation, false); }, "guard result is unavailable"));
}

TEST_CASE(opi_detached_known_null_cannot_substitute_for_canonical_unknown)
{
    Operation operation;
    OperationInfoModelCache detached(operation);
    const auto& canonical = operation.operationInfoModelCache();
    CHECK(&detached != &canonical);
    CHECK(detached.identitySnapshot().get() == canonical.identitySnapshot().get());
    CHECK(detached.commitNativeDefaultNull(operation, defaultsReceipt));
    CHECK(detached.lookup(operation, false) == nullptr);
    CHECK(canonical.knowledge() == OperationInfoModelCacheKnowledge::Unknown);
    CHECK(canonical.nullQualification().empty());
    CHECK(refuses([&] { canonical.lookup(operation, false); }, "guard result is unavailable"));
    CHECK(detached.knowledge() == OperationInfoModelCacheKnowledge::KnownNull);
}

TEST_CASE(opi_canonical_cache_rejects_the_diagnostic_promotion_api_even_after_const_removal)
{
    Operation operation;
    // The underlying member is nonconst; this deliberate API misuse verifies
    // the diagnostic receipt cannot become a canonical production shortcut.
    auto& attemptedMutation = const_cast<OperationInfoModelCache&>(operation.operationInfoModelCache());
    CHECK(refuses([&] { attemptedMutation.commitNativeDefaultNull(operation, defaultsReceipt); }, "no source-owned default-null producer"));
    CHECK(operation.operationInfoModelCache().knowledge() == OperationInfoModelCacheKnowledge::Unknown);
    CHECK(operation.operationInfoModelCache().nullQualification().empty());
}

TEST_CASE(opi_canonical_lookup_keeps_cleanup_and_current_guard_live)
{
    Operation operation;
    mst::OpClass cls(qt::L("CanonicalUnknown")); cls.current = &operation;
    engine::OperationInfoCleanupProgress progress(operation);
    const auto& canonical = operation.operationInfoModelCache();
    CHECK(refuses([&] { canonical.lookup(operation, false); }, "guard result is unavailable"));
    CHECK(progress.stage() == engine::OperationInfoCleanupStage::InspectProps);
    CHECK(engine::attemptOperationInfoPropsCleanup(operation, progress) == engine::OperationInfoCleanupOutcome::NullPropsSkipReached);
    CHECK(refuses([&] { canonical.lookup(operation, true); }, "guard result is unavailable"));
    CHECK(progress.stage() == engine::OperationInfoCleanupStage::EngineModelBoundary);
    CHECK(cls.current == &operation);
    CHECK(canonical.identitySnapshot().expiry() == OperationInfoExpiry::Live);
}

TEST_CASE(opi_canonical_cache_rejects_foreign_and_explicitly_expired_identity)
{
    Operation operation, other;
    const auto& canonical = operation.operationInfoModelCache();
    CHECK(refuses([&] { canonical.lookup(other, false); }, "does not bind"));
    CHECK(operation.commitOperationInfoQObjectBoundary("declared canonical-cache expiry fixture; not native cleanup"));
    CHECK(refuses([&] { canonical.lookup(operation, false); }, "does not bind"));
    auto& attemptedMutation = const_cast<OperationInfoModelCache&>(canonical);
    CHECK(refuses([&] { attemptedMutation.commitNativeDefaultNull(operation, defaultsReceipt); }, "does not bind"));
    CHECK(canonical.identitySnapshot().expiry() == OperationInfoExpiry::QObjectBoundary);
    CHECK(canonical.knowledge() == OperationInfoModelCacheKnowledge::Unknown);
}

TEST_CASE(opi_canonical_guard_snapshot_expires_without_rebinding_at_reused_operation_address)
{
    std::optional<Operation> storage;
    storage.emplace();
    auto* const address = &*storage;
    const auto snapshot = storage->operationInfoModelCache().identitySnapshot();
    OperationInfoModelCache detached(*storage);
    CHECK(detached.commitNativeDefaultNull(*storage, defaultsReceipt));
    storage.reset();
    CHECK(snapshot.get() == nullptr);
    CHECK(snapshot.expiry() == OperationInfoExpiry::AdapterIdentityLifetime);
    // Do not retain/dereference a canonical member reference past destruction.
    storage.emplace();
    CHECK(&*storage == address);
    CHECK(storage->operationInfoModelCache().identitySnapshot().get() == address);
    CHECK(storage->operationInfoModelCache().knowledge() == OperationInfoModelCacheKnowledge::Unknown);
    CHECK(snapshot.get() == nullptr);
    CHECK(refuses([&] { detached.lookup(*storage, false); }, "does not bind"));
    CHECK(refuses([&] { detached.commitNativeDefaultNull(*storage, defaultsReceipt); }, "does not bind"));
}

int main() { return testkit::run_all(); }
