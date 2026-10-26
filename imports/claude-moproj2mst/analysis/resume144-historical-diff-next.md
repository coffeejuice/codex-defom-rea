# CP144 historical DIFF: A55 remains behaviorally unresolved

## Selection

Prioritize `A:RS_55_Multiple_Operations_Example1`. It has one retained changed file and no missing/extra files, so it is the most useful historical discrepancy to resolve after the already-isolated B98 path-only delta. Its expected and candidate values differ in simulation behavior state, not merely a copied root string. The retained output is associated with an actual Save action and a near-Save screenshot, but neither proves the event sequence or settings that produced the two values.

## Raw result and source-qualified decision

The raw differing file is `Operations/Task00002/SimCtrl.KEY`, 2,146 bytes on both sides. At byte offsets 1439 and 1455 (line 52 after `SOLMTD 2 4`), candidate values are `0 0 0 0`; retained Save values are `0 1 0 1`. Candidate SHA-256 is `2bee048e6645fd36416d1b7582883c65a9727e5756715302a928ca3f1656854e`; retained Save SHA-256 is `79f9ec098a22b46f60e23072751a23fa036459f4db02f719e56e6421f243f79b`. The retained folder comparator reports one differing file, zero missing/extra, and no adjustment. These are raw historical bytes; no normalization is warranted.

The narrow source explanation remains conditional. Original `SfPreSimControl300` iteration fill `0x140ec19f0` checks the solver field at `SC+0x2e4`; for value 2 it sets the CG radio checked at UI `+0xa90` (`0x140ec21b6`). Setup connects auto-slots by name. With `on_m_rbtnDefSolCG_toggled(true)` at `0x140ed01b0` reached, the callback sets `SC+0x2f8` from 0 to 1 and sets `SC+0x308` to 1 unless its value is 1 or 5. This explains the pair of changed values if the callback fires. The original SOLMTD reader copies the explicit integers and does not normalize them, so a generic reader or writer correction would be misplaced.

## Missing witness and smallest valid next comparison

The retained `opened.png` shows the Simulation Controls pane displaying `Operation forming (2)` shortly before the Save log entry. It does not reveal global `MoInputMode`, the INI path/hash used by that process, radio checked-state transition, auto-connect completion, or callback delivery. The runner marks this 2013 project pre-v11; the retained historical Save is therefore not a fresh equivalent-session witness for current v11 behavior. The settings backup with `MoInputMode=1` is hours older and does not establish the settings read by the A55 process.

The smallest decisive evidence is a source-bound fresh original Open→Save witness on a disposable copy, recording before launch the exact settings path/hash and parsed `MoInputMode`, then the active operation, editor factory route, radio state before/after iteration fill, toggle delivery, resulting four parameter values, and raw Task00002 `SimCtrl.KEY` bytes/hash. Preserve the original output and the candidate output separately. If the original witness shows the checked toggle occurs, the candidate source correction is to deliver that exact enabled CG transition at the same active-owner/fill point, after resolving the existing `0x6504abb0` editor300 palette/setup guard. If the witness shows the historical action required an explicit click or different mode, A55 is not a fresh Open→Save oracle and must remain qualified rather than driving a speculative correction.

Under the current workflow, GUI input remains blocked pending a supported app-control release operation/status. Therefore there is no valid original-free source change or candidate-only rerun that can decide this discrepancy. Existing isolated `normalizeCgAtOpen` tests already cover the rule and are not evidence that the public Session owner reaches it.

## Current freeze and claim limits

CP140 is the latest source-bound full validation inspected: 335 registered, 328 clean, six logged-gap suites, one logged-difference suite, no skips/failures/incomplete output, and no fixture changes. Historical status is still 93 SAME / 5 DIFF; objective completion is false. The newer CP142/143 ABI tests are separate unvalidated additions and do not affect those counts. Public Forming editor300, DOE Save, Boolean occurrence/owner, remesh handler/process, and native Close gates remain open. This report does not treat the other four historical differences as solved: B120/B121/B122 each compare a before tree lacking GUI-created post-action state; B98 has a raw absolute `KFREAD` path difference already separated by a narrowly qualified candidate comparison.

## Evidence reviewed

- `analysis/wave2-save-diffs-diagnosis.md`, `analysis/resume48-a55-cg-timing.md`, and `analysis/resume66-a55-retained-evidence.md` for raw bytes, source chain, screenshot/settings qualification.
- `analysis/wave2-save-diffs-original-iteration-fill.asm`, `wave2-save-diffs-original-slot-identity.json`, and `wave2-save-diffs-original-reader.asm` for bounded static source evidence.
- `analysis/wave3-checkpoint140-validation.json` and `analysis/resume140-root-integration.md` for latest validated classification and current lifecycle guard.
- `source/docs/re/moproj2mst/runner/cases.tsv` for case age/pre-v11 qualification.

No source, fixture, build, GUI, original application, or mutable REA state was changed or run.
