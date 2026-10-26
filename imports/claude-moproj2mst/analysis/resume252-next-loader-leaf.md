# CP252 next loader/property leaf selection (reconciled)

## CP251 status correction

The previous CP252 draft incorrectly said the MultiBlow prologue owner command was pending. Current source and `analysis/resume251-multiblow-control-implementation.md` confirm `GuiModel::initializeMultiBlowLoadControl()` is implemented, the raw prologue writes/guard were removed, the command has direct parked and absent-step tests, and the real class-prologue test seeds/checks DTTYPE and NSTEP. The receipt records focused `m2m.mdt.multiblow_load` and `m2m.mdt.viewer_box` passes plus full build and source fingerprint stability. Root reports the source-stable full suite also passed. Treat CP251 as completed; do not schedule it again.

## Result: no separate dependency-ready finite loader/property leaf established

After CP251, the remaining apparent sites do not meet the same bounded evidence/order bar:

- **Forming post-load**: CP247 corrected the recorded-effects success order. Original manager/copy failure behavior remains an evidence blocker, including physical failure prefixes and the concrete manager path after `-1`. This is not a new source migration opportunity; do not use another owner wrapper to imply failure fidelity.
- **MultiBlow post-load**: `multiBlowPostLoad` still combines creation/refresh of missing model objects and slots, PDIE selection, `ObjEntry` movement flag, and lubricant reset. Its current unsupported branches and partial state prefix require a focused branch contract before carving out a model command.
- **MultiBlow save names**: `multiBlowSaveNames` calls base save names, resolves the model, checks `simCtl()`, then calls `simCtl()` again to write PDIE. Replacing the two admissions with one changes the number/timing of guarded accesses; existing whole-save Oracle B tests cover final files but do not isolate missing-control/parked behavior at this hook. A source-backed single-admission contract and direct hook test are needed before proposing a finite owner command.
- **HeatTreat property refresh**: CP249 already identified that snapshotting control fields would move reads ahead of schedule defaults, while moving the sequence into `GuiModel` introduces property-layer coupling. The required finite coordinator and tests for absent control, parked geometry, defaults-once and field order remain unresolved.
- **Other L2/L3 aliases**: CP244 and the CP222 ledger identify imports, inherited-object creation/copy, and equipment serialization paths spanning Project, Operation, object slots, and FileEffects. The remaining code does not reduce to a single value command without a separate effect/failure contract.

## Evidence and boundaries

CP251's reviewed MultiBlow prologue is closed as an implementation increment. L4 decompile/assembly qualify its ordering; current tests validate reconstructed class dispatch and command admission, not a newly executed native occurrence. CP247's Forming failure investigation remains separate and must keep the original GUI writer wrapper's hidden inner status distinct from observable file/add/path/ID outcomes. CP248's meshing writeback is complete and CP249's property/Close contract remains as documented. No global loader alias closure is claimed.

This read-only reconciliation writes no source, tests, build, shared state, original, GUI, or REA artifacts; no tests were run by this pass.
