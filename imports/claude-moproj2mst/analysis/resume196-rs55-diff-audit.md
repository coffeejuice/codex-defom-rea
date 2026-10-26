# CP196 RS55 historical folder difference audit

The two-byte historical difference is real, but the retained original capture does not establish the mode and CG event needed to justify a production correction. There is no evidence-backed source/test fix to select in this lane. The smallest next requirement is a process-bound original witness, described below; keep the raw difference and the public editor300 guard.

## Frozen result and directly verified bytes

CP195 `analysis/wave3-checkpoint195-validation.json` records 93 SAME/five DIFF and specifically `A:RS_55_Multiple_Operations_Example1`: one differing file, zero missing/extra, no comparator adjustment. The frozen suite log `build/test-runs/1792979918197143000/m2m.s.save_folder.log:47` identifies `operations/task00002/simctrl.key`. This diagnostic suite does not fail assertions for differences. Its `compareCase` removes the scratch tree, so CP195 supplies a file-level observation, not retained fresh CP195 differing bytes. Do not attribute earlier offsets or hashes to a CP195 raw capture.

Read-only reinspection of the retained wave2 pair on this audit confirms both files are 2,146 bytes and every differing byte is:

| Zero-based offset | Retained candidate | Retained original Save |
|---|---|---|
| 1439 | ASCII `0` / `30` hex | ASCII `1` / `31` hex |
| 1455 | ASCII `0` / `30` hex | ASCII `1` / `31` hex |

The header is `SOLMTD       2       4`; line 52 is candidate `       0       0       0       0\r\n` versus original `       0       1       0       1\r\n`. No normalization was applied.

* Candidate: `build/wave2-save-diffs/rs55-1792753172387017300/project/Operations/Task00002/SimCtrl.KEY`, SHA-256 `2bee048e6645fd36416d1b7582883c65a9727e5756715302a928ca3f1656854e`.
* Original Save: `evidence/gui_mo/oracle_b/RS_55_Multiple_Operations_Example1/Operations/Task00002/SimCtrl.KEY`, SHA-256 `79f9ec098a22b46f60e23072751a23fa036459f4db02f719e56e6421f243f79b`.
* Candidate capture receipt: `analysis/wave2-save-diffs-rs55-capture.json`; frozen CLI `--write-save`, default answers, `original_process_executed: false`. Its executable hash is `40d408c577357ad37008bda417cfcd4dc376881170bced1dd4d5716ca6fce974`, distinct from CP195 executable hash `45856b1438ac25da0e3ac2230ab22ef33d2b5aa7b1d74f8a101ba188354a8f49`.

## Original actions and qualification

The retained runner `evidence/gui_mo/oracle_b/_tools/rs.py` copies the corpus input into the before/output trees, renames MST to `.MST.corpus`, starts one fresh original process, performs File Open, Save, Close, answers intermediate dialogs with Enter and the close question No, then requests WM_CLOSE. `rs_run.out:135-136` and `rs_log.tsv:92-93` record RS55 close No and saved MST size 3,541, exit 0, one dialog.

Those receipts lack the actual process-bound INI path/hash/value, mode changes, active operation during initial fill, pre-fill CG checked state, and delivered callback. The generic settings backup `settings_backup_l5s/DEFORM-MO.ini` contains `MoInputMode=1` (recorded SHA-256 `909d618a352e7f1c972a53b887aa5978e933d875ff4b260a8dd107fa4797b4af`) but has no RS55 process binding. The project does not supply the global mode. A fresh process and the screenshot do not establish those missing conditions.

Read qualifications `analysis/resume63-a55-mode-qualification.md`, `resume79-a55-mode-matched-trial.md`, `resume112-a55-mode-diff.md`, `resume116-a55-mode-source.md`, and `wave2-save-diffs-diagnosis.json` before interpreting the retained static listings. Original PE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Static factory `0x1405ff0b0` selects editor100 for mode 0 and editor300 otherwise. In editor300, iteration fill `0x140ec19f0` checks CG for solver 2 at `0x140ec21b6`; auto-connect `0x140f21d67` and metadata method 64 identify `on_m_rbtnDefSolCG_toggled(bool)` / `0x140ed01b0`. With a delivered true toggle and count 4, the slot changes second parameter <=0 to 1 and fourth outside {1,5} to 1, explaining this exact tuple conditionally. Checking an already checked radio need not deliver a toggle. These are static facts, not a trace of RS55.

## Current source boundary

The current source still matches CP195 archived bytes for `loader_l3.cpp`, `save_current.cpp`, `simcontrol300.cpp`, `test_save_folder.cpp`, and `test_simcontrol300.cpp` (direct comparison against `build/wave3-checkpoint195-verified/source.zip`). CP195 receipt source-tree digest is `172f658eaa1ed403772e8407d97d224301c4e2163d63252620e2cdb7b4a04aba` over 3,554 files.

`tests/s/test_save_folder.cpp::runCase` invokes the convert overload without `OriginalSettings`; `src/core/convert.cpp` supplies `OriginalSettings{}` and `include/m2m/common.hpp:45` defaults mode to 0. `src/ops/family.cpp:183` selects editor300 only for nonzero mode. Thus the candidate sweep is established as mode 0; the historical original mode is unresolved.

`src/engine/save_current.cpp:278` obtains the current step and forwards it to `write_simctrl_key_stream`/`write_simctrl_key_file`; `source/port/defkey/src/item_writers/task_key_writers.cpp:65` sets flags, emits banner and writes model group 1. Neither boundary receives a CG event. A writer/reader normalization would invent that event and alter unrelated or inactive model values.

`src/engine/loader_l3.cpp:665` begins the actual Session-owned editor300 factory and refuses at `0x6504abb0` before setup, auto-connect and fill. Its isolated legacy fixture route calls `simcontrol300::normalizeCgAtOpen` (`src/engine/simcontrol300.cpp:887`). `tests/mdt/test_simcontrol300.cpp` covers mode 0, the isolated mode-1 tuple, inactive-model preservation and parameter limits, and explicitly expects the public factory guard. These tests do not establish public mode-1 Save parity. Completing public editor setup is a broader recovery lane; bypassing it solely to match RS55 is unsupported.

## Exact missing witness and decision

Obtain one original Open→Save witness on an immutable copy of the retained before-tree, using the recorded default-dialog policy and an owned process. Keep generated paths under `C:\PROBLEM` and below 100 characters. Bind executable hash/PID and actual settings source/path/hash/`MoInputMode` at startup, plus any mode setter through Open. Record first active operation; Task00002 solver, count and four parameters; CG checked state before/after initial iteration fill; whether method 64/`0x140ed01b0` is delivered; and resulting model values before Save. Retain raw whole output tree and Task00001/Task00002 SimCtrl hashes as control/effect evidence.

If mode 0 is observed, this editor300 chain is excluded and the first actual mutation must be traced. If nonzero mode, active Task00002, solver 2, completed setup and unchecked→checked with delivered method 64 are observed, the exact tuple is explained and candidate parity must use that proven mode after its public guard is legitimately implemented. A candidate-only rerun cannot supply original mode/event evidence.

No source/test pair is ready to reserve or edit independently of the active CP196 connector owner. Retain the unresolved historical DIFF. Parent checkout `analysis/gui-control-lifecycle-assessment.json` reports unsupported release API; `python tools/gui_validation_status.py` returned `release_ready:false`. No new GUI session is permitted under that gate. This audit used no GUI, original process, build/test execution, mutable REA database, source/test edits or fixture edits; its sole write is this report.
