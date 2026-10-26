# CP182–184 converter efficiency review

## Scope and result

Read-only review of the CP182 first dual attempt and fix, CP183 reentry-fix receipts and CP184 dual build/full-suite receipts, plus the CP181 efficiency review and repository efficiency policy. All successful full-suite receipts retain 354 passes, six logged-gap suites and one logged diagnostic-difference suite, with zero failed, skipped or incomplete-output cases. These are source-stable regression receipts; they do not establish fresh original-session acceptance or completion of the full application.

CP182's first focused preflight failed (exit 2, 7.671 s outer / 6.890 s command); its subsequent `resume182-dual-fix-build.json` and `resume182-dual-fix-tests.json` contain the repair run. CP183 records a successful reentry fix; CP184's material scope callback passed its focused gates and full suite. These recoverable focused failures/repairs are a concrete iteration cost. No controlled end-to-end duration, token telemetry or causal wall-clock measurement is present.

## Measured command times

| Receipt | Configure | Focused preflight | Six focused CTests | Build | Full-suite launcher |
|---|---:|---:|---:|---:|---:|
| CP182 dual repair | 2.250 s | 5.578 s | 13.922 s | 30.812 s | 16.844 s |
| CP183 reentry-fix | 2.422 s | 5.562 s | 13.922 s | 30.953 s | 17.390 s |
| CP184 dual | 2.640 s | 7.797 s | 13.939 s | 31.984 s | 16.875 s |

Focused CTest totals sum the six recorded outer elapsed values. The successful build receipts report 67.594 s (CP183) and 70.939 s (CP184) for their combined build workflow; this is a sum of command-step timings, not a claim of total task wall time. CP182 repair's analogous step sum is 69.094 s. CP184's individual build command was 31.265 s. The CP182 failed preflight adds 7.671 s outer (6.890 s command) before its successful repair, so the two attempts together include at least 15.342 s outer recorded preflight time; this excludes intervening work and is not total task wall time.

The nearby successful build commands differ by 1.031 s (30.953 vs 31.984); full-suite launcher times differ by 0.515 s (17.390 vs 16.875). Two observations are not enough to attribute a performance change. Output compaction and receipt storage avoid streaming large logs, but the policy correctly treats bytes saved as output reduction, not runtime or token savings.

## Safe next step and cadence

Keep the CP181 practice of source-backed adjacent batching and immediate closure of a verified phase. For the next efficiency opportunity, capture one compact per-checkpoint table of focused test names, outcomes and timings directly from each build/test receipt, retaining full logs on disk and inspecting failure details only on nonzero results. This is a bounded reporting improvement; current data do not establish that it saves elapsed time or tokens, so measure its authoring/inspection cost before claiming a benefit. Preserve focused tests, source stability, the full 354-case regression and all fidelity gates.

Review again after CP187 or one hour, whichever comes first, at a safe checkpoint. Do not infer a 10× or other total-recovery speedup from build/test command timing or compact output. Cached/uncached input and generated-token fields are not analyzed here because the requested receipts do not provide task-level comparable attribution.

## Evidence

`imports/claude-moproj2mst/analysis/resume182-dual-build.json`, `resume182-dual-fix-build.json`, `resume182-dual-fix-tests.json`, `resume183-reentryfix-build.json`, `resume183-reentryfix-tests.json`, `resume184-dual-build.json`, `resume184-dual-tests.json`, `resume181-efficiency-review.md`, and `analysis/efficiency-policy.md`. No source, plan, state, build, GUI or REA database files were changed.
