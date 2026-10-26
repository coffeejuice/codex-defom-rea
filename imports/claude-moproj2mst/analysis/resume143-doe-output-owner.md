# CP143 — DOE Output post-mkdir ownership and next-effect boundary

## Finding

The first effect is the private CP142 `<Study Control>/Regions` mkdir attempt, but its failed-attempt contract must remain distinct from native continuation. The retained native contract says `0x14081afb0(path, true)` displays the one-button warning when `QDir::mkpath` fails and nevertheless **returns true unconditionally**. After the user dismisses that warning, `0x140e2e730` continues into its later effects. CP142's `WarningModalRequired` can describe the UI that must be shown, but must not be treated as a terminal outcome or as permission to skip cleanup / later Save work. This is a concrete qualification for any future caller integration.

## Native order and owners after the attempt

The source-qualified order in `analysis/wave2-doe-lifecycle.md` (original image SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`) is:

1. `0x140e33210` requests `Control/Regions` through `0x14081afb0(..., true)`.
2. The callback obtains the operation viewer model through `0x140d0d670`. If there is a model, `0x14081b400` clears entries beneath `Control/Regions`: dot entries are skipped, directory entries are recursively removed by `0x14081b2b0`, and regular-file removal results are ignored. It retains the `Regions` root. The model is a gate for this cleanup; a path-only `FileEffects` owner does not establish the returned viewer model or its identity.
3. The callback independently requests removal of `Regions/Common_Mesh.Key` and `Regions/Common_Mesh_User.Key`, ignoring removal failure. ROI 0 selects the first name; ROI 1 selects the second. Geometry output `0x140895f30` additionally requires both a nonempty selected name and the cached common-mesh object. Neither ROI scalar nor the cached object is currently bound to the actual active operation instance in candidate public Save.
4. It invokes `0x1400543a0` (current-operation/property-to-engine step), then DoeOutput `vt+0x80`, `0x140e2fcd0`, which updates the active SubProject session document and serializes `<Control>/<project-base>.DS` through `0x140645030`.

The `.DS` document owner is the lazily created `SubProject+0x30` session document. Candidate `Study::outputDocument` is a Study-owned projection used by `doe::finishCurrentOutputClassData` and DoeOutput read helpers; this naming/structural similarity does **not** prove it is the same instance or supplies the original parented Qt document's linked-document and refresh behavior. Native Save synchronizes `SfTaskDoe` selection and operation/model state before serialization. The retained lifecycle report also says ordinary `SfTaskDoe` refresh updates existing DOM children/attributes and task lists; it does not imply complete document reconstruction.

## Candidate boundary

`src/doe/output_regions_mkpath_prefix.cpp::attemptOutputRegionsMkdirPrefix` establishes only the active Type2 Project/SubProject/Study owner and the immediate directory attempt. It has no viewer model, cached common-mesh object, ROI selection state, session-manager/task binding, parented Qt document, or `.DS` serializer owner. `src/doe/output_data.cpp::finishCurrentOutputClassData` is a class-data load projection: it selects the already-loaded `Study::outputDocument` current part/task, projects first ObjList/OprList values, resolves and resets the operation's ROI model, and stops at nonempty region geometry or constraint payload. It is not a Save callback or a continuation token after CP142.

The smallest ordered private next effect is therefore **ROI-model-gated Regions cleanup**, not immediate Common_Mesh deletion: reproduce the exact recursive direct-child cleanup only after binding the same active operation's viewer/ROI model result. Before that can be source-qualified, the missing owner witness is the `0x140d0d670` return for the same active `SfOprDoeOutput` occurrence, including whether null suppresses cleanup. Geometry remains separately gated by ROI choice, selected filename, and cached common-mesh pointer.

Do not wire the private helper into public Save from these facts. Public Save still reaches the refresh-delivery guard before the DoeOutput Save class branch (`engine/save_current.cpp`), and native DBLIST admission ownership is also unresolved in the active Type2 load path. This static order does not close either earlier gate.

## Evidence and limits

- `analysis/wave2-doe-lifecycle.md`: native output cleanup, geometry, current-operation and `.DS` order; source-qualified findings and explicit failure behavior.
- `analysis/wave2-doe-output-data-contract.json` and `analysis/wave2-doe-output-data-leaves-receipt.json`: candidate load-time Study/class-data prefix and original selected-task/list/zero-region leaf receipts; they do not bind the Save viewer/model instance.
- `source/apps/moproj2mst/src/doe/output_regions_mkpath_prefix.cpp`: isolated candidate mkdir attempt only.
- `source/apps/moproj2mst/src/doe/output_data.cpp`: candidate load-time selection/model-reset prefix and geometry/constraint guards.
- `source/apps/moproj2mst/src/engine/save_current.cpp::saveCurrentOperation`: public DoeOutput Save intentionally refuses before vt+0x68; it cites the missing viewer model and SubProject+0x30 document.

Static retained evidence only. No source/build/GUI/REA mutation or runtime observation was made. Modal dismissal behavior is inferred only to the extent explicitly recorded in `wave2-doe-lifecycle.md`; no claim is made about nested event timing or other warning branches.
