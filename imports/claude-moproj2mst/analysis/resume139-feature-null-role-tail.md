# CP139: bounded encoded-null role continuation

The explicit private runtime continuation now stores the same actual CP137 encoded-null token into eight distinct candidate roles across five pure leaves, then records the no-op return. It stops before caller14146ea3d ->14147e75c, which initializes a nonnull callback. No default converter callsite drives this method; public Close remains guarded.

Qualification is `analysis/resume139-feature-null-role-tail-next.md`, SHA-256 `7fb5aa436b49ac57e3da3f76b3022484d4017d5a5161992c8f13c78405dfcf03`. Original DEF_GUI_MO_64.exe v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Its bounded byte ranges and source-role table qualify these stores; no original executable, GUI or mutable REA session was used in this implementation. No reconstructed executable/deployment was inspected or changed.

## Actual private owner and order

`FeatureCrtRuntime::resumeEncodedNullRoleTail(canonicalLease)` requires the canonical shared-owner/control-block identity, actual heap/module producer completion, absence of a retained child frame and completed explicit CP137 first-role prefix. The CP137 path now preserves its actual encoding as the source146ea02 saved argument; this is private frame metadata, not another global store or an external caller-supplied token. Its prior four-event report is unchanged. Numeric zero remains admissible: readiness uses knownness and source progress, never token nonzeroness.

Under the same owner lock, the continuation checks that saved argument against the private first result and canonical first-role cell. It consumes its one-shot cursor before the first tail store. Each store writes a separate canonical candidate cell and records its readback/knownness. The native store/role order is:

| Store VA | Original role label |
| --- | --- |
|141481ca8|1447006d8|
|141470438|1446ffb40|
|141481ca0|1447006d0|
|141481ad4|1447006c0|
|141474954|1446ffb68|
|14147495b|1446ffb70|
|141474962|1446ffb78|
|141474969|1446ffb80|

The fixed14-event ledger interleaves the first four setter returns at141481caf,14147043f,141481ca7,141481adb, then the four-store leaf return141474970, then no-op return14146ff78. Returns certify only completion, without inventing a return-register value. Original addresses are labels, never writable candidate addresses. Repeats return canonical history without replay; foreign leases return a refusal copy without mutating canonical state. After the cursor starts, old history remains available even if a later isolated child lifetime occurs; it does not assert current native CRT role authority.

No new encoding/decoding, host CRT role mutation, callback invocation, FLS/PTD, LastError/errno, complete CRT startup, ContextEnvelope destruction or Project/Close completion is admitted. The next nonnull callback147e714/PTD/termination owner is still unavailable; replacing it with an original VA, null or a generic host callback would be unsupported. The earlier shorthand that every subsequent initializer was a null setter is corrected in the qualification report:146ff78 is a no-op and147e75c encodes a distinct nonnull callback.

## Focused verification

New target `m2m_model_feature_encoded_null_roles` has two Release-active Testkit cases: explicit prerequisite refusal and no implicit producer/encoding; absent and foreign-control-block leases; exact14-event golden including all eight distinct roles; same actual CP137 token and preserved first report; retry/no-replay; unchanged heap receipt; and retained owner/token/cells after inspection leases expire. The test accepts an actual numeric-zero result without fabricating one. Existing `m2m_model_feature_crt_runtime` remains12 cases. After independent review, its actual map case now explicitly completes CP137 on its actual canonical fresh heap owner before its existing child-map preparation. It then checks tail refusal immediately after preparation and before any map/sentinel allocation, with every role unknown, no effects and the first-role receipt preserved. After its existing valid map teardown it checks the same tail refusal again, with no retired pointer reads. Calling the old first-prefix API at that later point returns its already-completed history without replay; this profile no longer tests a never-started first-prefix refusal.

Static preflight passed: quoted includes resolve against source/target directories; Testkit entry points and supported macros exist; no C assert/REQUIRE_EQ; eight-store/five-setter-return/no-op source tuple order; fixed14-event capacity; canonical/fresh/prerequisite/one-shot ordering; and no encoding, allocation, free, FLS or LastError call in the new phase. Comparison against frozen CP138 source.zip checked12 old noexcept member bodies; they are identical except the two explicit saved-argument metadata lines in CP137. All owned files passed trailing-whitespace checks. No compilation or test execution was performed. Existing CMake globs supply the new target; no CMake/interface/public source edits.

Root focused targets: `m2m_model_feature_encoded_null_roles`, `m2m_model_feature_new_handler_prefix`, `m2m_model_feature_crt_runtime`, `m2m_model_feature_context_prefix`. CTest names use dots in place of the first two underscores, e.g. `m2m.model.feature_encoded_null_roles`. Root owns build/full validation/freeze.

The review repair changed only the existing runtime test and this receipt. Repeated static Testkit/include/macro/12-case/order checks and `git diff --no-index --check` passed; no build or test was rerun. The existing root focused-failure log `build/logs/resume139-focused-ctest.log` is preserved unchanged (SHA-256 `00286e4ed7786f2145928eb332490c9678a5a6a148ddae5e6f3731c7a6095f6d`); this test-only repair does not resolve or reinterpret its unrelated SPRING2 failure.

## Frozen source receipt

CP138 `build/wave3-checkpoint138-verified/receipt.json` existed before edits. Owned source is released for root's grouped build; hold subsequent edits until root freezes that candidate.

| File beneath source/apps/moproj2mst | SHA-256 |
| --- | --- |
|src/model/feature_crt_runtime.hpp|`a9c07b8e044dcb15e27eb13e75e5443e67137c31477942e384cf02947c10c7a3`|
|src/model/feature_crt_runtime.cpp|`48434b8eeacccb7e36f00ecbd423536b333696d668a18568756f7946f5183a35`|
|tests/model/test_feature_encoded_null_roles.cpp|`c49b8278069235731368dbe3ae53518a1801bdccd64c9159cf2d586faebcef46`|
|tests/model/test_feature_crt_runtime.cpp|`b02ec32e48d002a7919febe0697b5a90565c4cab3a889bf2ea816f6fffd5a29a`|
