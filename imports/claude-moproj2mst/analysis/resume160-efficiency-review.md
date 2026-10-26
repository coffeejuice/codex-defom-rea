# CP160 safe efficiency boundary: CP159â€“160 and CP161 preparation

Both accepted checkpoints passed complete bound regression, but each required a second build invocation. CP159 repeated a missing-declaration compile failure; CP160 added missing boundary tests after its first passing build. The one next adjustment is to finish the independent parser boundary-test matrix before the first frozen build, targeting the observed test-only rebuild without dropping any gate. This report is the only write; no source, build, GUI or mutable REA operation was performed.

## Measured phases

| Checkpoint/build | Envelope | Configure/focused/full commands | Fingerprints | Early runtime commands | Final full CTest | Final full launcher |
| --- | ---: | --- | ---: | ---: | ---: | ---: |
|159 first|8.745s|1.297 /2.750 /not reached|4.672s|Not reached|â€”|â€”|
|159 accepted repair|77.686s|1.219 /5.672 /54.296s|15.420s /30 calls|0.362s /6 tests|14.281s|30.672s|
|160 first|42.873s|1.406 /3.016 /26.797s|9.971s /18 calls|0.999s /2 tests|â€”|â€”|
|160 accepted repair|15.163s|1.250 /2.875 /0.109s|9.406s /18 calls|0.906s /2 tests|15.390s|32.954s|

The selected frozen builds/tests are `resume159-repair-build.json`/`resume159-repair-tests.json` and `resume160-spring-repair-build.json`/`resume160-spring-repair-tests.json`. Source counts are 3,551 in both final snapshots. Commands exclude later source/cache hashing; legacy step elapsed overlaps those operations and is not added again. Full-launcher timings include inventory, fixture and source/executable binding, CTest and report output, excluding its timing-receipt write/process exit.

CP159's accepted focused/full stages compiled 17/57 objects and linked 7/334 outputs. CP160's accepted repair compiled one test/linked one output, with no full-stage compile/link action. Its small final envelope therefore reflects reuse of the first broad build, not a from-change speedup.

CP159's total build envelopes are 86.431s and its build-plus-selected-launcher subtotal 117.103s. CP160's corresponding totals are 58.036s and 90.990s. For context, the preceding review records CP156/157/158 subtotals 128.050/74.039/77.863s. These are different changes and action sets; they do not establish faster per-feature recovery. The subtotals exclude review, untimed evidence activity and freeze/archive work. No complete checkpoint duration or work-hour cost is inferred.

## Gate cost and rework

CP159's accepted six early exact tests consumed 0.362s of command time and 9.138s in their eighteen associated source snapshots, or 9.500s before small administrative overhead. CP160's first/repair early commands plus six associated snapshots each cost 4.313/3.969s, totaling 8.282s. These samples are already inside envelopes and total fingerprint time. Configured test names in a failed preflight are not counted as executed tests; CP159's first attempt never reached the runtime gate.

Every accepted group still retains source/cache checks, full build, raw logs, bound full suite and exact archive verification. The early gate did not fail in either completed build. Its command-only field is not its total cost, and its passing result does not prove complete source/test coverage.

CP159's first 8.745s invocation stopped in focused compilation on a missing direct `KeywordConstructorRecord` include and an incorrectly qualified literal helper. This repeats the declaration-visibility failure class from CP156; the earlier direct-header recommendation remains applicable. The editor's wrong-cache-state fixtures were also repaired before the accepted broad build. The failed invocation's whole envelope is not an avoided-time estimate because concurrent compilation may have produced reusable output.

CP160's first source/build and focused tests passed. Independent review then requested explicit count-one `++1` acceptance and count-three sign refusal controls. The production implementation stayed unchanged; the later 15.163s build incorporates those tests. Its 9.406s fingerprint total is real retained binding work, not a separate counting pass that may be skipped. No failed full launcher is present in the selected CP159/160 full stdout receipts.

Final suites each register 360 tests: 353 ordinary passes, six gap-qualified and one difference-qualified, zero failed/skipped/incomplete, unchanged fixtures and historical 93 SAME/five DIFF. CP160's full launcher is 2.282s above CP159, of which CTest differs by 1.109s; no matched experiment isolates the cause.

## CP161 pipeline and parallel output

At this review's sampled boundary, no CP161 build receipt or frozen validation existed. Three preparatory reports were visible: the ReportGenerate key plan review, the FRCROT table-suffix plan review and the Inherit document/payload ownership investigation. These are measurable **document outputs**, not three accepted code increments, worker-hours or proof of simultaneous execution.

The ReportGenerate review preserves the known static metadata key but finds no actual candidate constructor/TaskInfo owner for faithful wiring. The table-suffix review finds a narrowly source-ready prepared-observation projection after explicit scanner-result/buffer qualifications. The Inherit investigation narrows payload/DOM ownership but does not admit actual Report cleanup. Keep those dispositions separate; a known value or private projection must not bypass the missing public owner/call chain. CP159 integrated the lazy empty-operations reader, cached-Stopping timer prefix and clone-failure test; CP160 integrated one parser-category behavior plus strengthened tests. Those source/test outputs differ from preparatory analysis.

No per-worker start/finish or occupancy data is present in these receipts. Tests explicitly mark token measurement unavailable; cached input, uncached input, generated tokens, model overhead and worker-hours are not inferred. Freeze receipts bind archives/artifacts but contain no elapsed field, so freeze duration is unavailable rather than derived from timestamps. No10Ã— or token-saving claim is supported.

## One actionable adjustment

For the next parser-category cut, close the **test-coverage review before the first frozen build**: enumerate the admitted primary counts, the nearest excluded count and the token-prefix cases that distinguish the new cursor transition. The CP160 examples are count-one `++1`, count-two repeated extraction and count-three refusal. Freeze the agreed tests with the source before root starts broad linking. This is a targeted ordering change to existing review, not a new general audit project.

Retain focused compilation/runtime and the full build/regression/freeze sequence. If review finds a new case after building, preserve its addendum and rebuild as done here. The observed 15.163s repair envelope motivates the change but is not a guaranteed saving: the test compilation and review still need to occur, and no controlled alternate schedule was measured. Do not remove snapshot boundaries, batch away required exact-test receipts or fabricate CP161 acceptance to shorten the pipeline.

Evidence: immutable CP159/160 build/test/validation/archive receipts, referenced timing/Ninja/early/full stdout logs, their root integration reports, the sampled CP161 preparatory reports, and `analysis/resume158-efficiency-review.md`. Accepted CP160 source SHA-256 is `043c9578138bcbd768e85b03ed29184550fc285a386aa2f48e956b53794c34c9`; objective completion remains false.
