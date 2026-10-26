# CP135 efficiency boundary: CP132â€“134

The remaining repeatable cost is broad relinking plus complete validation binding. The receipts do not justify skipping a build, source fingerprint, fixture snapshot or full regression. CP134 completed without a recorded repair build or failed full launcher, while CP132/133 retained substantial repair work. This read-only review follows `analysis/efficiency-policy.md`; only this report was written. No benchmark, build, source/admin edit, GUI or original execution was performed. Token usage is unavailable.

## Comparable accepted receipts

| Checkpoint | Source files | Final grouped build envelope | Configure step | Focused build step | Full build step | CTest | Full launcher |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
|132|3,473|31.824s|2.110s|0.750s|23.125s|13.734s|31.235s|
|133 final3|3,474|33.846s|2.125s|2.765s|23.078s|13.578s|30.484s|
|134|3,475|38.129s|2.156s|0.766s|29.313s|13.640s|30.203s|

Build values come from `analysis/resume132-final-build.json`, `resume133-final3-build.json`, and `resume134-final-build.json`. The step timer includes the post-command source/cache snapshot; it is not an isolated CMake subprocess measurement. The grouped envelope excludes the earlier command-environment setup. Launcher values come from the `timing_receipt` referenced by each frozen final tests JSON and include inventory, before/after bindings, CTest and report output. They exclude writing that timing receipt and process exit. These scopes must not be added as nested timings.

The launcher cost outside CTest is 17.501/16.906/16.563s respectively. Two fixture SHA snapshots take 11.891/11.625/11.828s; two execution-binding snapshots take 4.531/4.125/3.657s. CP134's launcher is 1.032s below CP132 with two more registered tests; this is an observation across different source snapshots, not a controlled optimization result. Accepted counts are 317/318/319 registered, with 310/311/312 ordinary passes, six gap-qualified and one difference-qualified pass each. No fixture changes; historical 93 SAME/five DIFF remain.

## Actions and repair cost

Ninja log lines were classified by their actual `Building CXX`/`Linking` text, rather than treating the changing `[n/total]` denominator as a reliable total. Directory checks are excluded from the following counts.

| Final full-build log | Compile lines | Link lines | Step elapsed |
| --- | ---: | ---: | ---: |
|`resume132-final-build-03.log`|0|292|23.125s|
|`resume133-final-build-03.log` (earlier source)|77|294|58.343s|
|`resume133-final2-build-03.log`|0|292|23.344s|
|`resume133-final3-build-03.log`|0|292|23.078s|
|`resume134-final-build-03.log`|10|294|29.313s|

Broad relinks are real work after the changed shared core archive, even when the focused build has already compiled its consumers. A no-compile full step still costs about 23.1s here. The extra CP134 compiles are consistent with additional editor-header consumers; action counts alone do not establish exact per-action cost or machine contention. Suppressing required relinks or treating a configure-cache match as binary freshness would weaken the gate.

CP132 has two narrow-build receipts totaling 16.202s, plus the 31.824s final envelope. CP133 has three narrow-build receipts totaling 25.753s and three distinct-source final envelopes totaling 133.440s. CP134 has one 9.435s narrow build and one 38.129s final envelope. These are recorded build-envelope sums, not whole-checkpoint wall times; independent review and runtime work are outside them.

The retained CP132 failed/full-rerun pair cost 30.718+31.235=61.953s of launcher time. CP133 root integration documents two earlier full failures and the EOF/process-exit repair. A directly parsed failed full receipt costs30.593s; two repaired, same-source full confirmations cost 30.578+30.484=61.062s. The second pre-repair failure is not assigned a duration here because its timing receipt was not selected. CP134's single recorded full launcher cost 30.203s. Repair and confirmation were justified by real failures; omitting them would not have been an equivalent validation strategy.

## One bounded next improvement

Add phase telemetry to the existing build receipt: source-fingerprint call count/duration and Ninja compile/link/directory-check counts, preserving every current call and assertion. This is one instrumentation increment, not a new benchmark project. It can reuse the retained logs for action counts and time the existing fingerprint function without altering its path/content/count result. Test that the returned hash and count, drift retry decisions and retained artifact bindings remain identical.

The source implementation already hashes and counts files in the same traversal. Its no-drift grouped success path invokes that combined function 11 times at initial, configure, focused, full and artifact boundaries. There is no separate count traversal to remove. CP132/133-final3/134 envelope residuals after subtracting the three recorded steps are 5.839/5.878/5.894s; this includes pre-step fingerprints and artifact hashing, while post-step fingerprints are already within step timing. It cannot be attributed entirely to source hashing or used as a promised saving. Do not replace content hashes with mtime/count cache acceptance or collapse before/after boundaries on these data.

The next measured decision should use those added timings to choose between optimizing the same-pass fingerprint implementation and reducing dependency fanout through a separately proven private interface boundary. Keep the current grouped static/focused preflight before broad linking, every full build/regression, exact source count/hash, test/binary binding, fixture integrity checks and archive rehash. No 10x, end-to-end or token-saving claim is supported by this review.
