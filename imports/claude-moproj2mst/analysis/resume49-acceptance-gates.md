# Resume 49 acceptance-gate review

## Current acceptance state

`python admin/check_continuation.py` returns `CONTINUE`, `complete: false`, 15 unfinished queue items, and all eight objective gates pending. This agrees with the latest objective state: checkpoint47 is a candidate validation, while checkpoint48 has a focused configure/build receipt but no full integrated test/acceptance receipt yet. Do not convert registered-test counts, completed checkpoints, or estimates into a completion percentage.

The checkpoint47 suite reports 150 registered, zero failing, with 143 plain passes, six gap-qualified passes, and one diagnostic-difference pass. Its scope explicitly says no public mode300 completion; it does not launch the original executable. The focused checkpoint48 build (`analysis/resume48-focused-a-build.json`) successfully configured and built six targeted binaries, including ABI and Save-I/O targets, against source digest `29e90ecc...`; it records no test execution. This proves those targets compile for that source snapshot, not that they pass or that the full candidate is accepted.

## Historical refusals and differences: what changed versus what remains open

| Item | Reconciled status | Gate consequence / next action |
|---|---|---|
| B:131R Cogging resave refusal | **Stale as a current refusal.** `analysis/wave2-geometry-validation.json` records both 131R and 131 as comparator `SAME` with zero differing/missing/extra files. The earlier refusal was resolved for these historical cases. | Remove that specific refusal from current acceptance summaries. Keep raw/fresh-session qualification; `SAME` does not close independent original acceptance. Continue the distinct multiple-reheat branch/failure-order investigation under RCOG. |
| Geometry scene-box refusals on earlier cases | **Stale for the two corrected cases.** The geometry validation says two previous scene-box refusals became comparator `SAME`. | Do not reopen the corrected path. RGEOM remains active because the populated 3D geometry-list item-box path remains unsupported and a suitable fixture is still needed. |
| Boolean denied-open case | **Settled bounded failure-order contract.** Checkpoint48 queue text says the current Boolean denied-open case is settled; RIO should proceed to the next writer-specific partial-write/failure-order contract. | Do not spend another lane reproducing that case. The broader failure/Close gate remains open until all writer ordering and Close ownership are covered. |
| ShapeOptim property-to-engine dispatch | **Resolved implementation item, acceptance still open.** Current ROPS explicitly says ShapeOptim dispatch is integrated and directs the next audit to another family override. | Do not report the old ShapeOptim omission as current. Continue family override inventory and public-branch evidence; a local integrated fix does not close reachable scope or lifecycle. |
| Normal Cogging writer ordering | **Resolved bounded path.** RCOG says normal Cogging CRT write order is integrated/verified; Cogging native I/O receipt records seven focused IO cases in a frozen validation. | Do not reimplement the normal writer. Multiple-reheat behavior and shared Close dependencies are still unresolved; historical 131/131R `SAME` only covers those particular retained rows. |
| B:98 Diestress historical diff | **Explained environmental mismatch, not a demonstrated logic omission.** `analysis/resume48-b98-raw-repro.md` reproduces the candidate line with its scratch absolute path; the only line difference is the case-root path, and exact prefix replacement yields candidate equality. The original path is from an older workspace. | Keep raw bytes and both hashes. Any qualification must be case-prefix-bounded; do not change the writer or apply a global normalizer. Fresh original/environment evidence remains needed for acceptance. |
| A:RS_55 historical diff | **Still a concrete historical state difference, but its fresh-session expectation is unresolved.** Resume48 A55 audit identifies two `SimCtrl.KEY` bytes under Task00002; static original evidence points to the CG radio toggle during active Forming editor300 iteration fill. The reconstructed public session path stops at palette guard before setup/fill, and no fresh original call trace exists. | Keep as an unresolved acceptance discrepancy. First close the editor300 setup/fill timing dependency and obtain a fresh original-first observation before implementing beyond the qualified guard. Do not infer that the historical Save equals fresh-session behavior. |
| DOE B120/B121/B122 differences | **Genuine remaining omissions/refusals.** Checkpoint47 still records one differing plus 7/8/10 missing files, and RDOE remains active behind unobserved Report QObject TLS/refcount/thread state. | Preserve the current guards; qualify the missing owner inputs and then integrate the bounded DOE paths. Do not relabel these as harmless diagnostics. |
| Remaining Cogging multiple-reheat behavior | **Genuine unresolved boundary.** RCOG retains the original multiple-reheat state/failure-continuation requirement; the current refusal cannot be replaced by a guessed deterministic output because the original pointer path has an out-of-bounds/unstable edge. | Continue evidence for native branch order/failure effects; keep raw expected result unavailable where the original result is undefined. Test only the port guard as a port contract, not parity. |

## Objective gates and concrete next work

All gates remain false in `analysis/takeover-state.json`; the reasons differ:

1. **Reachable scope accounted:** 15 queue items remain. R300, RDOE, RBOOL and RIO are active; several package audits, geometry, Close, oracle and acceptance work are still ready. Reconcile every virtual edge and refusal with source evidence; keep only current blockers in the ledger.
2. **Architecture verified:** session ownership, global/TLS/Fortran state, editor owners and Close/destructor ordering still have explicit unknowns. Continue the staged RCLOSE/DOE/Boolean dependencies and bind owners before enabling downstream effects.
3. **Lifecycle integrated:** current successful candidate comparisons remain historical; the fresh reference case 004 crashed immediately after Open. Prepare a qualified case and complete Open A → Save A → Close A on fresh original-equivalent state.
4. **No unexplained discrepancies/refusals:** preserve A55 and DOE differences as unresolved; qualify B98 as environment-derived while retaining raw diff; do not count the old corrected 131R/scene-box refusals again. Keep multi-reheat and populated geometry guards visible.
5. **Fresh independent comparison:** no checkpoint48 fresh original acceptance receipt exists. After dependencies close and a candidate freezes, compare original first on independent held-out projects; route each defect directly to implementation and regression.
6. **Failure and Close verified:** the Boolean denied-open slice is done, not this gate. Finish next writer-specific failure-order contracts and actual Close ownership/cleanup, including partial effects and repeated isolated calls.
7. **ABI and regression verified:** checkpoint47 full suite is regression evidence with explicit gaps and a permissive real-project ABI assertion. Checkpoint48 currently has compile-only focused targets. Root should run the new sequential ABI failure-path test and focused targets, then the full optimized regression on the final frozen candidate; successful ABI path and fresh lifecycle acceptance still need evidence.
8. **Reproducible delivery:** no final integrated frozen candidate with source/build/evidence handoff is recorded. Freeze only after all integrations, bind source and artifact hashes, record full regression and acceptance receipts, then require the invariant checker.

## Recommended immediate sequence

1. Finish checkpoint48 source/review reconciliation, including the focused test source change, and bind the current source hash to one candidate build.
2. Run the targeted ABI/Save-I/O tests associated with that source snapshot; treat current focused build success as compile evidence only.
3. Run the prescribed optimized full regression once integration is ready, preserving the six gap cases, one diagnostic difference, and historical raw folder differences.
4. Continue the executable queue: editor300 palette/gradient prerequisite, DOE actual owner inputs, Boolean startup dispatcher, next writer failure contract, then geometry and Close work. Do not re-open corrected 131R/ShapeOptim/normal Cogging items.
5. Once a candidate is integrated and frozen, run fresh original-first lifecycle comparisons. Complete `python admin/check_continuation.py --require-complete` only after every gate has evidence and no unexplained discrepancy/refusal remains.

## Evidence inspected

- `analysis/takeover-state.json`; read-only `python admin/check_continuation.py`.
- `analysis/wave3-checkpoint47-validation.json`, `analysis/resume47-combined-full-tests.json`, `analysis/resume47-combined-full-build.json`.
- `analysis/resume48-focused-a-build.json` and its configure/build logs.
- `analysis/wave2-geometry-validation.json`, `analysis/wave3-cogging-native-io-validation.json`, `analysis/wave3-project-save-validation.json`, `analysis/wave3-property-save-validation.json`.
- `analysis/resume48-b98-raw-repro.md`, `analysis/resume48-diff-a55.md`, `analysis/resume48-a55-cg-timing.md`, plus current queue actions for DOE, Boolean, Close, RGEOM, ROPS, RCOG and RIO.

This audit is read-only. No source, build, fixture, application, or REA database was changed or run.
