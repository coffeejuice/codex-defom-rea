# CP129: finish the isolated map constructor on sentinel success

The remaining normal-return path of12f1060 is source-closed for the existing isolated map frame: one actual48-byte allocation, five sentinel initialization stores, publication/self-links/count and returnM. No provider, PST or Project field is consulted. Add an explicit private resumeSentinel operation so the existing8-byte checkpoint remains independently inspectable. Default Project binding, native unwind and Close remain guarded.

## Exact retained source ledger

Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Helper12edac0 is fully retained through RET12edb0e; outer12f1060 is retained through RET12f10c6.

| Source occurrence | Successful-path effect |
| --- | --- |
|12edacd/12edad2|Request0x30 bytes through the same14146eabc→14147489c allocator wrapper. It rereads the module heap at the new malloc occurrence.|
|1414748f1|Actual HeapAlloc(currentHeap,0,48). Nonnull normal path skips new-mode/handler/errno, as for size8.|
|12edae3|H+0=null.|
|12edaf3|H+8=null.|
|12edaff|H+10=null.|
|12edb02|Byte H+28=1.|
|12edb06|Byte H+29=0. Helper returns unchanged H in RAX.|
|12f1097|M+30=H, only after the helper's stores.|
|12f109b|Byte H+29=1.|
|12f109f/12f10a3|Reload M+30; store H+8=H.|
|12f10a7/12f10ab|Reload M+30; store H+0=H.|
|12f10ae/12f10b2|Reload M+30; store H+10=H.|
|12f10b6|M+38=0, after all three self-links.|
|12f10be/12f10c6|Return M.|

For an actual valid nonnull48-byte heap block, the helper's H+8/H+10 address tests take their nonnull stores. Do not copy the odd null-RAX tail into a failed HeapAlloc branch: raw kernel null must first stop inside malloc before1414748ff reads new-mode. The outer operator-new has not returned at that boundary. No native throw is established by the kernel null alone.

Only H+0/+8/+10, H+28/+29 and M+30/+38 become known. H+18..27 and2a..2f remain untouched/unknown; they must not be default-zeroed as key/payload/padding. Map opaque bytes and every enclosing context/Project feature field remain unknown. A candidate complete isolated map constructor is not complete context12f10d0; that caller's next stores start at12f10e9/12f10f0, and no actual C/Project object is supplied by this unit.

## Exact private source cut

Reserve existing src/model/feature_crt_runtime.hpp, src/model/feature_crt_runtime.cpp and tests/model/test_feature_crt_runtime.cpp only. No new translation unit, CMake, Project/epoch/model.hpp, Session, C ABI or default hook is required. All native heap access stays in the existing runtime cpp.

Add `FeatureMapProxyReport FeatureMapProxyCall::resumeSentinel() noexcept`. Keep existing resume as the8-byte phase: repeated calls before explicit sentinel admission continue to return that checkpoint. After later completion/failure, either method returns the current receipt without replay.

The new operation requires the same real frame at SentinelPending, the genuine canonical runtime lease, the existing actual A/M/backpointer publication and current produced nonnull candidate heap. Lock in the existing owner-before-call order. It rereads the actual candidate slot for this second allocation; do not reuse an earlier report's handle as authority. Preserve candidate/current-module terminology throughout: original144700810 remains a source role, never an observed live global.

Add separate fixed-size sentinel phase state/report/ledger rather than overwriting the proxy allocation's raw arguments/effects/counters. Suggested nested sentinel report contains its own current-slot read, HeapAlloc48/flags0 arguments and outcome, H identity, initial null links, byte28=1, initial nil0, helper return, M.head publication, nil1, three head reloads/self-links, count0 and returnedM identity. There are20 ordered observations if both helper/map returns and all three head reloads are explicit; preallocate the ledger and all host metadata before kernel execution. Existing source8 fields continue to describe that first phase; an optional total allocation count can explicitly sum the two.

A private standard-layout48-byte SentinelHeader should have pointer fields+0/+8/+10, opaque16-byte payload+18..27, byte28, byte29 and opaque6-byte suffix2a..2f. Compile-check size/offsets and use an explicit empty constructor to begin object lifetime without initializing unknown members. Perform the five helper stores explicitly in source order. Publish M.head next, update nil, and reload the actual known M.head for each self-link store in+8,+0,+10 order. Store count0 last. Report observations by reading only fields already initialized. No host allocation, callback or throwing wrapper belongs between actual allocation and these stores.

Success advances to a MapConstructorReturned stage and a dynamic **candidateMapConstructorReturned** flag, with returned identity exactly M. Replace/migrate the old static nativeMapConstructed=false assertion rather than leaving a misleading statement about the now-completed isolated unit. Keep originalGlobalObserved, Project/context admission, native unwind/cleanup and public Close false. headKnown becomes true only at M.head publication; countKnown becomes true only after its final store. A separate key/payload knownness mask should remain false.

## Failure and retention

Actual HeapAlloc48 null records one attempted/returned allocation and parks at a distinct SentinelAllocationFailurePending state before new-mode/handler/errno. It leaves actual A, M.proxy and all prior8-byte effects intact, does not publish M.head/count, and does not retry. A current-slot-null or owner-unavailable observation must similarly preserve a consumed phase cursor and avoid replay/ledger overflow.

Native map EH state0 maps to141b9c670→12f5e30, which frees the proxy only if later construction actually unwinds. A staged unknown-new-mode guard is not that throw; do not free A or null M.proxy. The successfully allocated H must also be retained if any future continuation is parked. Existing runtime ownership of the call can retain M+A+H until this explicitly bounded test process ends. No destructor/epoch cleanup/HeapFree/HeapDestroy is introduced here. A completed isolated map could later support a separately qualified explicit empty-map destructor, but automatic RAII cleanup and accepted Project Close still do not follow from it.

If the new phase is skipped, existing process-retained M+A behavior is unchanged. If it succeeds, capacity remains one map with two native blocks. If it fails before allocating H, it remains one map/one proxy; losing a reported resource or allocating another map as retry is not allowed.

## Focused tests

Extend the existing real-map case after its current proxy checkpoint checks. That case already has the runtime-retained frame; this avoids another map, capacity exception or cross-case fixture registry. Before resumeSentinel, verify no second allocation/head/count knowledge. Then:

* Actual48-byte allocation uses the same qualified current candidate heap with flags0, one slot read/attempt and an actual nonnull H distinct from A.
* The source ledger records null H links in+0,+8,+10 order, byte28=1, nil0, helper return, M.head publication, nil1, self-links+8,+0,+10 with three actual head reloads, then count0 and returnedM.
* Observe actual final H self-pointers/nil/byte28 and M.head/count; proxy/backpointer identity and prior8-byte ledger are unchanged. Unknown key/payload/opaque bytes are not read or assumed zero. The dynamic candidate constructor return flag is true while context/Project/Close flags remain false.
* Repeating either phase produces no additional allocation/store, and dropping/reacquiring local leases retains M, A and H with the same identities. Distinct preparation remains at the existing explicit experiment-capacity guard.
* A separate declared sentinel-allocation-null projection has model origin and no actual runtime/handle/H. It preserves the rule that prior proxy effects are not undone, publishes no head/count, stops before new-mode and asserts no throw/unwind/free. It cannot be written into the real frame as an actual failure witness.

Do not force process OOM, pass invalid heap handles, invent a native new-handler result or turn a raw kernel null into operator-new null. The positive real test must report an actual allocation failure as its pending state/failing prerequisite rather than relabel it success. Test-only fixture seeding, if used, stays distinct from native unknown-field producers.

## Input receipt and scope

Root's CP128 final-tests receipt SHA `81155f17d240d2223ad4cfd45ac51e79606a80d98e8eb844768f3a3d40003644` records the selected runtime test passed with stable source/binaries. Its retained log build/test-runs/1792924660863150500/m2m.model.feature_crt_runtime.log SHA `f20c87d975bff41b2acfe63e1c251393c9ef046f1660d31e6fb91d66da932be5` records candidate create1/store1/compatibility1/producer1 and actual8-byte allocation/backpointer/publication1, sentinel pending; all eight cases passed. These are root observations, not execution in this read-only lane or original-process evidence.

Current CRT header SHA `2dfd0cad50f484565b71a183fc5fa8d2a1e8084711d1652f523567976d2384a7`; cpp `2d46a0b5a975d7bb345ac63ab590245ccf60944596f75cb6d9cb8c3c89377ff1`. CP128 implementation receipt SHA `fa7d0beaa8c30c50de02e822538fa3a9486b45d6c8799daadf479ad571cc2fa0` retains the prior prefix boundary.

Directly reread sentinel ASM analysis/resume114-feature-map-sentinel-1412edac0.asm SHA `08e448ba9345cf8a15f02465c0667b3518c520542de75516b576956290c764ef`; its bounded capture receipt SHA `ac0a5569745dd3f61c28eb8b84f2ec9cf17a53cc9fb35d8ce5ca45f59cb661a1` binds unwind-region1412edac0..1412edb0f and tool identity. Map constructor ASM SHA remains `cb7994cfa20f3bc8465016648e826ecdf9f0d8818e33b07f8620c88cb56376fc`; proxy EH cleanup leaf `4d0544f4631a2278c1768540856bce692ba32e259260a223dea92eaf8c7e646e`. Existing CP119 allocator and CP125/127 startup/retirement qualifications remain in force.

Only this NEW report was written. No source, test, build, live heap, GUI, original target or mutable REA was executed or changed. This proposes one complete isolated success path; default Project startup/binding, context producers and Close remain guarded.
