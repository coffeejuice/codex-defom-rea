#include <defrt/module_heap_role.hpp>

#include <mutex>
#include <utility>
#include <cstring>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace def::rt {
namespace {
[[maybe_unused]] void moduleHeapAnchor() noexcept {}
void recordSlot(Temp0Report& r, std::uintptr_t heap) noexcept
{
    r.started = true; r.heapToken = heap; ++r.heapSlotReads;
    r.effects[r.effectCount++] = Temp0Effect::HeapSlotRead;
    r.effects[r.effectCount++] = Temp0Effect::HeapNullTested;
    r.stage = Temp0Stage::HeapSlotObserved;
}
void recordEntry(Temp0Report& r) noexcept
{
    ++r.heapAllocCalls; r.stage = Temp0Stage::HeapAllocEntered;
    r.effects[r.effectCount++] = Temp0Effect::HeapAllocEntered;
}
void recordReturn(Temp0Report& r, bool nonNull, std::uintptr_t token) noexcept
{
    r.rawReturnKnown = true; r.rawReturnNonNull = nonNull; r.returnToken = token;
    r.effects[r.effectCount++] = Temp0Effect::HeapAllocReturned;
    r.stage = nonNull ? Temp0Stage::RawNonNullPending : Temp0Stage::RawNullPending;
    r.stop = Temp0Stop::RawKernelReturn;
}
void appendScalar(UnpublishedScalarReport& r, UnpublishedScalarEffect effect) noexcept
{ r.effects[r.effectCount++] = effect; }
void completeTempSuccessPrefix(UnpublishedScalarReport& r) noexcept
{
    // Nonnull 14748f7->492b->4942, then 146eadb->eae5. No failure-role reads.
    appendScalar(r, UnpublishedScalarEffect::TempMallocTail);
    appendScalar(r, UnpublishedScalarEffect::TempNewTail);
    appendScalar(r, UnpublishedScalarEffect::TempSaved); r.tempNormalTailReturned = true;
    // Exact old count0: execute the zero-count branch, never a host null-span copy.
    r.tempZeroCopyReturned = true; appendScalar(r, UnpublishedScalarEffect::TempZeroCopy);
}
void recordValueEntry(UnpublishedScalarReport& r, std::uintptr_t heap) noexcept
{
    r.heapToken = heap; ++r.heapSlotReads;
    appendScalar(r, UnpublishedScalarEffect::ValueHeapSlotRead);
    appendScalar(r, UnpublishedScalarEffect::ValueHeapNullTest);
    ++r.heapAllocCalls; appendScalar(r, UnpublishedScalarEffect::ValueHeapAllocEntered);
}
void recordValueReturn(UnpublishedScalarReport& r, bool nonNull, std::uintptr_t token) noexcept
{
    r.valueRawReturnKnown = true; r.valueRawReturnNonNull = nonNull; r.bufferToken = token;
    appendScalar(r, UnpublishedScalarEffect::ValueHeapAllocReturned);
    if (!nonNull) r.stage = UnpublishedScalarStage::ValueRawNullPending;
}
void completeUnpublishedValuePrefix(UnpublishedScalarReport& r, void* actualBuffer) noexcept
{
    static_assert(sizeof(double) == 8 && sizeof(std::uint64_t) == 8);
    appendScalar(r, UnpublishedScalarEffect::ValueMallocTail);
    appendScalar(r, UnpublishedScalarEffect::ValueNewTail); r.valueNormalTailReturned = true;
    // Both old copy ranges are [null,null). Equality returns before element access.
    r.leftEmptyCopyReturned = true; appendScalar(r, UnpublishedScalarEffect::LeftEmptyCopy);
    const double zero = 0.0;
    if (actualBuffer) {
        // Valid owned B8 only: no null-span memcpy or uninitialized-byte read.
        std::memcpy(actualBuffer, &zero, sizeof zero);
        std::memcpy(&r.initializedBits, actualBuffer, sizeof r.initializedBits);
    }
    r.fillCommitted = true; appendScalar(r, UnpublishedScalarEffect::Fill);
    r.rightEmptyCopyReturned = true; appendScalar(r, UnpublishedScalarEffect::RightEmptyCopy);
    // Reconstructed old-first==null prefix returns before heap read/HeapFree.
    r.candidateOldNullFreeReturned = true; appendScalar(r, UnpublishedScalarEffect::OldNullFreePrefix);
    r.stage = UnpublishedScalarStage::BeforeBodyPublication;
    appendScalar(r, UnpublishedScalarEffect::BeforeBodyPublication);
}
}
struct ModuleHeapRole::Storage {
    mutable std::mutex mutex;
    std::weak_ptr<ModuleHeapRole> canonical;
    std::shared_ptr<TemporaryZeroCall> call;
    HeapRoleProducerReport producer;
    Temp0Report temporary;
    UnpublishedScalarReport scalar;
    void* candidateHeap = nullptr;
    void* retainedT = nullptr; // Deliberate bounded retention, never attached to a CowVector.
    void* retainedB = nullptr; // Separate pending insertion-local buffer, never published.
    HeapPrefixMode mode = HeapPrefixMode::RawTemporaryOnly;
    bool reserved = false;
    Temp0Report attemptTemporaryLocked() noexcept;
};

ModuleHeapRole::ModuleHeapRole() : storage_(std::make_unique<Storage>())
{
    auto& r = storage_->producer;
    r.roleToken = reinterpret_cast<std::uintptr_t>(this);
#if defined(_WIN32)
    r.origin = HeapRoleOrigin::ActualWin32; r.processId = GetCurrentProcessId();
    HMODULE module = nullptr;
    if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN,
                         reinterpret_cast<LPCWSTR>(&moduleHeapAnchor), &module)) {
        r.modulePinned = true; r.moduleToken = reinterpret_cast<std::uintptr_t>(module);
    } else r.stage = HeapRoleProducerStage::ModuleUnavailable;
#else
    r.stage = HeapRoleProducerStage::ModuleUnavailable;
#endif
    storage_->temporary.origin = r.origin;
    storage_->temporary.roleToken = r.roleToken;
    storage_->scalar.origin = r.origin; storage_->scalar.roleToken = r.roleToken;
}
ModuleHeapRole::~ModuleHeapRole() = default; // Metadata only, never source cleanup.

std::shared_ptr<ModuleHeapRole> ModuleHeapRole::prepareCurrentModule()
{
    static auto* retained = [] {
        auto owner = std::shared_ptr<ModuleHeapRole>(new ModuleHeapRole);
        owner->storage_->canonical = owner;
        owner->storage_->call = std::shared_ptr<TemporaryZeroCall>(new TemporaryZeroCall(owner));
        return new std::shared_ptr<ModuleHeapRole>(std::move(owner));
    }();
    return *retained;
}
bool ModuleHeapRole::matchesRetainedLease(const std::shared_ptr<ModuleHeapRole>& lease) const noexcept
{
    const auto canonical = storage_->canonical.lock();
    return lease && lease.get() == this && canonical && canonical.get() == this &&
        !canonical.owner_before(lease) && !lease.owner_before(canonical);
}
HeapRoleProducerReport ModuleHeapRole::attemptProducer() noexcept
{
    std::lock_guard lock(storage_->mutex);
    auto& r = storage_->producer;
    if (!r.modulePinned || r.stage != HeapRoleProducerStage::NotAttempted) return r;
#if defined(_WIN32)
    ++r.heapCreateCalls;
    const HANDLE heap = HeapCreate(0, 4096, 0); //141484812, candidate role setup.
    storage_->candidateHeap = heap; r.heapToken = reinterpret_cast<std::uintptr_t>(heap);
    r.slotPublished = true; ++r.slotStores; //141484818 also publishes null.
    if (!heap) { r.stage = HeapRoleProducerStage::CreateReturnedNull; return r; }
    static_assert(static_cast<int>(HeapCompatibilityInformation) == 0 && sizeof(DWORD) == 4);
    DWORD compatibility = 2; ++r.compatibilityCalls;
    r.compatibilityResult = HeapSetInformation(heap, HeapCompatibilityInformation, &compatibility, 4);
    r.producerResult = 1; // Source ignores the compatibility BOOL.
    r.stage = HeapRoleProducerStage::CompatibilityReturned;
#endif
    return r;
}
HeapRoleProducerReport ModuleHeapRole::producerReport() const noexcept
{
    std::lock_guard lock(storage_->mutex); return storage_->producer;
}
Temp0PrepareResult ModuleHeapRole::prepareTemporaryZero(const std::shared_ptr<ModuleHeapRole>& lease, HeapPrefixMode mode) noexcept
{
    if (!matchesRetainedLease(lease)) return {Temp0PrepareStatus::LeaseMismatch, {}};
    std::lock_guard lock(storage_->mutex);
    if (storage_->reserved) return {Temp0PrepareStatus::CapacityUnavailable, {}};
    if (storage_->producer.stage != HeapRoleProducerStage::CompatibilityReturned || !storage_->candidateHeap)
        return {Temp0PrepareStatus::ProducerUnavailable, {}};
    storage_->reserved = true;
    storage_->mode = mode;
    if (mode == HeapPrefixMode::UnpublishedScalar) storage_->scalar.stage = UnpublishedScalarStage::Prepared;
    storage_->temporary.stage = Temp0Stage::Prepared;
    storage_->temporary.stop = Temp0Stop::BeforeHeapAlloc;
    storage_->temporary.occurrence = 1;
    return {Temp0PrepareStatus::Prepared, storage_->call};
}
Temp0Report TemporaryZeroCall::attempt(const std::shared_ptr<ModuleHeapRole>& lease) noexcept
{
    const auto owner = owner_.lock();
    if (!owner) return {};
    std::lock_guard lock(owner->storage_->mutex);
    auto& s = *owner->storage_; auto& r = s.temporary;
    if (!owner->matchesRetainedLease(lease)) { auto refused = r; refused.stop = Temp0Stop::LeaseMismatch; return refused; }
    if (s.mode != HeapPrefixMode::RawTemporaryOnly) { auto refused = r; refused.stop = Temp0Stop::ModeMismatch; return refused; }
    return s.attemptTemporaryLocked();
}
Temp0Report ModuleHeapRole::Storage::attemptTemporaryLocked() noexcept
{
    auto& s = *this; auto& r = temporary;
    // First latch check precedes every historical pointer/heap use on a retry.
    if (r.started || !s.reserved) return r;
    if (s.producer.stage != HeapRoleProducerStage::CompatibilityReturned) {
        r.stop = Temp0Stop::ProducerUnavailable; return r;
    }
    const auto heap = s.candidateHeap; // New occurrence reads the current candidate role.
    recordSlot(r, reinterpret_cast<std::uintptr_t>(heap));
    if (!heap || reinterpret_cast<std::uintptr_t>(heap) != s.producer.heapToken) {
        r.stop = Temp0Stop::NullHeapFatalUnavailable; return r;
    }
#if defined(_WIN32)
    r.threadId = GetCurrentThreadId(); recordEntry(r);
    void* const block = HeapAlloc(heap, 0, 1); //1414748f1; request0, normalized1, no zero-fill.
    s.retainedT = block;
    recordReturn(r, block != nullptr, reinterpret_cast<std::uintptr_t>(block));
#endif
    // Stop at raw return: no malloc/new branch, memcpy, request8, or free.
    return r;
}
UnpublishedScalarReport TemporaryZeroCall::attemptUnpublishedScalar(const std::shared_ptr<ModuleHeapRole>& lease,
    CurrentScalarProof proof, const void* context) noexcept
{
    const auto owner = owner_.lock();
    if (!owner) return {};
    std::lock_guard lock(owner->storage_->mutex);
    auto& s = *owner->storage_; auto& r = s.scalar;
    if (!owner->matchesRetainedLease(lease) || s.mode != HeapPrefixMode::UnpublishedScalar) return r;
    // No late promotion of a CP155/raw T or a prior interrupted extension.
    if (r.started || s.temporary.started || !s.reserved) return r;
    r.started = true;
    const auto temporary = s.attemptTemporaryLocked();
    if (temporary.stage != Temp0Stage::RawNonNullPending || !s.retainedT) {
        r.stage = temporary.stage == Temp0Stage::RawNullPending ? UnpublishedScalarStage::TempRawNullPending :
            UnpublishedScalarStage::TempUnavailable;
        return r;
    }
    r.temporaryToken = temporary.returnToken;
    completeTempSuccessPrefix(r);
    if (!proof || !proof(context)) { r.stage = UnpublishedScalarStage::CurrentOwnerUnavailable; return r; }
    r.currentOwnerProved = true; appendScalar(r, UnpublishedScalarEffect::CurrentOwnerProved);
    const auto heap = s.candidateHeap; // Second occurrence rereads current role, not saved T heap.
    if (!heap || reinterpret_cast<std::uintptr_t>(heap) != s.producer.heapToken ||
        reinterpret_cast<std::uintptr_t>(heap) != temporary.heapToken) {
        ++r.heapSlotReads; r.heapToken = reinterpret_cast<std::uintptr_t>(heap);
        appendScalar(r, UnpublishedScalarEffect::ValueHeapSlotRead);
        appendScalar(r, UnpublishedScalarEffect::ValueHeapNullTest);
        r.stage = UnpublishedScalarStage::ValueHeapUnavailable; return r;
    }
#if defined(_WIN32)
    recordValueEntry(r, reinterpret_cast<std::uintptr_t>(heap));
    void* const buffer = HeapAlloc(heap, 0, 8); //832a1d via same malloc/new role; no publication.
    s.retainedB = buffer;
    recordValueReturn(r, buffer != nullptr, reinterpret_cast<std::uintptr_t>(buffer));
    if (!buffer) return r; // Before its new-mode/handler/errno branch; T stays retained.
    completeUnpublishedValuePrefix(r, buffer);
#endif
    return r;
}
Temp0Report TemporaryZeroCall::report() const noexcept
{
    const auto owner = owner_.lock();
    if (!owner) return {};
    std::lock_guard lock(owner->storage_->mutex); return owner->storage_->temporary;
}
UnpublishedScalarReport TemporaryZeroCall::unpublishedScalarReport() const noexcept
{
    const auto owner = owner_.lock();
    if (!owner) return {};
    std::lock_guard lock(owner->storage_->mutex); return owner->storage_->scalar;
}
HeapRoleProducerReport declaredHeapRoleProducer(bool nonNull, int compatibilityResult) noexcept
{
    HeapRoleProducerReport r; r.origin = HeapRoleOrigin::DeclaredModel;
    r.heapCreateCalls = 1; r.slotStores = 1; r.slotPublished = true;
    if (!nonNull) r.stage = HeapRoleProducerStage::CreateReturnedNull;
    else {
        r.compatibilityCalls = 1; r.compatibilityResult = compatibilityResult;
        r.producerResult = 1; r.stage = HeapRoleProducerStage::CompatibilityReturned;
    }
    return r;
}
Temp0Report declaredTemporaryZeroOutcome(bool heapNonNull, bool returnNonNull) noexcept
{
    Temp0Report r; r.origin = HeapRoleOrigin::DeclaredModel; r.occurrence = 1;
    recordSlot(r, 0); // No actual handle or pointer, even on declared nonnull branch.
    if (!heapNonNull) r.stop = Temp0Stop::NullHeapFatalUnavailable;
    else { recordEntry(r); recordReturn(r, returnNonNull, 0); }
    return r;
}
UnpublishedScalarReport declaredUnpublishedScalarOutcome(bool tempNonNull, bool ownerMatches, bool valueNonNull) noexcept
{
    UnpublishedScalarReport r; r.origin = HeapRoleOrigin::DeclaredModel; r.started = true;
    if (!tempNonNull) { r.stage = UnpublishedScalarStage::TempRawNullPending; return r; }
    completeTempSuccessPrefix(r);
    if (!ownerMatches) { r.stage = UnpublishedScalarStage::CurrentOwnerUnavailable; return r; }
    r.currentOwnerProved = true; appendScalar(r, UnpublishedScalarEffect::CurrentOwnerProved);
    recordValueEntry(r, 0); recordValueReturn(r, valueNonNull, 0); // No actual handle/B authority.
    if (valueNonNull) completeUnpublishedValuePrefix(r, nullptr); // Declared stores only.
    return r;
}
} // namespace def::rt
