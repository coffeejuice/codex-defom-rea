# Focused iteration feedback remains separate from full acceptance

`tools/run_focused_feedback.py` prepares and runs exact named CTests against the
existing isolated build. It prints a plan by default; `--execute` is required to
build or test. Build/test execution must still be coordinated with the orchestrator
to avoid overlapping the shared candidate tree or test scratch directories.

The driver calls `build_isolated.py --skip-configure --target ...`, including the
selected native tests and both converter CLI/DLL products to cover indirect runtime
dependencies. It then uses an anchored, escaped CTest `-R` expression and serial
test execution. JUnit identities must exactly match requested tests; failures,
errors and skipped tests make the feedback run fail. Test success can still contain
logged fidelity gaps, so this is explicitly not a full-equivalence verdict.

Every run requires `--last-full-run` pointing to a successful full validation
receipt inside this checkout, with matching registered-test count and unchanged
fixtures. The focused receipt links and hashes that receipt. This establishes
provenance, not transfer of its acceptance result to the current source state.
Focused receipts always state `full_acceptance: false`, `full_suite_coverage:
false`, and `fixture_integrity_coverage: false`. No fixture hash sweep occurs.
The existing full acceptance gate remains required after iteration.

From the converter root, first inspect the plan:

```powershell
python optimization/test-speed/tools/run_focused_feedback.py --label feedback-core-1 --test m2m.g0.core --last-full-run build/test-runs/1792860215872606500/receipt.json
```

Replace the historical receipt with the latest completed full validation, then
add `--execute` only during an assigned I/O window. Repeat `--test` for multiple
exact names. Each execution requires a fresh label; existing receipts and logs
are never overwritten. The output directory retains the build console, verbose
CTest log, JUnit, timing and a reference to the build driver's receipt.

## Verification and measurement status

The plan-only invocation for `m2m.g0.core` succeeded and selected only that exact
CTest, with `m2m_g0_core`, `moproj2mst` and `moproj2mst_cli` build targets. It read
registrations and a prior receipt only; no build, test or fixture I/O occurred.

The orchestrator granted an exclusive I/O window after its full runs completed.
One warm `m2m.g0.core` run used the latest full receipt
`build/test-runs/1792860994043155000/receipt.json`. Evidence is retained in
`analysis/feedback-core-warm-1-focused-feedback/receipt.json` and
`analysis/feedback-core-warm-1-focused-build.json`.

| Measurement boundary | Seconds |
| --- | ---: |
| CMake target build command | 0.113 |
| Build driver subprocess, including its environment setup | 1.737 |
| CTest subprocess | 1.069 |
| Focused driver execution phase, including its environment setup | 4.185 |
| CTest-reported test time | 0.01 |
| CTest-reported total test time | 0.02 |

Ninja explicitly reported `no work to do` after glob verification. This measures
warm no-op build and focused-feedback overhead, **not an edit rebuild**. The
focused driver execution timer starts after planning/receipt initialization and
module import, so 4.185 seconds is that defined phase rather than whole-process
startup latency. Build and CTest subprocess timings include startup/wrapper work
not present in their inner tool timers. Environment initialization is performed
by both the focused driver and the existing build driver; this is measurable
overhead, not compilation work.

Exactly one CTest passed, with all six in-memory cases passing. The coverage label
is `focused-tests-passed-not-full-acceptance`: 1/150 registered tests, no fixture
integrity hash gate, no full-suite equivalence claim. No source timestamps,
production source or fixtures were changed to manufacture this measurement.
Actual test-edit rebuild timing remains unmeasured because no new test edit was
pending for this run.
