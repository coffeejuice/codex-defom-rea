# CP125: authenticate the candidate runtime before any Project binding

CP124 established one real candidate-process HeapCreate producer run. It did not observe the original EXE global, complete the original CRT startup, or bind a Project allocator. The smallest useful next private cut is canonical runtime lease validation: reject an unrelated shared control block even when it contains the same raw runtime address. This can be implemented in the existing three runtime files, with no Project/model/header/default-hook change and no additional kernel producer call.

## What CP124 actually established

Root's source-stable final build/test receipts report m2m.model.feature_crt_runtime passed. Its retained log records candidate PID34232, module7ff72f9d0000, create1, candidate-slot store1, compatibility1, producer return1 and explicit process-ending retention; all five cases passed. Those numbers identify that candidate test process/module. They do not identify a live DEF_GUI_MO_64 instance or original global144700810. False compatibility and null HeapCreate coverage are separately declared pure-model projections, not actual kernel observations.

The runtime factory is explicit owned-test-process opt-in. The default CLI, C ABI, direct core driver, Session and Project still do not call it. CP122's Project epoch only owns passive host storage. Its pending constructor receipt remains HeapProfileUnavailable, before any native proxy allocation.

## Original constructor dependency and lifetime

The retained original v11.0.2 EXE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Its direct source chain is:

1. CRT startup141470c28 calls producer141484800 at141470ca3. The retained same body later calls application entry140001410 at141470d7e. The heap producer precedes that application call on the represented successful startup path. Other startup helper success/failure behavior is not supplied by CP124.
2. Project constructor089880 at089917 passes actual P+60 to12f10d0. That constructor adds1458 and calls map constructor12f1060 at12f10e4. This occurs before Project's later f10=5 store089939.
3. Map constructor12f107b calls same-image operator-new14146eabc(size8); malloc14147489c reads actual CRT slot144700810 at1414748c0 before HeapAlloc. Actual nonnull return permits `[A]=M` at12f1085 then `M+0=A` at12f108c. No per-Project heap-binding routine appears on this chain: it is a dependency on the module CRT global.
4. A later source free141471b7c reads the same CRT heap role for nonnull HeapFree. Constructor unwind, later map destruction, provider release and CRT exit are distinct owners/occurrences. A successful first allocation is not permission to run any of them at adapter epoch release.

The original source role can be reconstructed in a candidate module without reading a running original EXE. What is missing for default admission is a coherent reconstructed startup/lifetime and consumer binding, not a requirement to borrow a live original heap handle. Original absolute addresses and matching profile strings remain evidence labels, not current handles.

## New bounded retirement evidence

A small read-only PE check here follows the already retained startup's return path. It adds evidence inside this report only; no analysis database, source, helper process target or original application was mutated/launched.

At141470d8f, the visible normal application-return branch calls14146e9bc with the application result. That thunk sets edx=0/r8d=0 and jumps14146e830. The adjacent14146e9d4 sets ecx=0/edx=0/r8d=1 and jumps the same body. Their meanings are derived from explicit arguments/body rather than assuming host CRT functions are interchangeable.

The complete retained-PE unwind region14146e830..14146e9ba first calls147fbdc(8), reads1446ffb18, and commits other termination-state fields. With edx=0 it decodes callback bounds from145054318/145054310, consumes entries in reverse, writes an encoded-null sentinel before calling the decoded callback, and rereads bounds after callbacks. At14146e941/954 it passes two static callback ranges to14146e710. The latter bounded body walks forward, calls each nonnull qword, and skips null entries. This is not a single unconditional heap destructor.

Static table141e14dd0..141e14df0 contains `[0,14148cca0,141495210,14146dc70]`;141e14df8..141e14e00 contains `[0]`. The last nonnull first-range callback14146dc70 calls1476d34, conditionally147f91c according to1446ffb10, then loads145054330 and tail-jumps to the same CRT free141471b7c. Its actual buffer can be null; when nonnull, the CRT heap is still needed at this late callback. Earlier callbacks, locks, live encoded callback lists and final process endpoint behavior remain unqualified. No HeapDestroy of slot144700810 is established by this extension. Do not infer that the heap is never destroyed from the bounded absence of such a call.

This strengthens the lifetime distinction: the source allocator is not owned by Project Close or by return from a candidate conversion. Even the application-return cleanup has further CRT consumers/state. CP124's module PIN and process retention remain candidate instrumentation, not a reconstruction of that exit machinery or DLL unloading.

New raw code/data bindings from the same immutable PE (end bounds exclusive):

| Span | SHA-256 of exact PE bytes |
| --- | --- |
|14146e830..14146e9ba, unwind region394 bytes|`d4bbbadeda4e0af5d3341d585876ede47aa55a1d43b8554ff8e9ce7e0dffe4dd`|
|14146e9bc..14146e9e1, three complete thunks plus padding37 bytes|`6802f7c55d19c7a9f1a60f1efffac84298bf00d46ca2c07b97bed7d0e849bcd3`|
|14146e710..14146e743, callback walker51 bytes|`1ea585365ad5359db718726fdd5cc50506dd3479e5c57a685c356d3fac2f93bd`|
|14146dc70..14146dc97, late callback39 bytes|`e0699f6a63ff97c216126eac57aae41c051d08a90b8de93b8f6e3d890ce73c05`|
|141495210..14149524b, inspected other callback59 bytes|`83099d086b108418300550875db56e9e77b91fb78bd795965dc5dd68164e60fc`|
|141e14dd0..141e14df0, static callback qwords32 bytes|`d01944e79e14da71e2065986f8a4464907490b2ec015a20764dee96eeeee266b`|
|141e14df8..141e14e00, null callback qword8 bytes|`af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc`|

The read used existing dumpbin14.44.35228.0 `/disasm:bytes /range:<start>,<end-1>` against evidence/original-binaries/DEF_GUI_MO_64.exe and a read-only PE section/.pdata parser for code offsets/bounds. An initial explicit thunk view ended in a partial next instruction at14146e9eb; that fragment is excluded from the qualified three-thunk span above. No broad exit/CRT completion claim follows from these local views.

## Exact ready private cut

Reserve only existing src/model/feature_crt_runtime.hpp, src/model/feature_crt_runtime.cpp and tests/model/test_feature_crt_runtime.cpp. No other source/header/CMake edit or new translation unit is required. Proposed public-to-private-model method:

```cpp
bool FeatureCrtRuntime::matchesRetainedLease(
    const std::shared_ptr<FeatureCrtRuntime>& lease) const noexcept;
```

This is candidate owner identity validation only. It must not mean heap readiness, original-global observation, Project admission, native constructor continuation or Close readiness. In particular it may return true before HeapCreate has been attempted; current report state separately shows whether the producer succeeded. A copied report or raw pointer never becomes runtime authority.

Add a weak_ptr<FeatureCrtRuntime> canonical lease to private Storage. The existing explicit retaining factory initializes it from the actual retained shared owner exactly once, before publishing the factory result and before any source kernel call. Keep it weak to avoid a runtime/control-block cycle. The existing function-local static initialization supplies publication ordering; do not write this weak member on every factory call or race report/attempt state.

The method refuses null, requires lease.get()==this, locks the stored canonical weak lease, requires that canonical pointer also equals this, then requires both owner_before comparisons to be false. It calls neither prepareOwnedTestProcess nor attemptProducer, performs no Windows/kernel/Project work, and creates no fallback owner. The temporary strong lock is synchronous and leaves no new retained owner after return. Do not create a process registry keyed by Project address or compare only report.runtimeIdentity.

This cut closes a concrete boundary: `shared_ptr<FeatureCrtRuntime>(genuine.get(),noOpDeleter)` currently contains a valid raw address with an unrelated control block. Private construction prevents arbitrary runtime birth, but does not authenticate that ownership lease. Canonical ownership validation makes this distinction explicit before future binding/borrowing APIs exist. It adds a private Storage field, not a Project/public-model field; completeForPublicClose and projectBindingAdmitted stay false.

Focused test additions in the existing runtime executable:

* Before the actual kernel-positive case, prepare the existing owned runtime metadata and verify a genuine/copied lease matches while producer counters remain0 and candidateSlotPublished remainsfalse.
* Reject an empty lease, a null-pointer alias sharing the genuine control block, and a same-raw-pointer lease with an unrelated no-op control block. The latter must leave the real retained owner intact and must not free it.
* Check matching/rejection changes no report stage, effects, module identity or kernel call count. Existing actual producer still performs exactly one create/compatibility sequence; existing process-retention test still observes the same owner after locals end.
* Repeat genuine/forged comparisons after actual producer success without a second kernel call. Keep declared-model cases separate and retain private-constructor/noncopy/no-report-conversion compile gates.

No second real runtime/module is fabricated for these tests. Cross-DLL source-role equivalence is not claimed by successful self-lease validation. Default Project binding remains fully guarded. Root owns compile/test/source-freeze acceptance; this proposal performs no checks by executing the new helper.

## What actual Project integration still needs

1. **Default runtime producer/owner.** Current CLI statically links abi.cpp/core; the DLL has its own static core, and direct C++ conversions can bypass both ABI fronts. A wmain-only hook misses other routes; Session/Ctx/private-call services end too early. Select an explicit module-scope reconstructed startup/lifetime profile before any first Project. The test-only factory/PIN policy must not silently become reusable-host production policy.
2. **Live allocator dependency.** Source malloc/free read the slot at their own occurrences. A future broker must validate current actual candidate module owner/heap role and generation at each admitted operation. CP124's receipt and static original source address are not a live original slot read or proof of all later writers/aliases. Additional source qualification is needed before assuming the original slot is immutable forever.
3. **First-constructor timing.** Bind dependency before actual prepared Mo's first089917, before f10/backlink completion. Current passive pending metadata after supported Open is historical, not a parked native constructor. There is no native per-Project bind call to replay; any adapter binding must preserve this distinction.
4. **Actual map/proxy owner and retirement.** CP122 bundles only an empty marker. A stable candidate map identity, actual HeapAlloc8 result, two ordered stores, no-replay cursor and unresolved-block retention are still absent. A guard before sentinel is not native EH, and epoch/anchor destruction must not invent proxy free.
5. **Failure and Close state.** Null HeapCreate caller mode, failed HeapAlloc new-mode/handler/errno, sentinel EH, provider features, optional context+1450/global cleanup and accepted Close remain separately guarded. Authentic runtime ownership alone closes none of them.

The ready three-file lease cut is therefore useful and disjoint; it does not enable actual default Project lifecycle. After it, a separately assigned candidate-only passive epoch/runtime dependency binding could be considered, with its stage/report explicitly experimental and original heap/profile/public admission still unavailable. Do not add such binding while implementing lease validation.

## Input receipt and sole write

Read source hashes: CRT header `6af8c9839e8626b8e2461754a585b6518eef24b6306a5f4a802d908d91d7d220`; CRT cpp `f33a1d43fee15ac580f087ca9135eed85dd69a9466a83bdd9fb575e75a07e8e0`; passive storage header `4890457bd665909fca8f6059bd09093b8b5bfab63aff12f3b4f9d2bb72f53ec0`; storage cpp `af2e1da0fe4bdc582e29731b2f0e6ce7d7f0a882ea2ade477f164807da1199e8`.

Root CP124 final-build JSON SHA `8451a3b4a53dc6772afd8b9241cbcdc8a949125156e788db2d311a3dc4ea45cb` reports source-stable build; final-tests JSON `7aa9c0434ddda416116694f07dbaaa8acfb4f8f8e6524b3bf87a9705046da19b` reports stable source/binaries and the passing selected test. Bound test executable SHA is `66d98e9294a059d93e9234a6ee83e8dda679ebe277240360b906365d5794cf89`. Its retained log build/test-runs/1792922188502749600/m2m.model.feature_crt_runtime.log SHA is `525eb5ecf2bc37deeb341173098a638a0998adfa168704fff51d458e0ec6804e`. Those are root's observations, not an execution in this read-only lane.

Retained original Project ctor ASM SHA `24a34f65dfdde858df38e7f6704be2b9efc0627e271ab23f10b8aa8121243711`; context ctor ASM `38e87349cb26103cb4f4d593fa0ac441ff4b5706283645e6cfdf00255cbe6e62`; CP116 lifetime report `8a24844a96af8d6897c96c4720635a3a646081548b376e7890520d8486013871` retains accepted feature-release/context/Mo ordering. Existing CP119 producer/caller/allocator qualifications remain in force.

Only this NEW report was written. Local PE reads/disassembly did not execute the original target. No source, test, build, live heap, GUI, original application launch or mutable REA work occurred.
