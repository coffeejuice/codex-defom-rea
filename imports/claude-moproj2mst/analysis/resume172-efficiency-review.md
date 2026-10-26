# CP170–172 converter efficiency review

## Evidence scope

Read-only review of CP170–172 build/test receipts and associated implementation, repair/preflight, source-review and freeze notes. The parent recovery task remains blocked on the separate GUI-release lifecycle; this converter audit does not depend on GUI or live REA work. Available receipts provide gate timings, but no task-level worker wall time, reviewer active time, or token counts.

## Measured gate costs

| Checkpoint/attempt | Configure | Focused preflight | Focused CTests | Full build | Full CTest | Full launcher | Outcome |
|---|---:|---:|---:|---:|---:|---:|---|
| CP170 grouped | 2.265 s | 3.453 s (failed) | — | — | — | — | `allocationIdentity` undeclared |
| CP170 repair | 2.219 s | 1.891 s (failed) | — | — | — | — | duplicate `string` declaration |
| CP170 compile-fix | 2.203 s | 3.406 s | 1.422 s | 29.812 s | 16.656 s | 34.453 s | completed, with recorded gaps |
| CP171 validator | 2.765 s | 4.094 s | 1.704 s | 30.703 s | — | — | completed build |
| CP171 final incremental | 2.250 s | 3.531 s | 1.438 s | 0.828 s | — | — | completed build |
| CP171 final suite | — | — | — | — | 16.234 s | 33.328 s | completed, with recorded gaps |
| CP172 initial | 2.313 s | 5.718 s (failed) | — | — | — | — | assignment to const `symbolRva` |
| CP172 matrix | 2.297 s | 5.656 s (failed) | — | — | — | — | inconsistent `auto` deduction |
| CP172 matrix-fix | 2.234 s | 3.609 s | 0.750 s (failed) | — | — | — | 1 of 43 focused cases failed |
| CP172 finalfix | 2.203 s | 3.563 s | 1.437 s | 0.813 s (incremental) | 16.015 s | 32.859 s | completed, with recorded gaps |
| CP172 final candidate | 2.234 s | 3.563 s | 1.438 s | 43.641 s | — | — | completed candidate build |
| CP172 final preflight | 2.265 s | 1.891 s (failed) | — | — | — | — | nonexistent QString count field |

CP170’s successful build workflow summed to 36.843 s. CP171’s initial full build workflow summed to 39.266 s, followed by an 8.047 s incremental final workflow. CP172’s finalfix workflow summed to 8.016 s and the final candidate workflow to 50.876 s. Its full build phase was 43.641 s, around 13 seconds longer than CP170/171 full build phases; the evidence does not establish why.

The cumulative ledger sum of recorded build workflows, including failed and incremental repeats, is 179.609 s: CP170 46.671 s, CP171 47.313 s, CP172 85.625 s. This combines different source snapshots, failure/retry attempts, and incremental builds; it is an accounting total, not a comparable per-checkpoint or recovery-time measure.

All three successful full suites recorded 354 passed, 6 passed with logged gaps, 1 passed with a logged difference, zero failures, zero skips, and complete output. The three full launcher times were 34.453, 33.328, and 32.859 s (100.640 s total); raw CTest totaled 48.905 s. Launcher timing also includes inventory, fixture hashes, source/binary binding, JUnit parsing and report output. The measured envelope does not establish overall recovery speed.

## Repair cycles and measurement limits

CP170 had two failed preflights totaling 5.344 s, with separate compile errors, before its compile-fix gate. CP172 had two initial failed preflights totaling 11.374 s, a 0.750 s focused runtime failure (one of 43 cases), then successful finalfix and freeze gates. A later 1.891 s final preflight also failed due to a nonexistent QString count field. The focused failure was an expected-diagnostic mismatch in the QVector prebinding refusal case. CP171 records source review/re-review and a full build followed by an incremental final build.

The receipt history records attempts, not exact worker repair cycles or durations. It does not provide inter-attempt waiting, model/reviewer active time, task wall time, or token usage. Do not attribute the missing time to reasoning, tool waits, approvals, or parallelism.

## One improvement to measure

After each source or test repair, run the focused compile preflight immediately before adding broader cases or proceeding to larger tests. Use the shared immutable constructor qualification pattern while preserving cut-local sparse-state checks, diagnostics, no-mutation matrices, freeze order, and full fidelity gates. CP170 and CP172 show measurable failed-preflight cost (5.344 s and 11.374 s), followed in CP172 by a focused runtime failure and later successful gates. This short preflight loop likely helps catch compile errors before broader attempts, but the receipts do not prove it would have prevented those specific failures or reduced total wall time. Its benefit remains unmeasured; no token or x10 speedup claim is supported. CP172’s slower final candidate full build also means no overall speedup is demonstrated.

## Conclusion

CP170–172 each have a successful full suite with the same qualified 354/6/1 classification. CP170 and CP172 also contain multiple failed attempts; CP172’s final frozen candidate and full launcher completed successfully. A focused compile-preflight loop is a reasonable efficiency improvement to measure on a comparable future change, with all review, freeze, test, and fidelity gates intact. This report changed no source, tests, build outputs, shared plan/state, GUI state, or live REA evidence.
