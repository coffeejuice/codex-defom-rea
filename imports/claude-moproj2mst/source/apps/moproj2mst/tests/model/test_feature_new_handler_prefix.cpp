// ctest-labels: m2m-model feature-context crt-startup
// Fresh owned test process: no child-map lifetime precedes this startup slice.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <testkit/check.hpp>
#include "../../src/model/feature_crt_runtime.hpp"
#include <array>
#include <cstring>

using namespace m2m::detail;
static_assert(!FeatureCrtEncodedHandlerReport::originalGlobalObserved);
static_assert(!FeatureCrtEncodedHandlerReport::hostCrtHandlerChanged && !FeatureCrtEncodedHandlerReport::handlerInvoked);
static_assert(!FeatureCrtEncodedHandlerReport::flsEntered && !FeatureCrtEncodedHandlerReport::lastErrorObserved);
static_assert(!FeatureCrtEncodedHandlerReport::completeCrtStartup && !FeatureCrtEncodedHandlerReport::completeForPublicClose);

namespace {
std::weak_ptr<FeatureCrtRuntime> handlerOwnerAfterInspection;
void checkCompleted(const FeatureCrtEncodedHandlerReport& report)
{
    CHECK(report.origin == FeatureCrtOrigin::ActualWin32);
    CHECK(report.encodePointerCalls == 1 && report.encodePointerReturned && report.encodePointerArgument == 0);
    CHECK(report.candidateRoleStores == 1 && report.candidateRoleKnown && report.firstSetterReturned);
    CHECK(report.observedCandidateRole == report.encodedNullResult);
    CHECK(report.stop == FeatureCrtEncodedHandlerStop::NextInitializerUnavailable);
    constexpr std::array<FeatureCrtEncodedHandlerEvent, 4> expected{{
        {0x1414813e2,FeatureCrtEncodedHandlerEffect::EncodePointerEntered},
        {0x14146e9ff,FeatureCrtEncodedHandlerEffect::EncodePointerReturned},
        {0x141481ce0,FeatureCrtEncodedHandlerEffect::CandidateRoleStored},
        {0x141481ce7,FeatureCrtEncodedHandlerEffect::FirstSetterReturned}
    }};
    REQUIRE(report.effectCount == expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
        CHECK(report.effects[i].sourceSite == expected[i].sourceSite);
        CHECK(report.effects[i].effect == expected[i].effect);
    }
    REQUIRE(report.nextSourceSite);
    CHECK(std::strcmp(report.nextSourceSite, "0x14146ea0d:0x141481ca8 next encoded-role initializer unavailable") == 0);
}
}

TEST_CASE(encoded_handler_requires_canonical_fresh_heap_owner_then_stores_actual_null_encoding_once)
{
    const auto runtime = FeatureCrtRuntime::prepareOwnedTestProcess(); REQUIRE(runtime);
    const auto initial = runtime->encodedNewHandlerReport();
    CHECK(initial.runtimeIdentity == runtime.get() && initial.processId == GetCurrentProcessId());
    CHECK(initial.stop == FeatureCrtEncodedHandlerStop::NotStarted && !initial.candidateRoleKnown);
    CHECK(initial.encodePointerCalls == 0 && initial.candidateRoleStores == 0 && initial.effectCount == 0);
    const std::shared_ptr<FeatureCrtRuntime> absent;
    const std::shared_ptr<FeatureCrtRuntime> foreign(runtime.get(), [](FeatureCrtRuntime*) {});
    CHECK(runtime->resumeEncodedNewHandlerPrefix(absent).stop == FeatureCrtEncodedHandlerStop::OwnerMismatch);
    CHECK(runtime->resumeEncodedNewHandlerPrefix(foreign).stop == FeatureCrtEncodedHandlerStop::OwnerMismatch);
    CHECK(runtime->encodedNewHandlerReport().stop == FeatureCrtEncodedHandlerStop::NotStarted);
    const auto premature = runtime->resumeEncodedNewHandlerPrefix(runtime);
    CHECK(premature.stop == FeatureCrtEncodedHandlerStop::ProducerUnavailable);
    CHECK(!premature.candidateRoleKnown && premature.encodePointerCalls == 0 && premature.effectCount == 0);
    CHECK(runtime->report().heapCreateCalls == 0); // No implicit producer.
    const auto producer = runtime->attemptProducer();
    REQUIRE(producer.heapNonNull == true && producer.producerReturn == 1u);
    const auto result = runtime->resumeEncodedNewHandlerPrefix(runtime);
    checkCompleted(result);
    CHECK(result.runtimeIdentity == runtime.get() && result.processId == producer.processId);
    CHECK(result.moduleIdentity == producer.moduleIdentity);
    // Test-only round trip after the source prefix; production never decodes or
    // invokes a handler. A numeric-zero encoding is equally admissible.
    CHECK(DecodePointer(reinterpret_cast<void*>(result.encodedNullResult)) == nullptr);
    checkCompleted(runtime->resumeEncodedNewHandlerPrefix(runtime));
    checkCompleted(runtime->encodedNewHandlerReport());
    const auto after = runtime->report();
    CHECK(after.effects == producer.effects && after.heapCreateCalls == producer.heapCreateCalls);
    CHECK(after.compatibilityCalls == producer.compatibilityCalls && after.producerReturn == producer.producerReturn);
    const auto wrongAfter = runtime->resumeEncodedNewHandlerPrefix(foreign);
    CHECK(wrongAfter.stop == FeatureCrtEncodedHandlerStop::OwnerMismatch && wrongAfter.candidateRoleStores == 1);
    checkCompleted(runtime->encodedNewHandlerReport()); // Foreign lease did not alter canonical role/cursor.
    handlerOwnerAfterInspection = runtime;
}

TEST_CASE(encoded_handler_role_survives_inspection_leases_without_reinitialization)
{
    REQUIRE(!handlerOwnerAfterInspection.expired());
    const auto runtime = handlerOwnerAfterInspection.lock(); REQUIRE(runtime);
    const auto prior = runtime->encodedNewHandlerReport();
    const auto again = runtime->resumeEncodedNewHandlerPrefix(runtime);
    checkCompleted(again);
    CHECK(again.runtimeIdentity == prior.runtimeIdentity && again.encodedNullResult == prior.encodedNullResult);
    CHECK(again.observedCandidateRole == prior.observedCandidateRole);
    CHECK(again.encodePointerCalls == 1 && again.candidateRoleStores == 1);
}

int main() { return testkit::run_all(); }
