# CP148 opt-in focused CTest gate

Added repeatable `--preflight-ctest NAME` to the grouped build flow. For each supplied name, in argument order, the builder runs one `ctest --test-dir ... --output-on-failure -R '^<escaped-name>$'` after focused target compilation and before the full build. Anchoring and escaping make the CTest selector match the exact supplied test name. No names are inferred and no CTest runs in the default grouped mode.

Each runtime invocation uses the existing step runner: stdout/stderr are retained in a per-step log, command/exit/timing/source/cache bindings are recorded, and a nonzero exit stops before the full build. Source or cache drift is handled through the existing bounded whole-group retry; the focused compile and requested CTests run again on the retried source snapshot. The existing full build, artifact binding, and later full regression/freeze gates remain required.

## Validation and timing

Ran:

```text
python -B -m unittest admin.tests.test_build_candidate_grouped admin.tests.test_check_new_cpp_bindings -q
Ran 34 tests in 0.563s — OK
```

Mocked coverage checks repeated exact test names, command order (configure, focused compilation, CTests, full build), retained raw logs, failure stopping before full build, and CLI rejection outside grouped mode. The existing grouped CLI without `--preflight-ctest` remains covered and runs no focused runtime tests. No converter build or CTest was executed, so this records no production-build or test-time savings. The 0.563s is only the focused unit-test suite's wall time.

Focused CTests may use `C:\PROBLEM`; an actual opt-in grouped invocation must have the same authorized scratch permission as the full suite. Access-denied results must remain failures; the gate must not normalize paths, skip named tests, or bypass the failure.

## SHA-256

- `admin/build_candidate.py`: `d1287d06ab54f36b01cf9971cc44ea3b8dd7fc28d6d3363a87ca6c43dc1dfd28`
- `admin/tests/test_build_candidate_grouped.py`: `4583bd4bea017662512083dd3a615d79f57ca3b9992ec4d9bd629d2337ab6959`
