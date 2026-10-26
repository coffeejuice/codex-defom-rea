# CP137 independent encoded new-handler prefix review

No blocking discrepancy found in the frozen private producer source/test. The API executes one actual EncodePointer(nullptr), publishes its opaque result to the canonical candidate role, and stops before the next initializer. It neither resets startup after a child lifetime nor supplies the missing failure/Close owners.

## Source qualification and execution order

Reviewed the new method/report, runtime storage/constructor, canonical lease matcher, child factories, two new tests, late-profile regression and both evidence/implementation receipts. Independent PE section reads reproduce the contract bytes/hashes for `[1414813e0,1414813e9)` `ddca6c63787621d57c9502905dce7225addfc84bb0508f6acdd3b30918b6e258`, `[14146e9f4,14146ea0d)` `2de82ca3b6011537f7db3ef428c2a5028f453f1bbf9f0aef5317d3ef1afeb854`, and `[141481ce0,141481ce8)` `077a491a73df9b5ebf668dd0f818bbfeb3673d40f153a15157171d52ee24e54a`. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The first wrapper explicitly zeros ECX before EncodePointer; the caller preserves its actual returned token and passes it to the one-store leaf, which writes the encoded new-handler role and returns. The candidate mirrors encoding-before-store and guards before `14146ea0d ->141481ca8`. Its four-event capacity matches entry, returned value, role store and first-setter return. There is no DecodePointer, handler invocation or next-initializer production call.

Ordinary storage backing starts at numeric zero but candidateRoleKnown remains false until the explicit store. The method authenticates the canonical shared control block under the runtime lock, then requires the actual successful pinned-module heap producer and no retained child-map frame. Wrong/absent leases return a refusal copy without changing canonical history. The one-shot count is consumed before EncodePointer; no nonzero predicate is imposed on its actual result. The actual separately stored candidate role supplies readback. Retry after completion returns history without another encoding/store; first invocation after any retained child preparation refuses even after A/H retirement because the frame remains retained.

## Ownership, regression and limits

The runtime and module remain deliberately process-retained. No default Project/session binding, host CRT handler mutation, original-global read, FLS/PTD/lock startup, LastError/errno capture, HeapDestroy or Close call was introduced. Public completeness flags remain false. New private APIs are referenced only by their implementation/header and the two focused test files. Earlier map/failure APIs do not drive this phase or gain failure-continuation authority.

The fresh separate-process test covers absent and same-pointer foreign-control-block leases, no implicit heap production, premature refusal, actual encoding/store/readback, exact four-event ledger, test-only DecodePointer null round trip, unchanged heap counters/history, retry and foreign-lease refusal after success. The second case drops local inspection ownership and checks retained role/counters. The existing runtime test verifies first invocation after an actual completed map lifetime refuses with zero encoding/store events and does not revive or alter the retired map. Both numerical zero and nonzero encoded-null results are admitted. Tests use Release-active Testkit checks with one main per executable. No required regression addition identified.

These tests are candidate Win32 source-role tests, not evidence of original startup execution or complete CRT initialization. Full FLS/error-state continuity, new-mode/error handling, native enclosing context construction/destruction and Project/public Close remain guarded, as the receipt states.

## Frozen binding

All four hashes match implementation receipt `analysis/resume137-feature-new-handler-prefix.md`, SHA-256 `1655c51fcecb52c14a3a7fac1d9738343d655d54437bae905eb94e10dd2c289d`:

- Runtime header: `d9729af5495cbed9fc1daf89c7db1102dcf04a90034754358cc545d94a4d321c`.
- Runtime implementation: `0c200c440699cf355b6289f7e010e35386c4413eb2941739e8f637ed7c955191`.
- Existing runtime test: `53b9838b6a68f3993bbc586cdb1a6430bfcaeaa50733cf767b06ec6a88ee00d1`.
- New prefix test: `e8a796ca69e11cb8dafec438710711bc3f81dcda840c91408834b18acd10ea7a`.

Prior contract hash `587d305a62db3ed2325a79918f5d372bbd7b10a697db1597922e93d32eb41d47` and CP136 evidence hash `da58f8912165bbe0e105e84053447bdbac0f76227c0a249e5780f99730eb0e6f` also match. Root owns focused compilation/execution. Only this report was written; no source/test edits, build/test run, GUI/original execution or mutable REA actions were performed.
