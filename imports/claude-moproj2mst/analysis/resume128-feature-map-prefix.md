# CP128 isolated map proxy prefix: source release

The explicit owned-test runtime now supports one isolated12f1060 map-constructor prefix. It prepares stable standalone map storage, uses the genuine produced candidate heap for actual HeapAlloc8, writes the proxy backpointer and then map proxy slot, and parks before sentinel allocation. It does not bind Project, observe the original global, construct a complete map or execute native cleanup/Close.

The three reserved existing files are frozen:

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| src/model/feature_crt_runtime.hpp | `2dfd0cad50f484565b71a183fc5fa8d2a1e8084711d1652f523567976d2384a7` |
| src/model/feature_crt_runtime.cpp | `2d46a0b5a975d7bb345ac63ab590245ccf60944596f75cb6d9cb8c3c89377ff1` |
| tests/model/test_feature_crt_runtime.cpp | `69aba7a5fd41878e4b4c4e521bcd0649752119ada902e17c717e17086ee9209e` |

## Owner, preparation and bounded retention

prepareOwnedTestMapProxy accepts only the receiver's genuine canonical shared lease. Empty, null-alias and same-pointer foreign-control-block leases refuse before effects. An actual published nonnull candidate heap and completed producer are required; preparation never lazily invokes HeapCreate or supplies a fallback. It creates all host call/storage/control-block metadata before any later explicit resume.

Runtime::Storage strongly retains one FeatureMapProxyCall, which weakly borrows the runtime. The retained call contains stable standalone M storage and the actual proxy pointer after publication. It has no Project pointer or source-scope claim. A second distinct preparation returns ExperimentCapacityUnavailable, explicitly a candidate experiment limit, before HeapAlloc. Repeated resumes of the existing frame are allowed but never replay allocation/read/stores. The runtime/global owner retains both M and A after local inspection leases end; no reciprocal strong cycle was added.

The standard-layout M storage has proxy+0, opaque08..2f, head+30 and count+38. Compile gates bind those offsets, x64 pointer size,8-byte ProxyNode and trivial destruction. An explicit empty MapHeader constructor leaves native fields uninitialized. Separate metadata reports head/count unknown and never reads them. Only the two positively committed slots are inspected after their actual stores. Host storage allocation is not claimed as a native Project+60 producer; this M cannot be transplanted into a Project later.

## Native-function prefix and failure boundary

Under owner/call locks, resume authenticates the actual stored runtime lease and readiness, reads the private candidate slot once, and records its current candidate handle identity separately from the earlier host readiness observation. It calls actual HeapAlloc(heap,0,8), with no zero-fill flag. Actual nonnull return permits placement construction of an8-byte trivial ProxyNode containing &M (12f1085), followed by M.proxy=proxy (12f108c). Recorded backpointer and proxy values are read from those actual committed fields. The frame then reports SentinelPending/SentinelAllocationUnavailable at12f1092→12edac0. No head/count/sentinel/provider store or constructor return is claimed.

Actual kernel null return is recorded as AllocationFailurePending inside malloc before1414748ff's new-mode read. It does not publish a null M.proxy, return a source operator-new result, invoke a host handler, retry, synthesize bad_alloc or unwind. A source-slot-null guard similarly stops before unported fatal work. A candidate-slot read or attempted allocation consumes this represented prefix so even those pending branches cannot overflow the effect ledger by replaying reads.

The only new actual HeapAlloc call site has literal arguments heap,0,8. No free/destroy/default hook exists. Native sentinel-constructor EH can free a proxy only on its actual throw path; a staged guard is not that throw. The raw block remains allocated on the process-retained candidate heap, with its map/backpointer owner also retained. Final OS process reclamation is candidate instrumentation, not original CRT teardown or native Close.

The existing producer's static admission flags remain scoped to its own producer receipt. A separate map report records this explicitly admitted isolated call's actual effects while originalGlobalObserved, nativeMapConstructed, projectBindingAdmitted, nativeUnwindPerformed, nativeCleanupPerformed and completeForPublicClose stay false. Candidate handle/map/proxy identities are never labeled original-global observations.

## Focused tests and static preflight

The existing runtime executable now has eight cases. The pre-producer lease case also checks that genuine metadata without HeapCreate refuses map preparation, false leases refuse, and no producer call is started. Existing real producer/lease tests remain unchanged in behavior.

The new declared failure case records source-ordered heap read, positive declared heap check, allocation entry and null return, then verifies the new-mode guard, zero publications, unknown head/count and no authority-producing owner identities. Its origin is DeclaredModel, with no actual handle, runtime, map or block; it is not claimed as an actual Win32 allocation failure.

The actual map case requires real producer success, prepares one real standalone M, checks preparation/capacity consume zero allocation calls, resumes and requires actual nonnull A. It checks the raw candidate heap argument matches the live read and earlier preparation, bytes8/flags0, exact six-effect order, actual A->map==M and M.proxy==A, one store each, unknown head/count and sentinel guard. Repeats preserve identities/counts. Dropping the local call lease leaves its weak observer live through runtime retention; reacquired resume preserves both objects. Further distinct preparation refuses capacity; a foreign lease still refuses identity before capacity. Producer counters/effects remain unchanged. A candidate-only raw pointer/argument receipt is printed for root's actual test log.

Static checks bind CP127 source archive build/wave3-checkpoint127-verified/source.zip SHA `e697b567634d15cb303cda5153c0fa54c6f761ba1d848ae3d3a955672c14fe4f`. Constructor, producer, producer report, canonical matcher, runtime factory and old declared producer projection function bodies match that archive. Checks also verified one HeapAlloc call site and ordered placement/publish, repeat guards, preparation with no kernel/factory calls, no HeapFree/HeapDestroy/GetProcessHeap or model/Session includes, resolved quoted includes, balanced delimiter counts, Testkit entrypoint/macros, eight cases and no C assert. These are static checks, not a compiler/runtime result.

Root target remains m2m_model_feature_crt_runtime / m2m.model.feature_crt_runtime. No CMake change or new standalone/p11 dependency was added. Public model.hpp remains SHA `6ee1d43bf8112ba37d224e3f626ac88d99ca6a89f4835169d8d13242a13c65b6`; existing Project storage cpp remains `af2e1da0fe4bdc582e29731b2f0e6ce7d7f0a882ea2ade477f164807da1199e8`.

## Evidence and remaining guard

Design analysis/resume127-feature-binding-next.md SHA `9642dd74679be972aab0a91d9f0aa6199f0a8015d164b4570c244b253436157e` binds original v11.0.2 EXE SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, map constructor ASM `cb7994cfa20f3bc8465016648e826ecdf9f0d8818e33b07f8620c88cb56376fc`, malloc ASM `9c524ffaeda19a20d8d5da27ca5fcb7def4b8eb1a4554353348ccb968b731d8a` and proxy-EH cleanup leaf `4d0544f4631a2278c1768540856bce692ba32e259260a223dea92eaf8c7e646e`. No new original execution or mutable REA was required.

Default reconstructed module startup, actual Project first089917 binding, suspended-constructor/adapter continuation policy, reusable-host resource retirement, sentinel/feature/provider state and accepted Close remain separate guarded prerequisites. An isolated successful map-prefix experiment supplies none of those admissions.

All three source files are released and held through root validation/archive. Only this NEW receipt accompanies them. No build, CTest, live kernel call, GUI, original process or mutable REA was executed by this lane. Root owns actual compile/runtime outcomes, including any environmental allocation failure.
