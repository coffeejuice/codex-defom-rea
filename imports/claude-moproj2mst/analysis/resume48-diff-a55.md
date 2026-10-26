# Resume 48 diff lane: A:RS_55_Multiple_Operations_Example1

## Checkpoint47 result

`analysis/wave3-checkpoint47-validation.json` and `analysis/resume47-combined-full-tests.json` retain the folder-sweep result as `DIFF`, one differing file, no missing or extra files, and no comparator adjustment. This is a historical comparator result, not fresh-original acceptance. The runner explicitly excludes corpus case 55 from the ordinary corpus comparisons; the diagnostic comes from the retained Oracle-B folder case.

The exact changed file is:

`Operations/Task00002/SimCtrl.KEY`

Candidate and historical original Save files are both 2,146 bytes. `analysis/wave2-save-diffs-diagnosis.md`, `wave2-save-diffs-diagnosis.json`, and `wave2-save-diffs-reference-inspection.json` locate exactly two byte differences at zero-based offsets **1439** and **1455**. At line 52, immediately after `SOLMTD       2       4`, the candidate has `0 0 0 0`; the retained original Save has `0 1 0 1`. Candidate hash: `2bee048e6645fd36416d1b7582883c65a9727e5756715302a928ca3f1656854e`; historical original Save hash: `79f9ec098a22b46f60e23072751a23fa036459f4db02f719e56e6421f243f79b`. The single-file total is consistent with the broader comparator's 1 differ / 0 missing / 0 extra.

## First source-qualified explanation

The narrowest source-qualified original explanation is the `SfPreSimControl300` slot **`on_m_rbtnDefSolCG_toggled(bool)` at `0x140ed01b0`**, reached during the iteration-page fill `0x140ec19f0` when solver value 2 checks the CG radio at UI `+0xa90` (`0x140ec21b6`). The retained metadata/setup evidence ties its auto-connected method to dispatch method 64. With the CG control enabled, this slot normalizes parameter 2 (`SC+0x2f8`) to `1` when its truncated value is `<= 0`, and parameter 4 (`SC+0x308`) to `1` unless its truncated value is 1 or 5. That accounts for the two observed zero-to-one changes.

The original `SOLMTD` reader at `0x140b69090` copies the four explicit input integers into model doubles and does not normalize them. The writer preserves the current model values. Therefore the difference is not explained by general KEY input parsing or a writer-side constant. The active reconstructed Forming model path currently uses the smaller `SfPreSimControl` path in `source/apps/moproj2mst/src/engine/loader_l3.cpp`; the editor300 iteration state and radio-toggle slot are not yet integrated at the fresh lifecycle boundary.

Qualification matters: the historical pair proves the saved output differs and the retained original static evidence explains a plausible concrete mutation. It does **not** prove the same GUI slot runs in a fresh original session for this fixture; original execution was not performed for the diagnosis. Six retained paired SimCtrl cases keep zero parameters for other tasks, including this project's inactive Task00001, so a global or every-task normalization would exceed the evidence.

## Smallest next action

Trace and source-qualify the active Forming editor's creation and iteration-page fill order for the selected fresh one-project lifecycle, specifically whether `SfPreSimControl300` is instantiated and whether automatic radio-toggle delivery occurs before Save for the active Task00002. Use the retained original fill, metadata, and slot receipts first; only capture the missing native sequence if those receipts do not settle the ordering. Keep the fix scoped to enabled CG iteration fills and the validated parameter rules, with regression checks that preserve valid S4=5 and leave inactive Task00001 unchanged. Do not normalize in the KEY reader, writer, or all model/task instances.

After the lifecycle invocation boundary is established, integrate the narrow editor-state update and compare the two exact bytes plus the complete SimCtrl file on a disposable fixture. A fresh original-first run remains necessary before treating the historical saved row as the expected fresh-session result.

## Evidence inspected

- `analysis/wave3-checkpoint47-validation.json`, `analysis/resume47-combined-full-tests.json`.
- `analysis/wave2-save-diffs-diagnosis.md` and `.json`.
- `analysis/wave2-save-diffs-reference-inspection.json`.
- `analysis/wave2-save-diffs-original-slot-identity.json`, `wave2-save-diffs-original-iteration-fill.asm`, `wave2-save-diffs-original-meta-dispatch.asm`, `wave2-save-diffs-original-ui-solver-state.asm`, and `wave2-save-diffs-original-reader.asm` (retained source/image analysis, not fresh runtime evidence).
- `source/docs/re/moproj2mst/runner/cases.tsv` and `source/docs/re/moproj2mst/runner/coverage.md` for case-55 age and exclusion qualification.

No source/build/app/REA data was modified or executed in this audit.
