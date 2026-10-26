# CP133: opt-in actual empty-map clear prefix

Implemented `FeatureMapProxyCall::resumeEmptyMapCleanupPrefix()` on the actual retained standalone or embedded CP130/132 map. It executes the source-closed empty full-range clear, records the iterator return, and stops before sentinel free1412f08b0. Exact map reset order is **H.link8=H, M.count=0, H.link0=H, H.link10=H**. The same M/A/H and optional C remain retained; no destructor hook, free, provider, Project or public Close admission was added.

Only the four reserved existing runtime header/cpp and two tests plus this receipt were edited. No model.hpp, Project, Session, CMake, plan or other source lane changes. No compilation/tests, kernel/GUI/original execution or mutable REA database in this lane. Root owns focused/grouped execution and source-stable freeze.

## Source qualification and ownership

`analysis/resume131-feature-map-destructor-next.md` SHA-256 `d59388913ba3ca45396e87c8e10e0ff2a1c488e7b6e29f320b15b62fd2b6371a` qualifies immutable original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Native chain12f0cf0 ->12f0860 ->12f06b0 selects full-range clear before generic equal-iterator handling.12ee410's empty root.nil branch skips node traversal/free, then performs the reset stores above. The implementation preserves14 ordered source-role events through12f08ac's sentinel-free argument load, stopping before its call. It reads no key/payload/suffix or C+1450 field.

Admission uses the real weak-borrowed canonical runtime and owner-before-call lock order, completed actual map constructor, known head/count/nil, matching actual A backpointer/M/H allocation identities, and initialized empty shape. These extra host admission checks are not represented as native count/shape branches. Premature or unqualified states return a guard before source effects. No caller-provided pointer, report or fake handle can create the frame or authorize writes.

The source phase reads actual H.link8/nil, records full-range/empty-root selection, reloads actual M.head before each link-reset group, and stores count between link8 and link0. It then constructs a local output iterator **node first, owner second**, matching12f0721/72a, and loads the sentinel-free argument at12f08ac. No free call, head-null/count-tail store, proxy-null store, allocation or callback is performed. Constructor/sentinel/context receipts remain historical observations, not replayed fields.

## Cursor and context exclusion

The nested FeatureMapEmptyClearReport has a fixed14-event ledger and a one-way started latch. After source admission, the frame reaches appended EmptyMapClearReturned with SentinelFreeUnavailable. Repeated clear resumes return the same receipt; old8/48-byte APIs cannot allocate or replay after that stage.

`resumeContextPrefix()` checks the clear latch before standalone/context-owner checks and before its prior prefix-complete fast return. It reports MapCleanupStarted while the main cursor stays at the sentinel-free guard. This prevents both new context-prefix adoption and replay after clear. Already recorded55 context stores remain inspectable as history. The prefix does not require or read the unknown optional context owner and is never labeled full context destruction. Guards are not native throws/EH; no automatic cleanup runs on lease drops or process-retained metadata destruction.

## Focused tests and static preflight

Extended the existing actual standalone constructor test (runtime target remains nine cases) and the existing separate embedded-context test (one case). They cover premature clear with no effects/allocation; actual empty root/owner/head identities; exact14-PC/effect ledger; link8/count/link0/link10 order and actual reset reloads; node/owner iterator values; retained head/free argument; unchanged allocation/constructor receipts; unknown payload/optional fields; stable repeated clear; blocked old allocation APIs; standalone context adoption refusal; and embedded context replay refusal with its55 historical stores intact. Existing one-frame-per-process capacity and separate executables remain unchanged. No fabricated nonempty pointer or corrupted heap fixture was introduced.

Static preflight passed real quoted-include resolution, Testkit main and supported Release-active macros, no C assert/REQUIRE_EQ, appended enum order,14-event capacity, actual write/output ordering, no allocation/free/kernel call in the new phase, early context latch placement and `git diff --check`. CP132's archived source comparison confirms producer,8/48-byte resumes and both preparation factory bodies unchanged after normalizing archive/current newline encodings. No compile/test result is claimed.

Root focused targets: `m2m_model_feature_crt_runtime` and `m2m_model_feature_context_prefix`; CTest names `m2m.model.feature_crt_runtime` and `m2m.model.feature_context_prefix`.

```text
cmake --build <build> --config Release --target m2m_model_feature_crt_runtime m2m_model_feature_context_prefix
ctest --test-dir <build> -R '^m2m\.model\.feature_(crt_runtime|context_prefix)$' --output-on-failure
```

## Frozen receipt

Paths relative to source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|src/model/feature_crt_runtime.hpp|`1164febe2b9befdeb49c39bde91ed5fc52c2f4fef50fc487a9d1c09604838a73`|
|src/model/feature_crt_runtime.cpp|`7f26fa734a47931b29e8829133e8e1e3ac4b0d4c038a36f9b46a5b77ccfc00f3`|
|tests/model/test_feature_crt_runtime.cpp|`591f5d0b4f2e5cc0897ed34266e7af7ae26a18033c7a2532db30bdadebc19435`|
|tests/model/test_feature_context_prefix.cpp|`153c8f94cd252148451288b3ec27a59bc003a11703faa4996b1dd2ded893864b`|

All four files are frozen and released to root. Next source effect is actual sentinel free through14dfc68/1471b7c with its current CRT-heap/HeapFree/error-path contract; no success or release is inferred from this guard. Enclosing C constructor strings/optional owner, provider teardown, Project admission, native EH and accepted Close's application/main/busy/modal/manager predecessors remain independent. This is a static source receipt, not a tested candidate archive. No reconstructed executable/deployment was inspected or changed.
