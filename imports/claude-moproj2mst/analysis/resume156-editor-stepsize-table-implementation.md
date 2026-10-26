# CP156: bounded type80 StepSize table lifecycle

Implemented normal owned1D, one-component DTTYPE tables through private Fill, constructor copies/backed refresh and Apply. Alias guards precede Apply mode/scalar writes; real copy data is checked before publishing a table page or certifying backup/restore/Apply completion. Legacy constants and the scalar/validation body remain unchanged. Assigned source/test files are frozen and released to root.

Only assigned simcontrol300.cpp, simcontrol300_lifecycle.cpp, NEW test_simcontrol300_stepsize_table.cpp and this receipt were written. No shared header/CMake/public API change, build/CTest, GUI/original execution or mutable REA occurred. Root owns grouped execution and archive binding.

## Contract and native order

Controlling report `resume156-editor-stepsize-table-next.md` SHA-256 `ecb28b90422c8a3a5865208c1aa57cf03b399e45638a9b90e49a513961853e43` and independent `resume156-step-type80-independent-review.md` qualify the normal profile. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Native89b4d0 maps nonconstant var1/2 to definition1/2;89b450 writes the same ndim1/var and does not evaluate samples. The current StepSize owner therefore survives the existing callback unchanged. STPDEF still determines definitionRadio separately. Initial mode normalization remains before the copy; STPDEF3 normalizes u58 to1 before the constructor backup. Native final Apply saves selected mode atEBD11A, copies the remembered curve atEBD141 and restores selected mode atEBD153. All scalar conversions and Invalid return ordering remain in their existing positions.

## Profile, ownership and copy checks

The new branch requires type80, ndim1, ncomp1, var0=1/2,1..4096 points, actual axis0/values and no inactive counts/axes/u68.4096 is a candidate resource cap, not an original format maximum. Unused var1/2 are preserved. Samples and constant bits are not evaluated or required finite. Legacy constantStepCurve is unchanged.

Bounded uintptr ranges reject within-table object/payload overlap, axis/value overlap and address overflow. Before any table Apply mode/scalar write, the current destination must have supported ordinary ownership/shape and every source object/payload range must be disjoint from every destination range. This rejects object self-copy, same/cross payload sharing and interior overlap without attempting to repair ownership. Logical extents belong to the admitted actual typed/deep-copy producer profile, not arbitrary padding or a fabricated heap receipt.

A private two-translation-unit bridge clones StepSize at its existing constructor positions. It checks the actual allocated arrays, copied metadata/constant/sample bytes and independent storage before publication; it adds no installed header or C ABI. Fill retains reached mode normalization if its copy is unavailable. Initial temporary-copy refusal retains reached state. Final pre-refill copy refusal breaks the existing loop so a valid SC backup still restores.

The SC backup's own DTTYPE deep copy is verified before backupCreated/backed refresh. An incomplete backup returns before refresh and is never copied over live SC; its ordinary host destructor cleans partial owned storage. Final SC restoration is checked before backupRestored/PageStateComplete. Apply restores selected mode in native order before its post-copy data/array check; unavailable destination storage yields UnsupportedSnapshot with reached scalar/mode/partial-copy state retained. No rollback, allocation-success seed, fault hook or native OOM/EH parity is claimed.

## Focused tests and static checks

NEW `m2m_mdt_simcontrol300_stepsize_table` / `m2m.mdt.simcontrol300_stepsize_table` has eight Release-active Testkit cases:

1. Both definition variables across modes/STPDEF1/2/3 retain raw qNaN/signed-zero payload and unused metadata, normalize mode correctly and publish independent complete Fill copies with scalar g6 text.
2. Actual bound Project/Operation/GuiModel constructor reaches all initial/backed pages, captures late table data through real Extra3D input seam, restores backup and retains independent table owners.
3. Owner-bound private composed Save restores remembered table over changed live data, preserves selected mode over the old temporary's mode, and reaches later pages/Extra3D/recovery.
4. Early and late Invalid scalar returns retain reached mode/scalar prefix but leave live table storage/data and remembered snapshot untouched.
5. A StepSize ValidationStopped still reaches later Environment/Extra3D and owner-bound recovery while leaving its table copy deferred.
6. Object self-alias, same/cross payload aliases, actual interior overlap and within-source alias refuse before mode/scalars; tests restore real pointer ownership for cleanup.
7. Unsupported shapes/types/variables/cap/missing arrays/nested owner guard Fill and live Apply destination before normalization.
8. Backed-refresh missing observation with an actual table takes the existing partial path and still restores the valid SC backup. This is not an injected allocation-failure witness.

No physical OOM was forced. Copy-failure guard placement was inspected statically; no native allocation/EH behavior is inferred from those candidate refusals. Existing constant/scalar targets remain root's required regression coverage.

Static checks passed whitespace/newline/no-NUL, actual include roots, eight unique cases/one main, supported Testkit macros and source-order checks. Against CP155 source.zip, constantStepCurve, Iteration/Advanced/owned-recovery/empty-recovery functions and the old StepSize scalar/validation segment are unchanged. Actual final copy precedes mode restore and then completeness check. Backup admission precedes certification and incomplete later copies cannot bypass common restore. No compiler or C++ test ran in this lane.

## Frozen release hashes

| Owned file | SHA-256 |
| --- | --- |
|source/apps/moproj2mst/src/engine/simcontrol300.cpp|`95459695e67b18ad4ff6de0e616fddbffbf5d7063c7c01cf83f4f48e961e238c`|
|source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp|`fcb187ffc0ad12f82ae565de419c3ca91e5ba5952fff07f42d65ea7aa02dfea1`|
|source/apps/moproj2mst/tests/mdt/test_simcontrol300_stepsize_table.cpp|`42a28fa6741f5a072a69b217cd55f822562503cab1c735e174b5dd9f8352eef0`|

All owned files are frozen/released. Full control/dialog/Qt allocation/EH ownership and outer public Save remain separately qualified; no public completion guard was relaxed.
