# CP127 acceptance next-step review of CP126

## CP126 disposition

The controlling receipt is `analysis/wave3-checkpoint126-validation.json`, bound to `analysis/resume126-fixture-repair-build.json` and `analysis/resume126-fixture-repair-tests.json`. The final build/test source fingerprint is `bb389d13f31bc34005022f8feff0f8ac727213909c9e87d5a895360559564495` (3,467 files) before and after; CTest exited 0. It reports 313 tests: 306 ordinary passes, six passed-with-logged-gaps suites, one passed-with-logged-differences suite, and zero incomplete, skipped, or failed tests. The status is `completed-with-recorded-gaps`, and `objective_complete` is false.

The CP126 classifier fix is reflected in the final receipt: `m2m.mdt.mdt_write` carries only `SKIP optional imported PACK/pack.dat header check: M2M_INSTALL_FIXTURE unset`. The empty `refused (active cogging operation):` diagnostic is absent. The six gap-suite count therefore stays six, while the recorded event-line total is 20. This confirms the false-positive diagnostic removal; it does not close the optional fixture skip in this full run. The separate CP124 focused PACK run already exercised that optional assertion and passed, so do not repeat it here.

The single difference-bearing suite remains the logging `m2m.s.save_folder` sweep. It still reports the same five historical case rows: A55, B120, B121, B122, and B98. The result count “one passed-with-logged-differences” is one diagnostic test, not one case-level file difference. CP126 leaves the historical comparison at 93 SAME / 5 DIFF, with no changed diagnostics. The B98 path substitution was separately qualified earlier; do not rerun it as the next action. Keep raw historical rows and any normalized/environment-qualified views separate.

## Boolean fixture failure and repair

The first CP126 attempt is preserved under run `1792923148070235500`, source fingerprint `8e879cd1b614fe4a87e87dec43d9debb8f2d4f95b03b63fe7b010db8ac72b199`. Its full receipt has one failed test target; `m2m.mdt.boolean_postload_creation` reports both test cases failing at `!fs::exists(root)` because the prior PID-derived scratch leaf remained present. This is a test-fixture identity collision, not evidence of a Boolean lifecycle regression.

The test owner added a process-run steady-clock tick to each disposable leaf while retaining the collision assertion and scratch-containment rules. Repaired run `1792923362154226700` uses source fingerprint `bb389d13f31bc34005022f8feff0f8ac727213909c9e87d5a895360559564495`; the Boolean target passes with no logged gap, and the complete source-stable suite passes with the counts above. Preserve both attempts in the record; only the repaired run is the final CP126 validation.

## Completion gates

`analysis/takeover-state.json` still has all eight gates `passed: false` with empty evidence arrays:

- `reachable_scope_accounted`
- `architecture_verified`
- `lifecycle_integrated`
- `no_unexplained_discrepancies_or_refusals`
- `fresh_independent_comparison`
- `failure_and_close_verified`
- `abi_and_regression_verified`
- `reproducible_delivery`

Passing the grouped suite and repairing its fixture do not satisfy those gates. The global recovery status also continues to block GUI-dependent acceptance because a supported GUI-control release API/status is unavailable. This review takes no GUI action and does not reclassify that external blocker.

## Smallest next original-free acceptance action

After the queued CP127 SPRING2 octal branch freezes, run only the focused `m2m.mdt.simcontrol300_spring_reader` test against a valid one-row ASCII `SPRING.INI` and companion bytes `018\r\n`. Require a successful read with flag `1` and assert both inputs remain byte-identical. The retained Qt getter evidence qualifies that the leading zero selects octal, parses `01`, leaves the `8` unread, and succeeds; the one-record caller requests no second value. This is a narrow candidate regression, headless and independent of PACK/B98, not fresh original-process parity. Keep `08\r\n`, multi-row cursor behavior, and broader radix syntax outside this one-case claim. The current CP126 candidate is documented as refusing the leading-zero/radix form, so perform this check only after the CP127 implementation source is frozen and source-bound.

No source, build, fixtures, GUI, original process, or mutable REA state was changed or run for this audit.
