# Checkpoint 46 DOE output and Save effects audit

## Scope and evidence basis

This is a source-and-receipt audit of the imported converter at checkpoint 46. The controlling scope is one fresh-session project through Open, Save and Close; study execution and simulation running remain excluded. No build, original runtime, GUI control or REA database mutation was performed for this audit. Ghidra receipt files examined here explicitly record `target_runtime_executed: false`, private project provenance, and output hashes. Those receipts establish provenance of static exports, not dynamic reachability or successful full-project behavior.

The current plan (`analysis/takeover-state.json`) says checkpoint 46 is active, full Save is not validated, and the last validation's DOE cases are diffs. Its checkpoint 45 validation reports B:120_B_DOE, B:121_B_DOE_VAR, and B:122_B_DOE_GEO as `DIFF`, with 7, 8 and 10 missing comparison entries respectively. These results are bounded historical comparator evidence, not raw-byte identity or fresh-session acceptance. The same plan records an original process crash after Open on the current billet case, so candidate completion is not parity evidence.

## Reachability and lifecycle classification

### DOE output operation (`SfOprDoeOutput`)

There are three materially different paths, which should not be conflated:

1. **MST helper path.** The original DOE dispatcher is selected in `mst/master.cpp::walkOperations` when the subproject has a DOE model. The implemented dispatcher/emitter formats DOE variables and geometry only while `doeMode` is set. However, `walkOperations` still has an explicit model-map stub when a DOE model exists (`DefDoeModel object map (DOE MST)`), and DOE TEMP output is rejected by the public conversion driver. This helper work therefore does not establish a completed public DOE MST route.
2. **Open/current-operation path.** `ops/noop.cpp::DoeOutputClass::loadOperation` resets and displays a model, invokes its class-data load, then explicitly refuses at selected-session/global-task application. Its `loadClassData` only proceeds for a Type2 subproject with an owned study and configured I/O, reads the DOE output document, applies recovered class-private output data, and returns to the higher refusal. The current Save driver always calls `engine::openActiveOperation` before Save, so an active DoeOutput operation reaches this incomplete lifecycle branch before Save's own stages can complete.
3. **Current Save slot.** `engine/save_current.cpp::saveCurrentOperation` explicitly refuses for current `SfOprDoeOutput` at vt+0x68 because source analysis says it writes the sample object through the viewer model and DOE output document; neither is reproduced. This is reachable only if the active operation survives the earlier Open path. It is a second independent gap, not proof that the slot is unreachable in all contexts.

Separately, `save_current.cpp` documents DoeOutput and Report as operations that must be last in a subproject; `save_project.cpp::lastOperationCheck` enforces that recovered validation rule. This ordering check does not implement output writing and does not erase the lifecycle refusals.

### Shape optimization (`SfOprShapeOptim`)

ShapeOptim is a distinct active-operation branch with meaningful, partially reachable Save effects. The source implements its specialized current Save order: optional widget/property synchronization, `saveAfterFiles`, then `saveOperationFiles`.

- `saveAfterFiles` copies properties and reads ROI object IDs; for type 3 it refuses because the optimization-study object and its Save behavior remain unmodeled.
- `saveOperationFiles` calls `studyFolder` before iterating ROIs. `studyFolder` queues creation of `<project>/DoeStudies/<study>/Control/Opt/`, but refuses when a DOE study is present because the study name is not modeled. Thus the exact study path is not currently constructible for a real DOE study. In the no-study modeled state, the empty name produces a path shape, but this is not evidence of correct populated-study behavior.
- With a supported path and nonempty ROI objects, the function then refuses because the ROI `.GEO` writer is not reproduced. For an empty ROI list, the source's loop adds no `.GEO` files, so the folder creation can be a real, partial Save side effect if execution reaches and applies the effects.
- ShapeOptim's Open/load side also has guards: study-name resolution and geometry reads are incomplete. The full Open-to-Save route may fail before the Save slot on populated cases.

These findings distinguish a real branch and queued side effect from a completed native Save contract. The receiver `FileEffects`/`applyEffects` route is used for normal Save effects, but a refusal thrown after appending a directory can prevent the queued effect from being applied. In the current native full-Save sequence, `saveCurrent` is followed by `applyEffects(saved.step2)` only after it returns. Therefore the source's queue insertion alone does not establish that the directory is present on disk when a later ShapeOptim guard throws.

### DOE output helper versus public Save

`mst/emit_b_doe.cpp` describes DOE text emitters and explicitly marks the geometry `.COR` writer as not run. The main writer calls DOE dispatch only under a DOE model, and its study map is still a stub. This is an isolated serializer/helper contract with unit tests, not a public lifecycle completion. The `M2M_FLAG_DOE_TEMP` flag is explicitly unsupported in `core/convert.cpp`; `mst/master.cpp` locates `.MST.TEMP` and DOE folder behavior at the top-level writer but does not implement it.

## Save side-file effects

The full Save driver in `core/convert.cpp` executes, in order: native operation/item blob effects and Open effects; Save step 2 (`saveCurrent`) effects; the `.moproj` writer; operation property-file writers in operation order; then MST output. This ordering matters for DOE branches:

- A current DOE Output operation can refuse during Open or step 2 before the later project/property/MST stages.
- ShapeOptim can queue the `Control/Opt` folder and then refuse on optimization state or ROI geometry; the caller's effect application occurs only after the hook returns, so do not report the queued folder as a verified disk write in these refusal cases.
- The general task-study I/O receipts cover parsed task/session and output data leaves, and the current source binds native Study I/O for `WRITE_SAVE`. That infrastructure does not itself establish the operation's document writer or complete side-file lifecycle.
- The top-level DOE `.MST.TEMP` path and DOE-related folders are separately identified in source, but explicitly rejected at the adapter flag boundary.
- DOE geometry emitter references `.COR` output only as an unrun original writer; `emit_b_doe.cpp` intentionally does not create those files. Do not infer `.COR` side effects from text containing `.COR` names.

## Evidence limits and actionable conclusion

Static code and retained receipts support that DOE output, optimization and DOE helper branches are present and connected to lifecycle dispatch at specific points. They do not support claiming successful public Open-to-Save equivalence. Checkpoint 45's three DOE comparison cases remain diffs, full Save is explicitly unvalidated, and the current source has guards at active-operation load/save boundaries. Existing DOE receipts, including output-data contracts, qualify themselves as static/helper projections or incomplete class-data coverage; none substitutes for original-first full lifecycle evidence.

For checkpoint continuation, treat the next useful proof boundary as a bounded original/candidate Save case that captures (a) whether the current operation reaches the DoeOutput slot or fails earlier in Open, (b) the precise project/operation context and fresh-session state, and (c) a before/after file inventory including DOE document, `DoeStudies` folders, ROI `.GEO`/`.COR`, `.MST.TEMP`, and the main MST. Preserve original and candidate outcomes separately if the reference crashes. Do not upgrade helper unit coverage or a static decompile receipt into acceptance.
