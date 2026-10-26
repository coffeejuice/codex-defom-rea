// ctest-labels: m2m-model feature-context crt-startup
// Fresh owned process: this explicit startup slice precedes every child frame.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <testkit/check.hpp>
#include "../../src/model/feature_crt_runtime.hpp"
#include <array>
#include <cstring>

using namespace m2m::detail;
static_assert(!FeatureCrtNullRoleTailReport::originalGlobalObserved && !FeatureCrtNullRoleTailReport::hostCrtRolesChanged);
static_assert(!FeatureCrtNullRoleTailReport::additionalEncodingEntered && !FeatureCrtNullRoleTailReport::handlerInvoked);
static_assert(!FeatureCrtNullRoleTailReport::flsEntered && !FeatureCrtNullRoleTailReport::lastErrorObserved);
static_assert(!FeatureCrtNullRoleTailReport::completeCrtStartup && !FeatureCrtNullRoleTailReport::completeForPublicClose);

namespace {
std::weak_ptr<FeatureCrtRuntime> ownerAfterInspection;
void checkCompleted(const FeatureCrtNullRoleTailReport& report, std::uintptr_t token)
{
    CHECK(report.origin == FeatureCrtOrigin::ActualWin32);
    CHECK(report.started && report.savedArgumentKnown && report.savedArgumentToken == token);
    CHECK(report.roleStores == 8 && report.leafReturns == 6 && report.noOpReturned && report.prefixReturned);
    CHECK(report.stop == FeatureCrtNullRoleTailStop::NonNullCallbackInitializerUnavailable);
    using E = FeatureCrtNullRoleTailEffect;
    // Independent source golden: distinct store VAs/roles, and returns carry no value claim.
    const std::array<FeatureCrtNullRoleTailEvent, 14> expected{{
        {0x141481ca8,E::CandidateRoleStored,0x1447006d8,token,true},
        {0x141481caf,E::LeafReturned,0,0,false},
        {0x141470438,E::CandidateRoleStored,0x1446ffb40,token,true},
        {0x14147043f,E::LeafReturned,0,0,false},
        {0x141481ca0,E::CandidateRoleStored,0x1447006d0,token,true},
        {0x141481ca7,E::LeafReturned,0,0,false},
        {0x141481ad4,E::CandidateRoleStored,0x1447006c0,token,true},
        {0x141481adb,E::LeafReturned,0,0,false},
        {0x141474954,E::CandidateRoleStored,0x1446ffb68,token,true},
        {0x14147495b,E::CandidateRoleStored,0x1446ffb70,token,true},
        {0x141474962,E::CandidateRoleStored,0x1446ffb78,token,true},
        {0x141474969,E::CandidateRoleStored,0x1446ffb80,token,true},
        {0x141474970,E::LeafReturned,0,0,false},
        {0x14146ff78,E::NoOpReturned,0,0,false}
    }};
    REQUIRE(report.effectCount == expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
        CHECK(report.effects[i].sourceSite == expected[i].sourceSite);
        CHECK(report.effects[i].effect == expected[i].effect);
        CHECK(report.effects[i].originalRoleSource == expected[i].originalRoleSource);
        CHECK(report.effects[i].observedValue == expected[i].observedValue);
        CHECK(report.effects[i].storedValueKnown == expected[i].storedValueKnown);
    }
    for (std::size_t i = 0; i < report.roleKnown.size(); ++i) {
        CHECK(report.roleKnown[i] && report.observedRoles[i] == token);
    }
    REQUIRE(report.nextSourceSite);
    CHECK(std::strcmp(report.nextSourceSite, "0x14146ea3d:0x14147e75c nonnull callback initializer unavailable") == 0);
}
}

TEST_CASE(encoded_null_tail_requires_explicit_first_prefix_and_same_owner_then_stores_once)
{
    const auto runtime = FeatureCrtRuntime::prepareOwnedTestProcess(); REQUIRE(runtime);
    const auto initial = runtime->encodedNullRoleTailReport();
    CHECK(initial.runtimeIdentity == runtime.get() && initial.processId == GetCurrentProcessId());
    CHECK(initial.stop == FeatureCrtNullRoleTailStop::NotStarted && !initial.started);
    CHECK(!initial.savedArgumentKnown && initial.roleStores == 0 && initial.effectCount == 0);
    for (bool known : initial.roleKnown) CHECK(!known);
    const std::shared_ptr<FeatureCrtRuntime> absent;
    const std::shared_ptr<FeatureCrtRuntime> foreign(runtime.get(), [](FeatureCrtRuntime*) {});
    CHECK(runtime->resumeEncodedNullRoleTail(absent).stop == FeatureCrtNullRoleTailStop::OwnerMismatch);
    CHECK(runtime->resumeEncodedNullRoleTail(foreign).stop == FeatureCrtNullRoleTailStop::OwnerMismatch);
    CHECK(runtime->encodedNullRoleTailReport().stop == FeatureCrtNullRoleTailStop::NotStarted);
    CHECK(runtime->resumeEncodedNullRoleTail(runtime).stop == FeatureCrtNullRoleTailStop::ProducerUnavailable);
    CHECK(runtime->report().heapCreateCalls == 0);
    const auto producer = runtime->attemptProducer();
    REQUIRE(producer.heapNonNull == true && producer.producerReturn == 1u);
    const auto premature = runtime->resumeEncodedNullRoleTail(runtime);
    CHECK(premature.stop == FeatureCrtNullRoleTailStop::FirstPrefixUnavailable);
    CHECK(!premature.started && premature.roleStores == 0 && premature.effectCount == 0);
    CHECK(runtime->encodedNewHandlerReport().encodePointerCalls == 0);
    const auto first = runtime->resumeEncodedNewHandlerPrefix(runtime);
    REQUIRE(first.encodePointerReturned && first.firstSetterReturned && first.candidateRoleKnown);
    CHECK(first.encodePointerCalls == 1 && first.candidateRoleStores == 1 && first.effectCount == 4);
    CHECK(runtime->encodedNullRoleTailReport().roleStores == 0); // Old API never auto-drives this tail.
    CHECK(runtime->resumeEncodedNullRoleTail(foreign).stop == FeatureCrtNullRoleTailStop::OwnerMismatch);
    const auto result = runtime->resumeEncodedNullRoleTail(runtime);
    checkCompleted(result, first.encodedNullResult); // Includes numeric zero; no nonzero-token assumption.
    CHECK(result.runtimeIdentity == runtime.get() && result.processId == producer.processId);
    CHECK(result.moduleIdentity == producer.moduleIdentity);
    checkCompleted(runtime->resumeEncodedNullRoleTail(runtime), first.encodedNullResult);
    checkCompleted(runtime->encodedNullRoleTailReport(), first.encodedNullResult);
    const auto wrongAfter = runtime->resumeEncodedNullRoleTail(foreign);
    CHECK(wrongAfter.stop == FeatureCrtNullRoleTailStop::OwnerMismatch && wrongAfter.roleStores == 8);
    checkCompleted(runtime->encodedNullRoleTailReport(), first.encodedNullResult);
    const auto afterFirst = runtime->resumeEncodedNewHandlerPrefix(runtime);
    CHECK(afterFirst.effectCount == first.effectCount && afterFirst.encodePointerCalls == 1);
    CHECK(afterFirst.candidateRoleStores == 1 && afterFirst.observedCandidateRole == first.encodedNullResult);
    CHECK(afterFirst.stop == first.stop && afterFirst.nextSourceSite == first.nextSourceSite);
    const auto after = runtime->report();
    CHECK(after.effects == producer.effects && after.heapCreateCalls == producer.heapCreateCalls);
    CHECK(after.compatibilityCalls == producer.compatibilityCalls && after.producerReturn == producer.producerReturn);
    ownerAfterInspection = runtime;
}

TEST_CASE(encoded_null_tail_retains_canonical_cells_after_all_inspection_leases)
{
    REQUIRE(!ownerAfterInspection.expired());
    const auto runtime = ownerAfterInspection.lock(); REQUIRE(runtime);
    const auto prior = runtime->encodedNullRoleTailReport();
    const auto again = runtime->resumeEncodedNullRoleTail(runtime);
    checkCompleted(again, runtime->encodedNewHandlerReport().encodedNullResult);
    CHECK(again.runtimeIdentity == prior.runtimeIdentity && again.moduleIdentity == prior.moduleIdentity);
    CHECK(again.processId == prior.processId && again.savedArgumentToken == prior.savedArgumentToken);
    CHECK(again.roleStores == prior.roleStores && again.effectCount == prior.effectCount);
}

int main() { return testkit::run_all(); }
