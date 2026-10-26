// ctest-labels: m2m-model geometry-temp-prefix
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <testkit/check.hpp>
#include <defrt/module_heap_role.hpp>
#include <memory>
#include <type_traits>

using namespace def::rt;
static_assert(!std::is_default_constructible_v<ModuleHeapRole>);
static_assert(!std::is_default_constructible_v<TemporaryZeroCall>);
static_assert(!Temp0Report::mallocReturned && !Temp0Report::operatorNewReturned && !Temp0Report::zeroCopyEntered);
static_assert(!Temp0Report::valueAllocationEntered && !Temp0Report::temporaryFreeEntered && !Temp0Report::heapDestroyEntered);
static_assert(!UnpublishedScalarReport::bodyPublished && !UnpublishedScalarReport::slotPublished);
static_assert(!UnpublishedScalarReport::heapFreeEntered && !UnpublishedScalarReport::nativeFreeObserved);

TEST_CASE(geometry_temp_declared_failure_boundaries_have_no_real_authority)
{
    const auto failed = declaredHeapRoleProducer(false, 0);
    CHECK(failed.origin == HeapRoleOrigin::DeclaredModel);
    CHECK(failed.stage == HeapRoleProducerStage::CreateReturnedNull);
    CHECK(failed.slotPublished && failed.slotStores == 1u && failed.compatibilityCalls == 0u);
    CHECK(failed.heapToken == 0u && failed.moduleToken == 0u);
    const auto compatibilityFalse = declaredHeapRoleProducer(true, 0);
    CHECK(compatibilityFalse.producerResult == 1);
    CHECK(compatibilityFalse.stage == HeapRoleProducerStage::CompatibilityReturned);
    CHECK(compatibilityFalse.compatibilityCalls == 1u && compatibilityFalse.compatibilityResult == 0);
    const auto nullHeap = declaredTemporaryZeroOutcome(false, false);
    CHECK(nullHeap.stop == Temp0Stop::NullHeapFatalUnavailable);
    CHECK(nullHeap.heapSlotReads == 1u && nullHeap.heapAllocCalls == 0u && !nullHeap.rawReturnKnown);
    for (const bool returned : {false, true}) {
        const auto modeled = declaredTemporaryZeroOutcome(true, returned);
        CHECK(modeled.origin == HeapRoleOrigin::DeclaredModel);
        CHECK(modeled.stage == (returned ? Temp0Stage::RawNonNullPending : Temp0Stage::RawNullPending));
        CHECK(modeled.started && modeled.rawReturnKnown && modeled.rawReturnNonNull == returned);
        CHECK(modeled.heapToken == 0u && modeled.returnToken == 0u && modeled.roleToken == 0u);
        CHECK(modeled.requestedBytes == 0u && modeled.kernelBytes == 1u && modeled.flags == 0u);
        CHECK(modeled.effectCount == 4u && modeled.heapAllocCalls == 1u);
        CHECK(!modeled.handlerEntered && !modeled.errnoEntered && !modeled.wholeLeafReturned);
    }
}

TEST_CASE(geometry_temp_actual_module_role_retains_one_raw_return_without_cleanup)
{
    auto role = ModuleHeapRole::prepareCurrentModule(); REQUIRE(role);
    CHECK(role->matchesRetainedLease(role));
    {
        const std::shared_ptr<ModuleHeapRole> absent;
        const std::shared_ptr<ModuleHeapRole> alias(role, nullptr);
        const std::shared_ptr<ModuleHeapRole> foreign(role.get(), [](ModuleHeapRole*) {});
        CHECK(!role->matchesRetainedLease(absent)); CHECK(!role->matchesRetainedLease(alias));
        CHECK(!role->matchesRetainedLease(foreign));
        CHECK(role->prepareTemporaryZero(foreign).status == Temp0PrepareStatus::LeaseMismatch);
    }
    CHECK(role->prepareTemporaryZero(role).status == Temp0PrepareStatus::ProducerUnavailable);
    const auto producer = role->attemptProducer();
    REQUIRE(producer.origin == HeapRoleOrigin::ActualWin32);
    REQUIRE(producer.stage == HeapRoleProducerStage::CompatibilityReturned);
    CHECK(producer.modulePinned && producer.moduleToken != 0u && producer.processId != 0u);
    CHECK(producer.heapToken != 0u && producer.slotPublished && producer.slotStores == 1u);
    CHECK(producer.heapCreateCalls == 1u && producer.compatibilityCalls == 1u && producer.producerResult == 1);
    CHECK(producer.heapFlags == 0u && producer.initialBytes == 4096u && producer.maximumBytes == 0u);
    CHECK(producer.compatibilityClass == 0u && producer.compatibilityValue == 2u && producer.compatibilityBytes == 4u);
    CHECK(role->attemptProducer().heapCreateCalls == 1u);
    auto prepared = role->prepareTemporaryZero(role);
    REQUIRE(prepared.status == Temp0PrepareStatus::Prepared); REQUIRE(prepared.call);
    CHECK(role->prepareTemporaryZero(role).status == Temp0PrepareStatus::CapacityUnavailable);
    {
        const std::shared_ptr<ModuleHeapRole> foreign(role.get(), [](ModuleHeapRole*) {});
        CHECK(prepared.call->attempt(foreign).stop == Temp0Stop::LeaseMismatch);
    }
    CHECK(prepared.call->report().heapAllocCalls == 0u);
    const auto raw = prepared.call->attempt(role);
    REQUIRE(raw.stage == Temp0Stage::RawNonNullPending); REQUIRE(raw.returnToken != 0u);
    CHECK(raw.origin == HeapRoleOrigin::ActualWin32 && raw.started && raw.rawReturnKnown && raw.rawReturnNonNull);
    CHECK(raw.requestedBytes == 0u && raw.kernelBytes == 1u && raw.flags == 0u && raw.threadId != 0u);
    CHECK(raw.heapToken == producer.heapToken && raw.occurrence == 1u);
    CHECK(raw.heapSlotReads == 1u && raw.heapAllocCalls == 1u && raw.effectCount == 4u);
    CHECK(raw.effects[0] == Temp0Effect::HeapSlotRead && raw.effects[1] == Temp0Effect::HeapNullTested);
    CHECK(raw.effects[2] == Temp0Effect::HeapAllocEntered && raw.effects[3] == Temp0Effect::HeapAllocReturned);
    CHECK(prepared.call->attempt(role).returnToken == raw.returnToken);
    const auto alwaysTrue = [](const void*) noexcept { return true; };
    CHECK(prepared.call->attemptUnpublishedScalar(role, alwaysTrue, nullptr).stage == UnpublishedScalarStage::NotEnabled);
    CHECK(prepared.call->unpublishedScalarReport().heapAllocCalls == 0u); // No promotion of old raw T.
    std::weak_ptr<ModuleHeapRole> weakRole = role;
    std::weak_ptr<TemporaryZeroCall> weakCall = prepared.call;
    prepared.call.reset(); role.reset();
    auto retainedRole = weakRole.lock(); auto retainedCall = weakCall.lock();
    REQUIRE(retainedRole); REQUIRE(retainedCall);
    CHECK(retainedRole == ModuleHeapRole::prepareCurrentModule());
    CHECK(retainedCall->report().returnToken == raw.returnToken);
    CHECK(retainedCall->attempt(retainedRole).heapAllocCalls == 1u);
    CHECK(!raw.mallocReturned && !raw.operatorNewReturned && !raw.zeroCopyEntered);
    CHECK(!raw.valueAllocationEntered && !raw.temporaryFreeEntered && !raw.heapDestroyEntered && !raw.wholeLeafReturned);
}

TEST_CASE(geometry_unpublished_scalar_declared_failures_preserve_partial_effects)
{
    const auto firstNull = declaredUnpublishedScalarOutcome(false, true, true);
    CHECK(firstNull.origin == HeapRoleOrigin::DeclaredModel);
    CHECK(firstNull.stage == UnpublishedScalarStage::TempRawNullPending);
    CHECK(!firstNull.tempNormalTailReturned && firstNull.heapAllocCalls == 0u && !firstNull.fillCommitted);
    const auto refused = declaredUnpublishedScalarOutcome(true, false, true);
    CHECK(refused.stage == UnpublishedScalarStage::CurrentOwnerUnavailable);
    CHECK(refused.tempNormalTailReturned && refused.tempZeroCopyReturned && !refused.currentOwnerProved);
    CHECK(refused.heapAllocCalls == 0u && !refused.valueRawReturnKnown);
    for (const bool valueNonNull : {false, true}) {
        const auto r = declaredUnpublishedScalarOutcome(true, true, valueNonNull);
        CHECK(r.origin == HeapRoleOrigin::DeclaredModel && r.currentOwnerProved);
        CHECK(r.temporaryToken == 0u && r.bufferToken == 0u && r.heapToken == 0u && r.roleToken == 0u);
        CHECK(r.requestedBytes == 8u && r.kernelBytes == 8u && r.flags == 0u);
        CHECK(r.heapSlotReads == 1u && r.heapAllocCalls == 1u && r.valueRawReturnKnown);
        CHECK(r.valueRawReturnNonNull == valueNonNull);
        CHECK(r.stage == (valueNonNull ? UnpublishedScalarStage::BeforeBodyPublication : UnpublishedScalarStage::ValueRawNullPending));
        CHECK(r.fillCommitted == valueNonNull && r.valueNormalTailReturned == valueNonNull);
        CHECK(r.candidateOldNullFreeReturned == valueNonNull && r.rightEmptyCopyReturned == valueNonNull);
        CHECK(r.initializedBits == 0u && r.effectCount == (valueNonNull ? 16u : 9u));
        CHECK(!r.heapFreeEntered && !r.nativeFreeObserved && !r.temporaryReleased && !r.bufferReleased);
        CHECK(!r.bodyPublished && !r.slotPublished && !r.vectorReturned && !r.wholeLeafReturned && !r.handlerEntered && !r.errnoEntered);
    }
}

namespace {
void runCurrentOwnerRefusalChild()
{
    auto role = ModuleHeapRole::prepareCurrentModule(); REQUIRE(role);
    REQUIRE(role->attemptProducer().stage == HeapRoleProducerStage::CompatibilityReturned);
    auto call = role->prepareTemporaryZero(role, HeapPrefixMode::UnpublishedScalar);
    REQUIRE(call.status == Temp0PrepareStatus::Prepared); REQUIRE(call.call);
    CHECK(call.call->attempt(role).stop == Temp0Stop::ModeMismatch);
    CHECK(call.call->report().heapAllocCalls == 0u);
    int proofs = 0;
    const auto refuse = [](const void* context) noexcept {
        ++*static_cast<int*>(const_cast<void*>(context)); return false;
    };
    const auto r = call.call->attemptUnpublishedScalar(role, refuse, &proofs);
    REQUIRE(call.call->report().stage == Temp0Stage::RawNonNullPending);
    CHECK(r.stage == UnpublishedScalarStage::CurrentOwnerUnavailable);
    CHECK(proofs == 1 && r.tempNormalTailReturned && r.tempZeroCopyReturned && !r.currentOwnerProved);
    CHECK(r.temporaryToken != 0u && r.bufferToken == 0u && r.heapSlotReads == 0u && r.heapAllocCalls == 0u);
    CHECK(!r.fillCommitted && !r.candidateOldNullFreeReturned && !r.heapFreeEntered);
    const auto acceptLater = [](const void* context) noexcept {
        ++*static_cast<int*>(const_cast<void*>(context)); return true;
    };
    CHECK(call.call->attemptUnpublishedScalar(role, acceptLater, &proofs).stage == UnpublishedScalarStage::CurrentOwnerUnavailable);
    CHECK(proofs == 1 && call.call->unpublishedScalarReport().heapAllocCalls == 0u);
    CHECK(call.call->report().heapAllocCalls == 1u);
}
}

TEST_CASE(geometry_unpublished_scalar_current_owner_recheck_is_one_shot_before_B)
{
    std::array<wchar_t, 32768> path{};
    const auto length = GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
    REQUIRE(length != 0 && length < path.size());
    std::wstring command = L"\"" + std::wstring(path.data(), length) + L"\" --scalar-owner-refused";
    STARTUPINFOW startup{}; startup.cb = sizeof(startup); PROCESS_INFORMATION process{};
    REQUIRE(CreateProcessW(path.data(), command.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW,
        nullptr, nullptr, &startup, &process) != FALSE);
    CloseHandle(process.hThread);
    const auto waited = WaitForSingleObject(process.hProcess, 30000); DWORD status = 1;
    if (waited == WAIT_OBJECT_0) CHECK(GetExitCodeProcess(process.hProcess, &status) != FALSE);
    else {
        (void)TerminateProcess(process.hProcess, 0xe1570001u);
        (void)WaitForSingleObject(process.hProcess, 2000);
        testkit::fail(__FILE__, __LINE__, "unpublished scalar child did not finish");
    }
    CloseHandle(process.hProcess); CHECK(status == 0);
}

int main(int argc, char** argv)
{
    if (argc == 2 && std::strcmp(argv[1], "--scalar-owner-refused") == 0) {
        try { runCurrentOwnerRefusalChild(); }
        catch (const testkit::Abort&) {}
        catch (const std::exception& error) { testkit::fail(__FILE__, __LINE__, error.what()); }
        return testkit::failures() == 0 ? 0 : 1;
    }
    if (argc != 1) return 2;
    return testkit::run_all();
}
