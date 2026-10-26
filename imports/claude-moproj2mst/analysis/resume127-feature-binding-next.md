# CP127: isolate the first map allocation before default Project admission

Canonical lease validation closes a candidate ownership check, not default Project allocator binding. The next source-closed executable slice is the isolated map-constructor prefix12f1060 through actual HeapAlloc8 and its two pointer stores. It can use the genuine CP124/126 candidate heap while retaining one pending map/proxy in the explicitly bounded owned test process. It must remain an isolated native-function reconstruction: no fabricated Project identity, default hook, provider state, sentinel, native unwind or Close result.

## Original contract and current frontier

Retained original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The native Project constructor calls12f10d0(P+60) at089917, before f10=5. Context construction passes M=C+1458 to12f1060 before initializing later context fields. The map constructor's bounded first branch uses only that M identity and the module CRT allocator; it reads no PST/provider/Project field.

| Source | Exact prefix effect or guard |
| --- | --- |
|12f1076/12f107b|Request8 through same-image operator-new14146eabc and malloc14147489c.|
|1414748c0/8c7|Read/test current module CRT heap slot144700810. Missing heap reaches unported fatal work.|
|1414748f1|Actual HeapAlloc(heap,0,8). No zero-fill flag.|
|nonnull return via14147492b and outer new|No new-mode, new-handler or errno branch is read on this successful path.|
|12f1085|Write actual M into actual returned block A.|
|12f108c|Publish actual A at M+0.|
|12f108f/1092|Constructor EH state0 is active; next call is sentinel helper12edac0. Guard before that separate48-byte request.|
|HeapAlloc returnednull|Park inside malloc before1414748ff reads new-mode144700858. This is not operator-new returningnull, a constructed map, or native exception/unwind.|

CP126's matchesRetainedLease authenticates pointer and shared control block. It intentionally returns true for genuine owner metadata before HeapCreate. Any allocation prefix must additionally validate the runtime's actual private candidate slot/current producer result under the runtime lock. A copied public report or matching pointer/profile string cannot supply that authority.

CP124's actual Win32 run remains candidate-only: its real heap and candidate slot are not the original global144700810. Source-role equivalence follows from reproducing the proved function path, not from claiming original runtime observation. No new test/runtime was executed in this investigation.

## Safe private successor: one isolated map call

Reserve existing src/model/feature_crt_runtime.hpp, src/model/feature_crt_runtime.cpp and tests/model/test_feature_crt_runtime.cpp only. An additional private map header may be split out for readability later, but is not required for this bounded cut. Do not edit Project/epoch/model.hpp/Session/C ABI/default CLI or standalone target wiring. Put definitions beside private Runtime::Storage, so no heap HANDLE is exposed and no storage-cpp dependency is added to standalone/p11 builds.

Proposed private model API:

```cpp
class FeatureMapProxyCall; // isolated source-function frame, not a Project context
struct FeatureMapProxyPrepareResult; // call or explicit metadata/profile refusal
// Member of FeatureCrtRuntime:
FeatureMapProxyPrepareResult prepareOwnedTestMapProxy(
    const std::shared_ptr<FeatureCrtRuntime>& canonicalLease);
// On the privately constructed, runtime-retained call:
FeatureMapProxyReport FeatureMapProxyCall::resume() noexcept;
FeatureMapProxyReport FeatureMapProxyCall::report() const noexcept;
```

Preparation may perform host C++ allocations and throw host allocation failure before any source HeapAlloc; it must not mark them as native CRT outcomes. It authenticates the supplied canonical lease and requires the existing producer's actual nonnull candidate slot. It never lazily starts a producer, creates a fallback heap, accepts an arbitrary HANDLE or uses declared-result projections. The call owns a real stable standalone candidate M storage object. It is explicitly not P+60+1458 and cannot later be transplanted into an existing Project as if it were that Project's original context.

Runtime::Storage retains one call strongly for this owned-process experiment. The call borrows the runtime weakly; no runtime/call strong cycle or Project owner is introduced. Reserve only one distinct pending map per runtime and refuse a second preparation with an explicit **candidate experiment-capacity** result before native effects. Repeated resume/report on the existing call is allowed and must never allocate again. The capacity guard is instrumentation, not a claimed original native branch or allocation failure. No unbounded production Open path may call this API.

The call's stable map storage can have standard-layout slots matching known M offsets: proxy+0, opaque bytes+8..2f, head+30 and count+38. Compile-check those offsets, x64 pointer size and an8-byte trivial ProxyNode containing only the map pointer. Its explicit empty host constructor must leave native slots uninitialized; keep separate knownness flags and never read unknown pointers/count. Do not use value-initialization/default-zero as native construction. Report/compare identity through the map object's address without pretending Project layout. Only after success may the map's proxy slot and ProxyNode backpointer be read.

Before entering HeapAlloc, prepare the entire host call/control-block/report/retention storage. The runtime lock serializes genuine owner/current-slot validation and execution. resume reads the actual candidate slot corresponding to source1414748c0, executes HeapAlloc(slot,0,8) once, then on actual nonnull return writes the backpointer and proxy slot in source order. A placement construction of an8-byte trivial ProxyNode can express the first store; it must not call host allocating new for the native block. Mark each committed effect and stop before sentinel allocation. Unknown head/count/feature state stays unknown.

On actual null return, record that the kernel call happened and permanently park at AllocationFailureInputsUnavailable before new-mode read. Repeat resume does not retry, call a host handler, publish a null map proxy, return a source operator-new result, or synthesize bad_alloc. Declared pure failure events may test state transitions, but cannot become an actual block/heap owner.

## Why retention must include M as well as A

The actual native8-byte block contains a pointer back to M. Retaining A while allowing its standalone map object to die would create a dangling backpointer even in this isolated experiment. Retain both stable map storage and block receipt in the existing explicit process-retention owner. Dropping local call/runtime inspection leases must leave both alive and the same call inspectable until that test process ends. Do not keep an expired Project strongly or use a raw-Project global registry.

Native state0 EH action141b9c670→12f5e30 frees the proxy only when the later constructor actually unwinds; a staged sentinel guard does not throw. Consequently neither call destruction, failed metadata preparation after publication, epoch destruction nor report inspection may HeapFree A. No HeapDestroy is inferred. The proposed bounded process policy reclaims resources only when its owned test process ends and must be labeled candidate instrumentation, not native Close. It is not the missing reusable-library retirement policy.

## Focused acceptance

Extend the same private test executable or add an independently owned model test if root prefers a separate process. Maintain one real producer and one real pending proxy per test process; do not create a second map merely to test capacity.

* Invalid/empty/null-alias/same-pointer foreign-control-block lease refuses before map preparation or HeapAlloc. Producer metadata without actual creation is not heap readiness; no lazy producer or kernel call is allowed.
* Actual producer success plus genuine lease creates one stable isolated M/call. Preparation itself makes zero HeapAlloc calls and leaves all native map slots unknown.
* Actual resume calls HeapAlloc with8/flags0 against that same live candidate heap. Observe real nonnull A, actual proxy->map==M and M.proxy==A, exact source-effect order, SentinelAllocationUnavailable and head/count still unknown. Completion/Project/Close flags stay false.
* Repeat resume and dropping/reacquiring local inspection leases preserve M/A identity and counts. A second distinct map preparation hits the explicit test-capacity guard before effects.
* Deterministic declared null allocation remains a model origin, has no publications, and parks before new-mode/handler/errno. It cannot be inserted into the real pending call as an actual receipt.
* Assert no free/destroy/provider call and no source unwind claim. Any test seeding opaque bytes is host fixture setup, not a native producer; the normal implementation must not inspect uninitialized suffix bytes merely to prove they are unknown.

Actual HeapAlloc success is an environmental prerequisite for the positive case. An actual null result is retained as its guarded path and cannot be relabeled success to pass a test. No public or original-process parity claim follows from the isolated unit.

## Default Project lifecycle remains guarded

Original startup calls HeapCreate producer before application entry; the current helper is explicit test-only and stops before later CRT startup. The CLI statically links ABI/core, the DLL has its own core instance and direct C++ entry bypasses wmain. Production binding needs a selected module-lifetime producer/profile covering these routes, not a per-Session or per-Project heap. Native allocation/free read the global at their own occurrences; no whole-program immutability or all-writer proof is supplied by one startup receipt.

Actual Project integration then needs a pre-first089917 binding to the prepared same Mo/slots/epoch and a coherent suspended-call/partial-effect policy. The current public initProject continues supported adapter f10/path/backlink work after recording its historical passive guard. That record is not a native stack frame that can be resumed after Open. A standalone M allocated in the proposed unit cannot authorize that continuation or be substituted for the actual inline context.

Close remains a separate frontier: accepted native feature release precedes Project destruction and can insert null map nodes, erase a node before provider deletion, and clear feature flags only after later successful work. Context cleanup has the unqualified optional+1450/global owner, distinct map/tree/sentinel/proxy effects and EH paths. The original CRT exit path additionally has lock/state/encoded callback dependencies and a late callback using the same CRT free after application return. None of these authorizes free at candidate Project epoch destruction. CP125 retains exact static spans and source evidence; no additional original trace was needed here.

Thus the proposed source-closed cut advances an actual native function prefix and block/owner state while keeping default Project integration closed. If root requires a public lifecycle change instead, the missing startup/first-phase/Close facts above are blockers; another passive readiness getter would not resolve them.

## Read-only receipt

Current CRT header SHA `25b1cc1059fa225017bec07dd5289b9084f92c12fdd78f0bade562d3ce65071c`; CRT cpp `89989f0d68c6bf7657b8d86e587ebfe550ca95a457c2f385efd5279a01911be8`; passive storage header `4890457bd665909fca8f6059bd09093b8b5bfab63aff12f3b4f9d2bb72f53ec0`; storage cpp `af2e1da0fe4bdc582e29731b2f0e6ce7d7f0a882ea2ade477f164807da1199e8`. CP126 lease receipt SHA `2af74f4b2d512dfeaaa7807f9ee8b3edd8b47b79d28198b167a3fd1413df4515`.

Directly reread immutable map constructor ASM SHA `cb7994cfa20f3bc8465016648e826ecdf9f0d8818e33b07f8620c88cb56376fc`; malloc ASM `9c524ffaeda19a20d8d5da27ca5fcb7def4b8eb1a4554353348ccb968b731d8a`; proxy EH cleanup leaf `4d0544f4631a2278c1768540856bce692ba32e259260a223dea92eaf8c7e646e`. CP125 binding report `f66eb5db2c08ed54654ff32139f9d92a1b4193946fc7684c6178a996f196246f` retains original startup/CRT exit and candidate-vs-original distinctions. These are source qualifications, not new runtime observations.

Only this NEW report was written. No source, test, build, GUI, original application, live heap or mutable REA work occurred.
