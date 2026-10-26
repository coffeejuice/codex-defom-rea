# CP218 RS55 historical A: diff and R300 reconciliation

## Decision

Keep `A:RS_55_Multiple_Operations_Example1` as an unresolved historical Save difference. No source, comparator, fixture, or test change is supported by the currently retained evidence. The exact two-byte effect is understood conditionally, but the original process mode and delivered GUI callback are not bound to the Save that produced the reference file. The smallest useful next item is the process-bound original Open→Save witness specified in CP196. A candidate-only rerun or another static disassembly cannot supply it.

## Difference and evidence chain

CP195's frozen validation (`analysis/wave3-checkpoint195-validation.json`) records 93 SAME/five DIFF, and identifies RS55 as one differing file with zero missing or extra files. Its diagnostic comparator does not fail on a difference, and the run removes its scratch tree; therefore it does not retain fresh CP195 bytes for this file.

The retained wave2 candidate/original pair remains directly inspectable and has equal 2,146-byte length. Its only differing zero-based offsets are 1439 and 1455: candidate ASCII `0`, original ASCII `1`. On `SOLMTD       2       4`, line 52 is candidate `       0       0       0       0` and original `       0       1       0       1`. There is no normalization in this comparison.

- Candidate `build/wave2-save-diffs/rs55-1792753172387017300/project/Operations/Task00002/SimCtrl.KEY`, SHA-256 `2bee048e6645fd36416d1b7582883c65a9727e5756715302a928ca3f1656854e`.
- Reference `evidence/gui_mo/oracle_b/RS_55_Multiple_Operations_Example1/Operations/Task00002/SimCtrl.KEY`, SHA-256 `79f9ec098a22b46f60e23072751a23fa036459f4db02f719e56e6421f243f79b`.
- `analysis/wave2-save-diffs-rs55-capture.json` binds the candidate to `--write-save`, default answers, and `original_process_executed: false`. Its executable hash differs from CP195's executable hash. Do not label these old pair bytes as a CP195 capture.

The original runner's retained log records RS55 close-No, a saved MST of 3,541 bytes, process exit 0 and one dialog. It does not bind the actual settings path/hash/value or mode changes to that process. A generic settings backup contains `MoInputMode=1`, but is not process-bound. Thus it cannot establish the mode used for the reference Save.

## Causal interpretation and why no correction is ready

The qualified static evidence summarized in `analysis/wave2-save-diffs-diagnosis.json` and `analysis/resume196-rs55-diff-audit.md` shows that the original editor300 iteration fill checks CG for solver 2. If an enabled true toggle is delivered with count 4, `on_m_rbtnDefSolCG_toggled(bool)` changes the second parameter when it is <=0 and the fourth when it is outside {1,5}. That behavior explains the retained tuple conditionally. A checked radio may not emit a toggle; static call order does not prove the event was delivered for RS55.

The original reader preserves explicit four-integer values, and six paired reference rows preserve zero second/fourth parameters, including inactive RS55 Task00001. A global reader/writer normalization would therefore contradict retained cases and could alter inactive models. The converter's public mode-0 sweep and isolated legacy normalization do not establish public mode-1 Save parity. CP196 also verified that the public editor300 factory still refuses before setup/fill; bypassing this guard to force parity is unsupported.

## R300 work is a separate open boundary

The latest R300 design recorded at `analysis/resume214-editor300-next.md`, reconciled with the static access audit `analysis/resume203-editor300-cache1-static.md`, still has no independently safe cache1 source increment. CP163's bounded fresh-A prefix and refusal remain the accepted boundary. Selected-length/cache1 publication remains guarded because `GuiModel::step` is publicly mutable and directly accessed across 41 application source files; `withStep`/`withOpStep` callbacks do not prevent pointer escape or pre-existing aliases. Completing that access closure is a broad API migration, not a one-file patch and not a remedy for the RS55 mode/event uncertainty.

Recent CP215–217 records concern DOE I/O identity, operation-directory effects, and a negative-ID parser preflight. They provide no new RS55 process witness or R300 cache1 access-closure proof. CP217's candidate parser run did not execute. No finding from those tracks should be treated as closing either gate.

## Minimum witness and scope

For RS55, preserve an immutable before-tree copy and bind the original executable hash and PID, actual settings source/path/hash/`MoInputMode` at startup, any mode setter through Open, first active operation, Task00002 solver/count/parameters, CG state before/after fill, delivery (or non-delivery) of method 64 / `0x140ed01b0`, and resulting values before Save. Retain the raw output tree and Task00001/Task00002 SimCtrl hashes. If mode 0 is observed, investigate the first actual mutation instead of attributing the difference to editor300. If the full nonzero-mode event chain is observed, candidate parity must use that proven public mode after the public guard is legitimately implemented.

The current recovery state still reports GUI release as unsupported (`analysis/gui-control-lifecycle-assessment.json`; the required status tool was run at the start of this recovery turn). This pass therefore used retained files only: no GUI or original process, no build or tests, no source/fixture edits, and no REA session. This report is the only new artifact.
