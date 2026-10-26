# CP135: isolated proxy free and map-destructor return

Implemented explicit `FeatureMapProxyCall::resumeProxyFreePrefix()` after real successful sentinel free. It attempts one actual HeapFree(current canonical candidate CRT heap,0,A). Nonzero completes the standalone12f0cf0 destructor's tail return; zero parks before errno-provider1471b9c. **M.proxy is never nulled**, matching native code. A/H and the isolated source-map lifetime are retired on success; remaining identities are host backing or opaque history, not source capabilities. C/Project/Close, native EH and HeapDestroy remain guarded.

Only the reserved runtime header/cpp, two existing tests and this new receipt were edited. CP134's frozen receipt/archive was present before edits (source.zip SHA-256 `e422fd0ef6ead95a453031a044ea0589423e1794c5073d812088634b8649cbae`). No model.hpp, Project, Session, CMake, plan, build/test/kernel/GUI/original execution or mutable REA database in this lane. Root owns compilation/execution/freeze.

## Source and lifetime contract

Read-only receipt `analysis/resume135-feature-proxy-free-next.md` SHA-256 `c5548eb7450556dc2324cf5c7c9665c3770743f10ff0956adb29557c747eda47` qualifies immutable original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`:12f0d0c loads A;12f0d14 tail-jumps14dfc68 ->1471b7c; actual nonnull uses current heap144700810, flags0 and HeapFree at1471b92. A nonzero result returns1471bb8 directly. Because the map destructor tail-jumped, that single RET also completes12f0cf0; no invented second native return or M.proxy-clear store is added.

The new function has no external pointer/handle/report parameter. Under owner-before-call locks, it requires the same canonical retained runtime, real sentinel-success/retirement/inner-cleanup return, actual M.head=null/count0, and matching still-live A/backpointer/source argument. Current heap provenance is checked against the actual8-byte proxy allocation heap, then reread for this new source occurrence; no old/unrelated heap is substituted.

Its one-shot latch precedes every A/H/M-source validation on retries and is consumed before the kernel call. New report identities/arguments are uintptr_t tokens. A's backpointer is read only while A is still live. After HeapFree returns, the function reads/writes only host metadata: actual nonzero records source-free return, A retirement and isolated map-destructor return; it performs no M/A/H field access, M.proxy null store, context callback or resource release beyond this one free. Native stale pointer bits remain in passive host M backing and cannot be used by a later source admission. Historical constructor/sentinel/clear receipts remain unchanged, with header comments explicitly marking retired identities as history.

Raw BOOL is preserved as int. Zero latches ProxyFreeFailurePending/FreeErrorContinuationUnavailable before1471b9c, without A retirement or destructor-return claim and without errno/GetLastError. Host-return TLS/LastError continuity is unqualified; no error resume or host CRT fallback is supplied. Declared failure metadata is separate from real frame authority.

Old8/48/context/clear/sentinel-free methods are unchanged. Their early cursor/latch exits prevent allocation, reset, context adoption/replay or retired-block validation after either proxy result. Success reaches MapDestructorReturned/EnclosingLifecycleUnavailable. Passive C/M backing, runtime/module heap and historical reports remain retained until the owned process ends; full native context construction/destruction and Project provenance remain unclaimed.

## Focused tests and static preflight

Runtime target now has12 Testkit cases (11 retained plus declared proxy-zero case); context target remains two. Existing actual standalone/embedded cases now additionally require actual nonzero A free, verify canonical current heap/A/flags0/raw result, exact six-event success ledger and one tail return, opaque pre-retirement tokens, no proxy-null store, unchanged allocation/sentinel/context histories, one H free/one A free, repeated free/old API no replay and sticky context exclusion. Premature proxy free does no kernel work. Post-lease cases now reacquire the retained retired-map history after all earlier inspection leases end and verify no second free or live-pointer admission.

The separate model-origin proxy-zero case has five events ending at result test, no actual addresses/handle/frame capability, no retirement/tail return and no errno/GetLastError. Real positive tests fail their prerequisite if the actual kernel returns zero; they do not replace it with modeled success. No invalid handles, corrupted blocks, forced OOM, double-free, post-free field dereference or heap query is introduced.

Static preflight passed real includes, Testkit mains/macros, no C assert/REQUIRE_EQ, enum append compatibility, six/five-event capacity, one-shot-before-validation, raw result, no M.proxy assignment and no M/A/H source-field access after HeapFree. No allocation, HeapDestroy or error-provider calls occur in the new phase. `git diff --check` passed. Comparison with CP134 archive confirms old producer/factories/8/48/context/clear/sentinel-free bodies unchanged after newline normalization. No compilation/tests ran here.

Root focused targets/CTest names remain:

```text
cmake --build <build> --config Release --target m2m_model_feature_crt_runtime m2m_model_feature_context_prefix
ctest --test-dir <build> -R '^m2m\.model\.feature_(crt_runtime|context_prefix)$' --output-on-failure
```

## Frozen exclusive receipt

Paths relative to source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|src/model/feature_crt_runtime.hpp|`5f09cddc39cff686024d1e1ceaa434eff564f73391b4b3a7654212659726e33e`|
|src/model/feature_crt_runtime.cpp|`3a3e4ccc7254ba38af6382237c00bd882c90a168c37b31e031a24e269291f85a`|
|tests/model/test_feature_crt_runtime.cpp|`59f286cdf735a4c4c1dd3b44567f169856b407777018be7095672c3cb167c9fc`|
|tests/model/test_feature_context_prefix.cpp|`7bb1e4ac45eeb0ad8475c4d76fd92a24be7468192d585676c4fe510787151e48`|

All four files are frozen and released. The remaining failure branch needs actual CRT FLS/PTD/error-state ownership; a previous paused failure cannot adopt a later host LastError. Enclosing C strings/optional-owner/provider state, native exception occurrence, Project and accepted Close root remain separate. This is a static implementation receipt, not a tested archive or original destructor observation. No reconstructed executable/deployment was inspected or changed.
