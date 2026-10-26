#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "feature_crt_runtime.hpp"
#include <new>
#include <type_traits>
#include <cstring>

namespace m2m::detail {
namespace {
constexpr const char* fatalModeSite = "0x141470cac:0x1446ffb58 startup fatal-mode read";
constexpr const char* laterStartupSite = "0x141470cce:0x141481708 later startup";
void moduleAnchor() noexcept {}

class Locked final {
public:
    explicit Locked(SRWLOCK& lock) noexcept : lock_(lock) { AcquireSRWLockExclusive(&lock_); }
    ~Locked() { ReleaseSRWLockExclusive(&lock_); }
    Locked(const Locked&) = delete;
    Locked& operator=(const Locked&) = delete;
private:
    SRWLOCK& lock_;
};

void append(FeatureCrtProducerReport& report, FeatureCrtEffect effect) noexcept
{ report.effects[report.effectCount++] = effect; }

void recordCreateReturn(FeatureCrtProducerReport& report) noexcept
{ append(report, FeatureCrtEffect::HeapCreateReturned); }

void recordCandidateSlotPublication(FeatureCrtProducerReport& report, bool nonNull) noexcept
{
    ++report.candidateSlotPublicationStores;
    report.candidateSlotPublished = true;
    report.heapNonNull = nonNull;
    append(report, FeatureCrtEffect::CandidateHeapSlotPublished);
    append(report, FeatureCrtEffect::HeapNullTested);
    report.stage = FeatureCrtProducerStage::CompatibilityCallPending;
    report.stop = FeatureCrtProducerStop::CompatibilityObservationUnavailable;
    report.nextSourceSite = "0x14148483c:HeapSetInformation";
}

void recordNullProducerReturn(FeatureCrtProducerReport& report) noexcept
{
    report.producerReturn = 0;
    report.stage = FeatureCrtProducerStage::HeapCreateReturnedNull;
    report.stop = FeatureCrtProducerStop::StartupFatalModeUnavailable;
    report.nextSourceSite = fatalModeSite;
    append(report, FeatureCrtEffect::ProducerReturned);
}

void recordCompatibilityReturn(FeatureCrtProducerReport& report, int result) noexcept
{
    report.compatibilityReturn = result;
    append(report, FeatureCrtEffect::CompatibilityCallReturned);
    report.producerReturn = 1; // 141484842 ignores the compatibility BOOL.
    report.stage = FeatureCrtProducerStage::CompatibilityCallReturned;
    report.stop = FeatureCrtProducerStop::LaterStartupUnavailable;
    report.nextSourceSite = laterStartupSite;
    append(report, FeatureCrtEffect::ProducerReturned);
}
}

DeclaredFeatureCrtProjection projectDeclaredFeatureCrtResults(
    bool nonNull, std::optional<int> compatibility) noexcept
{
    DeclaredFeatureCrtProjection result;
    if (!nonNull && compatibility.has_value()) {
        result.progress.stop = FeatureCrtProducerStop::DeclaredInputConflict;
        return result;
    }
    result.valid = true;
    auto& report = result.progress;
    ++report.heapCreateCalls;
    recordCreateReturn(report);
    recordCandidateSlotPublication(report, nonNull);
    if (!nonNull) recordNullProducerReturn(report);
    else if (compatibility.has_value()) {
        ++report.compatibilityCalls;
        recordCompatibilityReturn(report, *compatibility);
    }
    return result;
}

struct FeatureCrtRuntime::Storage {
    SRWLOCK lock = SRWLOCK_INIT;
    HANDLE candidateHeapSlot = nullptr; // Candidate role storage; initialization is not a publication receipt.
    FeatureCrtProducerReport progress;
    void* candidateEncodedNewHandlerSlot = nullptr; // Host backing; unknown until the explicit source store.
    FeatureCrtEncodedHandlerReport encodedHandler;
    std::uintptr_t savedEncodedNullArgument = 0; // Source146ea02 RBX snapshot, not another global role.
    bool savedEncodedNullArgumentKnown = false;
    std::array<std::uintptr_t, 8> candidateEncodedNullRoles{}; // Backing only; unknown until explicit stores.
    FeatureCrtNullRoleTailReport nullRoleTail;
    std::weak_ptr<FeatureCrtRuntime> retainedLease; // Set once before factory publication; no ownership cycle.
    std::shared_ptr<FeatureMapProxyCall> retainedMapCall; // Both standalone M and A outlive inspection leases.
};

namespace {
struct MapHeader;
struct ProxyNode { MapHeader* map; };
struct SentinelHeader {
    SentinelHeader* link0;
    SentinelHeader* link8;
    SentinelHeader* link10;
    std::byte opaque18[0x10];
    unsigned char byte28;
    unsigned char nil29;
    std::byte opaque2a[6];
    SentinelHeader() noexcept {} // Only explicit source stores make fields known.
};
struct MapHeader {
    ProxyNode* proxy;
    std::byte opaque08[0x28];
    void* head;
    std::uint64_t count;
    MapHeader() noexcept {} // Native slots deliberately uninitialized until their source stores.
};
struct ContextEnvelope {
    std::byte opaque00[0x3c];
    std::int32_t word3c;
    std::int32_t word40;
    std::int32_t word44;
    std::byte opaque48[0x1410];
    MapHeader map;
    std::int32_t cache1498;
    ContextEnvelope() noexcept {} // Host receiver backing; no native default/zero initialization.
};
static_assert(std::is_standard_layout_v<MapHeader> && sizeof(void*) == 8);
static_assert(offsetof(MapHeader, proxy) == 0 && offsetof(MapHeader, head) == 0x30);
static_assert(offsetof(MapHeader, count) == 0x38 && sizeof(ProxyNode) == 8);
static_assert(std::is_trivially_destructible_v<MapHeader> && std::is_trivially_destructible_v<ProxyNode>);
static_assert(std::is_standard_layout_v<SentinelHeader> && sizeof(SentinelHeader) == 48);
static_assert(offsetof(SentinelHeader, link0) == 0 && offsetof(SentinelHeader, link8) == 8);
static_assert(offsetof(SentinelHeader, link10) == 0x10 && offsetof(SentinelHeader, byte28) == 0x28);
static_assert(offsetof(SentinelHeader, nil29) == 0x29 && std::is_trivially_destructible_v<SentinelHeader>);
static_assert(std::is_standard_layout_v<ContextEnvelope> && std::is_trivially_destructible_v<ContextEnvelope>);
static_assert(offsetof(ContextEnvelope, word3c) == 0x3c && offsetof(ContextEnvelope, word40) == 0x40);
static_assert(offsetof(ContextEnvelope, word44) == 0x44 && offsetof(ContextEnvelope, map) == 0x1458);
static_assert(offsetof(ContextEnvelope, cache1498) == 0x1498 && sizeof(ContextEnvelope) == 0x14a0);
static_assert(sizeof(std::int32_t) == 4);

void appendMap(FeatureMapProxyReport& report, FeatureMapProxyEffect effect) noexcept
{ report.effects[report.effectCount++] = effect; }
void recordMapHeapRead(FeatureMapProxyReport& report, std::uintptr_t heap, bool nonNull) noexcept
{
    ++report.candidateSlotReads;
    report.candidateSlotReadIdentity = heap;
    report.heapReadNonNull = nonNull;
    appendMap(report, FeatureMapProxyEffect::CandidateHeapSlotRead);
    appendMap(report, FeatureMapProxyEffect::HeapNullTested);
}
void recordMapAllocationEntry(FeatureMapProxyReport& report, std::uintptr_t heap) noexcept
{
    report.heapAllocHeapArgument = heap;
    report.heapAllocBytesArgument = 8;
    report.heapAllocFlagsArgument = 0;
    ++report.heapAllocCalls;
    appendMap(report, FeatureMapProxyEffect::HeapAllocEntered);
}
void recordMapAllocationReturn(FeatureMapProxyReport& report, bool nonNull) noexcept
{
    report.heapAllocReturned = true;
    report.allocationNonNull = nonNull;
    appendMap(report, FeatureMapProxyEffect::HeapAllocReturned);
    if (!nonNull) {
        report.stage = FeatureMapProxyStage::AllocationFailurePending;
        report.stop = FeatureMapProxyStop::AllocationFailureInputsUnavailable;
        report.nextSourceSite = "0x1414748ff:0x144700858 new-mode read";
    }
}

void appendSentinel(FeatureMapSentinelReport& report, FeatureMapSentinelEffect effect) noexcept
{ report.effects[report.effectCount++] = effect; }
void recordSentinelHeapRead(FeatureMapSentinelReport& report, std::uintptr_t heap, bool nonNull) noexcept
{
    ++report.candidateSlotReads;
    report.candidateSlotReadIdentity = heap;
    report.heapReadNonNull = nonNull;
    appendSentinel(report, FeatureMapSentinelEffect::CandidateHeapSlotRead);
    appendSentinel(report, FeatureMapSentinelEffect::HeapNullTested);
}
void recordSentinelAllocationEntry(FeatureMapSentinelReport& report, std::uintptr_t heap) noexcept
{
    report.heapAllocHeapArgument = heap;
    report.heapAllocBytesArgument = 48;
    report.heapAllocFlagsArgument = 0;
    ++report.heapAllocCalls;
    appendSentinel(report, FeatureMapSentinelEffect::HeapAllocEntered);
}
void recordSentinelAllocationReturn(FeatureMapSentinelReport& report, bool nonNull) noexcept
{
    report.heapAllocReturned = true;
    report.allocationNonNull = nonNull;
    appendSentinel(report, FeatureMapSentinelEffect::HeapAllocReturned);
    if (!nonNull) {
        report.stop = FeatureMapProxyStop::AllocationFailureInputsUnavailable;
        report.nextSourceSite = "0x1414748ff:0x144700858 new-mode read (sentinel request)";
    }
}

void recordSentinelFreeEffect(FeatureMapSentinelFreeReport& report, std::uint64_t site,
                              FeatureMapSentinelFreeEffect effect) noexcept
{ report.effects[report.effectCount++] = {site, effect}; }
void recordSentinelFreeResult(FeatureMapSentinelFreeReport& report, int result) noexcept
{
    report.heapFreeReturned = true;
    report.heapFreeResult = result;
    recordSentinelFreeEffect(report, 0x141471b98, FeatureMapSentinelFreeEffect::HeapFreeReturned);
    recordSentinelFreeEffect(report, 0x141471b9a, FeatureMapSentinelFreeEffect::HeapFreeResultTested);
    if (result == 0) {
        report.stop = FeatureMapProxyStop::FreeErrorContinuationUnavailable;
        report.nextSourceSite = "0x141471b9c:CRT errno provider unavailable";
    }
}

void recordProxyFreeEffect(FeatureMapProxyFreeReport& report, std::uint64_t site,
                          FeatureMapProxyFreeEffect effect) noexcept
{ report.effects[report.effectCount++] = {site, effect}; }
void recordProxyFreeResult(FeatureMapProxyFreeReport& report, int result) noexcept
{
    report.heapFreeReturned = true;
    report.heapFreeResult = result;
    recordProxyFreeEffect(report, 0x141471b98, FeatureMapProxyFreeEffect::HeapFreeReturned);
    recordProxyFreeEffect(report, 0x141471b9a, FeatureMapProxyFreeEffect::HeapFreeResultTested);
    if (result == 0) {
        report.stop = FeatureMapProxyStop::FreeErrorContinuationUnavailable;
        report.nextSourceSite = "0x141471b9c:CRT errno provider unavailable (proxy free)";
    }
}

struct ContextStoreSpec { std::uint64_t site; unsigned offset; unsigned width; };
constexpr std::array<ContextStoreSpec, 18> firstReset{{
    {0x1412e6fb3,0x00,1},{0x1412e6fb5,0x01,1},{0x1412e6fb8,0x02,1},{0x1412e6fbb,0x03,1},
    {0x1412e6fbe,0x04,1},{0x1412e6fc1,0x34,4},{0x1412e6fc4,0x05,1},{0x1412e6fc7,0x06,1},
    {0x1412e6fca,0x07,1},{0x1412e6fcd,0x08,1},{0x1412e6fd0,0x09,1},{0x1412e6fd3,0x0a,1},
    {0x1412e6fd6,0x0b,1},{0x1412e6fd9,0x2f,1},{0x1412e6fdc,0x30,1},{0x1412e6fdf,0x38,1},
    {0x1412e6fe2,0x39,1},{0x1412e6fe5,0x3a,1}
}};
constexpr std::array<ContextStoreSpec, 14> secondReset{{
    {0x1412e6ff0,0x0c,1},{0x1412e6ff3,0x0e,1},{0x1412e6ff6,0x12,1},{0x1412e6ff9,0x14,1},
    {0x1412e6ffc,0x16,1},{0x1412e6fff,0x18,1},{0x1412e7002,0x1a,1},{0x1412e7005,0x1c,1},
    {0x1412e7008,0x1e,1},{0x1412e700b,0x20,1},{0x1412e700e,0x22,1},{0x1412e7011,0x24,1},
    {0x1412e7014,0x26,1},{0x1412e7017,0x28,1}
}};
constexpr std::array<ContextStoreSpec, 19> thirdReset{{
    {0x1412e7020,0x0d,1},{0x1412e7023,0x0f,1},{0x1412e7026,0x13,1},{0x1412e7029,0x15,1},
    {0x1412e702c,0x17,1},{0x1412e702f,0x19,1},{0x1412e7032,0x1b,1},{0x1412e7035,0x1d,1},
    {0x1412e7038,0x1f,1},{0x1412e703b,0x21,1},{0x1412e703e,0x23,1},{0x1412e7041,0x25,1},
    {0x1412e7044,0x27,1},{0x1412e7047,0x29,1},{0x1412e704a,0x2a,1},{0x1412e704d,0x2b,1},
    {0x1412e7050,0x2c,1},{0x1412e7053,0x2d,1},{0x1412e7056,0x2e,1}
}};
static_assert(firstReset.size() + secondReset.size() + thirdReset.size() + 4 == 55);
void recordContextStore(ContextEnvelope& context, FeatureContextPrefixReport& report,
                        ContextStoreSpec source) noexcept
{
    const auto* bytes = reinterpret_cast<const unsigned char*>(&context);
    std::uint32_t value = 0;
    std::memcpy(&value, bytes + source.offset, source.width); // Only the just-written known field.
    report.effects[report.effectCount++] = {source.site, source.offset, source.width, value};
    for (unsigned i = 0; i < source.width && source.offset + i < report.byteKnown.size(); ++i) {
        report.byteKnown[source.offset + i] = true;
        report.observedBytes[source.offset + i] = bytes[source.offset + i];
    }
}
template<std::size_t N>
void resetContextLeaf(ContextEnvelope& context, FeatureContextPrefixReport& report,
                      const std::array<ContextStoreSpec, N>& stores) noexcept
{
    auto* bytes = reinterpret_cast<unsigned char*>(&context);
    const std::uint32_t zero = 0; // Source DL0; DWORD34 receives its zero-extended value.
    for (const auto& source : stores) {
        std::memcpy(bytes + source.offset, &zero, source.width);
        recordContextStore(context, report, source);
    }
}
}

struct FeatureMapProxyCall::Storage {
    SRWLOCK lock = SRWLOCK_INIT;
    std::weak_ptr<FeatureCrtRuntime> runtime;
    std::unique_ptr<ContextEnvelope> contextOwner;
    MapHeader standaloneMap;
    MapHeader& map; // Selected once at preparation, never rebound after a native effect.
    SentinelHeader* headAllocation = nullptr; // Retained even before M.head publication.
    FeatureMapProxyReport progress;
    Storage(const std::shared_ptr<FeatureCrtRuntime>& owner, std::uintptr_t heap,
            ReceiverKind kind = ReceiverKind::StandaloneMap)
        : runtime(owner),
          contextOwner(kind == ReceiverKind::ContextPrefix ? std::make_unique<ContextEnvelope>() : nullptr),
          map(contextOwner ? contextOwner->map : standaloneMap)
    {
        progress.origin = FeatureCrtOrigin::ActualWin32; // Candidate runtime, not original global observation.
        progress.candidateRuntimeIdentity = owner.get();
        progress.candidateMapIdentity = &map;
        progress.preparedCandidateHeapIdentity = heap;
        progress.sentinel.origin = FeatureCrtOrigin::ActualWin32;
        progress.context.origin = FeatureCrtOrigin::ActualWin32;
        progress.emptyClear.origin = FeatureCrtOrigin::ActualWin32;
        progress.sentinelFree.origin = FeatureCrtOrigin::ActualWin32;
        progress.proxyFree.origin = FeatureCrtOrigin::ActualWin32;
        if (contextOwner) {
            progress.context.profile = FeatureContextReceiverProfile::ContextPrefix;
            progress.context.candidateContextIdentity = contextOwner.get();
            progress.context.candidateEmbeddedMapIdentity = &contextOwner->map;
        }
    }
};

FeatureMapProxyCall::FeatureMapProxyCall(const std::shared_ptr<FeatureCrtRuntime>& owner, std::uintptr_t heap)
    : storage_(std::make_unique<Storage>(owner, heap)) {}
FeatureMapProxyCall::FeatureMapProxyCall(const std::shared_ptr<FeatureCrtRuntime>& owner, std::uintptr_t heap,
                                       ReceiverKind kind)
    : storage_(std::make_unique<Storage>(owner, heap, kind)) {}
FeatureMapProxyCall::~FeatureMapProxyCall() = default; // Host metadata only; no native unwind/free.

FeatureMapProxyReport declaredFeatureMapAllocationFailure() noexcept
{
    FeatureMapProxyReport report;
    recordMapHeapRead(report, 0, true); // Declared positive check; no actual candidate handle is supplied.
    recordMapAllocationEntry(report, 0);
    recordMapAllocationReturn(report, false);
    return report;
}

FeatureMapSentinelReport declaredFeatureMapSentinelFailure() noexcept
{
    FeatureMapSentinelReport report;
    recordSentinelHeapRead(report, 0, true); // Declared positive check, not an actual handle.
    recordSentinelAllocationEntry(report, 0);
    recordSentinelAllocationReturn(report, false);
    return report;
}

FeatureMapSentinelFreeReport declaredFeatureMapSentinelFreeFailure() noexcept
{
    FeatureMapSentinelFreeReport report;
    report.started = true;
    report.sourcePointerNonNull = true; // Declared source condition, no actual pointer/handle.
    recordSentinelFreeEffect(report, 0x141471b7c, FeatureMapSentinelFreeEffect::PointerNonNullTested);
    ++report.candidateSlotReads;
    recordSentinelFreeEffect(report, 0x141471b89, FeatureMapSentinelFreeEffect::CandidateHeapSlotRead);
    ++report.heapFreeCalls; // Declared occurrence only; no kernel call.
    recordSentinelFreeEffect(report, 0x141471b92, FeatureMapSentinelFreeEffect::HeapFreeEntered);
    recordSentinelFreeResult(report, 0);
    return report;
}

FeatureMapProxyFreeReport declaredFeatureMapProxyFreeFailure() noexcept
{
    FeatureMapProxyFreeReport report;
    report.started = true;
    report.sourcePointerNonNull = true; // Model-only nonnull branch, not an actual address/handle.
    recordProxyFreeEffect(report, 0x141471b7c, FeatureMapProxyFreeEffect::PointerNonNullTested);
    ++report.candidateSlotReads;
    recordProxyFreeEffect(report, 0x141471b89, FeatureMapProxyFreeEffect::CandidateHeapSlotRead);
    ++report.heapFreeCalls;
    recordProxyFreeEffect(report, 0x141471b92, FeatureMapProxyFreeEffect::HeapFreeEntered);
    recordProxyFreeResult(report, 0);
    return report;
}

FeatureCrtRuntime::FeatureCrtRuntime() : storage_(std::make_unique<Storage>())
{
    auto& report = storage_->progress;
    report.origin = FeatureCrtOrigin::ActualWin32;
    report.runtimeIdentity = this;
    report.processId = GetCurrentProcessId();
    report.ownedTestProcessRetention = true; // Only the retaining private factory constructs this type.
    storage_->encodedHandler.origin = FeatureCrtOrigin::ActualWin32;
    storage_->encodedHandler.runtimeIdentity = this;
    storage_->encodedHandler.processId = report.processId;
    storage_->nullRoleTail.origin = FeatureCrtOrigin::ActualWin32;
    storage_->nullRoleTail.runtimeIdentity = this;
    storage_->nullRoleTail.processId = report.processId;
    // Host module-lifetime qualification, before source execution. PIN keeps
    // the isolated owner/code valid until this owned test process ends.
    HMODULE module = nullptr;
    if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN,
                           reinterpret_cast<LPCWSTR>(&moduleAnchor), &module)) {
        report.moduleIdentity = reinterpret_cast<std::uintptr_t>(module);
        report.modulePinned = true;
        storage_->encodedHandler.moduleIdentity = report.moduleIdentity;
        storage_->nullRoleTail.moduleIdentity = report.moduleIdentity;
    } else {
        report.stop = FeatureCrtProducerStop::ModuleScopeUnavailable;
    }
}

FeatureCrtRuntime::~FeatureCrtRuntime() = default; // Host metadata only; never HeapDestroy.

std::shared_ptr<FeatureCrtRuntime> FeatureCrtRuntime::prepareOwnedTestProcess()
{
    // Deliberate, bounded process retention for this opt-in experiment. There
    // is no static destructor, Project owner or reciprocal shared_ptr cycle.
    // All host allocations happen before any source kernel producer call.
    static auto* retained = [] {
        auto* owner = new std::shared_ptr<FeatureCrtRuntime>(new FeatureCrtRuntime);
        (*owner)->storage_->retainedLease = *owner;
        return owner;
    }();
    return *retained;
}

bool FeatureCrtRuntime::matchesRetainedLease(const std::shared_ptr<FeatureCrtRuntime>& lease) const noexcept
{
    if (!lease || lease.get() != this) return false;
    const auto canonical = storage_->retainedLease.lock();
    return canonical && canonical.get() == this &&
        !canonical.owner_before(lease) && !lease.owner_before(canonical);
}

FeatureMapProxyPrepareResult FeatureCrtRuntime::prepareOwnedTestMapProxy(const std::shared_ptr<FeatureCrtRuntime>& lease)
{
    if (!matchesRetainedLease(lease)) return {FeatureMapProxyPrepareStatus::OwnerMismatch, {}};
    Locked lock(storage_->lock);
    if (!storage_->candidateHeapSlot || !storage_->progress.candidateSlotPublished ||
        storage_->progress.stage != FeatureCrtProducerStage::CompatibilityCallReturned)
        return {FeatureMapProxyPrepareStatus::ProducerUnavailable, {}};
    if (storage_->retainedMapCall) return {FeatureMapProxyPrepareStatus::ExperimentCapacityUnavailable, {}};
    // All host allocation/retention is committed before a later explicit resume.
    auto call = std::shared_ptr<FeatureMapProxyCall>(
        new FeatureMapProxyCall(lease, reinterpret_cast<std::uintptr_t>(storage_->candidateHeapSlot)));
    storage_->retainedMapCall = call;
    return {FeatureMapProxyPrepareStatus::Prepared, call};
}

FeatureMapProxyPrepareResult FeatureCrtRuntime::prepareOwnedTestContextPrefix(const std::shared_ptr<FeatureCrtRuntime>& lease)
{
    if (!matchesRetainedLease(lease)) return {FeatureMapProxyPrepareStatus::OwnerMismatch, {}};
    Locked lock(storage_->lock);
    if (!storage_->candidateHeapSlot || !storage_->progress.candidateSlotPublished ||
        storage_->progress.stage != FeatureCrtProducerStage::CompatibilityCallReturned)
        return {FeatureMapProxyPrepareStatus::ProducerUnavailable, {}};
    if (storage_->retainedMapCall) return {FeatureMapProxyPrepareStatus::ExperimentCapacityUnavailable, {}};
    auto call = std::shared_ptr<FeatureMapProxyCall>(new FeatureMapProxyCall(
        lease, reinterpret_cast<std::uintptr_t>(storage_->candidateHeapSlot), FeatureMapProxyCall::ReceiverKind::ContextPrefix));
    storage_->retainedMapCall = call;
    return {FeatureMapProxyPrepareStatus::Prepared, call};
}

FeatureMapProxyReport FeatureMapProxyCall::resume() noexcept
{
    const auto runtime = storage_->runtime.lock();
    if (!runtime) {
        Locked lock(storage_->lock);
        auto report = storage_->progress;
        report.stop = FeatureMapProxyStop::OwnerUnavailable;
        return report;
    }
    Locked ownerLock(runtime->storage_->lock);
    Locked callLock(storage_->lock);
    auto& report = storage_->progress;
    if (report.stage != FeatureMapProxyStage::Prepared || report.candidateSlotReads || report.heapAllocCalls) return report;
    if (!runtime->matchesRetainedLease(runtime) || !runtime->storage_->progress.candidateSlotPublished ||
        runtime->storage_->progress.stage != FeatureCrtProducerStage::CompatibilityCallReturned) {
        report.stop = FeatureMapProxyStop::ProducerUnavailable;
        return report;
    }
    const HANDLE heap = runtime->storage_->candidateHeapSlot;
    recordMapHeapRead(report, reinterpret_cast<std::uintptr_t>(heap), heap != nullptr);
    if (!heap) {
        report.stop = FeatureMapProxyStop::NullHeapFatalInputsUnavailable;
        report.nextSourceSite = "0x1414748cc:unqualified null-heap fatal work";
        return report;
    }
    recordMapAllocationEntry(report, reinterpret_cast<std::uintptr_t>(heap));
    void* const block = HeapAlloc(heap, 0, 8); //1414748f1, actual candidate heap; no zero-fill flag.
    recordMapAllocationReturn(report, block != nullptr);
    if (!block) return report; // Inside malloc, before new-mode; not operator-new returning null.
    auto* const proxy = new (block) ProxyNode{&storage_->map}; //12f1085: actual backpointer store.
    report.candidateProxyIdentity = proxy;
    report.observedProxyBackpointer = proxy->map;
    ++report.backpointerStores;
    appendMap(report, FeatureMapProxyEffect::ProxyBackpointerStored);
    storage_->map.proxy = proxy; //12f108c: publish only after the backpointer.
    report.observedMapProxy = storage_->map.proxy;
    ++report.mapProxyStores;
    appendMap(report, FeatureMapProxyEffect::MapProxyPublished);
    report.stage = FeatureMapProxyStage::SentinelPending;
    report.stop = FeatureMapProxyStop::SentinelAllocationUnavailable;
    report.nextSourceSite = "0x1412f1092:0x1412edac0 sentinel allocation";
    return report;
}

FeatureMapProxyReport FeatureMapProxyCall::report() const noexcept
{
    Locked lock(storage_->lock);
    return storage_->progress;
}

FeatureMapProxyReport FeatureMapProxyCall::resumeSentinel() noexcept
{
    const auto runtime = storage_->runtime.lock();
    if (!runtime) {
        Locked lock(storage_->lock);
        auto report = storage_->progress;
        report.stop = FeatureMapProxyStop::OwnerUnavailable;
        return report;
    }
    Locked ownerLock(runtime->storage_->lock);
    Locked callLock(storage_->lock);
    auto& report = storage_->progress;
    auto& sentinel = report.sentinel;
    if (report.stage != FeatureMapProxyStage::SentinelPending || sentinel.candidateSlotReads || sentinel.heapAllocCalls)
        return report;
    if (!runtime->matchesRetainedLease(runtime) || !runtime->storage_->progress.candidateSlotPublished ||
        runtime->storage_->progress.stage != FeatureCrtProducerStage::CompatibilityCallReturned) {
        report.stop = FeatureMapProxyStop::ProducerUnavailable;
        return report;
    }
    if (report.backpointerStores != 1 || report.mapProxyStores != 1 ||
        storage_->map.proxy != report.candidateProxyIdentity || storage_->map.proxy->map != &storage_->map) {
        report.stop = FeatureMapProxyStop::ProxyPrefixUnavailable;
        return report;
    }
    const HANDLE heap = runtime->storage_->candidateHeapSlot; // New malloc occurrence rereads the candidate slot.
    recordSentinelHeapRead(sentinel, reinterpret_cast<std::uintptr_t>(heap), heap != nullptr);
    if (!heap) {
        report.stop = sentinel.stop = FeatureMapProxyStop::NullHeapFatalInputsUnavailable;
        report.nextSourceSite = sentinel.nextSourceSite = "0x1414748cc:unqualified null-heap fatal work";
        return report;
    }
    recordSentinelAllocationEntry(sentinel, reinterpret_cast<std::uintptr_t>(heap));
    void* const block = HeapAlloc(heap, 0, 48); //12edad2 via1414748f1, real candidate allocation.
    recordSentinelAllocationReturn(sentinel, block != nullptr);
    if (!block) {
        report.stage = FeatureMapProxyStage::SentinelAllocationFailurePending;
        report.stop = sentinel.stop;
        report.nextSourceSite = sentinel.nextSourceSite;
        return report; // No native throw/unwind or proxy rollback before unknown new-mode.
    }
    auto* const head = new (block) SentinelHeader;
    storage_->headAllocation = head;
    sentinel.candidateHeadIdentity = head;
    head->link0 = nullptr; //12edae3.
    sentinel.initialLinks[0] = head->link0;
    appendSentinel(sentinel, FeatureMapSentinelEffect::Link0NullStored);
    head->link8 = nullptr; //12edaf3.
    sentinel.initialLinks[1] = head->link8;
    appendSentinel(sentinel, FeatureMapSentinelEffect::Link8NullStored);
    head->link10 = nullptr; //12edaff.
    sentinel.initialLinks[2] = head->link10;
    sentinel.initialLinksKnown = true;
    appendSentinel(sentinel, FeatureMapSentinelEffect::Link10NullStored);
    head->byte28 = 1;
    sentinel.observedByte28 = head->byte28; sentinel.byte28Known = true;
    appendSentinel(sentinel, FeatureMapSentinelEffect::Byte28OneStored);
    head->nil29 = 0;
    sentinel.initialNil = head->nil29; sentinel.initialNilKnown = true;
    appendSentinel(sentinel, FeatureMapSentinelEffect::NilZeroStored);
    sentinel.helperReturnIdentity = head;
    appendSentinel(sentinel, FeatureMapSentinelEffect::HelperReturnedHead);
    storage_->map.head = head; //12f1097, after helper return.
    sentinel.observedMapHead = storage_->map.head; report.headKnown = true;
    appendSentinel(sentinel, FeatureMapSentinelEffect::MapHeadPublished);
    head->nil29 = 1;
    sentinel.finalNil = head->nil29; sentinel.finalNilKnown = true;
    appendSentinel(sentinel, FeatureMapSentinelEffect::NilOneStored);
    auto* loaded = static_cast<SentinelHeader*>(storage_->map.head);
    sentinel.headReloadValues[0] = loaded; ++sentinel.headReloads;
    appendSentinel(sentinel, FeatureMapSentinelEffect::MapHeadReloadFor8);
    loaded->link8 = loaded;
    sentinel.selfLinkValues[0] = loaded->link8; ++sentinel.selfLinkStores;
    appendSentinel(sentinel, FeatureMapSentinelEffect::Self8Stored);
    loaded = static_cast<SentinelHeader*>(storage_->map.head);
    sentinel.headReloadValues[1] = loaded; ++sentinel.headReloads;
    appendSentinel(sentinel, FeatureMapSentinelEffect::MapHeadReloadFor0);
    loaded->link0 = loaded;
    sentinel.selfLinkValues[1] = loaded->link0; ++sentinel.selfLinkStores;
    appendSentinel(sentinel, FeatureMapSentinelEffect::Self0Stored);
    loaded = static_cast<SentinelHeader*>(storage_->map.head);
    sentinel.headReloadValues[2] = loaded; ++sentinel.headReloads;
    appendSentinel(sentinel, FeatureMapSentinelEffect::MapHeadReloadFor10);
    loaded->link10 = loaded;
    sentinel.selfLinkValues[2] = loaded->link10; ++sentinel.selfLinkStores;
    appendSentinel(sentinel, FeatureMapSentinelEffect::Self10Stored);
    storage_->map.count = 0;
    sentinel.observedMapCount = storage_->map.count; report.countKnown = true;
    appendSentinel(sentinel, FeatureMapSentinelEffect::CountZeroStored);
    report.candidateMapConstructorReturned = true;
    sentinel.returnedMapIdentity = &storage_->map;
    report.stage = FeatureMapProxyStage::MapConstructorReturned;
    report.stop = sentinel.stop = FeatureMapProxyStop::EnclosingContextUnavailable;
    report.nextSourceSite = sentinel.nextSourceSite = "0x1412f10e9:enclosing feature context unavailable";
    appendSentinel(sentinel, FeatureMapSentinelEffect::MapConstructorReturned);
    return report;
}

FeatureMapProxyReport FeatureMapProxyCall::resumeContextPrefix() noexcept
{
    const auto runtime = storage_->runtime.lock();
    if (!runtime) {
        Locked lock(storage_->lock);
        auto report = storage_->progress;
        report.context.stop = FeatureMapProxyStop::OwnerUnavailable;
        return report;
    }
    Locked ownerLock(runtime->storage_->lock);
    Locked callLock(storage_->lock);
    auto& report = storage_->progress;
    auto& prefix = report.context;
    if (report.emptyClear.started) {
        prefix.stop = FeatureMapProxyStop::MapCleanupStarted;
        prefix.nextSourceSite = "0x1412f08b0:isolated map cleanup already started";
        return report;
    }
    if (!storage_->contextOwner) {
        prefix.stop = FeatureMapProxyStop::ContextOwnerUnavailable;
        prefix.nextSourceSite = "0x1412f10e9:standalone map has no enclosing context";
        return report;
    }
    if (prefix.prefixCompleted) return report;
    auto& context = *storage_->contextOwner;
    if (!report.candidateMapConstructorReturned || !report.headKnown || !report.countKnown ||
        report.sentinel.returnedMapIdentity != &context.map) {
        prefix.stop = FeatureMapProxyStop::ChildMapPending;
        prefix.nextSourceSite = "0x1412f10e4:embedded map constructor pending";
        return report;
    }
    if (!runtime->matchesRetainedLease(runtime) || prefix.candidateContextIdentity != &context ||
        prefix.candidateEmbeddedMapIdentity != &context.map || &storage_->map != &context.map) {
        prefix.stop = FeatureMapProxyStop::ContextOwnerUnavailable;
        return report;
    }
    prefix.resetContextArgument = &context;
    prefix.resetDlArgumentKnown = true;
    prefix.resetDlArgument = 0;
    context.cache1498 = -1;
    prefix.cache1498 = context.cache1498;
    recordContextStore(context, prefix, {0x1412f10f0, 0x1498, 4});
    context.word3c = 0;
    prefix.word3c = context.word3c;
    recordContextStore(context, prefix, {0x1412f10fa, 0x3c, 4});
    context.word40 = 0;
    prefix.word40 = context.word40;
    recordContextStore(context, prefix, {0x1412f10fd, 0x40, 4});
    resetContextLeaf(context, prefix, firstReset);
    resetContextLeaf(context, prefix, secondReset);
    resetContextLeaf(context, prefix, thirdReset);
    context.word44 = 0;
    prefix.word44 = context.word44;
    recordContextStore(context, prefix, {0x1412f111a, 0x44, 4});
    prefix.prefixCompleted = true;
    prefix.nextStringDestination = reinterpret_cast<const unsigned char*>(&context) + 0x48;
    report.stage = FeatureMapProxyStage::ContextPrefixComplete;
    report.stop = prefix.stop = FeatureMapProxyStop::StringRoutineUnavailable;
    report.nextSourceSite = prefix.nextSourceSite = "0x1412f111d:0x14146de38 first string routine";
    return report;
}

FeatureMapProxyReport FeatureMapProxyCall::resumeEmptyMapCleanupPrefix() noexcept
{
    const auto runtime = storage_->runtime.lock();
    if (!runtime) {
        Locked lock(storage_->lock);
        auto report = storage_->progress;
        report.emptyClear.stop = FeatureMapProxyStop::OwnerUnavailable;
        return report;
    }
    Locked ownerLock(runtime->storage_->lock);
    Locked callLock(storage_->lock);
    auto& report = storage_->progress;
    auto& clear = report.emptyClear;
    if (clear.started) return report; // Includes the free guard: never replay reset stores.
    if (!runtime->matchesRetainedLease(runtime) || !runtime->storage_->progress.candidateSlotPublished ||
        runtime->storage_->progress.stage != FeatureCrtProducerStage::CompatibilityCallReturned) {
        clear.stop = FeatureMapProxyStop::ProducerUnavailable;
        return report;
    }
    if ((report.stage != FeatureMapProxyStage::MapConstructorReturned &&
         report.stage != FeatureMapProxyStage::ContextPrefixComplete) ||
        !report.candidateMapConstructorReturned || !report.headKnown || !report.countKnown ||
        !report.sentinel.finalNilKnown || !storage_->headAllocation ||
        report.sentinel.returnedMapIdentity != &storage_->map ||
        storage_->map.head != storage_->headAllocation ||
        storage_->map.head != report.sentinel.candidateHeadIdentity ||
        !storage_->map.proxy || storage_->map.proxy != report.candidateProxyIdentity ||
        storage_->map.proxy->map != &storage_->map) {
        clear.stop = FeatureMapProxyStop::EmptyMapPrerequisiteUnavailable;
        return report;
    }
    auto* const head = static_cast<SentinelHeader*>(storage_->map.head);
    // Host admission checks only initialized actual fields, never a fabricated
    // shape/report or unknown key/payload. Nonempty tree effects stay guarded.
    if (storage_->map.count != 0 || head->nil29 != 1 ||
        head->link0 != head || head->link8 != head || head->link10 != head) {
        clear.stop = FeatureMapProxyStop::EmptyMapShapeUnavailable;
        return report;
    }
    struct Iterator { ProxyNode* owner; SentinelHeader* node; };
    const Iterator begin{storage_->map.proxy, head->link0};
    const Iterator end{storage_->map.proxy, head};
    if (!begin.owner || begin.owner != storage_->map.proxy || begin.node != head->link0 ||
        !end.owner || end.owner != storage_->map.proxy || end.node != head) {
        clear.stop = FeatureMapProxyStop::EmptyMapShapeUnavailable;
        return report;
    }
    clear.started = true; // Exclusive diagnostic cursor, before the first source effect.
    clear.mapIdentity = &storage_->map;
    clear.proxyIdentity = storage_->map.proxy;
    clear.headIdentity = head;
    const auto record = [&](std::uint64_t site, FeatureMapEmptyClearEffect effect) noexcept {
        clear.effects[clear.effectCount++] = {site, effect};
    };
    clear.fullRangeSelected = true;
    record(0x1412f0715, FeatureMapEmptyClearEffect::FullRangeSelected);
    auto* const root = head->link8; //12ee426, known live sentinel on this admitted empty shape.
    clear.rootIdentity = root;
    clear.rootNil = root->nil29;
    clear.emptyRootSkipped = true; //12ee431 ->12ee453, no node/key/payload access.
    record(0x1412ee431, FeatureMapEmptyClearEffect::EmptyRootSkipped);
    auto* loaded = static_cast<SentinelHeader*>(storage_->map.head);
    clear.headReloadValues[0] = loaded;
    record(0x1412ee453, FeatureMapEmptyClearEffect::HeadReloadFor8);
    loaded->link8 = loaded;
    clear.selfLinkValues[0] = loaded->link8;
    record(0x1412ee45c, FeatureMapEmptyClearEffect::Self8Stored);
    loaded = static_cast<SentinelHeader*>(storage_->map.head);
    clear.headReloadValues[1] = loaded;
    record(0x1412ee460, FeatureMapEmptyClearEffect::HeadReloadFor0);
    storage_->map.count = 0;
    clear.countAfterStore = storage_->map.count;
    record(0x1412ee464, FeatureMapEmptyClearEffect::CountZeroStored);
    loaded->link0 = loaded;
    clear.selfLinkValues[1] = loaded->link0;
    record(0x1412ee46c, FeatureMapEmptyClearEffect::Self0Stored);
    loaded = static_cast<SentinelHeader*>(storage_->map.head);
    clear.headReloadValues[2] = loaded;
    record(0x1412ee46f, FeatureMapEmptyClearEffect::HeadReloadFor10);
    loaded->link10 = loaded;
    clear.selfLinkValues[2] = loaded->link10;
    record(0x1412ee478, FeatureMapEmptyClearEffect::Self10Stored);
    record(0x1412ee481, FeatureMapEmptyClearEffect::ClearReturned);
    auto* const outputHead = static_cast<SentinelHeader*>(storage_->map.head);
    clear.outputHeadIdentity = outputHead;
    record(0x1412f071a, FeatureMapEmptyClearEffect::OutputHeadReloaded);
    Iterator output; // Native local output is populated node first, then owner.
    output.node = outputHead->link0;
    clear.outputIteratorNode = output.node;
    record(0x1412f0721, FeatureMapEmptyClearEffect::OutputNodeStored);
    output.owner = storage_->map.proxy;
    clear.outputIteratorOwner = output.owner;
    record(0x1412f072a, FeatureMapEmptyClearEffect::OutputOwnerStored);
    clear.sentinelFreeArgument = storage_->map.head;
    record(0x1412f08ac, FeatureMapEmptyClearEffect::SentinelFreeArgumentLoaded);
    clear.prefixReturned = true;
    report.stage = FeatureMapProxyStage::EmptyMapClearReturned;
    report.stop = clear.stop = FeatureMapProxyStop::SentinelFreeUnavailable;
    report.nextSourceSite = clear.nextSourceSite = "0x1412f08b0:sentinel free unavailable";
    return report;
}

FeatureMapProxyReport FeatureMapProxyCall::resumeSentinelFreePrefix() noexcept
{
    const auto runtime = storage_->runtime.lock();
    if (!runtime) {
        Locked lock(storage_->lock);
        auto report = storage_->progress;
        report.sentinelFree.stop = FeatureMapProxyStop::OwnerUnavailable;
        return report;
    }
    Locked ownerLock(runtime->storage_->lock);
    Locked callLock(storage_->lock);
    auto& report = storage_->progress;
    auto& free = report.sentinelFree;
    if (free.started) return report; // Before any H validation/read, including after success or zero.
    if (!runtime->matchesRetainedLease(runtime) || !runtime->storage_->progress.candidateSlotPublished ||
        runtime->storage_->progress.stage != FeatureCrtProducerStage::CompatibilityCallReturned) {
        free.stop = FeatureMapProxyStop::ProducerUnavailable;
        return report;
    }
    if (report.stage != FeatureMapProxyStage::EmptyMapClearReturned ||
        !report.emptyClear.started || !report.emptyClear.prefixReturned ||
        !storage_->headAllocation || storage_->map.head != storage_->headAllocation ||
        storage_->map.head != report.emptyClear.sentinelFreeArgument ||
        storage_->map.count != 0 || !storage_->map.proxy ||
        storage_->map.proxy != report.candidateProxyIdentity || storage_->map.proxy->map != &storage_->map) {
        free.stop = FeatureMapProxyStop::SentinelFreePrerequisiteUnavailable;
        return report;
    }
    // Host provenance gate; never substitute an older heap for the source's
    // current slot or turn an unqualified owner into a native free occurrence.
    const HANDLE heap = runtime->storage_->candidateHeapSlot;
    if (!heap || reinterpret_cast<std::uintptr_t>(heap) != report.sentinel.heapAllocHeapArgument) {
        free.stop = FeatureMapProxyStop::SentinelFreeOwnerUnavailable;
        return report;
    }
    void* const head = storage_->map.head; // Actual still-live H; no sentinel-field reads are needed.
    free.started = true;
    free.mapIdentity = &storage_->map;
    free.sourcePointerNonNull = head != nullptr;
    recordSentinelFreeEffect(free, 0x141471b7c, FeatureMapSentinelFreeEffect::PointerNonNullTested);
    // This new free occurrence observes the current canonical candidate slot.
    const HANDLE currentHeap = runtime->storage_->candidateHeapSlot;
    free.candidateSlotReadIdentity = reinterpret_cast<std::uintptr_t>(currentHeap);
    ++free.candidateSlotReads;
    recordSentinelFreeEffect(free, 0x141471b89, FeatureMapSentinelFreeEffect::CandidateHeapSlotRead);
    free.heapFreeHeapArgument = reinterpret_cast<std::uintptr_t>(currentHeap);
    free.heapFreeFlagsArgument = 0;
    free.heapFreePointerArgument = reinterpret_cast<std::uintptr_t>(head);
    ++free.heapFreeCalls; // Consume the occurrence before entering the kernel.
    recordSentinelFreeEffect(free, 0x141471b92, FeatureMapSentinelFreeEffect::HeapFreeEntered);
    const BOOL result = HeapFree(currentHeap, 0, head); //1471b92; no host CRT/free surrogate.
    recordSentinelFreeResult(free, result);
    if (result == 0) {
        report.stage = FeatureMapProxyStage::SentinelFreeFailurePending;
        report.stop = free.stop;
        report.nextSourceSite = free.nextSourceSite;
        return report; // Before1471b9c, not a source-free return or native abort.
    }
    free.sourceFreeReturned = true; //1471b9a ->1471bb3/1471bb8; no error-path calls.
    recordSentinelFreeEffect(free, 0x141471bb8, FeatureMapSentinelFreeEffect::SourceFreeReturned);
    free.sentinelRetired = true;
    storage_->headAllocation = nullptr; // Host retirement metadata only. Never read H after HeapFree.
    storage_->map.head = nullptr; //12f08b8, only after successful source-free return.
    free.headNullStored = true;
    free.observedMapHead = storage_->map.head;
    recordSentinelFreeEffect(free, 0x1412f08b8, FeatureMapSentinelFreeEffect::MapHeadNullStored);
    storage_->map.count = 0; //12f08bc follows head-null, even though count was already zero.
    free.countAfterStore = storage_->map.count;
    recordSentinelFreeEffect(free, 0x1412f08bc, FeatureMapSentinelFreeEffect::MapCountZeroStored);
    free.mapInnerDestructorReturned = true;
    recordSentinelFreeEffect(free, 0x1412f08c5, FeatureMapSentinelFreeEffect::MapInnerDestructorReturned);
    free.proxyFreeArgument = storage_->map.proxy; //12f0d0c, A remains allocated and its backpointer unchanged.
    recordSentinelFreeEffect(free, 0x1412f0d0c, FeatureMapSentinelFreeEffect::ProxyFreeArgumentLoaded);
    report.stage = FeatureMapProxyStage::SentinelFreeReturned;
    report.stop = free.stop = FeatureMapProxyStop::ProxyFreeUnavailable;
    report.nextSourceSite = free.nextSourceSite = "0x1412f0d14:isolated map proxy free unavailable";
    return report;
}

FeatureMapProxyReport FeatureMapProxyCall::resumeProxyFreePrefix() noexcept
{
    const auto runtime = storage_->runtime.lock();
    if (!runtime) {
        Locked lock(storage_->lock);
        auto report = storage_->progress;
        report.proxyFree.stop = FeatureMapProxyStop::OwnerUnavailable;
        return report;
    }
    Locked ownerLock(runtime->storage_->lock);
    Locked callLock(storage_->lock);
    auto& report = storage_->progress;
    auto& free = report.proxyFree;
    if (free.started) return report; // Before all A/H/M-source validation, also after failed free.
    if (!runtime->matchesRetainedLease(runtime) || !runtime->storage_->progress.candidateSlotPublished ||
        runtime->storage_->progress.stage != FeatureCrtProducerStage::CompatibilityCallReturned) {
        free.stop = FeatureMapProxyStop::ProducerUnavailable;
        return report;
    }
    if (report.stage != FeatureMapProxyStage::SentinelFreeReturned ||
        !report.sentinelFree.sourceFreeReturned || !report.sentinelFree.sentinelRetired ||
        !report.sentinelFree.mapInnerDestructorReturned || storage_->headAllocation ||
        storage_->map.head || storage_->map.count != 0 || !storage_->map.proxy ||
        storage_->map.proxy != report.sentinelFree.proxyFreeArgument ||
        storage_->map.proxy != report.candidateProxyIdentity || storage_->map.proxy->map != &storage_->map) {
        free.stop = FeatureMapProxyStop::ProxyFreePrerequisiteUnavailable;
        return report;
    }
    const HANDLE heap = runtime->storage_->candidateHeapSlot;
    if (!heap || reinterpret_cast<std::uintptr_t>(heap) != report.heapAllocHeapArgument) {
        free.stop = FeatureMapProxyStop::ProxyFreeOwnerUnavailable;
        return report;
    }
    auto* const proxy = storage_->map.proxy; // Same still-live A loaded at12f0d0c by the prior phase.
    free.started = true;
    free.mapIdentityToken = reinterpret_cast<std::uintptr_t>(&storage_->map);
    free.mapProxyTokenBefore = reinterpret_cast<std::uintptr_t>(proxy);
    free.backpointerTokenBefore = reinterpret_cast<std::uintptr_t>(proxy->map); // Before retirement only.
    free.sourcePointerNonNull = proxy != nullptr;
    recordProxyFreeEffect(free, 0x141471b7c, FeatureMapProxyFreeEffect::PointerNonNullTested);
    const HANDLE currentHeap = runtime->storage_->candidateHeapSlot; // New source free occurrence.
    free.candidateSlotReadIdentity = reinterpret_cast<std::uintptr_t>(currentHeap);
    ++free.candidateSlotReads;
    recordProxyFreeEffect(free, 0x141471b89, FeatureMapProxyFreeEffect::CandidateHeapSlotRead);
    free.heapFreeHeapArgument = reinterpret_cast<std::uintptr_t>(currentHeap);
    free.heapFreeFlagsArgument = 0;
    free.heapFreePointerArgument = reinterpret_cast<std::uintptr_t>(proxy);
    ++free.heapFreeCalls;
    recordProxyFreeEffect(free, 0x141471b92, FeatureMapProxyFreeEffect::HeapFreeEntered);
    const BOOL result = HeapFree(currentHeap, 0, proxy);
    recordProxyFreeResult(free, result);
    if (result == 0) {
        report.stage = FeatureMapProxyStage::ProxyFreeFailurePending;
        report.stop = free.stop;
        report.nextSourceSite = free.nextSourceSite;
        return report; // Before errno provider; neither retirement nor destructor return is certified.
    }
    free.sourceFreeReturned = true;
    free.proxyRetired = true; // Host lifetime metadata; native M.proxy deliberately stays stale.
    free.candidateMapDestructorReturned = true;
    //12f0d14 was a tail jump: the single CRT RET also returns12f0cf0. No M/A/H
    // field read/write occurs after HeapFree; all remaining values are metadata.
    recordProxyFreeEffect(free, 0x141471bb8, FeatureMapProxyFreeEffect::TailDestructorReturned);
    report.stage = FeatureMapProxyStage::MapDestructorReturned;
    report.stop = free.stop = FeatureMapProxyStop::EnclosingLifecycleUnavailable;
    report.nextSourceSite = free.nextSourceSite = "0x141471bb8:isolated map destructor returned; enclosing lifecycle unavailable";
    return report;
}

FeatureCrtProducerReport FeatureCrtRuntime::attemptProducer() noexcept
{
    Locked lock(storage_->lock);
    auto& report = storage_->progress;
    if (!report.modulePinned || report.stage != FeatureCrtProducerStage::NotAttempted)
        return report;
    ++report.heapCreateCalls;
    const HANDLE heap = HeapCreate(0, 4096, 0); // Source 141484812.
    recordCreateReturn(report);
    storage_->candidateHeapSlot = heap; // Candidate equivalent of source141484818; original EXE slot is untouched.
    recordCandidateSlotPublication(report, heap != nullptr);
    if (!heap) {
        recordNullProducerReturn(report);
        return report; // Guard before caller141470cac's unqualified fatal-mode read.
    }
    static_assert(static_cast<int>(HeapCompatibilityInformation) == 0);
    static_assert(sizeof(DWORD) == 4);
    DWORD compatibility = 2;
    ++report.compatibilityCalls;
    const BOOL result = HeapSetInformation(heap, HeapCompatibilityInformation, &compatibility, 4);
    recordCompatibilityReturn(report, result);
    return report; // No later startup, proxy allocation or inferred native cleanup.
}

FeatureCrtEncodedHandlerReport FeatureCrtRuntime::resumeEncodedNewHandlerPrefix(
    const std::shared_ptr<FeatureCrtRuntime>& lease) noexcept
{
    Locked lock(storage_->lock);
    if (!matchesRetainedLease(lease)) {
        auto refused = storage_->encodedHandler;
        refused.stop = FeatureCrtEncodedHandlerStop::OwnerMismatch;
        return refused; // Never replace the canonical receipt on an alien lease.
    }
    auto& report = storage_->encodedHandler;
    if (report.encodePointerCalls) return report; // One occurrence, including a numeric-zero encoding.
    if (!storage_->progress.modulePinned || !storage_->progress.candidateSlotPublished ||
        storage_->progress.stage != FeatureCrtProducerStage::CompatibilityCallReturned ||
        !storage_->candidateHeapSlot) {
        report.stop = FeatureCrtEncodedHandlerStop::ProducerUnavailable;
        return report;
    }
    if (storage_->retainedMapCall) {
        report.stop = FeatureCrtEncodedHandlerStop::ChildFrameAlreadyPrepared;
        return report; // Source startup cannot be inserted after a child lifetime.
    }
    const auto record = [&](std::uint64_t site, FeatureCrtEncodedHandlerEffect effect) noexcept {
        report.effects[report.effectCount++] = {site, effect};
    };
    report.encodePointerArgument = 0;
    ++report.encodePointerCalls; // Consume before the actual kernel encoding; never replay.
    record(0x1414813e2, FeatureCrtEncodedHandlerEffect::EncodePointerEntered);
    void* const encodedNull = EncodePointer(nullptr); //14813e0 input ECX0; actual candidate process result.
    report.encodePointerReturned = true;
    report.encodedNullResult = reinterpret_cast<std::uintptr_t>(encodedNull);
    record(0x14146e9ff, FeatureCrtEncodedHandlerEffect::EncodePointerReturned);
    storage_->savedEncodedNullArgument = report.encodedNullResult; //146ea02 saves the actual result in RBX.
    storage_->savedEncodedNullArgumentKnown = true;
    storage_->candidateEncodedNewHandlerSlot = encodedNull; //1481ce0 corresponding candidate role, not host CRT.
    ++report.candidateRoleStores;
    report.candidateRoleKnown = true;
    report.observedCandidateRole = reinterpret_cast<std::uintptr_t>(storage_->candidateEncodedNewHandlerSlot);
    record(0x141481ce0, FeatureCrtEncodedHandlerEffect::CandidateRoleStored);
    report.firstSetterReturned = true;
    record(0x141481ce7, FeatureCrtEncodedHandlerEffect::FirstSetterReturned);
    report.stop = FeatureCrtEncodedHandlerStop::NextInitializerUnavailable;
    report.nextSourceSite = "0x14146ea0d:0x141481ca8 next encoded-role initializer unavailable";
    return report;
}

FeatureCrtEncodedHandlerReport FeatureCrtRuntime::encodedNewHandlerReport() const noexcept
{
    Locked lock(storage_->lock);
    return storage_->encodedHandler;
}

FeatureCrtNullRoleTailReport FeatureCrtRuntime::resumeEncodedNullRoleTail(
    const std::shared_ptr<FeatureCrtRuntime>& lease) noexcept
{
    Locked lock(storage_->lock);
    if (!matchesRetainedLease(lease)) {
        auto refused = storage_->nullRoleTail;
        refused.stop = FeatureCrtNullRoleTailStop::OwnerMismatch;
        return refused;
    }
    auto& report = storage_->nullRoleTail;
    if (report.started) return report;
    if (!storage_->progress.modulePinned || !storage_->progress.candidateSlotPublished ||
        storage_->progress.stage != FeatureCrtProducerStage::CompatibilityCallReturned ||
        !storage_->candidateHeapSlot) {
        report.stop = FeatureCrtNullRoleTailStop::ProducerUnavailable;
        return report;
    }
    if (storage_->retainedMapCall) {
        report.stop = FeatureCrtNullRoleTailStop::ChildFrameAlreadyPrepared;
        return report; // Never insert or reset source startup after child preparation.
    }
    const auto& first = storage_->encodedHandler;
    if (!first.encodePointerReturned || !first.candidateRoleKnown || !first.firstSetterReturned ||
        first.encodePointerCalls != 1 || first.candidateRoleStores != 1) {
        report.stop = FeatureCrtNullRoleTailStop::FirstPrefixUnavailable;
        return report; // No implicit EncodePointer or first-role execution.
    }
    if (!storage_->savedEncodedNullArgumentKnown ||
        storage_->savedEncodedNullArgument != first.encodedNullResult ||
        reinterpret_cast<std::uintptr_t>(storage_->candidateEncodedNewHandlerSlot) != storage_->savedEncodedNullArgument) {
        report.stop = FeatureCrtNullRoleTailStop::SourceArgumentUnavailable;
        return report;
    }
    report.started = true;
    report.savedArgumentKnown = true;
    const auto argument = storage_->savedEncodedNullArgument; // Preserved actual source-frame argument, even if zero.
    report.savedArgumentToken = argument;
    const auto store = [&](std::size_t index, std::uint64_t site, std::uint64_t role) noexcept {
        storage_->candidateEncodedNullRoles[index] = argument;
        report.roleKnown[index] = true;
        report.observedRoles[index] = storage_->candidateEncodedNullRoles[index];
        ++report.roleStores;
        report.effects[report.effectCount++] = {site, FeatureCrtNullRoleTailEffect::CandidateRoleStored,
                                               role, report.observedRoles[index], true};
    };
    const auto returned = [&](std::uint64_t site) noexcept {
        ++report.leafReturns;
        report.effects[report.effectCount++] = {site, FeatureCrtNullRoleTailEffect::LeafReturned, 0, 0, false};
    };
    store(0, 0x141481ca8, 0x1447006d8); returned(0x141481caf);
    store(1, 0x141470438, 0x1446ffb40); returned(0x14147043f);
    store(2, 0x141481ca0, 0x1447006d0); returned(0x141481ca7);
    store(3, 0x141481ad4, 0x1447006c0); returned(0x141481adb);
    store(4, 0x141474954, 0x1446ffb68);
    store(5, 0x14147495b, 0x1446ffb70);
    store(6, 0x141474962, 0x1446ffb78);
    store(7, 0x141474969, 0x1446ffb80); returned(0x141474970);
    report.noOpReturned = true; //146ff78 is RET0, not another role store.
    ++report.leafReturns;
    report.effects[report.effectCount++] = {0x14146ff78, FeatureCrtNullRoleTailEffect::NoOpReturned, 0, 0, false};
    report.prefixReturned = true;
    report.stop = FeatureCrtNullRoleTailStop::NonNullCallbackInitializerUnavailable;
    report.nextSourceSite = "0x14146ea3d:0x14147e75c nonnull callback initializer unavailable";
    return report;
}

FeatureCrtNullRoleTailReport FeatureCrtRuntime::encodedNullRoleTailReport() const noexcept
{
    Locked lock(storage_->lock);
    return storage_->nullRoleTail;
}

FeatureCrtProducerReport FeatureCrtRuntime::report() const noexcept
{
    Locked lock(storage_->lock);
    return storage_->progress;
}
} // namespace m2m::detail
