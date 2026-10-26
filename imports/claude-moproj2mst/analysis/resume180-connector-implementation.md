# CP180 connector five-store implementation

## Source outcome and handoff

Implemented one source checkpoint for the selected QtCore straight-line range `[0x13120f,0x131220)`, starting from the accepted CP179 `BeforePrivateZero10` frame. The default native-profile continuation advances through all five stores and refuses at `BeforePrivateZero28`, RVA `0x131220`. Parent supplied CP179 as frozen/passed; this implementation does not independently reassert its build or acceptance receipt.

This lane releases exclusive edit ownership of the four files below at this report's hash snapshot. Parent owns independent review, configure/build, executed focused/full/original-comparison gates, candidate freeze, shared recovery state, and any later acceptance/deployment. No source outside these four files and this report was edited by this lane.

## Static evidence checked

Direct PE parsing of `evidence/original-binaries/QtCore4_64.dll` found `.text` raw offset `0x13060f` for the entry RVA. DLL SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Exact interval bytes are `48897b10897b2448897b38897b64897b60`, SHA-256 `a7de8449ab5859fce3867f62e28f3d4060546d70b30701092ad0f631e10a9144`. Excluded successor bytes are `48897b288b4320` (qword `+0x28` store, then dword `+0x20` read).

The retained sequence and qualifications were read from `analysis/resume180-connector-batching-review.md` and `analysis/resume180-connector-batch-plan-review.md`. This is static selected normal/equal-path evidence, not live occurrence, access-fault recovery, all QObject paths, or QObject completion.

| Ordered write | Exact sparse type | Successor phase | RVA cursor |
|---|---|---|---|
| private `+0x10 = 0` | `optional<uint64_t>` | `BeforePrivateZero24` | `0x131213` |
| private `+0x24 = 0` | `optional<uint32_t>` | `BeforePrivateZero38` | `0x131216` |
| private `+0x38 = 0` | `optional<uint64_t>` | `BeforePrivateZero64` | `0x13121a` |
| private `+0x64 = 0` | `optional<uint32_t>` | `BeforePrivateZero60` | `0x13121d` |
| private `+0x60 = 0` | `optional<uint32_t>` | `BeforePrivateZero28` | `0x131220` |

## Guard and partial-state design

Every entry/resumed prefix revalidates the CP179 allocation identity/size, Report kind and retained ordinal association, written address points, exact factory/allocation root ownership, QList/QString bindings and commit markers, second same-root QList, reachable QVector binding/marker/qualification, seven earlier observed zeros, canonical QtCore module/EDX/load/call provenance, and incomplete/backlink flags. Every batch field is checked before the next write: earlier fields must be observed zero and all later fields must remain unknown. A prematurely observed later zero is rejected as well as a nonzero value.

The accepted field write and its phase advance are adjacent nonthrowing scalar operations; no diagnostic allocation, counter change, or callback intervenes. Earlier accepted stores are never rolled back after a later refusal. The five-store interval has no root reference, observation-baseline, factory map, task/cache, TaskInfo, thread/counter, completion, or backlink effects. Private `+0x28` and `+0x20` remain unmodeled (compile-time absence assertions are included); task `+0x08` publication remains distinct from the already accepted private `+0x08` zero.

The only stop control is private C++ `TaskFactoryRuntime::privateZeroBatchStopOrdinal_`, absolute ordinal `1..5`, default `5`. A friend declaration admits `detail::TaskFactoryTestAccess`, defined only in the factory test translation unit; there is no production setter or public C ABI change. Unchanged target reentry refuses without replay; explicitly raising the target permits remaining stores. Invalid ordinals refuse before factory/allocation mutation. Adapter release code was not changed. The hook is an artificial diagnostic boundary, not a native throw or scheduler claim.

## Test source added and migrated (not executed here)

Seven CP180 cases were added. They cover all five stops, same/different/custom-key reentry, explicit resume from all four intermediate stops, full-frame comparison with an uninterrupted run, actual `ConversionSession` factory/root ownership, three independent delta histories (zero, distinct nonzero, distinct near-wrap), optional list/vector observation baselines, exact optional widths/fresh unknownness and nonthrowing scalar assignments.

The inherited owner/predecessor damage matrix runs every damage at entry and the four intermediate prefixes, with full frame/sparse/weak ownership/metadata/counter/cache snapshots. It includes replaced-private cases without dereferencing the invalidated local alias, malformed reachable QVector qualifier, all committed bindings, expired/foreign/both aliasing forms, version provenance and completion/backlinks. Separate cases cover all batch fields as earlier missing/nonzero or later premature zero/nonzero, inconsistent/unknown phases, null private state, invalid Report ordinal, unpaired/expired/aliasing roots, and invalid internal stop settings before initial allocation and at a retained prefix.

Historical factory-test terminal assertions were migrated from `0x13120f`/`BeforePrivateZero10` to `0x131220`/`BeforePrivateZero28`; successful rewinds clear all five new fields. Intentional earlier diagnostics remain. A bounded search found no old endpoint references in other `source/apps/moproj2mst/tests` files, so no out-of-scope migration was needed.

Static hygiene found no duplicate test names, balanced source delimiters, and no historical `private08.reset()` rewind missing a five-field clear. These are textual checks, not compilation or test success. Parent must compile the type/noexcept assertions and execute all required gates. No build, executed test, freeze, GUI control, live mutable REA, deployment, shared-state, or Git transport operation occurred in this lane. No GUI-control session was acquired, so there is no release receipt to claim.

## Source snapshot at edit-ownership release

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `f507339e99751ceb2cbc2d1bb79e33927ba5e1f3ccc223fe38b67db488fc368d` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `2b5750b79bac2883c6d8cded5c3661b50e009c8639f78f2eb81fc8cd9ea1cd6a` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `c26e389164941fe4f0b7e815408a7a99a9ffc3a135d2ce924b78bdcf9c04eb9e` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `921ad7e08473df867ca20dd3540b665cab78342159d169a62bb27edbcb0614fe` |
