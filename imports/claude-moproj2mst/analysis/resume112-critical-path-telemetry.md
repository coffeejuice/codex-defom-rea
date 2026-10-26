# CP112 critical-path timing telemetry

Added a read-only receipt analyzer at `admin/measure_critical_path.py` with focused tests in `admin/test_measure_critical_path.py`. It summarizes existing grouped-build steps, test counts and runtime, launcher phases, source binding, binary-binding stability, fixture hash settings, and token availability. It does not run a build/test, rehash source or fixtures, or modify input receipts.

To measure worker overlap in a future checkpoint, supply a separate `m2m-worker-events-v1` JSON receipt through `--worker-events`. It must use one `host_monotonic_seconds` clock, name the checkpoint and frozen source SHA, and list each task's ID, role, start/end times, and dependency IDs. The analyzer verifies the event source hash against the test receipt and validates dependency order before reporting observed wall span, summed worker busy time, busy/span ratio, peak simultaneous workers, and dependency critical path. It refuses mismatched checkpoints/hashes or incomplete event graphs. Without such an event receipt it reports overlap as unavailable; it never infers overlap from build or test durations.

## CP109 application

The analyzer processed the frozen CP109 build, full-suite test receipt, and launcher timing receipt into `analysis/resume112-cp109-critical-path-telemetry.json`. It confirmed:

- Build phases: configure 2.313 s, focused preflight 3.407 s, full-build phase 2.687 s; all zero exit.
- Full suite: 292 registered, 285 clean passes, six logged-gap tests, one logged-difference test, zero failures, skips, or incomplete outputs; CTest 12.157 s.
- Launcher: 29.344 s, including 12.047 s of before/after fixture SHA and 4.171 s of before/after source/binary binding.
- Source hash `2af75283ba49105d5b145609126edc8c40dcf669d3029f4a30b6fbd0636ec39a` remained stable; binary binding was stable; fixture changes were empty. Fixture hashing remained four-worker SHA-256 with caching disabled.
- Token usage and worker overlap: unavailable. Existing build/test receipts have no agent start/finish, queue-wait, or dependency-edge events, so the orchestration critical path cannot be recovered retrospectively.

The CP109 2.687-second full-build phase is the recorded command duration for its current build state, not a controlled clean-build comparison. This telemetry therefore supports no causal orchestration or configuration speedup claim. A future run must emit worker events during execution to make critical-path attribution reproducible; the analyzer alone cannot reconstruct missing timestamps.

## Verification

Ran `python -m unittest test_measure_critical_path -v` from `admin/`; all five focused unit tests passed. Ran the analyzer against CP109’s preserved receipts and produced the JSON summary. No converter build, full suite, source change, GUI action, or original execution was performed.
