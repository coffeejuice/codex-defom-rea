# CP246 independent review

## Verdict: HOLD

The contract correctly identifies a real successful-path ordering mismatch and a real failure-model gap. It is not ready to serve as a complete migration contract because its “smallest remaining investigation” repeats static manager/copy analysis that is already retained, and the key native failure prefix is still explicitly unclosed. A success-only coordinator can be implemented as such, but must not be presented as faithful across add/copy failures.

## Verified claims

- The retained L3 decompile establishes the successful branch order in the report: create the temporary KEY and call manager add; rename movement; get the manager path; write the slot ID then operation ID; reacquire the step and attempt the second KEY write. In current source, `createEquipment` records the second write before returning, and `formingPostLoad` assigns IDs after return. The mismatch is exact and relevant if final serialization/write fails.
- Current `addEquipment` treats non-null bytes as success: it records directory/write effects, appends the row, and returns an ID. It has no observable copy-failure result. This cannot model the original manager's `-1`/no-append path.
- Current `formingPostLoad` runs model lookup, guarded `simCtl`, reads PDIE/equipment ID, then object lookup and `objEntryN`; creation requires `equipmentId < 1`, a non-null object, and a slot. `objEntryN` can materialize missing in-range entries before a no-create branch. The contract's proposed branch-specific tests preserve that prefix. The original decompile also admits the manager before creation; current Project manager availability is assumed.
- The existing `forming_without_equipment_creates_one` is a positive-path caller test only. It does not establish slot identity, effect ordering, failure behavior, or native execution. Historical Oracle-B final-file comparison likewise cannot establish intermediate failure prefixes.

## Evidence and provenance

The retained decompile files match the import manifest and their current hashes:

- `evidence/gui_mo/s1/mdt/L3/d0.c`: `6D3703619843FB9AE1760F91E6633FB982C35CDC39D9DD2D89BEE7435477FBFF`.
- `evidence/gui_mo/s1/mdt/L3/d2.c`: `CF3F99AF220CB92EC9CE8A19328640113333DF4C0194FAAC2B20B475BA2C6F57`.
- Original `evidence/gui_mo/DEF_GUI_MO_64.exe`: `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`, matching the retained static-capture provenance. These are source/decompile and static-binary claims, not fresh native runtime observations.

The plan's proposed manager/copy inspection is already substantially present in `analysis/wave3-equipment-copy-investigation.json` and `analysis/wave3-equipment-fallback-contract.json`. Those receipts establish that manager `vt+0x160` resolves and checks the source, invokes the `vt+0x238` copy helper, appends only on successful copy, and returns `-1` on failure. The helper attempts parent creation, handles an existing destination, and delegates to `QFile::copy`. Retained Qt static work further covers the `CopyFileW` failure fallback, including source open, temporary destination, streaming, length check, rename, and cleanup. The receipts explicitly qualify themselves as static-only and enumerate remaining gaps: QTemporaryFile generation/open/collision semantics, QFile rename fallback, Windows backend stream details, and native temporary-source/add dependencies. Thus neither decomp nor retained static captures yet prove the physical filesystem prefix for all failures.

## Required correction and next investigation

Revise “Smallest remaining investigation” to consume and reconcile the existing wave3 receipts rather than repeat the manager vt+160/copy-helper inspection. The next bounded investigation should close the still-open native prefix: establish the temporary KEY writer's status/failure behavior and source lifetime; trace the remaining QTemporaryFile/rename/backend details that affect destination/temp cleanup; and determine what the original does after manager add returns `-1` (including name/path calls, both ID stores, and the second writer attempt). Bind each conclusion to the original binary/decompile hash and retain the static-versus-runtime qualification. If static evidence cannot settle filesystem outcomes, a fresh owned-original capture is a later evidence step subject to the established gates; this review did not run one.

A forming-specific success-only ordering extraction is safe if it is narrowly labelled and tested as a recorded-effects success route: perform ID stores before recording the final write, preserve lookup and lazy-slot order, and leave `equipmentSync` untouched. It does not close the add/copy failure contract. Do not migrate or describe it as a faithful full-prefix coordinator until the actual `-1` path and its effects are represented and validated. The proposed “instrumented failure test” also needs a named, real seam: current `FileEffects` does not expose an interruption at the second serialization/write, and no production observer should be added solely to manufacture one. First identify an existing writer/effect seam or testable failure mechanism that occurs after ID assignment.

No source, tests, build, shared state, original application, GUI, or REA state was changed or run.
