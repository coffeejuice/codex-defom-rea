# CP180 connector independent source review

## Verdict

**Accepted for one batch build/validation/freeze sequence; no blocking source defect found.** The released four-file snapshot preserves every intermediate prefix, explicit absolute-stop semantics and per-prefix refusal state. This is static source acceptance; compilation, executed focused/full regression, original-comparison qualifications and candidate freeze remain parent-owned gates.

## Exact native effects and source ordering

The batch loop at `task_factory.cpp:463` represents precisely the previously independently verified interval `[0x13120f,0x131220)`: `48897b10897b2448897b38897b64897b60`, SHA-256 `a7de8449ab5859fce3867f62e28f3d4060546d70b30701092ad0f631e10a9144`, in QtCore digest `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Native sequence is qword +0x10, dword +0x24, qword +0x38, dword +0x64, dword +0x60. Header optionals have those exact uint64/uint32 widths and initially unknown state. Successor cursors/phases are respectively 0x131213/BeforePrivateZero24, 0x131216/BeforePrivateZero38, 0x13121a/BeforePrivateZero64, 0x13121d/BeforePrivateZero60 and 0x131220/BeforePrivateZero28.

Each loop iteration matches one current phase, requalifies the full retained owner/predecessor and every batch field, then writes exactly one optional and its next phase. `matches` requires fields strictly before the current ordinal zero and every remaining field disengaged. Thus an observed later field refuses before an earlier next store can mutate. Accepted field/cursor assignments are adjacent scalar operations without callbacks, count changes or diagnostic allocation between them. No rollback erases earlier accepted fields.

At ordinal 5 the validated stop target is at most 5, so the stop check always throws before the switch or ordinal+1 indexing; the terminal path cannot access beyond the phase array. Excluded successor bytes `48897b288b4320` remain private28 store/private20 read, with no corresponding model fields/effects. No reference increment, baseline rewrite, binding/marker change, task/cache growth, thread/counter/TaskInfo, backlink or completion occurs in the batch. Native access-fault recovery is not simulated; evidence qualifies the selected owned normal/equal path only.

## Guards and hook

Every prefix reuses retained Report ordinal/pointee, allocation-time private identity, sizes/written address points, store/root control-block and pointee pairing, first QList/QString canonical bindings/markers and incomplete/backlink qualification. It additionally checks canonical committed QVector and optional qualification, second same-root QList ownership/marker, seven prior zeros including private08 and exact retained version/module/load/call. No fixed delta or absolute baseline is required.

`privateZeroBatchStopOrdinal_` is private C++, default 5, restricted to 1..5 before factory/allocation mutation. Its friend access struct is defined in the factory test translation unit; there is no production setter or public C ABI hook. Unchanged absolute target refuses at its retained cursor; raising it resumes. Lowering it also stops without replay. This resolves the plan's resume-versus-reentry distinction without inventing native throw semantics.

## Authored tests and migration

Independently counted 59 inherited damage variants multiplied by five retained prefixes: **295 owner/predecessor cases**. They cover identity/size/address/written flag, root/binding/expiry/aliasing/control-block/marker, six predecessor zero/version-provenance and completion/backlink faults; the batch adds private08 qualification. Full snapshot checks retain sparse fields, provenance, weak ownership, deltas/observations, identities and counts/maps. Replaced-private handling avoids dereferencing its invalidated old alias.

The Session test at `test_doe_task_factory.cpp:4353` uses the actual session factory/root pairing, each absolute stop 1..5 and three independent histories: zero, distinct nonzero and distinct near-wrap with optional observations. Each stop verifies exact sparse prefix and full preserved state; same/different/custom-key calls with unchanged target refuse. Raising to 5 resumes each intermediate prefix and the final full snapshot matches uninterrupted execution. No reference event or observation rewrite is tolerated.

The sparse matrix exercises all five fields at each entry/intermediate cursor, missing/nonzero earlier fields and premature zero/nonzero next/later fields, with no-mutation snapshots. Separate tests cover five owner forms at each prefix, inconsistent/unknown cursor, null private, invalid Report ordinal, invalid stop settings both before allocation and at a retained prefix, fresh widths/unknownness and nonthrowing assignments. Compile-time absence checks keep private28/private20 unmodeled. These are authored tests, not executed passes.

Historical ordinary stops migrate to 0x131220/BeforePrivateZero28. Earlier successful rewinds clear all five new fields; existing earlier-seam diagnostics and delta arithmetic are retained. The hook adds no C ABI exposure. No later private28/read-modify-write, QObject return or public task/private backlink effect is claimed.

## Exact release SHA-256

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `f507339e99751ceb2cbc2d1bb79e33927ba5e1f3ccc223fe38b67db488fc368d` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `2b5750b79bac2883c6d8cded5c3661b50e009c8639f78f2eb81fc8cd9ea1cd6a` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `c26e389164941fe4f0b7e815408a7a99a9ffc3a135d2ce924b78bdcf9c04eb9e` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `921ad7e08473df867ca20dd3540b665cab78342159d169a62bb27edbcb0614fe` |

Only this report was written. No source/test edit, build, executed tests, GUI control, live REA, deployment or shared recovery-state change occurred. Required executed gates remain with the parent; do not freeze a failed run or characterize projected gate savings as measured realized speedup.
