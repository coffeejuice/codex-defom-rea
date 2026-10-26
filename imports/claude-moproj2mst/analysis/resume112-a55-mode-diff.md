# CP112 — A:RS_55 SimCtrl mode difference

## Result

The retained raw mismatch is real, but there is no source-backed correction to make yet. The original Save process's `MoInputMode`, active operation at the first editor fill, and CG radio transition are not bound to the A55 capture. The retained source explains the exact two-byte delta only under those missing runtime conditions. Do not normalize the KEY reader/writer or apply a default CG transformation.

## Raw oracle and candidate

`analysis/wave2-save-diffs-reference-inspection.json` and `analysis/wave2-save-diffs-rs55-capture.json` retain the original and candidate `Operations/Task00002/SimCtrl.KEY` artifacts. Both are 2,146 bytes. The original Save has SHA-256 `79f9ec098a22b46f60e23072751a23fa036459f4db02f719e56e6421f243f79b`; the candidate has SHA-256 `2bee048e6645fd36416d1b7582883c65a9727e5756715302a928ca3f1656854e`. They differ only at zero-based offsets 1,439 and 1,455: candidate ASCII `0`, original ASCII `1`. Immediately after `SOLMTD 2 4`, the values are candidate `[0,0,0,0]` versus original `[0,1,0,1]`. The candidate was produced from a frozen executable with `--write-save`; the capture explicitly says `original_process_executed: false`.

The A55 runner log establishes a fresh original process and File→Open→Save→Close sequence, but does not record its INI path/content, any mode change, active operation during editor fill, CG initial state, or slot delivery. The generic retained Oracle-B `DEFORM-MO.ini` says `MoInputMode=1` (SHA-256 `909d618a352e7f1c972a53b887aa5978e933d875ff4b260a8dd107fa4797b4af`), but has no A55 process/session binding. The `.moproj` does not store this global setting. Conversely, the historical candidate folder sweep calls the C++ overload without `OriginalSettings`; it therefore used the default `moInputMode=0`. The comparison is not established as mode-equivalent.

## Source-qualified conditional explanation

Retained original static evidence is tied to executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`:

1. Factory `0x1405ff0b0` selects editor100 for mode 0 and editor300 for nonzero mode.
2. In editor300, Initial Iteration fill `0x140ec19f0` sees solver `SOLMTD==2` and calls `setChecked(true)` on the CG radio at `0x140ec21b6`.
3. Auto-connect metadata method 64 maps `toggled(bool)` to `on_m_rbtnDefSolCG_toggled(bool)`, whose body is `0x140ed01b0`. If the radio changes unchecked→checked and that callback is delivered, then with parameter count 4 it stores 1 at `SC+0x2f8` when S2 truncates to `<=0`, and stores 1 at `SC+0x308` when S4 truncates outside `{1,5}`. This transforms `[0,0,0,0]` to `[0,1,0,1]`; the first and third values are not changed by these conditions.

This is conditional, not a traced A55 execution. `setChecked(true)` need not emit `toggled(true)` if the button is already checked; the original setup source does not establish the button's initial state. The A55 capture also does not establish that Task00002 was the active operation at the first fill.

## Candidate writer boundary and decision

`source/apps/moproj2mst/src/engine/save_current.cpp::saveSimCtrl` passes the current `DB_STEP_DATA` to `write_simctrl_key_stream`/`write_simctrl_key_file`. The writer in `source/port/defkey/src/item_writers/task_key_writers.cpp` stores the format flags and writes the group-1 model state; it has no UI-mode or CG-event input. Inserting a writer normalization would invent a UI event for other saves and violate the explicit current model state. The candidate public mode-1 editor path currently stops at its qualified palette/setup guard before setupUi, auto-connect and Initial Iteration fill, so it cannot currently serve as a full mode-1 reproducer either.

No exact correction is supportable in the writer or the setup/test-only lane. The smallest resolving witness is a mode-bound original Open→Save record for the pristine A55 input that captures: actual INI path/hash/value and mode changes; first active operation; Task00002 SOLMTD/count/values; CG checked state before and after Initial Iteration fill; whether method 64 was delivered; post-fill model values; and raw Task00001/Task00002 KEY hashes. Candidate parity should then use that proven mode; mode 1 remains unavailable through the current public candidate guard.

## Evidence limit

This is a retained-artifact and static-source audit only. No source, tests, build, GUI session, original runtime, or mutable REA state was changed or used. Supporting qualifications: `analysis/resume62-a55-mode-chain.md`, `analysis/resume63-a55-mode-qualification.md`, `analysis/resume79-a55-mode-matched-trial.md`, `analysis/wave2-save-diffs-diagnosis.md`, and `analysis/wave2-save-diffs-rs55-capture.json`.
