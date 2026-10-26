# CP179–181 converter efficiency review

## Scope and outcome

Read-only review of the converter's CP179 dual, CP180 five-store batch, and CP181 dual/reentry-fix build and full-suite receipts, root integration notes, CP180 batching review, and the prior CP176–178 efficiency review. All three final checkpoints froze a 3,554-file source tree and reported 354 clean passes, six logged-gap suites, one diagnostic-difference suite, and zero failed, skipped, incomplete-output, fixture-change, or changed-diagnostic cases. Historical 93 SAME / five DIFF remains qualified; these are source-stable regression gates, not fresh original-session acceptance.

CP181's first dual attempt failed a focused reentry assertion (source was not stable during that attempt); the recorded reentry-fix build and full suite completed successfully. This is a concrete retry cost, alongside CP180's batching benefit. No token telemetry, review-hours, end-to-end task duration, or causal wall-clock measurement is available.

## Measured command times

| Checkpoint / receipt | Build workflow | Configure | Focused preflight | Focused CTests | Full build | Full CTest | Full suite launcher |
|---|---:|---:|---:|---:|---:|---:|---:|
| CP179 dual | 63.175 s | 2.312 s | 6.656 s | 9.906 s / 6 invocations | 29.875 s | 16.406 s | 17.172 s |
| CP180 batch | 62.906 s | 2.266 s | 7.250 s | 9.484 s / 6 invocations | 29.734 s | 16.125 s | 16.875 s |
| CP181 dual | 61.051 s | 2.250 s | 5.391 s | 9.406 s / 6 invocations | 29.844 s | 16.109 s | 17.047 s |

The build-workflow value is the receipt's per-command elapsed sum, not elapsed wall time. CP179–181 full-suite launcher total is 51.094 s; raw CTest totals 48.640 s. The roughly 2.45 s difference is wrapper/report overhead in these receipts. Full build commands are tightly grouped (29.734–29.875 s); suite launchers are likewise close (16.875–17.172 s).

## What CP180 batching saved

CP180 combined five consecutive native zero stores into one implementation/checkpoint while retaining each intermediate sparse cursor and its tests. The prior read-only CP180 review projected avoiding four repeated full build/full-suite gate sets if five single-store checkpoints would otherwise each have required an equivalent gate. Its estimated 324.9 s used the CP176–178 mean build workflow (47.990 s) plus launcher (33.240 s) four times; its narrower 252.1 s counted full build and launcher stages only. Those were projections, not observations.

The CP179–181 records do not establish what four future checkpoints would actually have cost or how much of their work could overlap. A retrospective apples-to-apples proxy using the immediately adjacent observed gate sets is four times CP179/181's mean build-workflow plus full-suite-launcher command time: 4 × ((63.175 + 61.051)/2 + (17.172 + 17.047)/2) = **312.9 s**. Using CP180's own build and suite receipt as the proxy gives 4 × (62.906 + 16.875) = **319.1 s**. The earlier 324.9 s projection is therefore about 5–12 seconds above these observed checkpoint proxies (roughly 2–4%). The directly attributable full-build plus full-suite proxy is **187.2 s** across four repeats (4 × (29.844 + 17.047)); CP180's corresponding proxy is **186.4 s**. The evidence thus supports that the projection was in the right range for repeated command costs, but no realized saving is measured: CP180 actually ran one set of gates, and there is no serial counterfactual, implementation/review cost, or wall-clock/overlap record.

## Safe improvement and cadence

Keep source-backed batching for adjacent instructions only when the native interval, intermediate states, refusal behavior, and per-prefix tests are independently specified. CP180 retained exactly those intermediate cursor states; CP181 then added the next `+0x28` store as its own checkpoint, preserving CP180 budgets 1–5. Do not broaden a batch across the unknown-input flags read without a separately reviewed contract.

The prior efficiency guidance recommends one bounded change per review and another review after three recovery checkpoints or 60 minutes, at a safe checkpoint. On the current cadence, review after CP184 or one hour, whichever comes first; preserve every focused, full-suite, source-stability and fidelity gate. No x10 or other total-recovery speedup is supported by these command receipts.

## Evidence and limits

Timing sources: `analysis/resume179-dual-build.json`, `resume179-dual-tests.json`, `resume180-batch-build.json`, `resume180-batch-tests.json`, `resume181-dual-build.json`, `resume181-reentryfix-build.json`, and `resume181-reentryfix-tests.json`. Gate classifications are summarized in each checkpoint's root integration note. CP181 dual's initial failure is retained in its build receipt; final values above use the successful reentry-fix receipt. Build and test command times are kept separate and are not added to claim task wall time. No GUI-control session, original application process, or live mutable REA session was used.

