# CP167–169 converter efficiency review

## Scope and evidence

This is a read-only converter-only audit following CP169. It used `analysis/resume167-final-build.json`, `analysis/resume168-final-build.json`, and `analysis/resume169-final-build.json`; their paired `*-final-tests.json` receipts and referenced `timing.json` records; and the CP167–169 implementation, source-review, repair, and plan-review notes. The shared recovery policy says to use bounded evidence and to report measured results without treating byte reduction as time savings. The parent `python tools/recovery_status.py` reports the broader DEFORM GUI acceptance work blocked on an unavailable verified GUI-release lifecycle; that does not block this independent converter audit.

## Recorded costs

| Checkpoint | Configure | Focused preflight | Two focused CTests | Full build | Build workflow sum | Full CTest | Full launcher |
|---|---:|---:|---:|---:|---:|---:|---:|
| CP167 | 2.219 s | 4.672 s | 1.452 s | 29.797 s | 38.140 s | 15.547 s | 32.234 s |
| CP168 | 2.234 s | 4.797 s | 1.438 s | 29.844 s | 38.313 s | 15.235 s | 31.891 s |
| CP169 | 2.250 s | 4.968 s | 1.453 s | 29.907 s | 38.578 s | 15.234 s | 32.312 s |

These are close repeat runs: the full build varies by 0.110 s across the three receipts, and full CTest by 0.313 s. The launcher times include inventory, fixture snapshots, pre/post source and binary binding, CTest, JUnit parsing, and report generation. Their recorded phase sums are 32.219 s, 31.891 s, and 32.125 s respectively. The roughly 16 s difference between launcher and raw CTest is therefore accounted for by documented guard/report work, principally fixture hashes and binary/source binding; it is not evidence of unexplained waiting. Launcher scope excludes receipt write and process exit.

All three test receipts report 354 passed, 6 passed with logged gaps, 1 passed with a logged difference, zero failed, zero skipped, and complete output. The 7 recorded qualifications remain visible and are not converted to clean-pass claims. The receipts bind source trees of 3554 files with distinct hashes per checkpoint.

## Retries, review/repair, and unknown time

The final build receipts each contain one successful configure, focused preflight, two focused CTests, and full build; their `retry_count` field is absent. Final test receipts report exit 0, and launcher records return code 0. Thus the recorded final gates have no retry. The notes document earlier source-review findings and repairs: CP167 first-call boundary expectations and missing refusal coverage; CP168 predecessor/refusal matrix issues; and CP169 prewrite matrix and allocation-time identity issues. These were repaired before the final snapshots, but the notes do not record how many review/repair cycles consumed elapsed time. CP167/168/169 implementation notes explicitly state those source-only owners did not build or run tests; integration receipts later record the runtime gates. No per-worker active duration, wait duration, total task wall time, or reviewer time is present. Unix start timestamps on build receipts and test run IDs are not sufficient to attribute time spent between tasks or by workers. Token measurement is explicitly `unavailable` in all three final test receipts. Do not infer that missing wall time was model reasoning, tool blocking, approval wait, or parallel-worker overhead.

## One concrete improvement to measure

For future repeated constructor cuts, extract the shared immutable admission checks into one narrowly scoped validator that returns a qualified snapshot (owner/private identity, phase, address points, root binding/commit markers, and predecessor field observations). Keep each instruction cut's own expected phase, new field write, next phase, and refusal diagnostic explicit. This targets the repeated long guard logic and review surface visible across the CP167–169 implementations, while preserving sparse unknown fields, owner binding, source qualification, and every focused refusal assertion. It is a source-review efficiency candidate, not a measured speedup yet.

Evaluate it only on a future constructor-cut slice: count duplicated predicates removed and review/repair cycles, retain the same source-stable focused/full/freeze gates and original fidelity comparisons, and compare equivalent build and launcher receipts. Do not drop source review, focused tests, the full suite, frozen chronology, or fidelity gates to make the cycle appear faster. No x10 or other speedup factor is supported by these measurements.

## Result

Build and full-suite timings are stable across CP167–169; no performance regression is visible in these envelopes. The recorded suite is clean of test failures but retains seven reported gaps/differences. The proposed shared validator is pending a fair measured comparison. This audit changed no source, test, shared plan/state, build, GUI, or live REA artifact.
