# CP147 efficiency boundary: CP145â€“147

The fast binding preflight is inexpensive and now reaches every recorded grouped attempt, but it cannot catch fixture-state errors. The next actionable change is to evaluate focused runtime **after focused compilation and before broad full linking**, retaining the complete final build, bound regression and archive gates. This read-only review writes only this report; no source, build, GUI or REA state was changed. Token and worker-hour savings are unavailable.

## Accepted timing scopes

| Checkpoint | Files/tests | Accepted build envelope | Configure/focused/full commands | Fingerprints | CTest | Full launcher |
| --- | --- | ---: | --- | ---: | ---: | ---: |
|145 repair1|3,537 /352|13.783s|1.703 /2.515 /0.140s|8.767s /12 calls|15.687s|36.547s|
|146|3,537 /352|37.789s|1.516 /2.344 /25.125s|8.172s /12 calls|13.718s|30.453s|
|147|3,540 /353|72.551s|1.547 /8.235 /53.875s|8.234s /12 calls|13.765s|30.328s|

These are the source-stable builds selected by `wave3-checkpoint145/146/147-validation.json`. Command timers exclude post-command fingerprints/cache work. Legacy step elapsed overlaps those operations; it is not added to command/fingerprint totals. Full launcher timings include inventory, fixture and executable/source binding, CTest and report output, excluding timing-receipt writing and process exit.

CP145's accepted repair full stage had no compile/link action because its earlier broad build completed first. CP146's full stage linked 329 outputs with no compilation; CP147's full stage compiled 63 objects and linked 329 outputs after 29 focused compiles/three links. Their 25.125s versus 53.875s full commands demonstrate different amounts of real work, not an isolated runtime regression.

The CP145 full launcher is 6.219s above CP147, with CTest 1.922s higher. Its paired fixture checks cost 14.906s versus 11.484s in CP147; execution binding cost 4.751s versus 3.907s. No paired experiment isolates the cause. Do not attribute the outlier to model reasoning, permissions, worker overhead or a particular source edit.

All final suites retain six gap-qualified and one difference-qualified pass; ordinary passes are 345/345/346, with zero final failures/skips/incomplete results and unchanged fixtures. Historical comparison remains 93 SAME/five DIFF. Public lifecycle and fresh-original qualifications are unchanged.

## Actual fast-preflight overhead

The new opt-in source-binding gate took 0.734s in the first CP145 build, 0.750s in its repair, 0.703s in CP146 and 0.719s in CP147. These are complete gate durations, not the earlier 18ms standalone checker benchmark: the timer includes a new full post-check source fingerprint. That occurrence takes 0.703/0.735/0.688/0.703s respectively. Subtraction leaves approximately 0.031/0.015/0.015/0.016s for the check and surrounding work, subject to rounding.

Every opt-in successful group therefore records twelve source fingerprints rather than the prior eleven. The extra call is a retained drift check, not wasted repeated counting; source SHA and count are still produced together. The gate's duration overlaps total fingerprint timing and must not be added to it again. All four recorded checks passed; no binding failure or internal configure/drift retry occurred. This proves the gate ran cheaply, not that it caused the absence of later compile failures.

## Repairs compared with CP142â€“144

| Checkpoint | Build invocations | Sum of recorded build envelopes | Reason for later invocation |
| --- | ---: | ---: | --- |
|142|5|142.489s|Includes/type-use/standalone link repairs|
|143|2|47.961s|Missing Testkit main|
|144|2|81.461s|FRCROT fixture expectation after a passing build|
|145|2|63.632s|Two fixture-state errors after a passing build|
|146|1|37.789s|No recorded repair|
|147|1|72.551s|No recorded build repair; independent source corrections occurred before build|

The earlier three-checkpoint sum is 271.911s; the latest is 173.972s. These different workloads are not a controlled 98s saving: root records fifteen prior source/test increments, including a path correction, versus six latest recovery increments plus the preflight tooling. No per-worker timing/occupancy or work-hour denominator is recorded. Supporting evidence and reviews are useful outputs but do not become extra integrated production features.

CP145's first 49.849s build passed, including a 32.671s broad full command. Focused CTest then found that a DOE negative fixture tried to bind an already-bound notifier and a SPRING2 test inherited uppercase bytes from the preceding case. Both repairs were test-only; the later build cost 13.783s. CP146/147 each have one recorded passing full launcher and no failed full-launcher receipt. None of these totals includes separately untimed focused CTest, review or archive work.

## One next workflow change

For a bounded upcoming cut, stage the existing focused runtime group immediately after its targets compile and before the broad full build. On failure, retain the raw focused receipt, repair under the usual exclusive ownership, establish a new source fingerprint and repeat the affected focused checks before proceeding. On success, still run the full build, full source/binary/fixture-bound regression and exact archive verification. Do not accept a focused result from an earlier source snapshot or use it to skip the later suite.

CP145 supplies the concrete motivation: static checks passed but two stateful fixtures failed only after broad linking. Earlier focused runtime could detect those fixtures sooner; it would not eliminate the necessary 32.671s broad build itself. No saved-seconds claim follows from the 13.783s repair envelope because it contains useful recompiled tests and all binding overhead. Implementing a pause/runtime phase may have its own pass-case cost, so measure one opt-in trial before changing the standard runner. Retain the current fast binding check; it covers a different failure class. No hashing-cache or worker/model change is bundled into this recommendation.

Evidence: frozen CP142â€“147 build/test/validation receipts and retained timing/Ninja/full stdout logs; CP145â€“147 root integration reports; `analysis/resume145-grouped-source-preflight.md`. No10Ã—, token or worker-hour saving is established.
