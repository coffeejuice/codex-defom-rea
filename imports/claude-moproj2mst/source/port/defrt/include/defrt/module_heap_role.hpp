#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

namespace def::rt {

// Candidate reconstruction of the EXE CRT heap role, not an original-global
// observation. Pending insertion-local B never enters a CowVector. No free/handler interface.
enum class HeapRoleOrigin { ActualWin32, DeclaredModel, Unavailable };
enum class HeapPrefixMode { RawTemporaryOnly, UnpublishedScalar };
enum class HeapRoleProducerStage { NotAttempted, ModuleUnavailable, CreateReturnedNull, CompatibilityReturned };
struct HeapRoleProducerReport {
    HeapRoleOrigin origin = HeapRoleOrigin::Unavailable;
    HeapRoleProducerStage stage = HeapRoleProducerStage::NotAttempted;
    std::uintptr_t roleToken = 0, moduleToken = 0, heapToken = 0;
    std::uint32_t processId = 0;
    bool modulePinned = false, slotPublished = false;
    std::size_t heapCreateCalls = 0, slotStores = 0, compatibilityCalls = 0;
    std::size_t initialBytes = 4096, maximumBytes = 0;
    std::uint32_t heapFlags = 0, compatibilityClass = 0, compatibilityValue = 2, compatibilityBytes = 4;
    int compatibilityResult = 0, producerResult = 0;
    static constexpr std::uintptr_t sourceProducer = 0x141484800;
    static constexpr std::uintptr_t sourceSlotStore = 0x141484818;
};
enum class Temp0Stage { Unprepared, Prepared, HeapSlotObserved, HeapAllocEntered, RawNullPending, RawNonNullPending };
enum class Temp0Stop { NotPrepared, BeforeHeapAlloc, ProducerUnavailable, LeaseMismatch, CapacityUnavailable, NullHeapFatalUnavailable, RawKernelReturn, ModeMismatch };
enum class Temp0Effect { HeapSlotRead, HeapNullTested, HeapAllocEntered, HeapAllocReturned };
struct Temp0Report {
    HeapRoleOrigin origin = HeapRoleOrigin::Unavailable;
    Temp0Stage stage = Temp0Stage::Unprepared;
    Temp0Stop stop = Temp0Stop::NotPrepared;
    std::uintptr_t roleToken = 0, heapToken = 0, returnToken = 0;
    std::uint64_t occurrence = 0;
    std::uint32_t threadId = 0, flags = 0;
    std::size_t requestedBytes = 0, kernelBytes = 1, heapSlotReads = 0, heapAllocCalls = 0;
    bool started = false, rawReturnKnown = false, rawReturnNonNull = false;
    std::array<Temp0Effect, 4> effects{};
    std::size_t effectCount = 0;
    static constexpr std::uintptr_t sourceCaller = 0x1409e47e4;
    static constexpr std::uintptr_t sourceHeapAlloc = 0x1414748f1;
    static constexpr std::uintptr_t nextSourceInstruction = 0x1414748f7;
    static constexpr bool originalGlobalObserved = false;
    static constexpr bool mallocReturned = false, operatorNewReturned = false, zeroCopyEntered = false;
    static constexpr bool valueAllocationEntered = false, temporaryFreeEntered = false, heapDestroyEntered = false;
    static constexpr bool handlerEntered = false, errnoEntered = false, wholeLeafReturned = false;
};
enum class UnpublishedScalarStage { NotEnabled, Prepared, TempUnavailable, TempRawNullPending, CurrentOwnerUnavailable,
    ValueHeapUnavailable, ValueRawNullPending, BeforeBodyPublication };
enum class UnpublishedScalarEffect { TempMallocTail, TempNewTail, TempSaved, TempZeroCopy, CurrentOwnerProved,
    ValueHeapSlotRead, ValueHeapNullTest, ValueHeapAllocEntered, ValueHeapAllocReturned, ValueMallocTail,
    ValueNewTail, LeftEmptyCopy, Fill, RightEmptyCopy, OldNullFreePrefix, BeforeBodyPublication };
struct UnpublishedScalarReport {
    HeapRoleOrigin origin = HeapRoleOrigin::Unavailable;
    UnpublishedScalarStage stage = UnpublishedScalarStage::NotEnabled;
    std::uintptr_t roleToken = 0, temporaryToken = 0, heapToken = 0, bufferToken = 0;
    std::uint32_t flags = 0;
    std::size_t requestedBytes = 8, kernelBytes = 8, heapSlotReads = 0, heapAllocCalls = 0;
    bool started = false, tempNormalTailReturned = false, tempZeroCopyReturned = false, currentOwnerProved = false;
    bool valueRawReturnKnown = false, valueRawReturnNonNull = false, valueNormalTailReturned = false;
    bool leftEmptyCopyReturned = false, fillCommitted = false, rightEmptyCopyReturned = false;
    bool candidateOldNullFreeReturned = false;
    std::uint64_t initializedBits = 0;
    std::array<UnpublishedScalarEffect, 16> effects{};
    std::size_t effectCount = 0;
    static constexpr std::uintptr_t sourceCaller = 0x140832a1d, guardSourceInstruction = 0x140832adc;
    static constexpr bool originalGlobalObserved = false, bodyPublished = false, slotPublished = false;
    static constexpr bool heapFreeEntered = false, nativeFreeObserved = false, temporaryReleased = false, bufferReleased = false;
    static constexpr bool handlerEntered = false, errnoEntered = false, vectorReturned = false, wholeLeafReturned = false;
};
// Stack-borrowed proof used synchronously after T and immediately before B.
// The app bridge supplies its sealed current-frame proof; this callback never escapes.
using CurrentScalarProof = bool (*)(const void*) noexcept;
class TemporaryZeroCall;
enum class Temp0PrepareStatus { Prepared, LeaseMismatch, ProducerUnavailable, CapacityUnavailable };
struct Temp0PrepareResult {
    Temp0PrepareStatus status = Temp0PrepareStatus::ProducerUnavailable;
    std::shared_ptr<TemporaryZeroCall> call;
};
class ModuleHeapRole final {
public:
    // Explicit opt-in. All owner/call/report backing exists before HeapCreate.
    // One retained role/call per containing module; never reset or HeapDestroy.
    static std::shared_ptr<ModuleHeapRole> prepareCurrentModule();
    ~ModuleHeapRole();
    ModuleHeapRole(const ModuleHeapRole&) = delete;
    ModuleHeapRole& operator=(const ModuleHeapRole&) = delete;
    bool matchesRetainedLease(const std::shared_ptr<ModuleHeapRole>&) const noexcept;
    HeapRoleProducerReport attemptProducer() noexcept;
    HeapRoleProducerReport producerReport() const noexcept;
    Temp0PrepareResult prepareTemporaryZero(const std::shared_ptr<ModuleHeapRole>&,
        HeapPrefixMode mode = HeapPrefixMode::RawTemporaryOnly) noexcept;
private:
    friend class TemporaryZeroCall;
    ModuleHeapRole();
    struct Storage;
    std::unique_ptr<Storage> storage_;
};
class TemporaryZeroCall final {
public:
    ~TemporaryZeroCall() = default; // No source free/unwind; raw T is process retained.
    TemporaryZeroCall(const TemporaryZeroCall&) = delete;
    TemporaryZeroCall& operator=(const TemporaryZeroCall&) = delete;
    Temp0Report attempt(const std::shared_ptr<ModuleHeapRole>&) noexcept;
    // Fresh integrated entry only: never continues an already-started raw T.
    UnpublishedScalarReport attemptUnpublishedScalar(const std::shared_ptr<ModuleHeapRole>&,
        CurrentScalarProof, const void* currentProofContext) noexcept;
    Temp0Report report() const noexcept;
    UnpublishedScalarReport unpublishedScalarReport() const noexcept;
private:
    friend class ModuleHeapRole;
    explicit TemporaryZeroCall(const std::shared_ptr<ModuleHeapRole>& owner) : owner_(owner) {}
    std::weak_ptr<ModuleHeapRole> owner_;
};
// Pure declared outcomes: no actual handle/pointer or live model authority.
HeapRoleProducerReport declaredHeapRoleProducer(bool heapNonNull, int compatibilityResult) noexcept;
Temp0Report declaredTemporaryZeroOutcome(bool heapNonNull, bool returnNonNull) noexcept;
UnpublishedScalarReport declaredUnpublishedScalarOutcome(bool tempNonNull, bool currentOwnerMatches, bool valueNonNull) noexcept;

} // namespace def::rt
