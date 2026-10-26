# CP139 independent encoded-null role-tail review

## Result

No source-order or ownership blocker found. The tail uses the same saved actual CP137 encoded-null result, writes eight distinct candidate role cells in the retained order, records five setter-leaf returns plus the separate no-op return, then stops before the nonnull callback initializer. It does not claim original-global/FLS/Close completion.

## Owner and gating

`resumeEncodedNullRoleTail` requires the canonical retained `shared_ptr` control block and runtime identity, completed actual heap/module producer, no retained child map frame, and the explicit CP137 first-prefix completion. It validates the preserved private saved argument against both the first-prefix result and first candidate-role cell using knownness/equality, so numeric zero is not rejected. No `EncodePointer` call or caller-provided value is used in the tail. Under the same runtime lock, `started` is latched before tail writes; each role cell is distinct and marked known only as its store occurs. Repeated calls return the retained report without replay; foreign leases return a refusal copy.

The fixed event list matches the table: stores at `1481ca8`, `1470438`, `1481ca0`, `1481ad4`, then four ordered stores at `1474954/95b/962/969`; four one-store leaves and the four-store leaf have normal-return events; `146ff78` is recorded as a separate no-op. Stop is `NonNullCallbackInitializerUnavailable` at `146ea3d -> 147e75c`; the latter's distinct nonnull callback encoder is not entered.

The report and compile-time constants retain `originalGlobalObserved=false`, `hostCrtRolesChanged=false`, no additional encoding, handler invocation, FLS, LastError, CRT-startup or public-Close completion. Role labels are not written as original addresses. There is no default converter/Close callsite driving this continuation.

## Test coverage qualification

The new positive test uses one owned process and checks the exact 14-event ledger, token continuity, role values, repeat/no-replay, old CP137 report stability, and zero-token admission without forcing/fabricating a zero. It checks prerequisite and foreign-control-block refusals. The test does not specifically execute the combination “CP137 first prefix completed, then a child map frame prepared, then the new tail refused.” Existing negative coverage prepares/retains and retires a child map before trying the tail, before CP137 is run. The source guard checks `retainedMapCall` before the first-prefix gate and appears correct; the receipt explicitly discloses the missing combined test. This is a narrow test-coverage gap, not evidence of a source defect or permission to broaden the startup claim.

## Hash binding

The four receipt hashes match current files:

- `src/model/feature_crt_runtime.hpp`: `a9c07b8e044dcb15e27eb13e75e5443e67137c31477942e384cf02947c10c7a3`
- `src/model/feature_crt_runtime.cpp`: `48434b8eeacccb7e36f00ecbd423536b333696d668a18568756f7946f5183a35`
- `tests/model/test_feature_encoded_null_roles.cpp`: `c49b8278069235731368dbe3ae53518a1801bdccd64c9159cf2d586faebcef46`
- `tests/model/test_feature_crt_runtime.cpp`: `8f04e9c4959fc8b44cac970a9f6ad0a8047a4d9b5197d71f92841e82180522a4`

Static review only. No build, test execution, GUI, original application or mutable REA access occurred.
