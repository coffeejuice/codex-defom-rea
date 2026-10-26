# CP180 connector independent batch plan review

## Decision

**One CP180 implementation and one accepted freeze are safe, conditional on accepted CP179 and preservation of every intermediate prefix.** No native control-flow or state-dependency boundary requires five separate build/freeze cycles. Do not collapse the five effects into an uninspectable final state. Resolve the stop-hook semantics explicitly as described below before treating intermediate reentry tests as meaningful. Parent must retain source-stable focused/full/original-comparison qualifications and freeze only a passing batch.

## Independent native check

Direct PE read of canonical QtCore DLL SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` confirms `[0x13120f,0x131220)` bytes `48897b10897b2448897b38897b64897b60`, SHA-256 `a7de8449ab5859fce3867f62e28f3d4060546d70b30701092ad0f631e10a9144`. Retained disassembly agrees: qword +0x10, dword +0x24, qword +0x38, dword +0x64, dword +0x60. Successor cursors are respectively `0x131213`, `0x131216`, `0x13121a`, `0x13121d`, `0x131220`. All destinations fit in 0x88, are nonoverlapping and do not overwrite prior sparse observations.

There is no call, branch, explicit throw or register write within the five-store interval. Previously qualified RBX private and zero RDI/EDI survive the selected equality branch. The next bytes `48897b288b4320` are the excluded sixth zero store and following changed-memory read. No claim about private20 read/modify/write inputs, constructor return or later task/private backlinks follows from this batch.

The lack of native explicit throws does not prove immunity from access faults. This model admits a valid owned private allocation; native invalid-memory faults and recovery are not modeled by artificial host stop diagnostics. Retained unwind-region evidence qualifies the selected static normal/equal path, not live occurrence, all exceptional paths or full QObject construction.

## Ownership, sparse order and stop semantics

Use optional<uint64_t> private10/private38 and optional<uint32_t> private24/private64/private60, initially unknown. Preserve exact native order and a distinct phase for each successor. At **every** entry/resumed prefix, validate retained Report/private allocation identity, sizes/written address points, store/root pointee and control block, canonical version/load/call/module, committed same-root QList bindings and QString/QVector, seven original zeros including private08, completion/backlinks, earlier batch fields zero, and all later batch fields unknown. Full-current-prefix validation must precede its next write; otherwise a prematurely observed later field could admit partial mutation before refusal.

Each accepted store and cursor change must be a nonthrowing pair after qualification. Do not allocate diagnostics or mutate shared counters between the field and cursor writes. A refusal at a later retained prefix preserves its accepted earlier zeros and exact phase; never roll back the prefix to make the whole batch transactional. No count, binding, marker, observation baseline, task/cache map or backlink effect belongs here.

The plan asks both intermediate reentry to refuse and intermediate resume to progress. Those are compatible only with explicit hook semantics. A clean internal test hook is an **absolute batch stop ordinal** 1..5, default 5: unchanged target causes same/different-key calls at that target to refuse without mutation; raising the target permits continuation from the retained prefix. Alternatively use a per-call budget with zero-budget refusal and positive-budget resume, but tests must distinguish those operations. A positive per-call budget cannot be expected both to resume and to refuse at an intermediate prefix. Keep this control internal and outside the public C ABI; do not characterize it as a native exception or scheduler behavior.

## Required acceptance tests

1. Fresh/type checks for all five unknown optionals with exact widths; actual ConversionSession pairing; uninterrupted five-store run to 0x131220.
2. For each stop ordinal 1..5, inspect exact phase, ordered observed zero widths/values, later unknowns, original seven zeros, same private/root/Report identities, canonical caller evidence, bindings/markers, three distinct deltas and available observation metadata, counts/maps and incomplete/backlink flags. Include zero, independent nonzero and near-wrap history without inventing absolute counts.
3. At each intermediate target, same-key and different-key reentry with unchanged stop configuration must leave the full frame fixed. Then explicitly raise the target/resume budget and compare the final full frame to uninterrupted execution. Verify no store/references replay and no key-dependent growth. Reject invalid hook configuration before any mutation, if such values are representable.
4. At entry and all four intermediate prefixes, corrupt predecessor zero, each newly required prior batch zero, next-field unknownness and a **later** field's unknownness. Premature zero and nonzero observations must refuse before the next write. Include unknown/missing or nonzero earlier batch field with a cursor claiming it was committed.
5. Across retained prefixes cover allocation identity/size/address/written flag/Report faults, absent/expired/foreign/both aliasing root forms, first/second QList mismatch and all bindings/control blocks/markers, malformed reachable QVector qualifier, missing/wrong version/module/load/call, phase inconsistency and completion/backlinks. Snapshot full frame, weak ownership, counters/observations and maps around each refusal; prior accepted fields stay intact. Replaced-private cases must avoid dangling references.
6. Migrate older successful rewind fixtures to clear all five batch fields. Preserve intentional earlier diagnostic tests and delta arithmetic. Boundary checks require private28/private20 still unknown/unmodeled, task/private08 distinction, incomplete QObject, unavailable backlinks/thread/counter/TaskInfo and no mismatch-call effects.
7. Run focused factory/root-owner/Session tests including every prefix, independent source review, then one source-stable full build/regression/comparison gate and freeze with exact source/binary receipts. Preserve logged gaps and diagnostic differences as qualifications. The timing note's projected avoided gates are not measured realized speed or token savings.

## Exact snapshot receipts

| File | SHA-256 |
|---|---|
| `analysis/resume180-connector-batching-review.md` | `deac21d81293a4c2e927931ab9aa7cdf932d3344d594b9a35377711ab5c5a1cf` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `ea7b2db9ac979e51b6f196360fc98d5ec406d34c52b439ed3a886c44fa0534b0` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `0dda6382a161822eaedd510c08e91b2a7231bc3a31a83c6bfeee19489065128e` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `8ea82fda1aa489da24361d172e6b230fa362d35531d48ca0aeea8871b06ef6de` |

Only this report was written. No source/test edit, build, executed tests, GUI control, live REA, deployment or shared recovery-state changes occurred.
