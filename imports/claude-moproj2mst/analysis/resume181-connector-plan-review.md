# CP181 connector independent plan review

## Decision

**Source-ready for private28 alone, with an explicit CP180 stop-hook integration decision required.** No native evidence defect blocks this cut. Do not include private20 flags read/modify/write: its concrete input is unknown. Parent supplies accepted frozen CP180; source receipts match that release. This review does not rerun acceptance or original comparison.

## Independent native and unknown-input check

Direct PE read confirms raw `0x130620` bytes `48897b28`, SHA-256 `004d4c4c5e01ae7e597c0f9e23d125455cb71368395152f5095c20414a587d17`, encoding the qword private28 zero. Next bytes are `8b432025f0fcffff83c860894320`: read private20, AND, OR, write private20. Retained selected-path evidence establishes RBX private and RDI zero through CP180 and this store; private28 occupies +0x28..+0x2f inside 0x88. CP178 skips the mismatch call. The later public task/private backlinks remain after initializer return.

The flags relation `(in20 & 0xfffffcf0) | 0x60` preserves unknown bits `0xfffffc90`, forces zeros `0x30f` and ones `0x60`. Prior private24 zero is adjacent and does not determine private20. Therefore a concrete flags result, EAX value or allocator-zero assumption would be unsupported. Stop at `0x131224` before the read; future symbolic treatment needs a separate reviewed contract. Static unwind-region evidence qualifies selected normal/equal-path behavior only, not live allocator contents, exceptions, cleanup or full construction.

## Ownership and sparse contract

Use initially disengaged optional<uint64_t> private28 and exact `BeforePrivateZero28` admission. Revalidate retained Report/private allocation identity, sizes/written address points, canonical version/module/load/call and store/root control-block and pointee association. Require both canonical committed QList bindings, canonical committed QString/QVector with reachable optional qualification, the seven original private zeros and all five exact-width CP180 zeros. Private28 must remain unknown; both observed zero and nonzero refuse. Completion/base and backlinks stay unavailable.

Validate every guard before the sole private28 write and cursor advance to `BeforePrivateFlags20Read` / `0x131224`. Preserve all other fields, weak ownership/bindings/markers, three deltas and observations, identities and maps/counts. Do not introduce private20/EAX/flags-result state in this cut.

## Required stop-hook integration

Current CP180 loop throws at ordinal 5 for every legal setting (1..5), including its default. Simply appending a CP181 branch after that loop is unreachable. Simply removing the ordinal-5 stop would break the test hook's promised stop-at-5 semantics and could let the sixth store execute during CP180 intermediate tests.

Choose and document one explicit internal solution. For example extend absolute stop ordinals to 1..6 with default 6: ordinals 1..5 retain their exact CP180 stops, ordinal 6 admits CP181 and stops before flags read. This extends only the private C++ diagnostic control, never the C ABI; update invalid-stop tests to use 0,7,max instead of treating 6 invalid. Alternatively distinguish enabled test stops from ordinary production continuation. In either design, unchanged stop 5 must preserve unknown private28, raising/enabling continuation must execute it once, and malformed hook values must still refuse before factory/allocation mutation.

This is an implementation integration requirement, not a native uncertainty. Preserve every CP180 intermediate prefix/refusal and no-rollback contract while allowing the new default path to progress.

## Focused tests and migration

Use actual ConversionSession and independently reconstructed CP180 final frame: canonical evidence, all twelve prior zeros, private28 unknown. Require only private28/cursor change, with independent zero/nonzero/near-wrap histories and optional observations unchanged. Same/different/custom-key reentry at 0x131224 preserves the complete terminal state. Fresh/type checks establish unknown optional<uint64_t> and absence of private20/flags-result/EAX model.

At CP181 entry cover absent/expired/foreign/both aliasing roots, binding/control-block/marker faults, malformed QVector qualifier, missing/wrong version provenance, Report/private identity/size/address/written flag/phase errors, every missing/nonzero earlier zero including all five CP180 fields, observed private28 zero/nonzero and premature completion/backlinks. Snapshot full state/counters/ownership before refusal; no repair, prior increment replay or mismatch call is allowed.

Keep CP180 tests at each absolute stop 1..5; assert private28 remains unknown there. Resume all four intermediate prefixes through CP180 and explicitly test stop5-to-CP181 progression. Compare uninterrupted and resumed full frames. Historical ordinary stops migrate to 0x131224, but CP180 diagnostic targets remain their original RVAs. Earlier successful rewinds now clear private28; snapshot equivalence and compile-time absence checks must distinguish private28's newly authorized presence from still-absent private20. Existing correct delta arithmetic remains unchanged.

Focused/root-owner/Session, source-stable checkpoint regression and original-comparison gates remain implementation acceptance work.

## Exact SHA-256 receipts

| File | SHA-256 |
|---|---|
| `analysis/resume181-connector-next.md` | `0f467499a94064401a5ee832c5740550d7459b9413bee8dc78ba495c0ca18af0` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `f507339e99751ceb2cbc2d1bb79e33927ba5e1f3ccc223fe38b67db488fc368d` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `2b5750b79bac2883c6d8cded5c3661b50e009c8639f78f2eb81fc8cd9ea1cd6a` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `c26e389164941fe4f0b7e815408a7a99a9ffc3a135d2ce924b78bdcf9c04eb9e` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `921ad7e08473df867ca20dd3540b665cab78342159d169a62bb27edbcb0614fe` |

Only this report was written. No source/test edit, build, executed tests, GUI control, live REA, deployment or shared recovery-state change occurred.
