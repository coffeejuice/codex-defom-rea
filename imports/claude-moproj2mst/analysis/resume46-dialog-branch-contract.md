# Focused branch contract: Save last-operation warning

## Selection and evidence boundary

The invalid last-operation Save warning is the highest-impact dialog candidate in the prior RDIALOG audit: it lies directly on the selected Save path and currently causes an unconditional port refusal. Existing retained static evidence is sufficient to define a focused candidate branch test without another original capture. It is not runtime evidence, and the test must remain qualified as static-contract-driven until independent original acceptance is available.

The retained contract `analysis/wave3-close-save-return-contract.json` is based on offline original PE/DUMPBIN inspection, with original binary SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; it explicitly says no runtime, app control, DLL loading, or builds. It records:

1. Original `0x1400b8fb0` scans the connector chain and warns when a must-be-last operation precedes another item.
2. The QMessageBox call passes only the Yes button (`0x4000`) and requests No (`0x10000`) as default. The Qt enum wrapper forwards the absent No default; it does not create a Yes|No choice. The shared helper creates the sole Yes button, and the receipt derives a fresh answer of Yes (`0x4000`).
3. The helper returns that answer. Save `0x1400980d0` returns immediately on any per-subproject result other than Yes, before current-operation Save and output writers. The normal Close caller maps Yes to Cancel, retaining the project.
4. Modal event delivery remains an explicit opportunity and is not recovered. No due timers, event callback effects, or elapsed-time behavior should be asserted by this test.

The current port decision in `source/apps/moproj2mst/src/engine/save_project.cpp` differs: `lastOperationCheck` detects `SfOprShapeOptim`, `SfOprDoeOutput`, or `SfOprReport` before the final chain item, sets `lastOperationModalPending=true` and `pendingDefault=Yes`, then throws `M2M_E_UNSUPPORTED`. It preserves the prior `call.answer` (which can be No from an earlier checked subproject), marks no completed validation, and prevents downstream writes by exception. `tests/s/test_save_answer.cpp` already covers that pending-state/error behavior; it does not establish the original answer return or caller mapping.

## Focused test to add when implementation is assigned

Add a unit case beside the existing `last_operation_warning_keeps_pending_default_separate_from_returned_answer` case in `source/apps/moproj2mst/tests/s/test_save_answer.cpp`. Construct an opened project with one nonnull subproject containing a `SfOprReport` item followed by an ordinary item, and invoke the Save-validation seam.

Expected branch assertions, derived from the retained source contract:

- The result is the modeled Yes dialog answer (`DialogAnswer::Yes`), rather than `M2M_E_UNSUPPORTED`.
- The offending operation is identified as the warning target and the warning branch is recorded; do not model a No alternative, because the shipped message box exposes only Yes.
- The per-subproject check returns Yes, so `checkedSubprojects` advances as the original caller does after a normal check. Validation may then continue to the next subproject; with a single subproject it reaches the normal post-validation point.
- The Save routine itself must preserve the original caller boundary: the existing nested-Save contract says a Yes per-subproject answer stops `0x1400980d0` before current-operation Save and project/property/MST output writes. In this branch, therefore, assert zero current-operation and file-writer attempts and the corresponding caller-facing answer, not successful Save.
- Where the Close composition seam is covered, assert that this returned Yes maps to Cancel and leaves the project attached. Keep this as a separate caller-level assertion if the unit seam cannot exercise Close without broad fixtures.
- Do not assert zero modal event delivery, timer state, scene callback state, or GUI behavior. Those remain outside this static branch unit test.

The current test fixture may need a production seam change: `saveCurrent` currently communicates warnings via exception, while the faithful branch requires a returned answer and continuation/short-circuit at its caller. Keep fixture construction minimal and test the exact per-subproject answer separately from file-writer booleans; do not turn `M2M_FLAG_STRICT` warnings into the original dialog answer.

## Remaining evidence gate

This defines a focused implementation test, not acceptance of the default-dialog contract. The retained static decompilation supplies the button configuration and caller result flow, but does not prove application startup style/customization or fresh runtime event scheduling. Final RDIALOG closure still needs a fresh original-process fixture receipt showing the dialog, actual default answer, caller messages/status, continuation, and unchanged/surviving writes; keep that original capture separate from the unit test and preserve the modal event-delivery qualification.
