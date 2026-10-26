# CP137: actual encoded-null handler producer prefix

Implemented explicit `FeatureCrtRuntime::resumeEncodedNewHandlerPrefix(canonicalLease)` in the existing candidate process/module owner. After its real successful heap producer and before any child-map preparation, it makes one actual EncodePointer(nullptr) call, stores the returned opaque value in the canonical candidate handler role, records readback and stops before146ea0d's next initializer. It never changes the host CRT handler or admits FLS/PTD/LastError/Project/Close work.

The source contract was written before source edits: `analysis/resume137-feature-new-handler-contract.md` SHA-256 `587d305a62db3ed2325a79918f5d372bbd7b10a697db1597922e93d32eb41d47`. CP136 evidence is `analysis/resume136-feature-error-owner-next.md` SHA `da58f8912165bbe0e105e84053447bdbac0f76227c0a249e5780f99730eb0e6f`. It qualifies original DEF_GUI_MO_64.exe v11.0.2 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`:1470cce ->1481708 ->146e9f4 ->14813e0 EncodePointer(null), then146ea05 ->1481ce0 store to source role1447006e0, then next initializer146ea0d ->1481ca8. This is candidate source-role execution, never original-global observation or complete CRT startup.

Only reserved runtime hpp/cpp, existing runtime test, NEW test_feature_new_handler_prefix.cpp and analysis files were edited. No public model/Project/Session/CMake/other source edits, compilation/tests, kernel/GUI/original execution or mutable REA database in this lane. CP136 archive/receipt was present and its stable-source verification checked before release. Root owns actual execution/freeze.

## Private state and ordering

Ordinary owner construction leaves handler-role knowledge unavailable; backing numeric zero is not a null-default event. Canonical lease/control-block validation precedes effects. Absent/wrong lease returns a refusal copy, preserving canonical history. The actual produced heap/module and absence of a retained child frame are required; an old map profile cannot insert startup retrospectively, even after its resources were freed.

One-shot count is consumed before actual EncodePointer. Its raw pointer result is preserved as an opaque uintptr_t token, with no nonzero predicate. The source phase has four events: actual encoding entry14813e2, return146e9ff, candidate role store1481ce0 and first setter return1481ce7. Readback uses the actual separately stored canonical candidate role. Repeated calls do not encode/store again. Old producer/map/cleanup APIs do not auto-drive this phase.

No production DecodePointer, handler callback, next initializer, lock/FLS/PTD setup, LastError/errno, original-address read or host CRT handler mutation occurs. The encoded role's later current-state/handler-invocation contract remains unbound; this prefix cannot bypass allocator failure/new-mode/error guards.

## Focused tests and static preflight

New separate-process target `m2m_model_feature_new_handler_prefix` has two Release-active Testkit cases: absent/foreign canonical leases; no implicit heap production; real fresh owner/result/store/readback and exact four-event ledger; test-only DecodePointer round trip to null; retry/lease stability; unchanged heap receipt; and wrong-lease refusal preserving canonical history. Zero-valued encodings are allowed. The test's decode occurs after the source prefix and is not production behavior. Existing runtime target remains12 cases and now checks late startup refusal after its actual isolated map lifetime. One-map capacity and context tests are unchanged.

Static preflight passed real includes/Testkit main/macros, no C assert/REQUIRE_EQ, canonical/fresh-profile/one-shot order, one actual EncodePointer and one role store, four-event capacity, no unrelated kernel/allocator/error/handler calls in the phase, and `git diff --check`. CP136 source.zip comparison confirms all old producer/factory/8/48/context/clear/sentinel/proxy-free bodies unchanged after newline normalization. No compilation or tests ran here. Existing CMake test glob supplies the new target; no manual registration was added.

Root focused validation:

```text
cmake --build <build> --config Release --target m2m_model_feature_crt_runtime m2m_model_feature_new_handler_prefix m2m_model_feature_context_prefix
ctest --test-dir <build> -R '^m2m\.model\.feature_(crt_runtime|new_handler_prefix|context_prefix)$' --output-on-failure
```

## Frozen receipt

Paths relative to source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|src/model/feature_crt_runtime.hpp|`d9729af5495cbed9fc1daf89c7db1102dcf04a90034754358cc545d94a4d321c`|
|src/model/feature_crt_runtime.cpp|`0c200c440699cf355b6289f7e010e35386c4413eb2941739e8f637ed7c955191`|
|tests/model/test_feature_crt_runtime.cpp|`53b9838b6a68f3993bbc586cdb1a6430bfcaeaa50733cf767b06ec6a88ee00d1`|
|tests/model/test_feature_new_handler_prefix.cpp|`e8a796ca69e11cb8dafec438710711bc3f81dcda840c91408834b18acd10ea7a`|

All four files are frozen and released. Next initializer, complete thread startup, FLS callbacks/indices, failed-free TLS/LastError continuity, native context lifetime and public Close remain guarded. This is static preflight, not a tested archive or original runtime witness. No reconstructed executable/deployment was inspected or changed.
