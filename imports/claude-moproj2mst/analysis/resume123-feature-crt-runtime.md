# CP123: isolate the CRT heap producer before binding any Project

An isolated private runtime can execute the source-closed HeapCreate producer and retain its actual result without modifying model.hpp or pretending that Project construction has resumed. The proposed next increment is three new private source/test files only. It must remain disconnected from default conversion and feature-proxy allocation. Public startup integration still needs a module-scoped owner and a first-constructor-entry binding contract; Session ownership or a CLI-only hook does not supply those facts.

## Current actual startup and conversion seams

The source has two deployment forms, with an important link distinction:

| Current source | Actual scope and ordering |
| --- | --- |
|CMakeLists.txt:90..105|m2m_core is static. The six-export DLL compiles src/dll/abi.cpp and links that core. No application DllMain or source-profile CRT heap startup exists.|
|CMakeLists.txt:112..124|The CLI also compiles src/dll/abi.cpp directly and statically links m2m_cli_private/m2m_core. Ordinary CLI conversion does not call through an imported converter DLL. The separate DLL remains a separate module instance.|
|src/cli/main.cpp:32..38|wmain sets error mode, then routes the isolated worker before ordinary argument parsing. A startup hook inserted later in normal CLI parsing would miss that worker. A hook only in wmain would also miss DLL clients and direct core C++ calls.|
|src/dll/abi.cpp:146 onward|Wide/UTF-8 conversion exports translate/snapshot input and call m2m::convert. Settings setters have module-local adapter globals. No source-profile heap owner is created or borrowed.|
|src/core/convert.cpp:154..199|All convert/convertDetailed/private-service/bytes routes eventually enter run. A static mutex serializes that core instance. Per-call Ctx is created, arguments/settings are checked, then a per-call ConversionSession is constructed.|
|src/core/convert.cpp:205..208|The prepared Mo precedes initProject, and backlink commit follows it. This is the future first-phase binding site, not a process-owner birth.|
|src/core/conversion_call_services.hpp|Services are borrowed for one synchronous conversion only. They intentionally do not enter Session/Project/C ABI. This is not a retention owner.|
|src/cli/isolated_save_worker.cpp:110 onward|The owned worker validates its protocol/job, restores CWD, then directly calls convertDetailedWithServices. Its terminal service is stack-owned. No source-profile CRT heap exists here either.|

The Session is destroyed at each run exit; its Project can be released earlier. Ctx, stack services, Project's weak epoch and Mo slots are therefore unsuitable as the CRT heap's strong lifetime. A source-role module owner must span those calls. Static core copies in different executable/DLL images must not be silently treated as the same runtime because their C++ type name matches. The current engine mutex is likewise per linked core instance, not a cross-module process lock.

No HeapCreate, HeapAlloc, HeapFree, HeapDestroy, GetProcessHeap or explicit DllMain occurs under current src/. CP122 added only passive typed storage. This negative source search does not claim anything about the host compiler's actual modern CRT internals; those internals are not the original static-CRT slot144700810.

## Exact original producer and failure boundary

Retained original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Producer141484800 is called by startup141470c28 at141470ca3. Its incoming ecx=1 is saved, but HeapCreate receives separately prepared flags0, initial4096 and maximum0.

The exact source sequence is:

1. 141484812 calls HeapCreate(0,0x1000,0).
2. 141484818 stores the actual returned value into144700810, **including null**, before testing it at14148481f.
3. Null branches at141484822 directly to the epilogue141484847 and returns0. There is no HeapSetInformation, fallback heap or retry inside this producer.
4. Nonnull prepares class0, a DWORD containing2 and byte count4;14148483c calls HeapSetInformation. Its Boolean result is ignored.141484842 sets eax=1 and returns. False compatibility result must not revoke the stored nonnull heap or change the producer return to0.
5. Caller141470ca8 tests the producer result. Success jumps to141470cce and executes later startup helper141481708. This helper and the remaining CRT/application startup are outside the proposed producer.
6. Failure first reads runtime DWORD1446ffb58 at141470cac and compares with2. The unequal route calls14147e9a0; both visible branches reach14147e778(0x1c), then14146e6e0(0xff). The mode's current value, diagnostics and termination semantics are not supplied by this bounded producer. Stop before that read. Do not map it to a guessed process exit or host exception.

The producer itself therefore has a source-closed successful or null return. Its caller's full startup failure is not source-closed here. Reports must distinguish ProducerReturned0 from CompletedFatalStartup, and ProducerReturned1 from CompletedCRTStartup.

Subsequent allocation14147489c reads this slot and uses HeapAlloc(handle,0,size). No GetProcessHeap substitution is source-backed. Its null return later needs new-mode/handler/errno state, independently of the producer's failure. CP119's direct HeapDestroy observations concern global144701508 and an object+8, not this slot. CRT heap teardown remains unqualified.

## Minimal isolated runtime proposal

Reserve exactly these new files:

* src/model/feature_crt_runtime.hpp
* src/model/feature_crt_runtime.cpp
* tests/model/test_feature_crt_runtime.cpp

The existing source/test globs discover them after root configure. No production callsite, model.hpp, Project/Session field, passive epoch code, C ABI, CLI flag, CMake list or p11 source edit is needed. Crucially project_owned_storage.cpp must not reference the new runtime cpp in this increment: standalone packet targets compile that existing cpp directly and would otherwise need a new link dependency.

Proposed model-private API:

```cpp
namespace m2m::detail {
enum class FeatureCrtProducerStage {
    NotAttempted, HeapCreateReturnedNull, CompatibilityCallReturned
};
enum class FeatureCrtProducerStop {
    NotStarted, StartupFatalModeUnavailable, LaterStartupUnavailable,
    ModuleScopeUnavailable
};
struct FeatureCrtProducerReport; // immutable read-only receipt, no allocator authority
class FeatureCrtRuntime final {
public:
    // Prepare actual current-process/current-code-module identity and all host
    // receipt storage first. Does not call the source HeapCreate producer.
    static std::shared_ptr<FeatureCrtRuntime> prepareCurrentModule();
    FeatureCrtProducerReport attemptProducer() noexcept;
    FeatureCrtProducerReport report() const noexcept;
    FeatureCrtRuntime(const FeatureCrtRuntime&) = delete;
    FeatureCrtRuntime& operator=(const FeatureCrtRuntime&) = delete;
private:
    FeatureCrtRuntime(); // no caller-provided HANDLE, result or Project identity
};
}
```

This is a proposal, not added code. The actual implementation should hide HANDLE and kernel calls in the cpp. The factory identifies the current code's containing module, not blindly GetModuleHandle(nullptr) for every linked DLL case. If it uses GetModuleHandleEx FROM_ADDRESS/PIN to retain that actual module for the owned-process experiment, record that as host lifetime instrumentation before source execution; it is not an original CRT call. A pin/process-retention policy needs root's explicit selection, not an inferred native unload behavior.

The report should contain stage/stop, actual owner identity/process/module generation, source profile, whether the source slot has been observed, actual nullness, ordered effect counts/sequence, optional compatibility BOOL and original producer return. Before HeapCreate returns, slot knownness is unavailable; neither a value-initialized C++ handle nor PE initial bytes is a live read receipt. Compatibility BOOL is diagnostic only. Flags must separately say full CRT startup, Project binding, proxy allocation, native teardown and public Close are unadmitted. Public report data must not be convertible into a qualified runtime by filling a struct or supplying an arbitrary handle.

attemptProducer executes the real imported kernel calls against this owner, records the actual returned handle and publication before the branch, and stops at the corresponding caller boundary. It must not call malloc/new for metadata after HeapCreate succeeds; prepare storage/lease capacity first. It must not run unknown diagnostics on null, call current host new-handler, request zero-fill, allocate8 or initialize map/feature fields. Subsequent attempts return the same receipt without another HeapCreate. This is one represented startup occurrence with an idempotent diagnostic retry, not a claim that the original producer universally ignores second invocations.

## Explicit retention policy for the isolated experiment

A successful HeapCreate creates a resource even before any proxy block exists. Dropping the wrapper at Project scope or adding HeapDestroy to its default destructor would silently choose an unproved CRT teardown. The smallest coherent experiment uses one module/process retention record, prepared before the source producer and held until the owned test process ends. Inspection leases can be released; the record/heap stay retained. Its host process/module pinning and final OS reclamation are labeled candidate instrumentation, not native CRT destruction or Close.

This is a typed source-role heap record, not a registry of raw Project addresses. It owns no Project, anchor or storage epoch. Avoid runtime->record->runtime shared-pointer cycles: the explicit retained process record owns its state, and borrowed observation leases do not become reciprocal owners. Do not perform HeapDestroy from static destruction or DLL detach. No public default call may create these records; the new focused executable deliberately creates one bounded runtime in its own process. This policy is sufficient for that isolated producer test, not authorization for accumulating unresolved proxy allocations in a reusable library host.

If root does not select process-ending retention, an explicit host-only empty-heap retirement policy must be designed separately and labeled as adapter cleanup; its HeapDestroy would not be an original teardown reconstruction. Without either policy, actual HeapCreate should remain unexecuted and the next increment should only prepare passive owner state. The current report does not infer native heap retirement from the availability of a Windows cleanup function.

## Focused tests and qualification

One ordinary Testkit executable can own a single real process/module producer attempt. It should verify actual HeapCreate success, exactly one source-slot publication, actual compatibility-call occurrence and actual producer return1, then verify repeated factory/attempt observations refer to that same runtime without replay. Its lifetime test drops local observation leases while the process retention owner remains and obtains the same identity again. No Project is constructed or bound; no proxy block exists to clean up. At process exit, only process reclamation is established.

For deterministic null-return and ignored-false cases, test a separate pure progress/receipt fold with **declared model events**, visibly labeled. That fold must have no path to construct a FeatureCrtRuntime or qualified live handle. Verify null is published before failure stop, compatibility is absent for null, compatibility false still yields producer1 for nonnull, incomplete/misordered observations refuse, and no later startup/fatal work is reported. A fabricated nonnull model word is not a real heap owner. Actual kernel failure is not forced or claimed by that model test; actual ignored-false kernel coverage exists only if Windows returns false in the real attempt.

Keep the no-Project contract mechanically testable: new runtime header/cpp need not include model.hpp, project_feature_storage.hpp or conversion_session.hpp. The test must not bind a synthetic Project/context, alter default call services or widen the C ABI. Existing opaque header/p11/epoch tests remain normal regression gates, but new runtime objects are linked through core only when referenced, rather than becoming an unconditional dependency of the storage cpp. Root owns all builds/tests; no actual HeapCreate or original process was run in this lane.

## Remaining prerequisites for public or proxy integration

The isolated producer closes one source function; it does not close these next edges:

* Select and instantiate a real module-lifetime runtime in both CLI and DLL/direct-core entry architectures, with a defined unload/process ending policy. A function-local lazy producer at first Project construction changes original startup timing and cannot be called startup parity.
* Bind the qualified runtime to the same Project epoch before first089917. CP122's passive receipt after completed Open is not a suspended constructor. Preserve default supported behavior while this binding is unavailable.
* Before any HeapAlloc8, provide a typed unresolved-block retention owner and a coherent report stage. Heap lifetime alone does not record ownership of a successful proxy or justify native free on adapter release.
* Keep null HeapCreate caller failure at1446ffb58, failed HeapAlloc's new-mode/handler/errno, sentinel construction/EH, provider state and native Close guarded until separately ported.

Recommended root action is the isolated three-file producer/retention experiment after approving the bounded process-retention qualification. No Project binding or shared model header change is needed. If the intended task instead requires immediate default Open admission, the missing startup/retirement and first-phase producer remain exact blockers.

## Read-time evidence receipt

Current source hashes: CLI main `9e62898ae67a8f91a15acc65957cbbde09b4a01fa60b9e5d373752d4237aba5b`; DLL ABI `6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848`; core convert `71fe67b9c6be7bde83001e663aa7c50f01c4a6421f357c0db893a5df0a0bd632`; private services `4b0c5f823618a3804dd6f470ee7e263d6aabe5e2519b38605c7162a36a2454bd`; isolated worker `19d3191d1039772fea9e75e49a5042be5781e076cba3714a4ab5bfa47dfb182f`; CMake `582ebc7f8e015a9a2cf8834d3e28f1b650073288bf8712093b8bffea694368b2`; CP122 storage header `4890457bd665909fca8f6059bd09093b8b5bfab63aff12f3b4f9d2bb72f53ec0`; storage cpp `af2e1da0fe4bdc582e29731b2f0e6ce7d7f0a882ea2ade477f164807da1199e8`.

Directly reread retained producer ASM has SHA `a9efa00fe5209dffb9572fbeb0b604120b79a7a42d410255a199d00d0ca098bd`; startup caller ASM `dce84f25fc57bf7a407e23126fb051e9bd1f6b59dc24aa5bb0a598682836c481`. CP119 inputs JSON SHA remains `ff1ea36ace289559ebca9ef69c0f0811f7a0626f8ed6efd8c6216b3aa008cf51`; CP122 proxy proposal `17d0b0f43773c84bd73b418d5cec3195af704b0f985d87cc10695066bc9b2995` contains the separate allocation/failure/destructor qualifications. These are retained static views, not live heap observations.

Only this unique report was written. No source/header/test/CMake, GUI, original launch, live heap, build/test or mutable REA operation occurred.
