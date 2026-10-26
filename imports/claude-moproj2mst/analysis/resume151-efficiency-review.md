# CP151 safe-boundary efficiency review

CP149â€“151 each completed one build and one full launcher without a recorded source/test repair build. Independent review strengthened fixtures before building; it does not supply a counterfactual saved duration. The remaining concrete workflow correction is to validate all freeze-contract paths and supplied hashes before invoking freeze, addressing CP150's misplaced receipt without changing scientific gates.

This review writes only this report. No source, runner, build, benchmark, GUI or REA state was changed. Token measurements, worker occupancy and work-hour costs are unavailable.

## Accepted build and test scopes

| Checkpoint | Build envelope | Configure/focused/full commands | All fingerprints | Early CTest commands | Full CTest | Full launcher | Build+launcher subtotal |
| --- | ---: | --- | ---: | ---: | ---: | ---: | ---: |
|149|38.835s|1.234 /2.625 /24.687s|9.127s /18 calls|0.546s|13.891s|30.469s|69.304s|
|150|38.996s|1.235 /2.687 /24.735s|9.077s /18 calls|0.625s|13.953s|30.547s|69.543s|
|151|39.537s|1.250 /2.875 /24.859s|9.219s /18 calls|0.656s|14.078s|31.562s|71.099s|

Each source tree has 3,541 files. Every focused build compiled four objects and linked three outputs; every full build then linked 330 outputs without compilation. These closely matched action sets make the stable roughly 24.7s full command meaningful, but do not prove an unnecessary relink: the shared core archive changed. Command totals are 29.092/29.282/29.640s; remaining envelope cost after subtracting command and fingerprint totals is 0.616/0.637/0.678s. Legacy step timings overlap post-command fingerprints and are not added again.

The full launcher includes fixture/source/executable binding, inventory, CTest and reporting; its duration is not CTest alone. Final suites are 354 registered tests each: 347 ordinary passes, six gap-qualified and one difference-qualified, zero failed/skipped/incomplete, unchanged fixtures and historical 93 SAME/five DIFF. All source/binary/archive bindings remain required. Build-plus-launcher subtotals sum 209.946s across the three checkpoints; they exclude source review, separate evidence work and untimed archive/freeze operations, so they are not whole-checkpoint or worker-hour totals.

## Opt-in runtime cost versus CP148

Each latest build runs two exact changed CTests before full linking. Their command field is only 0.546/0.625/0.656s. The six extra fingerprint occurrences associated with those tests cost 3.000/2.968/2.985s, yielding directly attributed early-test work of 3.546/3.593/3.641s before small cache/log overhead. These samples are already inside the build envelopes and total fingerprint duration. Summing those two reported scopes is valid; adding them again to the envelope is not.

CP148 had three early tests in its first build and one after the geometry test-only repair. Its early commands plus twelve runtime-associated fingerprints cost 6.720s across two builds. Total build envelopes were 54.048s and the single full launcher 30.453s, for 84.501s of recorded work. CP149â€“151's lower individual subtotals are observations of different source cuts and no repair builds, not a measured speedup caused by the early gate. No early runtime failure was recorded in CP148â€“151; the gate's benefit remains prospective while its binding cost is real. Do not remove binding boundaries to make the command-only field appear to be total overhead.

The static changed-source binding gate also remained enabled, with complete durations 0.531/0.547/0.562s including its post-check fingerprint. This is distinct from the roughly 18ms isolated checker benchmark. All three checks passed, and no source/cache drift retry is recorded.

## Source review and freeze outcome

CP148's all-zero copy fixture passed runtime tests but could not distinguish a real copy from zero-fill. Its later nonzero full-extent witness required a 12.140s repair build. CP149/150 reviews now verify independently seeded rows and distinct destination sentinels per recipient, live-table identity revalidation, partial effects and no replay. CP150's octal boundary/tab addendum was incorporated before its single build. CP151's generalized loop review precedes acceptance and checks append/identity/reference lifetime rules, while tests cover actual count growth and distinct rows through eight recipients. These are concrete pre-build review outputs; no duration or prevented-failure count is assigned to them.

Root integration records two bounded increments per latest checkpoint, plus supporting evidence/reviews. There is no measured per-worker timeline or token telemetry. The work of reviewing, correcting and coordinating cannot be derived by subtracting build/test time from an unknown wall interval, nor can six increments be equated to a six-worker throughput rate.

CP149/151 freezes completed with retained archives. CP150's first freeze failed before writing validation because an implementation receipt was outside the converter subtree. Moving the exact report into the subtree and verifying its SHA allowed freeze to pass without a source change, rebuild or repeated full regression. No freeze command elapsed-time receipt is available; file timestamps and archive sizes are not used as freeze-duration estimates. The failed attempt remains documented rather than assigned an invented time cost.

## One actionable workflow adjustment

Before the first freeze invocation, resolve the complete supplied contract list under the converter root, verify each report exists there and matches its owner's released SHA, and pass those verified relative paths to the unchanged freeze command. Have the handoff include the converter-local final path, rather than relying on a parent-checkout relative path. This addresses the actual CP150 failure and the same report-placement confusion recorded earlier, without rebuilding settled source or changing any acceptance assertion.

The check is an earlier diagnostic, not a substitute for freeze's own existence/hash validation, exact source/archive read-back, binary/test binding, complete regression or independent evidence review. Its own cost and any avoided freeze time are unmeasured; no seconds, tokens, work hours or10Ã— improvement are claimed. No second timing experiment or production runner change is bundled into this review.

Evidence: frozen `resume149/150/151-build.json` and `*-tests.json`, corresponding `wave3-checkpoint*-validation.json` and `*-verified/receipt.json`, referenced timing/Ninja/early/full logs, CP149â€“151 root integration and independent geometry reviews, and `resume150-spring-octal-test-addendum.md`. Objective completion remains false.
