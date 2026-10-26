// ctest-labels: m2m-model feature-context ownership
// One real source-role producer in this owned process; no Project or native Close.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <testkit/check.hpp>
#include "../../src/model/feature_crt_runtime.hpp"
#include <cstdio>
#include <cstring>
#include <type_traits>

using namespace m2m::detail;
static_assert(!std::is_default_constructible_v<FeatureCrtRuntime>);
static_assert(!std::is_copy_constructible_v<FeatureCrtRuntime>);
static_assert(!std::is_constructible_v<FeatureCrtRuntime, FeatureCrtProducerReport>);
static_assert(!FeatureCrtProducerReport::completeCrtStartup);
static_assert(!FeatureCrtProducerReport::projectBindingAdmitted);
static_assert(!FeatureCrtProducerReport::proxyAllocationAdmitted);
static_assert(!FeatureCrtProducerReport::nativeTeardownCompleted);
static_assert(!FeatureCrtProducerReport::completeForPublicClose);
static_assert(!std::is_default_constructible_v<FeatureMapProxyCall>);
static_assert(!std::is_copy_constructible_v<FeatureMapProxyCall>);
static_assert(!FeatureMapProxyReport::originalGlobalObserved && !FeatureMapProxyReport::nativeProjectContextConstructed);
static_assert(!FeatureMapProxyReport::projectBindingAdmitted && !FeatureMapProxyReport::completeForPublicClose);
static_assert(!FeatureMapProxyReport::nativeUnwindPerformed && !FeatureMapProxyReport::nativeCleanupPerformed);
static_assert(!FeatureMapEmptyClearReport::sentinelFreeEntered && !FeatureMapEmptyClearReport::proxyFreeEntered);
static_assert(!FeatureMapEmptyClearReport::nativeContextDestructorEntered && !FeatureMapEmptyClearReport::completeForPublicClose);
static_assert(!FeatureMapSentinelFreeReport::errnoProviderEntered && !FeatureMapSentinelFreeReport::getLastErrorEntered);
static_assert(!FeatureMapSentinelFreeReport::proxyFreeEntered && !FeatureMapSentinelFreeReport::heapDestroyEntered);
static_assert(FeatureMapProxyFreeReport::mapProxyNullStores == 0);
static_assert(!FeatureMapProxyFreeReport::errnoProviderEntered && !FeatureMapProxyFreeReport::getLastErrorEntered);
static_assert(!FeatureMapProxyFreeReport::nativeContextDestructorEntered && !FeatureMapProxyFreeReport::completeForPublicClose);

namespace {
std::weak_ptr<FeatureMapProxyCall> standaloneAfterMapDestructor;
void checkLeaseMatchesWithoutEffects(const std::shared_ptr<FeatureCrtRuntime>& runtime)
{
    const auto before = runtime->report();
    const auto copy = runtime;
    const std::shared_ptr<FeatureCrtRuntime> empty;
    const std::shared_ptr<FeatureCrtRuntime> nullAlias(runtime, nullptr);
    const std::shared_ptr<FeatureCrtRuntime> foreign(runtime.get(), [](FeatureCrtRuntime*) {});
    CHECK(runtime->matchesRetainedLease(runtime) && runtime->matchesRetainedLease(copy));
    CHECK(!runtime->matchesRetainedLease(empty) && !runtime->matchesRetainedLease(nullAlias));
    CHECK(foreign.get() == runtime.get());
    CHECK(foreign.owner_before(runtime) || runtime.owner_before(foreign));
    CHECK(!runtime->matchesRetainedLease(foreign));
    const auto after = runtime->report();
    CHECK(after.origin == before.origin && after.stage == before.stage && after.stop == before.stop);
    CHECK(after.runtimeIdentity == before.runtimeIdentity && after.processId == before.processId);
    CHECK(after.moduleIdentity == before.moduleIdentity && after.modulePinned == before.modulePinned);
    CHECK(after.ownedTestProcessRetention == before.ownedTestProcessRetention);
    CHECK(after.candidateSlotPublished == before.candidateSlotPublished && after.heapNonNull == before.heapNonNull);
    CHECK(after.compatibilityReturn == before.compatibilityReturn && after.producerReturn == before.producerReturn);
    CHECK(after.heapCreateCalls == before.heapCreateCalls);
    CHECK(after.candidateSlotPublicationStores == before.candidateSlotPublicationStores);
    CHECK(after.compatibilityCalls == before.compatibilityCalls);
    CHECK(after.effectCount == before.effectCount && after.effects == before.effects);
    CHECK(after.nextSourceSite == before.nextSourceSite);
}
}

TEST_CASE(declared_null_heap_is_published_before_unqualified_startup_failure)
{
    const auto result = projectDeclaredFeatureCrtResults(false, std::nullopt);
    REQUIRE(result.valid);
    const auto& report = result.progress;
    CHECK(report.origin == FeatureCrtOrigin::DeclaredModel);
    CHECK(report.runtimeIdentity == nullptr && report.processId == 0 && report.moduleIdentity == 0);
    CHECK(!report.modulePinned && !report.ownedTestProcessRetention);
    CHECK(report.candidateSlotPublished && report.heapNonNull.has_value() && !*report.heapNonNull);
    CHECK(report.stage == FeatureCrtProducerStage::HeapCreateReturnedNull);
    CHECK(report.stop == FeatureCrtProducerStop::StartupFatalModeUnavailable);
    CHECK(report.producerReturn == 0u && !report.compatibilityReturn);
    CHECK(report.heapCreateCalls == 1 && report.candidateSlotPublicationStores == 1 && report.compatibilityCalls == 0);
    REQUIRE(report.effectCount == 4);
    CHECK(report.effects[0] == FeatureCrtEffect::HeapCreateReturned);
    CHECK(report.effects[1] == FeatureCrtEffect::CandidateHeapSlotPublished);
    CHECK(report.effects[2] == FeatureCrtEffect::HeapNullTested);
    CHECK(report.effects[3] == FeatureCrtEffect::ProducerReturned);
    REQUIRE(report.nextSourceSite);
    CHECK(std::strcmp(report.nextSourceSite, "0x141470cac:0x1446ffb58 startup fatal-mode read") == 0);
}

TEST_CASE(declared_compatibility_false_does_not_revoke_created_heap_or_producer_success)
{
    const auto result = projectDeclaredFeatureCrtResults(true, 0);
    REQUIRE(result.valid);
    const auto& report = result.progress;
    CHECK(report.origin == FeatureCrtOrigin::DeclaredModel && report.runtimeIdentity == nullptr);
    CHECK(report.heapNonNull == true && report.compatibilityReturn == 0 && report.producerReturn == 1u);
    CHECK(report.stage == FeatureCrtProducerStage::CompatibilityCallReturned);
    CHECK(report.stop == FeatureCrtProducerStop::LaterStartupUnavailable);
    CHECK(report.heapCreateCalls == 1 && report.candidateSlotPublicationStores == 1 && report.compatibilityCalls == 1);
    REQUIRE(report.effectCount == 5);
    CHECK(report.effects[0] == FeatureCrtEffect::HeapCreateReturned);
    CHECK(report.effects[1] == FeatureCrtEffect::CandidateHeapSlotPublished);
    CHECK(report.effects[2] == FeatureCrtEffect::HeapNullTested);
    CHECK(report.effects[3] == FeatureCrtEffect::CompatibilityCallReturned);
    CHECK(report.effects[4] == FeatureCrtEffect::ProducerReturned);
    REQUIRE(report.nextSourceSite);
    CHECK(std::strcmp(report.nextSourceSite, "0x141470cce:0x141481708 later startup") == 0);
}

TEST_CASE(declared_results_preserve_raw_bool_and_refuse_conflicting_or_incomplete_completion)
{
    const auto nonzero = projectDeclaredFeatureCrtResults(true, 17);
    CHECK(nonzero.valid && nonzero.progress.compatibilityReturn == 17 && nonzero.progress.producerReturn == 1u);
    const auto partial = projectDeclaredFeatureCrtResults(true, std::nullopt);
    REQUIRE(partial.valid);
    CHECK(partial.progress.stage == FeatureCrtProducerStage::CompatibilityCallPending);
    CHECK(partial.progress.stop == FeatureCrtProducerStop::CompatibilityObservationUnavailable);
    CHECK(partial.progress.effectCount == 3 && !partial.progress.producerReturn);
    const auto conflict = projectDeclaredFeatureCrtResults(false, 0);
    CHECK(!conflict.valid && conflict.progress.stop == FeatureCrtProducerStop::DeclaredInputConflict);
    CHECK(!conflict.progress.candidateSlotPublished && conflict.progress.effectCount == 0);
}

TEST_CASE(candidate_runtime_lease_matching_checks_control_block_without_starting_producer)
{
    const auto runtime = FeatureCrtRuntime::prepareOwnedTestProcess(); REQUIRE(runtime);
    const auto before = runtime->report();
    REQUIRE(before.stage == FeatureCrtProducerStage::NotAttempted);
    CHECK(!before.candidateSlotPublished && before.heapCreateCalls == 0);
    checkLeaseMatchesWithoutEffects(runtime);
    const auto unready = runtime->prepareOwnedTestMapProxy(runtime);
    CHECK(unready.status == FeatureMapProxyPrepareStatus::ProducerUnavailable && !unready.call);
    const auto unreadyContext = runtime->prepareOwnedTestContextPrefix(runtime);
    CHECK(unreadyContext.status == FeatureMapProxyPrepareStatus::ProducerUnavailable && !unreadyContext.call);
    const std::shared_ptr<FeatureCrtRuntime> empty;
    const std::shared_ptr<FeatureCrtRuntime> nullAlias(runtime, nullptr);
    const std::shared_ptr<FeatureCrtRuntime> foreign(runtime.get(), [](FeatureCrtRuntime*) {});
    CHECK(runtime->prepareOwnedTestMapProxy(empty).status == FeatureMapProxyPrepareStatus::OwnerMismatch);
    CHECK(runtime->prepareOwnedTestMapProxy(nullAlias).status == FeatureMapProxyPrepareStatus::OwnerMismatch);
    CHECK(runtime->prepareOwnedTestMapProxy(foreign).status == FeatureMapProxyPrepareStatus::OwnerMismatch);
    CHECK(runtime->report().heapCreateCalls == 0);
}

TEST_CASE(actual_owned_process_source_producer_uses_one_heap_and_never_replays)
{
    const auto runtime = FeatureCrtRuntime::prepareOwnedTestProcess(); REQUIRE(runtime);
    const auto before = runtime->report();
    REQUIRE(before.modulePinned && before.ownedTestProcessRetention);
    CHECK(before.origin == FeatureCrtOrigin::ActualWin32 && before.runtimeIdentity == runtime.get());
    CHECK(before.processId == GetCurrentProcessId());
    CHECK(before.moduleIdentity == reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr)));
    CHECK(before.stage == FeatureCrtProducerStage::NotAttempted && !before.candidateSlotPublished);
    CHECK(!before.heapNonNull && before.heapCreateCalls == 0 && before.effectCount == 0);
    const auto first = runtime->attemptProducer();
    CHECK(first.candidateSlotPublished && first.heapCreateCalls == 1 && first.candidateSlotPublicationStores == 1);
    REQUIRE(first.heapNonNull.has_value() && *first.heapNonNull);
    CHECK(first.compatibilityCalls == 1 && first.compatibilityReturn.has_value());
    CHECK(first.producerReturn == 1u && first.stage == FeatureCrtProducerStage::CompatibilityCallReturned);
    CHECK(first.stop == FeatureCrtProducerStop::LaterStartupUnavailable && first.effectCount == 5);
    const auto same = FeatureCrtRuntime::prepareOwnedTestProcess();
    CHECK(same == runtime && !same.owner_before(runtime) && !runtime.owner_before(same));
    const auto again = same->attemptProducer();
    CHECK(again.runtimeIdentity == first.runtimeIdentity && again.processId == first.processId);
    CHECK(again.moduleIdentity == first.moduleIdentity && again.effectCount == first.effectCount);
    CHECK(again.heapCreateCalls == 1 && again.candidateSlotPublicationStores == 1 && again.compatibilityCalls == 1);
    CHECK(again.heapNonNull == first.heapNonNull && again.compatibilityReturn == first.compatibilityReturn);
    CHECK(again.producerReturn == first.producerReturn && again.effects == first.effects);
    checkLeaseMatchesWithoutEffects(runtime);
    CHECK(runtime->report().heapCreateCalls == 1 && runtime->matchesRetainedLease(same));
    REQUIRE(first.compatibilityReturn.has_value() && first.producerReturn.has_value());
    std::printf("[candidate CRT producer] pid=%u module=%llx create=%u slot=%u compatibility=%d returned=%u retained-until-process-end=1\n",
                static_cast<unsigned>(first.processId), static_cast<unsigned long long>(first.moduleIdentity),
                first.heapCreateCalls, first.candidateSlotPublicationStores, *first.compatibilityReturn, *first.producerReturn);
}

TEST_CASE(declared_map_allocation_failure_stops_inside_malloc_without_publication_or_unwind)
{
    const auto report = declaredFeatureMapAllocationFailure();
    CHECK(report.origin == FeatureCrtOrigin::DeclaredModel);
    CHECK(report.candidateRuntimeIdentity == nullptr && report.candidateMapIdentity == nullptr);
    CHECK(report.preparedCandidateHeapIdentity == 0 && report.candidateSlotReadIdentity == 0);
    CHECK(report.heapReadNonNull == true); // Declared result, not a real handle.
    CHECK(report.heapAllocCalls == 1 && report.heapAllocReturned && report.allocationNonNull == false);
    CHECK(report.heapAllocBytesArgument == 8 && report.heapAllocFlagsArgument == 0);
    CHECK(report.stage == FeatureMapProxyStage::AllocationFailurePending);
    CHECK(report.stop == FeatureMapProxyStop::AllocationFailureInputsUnavailable);
    CHECK(report.candidateProxyIdentity == nullptr && report.backpointerStores == 0 && report.mapProxyStores == 0);
    CHECK(!report.headKnown && !report.countKnown);
    REQUIRE(report.effectCount == 4);
    CHECK(report.effects[0] == FeatureMapProxyEffect::CandidateHeapSlotRead);
    CHECK(report.effects[1] == FeatureMapProxyEffect::HeapNullTested);
    CHECK(report.effects[2] == FeatureMapProxyEffect::HeapAllocEntered);
    CHECK(report.effects[3] == FeatureMapProxyEffect::HeapAllocReturned);
    REQUIRE(report.nextSourceSite);
    CHECK(std::strcmp(report.nextSourceSite, "0x1414748ff:0x144700858 new-mode read") == 0);
}

TEST_CASE(declared_sentinel_allocation_failure_has_no_head_publication_or_native_unwind)
{
    const auto report = declaredFeatureMapSentinelFailure();
    CHECK(report.origin == FeatureCrtOrigin::DeclaredModel);
    CHECK(report.heapReadNonNull == true && report.candidateSlotReadIdentity == 0);
    CHECK(report.candidateSlotReads == 1 && report.heapAllocHeapArgument == 0);
    CHECK(report.heapAllocBytesArgument == 48 && report.heapAllocFlagsArgument == 0);
    CHECK(report.heapAllocCalls == 1 && report.heapAllocReturned && report.allocationNonNull == false);
    CHECK(report.candidateHeadIdentity == nullptr && report.helperReturnIdentity == nullptr);
    CHECK(report.observedMapHead == nullptr && report.returnedMapIdentity == nullptr);
    CHECK(!report.initialLinksKnown && !report.byte28Known && !report.initialNilKnown && !report.finalNilKnown);
    CHECK(report.headReloads == 0 && report.selfLinkStores == 0 && !report.keyPayloadKnown && !report.suffixKnown);
    CHECK(report.stop == FeatureMapProxyStop::AllocationFailureInputsUnavailable);
    REQUIRE(report.effectCount == 4);
    CHECK(report.effects[0] == FeatureMapSentinelEffect::CandidateHeapSlotRead);
    CHECK(report.effects[1] == FeatureMapSentinelEffect::HeapNullTested);
    CHECK(report.effects[2] == FeatureMapSentinelEffect::HeapAllocEntered);
    CHECK(report.effects[3] == FeatureMapSentinelEffect::HeapAllocReturned);
    REQUIRE(report.nextSourceSite);
    CHECK(std::strcmp(report.nextSourceSite, "0x1414748ff:0x144700858 new-mode read (sentinel request)") == 0);
}

TEST_CASE(declared_sentinel_free_zero_parks_before_errno_and_caller_stores)
{
    const auto free = declaredFeatureMapSentinelFreeFailure();
    CHECK(free.origin == FeatureCrtOrigin::DeclaredModel && free.started);
    CHECK(free.sourcePointerNonNull == true && free.heapFreeResult == 0);
    CHECK(free.candidateSlotReads == 1 && free.heapFreeCalls == 1 && free.heapFreeReturned);
    CHECK(free.mapIdentity == nullptr && free.heapFreeHeapArgument == 0 && free.heapFreePointerArgument == 0);
    CHECK(!free.sourceFreeReturned && !free.sentinelRetired && !free.headNullStored);
    CHECK(!free.countAfterStore && !free.mapInnerDestructorReturned && free.proxyFreeArgument == nullptr);
    CHECK(free.stop == FeatureMapProxyStop::FreeErrorContinuationUnavailable);
    REQUIRE(free.nextSourceSite);
    CHECK(std::strcmp(free.nextSourceSite, "0x141471b9c:CRT errno provider unavailable") == 0);
    REQUIRE(free.effectCount == 5);
    CHECK(free.effects[0].effect == FeatureMapSentinelFreeEffect::PointerNonNullTested);
    CHECK(free.effects[1].effect == FeatureMapSentinelFreeEffect::CandidateHeapSlotRead);
    CHECK(free.effects[2].effect == FeatureMapSentinelFreeEffect::HeapFreeEntered);
    CHECK(free.effects[3].effect == FeatureMapSentinelFreeEffect::HeapFreeReturned);
    CHECK(free.effects[4].effect == FeatureMapSentinelFreeEffect::HeapFreeResultTested);
    CHECK(free.effects[4].sourceSite == 0x141471b9a);
    // Model-only count, never a failed kernel free or a resumable TLS/error witness.
}

TEST_CASE(declared_proxy_free_zero_does_not_retire_proxy_or_complete_map_destructor)
{
    const auto free = declaredFeatureMapProxyFreeFailure();
    CHECK(free.origin == FeatureCrtOrigin::DeclaredModel && free.started && free.sourcePointerNonNull == true);
    CHECK(free.candidateSlotReads == 1 && free.heapFreeCalls == 1 && free.heapFreeReturned && free.heapFreeResult == 0);
    CHECK(free.mapIdentityToken == 0 && free.mapProxyTokenBefore == 0 && free.backpointerTokenBefore == 0);
    CHECK(free.heapFreeHeapArgument == 0 && free.heapFreePointerArgument == 0);
    CHECK(!free.sourceFreeReturned && !free.proxyRetired && !free.candidateMapDestructorReturned);
    CHECK(free.stop == FeatureMapProxyStop::FreeErrorContinuationUnavailable);
    REQUIRE(free.effectCount == 5 && free.nextSourceSite);
    CHECK(free.effects[4].effect == FeatureMapProxyFreeEffect::HeapFreeResultTested);
    CHECK(free.effects[4].sourceSite == 0x141471b9a);
    CHECK(std::strcmp(free.nextSourceSite, "0x141471b9c:CRT errno provider unavailable (proxy free)") == 0);
    // Pure result projection; no actual handle/frame/TLS-error continuity.
}

TEST_CASE(actual_isolated_map_constructor_retains_proxy_then_completes_sentinel_without_replay)
{
    const auto runtime = FeatureCrtRuntime::prepareOwnedTestProcess(); REQUIRE(runtime);
    const auto producerBefore = runtime->report();
    REQUIRE(producerBefore.producerReturn == 1u && producerBefore.heapNonNull == true);
    // Complete the actual first-role occurrence while the canonical owner is
    // fresh. Child preparation must still prohibit the not-yet-started tail.
    const auto firstHandler = runtime->resumeEncodedNewHandlerPrefix(runtime);
    REQUIRE(firstHandler.encodePointerReturned && firstHandler.candidateRoleKnown && firstHandler.firstSetterReturned);
    REQUIRE(firstHandler.encodePointerCalls == 1 && firstHandler.candidateRoleStores == 1);
    REQUIRE(firstHandler.runtimeIdentity == runtime.get() && firstHandler.moduleIdentity == producerBefore.moduleIdentity);
    CHECK(runtime->encodedNullRoleTailReport().stop == FeatureCrtNullRoleTailStop::NotStarted);
    const std::shared_ptr<FeatureCrtRuntime> foreign(runtime.get(), [](FeatureCrtRuntime*) {});
    CHECK(runtime->prepareOwnedTestMapProxy(foreign).status == FeatureMapProxyPrepareStatus::OwnerMismatch);
    auto prepared = runtime->prepareOwnedTestMapProxy(runtime);
    REQUIRE(prepared.status == FeatureMapProxyPrepareStatus::Prepared && prepared.call);
    const auto before = prepared.call->report();
    const auto blockedTail = runtime->resumeEncodedNullRoleTail(runtime);
    CHECK(blockedTail.stop == FeatureCrtNullRoleTailStop::ChildFrameAlreadyPrepared);
    CHECK(!blockedTail.started && !blockedTail.savedArgumentKnown && blockedTail.roleStores == 0 && blockedTail.effectCount == 0);
    for (bool known : blockedTail.roleKnown) CHECK(!known);
    CHECK(prepared.call->report().heapAllocCalls == 0 && prepared.call->report().sentinel.heapAllocCalls == 0);
    const auto preservedHandler = runtime->encodedNewHandlerReport();
    CHECK(preservedHandler.encodePointerCalls == 1 && preservedHandler.candidateRoleStores == 1);
    CHECK(preservedHandler.encodedNullResult == firstHandler.encodedNullResult && preservedHandler.effectCount == firstHandler.effectCount);
    const auto prematureFree = prepared.call->resumeSentinelFreePrefix();
    const auto prematureProxyFree = prepared.call->resumeProxyFreePrefix();
    CHECK(prematureProxyFree.proxyFree.stop == FeatureMapProxyStop::ProxyFreePrerequisiteUnavailable);
    CHECK(!prematureProxyFree.proxyFree.started && prematureProxyFree.proxyFree.heapFreeCalls == 0);
    CHECK(prematureFree.sentinelFree.stop == FeatureMapProxyStop::SentinelFreePrerequisiteUnavailable);
    CHECK(!prematureFree.sentinelFree.started && prematureFree.sentinelFree.heapFreeCalls == 0);
    const auto prematureClear = prepared.call->resumeEmptyMapCleanupPrefix();
    CHECK(prematureClear.emptyClear.stop == FeatureMapProxyStop::EmptyMapPrerequisiteUnavailable);
    CHECK(!prematureClear.emptyClear.started && prematureClear.emptyClear.effectCount == 0);
    CHECK(prematureClear.heapAllocCalls == 0 && prematureClear.sentinel.heapAllocCalls == 0);
    CHECK(before.origin == FeatureCrtOrigin::ActualWin32 && before.stage == FeatureMapProxyStage::Prepared);
    CHECK(before.candidateRuntimeIdentity == runtime.get() && before.candidateMapIdentity != nullptr);
    CHECK(before.preparedCandidateHeapIdentity != 0);
    CHECK(before.candidateSlotReads == 0 && before.heapAllocCalls == 0 && !before.heapAllocReturned);
    CHECK(!before.allocationNonNull && before.candidateProxyIdentity == nullptr);
    CHECK(before.backpointerStores == 0 && before.mapProxyStores == 0 && !before.headKnown && !before.countKnown);
    CHECK(before.effectCount == 0);
    CHECK(before.context.profile == FeatureContextReceiverProfile::StandaloneMap);
    CHECK(before.context.candidateContextIdentity == nullptr && before.context.candidateEmbeddedMapIdentity == nullptr);
    const auto noContext = prepared.call->resumeContextPrefix();
    CHECK(noContext.context.stop == FeatureMapProxyStop::ContextOwnerUnavailable && noContext.context.effectCount == 0);
    CHECK(noContext.heapAllocCalls == 0 && noContext.sentinel.heapAllocCalls == 0);
    CHECK(!before.candidateMapConstructorReturned && before.sentinel.heapAllocCalls == 0);
    const auto prematureSentinel = prepared.call->resumeSentinel();
    CHECK(prematureSentinel.stage == FeatureMapProxyStage::Prepared && prematureSentinel.heapAllocCalls == 0);
    CHECK(prematureSentinel.sentinel.heapAllocCalls == 0 && prematureSentinel.sentinel.effectCount == 0);
    const auto capacity = runtime->prepareOwnedTestMapProxy(runtime);
    CHECK(capacity.status == FeatureMapProxyPrepareStatus::ExperimentCapacityUnavailable && !capacity.call);
    CHECK(prepared.call->report().heapAllocCalls == 0);

    const auto first = prepared.call->resume();
    CHECK(first.heapReadNonNull == true && first.candidateSlotReads == 1);
    CHECK(first.candidateSlotReadIdentity == before.preparedCandidateHeapIdentity);
    CHECK(first.heapAllocHeapArgument == first.candidateSlotReadIdentity);
    CHECK(first.heapAllocBytesArgument == 8 && first.heapAllocFlagsArgument == 0);
    CHECK(first.heapAllocCalls == 1 && first.heapAllocReturned);
    REQUIRE(first.allocationNonNull.has_value() && *first.allocationNonNull);
    REQUIRE(first.candidateProxyIdentity);
    CHECK(first.candidateRuntimeIdentity == runtime.get() && first.candidateMapIdentity == before.candidateMapIdentity);
    CHECK(first.observedProxyBackpointer == first.candidateMapIdentity);
    CHECK(first.observedMapProxy == first.candidateProxyIdentity);
    CHECK(first.backpointerStores == 1 && first.mapProxyStores == 1);
    CHECK(!first.headKnown && !first.countKnown);
    CHECK(!first.candidateMapConstructorReturned && first.sentinel.heapAllocCalls == 0);
    CHECK(first.stage == FeatureMapProxyStage::SentinelPending && first.stop == FeatureMapProxyStop::SentinelAllocationUnavailable);
    REQUIRE(first.effectCount == 6);
    CHECK(first.effects[0] == FeatureMapProxyEffect::CandidateHeapSlotRead);
    CHECK(first.effects[1] == FeatureMapProxyEffect::HeapNullTested);
    CHECK(first.effects[2] == FeatureMapProxyEffect::HeapAllocEntered);
    CHECK(first.effects[3] == FeatureMapProxyEffect::HeapAllocReturned);
    CHECK(first.effects[4] == FeatureMapProxyEffect::ProxyBackpointerStored);
    CHECK(first.effects[5] == FeatureMapProxyEffect::MapProxyPublished);
    const auto again = prepared.call->resume();
    CHECK(again.candidateProxyIdentity == first.candidateProxyIdentity && again.candidateMapIdentity == first.candidateMapIdentity);
    CHECK(again.heapAllocCalls == 1 && again.candidateSlotReads == 1 && again.effects == first.effects);
    CHECK(again.backpointerStores == 1 && again.mapProxyStores == 1);
    std::weak_ptr<FeatureMapProxyCall> weak = prepared.call;
    prepared.call.reset();
    REQUIRE(!weak.expired());
    auto retained = weak.lock(); REQUIRE(retained);
    const auto afterLease = retained->resume();
    CHECK(afterLease.candidateMapIdentity == first.candidateMapIdentity && afterLease.candidateProxyIdentity == first.candidateProxyIdentity);
    CHECK(afterLease.heapAllocCalls == 1 && afterLease.observedProxyBackpointer == first.candidateMapIdentity);
    CHECK(runtime->prepareOwnedTestMapProxy(runtime).status == FeatureMapProxyPrepareStatus::ExperimentCapacityUnavailable);
    CHECK(runtime->prepareOwnedTestMapProxy(foreign).status == FeatureMapProxyPrepareStatus::OwnerMismatch);
    const auto producerAfter = runtime->report();
    CHECK(producerAfter.effects == producerBefore.effects && producerAfter.heapCreateCalls == producerBefore.heapCreateCalls);
    CHECK(producerAfter.compatibilityCalls == producerBefore.compatibilityCalls && producerAfter.producerReturn == producerBefore.producerReturn);
    std::printf("[candidate map prefix] map=%p proxy=%p heap=%llx bytes=%zu flags=%u alloc=%u backpointer=%u publish=%u sentinel-pending=1 process-retained=1\n",
                const_cast<void*>(first.candidateMapIdentity), const_cast<void*>(first.candidateProxyIdentity),
                static_cast<unsigned long long>(first.heapAllocHeapArgument), first.heapAllocBytesArgument,
                first.heapAllocFlagsArgument, first.heapAllocCalls, first.backpointerStores, first.mapProxyStores);

    const auto complete = retained->resumeSentinel();
    const auto& sentinel = complete.sentinel;
    CHECK(sentinel.origin == FeatureCrtOrigin::ActualWin32);
    CHECK(sentinel.candidateSlotReads == 1 && sentinel.heapReadNonNull == true);
    CHECK(sentinel.candidateSlotReadIdentity == first.heapAllocHeapArgument);
    CHECK(sentinel.heapAllocHeapArgument == sentinel.candidateSlotReadIdentity);
    CHECK(sentinel.heapAllocBytesArgument == 48 && sentinel.heapAllocFlagsArgument == 0);
    CHECK(sentinel.heapAllocCalls == 1 && sentinel.heapAllocReturned);
    REQUIRE(sentinel.allocationNonNull.has_value() && *sentinel.allocationNonNull);
    REQUIRE(sentinel.candidateHeadIdentity);
    CHECK(sentinel.candidateHeadIdentity != first.candidateProxyIdentity);
    CHECK(sentinel.initialLinksKnown && sentinel.initialLinks[0] == nullptr && sentinel.initialLinks[1] == nullptr && sentinel.initialLinks[2] == nullptr);
    CHECK(sentinel.byte28Known && sentinel.observedByte28 == 1);
    CHECK(sentinel.initialNilKnown && sentinel.initialNil == 0 && sentinel.finalNilKnown && sentinel.finalNil == 1);
    CHECK(sentinel.helperReturnIdentity == sentinel.candidateHeadIdentity && sentinel.observedMapHead == sentinel.candidateHeadIdentity);
    CHECK(sentinel.headReloads == 3 && sentinel.selfLinkStores == 3);
    for (std::size_t i = 0; i < 3; ++i) {
        CHECK(sentinel.headReloadValues[i] == sentinel.candidateHeadIdentity);
        CHECK(sentinel.selfLinkValues[i] == sentinel.candidateHeadIdentity);
    }
    CHECK(complete.headKnown && complete.countKnown && sentinel.observedMapCount == 0);
    CHECK(!sentinel.keyPayloadKnown && !sentinel.suffixKnown);
    CHECK(complete.candidateMapConstructorReturned && sentinel.returnedMapIdentity == first.candidateMapIdentity);
    CHECK(complete.stage == FeatureMapProxyStage::MapConstructorReturned && complete.stop == FeatureMapProxyStop::EnclosingContextUnavailable);
    CHECK(complete.candidateProxyIdentity == first.candidateProxyIdentity && complete.observedProxyBackpointer == first.candidateMapIdentity);
    CHECK(complete.heapAllocCalls == 1 && complete.effects == first.effects && complete.backpointerStores == 1 && complete.mapProxyStores == 1);
    const std::array<FeatureMapSentinelEffect, 20> expected{{
        FeatureMapSentinelEffect::CandidateHeapSlotRead, FeatureMapSentinelEffect::HeapNullTested,
        FeatureMapSentinelEffect::HeapAllocEntered, FeatureMapSentinelEffect::HeapAllocReturned,
        FeatureMapSentinelEffect::Link0NullStored, FeatureMapSentinelEffect::Link8NullStored,
        FeatureMapSentinelEffect::Link10NullStored, FeatureMapSentinelEffect::Byte28OneStored,
        FeatureMapSentinelEffect::NilZeroStored, FeatureMapSentinelEffect::HelperReturnedHead,
        FeatureMapSentinelEffect::MapHeadPublished, FeatureMapSentinelEffect::NilOneStored,
        FeatureMapSentinelEffect::MapHeadReloadFor8, FeatureMapSentinelEffect::Self8Stored,
        FeatureMapSentinelEffect::MapHeadReloadFor0, FeatureMapSentinelEffect::Self0Stored,
        FeatureMapSentinelEffect::MapHeadReloadFor10, FeatureMapSentinelEffect::Self10Stored,
        FeatureMapSentinelEffect::CountZeroStored, FeatureMapSentinelEffect::MapConstructorReturned
    }};
    CHECK(sentinel.effectCount == expected.size() && sentinel.effects == expected);
    const auto repeated = retained->resumeSentinel();
    const auto oldPhase = retained->resume();
    CHECK(repeated.sentinel.effects == sentinel.effects && repeated.sentinel.heapAllocCalls == 1);
    CHECK(oldPhase.candidateMapConstructorReturned && oldPhase.sentinel.heapAllocCalls == 1 && oldPhase.heapAllocCalls == 1);
    retained.reset();
    REQUIRE(!weak.expired());
    const auto reacquired = weak.lock(); REQUIRE(reacquired);
    const auto afterAllLeases = reacquired->resumeSentinel();
    CHECK(afterAllLeases.candidateMapIdentity == first.candidateMapIdentity && afterAllLeases.candidateProxyIdentity == first.candidateProxyIdentity);
    CHECK(afterAllLeases.sentinel.candidateHeadIdentity == sentinel.candidateHeadIdentity && afterAllLeases.sentinel.effects == expected);
    CHECK(afterAllLeases.heapAllocCalls + afterAllLeases.sentinel.heapAllocCalls == 2);
    const auto stillStandalone = reacquired->resumeContextPrefix();
    CHECK(stillStandalone.context.stop == FeatureMapProxyStop::ContextOwnerUnavailable);
    CHECK(stillStandalone.context.candidateContextIdentity == nullptr && stillStandalone.context.effectCount == 0);
    CHECK(stillStandalone.candidateMapConstructorReturned && stillStandalone.sentinel.candidateHeadIdentity == sentinel.candidateHeadIdentity);
    CHECK(stillStandalone.heapAllocCalls + stillStandalone.sentinel.heapAllocCalls == 2);
    CHECK(runtime->prepareOwnedTestMapProxy(runtime).status == FeatureMapProxyPrepareStatus::ExperimentCapacityUnavailable);
    CHECK(runtime->prepareOwnedTestContextPrefix(runtime).status == FeatureMapProxyPrepareStatus::ExperimentCapacityUnavailable);
    CHECK(runtime->report().heapCreateCalls == producerBefore.heapCreateCalls);
    std::printf("[candidate map constructor] map=%p proxy=%p head=%p bytes=%zu flags=%u sentinel-alloc=%u stores=%zu returned-map=1 process-retained=1\n",
                const_cast<void*>(complete.candidateMapIdentity), const_cast<void*>(complete.candidateProxyIdentity),
                const_cast<void*>(sentinel.candidateHeadIdentity), sentinel.heapAllocBytesArgument,
                sentinel.heapAllocFlagsArgument, sentinel.heapAllocCalls, sentinel.effectCount);
    const auto cleared = reacquired->resumeEmptyMapCleanupPrefix();
    const auto& clear = cleared.emptyClear;
    REQUIRE(clear.started && clear.prefixReturned && clear.fullRangeSelected && clear.emptyRootSkipped);
    CHECK(cleared.stage == FeatureMapProxyStage::EmptyMapClearReturned);
    CHECK(cleared.stop == FeatureMapProxyStop::SentinelFreeUnavailable && clear.stop == cleared.stop);
    CHECK(clear.origin == FeatureCrtOrigin::ActualWin32);
    CHECK(clear.mapIdentity == complete.candidateMapIdentity && clear.proxyIdentity == complete.candidateProxyIdentity);
    CHECK(clear.headIdentity == sentinel.candidateHeadIdentity && clear.rootIdentity == clear.headIdentity);
    CHECK(clear.rootNil == 1 && clear.countAfterStore == 0);
    for (std::size_t i = 0; i < 3; ++i) {
        CHECK(clear.headReloadValues[i] == clear.headIdentity);
        CHECK(clear.selfLinkValues[i] == clear.headIdentity);
    }
    CHECK(clear.outputHeadIdentity == clear.headIdentity && clear.outputIteratorNode == clear.headIdentity);
    CHECK(clear.outputIteratorOwner == clear.proxyIdentity && clear.sentinelFreeArgument == clear.headIdentity);
    constexpr std::array<FeatureMapEmptyClearEvent, 14> clearExpected{{
        {0x1412f0715,FeatureMapEmptyClearEffect::FullRangeSelected},
        {0x1412ee431,FeatureMapEmptyClearEffect::EmptyRootSkipped},
        {0x1412ee453,FeatureMapEmptyClearEffect::HeadReloadFor8},
        {0x1412ee45c,FeatureMapEmptyClearEffect::Self8Stored},
        {0x1412ee460,FeatureMapEmptyClearEffect::HeadReloadFor0},
        {0x1412ee464,FeatureMapEmptyClearEffect::CountZeroStored},
        {0x1412ee46c,FeatureMapEmptyClearEffect::Self0Stored},
        {0x1412ee46f,FeatureMapEmptyClearEffect::HeadReloadFor10},
        {0x1412ee478,FeatureMapEmptyClearEffect::Self10Stored},
        {0x1412ee481,FeatureMapEmptyClearEffect::ClearReturned},
        {0x1412f071a,FeatureMapEmptyClearEffect::OutputHeadReloaded},
        {0x1412f0721,FeatureMapEmptyClearEffect::OutputNodeStored},
        {0x1412f072a,FeatureMapEmptyClearEffect::OutputOwnerStored},
        {0x1412f08ac,FeatureMapEmptyClearEffect::SentinelFreeArgumentLoaded}
    }};
    REQUIRE(clear.effectCount == clearExpected.size());
    for (std::size_t i = 0; i < clearExpected.size(); ++i) {
        CHECK(clear.effects[i].sourceSite == clearExpected[i].sourceSite);
        CHECK(clear.effects[i].effect == clearExpected[i].effect);
    }
    CHECK(cleared.effects == complete.effects && cleared.sentinel.effects == complete.sentinel.effects);
    CHECK(cleared.heapAllocCalls + cleared.sentinel.heapAllocCalls == 2);
    CHECK(!cleared.sentinel.keyPayloadKnown && !cleared.sentinel.suffixKnown);
    CHECK(!cleared.context.optionalOwner1450Known && !cleared.context.candidateContextConstructorReturned);
    REQUIRE(clear.nextSourceSite);
    CHECK(std::strcmp(clear.nextSourceSite, "0x1412f08b0:sentinel free unavailable") == 0);
    const auto clearAgain = reacquired->resumeEmptyMapCleanupPrefix();
    CHECK(clearAgain.emptyClear.effectCount == clear.effectCount);
    for (std::size_t i = 0; i < clear.effectCount; ++i) {
        CHECK(clearAgain.emptyClear.effects[i].sourceSite == clear.effects[i].sourceSite);
        CHECK(clearAgain.emptyClear.effects[i].effect == clear.effects[i].effect);
    }
    CHECK(reacquired->resume().stage == FeatureMapProxyStage::EmptyMapClearReturned);
    CHECK(reacquired->resumeSentinel().stage == FeatureMapProxyStage::EmptyMapClearReturned);
    const auto noAdoption = reacquired->resumeContextPrefix();
    CHECK(noAdoption.context.stop == FeatureMapProxyStop::MapCleanupStarted);
    CHECK(noAdoption.context.effectCount == 0 && noAdoption.context.candidateContextIdentity == nullptr);
    CHECK(noAdoption.stop == FeatureMapProxyStop::SentinelFreeUnavailable);
    CHECK(noAdoption.candidateMapIdentity == clear.mapIdentity && noAdoption.sentinel.candidateHeadIdentity == clear.headIdentity);
    CHECK(runtime->report().heapCreateCalls == producerBefore.heapCreateCalls);
    const auto headToken = reinterpret_cast<std::uintptr_t>(clear.headIdentity); // Before retirement.
    const auto freed = reacquired->resumeSentinelFreePrefix();
    const auto& free = freed.sentinelFree;
    REQUIRE(free.heapFreeReturned && free.heapFreeResult.has_value() && *free.heapFreeResult != 0);
    CHECK(free.origin == FeatureCrtOrigin::ActualWin32 && free.started && free.sourcePointerNonNull == true);
    CHECK(free.candidateSlotReads == 1 && free.heapFreeCalls == 1);
    CHECK(free.candidateSlotReadIdentity == sentinel.heapAllocHeapArgument);
    CHECK(free.heapFreeHeapArgument == free.candidateSlotReadIdentity && free.heapFreeFlagsArgument == 0);
    CHECK(free.heapFreePointerArgument == headToken && free.mapIdentity == clear.mapIdentity);
    CHECK(free.sourceFreeReturned && free.sentinelRetired && free.headNullStored && free.observedMapHead == nullptr);
    CHECK(free.countAfterStore == 0 && free.mapInnerDestructorReturned && free.proxyFreeArgument == clear.proxyIdentity);
    CHECK(freed.stage == FeatureMapProxyStage::SentinelFreeReturned && freed.stop == FeatureMapProxyStop::ProxyFreeUnavailable);
    constexpr std::array<FeatureMapSentinelFreeEvent, 10> freeExpected{{
        {0x141471b7c,FeatureMapSentinelFreeEffect::PointerNonNullTested},
        {0x141471b89,FeatureMapSentinelFreeEffect::CandidateHeapSlotRead},
        {0x141471b92,FeatureMapSentinelFreeEffect::HeapFreeEntered},
        {0x141471b98,FeatureMapSentinelFreeEffect::HeapFreeReturned},
        {0x141471b9a,FeatureMapSentinelFreeEffect::HeapFreeResultTested},
        {0x141471bb8,FeatureMapSentinelFreeEffect::SourceFreeReturned},
        {0x1412f08b8,FeatureMapSentinelFreeEffect::MapHeadNullStored},
        {0x1412f08bc,FeatureMapSentinelFreeEffect::MapCountZeroStored},
        {0x1412f08c5,FeatureMapSentinelFreeEffect::MapInnerDestructorReturned},
        {0x1412f0d0c,FeatureMapSentinelFreeEffect::ProxyFreeArgumentLoaded}
    }};
    REQUIRE(free.effectCount == freeExpected.size());
    for (std::size_t i = 0; i < freeExpected.size(); ++i) {
        CHECK(free.effects[i].sourceSite == freeExpected[i].sourceSite);
        CHECK(free.effects[i].effect == freeExpected[i].effect);
    }
    CHECK(freed.effects == complete.effects && freed.sentinel.effects == complete.sentinel.effects);
    CHECK(freed.emptyClear.effectCount == clear.effectCount); // Earlier clear is historical only.
    CHECK(freed.heapAllocCalls + freed.sentinel.heapAllocCalls == 2);
    CHECK(reacquired->resumeSentinelFreePrefix().sentinelFree.heapFreeCalls == 1);
    CHECK(reacquired->resumeEmptyMapCleanupPrefix().stage == FeatureMapProxyStage::SentinelFreeReturned);
    CHECK(reacquired->resume().stage == FeatureMapProxyStage::SentinelFreeReturned);
    CHECK(reacquired->resumeSentinel().stage == FeatureMapProxyStage::SentinelFreeReturned);
    CHECK(reacquired->resumeContextPrefix().context.stop == FeatureMapProxyStop::MapCleanupStarted);
    REQUIRE(free.nextSourceSite);
    CHECK(std::strcmp(free.nextSourceSite, "0x1412f0d14:isolated map proxy free unavailable") == 0);
    standaloneAfterMapDestructor = reacquired;
    // No H field/pointer dereference or post-free heap query occurs after success.
    const auto proxyToken = reinterpret_cast<std::uintptr_t>(clear.proxyIdentity); // A is still live here.
    const auto mapToken = reinterpret_cast<std::uintptr_t>(clear.mapIdentity);
    const auto proxyFreed = reacquired->resumeProxyFreePrefix();
    const auto& proxyFree = proxyFreed.proxyFree;
    REQUIRE(proxyFree.heapFreeReturned && proxyFree.heapFreeResult.has_value() && *proxyFree.heapFreeResult != 0);
    CHECK(proxyFree.origin == FeatureCrtOrigin::ActualWin32 && proxyFree.started && proxyFree.sourcePointerNonNull == true);
    CHECK(proxyFree.candidateSlotReads == 1 && proxyFree.heapFreeCalls == 1);
    CHECK(proxyFree.heapFreePointerArgument == proxyToken && proxyFree.mapProxyTokenBefore == proxyToken);
    CHECK(proxyFree.mapIdentityToken == mapToken && proxyFree.backpointerTokenBefore == mapToken);
    CHECK(proxyFree.candidateSlotReadIdentity == first.heapAllocHeapArgument);
    CHECK(proxyFree.heapFreeHeapArgument == proxyFree.candidateSlotReadIdentity && proxyFree.heapFreeFlagsArgument == 0);
    CHECK(proxyFree.sourceFreeReturned && proxyFree.proxyRetired && proxyFree.candidateMapDestructorReturned);
    CHECK(proxyFreed.stage == FeatureMapProxyStage::MapDestructorReturned);
    CHECK(proxyFreed.stop == FeatureMapProxyStop::EnclosingLifecycleUnavailable);
    constexpr std::array<FeatureMapProxyFreeEvent, 6> proxyExpected{{
        {0x141471b7c,FeatureMapProxyFreeEffect::PointerNonNullTested},
        {0x141471b89,FeatureMapProxyFreeEffect::CandidateHeapSlotRead},
        {0x141471b92,FeatureMapProxyFreeEffect::HeapFreeEntered},
        {0x141471b98,FeatureMapProxyFreeEffect::HeapFreeReturned},
        {0x141471b9a,FeatureMapProxyFreeEffect::HeapFreeResultTested},
        {0x141471bb8,FeatureMapProxyFreeEffect::TailDestructorReturned}
    }};
    REQUIRE(proxyFree.effectCount == proxyExpected.size());
    for (std::size_t i = 0; i < proxyExpected.size(); ++i) {
        CHECK(proxyFree.effects[i].sourceSite == proxyExpected[i].sourceSite);
        CHECK(proxyFree.effects[i].effect == proxyExpected[i].effect);
    }
    CHECK(proxyFreed.heapAllocCalls + proxyFreed.sentinel.heapAllocCalls == 2);
    CHECK(proxyFreed.sentinelFree.heapFreeCalls == 1 && proxyFreed.sentinelFree.sentinelRetired);
    CHECK(reacquired->resumeProxyFreePrefix().proxyFree.heapFreeCalls == 1);
    CHECK(reacquired->resumeSentinelFreePrefix().stage == FeatureMapProxyStage::MapDestructorReturned);
    CHECK(reacquired->resumeEmptyMapCleanupPrefix().stage == FeatureMapProxyStage::MapDestructorReturned);
    CHECK(reacquired->resume().stage == FeatureMapProxyStage::MapDestructorReturned);
    CHECK(reacquired->resumeSentinel().stage == FeatureMapProxyStage::MapDestructorReturned);
    CHECK(reacquired->resumeContextPrefix().context.stop == FeatureMapProxyStop::MapCleanupStarted);
    REQUIRE(proxyFree.nextSourceSite);
    CHECK(std::strcmp(proxyFree.nextSourceSite, "0x141471bb8:isolated map destructor returned; enclosing lifecycle unavailable") == 0);
    // H/A and the source M lifetime are retired. No pointer fields are inspected.
    const auto lateHandler = runtime->resumeEncodedNewHandlerPrefix(runtime);
    CHECK(lateHandler.stop == FeatureCrtEncodedHandlerStop::NextInitializerUnavailable);
    CHECK(lateHandler.encodePointerCalls == 1 && lateHandler.candidateRoleStores == 1 && lateHandler.candidateRoleKnown);
    CHECK(lateHandler.effectCount == firstHandler.effectCount && lateHandler.encodedNullResult == firstHandler.encodedNullResult);
    const auto lateTail = runtime->resumeEncodedNullRoleTail(runtime);
    CHECK(lateTail.stop == FeatureCrtNullRoleTailStop::ChildFrameAlreadyPrepared);
    CHECK(!lateTail.started && !lateTail.savedArgumentKnown && lateTail.roleStores == 0 && lateTail.effectCount == 0);
    for (bool known : lateTail.roleKnown) CHECK(!known);
    CHECK(runtime->encodedNullRoleTailReport().effectCount == 0);
    CHECK(reacquired->report().stage == FeatureMapProxyStage::MapDestructorReturned);
}

TEST_CASE(standalone_retired_map_history_survives_all_inspection_leases)
{
    REQUIRE(!standaloneAfterMapDestructor.expired());
    const auto retained = standaloneAfterMapDestructor.lock(); REQUIRE(retained);
    const auto report = retained->resumeSentinelFreePrefix();
    CHECK(report.sentinelFree.sentinelRetired && report.sentinelFree.heapFreeCalls == 1);
    CHECK(report.proxyFree.proxyRetired && report.proxyFree.heapFreeCalls == 1 && report.proxyFree.candidateMapDestructorReturned);
    CHECK(report.stage == FeatureMapProxyStage::MapDestructorReturned && report.stop == FeatureMapProxyStop::EnclosingLifecycleUnavailable);
    CHECK(retained->resumeEmptyMapCleanupPrefix().sentinelFree.effectCount == 10);
    CHECK(retained->resumeProxyFreePrefix().proxyFree.effectCount == 6);
}

TEST_CASE(actual_heap_owner_is_retained_after_local_inspection_leases_end)
{
    std::weak_ptr<FeatureCrtRuntime> weak;
    const void* identity = nullptr;
    {
        const auto lease = FeatureCrtRuntime::prepareOwnedTestProcess();
        const auto report = lease->attemptProducer();
        REQUIRE(report.heapNonNull.has_value() && *report.heapNonNull);
        identity = lease.get(); weak = lease;
    }
    CHECK(!weak.expired()); // Explicit host process retention, not native teardown.
    const auto lease = FeatureCrtRuntime::prepareOwnedTestProcess();
    CHECK(lease.get() == identity && lease->report().ownedTestProcessRetention);
    CHECK(lease->report().heapCreateCalls == 1);
}

int main() { return testkit::run_all(); }
