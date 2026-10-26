# Converter suite timing harness

`admin/benchmark_converter_suite.py` measures the relocated converter suite against an **existing** CMake build. It invokes `admin/run_converter_tests.py` once per repetition and measures the external Python process from entry to exit with `time.perf_counter()`. The runner's `elapsed_seconds` remains its narrower CTest subprocess measurement. A newer runner's `timing` field and per-run `timing.json`, when present, are preserved without replacing either measurement. The harness performs no build, original-app launch, or cache clearing.

Example (run only after the requested build has been configured and built):

```powershell
python admin/benchmark_converter_suite.py --build build/candidate --runs 5 --jobs 8 --cache-state warm --note "same host, no other suite runner"
```

For a comparison, supply `--baseline <earlier-summary.json>` and a new `--output <new-directory>` if you want a named result directory. The default is a unique UTC-stamped directory under `optimization/test-speed/analysis/benchmark-runs/`. The output directory must be new and beneath this project. `--build` must be an existing directory beneath `build/`. The harness calls the full existing suite; it does not offer a partial-test option.

Each `run-NNN/` retains stdout, stderr, a benchmark timing record, a copy of the runner's full receipt, and a copy of the runner's per-run timing file if one exists. The runner's normal `build/test-runs/<id>/` directory retains its inventory, logs, JUnit, and fixture snapshots. The harness reports those artifact paths. The runner also writes a shared `analysis/test-relocation-results.json` on each invocation, so the harness checks that the copied receipt's run directory and build agree with that invocation's stdout. A concurrent writer can therefore make a repetition invalid rather than silently mixing receipts; avoid competing suite runs during measurement.

`summary.json` is updated after each repetition and finalized with valid-run median and nearest-rank p95 for external wall and CTest seconds. Failed, incomplete, or mismatched repetitions remain in the records and are excluded from statistics. A baseline comparison reports median changes only when both summaries have no invalid runs. It flags differing `--jobs` and cache labels. Compare only otherwise equivalent host load, fixtures, test registration, and build conditions. A median change is a timing observation, not evidence of fidelity or token savings.

`--cache-state warm|cold|unspecified` and repeated `--note` values record operator observations. “Cold” is never asserted from this label alone: the harness cannot establish OS file-cache state and does not flush it. For warm repetitions, use a preceding, separately recorded suite invocation if that is part of the study design. Preserve its receipt too. Record the actual host and cache preparation in notes; compare cold and warm groups separately. With five samples the nearest-rank p95 equals the maximum; use more repetitions for a useful tail estimate.
