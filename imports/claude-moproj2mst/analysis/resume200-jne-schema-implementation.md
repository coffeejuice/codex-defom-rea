# CP200 unresolved JNE schema implementation

Implemented the default-disabled, non-executing paired-edge schema against the frozen CP199 CMP/read predecessor. The current boundary remains QtCore RVA `0x2961b`; neither target becomes the cursor and neither edge is selected.

## Source changes

- `doe_task_runtime.hpp`: appended `TlsIndexJneUnresolved` phase outside CP198 ordinals; added immutable published record with exact CMP handle, module/hash qualification, six bytes, length, CMP flags index 3, unresolved status, ordered ZF-set/fallthrough `0x29621` and ZF-clear/taken `0x296c0` alternatives, and occurrence. Added explicit TLS boundary helpers; both TLS phases retain RSP `-0xa8` and boundary `0x2961b`. Existing CMP-first flags and live returns remain unchanged.
- `doe_task_factory.hpp`: added private issuance anchor, shared CMP-history validator, JNE validator/advance, default-false schema switch and preparation-failure switch.
- `task_factory.cpp`: audited/replaced every explicit callee-or-CP199 predicate with the shared boundary helper; admitted both TLS phases at the CMP premature-state gate; required frame and factory JNE handles exactly in the new phase; included issuance in pristine/no-capability checks and scope release. CP199 phase wrapper retains exact-phase authorization. CP200 validates inherited CMP/read/history first, then record occurrence/predecessor/qualification fields and exact object plus control-block ownership. Ordinal-19 dispatch validates terminal CP200 before CP199 advancement; production default continues to stop at CP199.

JNE preparation constructs the occurrence, immutable record and terminal Error locally. Failure injection runs immediately before three nonthrowing publication assignments. No engine/root issuer change or additional read/flags/register/stack/private/shared mutation was added. This is exception-atomic preparation/publication in the serialized adapter model; throwing/transporting a terminal diagnostic after publication does not promise rollback.

## Tests and snapshots

Extended factory source snapshots and frame-value snapshots with JNE handles and every JNE field. `Cp180Snapshot` also verifies factory/frame JNE ownership and predecessor control-block ownership. The privileged CP199 rewind helper now deliberately clears the new JNE anchors; a separate test-only CP200 predecessor reconstruction permits comparing the complete CP199 snapshot after publication without reissuing CMP/read.

Five new Testkit cases cover:

1. Default CP199 stop; exact pair/static target arithmetic; same CMP/read and full unknown domain; phase, RSP, flags, live return generations; terminal reentry under enabled/disabled schema and every callee budget; complete predecessor-state preservation.
2. Every CP198 callee interruption (0 through 10), CP199 resume, after-allocation preparation failure twice with exact snapshot/control preservation, and successful retry.
3. Thirty-one malformed/copy/foreign-control-block/predecessor/module/byte/length/flag/status/guard/target/occurrence, foreign read/root grant/frame/call/history, wrong RSP, partial and coherent public rewind cases, each comparing the complete pre-refusal snapshot.
4. Factory/grant and allocation-scope release with retained diagnostic pair and mutation-free refusal.
5. Ordinary Session driver reaching CP199 with schema enabled, preparation failure/retry, independent-fixture semantic pair comparison, and retained-record refusal after root/Session expiry.

Compile-time concepts establish absence of `selectedEdge`, `concreteValue`, and `concreteZf`; the published handle has a const element type. Existing CP199 tests remain present. The type is publicly copyable, but copies have no private issuance authority and exact owner validation refuses them, as prescribed by the design.

## Verification and limitations

Static preflight passed for appended phase/range helpers, fixed cursor/RSP, default-disabled switch, one unchanged root-read commit site, and five Testkit registrations. A focused `cmake --build build/candidate --parallel 4 --target m2m_mdt_doe_task_factory` attempt stopped before source diagnostics because the shell lacked MSVC INCLUDE (`cstddef`, `cstdint`, `functional` unavailable). A subsequent environment-wrapper command failed shell quoting before any build. Root requested no further retries and owns initialized-environment build, focused execution, full regression, review, and freeze. No test pass or source compile success is claimed here.

Representative earlier constructor-cut semantics rely on existing retained fixtures plus the unchanged common validators; the new dedicated interruption loop exhausts callee cuts only. Post-publication exception-transport failure is not injected. No new runtime branch feasibility, target execution, TLS allocation, once-state read, getter/epilogue/return, QObject completion, or SessionTask publication is supported or claimed.

## Ownership release

Released these four source/test files under `source/apps/moproj2mst/` and this report: `include/m2m/doe_task_runtime.hpp`, `include/m2m/doe_task_factory.hpp`, `src/doe/task_factory.cpp`, `tests/mdt/test_doe_task_factory.cpp`. No other source, engine TLS issuer, CMake, native evidence, GUI, REA, installed application, parent DMO, or Claude file was edited. No deployment was performed.

## Focused-test repair after root build

Root's initialized-environment build compiled successfully; focused task-factory execution reported 176/177 passing, with `doe_task_factory_cp200_retained_records_cannot_restore_released_scope_authority` failing snapshot comparisons. Inspection of `build/logs/resume200-jne-schema-focused-ctest-01-03.log` and `TaskAllocationStore::releaseAdapterScope()` established a test lifetime defect: allocation release clears `prototypes_`, destroying `fixture.pending`, while the test subsequently constructed and checked `Cp180Snapshot` through that dangling pointer. This was undefined behavior, not evidence of a production refusal mutation.

Repaired only the test. Factory-only release retains the frame and still uses the complete `Cp180Snapshot`. Allocation-store release now snapshots all surviving factory construction/issuance, root symbol/grant/read state, retained JNE/CMP/read qualification values and JNE issuance ownership, and requires the released store to stay inactive with zero prototypes/tasks/sessions across repeated refusals. It never dereferences the destroyed frame. No valid refusal invariant was removed: all state that exists after release remains compared. Root owns rebuilding and rerunning the repaired test; this repair ran no build or executable test. Test-file ownership released again.

## Acceptance-coverage extension after independent review

Addressed both test-evidence gaps in `resume200-jne-schema-source-review.md`, with tests only. No production field, value, branch observation, or target execution was added.

`cp200SemanticEqual` compares all JNE qualification fields, CMP width/RVA/immediate/sign extension/result/arithmetic schema, read identity/domain/context ordinal/instruction, all eight callee stack-slot scalar descriptions and token roles, both SUB histories/current flags, nested and outer calls, live return slots, saved RBX/RDI and initializer-call qualifications, source identity, historical private flags expression, caller flags/parent TEST/branch history, phase/RSP/current boundary and current-flags kind. Per-run opaque occurrences and allocation addresses are intentionally excluded from semantic equality. `cp200ExactAuthority` separately requires exact object/control-block relationships for the factory JNE, predecessor CMP, factory CMP, root read, and read root/thread/grant/symbol/frame/call contexts.

Every callee interruption (0 through 10) now compares its terminal semantic state with a baseline, checks exact within-run authority, and compares the full accepted CP199 snapshot after projecting only the JNE diagnostic boundary back through privileged test access. The exact same JNE handle is restored afterward and terminal reentry must preserve its full snapshot, with no new issuance. This projection is fixture inspection, never a production rewind API. Seven earlier constructor cuts (flags ordinals 0, 4, 8, 12, 16, 17, 18) now perform the same semantic/ownership/full-predecessor/reentry checks. The ordinary Session driver comparison also invokes the full semantic comparator.

Twelve new retained-JNE refusal variants independently exercise a foreign root, an expired replacement root weak owner, a foreign root thread, a missing current thread, missing and foreign factory capability, a foreign grant installed in both root and factory, replacement grants with expired thread or capability links, independent live-grant revocation, original JNE replay through a second actual Session/factory, and root-only grant removal. Both Sessions' complete observable snapshots, the authority-owner state, and retained JNE/read values must remain unchanged across repeated refusals. Cases retain the original accepted JNE throughout.

Ownership qualification: actual root expiry while an intact Session remains live is unavailable through production ownership: `ConversionSession::widgetRuntime_` owns the root strongly and Session destruction releases the factory adapter. The root owns its execution thread strongly (`WidgetRuntimeState::thread_`); resetting a local thread handle cannot expire it. The accepted frame/source binding retains the Session capability strongly (`SessionReportNullBinding::capability`), so clearing only the factory capability cannot expire the same capability while that diagnostic frame survives. The new tests therefore exercise available foreign/missing-owner substitutions, an expired replacement root weak owner while the adapter remains live, and invalid immutable grant replacements with expired weak thread/capability links. They do not claim independently destroying the original intact Session root/thread/capability. The existing post-Session test remains a joint lifecycle expiry test. Independent `revokeTlsGrant` now proves loss of grant authority without releasing the adapter or allocation store.

These additions bring dedicated CP200 Testkit registrations to seven. Static inspection confirms all eleven callee cuts invoke semantic and exact-authority checks, the seven earlier cuts invoke the same checks, and all twelve authority variants perform repeated complete pre-refusal comparisons. No compile, build or executable test ran in this extension lane; root owns validation and the updated receipt. Only the assigned test file and this report changed. Test-file ownership released.
