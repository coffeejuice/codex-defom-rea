# CP108 explicit isolated conversion-worker role

The approved supervisor/header and new focused role test are frozen for root integration. This adds an explicit WorkerRole selector and finite conversion cases while preserving SelfTest defaults, v1/v2 protocols, ownership, deadline and preparation lease behavior. No core/convert, editor300/remesh service, product CLI, C ABI or other worker-test file was edited.

## Exact consumer contract

`WorkerRole { SelfTest, RemeshConversion }` is declared in the private supervisor header. Options appends `workerRole = WorkerRole::SelfTest`; it does not repurpose a protocol profile as execution authority. ChildCase0..16 remain unchanged. Appended values are:

| Value | ChildCase |
| ---: | --- |
|17|ConversionNormal|
|18|ConversionDenied|
|19|ConversionDeniedProjectFalse|
|20|ConversionDeniedMstFalse|
|21|ConversionUnknown|
|22|ConversionCustom|
|23|ConversionPublicationFailure|
|24|ConversionEndpointReturned|

`validWorkerSelection(role,profile,case) noexcept` permits RemeshConversion only with RemeshV2 and one of17..24. SelfTest rejects all conversion cases and unknown values. It also rejects previously unsupported self-test combinations before spawn: SafetyV1+PreparedFileProbe, and RemeshV2+Terminal/TerminalWithoutBoundary/OutputFlood/SmallOutput. Existing passing v1/v2 cases retain their route.

The supervisor validates selection immediately after explicit parent opt-in, before hashing/preparation/directory creation or process spawn. Its launch copy remains immutable against preparation callback mutation. Own-image/hash checks, suspended job assignment, restricted inheritance, protocol deadline/capture limits, sticky failures and held preparation/inspection lease remain in place.

Exact conversion-role argc7:

| argv | Value |
| --- | --- |
|0|absolute own test executable|
|1|`--terminal-remesh-conversion-worker`|
|2|`--allow-owned-worker-termination`|
|3|unsigned decimal conversion case17..24|
|4|protocol write HANDLE as uintptr decimal|
|5|nonzero uint64 nonce decimal|
|6|unrelated HANDLE uintptr decimal, normally0|

Existing self-test command tokens are unchanged. Supplied childMain still accepts only those old roles and cases0..16; the separate conversion worker main must handle the new token before falling back to childMain. This lane does not implement the actual conversion service consumer. The exact enum/argv/header contract was sent to its assigned owner and root before implementation proceeded.

## Focused validation and limits

New target `m2m_mdt_terminal_harness_worker_role`, CTest `m2m.mdt.terminal_harness_worker_role`, contains5 Release-active cases: defaults/finite role-profile matrix; rejection before preparation or spawn; actual supervisor-to-owned-child argv/nonce/PID transport; explicit opt-in still required; and old childMain rejecting the conversion role/case.

The positive child is deliberately a role transport echo. It validates all7 arguments, requires a prepared ROLE-TRANSPORT-ONLY fixture, emits v2 Started then Returned0 and never invokes conversion, remesh service, terminal endpoint or normal Close. It checks preparation/inspection/destruction and binary identity, without claiming source-bound terminal behavior. Invalid selection tests prove the callback and child never start. Unknown enum/case values and legacy self-test misuse fail closed.

All fixture/capture/diagnostic paths remain under checkout build scratch. A bounded aggregate `terminal-role-last-run.txt` records expected process outcomes and hashes; successful exclusive case roots are removed after canonical containment checks, failures preserve captures. The role tests neither load the original binary nor modify installation/session state.

Static `admin/check_test_source_preflight.py` passed for the new test. Quoted includes resolve, supported CHECK/CHECK_EQ/REQUIRE macros and Testkit main are present, and delimiter/whitespace checks passed. The v1 safety and v2 control test files and protocol files were not changed. No compiler/build/Testkit child/GUI/original target/REA execution occurred. Root owns focused runtime validation, full required tests and freeze; dynamic command transport and compatibility remain unverified by this worker until those receipts exist.

Frozen SHA-256 values relative to `source/apps/moproj2mst`:

| File | SHA-256 |
| --- | --- |
| `src/engine/terminal_harness_supervisor.hpp` | `186c3e7bfb531fae8e511babbfff959a3e492734802af036b03d1a2f6730bce3` |
| `src/engine/terminal_harness_supervisor.cpp` | `20a18180ba4327b1a6493a3ec19377566104978fdd0d31484efc62df791a8b3b` |
| `tests/mdt/test_terminal_harness_worker_role.cpp` | `8c4325ded5d9764a2a4f5c8fb12c3430053392bee55974be48fdd21f2bb6f115` |
