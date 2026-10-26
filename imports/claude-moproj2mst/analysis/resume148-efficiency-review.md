# CP148 focused-runtime trial and repair cost

The early focused CTest trial passed without detecting a defect and added more binding time than CTest command time. The later geometry repair strengthened an all-zero copy assertion; it was not a runtime failure. Keep the gate opt-in and retain every full validation requirement. The actionable workflow adjustment is to complete the existing copy-fixture witness review before releasing the first source freeze/build. Only this report was written; no source, runner, build, GUI or REA operation was changed or run.

## Recorded CP148 costs

| Invocation | Build envelope | All command time | Fingerprints | Early CTest command time | Full-build command |
| --- | ---: | ---: | ---: | ---: | ---: |
|First passing build|41.908s|30.562s|10.704s /21 calls|0.578s /3 tests|25.078s|
|Test-only repair|12.140s|3.891s|7.671s /15 calls|0.156s /1 test|0.110s|
|Sum|54.048s|34.453s|18.375s /36 calls|0.734s /4 invocations|25.188s|

The first build compiled six focused objects/linked four outputs, then linked 329 full-stage outputs without compilation. The repair compiled one test/linked one output; its full stage had no compile/link action. Both source snapshots were stable and passed their exact early focused tests before full linking. No internal source/cache reconfigure retry is recorded.

The first runtime gate's `elapsed_seconds:0.578` is the sum of **command** timings, not its total envelope overhead. Source inspection and the occurrence-ordered samples establish three additional complete fingerprints per requested exact test: before the runtime boundary and before/after the command step. The nine runtime-associated samples in the first build sum 4.501s; the repair's three sum 1.485s. Thus directly attributable commands plus those snapshots total 5.079s and 1.641s respectively, or 6.720s across the two invocations, before small cache/log/administrative overhead. This is observed work, not a controlled no-gate comparison. These snapshot times are already inside the build envelopes/fingerprint totals and are not added again.

The gate retained exact escaped test selection, per-step logs/exits, source/cache comparisons, bounded retry behavior and stop-before-full-build on failure. It reported no failure in this trial. Its 34 pure admin tests passed in 0.563s, as recorded in `resume148-focused-ctest-gate.md`; that is tool validation, not a production saving.

The final full launcher took 30.453s, including 13.860s CTest, 11.593s fixture hashing, 3.844s source/binary binding, 1.016s inventory and 0.125s parsing/report work. It passed 354 registered tests: 347 ordinary, six gap-qualified, one difference-qualified; zero failed/skipped/incomplete and unchanged fixtures. Historical comparison remains 93 SAME/five DIFF. No full launcher preceded the repair in the selected stdout receipts.

The two build envelopes plus the one full launcher sum 84.501s. That sum already includes the early CTests and their binding work. It excludes archive/freeze, source review and other untimed work, so it is not whole-checkpoint elapsed time. Recorded early CTest commands plus the full CTest stage total 14.594s across their respective timing scopes, not an extra addition to 84.501s. Raw command time includes process/log overhead, whereas the full CTest stage is the launcher's retained subprocess interval.

## Comparison with CP145â€“147

| Checkpoint | All recorded build envelopes | Final full launcher | Build+launcher subtotal | Repair qualification |
| --- | ---: | ---: | ---: | --- |
|145|63.632s|36.547s|100.179s|Two fixture-state errors after initial passing build|
|146|37.789s|30.453s|68.242s|One build; no recorded repair|
|147|72.551s|30.328s|102.879s|One build; pre-build review corrections|
|148|54.048s|30.453s|84.501s|One later discriminatory test repair|

Different production changes, dependency invalidation and reusable build products prevent a causal speedup claim. CP145â€“147 recorded twelve fingerprints per successful group; CP148's extra runtime bindings raise that to 21 and 15. CP148's lower per-snapshot timing does not prove the gate makes hashing faster. Its full launcher matches CP146 numerically and is 0.125s above CP147, but those are different sources and suites. No tenfold, token or worker-hour saving follows; token measurement remains unavailable and worker timing/occupancy is not recorded.

## One workflow adjustment

For an admitted memory-copy path, finish the **discriminating source-value review before the first frozen build**: the fixture must make copied bytes observably different from both a no-op destination and a zero-fill, then compare the entire qualified extent. CP148's geometry review found the existing all-zero 24-byte fixture could pass under a zero-fill implementation. The addendum supplied distinct values and full 24-byte comparisons; the early CTest gate had correctly passed the weaker test and could not diagnose its scientific weakness.

Move that existing narrow review earlier rather than adding another runtime or weakening any gate. It targets the observed 12.140s repair invocation, but not every second of that envelope is a proven saving: the necessary test compilation and host conditions differ if repaired before the first build. Preserve the repaired witness, independent review, early checks when selected, full build, complete source/executable/fixture-bound suite and exact archive verification. No parallel-hash change or batching away binding boundaries is bundled into this adjustment.

Evidence: `resume148-build.json`, `resume148-repair1-build.json`, `resume148-repair1-tests.json`, referenced timing/Ninja/early-CTest/full stdout logs, `wave3-checkpoint148-validation.json`, `resume148-root-integration.md`, the geometry source-values addendum/recheck, and frozen CP145â€“147 receipts. Accepted CP148 source is `9c510af7460e4ace31c5800ba3d431c334f0ff4e69c2116f0ac1cd21e8bfcf78` over 3,541 files. Objective completion remains false.
