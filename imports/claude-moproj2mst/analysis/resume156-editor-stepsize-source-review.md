# CP156: released StepSize table source review

Accepted within the frozen private type80 contract; no blocking source/test defect was found by static inspection. This review performed no source edits, build/test execution, GUI/original execution or REA operation. Root owns compiler/full-suite validation and archive binding.

## Owner, alias and copy ordering

`tableStepCurve` admits exactly type80/ndim1/ncomp1/var0=1-or2, positive1..4096 points, required axis/value arrays, no inactive counts/axes/u68, aligned bounded nonoverlapping object/payload ranges. The resource cap remains candidate admission. Samples are copied without finite/evaluation checks. The legacy constant predicate is byte-equivalent to CP155 after newline normalization.

For table Apply, the live destination must also satisfy the supported constant/table profile. `independentStepCurves` checks all source object/axis/value ranges against every destination range before the mode or scalar stores. This covers object self-copy, same/cross payload sharing and interior overlap. Guards do not silently repair owner bindings. Raw allocation extents remain qualified by actual valid typed/deep-copy producers, not proven by pointer arithmetic alone.

`copyStepSizeCurve` validates the source, makes a real independent DYNARY, copies it, then checks complete shape/arrays, metadata/constant/sample bytes and source mode before returning the table owner. Fill normalizes mode before this copy, retaining that reached effect on refusal. It does not publish an incomplete table as supported.

The original StepSize scalar/validation segment compares equal to CP155. It still writes selected mode and sequential scalar prefix, returns ValidationStopped before table copy on Invalid, and copies the remembered table only at the successful final tail. Selected live mode is restored before destination completeness certification, so old temporary mode does not overwrite it. A failed table allocation/copy certification returns UnsupportedSnapshot with reached scalar/mode/partial storage retained; it does not roll back or claim native OOM/EH parity.

## Constructor and restoration

The initial temporary-copy position now uses the checked StepSize clone. Backup DTTYPE is certified before backupCreated/backed refresh; an incomplete backup is never copied over live SC. The final StepSize clone remains **before** its refill. A refusal breaks the loop and reaches common valid-backup restoration rather than returning past cleanup. Final restoration is certified before backupRestored/PageStateComplete. Other environment/view-factor copy helpers retain their prior behavior.

STPDEF3's initial mode normalization still precedes backup and survives restore. The native definition callback's var1/2 idempotence remains the relevant admission proof; no definition-dialog, interpolation or physical widget enable behavior was added. The checks do not certify unrelated Qt/allocator/EH paths.

## Tests and scope

The new file has eight unique Testkit cases and one main, with expected includes and visible signatures. Direct Fill covers both variables/modes/STPDEF, independent arrays, scalar g6 and raw qNaN/signed-zero payloads. Bound private construction covers late actual table input, initial/temporary/backed phases, backup restore and final independent page/live data. Owner-bound private Apply checks remembered table restoration, selected mode, later pages/Extra3D/recovery. Early/late Invalid preserves live table storage and prior scalar effects; lifecycle ValidationStopped still continues later pages.

Alias tests use real allocations and restore modified pointers/counts before destructor cleanup; the interior slice stays within the original allocated axis. Profile tests restore metadata/pointers and detach their explicitly owned nested object for safe cleanup. The backed-refresh refusal uses a missing observation and verifies common valid-backup restore; it is correctly not labeled allocator-failure injection. No obvious static compile or fixture-lifetime defect was found. Actual compiler/test results remain external to this review.

Public6504abb0/140eb5180 guards and false public-completeness claims remain unchanged. The positive fixture is an actual Project/Operation/GuiModel-bound **private** StagedEditor300 page composition; it does not establish a physical QWidget constructor or public Session Save.

## Frozen identities

Implementation receipt SHA-256: `aaf0de8a3a0d237489ed9577e0c3560b8a0be513448b65f21b48d360fe471142`. Exact reviewed files under source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|src/engine/simcontrol300.cpp|`95459695e67b18ad4ff6de0e616fddbffbf5d7063c7c01cf83f4f48e961e238c`|
|src/engine/simcontrol300_lifecycle.cpp|`fcb187ffc0ad12f82ae565de419c3ca91e5ba5952fff07f42d65ea7aa02dfea1`|
|tests/mdt/test_simcontrol300_stepsize_table.cpp|`42a28fa6741f5a072a69b217cd55f822562503cab1c735e174b5dd9f8352eef0`|

Controlling contract SHA `ecb28b90422c8a3a5865208c1aa57cf03b399e45638a9b90e49a513961853e43`; independent readiness review SHA `4a8399133c55d10ef041141f257392f86ddef499214c0a8852b79a03b8db5832`. Those bind the verified original DEF_GUI_MO_64 v11.0.2 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and nine native ranges. No reconstructed executable/deployment was inspected. This acceptance applies only to the reviewed source hashes and bounded profile.
