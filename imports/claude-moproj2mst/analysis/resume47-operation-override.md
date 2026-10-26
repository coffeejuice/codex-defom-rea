# Resume 47: next operation override audit

## Reconciliation

C07 in `analysis/closure-reconciliation-checkpoint35.json` is correctly still marked “base dispatch integrated; remaining families open.” The prior base widget order work does not close class-specific slots. ShapeOptim is not a remaining gap: `save_current.cpp::propsToEngine` delegates to `OpClass::propsToEngine`, and `ShapeOptimClass::propsToEngine` in `source/apps/moproj2mst/src/ops/report.cpp` calls the qualified mapping in `propsToClass` (MaxSims -> `fcc_` / +0xcc; MinObjFuncVal -> `fe0_` / +0xe0). The last dispatch test extension is only for routing and is frozen pending root's build.

## First remaining reachable override candidate: Distortion default contact pairs

The best evidenced next target is Distortion's `vt+0x198` override in the inter-object rebuild path. `source/apps/moproj2mst/src/engine/save_current.cpp::saveCurrentOperation` reaches `syncOperationFiles` for active operations. That calls `saveInterObject`; with `op.lubSystem` set and an engine model present, it calls `rebuildSystemContacts`. That function calls `classDefaultPairs`, whose current Distortion2D/3D arm explicitly throws `notPorted(op, "vt+0x198")`.

This branch is source-reachable under concrete guards. The existing original-qualified `source/docs/re/moproj2mst/s/spec.md` section 1 documents the Save order and that `0x14005a8a0` rebuilds contacts from class defaults when the System lubricant byte is set. Section 4 documents the original virtual call `ob->vt[0x198]`, names Distortion as one of the own overrides, and describes the downstream pair append/mirror effects. The current source's refusal therefore blocks an established Save branch rather than representing an inherited lexical candidate.

## Exact implementation boundary and missing evidence

Do not implement Distortion pair values from the base algorithm: the source specification explicitly says Distortion has its own override, so substituting base pairs would invent behavior. Existing retained Distortion static artifacts (`analysis/wave3-distortion-save-static.json` and `wave3-distortion-save-*.asm`) cover properties/writer/save-three file bodies, not the Distortion `vt+0x198` body. The L6 spec does not provide that method's pair-selection semantics. The exact next evidence action is to recover and qualify the original Distortion 2D/3D vtable slot +0x198 target and its full function body from the preserved original PE, then map its object/slot/property inputs and return-vector elements against the already recovered common `ContactPair` representation. This is required before a source edit.

If that recovery shows a bounded no-op or fixed rule, implement only that branch and add a `saveInterObject`/default-pair unit case with a System lubricant, an active Distortion operation, and a model that exercises the recovered object-selection branch. Acceptance must assert the generated pair count, master/slave indices, friction/IHTC and 3D flag, plus the unchanged save ordering and mirrored pair records, against the qualified original body. If it depends on unported state, record that dependency and test the refusal boundary instead of guessing.

## Limits

Read-only static audit. No build, test, app, REA session, or source edit was performed for this audit.
