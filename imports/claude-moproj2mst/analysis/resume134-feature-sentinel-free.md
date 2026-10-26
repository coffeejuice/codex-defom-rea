# CP134: actual isolated sentinel free continuation

Implemented opt-in `FeatureMapProxyCall::resumeSentinelFreePrefix()` after CP133's completed empty clear. It attempts one actual HeapFree(current canonical candidate CRT heap,0,H). Actual nonzero completes the qualified CRT-free success tail, then stores M.head=null and M.count=0, returning only the inner map cleanup and stopping before standalone map-destructor proxy free12f0d14. Actual zero parks before errno-provider1471b9c without caller stores or replay. M/A/C/runtime remain retained; no Project/Close, automatic destructor, proxy free or HeapDestroy was added.

Only the reserved runtime header/cpp and two existing tests plus this new receipt were edited. No Project/model.hpp/Session/CMake/plan changes, compiler/test/kernel execution, GUI/original execution or mutable REA database in this lane. Root owns actual grouped execution and source-stable freeze.

## Source order and owner

Read-only contract `analysis/resume134-feature-clear-next.md` SHA-256 `6275cf3cdd385e2607588a73e4d827d4b591c96d587b4be1852d3477e01b676d` qualifies immutable original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`:12f08b0 ->14dfc68 ->1471b7c, nonnull argument ->current144700810 heap read ->HeapFree at1471b92. Nonzero result skips the entire CRT error path and returns1471bb8; zero first calls1474134, then GetLastError and mapping before the source routine can return. Caller head-null12f08b8 and count-zero12f08bc follow only after return.

The new API has no caller-supplied pointer/heap/report. It authenticates the actual retained runtime, source producer completion and explicit completed empty-clear stage under owner-before-call locks. It checks actual still-live H ownership/M.head/free-argument identity and live A backpointer. A host provenance gate requires the current candidate heap still match the qualified allocation heap; it never substitutes an old heap. The source occurrence rereads the actual current slot and calls HeapFree with flags0 and that actual H. Host gates are not invented native failure branches.

The fixed10-event success ledger records pointer condition, current slot read, kernel entry/raw return/result test, source-free return, head-null, count-zero, inner cleanup return and proxy argument load. Raw BOOL is retained as int, not normalized. No H fields are dereferenced in this phase. On success, internal headAllocation metadata is retired before native M.head/count stores; all later H identities are historical opaque tokens/receipt values. New free arguments are exposed as uintptr_t tokens, never a dereferenceable H API. A stays allocated and M.proxy is unchanged.

The one-shot started latch is checked before all H validation on every retry, and consumed before entering HeapFree. Success reaches appended SentinelFreeReturned/ProxyFreeUnavailable. Zero reaches SentinelFreeFailurePending/FreeErrorContinuationUnavailable; the API cannot replay or resume the unqualified error branch. It calls neither errno/provider nor GetLastError. Returning to the host **does not preserve or certify same-thread TLS/LastError continuity**; a future error continuation requires a separately qualified provider/error-state contract, not the host's later current errno/LastError. Zero is not labeled native abort or a source-free return.

Old allocation, context-prefix and empty-clear methods are unchanged. Their existing stage/latch guards prevent allocation/reset/context replay after free, before any retired-H access. Earlier sentinel/clear/context receipts remain historical. In particular, the old clear-phase static sentinelFreeEntered=false describes that earlier phase, while the new free report records this later actual occurrence. Full context destructor, C+1450, strings/providers, native EH and accepted Close remain guarded.

## Focused tests and static checks

The runtime test executable now has11 cases (nine retained, one declared-zero case, one post-lease retirement case); context executable has two (one retained, one post-lease case). Existing actual standalone and embedded cases now additionally check premature free refusal, actual HeapFree arguments/raw positive result, exact10-event success order, retirement/head-null/count-zero, retained A/M/C, unchanged allocation/constructor/context receipts, guarded proxy-free boundary, stable repeated free/clear/old API calls and sticky context replay refusal. New separate cases reacquire weak inspection leases after all earlier test-local strong leases have ended, proving the remaining process-retained owner is still available without freeing again.

The actual positive cases REQUIRE a real nonzero kernel result. Environmental zero remains a recorded pending failure and fails that positive prerequisite; it is never replaced by a model success. A separate `declaredFeatureMapSentinelFreeFailure()` returns model-origin metadata only, with five declared events ending at the result test and no actual handle/frame authority. Its test verifies no source-free return, retirement, head/count stores, proxy load, errno or GetLastError. No intentional invalid-handle, corruption, double-free or kernel-OOM test was added. No H field/dereference or post-free heap query is used after positive retirement.

Static preflight passed actual include resolution, Testkit mains and Release-active macros, no C assert/REQUIRE_EQ, enum append compatibility,10/5-event capacities, one kernel free call/no retry, raw result, absence of H dereferences and errno/GetLastError/alloc/proxy-free calls, success-retirement-store order and `git diff --check`. Comparison with CP133 source.zip confirms producer, both preparation factories and old8/48/context/empty-clear method bodies unchanged after newline normalization. No compilation/tests ran here.

Root focused targets/CTest names:

```text
cmake --build <build> --config Release --target m2m_model_feature_crt_runtime m2m_model_feature_context_prefix
ctest --test-dir <build> -R '^m2m\.model\.feature_(crt_runtime|context_prefix)$' --output-on-failure
```

## Frozen exclusive receipt

Paths relative to source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|src/model/feature_crt_runtime.hpp|`b2fb04cb60a05a06188264456075a2b1f2700bc65087b388c011677a6be7ad54`|
|src/model/feature_crt_runtime.cpp|`c02fb82749f1cee9c552d4d2216856893c8cf8021f41cbfb25fd04317eb0a01d`|
|tests/model/test_feature_crt_runtime.cpp|`a52cfc0cc545036b28fbd3cde4d3d7900eacdf07f571b35c6767be1f5ce39c37`|
|tests/model/test_feature_context_prefix.cpp|`a46d9009174b04aa5919527b4d6226926f6866c5d8f3908673673db21bf5c1a5`|

All four files are frozen and released to root. This is a static implementation receipt, not a tested archive or original destructor occurrence. No reconstructed executable/deployment was inspected or changed.
