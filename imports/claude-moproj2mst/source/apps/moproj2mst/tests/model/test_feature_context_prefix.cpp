// ctest-labels: m2m-model feature-context ownership
// One isolated context receiver in its own process; no Project/native Close.
#include <testkit/check.hpp>
#include "../../src/model/feature_crt_runtime.hpp"
#include <array>
#include <cstdio>
#include <cstring>
#include <memory>

using namespace m2m::detail;
static_assert(!FeatureContextPrefixReport::originalGlobalObserved);
static_assert(!FeatureContextPrefixReport::projectBindingAdmitted);
static_assert(!FeatureContextPrefixReport::nativeCleanupPerformed && !FeatureContextPrefixReport::completeForPublicClose);

namespace {
std::weak_ptr<FeatureMapProxyCall> contextAfterMapDestructor;
// Golden store ledger transcribed from retained12f10d0/12e6fb0/12e6ff0/12e7020.
constexpr std::array<FeatureContextStore, 55> expected{{
    {0x1412f10f0,0x1498,4,0xffffffffu},{0x1412f10fa,0x3c,4,0},{0x1412f10fd,0x40,4,0},
    {0x1412e6fb3,0x00,1,0},{0x1412e6fb5,0x01,1,0},{0x1412e6fb8,0x02,1,0},{0x1412e6fbb,0x03,1,0},
    {0x1412e6fbe,0x04,1,0},{0x1412e6fc1,0x34,4,0},{0x1412e6fc4,0x05,1,0},{0x1412e6fc7,0x06,1,0},
    {0x1412e6fca,0x07,1,0},{0x1412e6fcd,0x08,1,0},{0x1412e6fd0,0x09,1,0},{0x1412e6fd3,0x0a,1,0},
    {0x1412e6fd6,0x0b,1,0},{0x1412e6fd9,0x2f,1,0},{0x1412e6fdc,0x30,1,0},{0x1412e6fdf,0x38,1,0},
    {0x1412e6fe2,0x39,1,0},{0x1412e6fe5,0x3a,1,0},
    {0x1412e6ff0,0x0c,1,0},{0x1412e6ff3,0x0e,1,0},{0x1412e6ff6,0x12,1,0},{0x1412e6ff9,0x14,1,0},
    {0x1412e6ffc,0x16,1,0},{0x1412e6fff,0x18,1,0},{0x1412e7002,0x1a,1,0},{0x1412e7005,0x1c,1,0},
    {0x1412e7008,0x1e,1,0},{0x1412e700b,0x20,1,0},{0x1412e700e,0x22,1,0},{0x1412e7011,0x24,1,0},
    {0x1412e7014,0x26,1,0},{0x1412e7017,0x28,1,0},
    {0x1412e7020,0x0d,1,0},{0x1412e7023,0x0f,1,0},{0x1412e7026,0x13,1,0},{0x1412e7029,0x15,1,0},
    {0x1412e702c,0x17,1,0},{0x1412e702f,0x19,1,0},{0x1412e7032,0x1b,1,0},{0x1412e7035,0x1d,1,0},
    {0x1412e7038,0x1f,1,0},{0x1412e703b,0x21,1,0},{0x1412e703e,0x23,1,0},{0x1412e7041,0x25,1,0},
    {0x1412e7044,0x27,1,0},{0x1412e7047,0x29,1,0},{0x1412e704a,0x2a,1,0},{0x1412e704d,0x2b,1,0},
    {0x1412e7050,0x2c,1,0},{0x1412e7053,0x2d,1,0},{0x1412e7056,0x2e,1,0},
    {0x1412f111a,0x44,4,0}
}};
void checkStores(const FeatureContextPrefixReport& report)
{
    REQUIRE(report.effectCount == expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
        CHECK(report.effects[i].sourceSite == expected[i].sourceSite);
        CHECK(report.effects[i].offset == expected[i].offset);
        CHECK(report.effects[i].width == expected[i].width);
        CHECK(report.effects[i].observedValue == expected[i].observedValue);
    }
}
void checkUnknownOuterFields(const FeatureContextPrefixReport& report)
{
    CHECK(!report.byteKnown[0x10] && !report.byteKnown[0x11]);
    CHECK(!report.byteKnown[0x31] && !report.byteKnown[0x32] && !report.byteKnown[0x33] && !report.byteKnown[0x3b]);
    CHECK(!report.stringsKnown && !report.fallback1448Known && !report.optionalOwner1450Known);
    CHECK(!report.stringRoutineEntered && !report.candidateContextConstructorReturned);
}
}

TEST_CASE(isolated_context_owns_embedded_map_before_allocations_and_commits_pre_string_prefix_once)
{
    const auto runtime = FeatureCrtRuntime::prepareOwnedTestProcess(); REQUIRE(runtime);
    CHECK(runtime->prepareOwnedTestContextPrefix(runtime).status == FeatureMapProxyPrepareStatus::ProducerUnavailable);
    CHECK(runtime->report().heapCreateCalls == 0);
    const std::shared_ptr<FeatureCrtRuntime> empty;
    const std::shared_ptr<FeatureCrtRuntime> foreign(runtime.get(), [](FeatureCrtRuntime*) {});
    CHECK(runtime->prepareOwnedTestContextPrefix(empty).status == FeatureMapProxyPrepareStatus::OwnerMismatch);
    CHECK(runtime->prepareOwnedTestContextPrefix(foreign).status == FeatureMapProxyPrepareStatus::OwnerMismatch);
    const auto producer = runtime->attemptProducer();
    REQUIRE(producer.producerReturn == 1u && producer.heapNonNull == true);
    auto prepared = runtime->prepareOwnedTestContextPrefix(runtime);
    REQUIRE(prepared.status == FeatureMapProxyPrepareStatus::Prepared && prepared.call);
    const auto initial = prepared.call->report();
    REQUIRE(initial.context.profile == FeatureContextReceiverProfile::ContextPrefix);
    REQUIRE(initial.context.candidateContextIdentity && initial.context.candidateEmbeddedMapIdentity);
    const auto contextAddress = reinterpret_cast<std::uintptr_t>(initial.context.candidateContextIdentity);
    CHECK(contextAddress + 0x1458 == reinterpret_cast<std::uintptr_t>(initial.candidateMapIdentity));
    CHECK(initial.context.candidateEmbeddedMapIdentity == initial.candidateMapIdentity);
    CHECK(initial.heapAllocCalls == 0 && initial.sentinel.heapAllocCalls == 0 && initial.context.effectCount == 0);
    CHECK(!initial.context.cache1498 && !initial.context.word3c && !initial.context.word40 && !initial.context.word44);
    for (bool known : initial.context.byteKnown) CHECK(!known);
    checkUnknownOuterFields(initial.context);
    const auto tooEarly = prepared.call->resumeContextPrefix();
    const auto clearTooEarly = prepared.call->resumeEmptyMapCleanupPrefix();
    const auto freeTooEarly = prepared.call->resumeSentinelFreePrefix();
    const auto proxyTooEarly = prepared.call->resumeProxyFreePrefix();
    CHECK(!proxyTooEarly.proxyFree.started && proxyTooEarly.proxyFree.heapFreeCalls == 0);
    CHECK(proxyTooEarly.proxyFree.stop == FeatureMapProxyStop::ProxyFreePrerequisiteUnavailable);
    CHECK(!freeTooEarly.sentinelFree.started && freeTooEarly.sentinelFree.heapFreeCalls == 0);
    CHECK(freeTooEarly.sentinelFree.stop == FeatureMapProxyStop::SentinelFreePrerequisiteUnavailable);
    CHECK(clearTooEarly.emptyClear.stop == FeatureMapProxyStop::EmptyMapPrerequisiteUnavailable);
    CHECK(!clearTooEarly.emptyClear.started && clearTooEarly.emptyClear.effectCount == 0);
    CHECK(tooEarly.context.stop == FeatureMapProxyStop::ChildMapPending && tooEarly.context.effectCount == 0);
    CHECK(tooEarly.heapAllocCalls == 0 && tooEarly.sentinel.heapAllocCalls == 0);
    const auto proxy = prepared.call->resume();
    REQUIRE(proxy.stage == FeatureMapProxyStage::SentinelPending && proxy.allocationNonNull == true);
    CHECK(proxy.candidateMapIdentity == initial.candidateMapIdentity && proxy.observedProxyBackpointer == initial.candidateMapIdentity);
    const auto afterProxy = prepared.call->resumeContextPrefix();
    CHECK(afterProxy.context.stop == FeatureMapProxyStop::ChildMapPending && afterProxy.context.effectCount == 0);
    CHECK(!afterProxy.context.cache1498 && !afterProxy.context.word3c && afterProxy.heapAllocCalls == 1);
    const auto map = prepared.call->resumeSentinel();
    REQUIRE(map.candidateMapConstructorReturned && map.sentinel.allocationNonNull == true);
    CHECK(map.sentinel.returnedMapIdentity == initial.context.candidateEmbeddedMapIdentity);
    CHECK(map.context.effectCount == 0 && !map.context.cache1498 && !map.context.prefixCompleted);
    const auto result = prepared.call->resumeContextPrefix();
    const auto& context = result.context;
    REQUIRE(context.prefixCompleted);
    CHECK(context.origin == FeatureCrtOrigin::ActualWin32 && context.candidateContextIdentity == initial.context.candidateContextIdentity);
    CHECK(context.candidateEmbeddedMapIdentity == initial.candidateMapIdentity);
    CHECK(context.resetContextArgument == context.candidateContextIdentity && context.resetDlArgumentKnown && context.resetDlArgument == 0);
    CHECK(context.cache1498 == -1 && context.word3c == 0 && context.word40 == 0 && context.word44 == 0);
    checkStores(context);
    for (std::size_t i = 0; i < context.byteKnown.size(); ++i) {
        const bool known = i <= 0x0f || (i >= 0x12 && i <= 0x30) || (i >= 0x34 && i <= 0x3a) || i >= 0x3c;
        CHECK(context.byteKnown[i] == known);
        if (known) CHECK(context.observedBytes[i] == 0); // No unknown backing byte is read.
    }
    checkUnknownOuterFields(context);
    CHECK(result.stage == FeatureMapProxyStage::ContextPrefixComplete && result.stop == FeatureMapProxyStop::StringRoutineUnavailable);
    CHECK(context.stop == FeatureMapProxyStop::StringRoutineUnavailable);
    CHECK(reinterpret_cast<std::uintptr_t>(context.nextStringDestination) == contextAddress + 0x48);
    REQUIRE(context.nextSourceSite);
    CHECK(std::strcmp(context.nextSourceSite, "0x1412f111d:0x14146de38 first string routine") == 0);
    CHECK(result.candidateProxyIdentity == proxy.candidateProxyIdentity && result.sentinel.candidateHeadIdentity == map.sentinel.candidateHeadIdentity);
    CHECK(result.effects == proxy.effects && result.sentinel.effects == map.sentinel.effects);
    CHECK(result.heapAllocCalls == 1 && result.sentinel.heapAllocCalls == 1);
    const auto repeated = prepared.call->resumeContextPrefix();
    checkStores(repeated.context);
    CHECK(repeated.context.byteKnown == context.byteKnown && repeated.context.observedBytes == context.observedBytes);
    CHECK(prepared.call->resume().stage == FeatureMapProxyStage::ContextPrefixComplete);
    CHECK(prepared.call->resumeSentinel().stage == FeatureMapProxyStage::ContextPrefixComplete);
    std::weak_ptr<FeatureMapProxyCall> weak = prepared.call;
    prepared.call.reset(); REQUIRE(!weak.expired());
    const auto retained = weak.lock(); REQUIRE(retained);
    const auto afterLease = retained->resumeContextPrefix();
    checkStores(afterLease.context);
    CHECK(afterLease.context.candidateContextIdentity == context.candidateContextIdentity);
    CHECK(afterLease.candidateMapIdentity == initial.candidateMapIdentity && afterLease.candidateProxyIdentity == proxy.candidateProxyIdentity);
    CHECK(afterLease.sentinel.candidateHeadIdentity == map.sentinel.candidateHeadIdentity);
    CHECK(afterLease.heapAllocCalls + afterLease.sentinel.heapAllocCalls == 2);
    CHECK(runtime->prepareOwnedTestContextPrefix(runtime).status == FeatureMapProxyPrepareStatus::ExperimentCapacityUnavailable);
    CHECK(runtime->prepareOwnedTestMapProxy(runtime).status == FeatureMapProxyPrepareStatus::ExperimentCapacityUnavailable);
    CHECK(runtime->prepareOwnedTestContextPrefix(foreign).status == FeatureMapProxyPrepareStatus::OwnerMismatch);
    const auto producerAfter = runtime->report();
    CHECK(producerAfter.effects == producer.effects && producerAfter.heapCreateCalls == 1 && producerAfter.compatibilityCalls == 1);
    REQUIRE(context.cache1498.has_value());
    std::printf("[candidate context prefix] context=%p map=%p proxy=%p head=%p stores=%zu cache=%d first-string-guard=1 process-retained=1\n",
                const_cast<void*>(context.candidateContextIdentity), const_cast<void*>(result.candidateMapIdentity),
                const_cast<void*>(result.candidateProxyIdentity), const_cast<void*>(result.sentinel.candidateHeadIdentity),
                context.effectCount, *context.cache1498);
    const auto clearResult = retained->resumeEmptyMapCleanupPrefix();
    const auto& clear = clearResult.emptyClear;
    REQUIRE(clear.started && clear.prefixReturned && clear.effectCount == 14);
    CHECK(clearResult.stage == FeatureMapProxyStage::EmptyMapClearReturned);
    CHECK(clear.mapIdentity == result.candidateMapIdentity && clear.proxyIdentity == result.candidateProxyIdentity);
    CHECK(clear.headIdentity == result.sentinel.candidateHeadIdentity && clear.rootIdentity == clear.headIdentity);
    CHECK(clear.outputIteratorNode == clear.headIdentity && clear.outputIteratorOwner == clear.proxyIdentity);
    CHECK(clear.countAfterStore == 0 && clear.sentinelFreeArgument == clear.headIdentity);
    CHECK(clear.effects[3].effect == FeatureMapEmptyClearEffect::Self8Stored);
    CHECK(clear.effects[5].effect == FeatureMapEmptyClearEffect::CountZeroStored);
    CHECK(clear.effects[6].effect == FeatureMapEmptyClearEffect::Self0Stored);
    CHECK(clear.effects[8].effect == FeatureMapEmptyClearEffect::Self10Stored);
    checkStores(clearResult.context); // Historical context initialization is retained, not replayed.
    checkUnknownOuterFields(clearResult.context);
    CHECK(clearResult.context.candidateContextIdentity == context.candidateContextIdentity);
    CHECK(clearResult.heapAllocCalls + clearResult.sentinel.heapAllocCalls == 2);
    CHECK(clearResult.effects == result.effects && clearResult.sentinel.effects == result.sentinel.effects);
    const auto cannotReplayContext = retained->resumeContextPrefix();
    CHECK(cannotReplayContext.context.stop == FeatureMapProxyStop::MapCleanupStarted);
    CHECK(cannotReplayContext.stage == FeatureMapProxyStage::EmptyMapClearReturned);
    CHECK(cannotReplayContext.stop == FeatureMapProxyStop::SentinelFreeUnavailable);
    checkStores(cannotReplayContext.context);
    const auto clearAgain = retained->resumeEmptyMapCleanupPrefix();
    CHECK(clearAgain.emptyClear.effectCount == clear.effectCount);
    CHECK(retained->resume().stage == FeatureMapProxyStage::EmptyMapClearReturned);
    CHECK(retained->resumeSentinel().stage == FeatureMapProxyStage::EmptyMapClearReturned);
    CHECK(clearAgain.context.candidateContextIdentity == context.candidateContextIdentity);
    CHECK(clearAgain.candidateMapIdentity == result.candidateMapIdentity && clearAgain.sentinel.candidateHeadIdentity == clear.headIdentity);
    const auto headToken = reinterpret_cast<std::uintptr_t>(clear.headIdentity); // Before retirement.
    const auto freeResult = retained->resumeSentinelFreePrefix();
    const auto& free = freeResult.sentinelFree;
    REQUIRE(free.heapFreeReturned && free.heapFreeResult.has_value() && *free.heapFreeResult != 0);
    CHECK(free.started && free.sourcePointerNonNull == true && free.heapFreeCalls == 1);
    CHECK(free.heapFreePointerArgument == headToken && free.heapFreeFlagsArgument == 0);
    CHECK(free.candidateSlotReads == 1 && free.candidateSlotReadIdentity == result.sentinel.heapAllocHeapArgument);
    CHECK(free.heapFreeHeapArgument == free.candidateSlotReadIdentity);
    CHECK(free.sourceFreeReturned && free.sentinelRetired && free.headNullStored && free.countAfterStore == 0);
    CHECK(free.mapInnerDestructorReturned && free.proxyFreeArgument == result.candidateProxyIdentity);
    CHECK(freeResult.stage == FeatureMapProxyStage::SentinelFreeReturned && freeResult.stop == FeatureMapProxyStop::ProxyFreeUnavailable);
    REQUIRE(free.effectCount == 10);
    CHECK(free.effects[5].effect == FeatureMapSentinelFreeEffect::SourceFreeReturned);
    CHECK(free.effects[6].effect == FeatureMapSentinelFreeEffect::MapHeadNullStored);
    CHECK(free.effects[7].effect == FeatureMapSentinelFreeEffect::MapCountZeroStored);
    checkStores(freeResult.context); checkUnknownOuterFields(freeResult.context);
    CHECK(freeResult.context.candidateContextIdentity == context.candidateContextIdentity);
    CHECK(freeResult.heapAllocCalls + freeResult.sentinel.heapAllocCalls == 2);
    CHECK(retained->resumeSentinelFreePrefix().sentinelFree.heapFreeCalls == 1);
    CHECK(retained->resumeEmptyMapCleanupPrefix().stage == FeatureMapProxyStage::SentinelFreeReturned);
    CHECK(retained->resume().stage == FeatureMapProxyStage::SentinelFreeReturned);
    CHECK(retained->resumeSentinel().stage == FeatureMapProxyStage::SentinelFreeReturned);
    const auto stillBlocked = retained->resumeContextPrefix();
    CHECK(stillBlocked.context.stop == FeatureMapProxyStop::MapCleanupStarted);
    checkStores(stillBlocked.context);
    contextAfterMapDestructor = retained;
    // H is retired: all remaining observations are live M/A/C or host history.
    const auto proxyToken = reinterpret_cast<std::uintptr_t>(free.proxyFreeArgument); // Before A retirement.
    const auto mapToken = reinterpret_cast<std::uintptr_t>(free.mapIdentity);
    const auto proxyResult = retained->resumeProxyFreePrefix();
    const auto& proxyFree = proxyResult.proxyFree;
    REQUIRE(proxyFree.heapFreeReturned && proxyFree.heapFreeResult.has_value() && *proxyFree.heapFreeResult != 0);
    CHECK(proxyFree.heapFreeCalls == 1 && proxyFree.candidateSlotReads == 1);
    CHECK(proxyFree.heapFreePointerArgument == proxyToken && proxyFree.mapProxyTokenBefore == proxyToken);
    CHECK(proxyFree.mapIdentityToken == mapToken && proxyFree.backpointerTokenBefore == mapToken);
    CHECK(proxyFree.heapFreeHeapArgument == result.heapAllocHeapArgument && proxyFree.heapFreeFlagsArgument == 0);
    CHECK(proxyFree.sourceFreeReturned && proxyFree.proxyRetired && proxyFree.candidateMapDestructorReturned);
    CHECK(proxyResult.stage == FeatureMapProxyStage::MapDestructorReturned && proxyResult.stop == FeatureMapProxyStop::EnclosingLifecycleUnavailable);
    REQUIRE(proxyFree.effectCount == 6);
    CHECK(proxyFree.effects[5].effect == FeatureMapProxyFreeEffect::TailDestructorReturned);
    CHECK(proxyFree.effects[5].sourceSite == 0x141471bb8);
    checkStores(proxyResult.context); checkUnknownOuterFields(proxyResult.context); // Host history only.
    CHECK(proxyResult.context.candidateContextIdentity == context.candidateContextIdentity);
    CHECK(proxyResult.sentinelFree.heapFreeCalls == 1 && proxyResult.heapAllocCalls + proxyResult.sentinel.heapAllocCalls == 2);
    CHECK(retained->resumeProxyFreePrefix().proxyFree.heapFreeCalls == 1);
    CHECK(retained->resumeSentinelFreePrefix().stage == FeatureMapProxyStage::MapDestructorReturned);
    CHECK(retained->resumeEmptyMapCleanupPrefix().stage == FeatureMapProxyStage::MapDestructorReturned);
    CHECK(retained->resumeContextPrefix().context.stop == FeatureMapProxyStop::MapCleanupStarted);
    // No A/H field or native M storage is read after logical map destruction.
}

TEST_CASE(embedded_host_context_history_survives_inspection_leases_after_map_destruction)
{
    REQUIRE(!contextAfterMapDestructor.expired());
    const auto retained = contextAfterMapDestructor.lock(); REQUIRE(retained);
    const auto report = retained->resumeSentinelFreePrefix();
    CHECK(report.sentinelFree.sentinelRetired && report.sentinelFree.heapFreeCalls == 1);
    CHECK(report.proxyFree.proxyRetired && report.proxyFree.heapFreeCalls == 1 && report.proxyFree.candidateMapDestructorReturned);
    CHECK(report.stage == FeatureMapProxyStage::MapDestructorReturned);
    CHECK(report.context.candidateContextIdentity != nullptr && report.context.prefixCompleted);
    checkStores(report.context); checkUnknownOuterFields(report.context);
    CHECK(retained->resumeContextPrefix().context.stop == FeatureMapProxyStop::MapCleanupStarted);
    CHECK(retained->resumeProxyFreePrefix().proxyFree.effectCount == 6);
}

int main() { return testkit::run_all(); }
