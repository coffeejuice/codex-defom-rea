# CP191–193 converter efficiency review

## Scope and outcome

This review covers the CP191–193 converter build and full-test receipts, the failed CP193 focused guard-order attempt and repair, and the prior CP190 efficiency review. The final CP193 guard-fix build was source-stable and completed; the full imported-converter suite completed with 354 clean passes, six passes with logged gaps, one pass with a logged difference, and zero incomplete, skipped, or failed tests (361 total). The one failed focused attempt was a real ordering compatibility regression: the early CP193 guard displaced historical prewrite diagnostics for a bare marker. The corrected guard lets the established phase validator preserve those diagnostics while still rejecting premature value-bearing publication. These are synthetic/imported regression results, not fresh original-application acceptance.

## Measured command times

| Checkpoint / workflow | Configure | Focused preflight | Focused CTests | Full build | Full CTest | Receipt workflow elapsed |
|---|---:|---:|---:|---:|---:|---:|
| CP191 | included in receipt | included | included | included | 16.984 s | build 70.412 s |
| CP192 | 2.359 s | 8.438 s | 0.860, 0.797, 0.828, 1.047, 5.250, 0.828 s | 30.828 s | 17.188 s | build 66.010 s |
| CP193 initial attempt | 2.344 s | 8.593 s | first focused test 0.844 s, failed | not reached | not reached | failed build workflow 17.437 s |
| CP193 guard-fix | 2.312 s | 6.750 s | 0.859, 0.765, 0.828, 1.047, 5.219, 0.812 s | 29.828 s | 17.406 s | build 63.367 s |

CP191 full build elapsed was 70.412 s; its receipt does not expose comparable per-step values in this summary. CP193’s initial attempt also recorded 11 source-fingerprint calls totaling 7.766 s and reported source instability. The corrected CP193 build reported source stability and reused the unchanged CMake cache without reconfiguration. Elapsed values above are receipt elapsed values; totals do not include separate unreported coordination time. Adding the failed CP193 attempt to the successful repair gives 80.773 s of build workflow plus 17.406 s for the final full test run, or 98.179 s across those receipts. This is observed effort, not a before/after speedup.

The CP191, CP192, and final CP193 full-suite receipts each report the same 361-test disposition. Full CTest elapsed was 16.984 s in CP191, 17.188 s in CP192, and 17.406 s in CP193. This small spread does not establish a trend. CP190’s prior review had recommended compact retry evidence and explicitly cautioned against inferring speedup from queue correction; the CP193 receipts still do not measure an efficiency gain or token savings.

## One safe optimization for CP196

Add a source-stability admission check immediately before the expensive focused preflight/build sequence: if source owners are still editing or the source fingerprint changes during admission, defer that sequence until the tree is stable. Keep all existing source/executable binding, focused tests, full suite, and freeze requirements. Record rejected starts and any admission wait separately from command elapsed time, then compare against CP193’s 17.437 s failed attempt and 7.766 s fingerprint cost. This is a workflow proposal, not a measured saving; it must not skip validation. No x10 or token-saving claim is supported.

Next review: after CP196 or one hour, at the next safe checkpoint.

## Evidence reviewed

`analysis/resume190-efficiency-review.md`; `analysis/resume191-dual-build.json`, `analysis/resume191-dual-tests.json`; `analysis/resume192-dual-build.json`, `analysis/resume192-dual-tests.json`; `analysis/resume193-dual-build.json`; `analysis/resume193-guardfix-build.json`, `analysis/resume193-guardfix-tests.json`; `analysis/resume193-connector-implementation.md`; and `analysis/efficiency-policy.md` in the parent recovery checkout.
