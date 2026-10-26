# CP63 A55 mode qualification

## Result

The retained evidence does **not** bind A55's original `MoInputMode` to either 0 or 1. The settings snapshot makes mode 1 plausible, but using it as A55's established input would overstate the record. Consequently the historical A55 byte delta remains unqualified by mode, and must not be normalized away or treated as a candidate defect yet.

The snapshot at `evidence/gui_mo/oracle_b/_tools/settings_backup_l5s/DEFORM-MO.ini` contains `[Setting] MoInputMode=1`; its recorded SHA-256 is `909d618a352e7f1c972a53b887aa5978e933d875ff4b260a8dd107fa4797b4af`. Its timestamp predates the retained `rs_run.out` A55 close entry, but the settings folder is a generic Oracle-B backup and contains no A55 process/session identifier or capture-time link. The A55 log records the close action, not the process's INI path/value or intervening mode changes. The `.moproj` itself does not carry this global preference. Timestamp ordering alone is insufficient to bind the snapshot to A55.

## Candidate-side qualification

The historical candidate folder sweep at `source/apps/moproj2mst/tests/s/test_save_folder.cpp:305` invokes the C++ conversion overload without `OriginalSettings`; `OriginalSettings::moInputMode` defaults to 0. This establishes that the candidate side used mode 0, not that A55's original used mode 0.

The candidate's C ABI can accept `M2M_MO_INPUT_MODE` (unset/empty maps to 0; `1` selects the nonzero branch). This permits a safe, disposable, candidate-only diagnostic without GUI control:

1. Copy the pristine A55 `*_before` project to a disposable short-path directory and hash the copied inputs.
2. Run the same prebuilt candidate CLI in that copy with the environment unset, preserving exit status, stdout/stderr and output-tree hashes.
3. Reset from the pristine copy and run with `M2M_MO_INPUT_MODE=1`; preserve the same receipts and verify the input copy remained unchanged.
4. Interpret this only as candidate API behavior. The mode-1 public lifecycle currently stops at the qualified `0x6504abb0` palette/setup guard before the editor setup/fill and cannot produce a comparable Save tree. A successful mode-0 tree is not a mode-matched comparator if the original was mode 1.

Existing `simcontrol300` model tests can exercise the narrow mode-1 CG transformation, but that projection is not a completed public lifecycle comparator. No candidate-only run can establish what mode the historical original process had.

## What the raw delta does and does not establish

The retained raw comparison is exact: `Operations/Task00002/SimCtrl.KEY` differs at offsets 1439 and 1455 only; candidate values are ASCII `0`, original values ASCII `1`. These are the second/fourth CG values after `SOLMTD 2 4`, yielding candidate `[0,0,0,0]` and original `[0,1,0,1]`.

Static original evidence identifies the mode branch: factory `0x1405ff0b0` selects editor100 for mode 0 and editor300 for nonzero mode. In the editor300 path, the initial Iteration fill can check the CG radio for solver 2; if that changes unchecked→checked and method 64 receives `toggled(true)`, the callback sets precisely the second and fourth values to 1 under the observed parameter conditions. This is a plausible explanation only if mode, active operation, setup/fill, and the state transition all match. The retained A55 capture does not establish those runtime preconditions. Static call evidence does not substitute for a captured execution trace.

## Minimum evidence to resolve the mode question

At the next permitted original comparison, record the actual `DEFORM-MO.ini` path, file hash and `[Setting] MoInputMode` immediately before launching the pristine A55 copy. Record any mode-setting actions/signals before or during Open, then capture the first active tree operation and the CG checked-state transition at initial Iteration fill. Save the post-fill model values and raw Save outputs with hashes. Only compare candidate output under that proven mode; if mode 1, the current candidate's documented setup refusal is a refusal receipt, not a parity result.

The candidate-side mode branch and callback projection are supported by `analysis/resume62-a55-mode-chain.md`, `analysis/wave2-save-diffs-diagnosis.json`, and `analysis/wave3-boolean-external-callback-contract.json`. No original runtime, GUI control, source edit, build, test run, or mutable REA operation was performed for this qualification.
