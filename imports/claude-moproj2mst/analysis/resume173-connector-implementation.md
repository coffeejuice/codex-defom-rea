# CP173 connector implementation: QVector shared-null atomic add

Implemented the bounded QtCore RVA `[0x1311d2,0x1311da)` cut after the accepted CP172 private `+0x68` binding. The same associated root-owned QVector record receives one unsigned 32-bit symbolic increment, the private committed marker becomes true, and the cursor advances to `BeforeSecondQListBinding`. The diagnostic stops before RVA `0x1311da`, leaving private `+0x70` unavailable and the second QList reference unapplied.

## Guards and preserved state

The continuation reuses `qualifyRetainedZeroPrefix` for the exact retained pending Report/private identity, allocation sizes, address points, earlier QList/QString commitments and bindings, live allocation-store/root pairing, and incomplete/unbacklinked QObject. The helper now compares both pointee and shared ownership identity for the earlier bindings. The new cut compares the locked private QVector binding against the associated root QVector by pointee and control block; validates the immutable module qualification and optional symbol-RVA override; requires all four observed zero fields and a false QVector committed marker. Every check precedes the root delta, marker, and cursor mutations. No numeric native baseline is required and preexisting nonzero deltas are accepted.

Added a minimal root commit method in `view_manager.hpp`, matching the existing QList/QString methods. The adapter models unsigned wrap and does not claim native atomic concurrency or a live Qt count. Existing optional absolute observations and their qualification remain unchanged; no projected-count API was introduced. Module SHA-256 and canonical symbol RVA are immutable constexpr fields in this model, so a mutable wrong-digest test is inapplicable; the existing optional RVA override supplies malformed-symbol coverage.

## Focused test changes and static review

Natural constructor-stop expectations advance from `0x1311d2` to `0x1311da` and `BeforeSecondQListBinding`. The actual `ConversionSession` test now checks one committed increment, matching bound/root ownership, and same-key/different-key refusal. A separate positive fixture resumes a reconstituted CP172 frame with root delta `7` or `0xffffffff`, expects `8` or `0`, and checks unknown/supplied absolute observations stay unchanged. It preserves the identities, bindings, prior deltas, zero fields, counts/maps, completion, and backlink flags.

Added 35 scheduled refusal cases for missing/expired/foreign/aliasing QVector bindings (including identical pointee with foreign control block), prior list/string bindings/markers and ownership, root expiry, predecessor zeros, sizes/address points/identity, phase, already committed marker, kind, and premature completion/backlinks. Their snapshots cover the retained frame, all three root deltas, foreign deltas, observation fields, counts and maps. Existing earlier guards cover unavailable and foreign root association before the constructor can reach CP173; root association is one-shot and cannot be rebound by this cut.

Independently checked the matrix inventory: all 35 declared cases occur in both the scheduled array and switch. Checked all test/source phase references against the enum and the referenced QVector optional observations/delta members and QString delta against their actual header declarations. Read the changed positive/session/refusal expectations against the new branch order. A static whitespace scan of the four owned source files passed. These are source inspections only, not compilation or executed test results.

## Limitations and handoff

No build, compilation, executable test, candidate freeze, original comparison, GUI control, live mutable REA operation, deployment, or shared recovery-state edit occurred. Focused factory/root-owner tests and the source-stable checkpoint/original-comparison gates remain for the parent after source review. The selected normal non-null allocation path and original instruction qualification are inherited from `resume172-connector-next.md` and `resume173-connector-plan-review.md`; live count, exceptional cleanup and native release remain unestablished.

Ownership of the four source files is released with this report. No other source file was edited.

## SHA-256 source receipts

These identify the released source snapshot, not an accepted candidate freeze.

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `14ab160c5e04344d536438d6cb2b6d47f738eab0672443936ac886537b6e47b0` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `15ecf0c1d8e1fddbb6d6f1a0a3fd03997044cb46baa18baf9dfcf314ca6c3357` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `51849bde4e923f2af7eaa7c5dd8d758dde4d27e32c97b3c210be621cefa4dc7f` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `ad7451cadae54b43b0b628f14e2f2cace0ba403a248f846251a7d27c449eabd9` |
