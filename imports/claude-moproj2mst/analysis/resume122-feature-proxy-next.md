# CP122: keep the first proxy allocation attached to its real epoch

The successful 8-byte native prefix is closed by retained source evidence, and CP121 now permits its private state/report definitions to evolve without changing model.hpp. It is not yet a closed public integration: the candidate has no source-profile CRT heap producer, no retirement policy for an allocation parked before sentinel construction, and no suspended native constructor frame behind the existing passive receipt. The next immediately implementable increment is passive typed epoch storage. Actual allocation should follow only after the runtime retention and entry-timing contracts below are selected. This report proposes exact private seams; it implements none of them.

## Evidence and exact effect boundary

Retained original DEF_GUI_MO_64 v11.0.2 has SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. CP119 closes the positive allocation path; CP114/116 close its pointer stores and distinguish constructor unwind from normal context destruction.

| Original source occurrence | Required effect |
| --- | --- |
|141484800 through 14148484b, called from CRT startup141470ca3|HeapCreate(0,4096,0); store its actual result in runtime slot144700810, including null; on nonnull call HeapSetInformation(handle,class0,pDWORD2,4), ignore its Boolean result and return1. Null startup continuation is unported.|
|12f107b, operator-new14146eabc to malloc14147489c|For positive size8, read that same actual runtime heap owner and call HeapAlloc(handle,0,8) at1414748f1. Actual nonnull return skips all new-mode, handler and errno paths.|
|12f1085|Write the actual map identity M into the returned block A: `[A]=M`.|
|12f108c|Publish the same returned pointer into M+0. Neither publication precedes allocation return.|
|12f108f onward|Map-constructor EH state0 becomes active before the separate sentinel helper12edac0. Stop before its 48-byte request. M+30/head, M+38/count, sentinel links and context feature fields remain uninitialized/unrepresented.|

HeapAlloc returning null stops before the next read at1414748ff of live new-mode144700858. It must not mean source operator-new returned null, completed failure, a host bad_alloc, or permission to try again. Native continuation can consult the decoded live new-handler more than once and write the native CRT errno slot. Those inputs remain unavailable. A host callback returning success or a nonnull arbitrary pointer is not an actual allocation receipt.

The isolated free path14dfc68 to141471b7c requires the same source heap and calls HeapFree(handle,0,pointer). Failure enters GetLastError/native errno mapping. A guard before sentinel allocation does not throw and does not invoke the constructor-EH proxy free. Neither per-Project HeapDestroy nor automatic free at the adapter epoch boundary follows from the source. CP119's two direct HeapDestroy observations belong to other owners.

## Actual candidate ownership and timing

The following live source boundaries survived CP121 unchanged:

* project_owned_storage.cpp:118 owns slotBorrow_ as shared_ptr<const ProjectStorageLifetime>; this is the Project storage epoch. Anchor and Mo slots borrow it weakly. A retained Mo/slot capsule or escaped anchor must not extend it.
* The Project constructor body binds the actual Project/slots/epoch before creating passive anchor metadata. No CRT heap exists there.
* project.cpp:38 calls beginFeatureContextConstructorAttempt after tokMode and before f10/path and other adapter initialization. Current actual Open prepares the Mo first in convert.cpp:206, calls initProject at207, then commits the Mo backlink at208.
* Project destructor entry at project_owned_storage.cpp:128 invalidates sourceOwnerAlive before ordinary member teardown; slotBorrow_ expires later in member order. Session releaseProjectStorage ends its thread-local scope, destroys Project, then clears the retained Mo backlink at conversion_session.cpp:429 onward. This is adapter cleanup, not native accepted Close.

A bounded search of current src/ and include/ finds no HeapCreate/HeapAlloc/HeapFree/HeapSetInformation/GetProcessHeap call or existing CRT-heap owner. CP121 did not add one. An original absolute slot address is source-role evidence, not a live handle in the candidate process. A faithful candidate binding would execute/bind its own reconstructed process/module CRT producer; it must not read that original absolute VA in the candidate or demand the original GUI process as its allocator.

The existing anchor records a historical pending occurrence while legacy supported initialization continues. It is not a lease on an interrupted native call stack. Therefore a new method must not allocate retrospectively after successful Open merely because that passive receipt still says ProxyAllocationPending. Actual prefix execution requires a prebound runtime and execution at the first logical089917 bridge, or a separately implemented suspended constructor barrier. Re-running initProject to reach a later native phase would replay earlier adapter effects and is not an acceptable resume mechanism.

## Private owner design with no model.hpp edit

Retain the exact Project members and existing marker type. In project_owned_storage.cpp replace only the epoch construction expression with a local private factory. A private bundle contains the empty marker, stable logical map storage and a typed feature cursor. Return a shared pointer to the marker whose custom deleter owns the bundle. Use typed get_deleter plus exact marker-address validation to recover that bundle from the existing epoch; never use container-of, inheritance from the final marker, a raw-Project global registry, or an untyped offset cast.

Proposed private types, declared in NEW src/model/project_feature_storage.hpp, are:

```cpp
namespace m2m::detail {
class FeatureCrtHeapRuntime; // real reconstructed module runtime; never a HANDLE wrapper supplied by a caller
struct FeatureEpochBundle;  // marker + stable map identity + typed cursor
struct FeatureEpochDelete {
    FeatureEpochBundle* bundle;
    void operator()(const engine::ProjectStorageLifetime*) const noexcept; // host bundle only
};
std::shared_ptr<const engine::ProjectStorageLifetime> makeFeatureEpoch();
FeatureEpochBundle* featureEpoch(
    const std::shared_ptr<const engine::ProjectStorageLifetime>&) noexcept;
enum class FeatureRuntimeBind { Bound, SameOwner, OwnerUnavailable, Conflict, EpochUnavailable, PhasePassed };
enum class FeatureProxyStop {
    PhaseNotEntered, HeapOwnerUnavailable, OwnerChanged, EpochUnavailable,
    AllocationFailureInputsUnavailable, SentinelAllocationUnavailable,
    RetirementUnavailable
};
struct FeatureProxyReport; // immutable value receipt; no owning raw block pointer
}
```

These are proposed names/signatures, not added APIs. Factory/recovery definitions stay in the existing storage cpp so all ten explicit storage object edges and p11's textual include remain link-complete without a new cpp/CMake dependency. The header must use literal namespace m2m (or m2m::detail), preserving p11's macro rename. Its definition must not be included from model.hpp.

The factory creates passive host storage only. Its deleter owns exactly once on shared-control-block construction failure; do not combine an independently armed unique_ptr with the shared_ptr constructor's failure deleter. The bundle must not strongly retain its own marker/Project/anchor/slots. The weak control-block epoch remains the identity key, including at an identical reused Project address. Borrowing the epoch for one synchronous internal operation does not admit callbacks or asynchronous resume; preserve the existing serialized-call assumption and source-scope invalidation check.

The map needs its own stable actual candidate storage. Using `Project*+60+1458`, the address of the anchor, or a numeric original M as the proxy backpointer would fabricate layout/identity. A private MapHeaderStorage can represent the known M+0/+30/+38 slots with compile-checked x64 offsets and explicit knownness; its unknown fields must not be read or value-initialized as claimed native state. `[A]=&bundle.map` then `bundle.map.proxy=A` are the two actual candidate pointer stores corresponding to the source occurrences. The typed 8-byte ProxyNode contains only that map pointer; compile-check sizeof(pointer)==8 and sizeof(ProxyNode)==8. Host bundle/shell allocation is adapter storage, not source CRT allocation.

## Conditional heap binding and execution seam

No public Project method or member is required. The private anchor definition can friend a model-private FeatureContextAccess bridge and provide these private-header free functions:

```cpp
namespace m2m::detail {
FeatureRuntimeBind bindFeatureRuntime(
    const Project&, const std::shared_ptr<FeatureCrtHeapRuntime>&) noexcept;
FeatureProxyReport featureProxyReport(const Project&) noexcept;
}
```

The observer gets the actual anchor through Project::featureContextAnchor and validates its Project identity, weak epoch and prepared Mo/slot/control-block identity. The private friend recovers the bundle through the anchor's real weak epoch. Binding changes only bundle metadata, before the first logical089917 occurrence; unknown to same actual runtime is monotonic, conflicting control blocks/generations or late binding after adapter continuation refuse. It never creates a heap or allocates a proxy. No const_cast of the Project's public const anchor getter is necessary: the weak epoch borrows mutable private bundle state while the anchor remains metadata.

Actual prefix execution is a private helper called only by Project::beginFeatureContextConstructorAttempt at its first captured occurrence, after existing prepared-owner checks and before returning to f10=5. On entry it requires a prebound live runtime, the same actual epoch/model/slots, preallocated host receipt/retention capacity, and a selected retirement contract. There is no exported late-resume function and no default heap. An unbound default Open remains passive, preserving current supported behavior.

The runtime must have an actual producer receipt: process identity, stable module-role/generation, source profile, real HeapCreate result and subsequent compatibility-call occurrence. Its creation scope must span Projects; lazy HeapCreate per Project or GetProcessHeap does not satisfy this dependency. Compatibility false after nonnull HeapCreate is allowed. Arbitrary HANDLE constructors, file-byte-zero inference and caller Boolean success witnesses are excluded. The current candidate startup lacks this producer, so this binding has no production producer until root assigns that integration separately.

Prepare all host metadata/retention capacity before the native kernel call so no host allocation/exception can strand an unrecorded successful block between the two source stores. On successful HeapAlloc, commit a typed allocation receipt, execute the two stores in order and park before sentinel request; retries inspect the receipt and never reallocate/rewrite. On null, retain the attempted call and stop before new-mode read, without retry, publication, host handler call or synthesized unwind. The existing noexcept bridge must not turn a metadata allocation exception into process termination; preparatory host failures stay outside native-effect admission.

## Retirement and report coherence are mandatory gates

The native successful-prefix block cannot be uniquely owned by escaped anchor metadata. Nor may a bundle destructor automatically HeapFree it when adapter Project storage expires: neither event is the native sentinel-construction exception or later map destructor.

A minimal actual-allocation experiment can be coherent only with an explicit **candidate runtime retention policy**: a process/module runtime owns the allocation receipt, keeps the actual heap alive, and retains an orphaned pending block after the Project epoch expires. The record becomes permanently inadmissible for source continuation and records expired provenance. It does not retain Project storage. No native free/Close/HeapDestroy is emitted; OS process reclamation is not reported as source cleanup. This policy is candidate instrumentation/resource retention, not an inferred native semantic. It should be opt-in in an owned bounded worker until real native retirement is ported; enabling it for unbounded public Opens would accumulate allocations. Root has not yet selected or implemented that policy. Without it, the correct guard is before HeapAlloc, not successful allocation followed by a lost pointer or improvised deleter.

A runtime receipt must not keep itself alive by a runtime->record->runtime shared-pointer cycle. Its external process/module owner retains records; records borrow runtime identity. Runtime retirement is explicitly refused while unresolved owned blocks exist, and no early per-Project destructor may destroy its heap. The private implementation needs an actual external owner and a defined process-ending policy, not a function-local shared_ptr that disappears after binding. The startup owner and unresolved CRT heap-destruction boundary are the exact remaining lifetime producers.

CP121's private definitions make honest stage evolution possible: introduce a distinct ProxyPublishedSentinelPending stage and a failure-input pending state, and add matching private stop values. featureContextAttemptReport/stage observers must delegate to that same private cursor; do not leave a successful block reported as awaiting its first allocation. The current static `nativeHeapEffectsCommitted=false` cannot remain an authoritative claim once the optional prefix actually allocates. Replace it with an instance observation of committed source heap effects (and migrate the existing private test); keep nativeConstructorCompleted, feature-values/registration completion and completeForPublicClose false. This needs only the private header/storage cpp/test, not model.hpp. Default passive instances still report zero heap effects.

The raw map object is not constructed after only its proxy store. Thus native map/context destructors are not callable, and accepted Close remains guarded at every stage described here. Adapter Session destruction may continue its current storage release while the explicit retention owner records the unresolved block. Do not conflate that with a publicly completed native Close.

## Smallest implementation order and exact reservations

**Ready now: passive epoch bundle only.** Reserve NEW src/model/project_feature_storage.hpp, src/model/project_owned_storage.cpp and NEW tests/model/test_project_feature_storage_epoch.cpp. If private observer access is needed for the test, additionally reserve src/model/project_feature_context_anchor.hpp for a private friend/access bridge; no new Project methods/fields. Keep all factory definitions in the existing cpp. No Windows allocation, runtime binding, new source stages, public Open behavior, native cleanup or CMake change in this first patch. Existing anchor/opaque tests and p11/standalone link gates apply.

**Conditional second patch: actual producer/prefix.** Reserve the private storage header, private anchor header, existing storage cpp, existing anchor test and NEW tests/model/test_project_feature_proxy_prefix.cpp. A real process/module runtime producer/owner must be assigned first; its exact source owner does not currently exist, so a production file reservation cannot be honestly reduced to this list yet. Do not create a fake one inside Project. Test-only owned-worker runtime creation can establish candidate-only kernel success and retention behavior while leaving public production binding unavailable. No edit to model.hpp, project.cpp, convert.cpp or conversion_session.cpp is proposed by this report; changing their public flow would require a separately source-qualified constructor barrier, not this optional prefix.

Focused acceptance for the passive increment: same actual Session prepared-Mo/slot identity; shell creates no native effects; escaped anchor does not retain epoch; Project release expires marker while registered Mo/slots survive; placement Project address reuse gets a fresh bundle/control block; foreign/legacy non-bundle marker lookup refuses safely; failed host control-block construction destroys its bundle once; p11 renamed private types link; opaque public-header test remains incomplete. Any fault injection must be private host-allocation testing, not a claimed original CRT failure observation.

Additional conditional prefix tests: actual source-profile HeapCreate producer before Project, compatibility false still accepted; bind same owner versus generation/control-block conflict; unknown/late binding refuses with zero calls; actual HeapAlloc8 nonnull produces exactly ordered backpointer/publication and sentinel-pending report; repeated inspect/retry produces no second allocation; untouched native fields remain unknown rather than checked by reading uninitialized bytes; actual epoch loss invalidates continuation without native free; runtime retention survives anchor/Project/slots destruction independently; no Close admission; bounded worker ending explicitly qualified as process reclamation. Real failure must be observed or separately labeled model injection; never claim a deterministic injected null is a real source allocation-failure execution. Default existing Open/Save paths remain unbound and preserve their prior behavior.

## Read-only receipt

Read-time CP121 source hashes: private anchor header `46be402688973a6412f17de8e340c53fead5552370b824efe60239ea3a7bc540`; storage cpp `4c49fa2daa380612e223c41a19410a7fc30bd74194be5bd65f000b5e2986e32f`; model.hpp `6ee1d43bf8112ba37d224e3f626ac88d99ca6a89f4835169d8d13242a13c65b6`; project.cpp `00778dfa9e2d05acf091aad5d459d46b7f54f11720a8cd41e6057910497e5868`; conversion_session.cpp `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604`.

Retained inputs: CP120 ownership addendum SHA `3fe505315ffc6dff91a810111895a4696b5d63f976c07850e179988ac5bcdacb`; CP119 allocator report `dedc4f677a1f1bbcadde4b4dc55fc34663fcdc70ad8678b72e1407bb57e3dedb`; CP119 inputs JSON `ff1ea36ace289559ebca9ef69c0f0811f7a0626f8ed6efd8c6216b3aa008cf51` (all eight referenced raw artifacts rehashed and matched here); CP116 lifetime report `8a24844a96af8d6897c96c4720635a3a646081548b376e7890520d8486013871`; its evidence JSON `215c68d512628362459b92755baf4c71c664a78683acc55e16d605867f768a94` was hash-checked, with no claim of revalidating its whole artifact inventory. Directly reread map constructor ASM SHA `cb7994cfa20f3bc8465016648e826ecdf9f0d8818e33b07f8620c88cb56376fc`.

Only this unique report was written. No production/shared source, test, CMake, build, live heap, original process, GUI or mutable REA operation occurred. This is an actionable private ownership/producer proposal with explicit blockers, not a completed proxy allocation or native Close implementation.
