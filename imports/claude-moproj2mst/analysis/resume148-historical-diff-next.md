# CP148 historical Save DIFF audit and next evidence

## Latest validated classification

CP147 is the newest full source-bound validation inspected (`analysis/wave3-checkpoint147-validation.json`): 353 registered, 346 clean, six suites passed with logged gaps, one suite passed with logged differences, zero incomplete/skipped/failed, no fixture changes, objective false. Historical output remains 93 SAME / five DIFF, with the same case counts as CP140. The CP147 source tree hash is `be223aec2c01736c4a5994ad4c85e06a1128edc1d84584a61446a9cfc3e88363` over 3,540 files. CP148's new opt-in focused-CTest gate and SPRING2 numeric admission proposal do not constitute a new full validation or touch these five raw case results.

| Historical case | Retained raw result | Current qualification |
|---|---:|---|
| `A:RS_55_Multiple_Operations_Example1` | 1 differing file, 0 missing, 0 extra | `Operations/Task00002/SimCtrl.KEY` differs at bytes 1439 and 1455 (`0` candidate vs `1` retained Save). A native CG radio callback conditionally explains both mutations; actual original process mode and signal delivery are not bound. This is the highest-value unexplained *behavioral* difference. |
| `B:120_B_DOE` | 1 differing `.moproj`, 7 missing files | The before tree lacks the post-action DOE study/output. CP144/145 actual-reader constructor/notifier prefixes do not create that state or complete Save. |
| `B:121_B_DOE_VAR` | 1 differing `.moproj`, 8 missing files | The before tree lacks the post-action nonempty Type2 variable; current scalar reconciliation/public continuation remains guarded. |
| `B:122_B_DOE_GEO` | 1 differing `.moproj`, 10 missing files | The before tree lacks the geometry variable/DOE operation and GEO outputs. Current geometry/DOE prefixes remain partial and do not match the post-action state. |
| `B:98_B_DS3F` | 1 raw MST difference, 0 missing, 0 extra | Exact path-root value difference in `Gear_blank.MST`; existing candidate-only comparison isolates the one `KFREAD` path occurrence. Keep the raw DIFF; it is an environment-path qualification, not an unexplained Save algorithm difference. |

## Selected discrepancy: A55

A55 is the best next acceptance target because its raw delta is only two numeric bytes in one complete same-size file and corresponds to a concrete, conditional native decision. `analysis/wave2-save-diffs-diagnosis.md` binds candidate file SHA-256 `2bee048e6645fd36416d1b7582883c65a9727e5756715302a928ca3f1656854e` and retained file SHA-256 `79f9ec098a22b46f60e23072751a23fa036459f4db02f719e56e6421f243f79b`; the comparator reports no other file difference or adjustment.

Original static evidence says `SfPreSimControl300` iteration fill checks `SC+0x2e4`; for solver 2 it sets the CG radio checked, whose auto-connected `on_m_rbtnDefSolCG_toggled(true)` normalizes `SC+0x2f8` from 0 to 1 and `SC+0x308` to 1 unless it is already 1 or 5. The original SOLMTD reader copies the four values and is not the source of this mutation. Thus a reader clamp, writer substitution or all-task normalization would be unsupported.

The retained `opened.png` shows the Simulation Controls pane on `Operation forming (2)` shortly before the Save log entry. It does not show which `MoInputMode` the A55 process read, the CG checked transition, completed auto-connect, callback delivery or whether the screenshot and saved output share the exact event sequence. `cases.tsv` identifies this as a pre-v11 (2013) case. The old settings backup with `MoInputMode=1` is hours earlier and is not tied to A55's process. The raw pair is an actual retained Save outcome, but not a fresh, mode-bound current-session oracle.

## Smallest discriminating next action

No source correction is justified from current evidence. The smallest decisive action is a fresh original Open→Save capture after the GUI release gate is satisfied: bind the exact settings path/hash and parsed `MoInputMode`; record active operation and editor factory route; record CG radio state before/after iteration fill and whether `toggled(true)` delivered; then retain post-fill values and raw Task00002 `SimCtrl.KEY` bytes/hash. Compare candidate output only after the original trigger is established. If the original capture confirms that checked transition, resume R300 by resolving the selected editor's `QWidgetPrivate` palette/setup input at the existing guard `0x6504abb0`, then integrate the callback at the proven active-owner/fill point. If it shows no callback or a different mode/action, A55's historical output is not the expected fresh Open→Save result and must remain qualified.

The current status `analysis/resume146-gui-release-capability.md` and root `tools/gui_validation_status.py` report the supported release API and verifiable receipt/status are unavailable. The user's workflow rule therefore bars a new original GUI session. Independent work can continue on R300 static owner/palette inputs and other headless cases, but those cannot settle A55's process-specific event. CP144/145 DOE notifier/reader prefixes, CP146/147 geometry/codec prefixes, and CP148's focused CTest gate are separate areas and do not explain or repair these two A55 bytes.

## Completion boundary

Keep all five raw DIFFs in the ledger. Do not count DOE after-action missing files as ordinary Save defects without matched pre-state; do not treat B98's path-only qualification as raw equality; and do not infer A55's conditional native slot ran from the retained screenshot. CP147's suite counts and historical 93/5 are not objective completion, and CP142/143 public ABI tests are not part of the CP147 validation receipt. No source, build, fixture, GUI, original process, or mutable REA action was used for this audit.

## Evidence reviewed

- CP144–147 validation receipts: `analysis/wave3-checkpoint144-validation.json` through `analysis/wave3-checkpoint147-validation.json`.
- CP144–147 integration reports, especially `analysis/resume145-root-integration.md` and `analysis/resume147-root-integration.md`, for changes and retained guards.
- `analysis/wave2-save-diffs-diagnosis.md`, `analysis/resume48-a55-cg-timing.md`, `analysis/resume66-a55-retained-evidence.md`, and `analysis/resume146-gui-release-capability.md`.
- `analysis/resume148-format-next-admission.md` and `analysis/resume148-focused-ctest-gate.md` to distinguish queued/prefix work from new historical full-case evidence.
- `source/docs/re/moproj2mst/runner/cases.tsv` for case-age qualification.
