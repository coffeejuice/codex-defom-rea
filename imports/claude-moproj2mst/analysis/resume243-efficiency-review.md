# CP241–243 efficiency review

## Scope

Receipts reviewed: CP241 L3 non-type7 count, CP242 heat express PDIE, and CP243 editor dimension-or. All three full suites passed. Build receipts fingerprint the full 3,563-file source tree, but provide no per-header Ninja explanation; measured edge counts and full-tree hashes cannot identify which changed file caused fan-out. Token telemetry is unavailable, so no token-efficiency claim is made.

## Build and test measurements

| Checkpoint | Focused configure/build | Full configure/build | Full build edges | CTest | Full launcher | Tests |
|---|---:|---:|---:|---:|---:|---:|
| CP241 | 1.813 / 19.047 s | 1.688 / 113.359 s | 551 | 17.984 s | 35.562 s | 369 |
| CP242 | 1.687 / 18.797 s | 1.766 / 115.781 s | 551 | 18.296 s | 35.453 s | 369 |
| CP243 | 1.687 / 19.844 s | 1.829 / 114.203 s | 546 | 18.172 s | 35.203 s | 369 |

Each freeze records 362 clean passes, 6 logged-gap passes, 1 logged-difference pass, and zero incomplete outputs, skips, or failures. Each freeze still reports `objective_complete: false`.

Full build times range 113.359–115.781 s with 546–551 logged edges. Focused build commands are about 19 seconds, but they target different edits and are not comparable trials. Full-tree source hashes differ across checkpoints as expected after distinct edits. Source-fingerprint calls took 1.594, 1.453, and 1.470 seconds respectively for two calls each. Those timings do not include all source/binary binding or fixture hashing performed by the test launcher.

CTest remained within 17.984–18.296 s. Launcher time was 35.203–35.562 s. Fixture SHA phases totaled 12.250 s (CP241), 11.844 s (CP242), and 11.687 s (CP243); source/binary execution bindings totaled 4.094, 4.047, and 4.094 s. These measured integrity costs are a substantial, stable part of the launcher and remain required. No reliable end-to-end speed change is shown.

The logged full-build edge count is nearly flat. It is an overall incremental workload count, not per-header fan-out evidence. There is no saved Ninja explain or dependency query tying a specific source/header edit to those 546–551 edges. Therefore no claim that a particular header caused broad rebuilds, or that build time improved, follows from these receipts.

## CP240 timing-repair reconciliation

CP240's first full suite attempt exited 8 after 18.078 s. A test-only timing repair was followed by a successful 18.109 s suite and a stable source/binary binding during that rerun. The report records distinct full-tree fingerprints before versus after the repair interval; only the successful rerun's own before/after source binding is unchanged. CP241–243 have no similar failed full-suite rerun sequence in the reviewed receipts. Their ordinary 17.984–18.296 s suite times are not evidence that the CP240 repair caused a general suite speedup.

CP240's report arithmetic is corrected: all six configure/build command durations sum to 140.781 s; the two focused receipts alone sum to 26.547 s. This review makes no tenfold or other unsupported speed claim.

## Recommendation

No workflow change is supported by CP241–243. Retain the full suite, source/binary bindings, uncached fixture SHA checks and freeze criteria. For a build optimization claim, a future controlled experiment should record Ninja `-d explain` and compile/link edges for one isolated changed header, then compare an equivalent edit on the same configured tree and compiler. Without that per-edit evidence, keep the current measured times as observations only.

## Evidence

CP241: `analysis/cp241-l3-non-type7-count-build.json`, `cp241-l3-non-type7-count-full-build.json`, `cp241-l3-non-type7-count-full-tests.json`, `wave3-checkpoint241-validation.json`, and its `timing.json`.
CP242: `analysis/cp242-heat-express-pdie-build.json`, `cp242-heat-express-pdie-full-build.json`, `cp242-heat-express-pdie-full-tests.json`, `wave3-checkpoint242-validation.json`, and its `timing.json`.
CP243: `analysis/cp243-dimensionor-build.json`, `cp243-editor-dimension-or-full-build.json`, `cp243-editor-dimension-or-full-tests.json`, `wave3-checkpoint243-validation.json`, and its `timing.json`.
CP240 reconciliation: `analysis/resume240-efficiency-review.md`, `analysis/cp240-editor-step-count-full-build.json`, failed-run timing/receipt under `build/test-runs/1793037803632603400`, repaired full tests, and `wave3-checkpoint240-validation.json`.
