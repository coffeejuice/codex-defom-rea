# CP134 independent sentinel-free review

No blocking defect was found in the frozen sentinel-free continuation. It uses the genuine current candidate heap/H once, preserves the raw kernel result, guards zero before the native errno provider, and on nonzero retires H before clearing M.head/count. It does not dereference H in the free phase or free A. Acceptance is limited to this explicit isolated map operation; it supplies no full C/Project destructor or public Close admission.

## Reviewed source binding

All four release hashes were independently rechecked under source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|src/model/feature_crt_runtime.hpp|`b2fb04cb60a05a06188264456075a2b1f2700bc65087b388c011677a6be7ad54`|
|src/model/feature_crt_runtime.cpp|`c02fb82749f1cee9c552d4d2216856893c8cf8021f41cbfb25fd04317eb0a01d`|
|tests/model/test_feature_crt_runtime.cpp|`a52cfc0cc545036b28fbd3cde4d3d7900eacdf07f571b35c6767be1f5ce39c37`|
|tests/model/test_feature_context_prefix.cpp|`a46d9009174b04aa5919527b4d6226926f6866c5d8f3908673673db21bf5c1a5`|

Owner receipt analysis/resume134-feature-sentinel-free.md SHA is `c4b4b53c5a607c6fb41155b8386857fdb7e8c32225bb85f1f5e79e18dbd565fe`; retained contract analysis/resume134-feature-clear-next.md SHA is `6275cf3cdd385e2607588a73e4d827d4b591c96d587b4be1852d3477e01b676d`.

## Canonical allocation owner and source ordering

Runtime cpp709..786 operates under existing owner-before-call locks. The started latch at722 precedes all H/owner validation on retries. First admission requires the genuine canonical runtime, completed actual producer and empty-clear cursor, actual M.head/recorded head allocation/free-argument agreement, live A/M backpointer and zero count. The host provenance check uses the current candidate slot and requires it to match H's actual allocation heap; it does not substitute a remembered handle. The subsequent represented source occurrence rereads that current slot at750 and calls actual HeapFree(currentHeap,0,head) at759.

Arguments are captured as candidate identities/flags0/H before the kernel call; one-shot occurrence is consumed first. This is not original144700810 observation or arbitrary caller-provided heap authority. The preconditions ensure the native null-pointer shortcut is not fabricated for an allocated sentinel.

recordSentinelFreeResult at198..207 retains the raw BOOL as int, emits return/result-test observations and assigns the error guard only on zero. Zero returns at761..765 before source-free return, head/count stores or proxy argument load. No errno,1474134, GetLastError, mapping function, fallback free, retry or source abort is invented. This agrees with original1471b9c occurring before its later GetLastError1471ba4. Returning to the host does not certify TLS/LastError continuity for a future error-path resume; the implementation receipt explicitly retains that missing witness.

Nonzero records the qualified1471bb8 free return, marks H retired and clears only host head-allocation metadata, then executes M.head=null at771 and M.count=0 at775. Those source stores are after successful free return and remain in order even though count was already zero. It records inner12f0860 return and loads actual still-live A from M.proxy for the next12f0d14 guard. No M.proxy-null store or A free is added.

Static inspection independently checked the one actual HeapFree site, the result branch before caller stores, success-retirement-head-count order, one-shot guard placement, no H-field dereference anywhere in this method and no allocation, error query, additional free or HeapDestroy. The fixed ten-event success ledger and five-event declared-zero prefix match the qualified boundaries.

## Post-free safety and context interaction

After positive deallocation no H field is read. Source M remains valid host receiver storage; its pointer/count are assigned without dereferencing the retired sentinel. New arguments use uintptr_t historical tokens. Old sentinel/clear identities and values remain phase history, not live allocation capabilities. The new sentinelFree.sentinelRetired/sourceFreeReturned/headNullStored/countAfterStore fields carry current progress; the earlier clear report's sentinelFreeEntered=false is explicitly scoped to that earlier phase.

The actual A allocation is untouched and its backpointer still names stable M. Runtime retains the frame and any embedded C; the heap/module owner remains retained, with no HeapDestroy or automatic destructor hook. H release therefore does not release M/C backing while A still refers to it. This safely separates H from later proxy/M/C lifetime.

Old empty-clear resume returns on its sticky started latch before any head validation; old8/48-byte methods refuse the later stage. Context resume checks that same destructive lane before owner/profile or completed-prefix fast paths. It cannot adopt/replay C after clear/free even though constructor/context receipts remain historical. C1450, strings/PST/provider state stay unknown; no12f0d20 context destructor is invoked or implied.

Loading A for the explicit isolated map-tail guard is not proof that normal context destruction reached its corresponding tail. Standalone12f0cf0 and context12f0d20 are distinct callers; the latter has unqualified optional-member/global work first. This implementation stops before A free and does not claim either full map-wrapper completion or accepted C/Project Close. Future proxy free must preserve that caller distinction and the absence of a native M.proxy=null store.

## Tests and review limits

The reviewed tests assert actual positive raw HeapFree prerequisite/arguments, ten-event order, head/count state, A/M/C retention, unchanged constructor/allocation/context history and repeated-call safety. Dedicated later test cases reacquire weak inspection leases after prior local leases ended. The declared-zero test has model origin/no real allocation authority and checks no source return, retirement, caller stores, proxy load or error-query entry. Tests inspect no retired-H data or post-free heap query. They remain assertions reviewed in source, not runs performed by this reviewer.

Root owns focused targets m2m_model_feature_crt_runtime and m2m_model_feature_context_prefix, grouped execution and source-stable archive. Actual environmental zero is an unresolved error continuation/failing positive prerequisite, not a native Close-abort result. Proxy free/error mapping, original CRT retirement, full context/provider cleanup, native EH and public lifecycle remain outside this acceptance.

Native comparison directly reread original DEF_GUI_MO_64 v11.0.2 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, retained free ASM SHA `d8ad91d7c47a3f6610809dee457c102415bbc6b5550267d3a8b96860b3b597af`, map-wrapper tail `81a3fcc307651a95473c93dda2ffb73805268a43f0bc0cc9c1c7cc7dcc49d5e7` and context-destructor sequence, with source qualifications retained in the contract. No original process/global/heap was observed.

Only this NEW review report was written. No source/test edit, build, kernel/runtime, GUI, original target execution or mutable REA occurred.
