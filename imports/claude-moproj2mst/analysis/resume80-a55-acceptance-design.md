# CP80 A55 mode-matched acceptance design

## Acceptance question

Does a fresh Open→Save of the same A55 before-state produce the two observed CG parameter changes under an equivalent, recorded `MoInputMode` and active-operation context? The retained historical artifacts show a precise raw delta, but they do not bind the original process setting. A generic `DEFORM-MO.ini` backup with mode 1 is a hypothesis only; the candidate folder-sweep overload defaults to mode 0. Preserve the historical files and hashes as historical evidence, not as the fresh trial's expected output.

Current status remains `blocked_external_gui_release`. Do not begin the trial or acquire GUI control until a supported release operation and independently verifiable release receipt/status exist.

## Exact fixture and action profile

Use only the pristine retained input directory:

`evidence/gui_mo/oracle_b/RS_55_Multiple_Operations_Example1_before/`

It contains 28 files, including `Multiple_Operations_Example1.moproj`, `Multiple_Operations_Example1.MST.corpus`, and both tasks' SimCtrl input files. Before copying, record a sorted relative-path/byte-count/SHA-256 manifest of the entire tree, plus separate hashes for:

- `Multiple_Operations_Example1.moproj`
- `Operations/Task00002/SimCtrl.KEY` (the changed file)
- `Operations/Task00001/SimCtrl.KEY` (negative control)
- `Multiple_Operations_Example1.MST.corpus`

The original historical runner description in `_tools/rs.py` is one fresh process for this case: open the copied project, answer observed Open/Save dialogs with their recorded default Enter action, Save, close with “No” to discard close-time operation edits, then close the owned process. The accepted comparison action must stay this narrow. Do not click Simulation Controls, switch operations/modes, or otherwise edit the model to manufacture the expected callback.

Use a disposable root such as `C:\PROBLEM\a55r\run\Multiple_Operations_Example1.moproj`; verify the complete project, side-file, temporary and generated paths are shorter than 100 characters before launch. Run original and candidate serially against the **same exact absolute path**, restoring the project tree from the pristine manifest between runs. Copy each run's output tree and logs to separate receipt directories before resetting. Do not reuse the historical after-folder as the trial output.

## Mode and process provenance (must be captured before opening)

The exact mode is currently unknown. Bind it from the original process's effective settings, without assuming that the retained backup was active and without modifying a user profile:

1. Record original executable absolute path, file/product version, SHA-256, process ID, parent, command line, working directory, start timestamp and Windows account/session identity.
2. Record the exact `DEFORM-MO.ini` path used by that process, raw file SHA-256, parsed `[Setting] MoInputMode`, and file timestamp. Record the effective mode at process start and any explicit mode-setting event/signal before/during Open. If the application can read a different config source or the effective value cannot be bound, stop classification as mode-unknown.
3. Hash the settings file again after the trial and preserve before/after hash/value. Do not adjust the setting during this comparison.

The old generic settings snapshot at `evidence/gui_mo/oracle_b/_tools/settings_backup_l5s/DEFORM-MO.ini` reports mode 1 with SHA-256 `909d618a352e7f1c972a53b887aa5978e933d875ff4b260a8dd107fa4797b4af`; it has no A55 process/session binding. The original run log records “close dialog 1 / Question answered No” and a 3,541-byte saved MST, but no setting or event trace. Those receipts remain historical and must not substitute for the process-specific capture above.

## Native decision checkpoints

Capture a compact, timestamped trace around the first Forming operation/editor fill:

1. Effective `MoInputMode` and selected tree/current operation immediately before the editor factory. Record whether Task00002 is the active item and which class/factory is selected.
2. For Task00002, record `SC+0x2e4` solver, `SC+0x2e8` parameter count, values at `SC+0x2f0`, `+0x2f8`, `+0x300`, `+0x308`, and the CG radio's checked state immediately before the initial Iteration fill.
3. Record whether the initial fill executes, whether `setChecked(true)` changes unchecked→checked, whether the `toggled(true)` event reaches metadata method 64 / `on_m_rbtnDefSolCG_toggled(bool)` at `0x140ed01b0`, and the four model values immediately after the event and after Open.
4. Record Task00001's corresponding four values before/after as a negative control. Do not generalize the update across inactive models.

The retained static rule predicts a change to `[0,1,0,1]` only if mode is nonzero, Task00002 is active, solver is 2, parameter count is 4, setup reaches the first iteration fill, and `setChecked(true)` causes an unchecked→checked transition that emits `toggled(true)`. A button already checked may not emit a change signal. Mode 0 chooses editor100, so this editor300 callback is not expected from that path. These are static decision conditions, not proof of historical execution.

## Raw side-file receipts and comparison

For each original and candidate run, preserve:

- Complete before and after sorted manifests: relative path, byte count, SHA-256, and whether present before. Include `.moproj`, output `.MST`, `MST.corpus`, all `Operations/Task00001/*` and `Operations/Task00002/*` files, and any created/deleted/renamed files. Keep `opened.png`, dialog captures, trace/logs and other provenance sidecars in a separate evidence inventory, not mixed into generated-project output comparison.
- Raw bytes and SHA-256 for output `.moproj`, output `.MST`, Task00002 `SimCtrl.KEY`, Task00001 `SimCtrl.KEY`, and every file changed from the before manifest. The historical raw pair hashes are Task00002 candidate `2bee048e6645fd36416d1b7582883c65a9727e5756715302a928ca3f1656854e` and original `79f9ec098a22b46f60e23072751a23fa036459f4db02f719e56e6421f243f79b`; these are reference identifiers only, not expected hashes for the new run.
- Action/response record for each dialog, Save completion evidence, Close-No response, owned-process exit code, and final process-liveness check. Record refusals, crashes and partial outputs without cleanup or retry that would overwrite the first result.
- A raw bytewise tree comparison first. Report every differing/missing/extra path. Do not normalize paths, banners, line endings or `.moproj` view values for this case. The repeated run uses the same absolute root, so path-root mapping should not be needed.

## Candidate pairing and interpretation

After the original run, preserve its snapshot, restore a hash-verified pristine copy to the same project path, and run the candidate with the **same captured mode** through an entry point that explicitly accepts `OriginalSettings`; record exact argv/API, cwd, environment and settings values. Do not assume the folder-sweep overload is mode-matched—it defaults to 0.

- If original effective mode is 0, the editor300 callback cannot explain the historical transition. Compare the fresh raw trees in mode 0 and investigate any remaining difference at its first observed source decision.
- If mode is nonzero and all callback preconditions/events above are observed, the original trace supports the exact `[0,1,0,1]` transition. The current candidate public mode-nonzero route is known to stop at `0x6504abb0` before setup/fill; preserve that as a candidate refusal, not a parity result. Complete the missing candidate editor setup/fill path in a separate qualified change before a final mode-matched output comparison.
- If mode is nonzero but Task00002 is inactive, the CG button is already checked, fill does not execute, or the slot is not delivered, do not attribute the raw delta to this callback. Preserve the trace and locate the actual earliest mutation before changing code.
- If effective original mode, active operation or callback delivery is unobservable, label the trial inconclusive; do not map it to the retained two bytes.

## Historical versus fresh evidence

Historical evidence: the original `rs.py` action recipe, saved A55 tree, old settings backup, screenshot, run log, original/candidate raw KEY hashes and static PE callback analysis. These establish the old action class and a conditional source explanation only.

Fresh independent evidence required: process-bound effective setting and file hash; fresh pristine copy and manifest; owned original and candidate process identities; equivalent recorded mode/action; active-operation and before/after callback checkpoints; complete raw output manifests and hashes. Only this new receipt can establish fresh mode-matched behavior. It is not available now because the release gate is closed.

This is acceptance design only. No GUI/app control, source, build, test, fixture, settings, or mutable REA state was changed or run.
