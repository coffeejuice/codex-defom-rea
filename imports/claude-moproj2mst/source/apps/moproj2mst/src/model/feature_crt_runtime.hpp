// Isolated source-role CRT producer; no Project binding or allocator interface.
#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>

namespace m2m::detail {
enum class FeatureCrtOrigin { DeclaredModel, ActualWin32 };
enum class FeatureCrtProducerStage {
    NotAttempted, CompatibilityCallPending, HeapCreateReturnedNull, CompatibilityCallReturned
};
enum class FeatureCrtProducerStop {
    NotStarted, StartupFatalModeUnavailable, LaterStartupUnavailable,
    ModuleScopeUnavailable, CompatibilityObservationUnavailable, DeclaredInputConflict
};
enum class FeatureCrtEffect {
    None, HeapCreateReturned, CandidateHeapSlotPublished, HeapNullTested,
    CompatibilityCallReturned, ProducerReturned
};

struct FeatureCrtProducerReport {
    static constexpr const char* sourceBinarySha256 =
        "fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc";
    static constexpr std::uint64_t sourceProducer = 0x141484800;
    static constexpr std::uint64_t originalHeapSlotSourceRole = 0x144700810;
    FeatureCrtOrigin origin = FeatureCrtOrigin::DeclaredModel;
    FeatureCrtProducerStage stage = FeatureCrtProducerStage::NotAttempted;
    FeatureCrtProducerStop stop = FeatureCrtProducerStop::NotStarted;
    // Identities and ActualWin32 origin refer only to the candidate process/module.
    const void* runtimeIdentity = nullptr; // observation only, never a handle
    std::uint32_t processId = 0;
    std::uintptr_t moduleIdentity = 0;
    bool modulePinned = false; // candidate host lifetime instrumentation
    bool ownedTestProcessRetention = false;
    bool candidateSlotPublished = false; // Candidate member store only; original EXE global is not observed.
    std::optional<bool> heapNonNull;
    std::optional<int> compatibilityReturn; // actual BOOL word or declared model word
    std::optional<unsigned> producerReturn;
    // Counts/events are declared only when origin==DeclaredModel.
    unsigned heapCreateCalls = 0;
    unsigned candidateSlotPublicationStores = 0;
    unsigned compatibilityCalls = 0;
    std::array<FeatureCrtEffect, 5> effects{};
    std::size_t effectCount = 0;
    const char* nextSourceSite = nullptr;
    // These flags describe this producer receipt, not a later isolated call.
    static constexpr bool completeCrtStartup = false;
    static constexpr bool projectBindingAdmitted = false;
    static constexpr bool proxyAllocationAdmitted = false;
    static constexpr bool nativeTeardownCompleted = false;
    static constexpr bool completeForPublicClose = false;
};

struct DeclaredFeatureCrtProjection {
    bool valid = false;
    FeatureCrtProducerReport progress;
};
enum class FeatureCrtEncodedHandlerStop {
    NotStarted, OwnerMismatch, ProducerUnavailable, ChildFrameAlreadyPrepared, NextInitializerUnavailable
};
enum class FeatureCrtEncodedHandlerEffect {
    EncodePointerEntered, EncodePointerReturned, CandidateRoleStored, FirstSetterReturned
};
struct FeatureCrtEncodedHandlerEvent {
    std::uint64_t sourceSite = 0;
    FeatureCrtEncodedHandlerEffect effect = FeatureCrtEncodedHandlerEffect::EncodePointerEntered;
};
struct FeatureCrtEncodedHandlerReport {
    FeatureCrtOrigin origin = FeatureCrtOrigin::DeclaredModel;
    const void* runtimeIdentity = nullptr;
    std::uint32_t processId = 0;
    std::uintptr_t moduleIdentity = 0;
    unsigned encodePointerCalls = 0;
    bool encodePointerReturned = false;
    std::uintptr_t encodePointerArgument = 0;
    std::uintptr_t encodedNullResult = 0; // Opaque actual result; zero is not a failure predicate.
    unsigned candidateRoleStores = 0;
    bool candidateRoleKnown = false;
    std::uintptr_t observedCandidateRole = 0;
    bool firstSetterReturned = false;
    std::array<FeatureCrtEncodedHandlerEvent, 4> effects{};
    std::size_t effectCount = 0;
    FeatureCrtEncodedHandlerStop stop = FeatureCrtEncodedHandlerStop::NotStarted;
    const char* nextSourceSite = nullptr;
    static constexpr std::uint64_t originalRoleSource = 0x1447006e0;
    static constexpr bool originalGlobalObserved = false;
    static constexpr bool hostCrtHandlerChanged = false;
    static constexpr bool handlerInvoked = false;
    static constexpr bool flsEntered = false;
    static constexpr bool lastErrorObserved = false;
    static constexpr bool completeCrtStartup = false;
    static constexpr bool completeForPublicClose = false;
};
enum class FeatureCrtNullRoleTailStop {
    NotStarted, OwnerMismatch, ProducerUnavailable, ChildFrameAlreadyPrepared,
    FirstPrefixUnavailable, SourceArgumentUnavailable, NonNullCallbackInitializerUnavailable
};
enum class FeatureCrtNullRoleTailEffect { CandidateRoleStored, LeafReturned, NoOpReturned };
struct FeatureCrtNullRoleTailEvent {
    std::uint64_t sourceSite = 0;
    FeatureCrtNullRoleTailEffect effect = FeatureCrtNullRoleTailEffect::CandidateRoleStored;
    std::uint64_t originalRoleSource = 0; // Label only, never a writable original address.
    std::uintptr_t observedValue = 0;
    bool storedValueKnown = false; // Return instructions do not certify a register return value.
};
struct FeatureCrtNullRoleTailReport {
    FeatureCrtOrigin origin = FeatureCrtOrigin::DeclaredModel;
    const void* runtimeIdentity = nullptr;
    std::uint32_t processId = 0;
    std::uintptr_t moduleIdentity = 0;
    bool started = false;
    bool savedArgumentKnown = false;
    std::uintptr_t savedArgumentToken = 0; // Same actual CP137 result saved for native RBX.
    std::array<bool, 8> roleKnown{};
    std::array<std::uintptr_t, 8> observedRoles{};
    unsigned roleStores = 0;
    unsigned leafReturns = 0;
    bool noOpReturned = false;
    bool prefixReturned = false;
    std::array<FeatureCrtNullRoleTailEvent, 14> effects{};
    std::size_t effectCount = 0;
    FeatureCrtNullRoleTailStop stop = FeatureCrtNullRoleTailStop::NotStarted;
    const char* nextSourceSite = nullptr;
    static constexpr bool originalGlobalObserved = false;
    static constexpr bool hostCrtRolesChanged = false;
    static constexpr bool additionalEncodingEntered = false;
    static constexpr bool handlerInvoked = false;
    static constexpr bool flsEntered = false;
    static constexpr bool lastErrorObserved = false;
    static constexpr bool completeCrtStartup = false;
    static constexpr bool completeForPublicClose = false;
};
// Pure declared-result projection. It cannot create a runtime or qualify a handle.
DeclaredFeatureCrtProjection projectDeclaredFeatureCrtResults(
    bool heapCreateNonNull, std::optional<int> compatibilityReturn) noexcept;

enum class FeatureMapProxyStage {
    Prepared, AllocationFailurePending, SentinelPending, SentinelAllocationFailurePending, MapConstructorReturned,
    ContextPrefixComplete, EmptyMapClearReturned, SentinelFreeFailurePending, SentinelFreeReturned,
    ProxyFreeFailurePending, MapDestructorReturned
};
enum class FeatureMapProxyStop {
    NotStarted, OwnerUnavailable, ProducerUnavailable, AllocationFailureInputsUnavailable,
    SentinelAllocationUnavailable, NullHeapFatalInputsUnavailable, ProxyPrefixUnavailable, EnclosingContextUnavailable,
    ContextOwnerUnavailable, ChildMapPending, StringRoutineUnavailable,
    EmptyMapPrerequisiteUnavailable, EmptyMapShapeUnavailable, SentinelFreeUnavailable, MapCleanupStarted,
    SentinelFreePrerequisiteUnavailable, SentinelFreeOwnerUnavailable, FreeErrorContinuationUnavailable, ProxyFreeUnavailable,
    ProxyFreePrerequisiteUnavailable, ProxyFreeOwnerUnavailable, EnclosingLifecycleUnavailable
};
enum class FeatureMapProxyEffect {
    None, CandidateHeapSlotRead, HeapNullTested, HeapAllocEntered, HeapAllocReturned,
    ProxyBackpointerStored, MapProxyPublished
};
enum class FeatureMapSentinelEffect {
    None, CandidateHeapSlotRead, HeapNullTested, HeapAllocEntered, HeapAllocReturned,
    Link0NullStored, Link8NullStored, Link10NullStored, Byte28OneStored, NilZeroStored,
    HelperReturnedHead, MapHeadPublished, NilOneStored, MapHeadReloadFor8, Self8Stored,
    MapHeadReloadFor0, Self0Stored, MapHeadReloadFor10, Self10Stored, CountZeroStored, MapConstructorReturned
};
struct FeatureMapSentinelReport {
    FeatureCrtOrigin origin = FeatureCrtOrigin::DeclaredModel;
    std::uintptr_t candidateSlotReadIdentity = 0;
    std::optional<bool> heapReadNonNull;
    unsigned candidateSlotReads = 0;
    std::uintptr_t heapAllocHeapArgument = 0;
    std::size_t heapAllocBytesArgument = 0;
    unsigned heapAllocFlagsArgument = 0;
    unsigned heapAllocCalls = 0;
    bool heapAllocReturned = false;
    std::optional<bool> allocationNonNull;
    const void* candidateHeadIdentity = nullptr; // Constructor history only after sentinel retirement.
    std::array<const void*, 3> initialLinks{}; // +0,+8,+10; read only after their stores.
    bool initialLinksKnown = false;
    bool byte28Known = false;
    unsigned char observedByte28 = 0;
    bool initialNilKnown = false;
    unsigned char initialNil = 0;
    const void* helperReturnIdentity = nullptr;
    const void* observedMapHead = nullptr;
    bool finalNilKnown = false;
    unsigned char finalNil = 0;
    std::array<const void*, 3> headReloadValues{}; // Source order +8,+0,+10.
    std::array<const void*, 3> selfLinkValues{};
    unsigned headReloads = 0;
    unsigned selfLinkStores = 0;
    std::uint64_t observedMapCount = 0; // Parent countKnown qualifies this value.
    const void* returnedMapIdentity = nullptr;
    bool keyPayloadKnown = false;
    bool suffixKnown = false;
    std::array<FeatureMapSentinelEffect, 20> effects{};
    std::size_t effectCount = 0;
    FeatureMapProxyStop stop = FeatureMapProxyStop::NotStarted;
    const char* nextSourceSite = nullptr;
};
enum class FeatureContextReceiverProfile { StandaloneMap, ContextPrefix };
struct FeatureContextStore {
    std::uint64_t sourceSite = 0; // Retained original instruction role, not a live original PC.
    unsigned offset = 0;
    unsigned width = 0;
    std::uint32_t observedValue = 0;
};
struct FeatureContextPrefixReport {
    FeatureCrtOrigin origin = FeatureCrtOrigin::DeclaredModel;
    FeatureContextReceiverProfile profile = FeatureContextReceiverProfile::StandaloneMap;
    const void* candidateContextIdentity = nullptr;
    const void* candidateEmbeddedMapIdentity = nullptr;
    const void* resetContextArgument = nullptr;
    bool resetDlArgumentKnown = false;
    unsigned char resetDlArgument = 0;
    std::optional<std::int32_t> cache1498;
    std::optional<std::int32_t> word3c;
    std::optional<std::int32_t> word40;
    std::optional<std::int32_t> word44;
    std::array<bool, 0x48> byteKnown{};
    std::array<unsigned char, 0x48> observedBytes{}; // Meaningful only where byteKnown is true.
    bool prefixCompleted = false;
    bool candidateContextConstructorReturned = false;
    bool stringsKnown = false;
    bool fallback1448Known = false;
    bool optionalOwner1450Known = false;
    bool stringRoutineEntered = false;
    const void* nextStringDestination = nullptr;
    static constexpr std::uint64_t nextStringSourceRole = 0x142146f86;
    std::array<FeatureContextStore, 55> effects{};
    std::size_t effectCount = 0;
    FeatureMapProxyStop stop = FeatureMapProxyStop::NotStarted;
    const char* nextSourceSite = nullptr;
    static constexpr bool originalGlobalObserved = false;
    static constexpr bool projectBindingAdmitted = false;
    static constexpr bool nativeCleanupPerformed = false;
    static constexpr bool completeForPublicClose = false;
};
enum class FeatureMapEmptyClearEffect {
    FullRangeSelected, EmptyRootSkipped, HeadReloadFor8, Self8Stored,
    HeadReloadFor0, CountZeroStored, Self0Stored, HeadReloadFor10, Self10Stored,
    ClearReturned, OutputHeadReloaded, OutputNodeStored, OutputOwnerStored, SentinelFreeArgumentLoaded
};
struct FeatureMapEmptyClearEvent {
    std::uint64_t sourceSite = 0;
    FeatureMapEmptyClearEffect effect = FeatureMapEmptyClearEffect::FullRangeSelected;
};
struct FeatureMapEmptyClearReport {
    FeatureCrtOrigin origin = FeatureCrtOrigin::DeclaredModel;
    bool started = false;
    bool fullRangeSelected = false;
    bool emptyRootSkipped = false;
    bool prefixReturned = false; // Candidate prefix return, not the native map destructor return.
    const void* mapIdentity = nullptr;
    const void* proxyIdentity = nullptr;
    const void* headIdentity = nullptr;
    const void* rootIdentity = nullptr;
    std::optional<unsigned char> rootNil;
    std::array<const void*, 3> headReloadValues{}; // Reset order +8,+0,+10.
    std::array<const void*, 3> selfLinkValues{};
    std::optional<std::uint64_t> countAfterStore;
    const void* outputHeadIdentity = nullptr;
    const void* outputIteratorNode = nullptr;
    const void* outputIteratorOwner = nullptr;
    const void* sentinelFreeArgument = nullptr; // Loaded only; never passed to free here.
    std::array<FeatureMapEmptyClearEvent, 14> effects{};
    std::size_t effectCount = 0;
    FeatureMapProxyStop stop = FeatureMapProxyStop::NotStarted;
    const char* nextSourceSite = nullptr;
    static constexpr bool sentinelFreeEntered = false; // This earlier clear phase only, even after a later free.
    static constexpr bool proxyFreeEntered = false; // This earlier clear phase only.
    static constexpr bool nativeContextDestructorEntered = false;
    static constexpr bool completeForPublicClose = false;
};
enum class FeatureMapSentinelFreeEffect {
    PointerNonNullTested, CandidateHeapSlotRead, HeapFreeEntered, HeapFreeReturned,
    HeapFreeResultTested, SourceFreeReturned, MapHeadNullStored, MapCountZeroStored,
    MapInnerDestructorReturned, ProxyFreeArgumentLoaded
};
struct FeatureMapSentinelFreeEvent {
    std::uint64_t sourceSite = 0;
    FeatureMapSentinelFreeEffect effect = FeatureMapSentinelFreeEffect::PointerNonNullTested;
};
struct FeatureMapSentinelFreeReport {
    FeatureCrtOrigin origin = FeatureCrtOrigin::DeclaredModel;
    bool started = false;
    const void* mapIdentity = nullptr; // M remains live at this phase's proxy-free guard.
    std::optional<bool> sourcePointerNonNull;
    std::uintptr_t candidateSlotReadIdentity = 0;
    unsigned candidateSlotReads = 0;
    std::uintptr_t heapFreeHeapArgument = 0;
    unsigned heapFreeFlagsArgument = 0;
    std::uintptr_t heapFreePointerArgument = 0; // Opaque historical token; never dereference retired H.
    unsigned heapFreeCalls = 0;
    bool heapFreeReturned = false;
    std::optional<int> heapFreeResult; // Raw actual BOOL word, or explicit declared-model result.
    bool sourceFreeReturned = false;
    bool sentinelRetired = false;
    bool headNullStored = false;
    const void* observedMapHead = nullptr; // Qualified only by headNullStored.
    std::optional<std::uint64_t> countAfterStore;
    bool mapInnerDestructorReturned = false;
    const void* proxyFreeArgument = nullptr; // A was live at this phase's guard; history after later proxy free.
    std::array<FeatureMapSentinelFreeEvent, 10> effects{};
    std::size_t effectCount = 0;
    FeatureMapProxyStop stop = FeatureMapProxyStop::NotStarted;
    const char* nextSourceSite = nullptr;
    static constexpr bool errnoProviderEntered = false;
    static constexpr bool getLastErrorEntered = false;
    static constexpr bool proxyFreeEntered = false; // This earlier sentinel-free phase only.
    static constexpr bool heapDestroyEntered = false;
    static constexpr bool nativeContextDestructorEntered = false;
    static constexpr bool completeForPublicClose = false;
};
enum class FeatureMapProxyFreeEffect {
    PointerNonNullTested, CandidateHeapSlotRead, HeapFreeEntered, HeapFreeReturned,
    HeapFreeResultTested, TailDestructorReturned
};
struct FeatureMapProxyFreeEvent {
    std::uint64_t sourceSite = 0;
    FeatureMapProxyFreeEffect effect = FeatureMapProxyFreeEffect::PointerNonNullTested;
};
struct FeatureMapProxyFreeReport {
    FeatureCrtOrigin origin = FeatureCrtOrigin::DeclaredModel;
    bool started = false;
    std::uintptr_t mapIdentityToken = 0; // Host backing identity, never a live source-map capability after return.
    std::uintptr_t mapProxyTokenBefore = 0; // Native M.proxy is never nulled by this tail.
    std::uintptr_t backpointerTokenBefore = 0; // Qualified only while A is still live.
    std::optional<bool> sourcePointerNonNull;
    std::uintptr_t candidateSlotReadIdentity = 0;
    unsigned candidateSlotReads = 0;
    std::uintptr_t heapFreeHeapArgument = 0;
    unsigned heapFreeFlagsArgument = 0;
    std::uintptr_t heapFreePointerArgument = 0; // Historical A token only; never dereference after success.
    unsigned heapFreeCalls = 0;
    bool heapFreeReturned = false;
    std::optional<int> heapFreeResult;
    bool sourceFreeReturned = false;
    bool proxyRetired = false;
    bool candidateMapDestructorReturned = false; // Explicit isolated12f0cf0 tail only, not C/Project/Close.
    std::array<FeatureMapProxyFreeEvent, 6> effects{};
    std::size_t effectCount = 0;
    FeatureMapProxyStop stop = FeatureMapProxyStop::NotStarted;
    const char* nextSourceSite = nullptr;
    static constexpr unsigned mapProxyNullStores = 0;
    static constexpr bool errnoProviderEntered = false;
    static constexpr bool getLastErrorEntered = false;
    static constexpr bool heapDestroyEntered = false;
    static constexpr bool nativeContextDestructorEntered = false;
    static constexpr bool completeForPublicClose = false;
};
struct FeatureMapProxyReport {
    FeatureCrtOrigin origin = FeatureCrtOrigin::DeclaredModel;
    FeatureMapProxyStage stage = FeatureMapProxyStage::Prepared;
    FeatureMapProxyStop stop = FeatureMapProxyStop::NotStarted;
    const void* candidateRuntimeIdentity = nullptr;
    const void* candidateMapIdentity = nullptr; // Host backing/history; never a post-destruction source capability.
    std::uintptr_t preparedCandidateHeapIdentity = 0; // Host readiness observation only.
    std::uintptr_t candidateSlotReadIdentity = 0;
    std::optional<bool> heapReadNonNull;
    // Arguments are observed only when heapAllocCalls is nonzero.
    std::uintptr_t heapAllocHeapArgument = 0; // Candidate handle identity, not allocator authority.
    std::size_t heapAllocBytesArgument = 0;
    unsigned heapAllocFlagsArgument = 0;
    unsigned candidateSlotReads = 0;
    unsigned heapAllocCalls = 0;
    bool heapAllocReturned = false;
    std::optional<bool> allocationNonNull;
    const void* candidateProxyIdentity = nullptr; // Constructor history only after proxy retirement.
    const void* observedProxyBackpointer = nullptr;
    const void* observedMapProxy = nullptr;
    unsigned backpointerStores = 0;
    unsigned mapProxyStores = 0;
    bool headKnown = false;
    bool countKnown = false;
    bool candidateMapConstructorReturned = false; // Isolated candidate M only, not a Project context.
    FeatureMapSentinelReport sentinel;
    FeatureContextPrefixReport context;
    FeatureMapEmptyClearReport emptyClear;
    FeatureMapSentinelFreeReport sentinelFree;
    FeatureMapProxyFreeReport proxyFree;
    std::array<FeatureMapProxyEffect, 6> effects{};
    std::size_t effectCount = 0;
    const char* nextSourceSite = nullptr;
    static constexpr bool originalGlobalObserved = false;
    static constexpr bool nativeProjectContextConstructed = false;
    static constexpr bool projectBindingAdmitted = false;
    static constexpr bool nativeUnwindPerformed = false;
    static constexpr bool nativeCleanupPerformed = false;
    static constexpr bool completeForPublicClose = false;
};

// Declared failure only; this creates no live runtime, map or allocation receipt.
FeatureMapProxyReport declaredFeatureMapAllocationFailure() noexcept;
FeatureMapSentinelReport declaredFeatureMapSentinelFailure() noexcept;
// Pure zero-result/error-boundary model; supplies no actual heap/H/frame authority.
FeatureMapSentinelFreeReport declaredFeatureMapSentinelFreeFailure() noexcept;
FeatureMapProxyFreeReport declaredFeatureMapProxyFreeFailure() noexcept;

class FeatureCrtRuntime;
class FeatureMapProxyCall final {
public:
    ~FeatureMapProxyCall();
    FeatureMapProxyCall(const FeatureMapProxyCall&) = delete;
    FeatureMapProxyCall& operator=(const FeatureMapProxyCall&) = delete;
    FeatureMapProxyReport resume() noexcept;
    FeatureMapProxyReport resumeSentinel() noexcept;
    FeatureMapProxyReport resumeContextPrefix() noexcept;
    // Explicit isolated M clear only. No C+1450 read, destructor hook or free.
    // Latches cleanup so later context adoption/replay cannot resume this frame.
    FeatureMapProxyReport resumeEmptyMapCleanupPrefix() noexcept;
    // One actual current-CRT-heap free after explicit empty clear. Success only
    // commits head-null/count-zero; zero stops before native errno work. No retry.
    FeatureMapProxyReport resumeSentinelFreePrefix() noexcept;
    // Actual retained A only, after successful sentinel free. Nonzero completes
    // the isolated map tail; M.proxy stays stale and all A/H access is retired.
    FeatureMapProxyReport resumeProxyFreePrefix() noexcept;
    FeatureMapProxyReport report() const noexcept;
private:
    friend class FeatureCrtRuntime;
    enum class ReceiverKind { StandaloneMap, ContextPrefix };
    FeatureMapProxyCall(const std::shared_ptr<FeatureCrtRuntime>&, std::uintptr_t preparedHeap);
    FeatureMapProxyCall(const std::shared_ptr<FeatureCrtRuntime>&, std::uintptr_t preparedHeap, ReceiverKind);
    struct Storage;
    std::unique_ptr<Storage> storage_;
};
enum class FeatureMapProxyPrepareStatus { Prepared, OwnerMismatch, ProducerUnavailable, ExperimentCapacityUnavailable };
struct FeatureMapProxyPrepareResult {
    FeatureMapProxyPrepareStatus status;
    std::shared_ptr<FeatureMapProxyCall> call;
};

class FeatureCrtRuntime final {
public:
    // Explicit owned-test-process opt-in only. One retained owner per containing
    // module; no default conversion hook. No source HeapCreate until attempt.
    static std::shared_ptr<FeatureCrtRuntime> prepareOwnedTestProcess();
    ~FeatureCrtRuntime();
    FeatureCrtRuntime(const FeatureCrtRuntime&) = delete;
    FeatureCrtRuntime& operator=(const FeatureCrtRuntime&) = delete;
    FeatureCrtProducerReport attemptProducer() noexcept;
    FeatureCrtProducerReport report() const noexcept;
    // Explicit fresh-startup profile only, after real heap producer and before
    // any child-map preparation. No host CRT handler/FLS/LastError operations.
    FeatureCrtEncodedHandlerReport resumeEncodedNewHandlerPrefix(const std::shared_ptr<FeatureCrtRuntime>&) noexcept;
    FeatureCrtEncodedHandlerReport encodedNewHandlerReport() const noexcept;
    // Explicit continuation of the same captured encoding: eight role stores,
    // then the no-op leaf. Stops before147e75c's nonnull callback initializer.
    FeatureCrtNullRoleTailReport resumeEncodedNullRoleTail(const std::shared_ptr<FeatureCrtRuntime>&) noexcept;
    FeatureCrtNullRoleTailReport encodedNullRoleTailReport() const noexcept;
    // Candidate ownership identity only; this does not assert heap readiness,
    // original-global observation, Project binding or native Close admission.
    bool matchesRetainedLease(const std::shared_ptr<FeatureCrtRuntime>&) const noexcept;
    // Host preparation only; may throw host allocation failure before source
    // effects. Capacity is one isolated pending M per owned test runtime.
    FeatureMapProxyPrepareResult prepareOwnedTestMapProxy(const std::shared_ptr<FeatureCrtRuntime>&);
    FeatureMapProxyPrepareResult prepareOwnedTestContextPrefix(const std::shared_ptr<FeatureCrtRuntime>&);
private:
    friend class FeatureMapProxyCall;
    FeatureCrtRuntime();
    struct Storage;
    std::unique_ptr<Storage> storage_;
};
} // namespace m2m::detail
