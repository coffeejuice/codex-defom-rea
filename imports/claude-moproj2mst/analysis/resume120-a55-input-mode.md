# CP120 — A55 editor300 mode provenance and CG event

## Finding

No retained receipt binds the original A55 process to editor300 (`MoInputMode != 0`), and no retained runtime trace proves that the CG `toggled(true)` slot was delivered. The exact `[0,0,0,0]` → `[0,1,0,1]` Save delta is explained by the original editor300 slot only under explicit preconditions. It remains a compatible hypothesis, not an attribution.

## Mode provenance

The retained A55 driver `evidence/gui_mo/oracle_b/_tools/rs.py::drive` launches `DEF_GUI_MO_64.exe` with no project/mode arguments, inherits the Python process environment, and sets the working directory to `_tools`. It then opens the prepared `.moproj`, invokes File→Save, and closes. The driver records no PID-correlated settings path, INI bytes/hash, parsed `[Setting] MoInputMode`, environment override, or mode action. `rs_run.out` reports the Save/Close result only.

The only nearby settings artifact, `evidence/gui_mo/oracle_b/_tools/settings_backup_l5s/DEFORM-MO.ini`, says `MoInputMode=1` and has hash `909d618a352e7f1c972a53b887aa5978e933d875ff4b260a8dd107fa4797b4af`. It is a generic backup with no A55 launch/session binding. Its location beside the driver does not establish that the binary read that file; the driver supplies no explicit config path. The A55 project itself has no `MoInputMode` field. The candidate's historical sweep used mode 0 because its C++ conversion call omitted `OriginalSettings`, whose default is 0. Thus only the candidate mode is known.

Static input-mode evidence narrows possible mutation: original producers `0x14002fc20` and `0x14002fe50` set global mode 0/1 and emit `signalMoInputModeChanged`; their identified direct callers are action thunks `0x14000ace0`/`0x14000ad10`. The retained Open wrapper `0x1400271b0` does not directly call those producers or emit that notification. This is a bounded static caller result, not proof that no indirect or earlier action occurred in the A55 session. The evidence therefore cannot derive launch mode from the Open sequence alone.

## CG slot chain and dependency

Static evidence from the retained original binary (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`) establishes this conditional editor300 path:

1. Factory `0x1405ff0b0` selects editor100 for mode 0 and editor300 for nonzero mode.
2. Editor300 setup calls `connectSlotsByName` (`0x140f21d67`); metadata method 64 names `on_m_rbtnDefSolCG_toggled(bool)` and dispatches to `0x140ed01b0`.
3. Initial iteration fill `0x140ec19f0` checks the CG radio at `0x140ec21b6` when the current model's `SOLMTD` is 2. The A55 Task00002 input has `SOLMTD 2 4` and four zeros.
4. If the button's state changes false→true and Qt delivers method 64, the slot sets S2 (`SC+0x2f8`) to 1 when truncated S2 is `<=0` and sets S4 (`SC+0x308`) to 1 unless truncated S4 is 1 or 5. That predicts the two retained byte changes while preserving S1/S3.

The `setChecked(true)` instruction proves a call, not the pre-call checked state or emitted signal. No A55 event record establishes current active operation as Task00002 at fill, successful editor setup/auto-connect, checked-state transition, or callback delivery. Static widget/default hints do not bind those values to the historical Save session. Current candidate mode-1 public path is separately guarded at palette/setup before it can exercise this sequence; its model projection cannot substitute for the original event witness.

Supporting retained qualifications: `analysis/wave2-save-diffs-original-iteration-fill.asm`, `analysis/wave2-save-diffs-original-meta-dispatch.asm`, `analysis/wave2-save-diffs-original-slot-identity.json`, `analysis/wave2-save-diffs-diagnosis.json`, `analysis/wave3-boolean-external-callback-contract.json`, and prior bounded audits `analysis/resume62-a55-mode-chain.md`, `analysis/resume79-a55-mode-matched-trial.md`, `analysis/resume116-a55-mode-source.md`.

## Smallest missing dependency

Use one fresh original process on a pristine A55 copy and retain one process-bound checkpoint containing the actual settings file path/hash/value at startup and any mode setter before editor creation; then record first active operation, Task00002 solver/count/parameters, CG checked state before/after iteration fill, whether method 64 runs, post-fill values, and raw Task00001/Task00002 Save hashes. The first missing fact is the actual process settings provenance; the decisive CG fact is the false→true transition plus delivered callback. Without those receipts, neither the generic INI nor the historical output can resolve original A55 mode or event causation.

No GUI control, mutable REA database, source edit, build, or test was used or changed for this audit.
