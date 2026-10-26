# Efficiency boundary: CP142â€“144

Retain the newly measured binding preflight before the first grouped build. Its roughly 18ms scan found two real standalone source-list omissions, addressing the repeated late include/link failures without weakening compilation or regression. Build-envelope totals fell from 142.489s in CP142 to 47.961s in CP143; CP144 took 81.461s including a test-only repair. Different source changes and reusable build products prevent interpreting those totals as a controlled speedup.

This report is read-only apart from its own creation. No source, runner, build, GUI or REA state was changed. `analysis/efficiency-policy.md` calls for a bounded safe-checkpoint review and at most one new measured improvement; this report evaluates the existing preflight result rather than launching another benchmark or project.

## Frozen timing scopes

| Checkpoint | Accepted files/tests | Accepted build envelope | Configure/focused/full commands | Eleven fingerprints | CTest | Full launcher |
| --- | --- | ---: | --- | ---: | ---: | ---: |
|142 repair4|3,516 /343|15.855s|1.515 /2.265 /4.047s|7.422s|13.828s|30.938s|
|143 repair1|3,527 /348|36.201s|1.578 /1.219 /25.265s|7.532s|14.078s|31.390s|
|144 repair1|3,536 /351|11.063s|1.531 /1.203 /0.141s|7.594s|14.187s|31.562s|

These are the builds/tests selected by the three `wave3-checkpoint*-validation.json` freezes. Command timings exclude subsequent source/cache work; fingerprint durations overlap legacy step elapsed and must not be added to it again. The accepted build's complete fingerprint cost stays within 0.172s across this interval. Every build attempt records one configure and no internal drift/reconfigure retry; outer repair invocations are counted separately below.

The full launcher includes inventory, before/after fixture and source/executable SHA bindings, CTest and report processing. Its cost outside CTest is 17.110/17.312/17.375s. CP142/143/144 before/after fixture checks cost 11.765/11.891/12.218s; execution bindings cost 4.250/4.281/3.954s. Full launchers differ by 0.624s and CTest by 0.359s. These receipts do not establish a faster validation implementation.

All final suites retain six gap-qualified and one difference-qualified pass; ordinary passes rise 336â†’341â†’344, with zero failed/skipped/incomplete tests and unchanged fixtures. Historical comparison remains 93 SAME/five DIFF. Passing tests and timing changes do not close the original lifecycle/acceptance gaps.

## Repair and action costs

| Checkpoint | Build invocations | Recorded envelope sum | Repair qualification |
| --- | ---: | ---: | --- |
|142|5|142.489s|Three focused compile failures, then one full link failure before the passing build|
|143|2|47.961s|11.760s first preflight failed on a missing Testkit main; 36.201s repair build passed|
|144|2|81.461s|70.398s initial build passed; focused runtime found one FRCROT expectation error; 11.063s test-only repair passed|

CP142's 74.531s failed full command reached the second P09 standalone target without the new Item owner dependency. Much of that command compiled reusable products, so its whole duration is not an avoidable-time estimate. CP143's passing full command still compiled one object and linked 320 outputs. CP144's initial group compiled 25 focused objects/five links, then 53 full objects/325 links; its final repair compiled one test/one link and the full stage had no compile/link action. Comparing only the small final repair envelopes would conceal necessary earlier broad work.

The selected full stdout receipts show one successful full launcher per checkpoint and no additional failed full launcher in CP142â€“144. Separate focused runtime and archive/freeze durations are not timed here. Thus the table is a sum of recorded build invocations, not total checkpoint elapsed or model reasoning time.

## Worker and token accounting

Root integration records 5/6/4 accepted source/test increments across CP142/143/144. CP143's six includes a path-policy correction to an existing ABI test; it is not six new production behaviors. CP144 additionally delivered the binding checker and twelve pure self-tests as support tooling. Independent native/source reviews and pending owner investigations are supporting work, not extra integrated features.

The receipts contain no common per-worker start/finish timeline, exclusive worker elapsed totals or measured occupancy denominator. No per-worker productivity, orchestration-overhead duration or twenty-worker speedup can be calculated. Test receipts explicitly mark token measurement unavailable; cached input, uncached input and generated token costs are not inferred from output size or wall time.

## One actionable process change

Adopt the new read-only binding checker as a standard first-preflight input for the **complete explicit changed/new C++ file list**, including all packet-local CMake and `ctest-standalone` routes. Require that list in the worker release receipt so late-arriving tests are included. Keep the existing include/Testkit checks, focused compilation, full build, complete bound regression and freeze/archive rehash after it.

`resume144-new-cpp-bindings-preflight.md` records seven scans with 18.381ms median (18.110â€“20.121ms), unchanged five-file input binding, and two actual P09 subproject source-list omissions. Root's CP144 integration records those omissions corrected before building. Its twelve checker self-tests passed in 0.061s. This is evidence of inexpensive useful detection, not proof that a particular build duration was saved. The checker cannot type-check non-streamable String assertions or undeclared C++ names and cannot establish runtime expectations; CP144's FRCROT test repair demonstrates why focused execution remains necessary.

No additional runner rewrite, content cache, worker-count change or hashing experiment is proposed in this review. The previous disposable parallel-hash result remains separate and unadopted. Evaluate the adopted preflight by recording concrete pre-build findings and subsequent avoidable include/link failures, while keeping all fidelity and binding gates unchanged.

Evidence: frozen CP142â€“144 build/test/validation receipts, referenced Ninja/timing/full stdout logs, corresponding root integration reports, and `analysis/resume144-new-cpp-bindings-preflight.md`. No10Ã— or token saving is established.
