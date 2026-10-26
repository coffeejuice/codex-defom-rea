# CP69–72 efficiency review and bounded next experiment

## Purpose

Determine whether grouped configure reuse has produced a measured end-to-end gain, and select the next performance experiment without reducing test, fixture-hash, or source/binary-binding coverage.

## Receipt summary

| Checkpoint | Build receipt | Configure / focused / full (s) | Full launcher (s) | CTest (s) | Fixture SHA before + after (s) | Binding before + after (s) | Outcome |
|---|---|---:|---:|---:|---:|---:|---|
| CP69 | `resume69-integrated-full-build.json` | 1.266 / n/a / 28.234 | 29.500 | 12.172 | 6.344 + 6.344 | 1.781 + 1.781 | 214 tests; passed with recorded gaps |
| CP70 | `resume70-integrated-grouped3-build.json` | 1.875 / 3.062 / 0.750 | 29.594 | 12.250 | 6.328 + 6.391 | 1.781 + 1.781 | 216 tests; passed with recorded gaps |
| CP71 | `resume71-integrated-grouped2-build.json` | 1.968 / 3.000 / 20.250 | 29.985 | 12.407 | 6.469 + 6.422 | 1.781 + 1.812 | 216 tests; passed with recorded gaps |
| CP72 | `resume72-integrated-grouped-build.json` | 1.922 / 11.406 / 27.547 | 30.063 | 12.250 | 6.734 + 6.359 | 1.860 + 1.812 | 217 tests; passed with recorded gaps |

The full test launcher phases sum to approximately 29.5–30.1 seconds. Fixture snapshots consume 12.69–13.09 seconds (about 42–44%); execution binding consumes 3.56–3.67 seconds; CTest consumes 12.17–12.41 seconds. Inventory and report parsing are around one second combined. The immutable full fixture set, source/binary binding, and CTest were retained in every successful run. Test counts rose from 214 to 217; these are not identical test inventories.

Build receipts bind each run to a stable source tree. CP72 source SHA-256 is `1ab0ff16933b42355e4297bf8fcdd94ad831273a19b5e8f0aca2d6ab8c15b72f` (3,296 files); its CMake cache hash was identical before/after (`e1141197…`). CP72's grouped build succeeded on its first recorded attempt. CP70 and CP71 also have earlier failed attempts, so their successful retry timings do not represent a clean first-attempt workflow. Failures included an unknown target at CP70 and a compiler error at CP71. CP69's focused preflight initially failed with a compiler error before a successful follow-up; do not treat only the successful follow-up as total iteration cost.

## What the receipts establish

There is no defensible causal comparison showing grouped configure reuse saved time across CP69–72. The configurations, source trees, target sets, and incremental build states differ. Successful configure phases are about 1.25–1.97 seconds in this interval, and there is no matched prior run with the same source, targets, and cache from which to calculate a reuse benefit. Build elapsed time ranges from 7.34 seconds (CP69 focused preflight) to 46.54 seconds (CP72 grouped build); that variation tracks different work and retries, not a controlled speed comparison.

The test launcher is stable near 30 seconds, but the earlier accepted best full-launcher observation is 25.250 seconds. That historical point is not a matched baseline. The 10x objective is not achieved: even the fixture SHA phases alone take roughly 13 seconds per run. No test-time or token savings should be claimed from these receipts. Token measurement is unavailable in the test-run receipts.

The isolated real-fixture benchmark reports that the two-worker hashing helper had a 10.831-second median versus 17.523 seconds for serial hashing across two passes, with exact snapshot equality. However, the first pass had uncontrolled cache state, the sample count was only two per engine, and the report explicitly says a full-launcher saving was not established. It is directional evidence for a carefully controlled end-to-end experiment, not an accepted speedup claim.

## Recommended bounded next experiment

Run a paired, three-repetition full-launcher comparison of the existing serial fixture snapshot and the exact two-worker helper, on the same frozen source and built binaries, same build cache, same 217-test inventory, and same four fixture roots. Alternate run order to reduce warm-cache bias. For every run, require unchanged full fixture manifests before/after, unchanged source/binary bindings, identical registered-test inventory and CTest success. Record per-phase durations and compare medians; do not change worker count, skip hashing, cache results across runs, or weaken the comparator. Stop and reject the candidate if any digest, ordering, metadata, binding, inventory, or test result differs.

This is the most bounded opportunity because fixture hashing is the largest repeated launcher phase and the prior benchmark supplies a concrete candidate already checked for snapshot equality. If the two-worker path is already active, compare it against serial only in a disposable runner configuration; do not infer its effect from these checkpoint receipts. Increasing concurrency beyond two workers is not justified by the existing benchmark: the 2/4/8-worker medians differ by under 0.1 seconds, below what two repetitions establish reliably.

A separate low-cost prevention is to validate grouped preflight target names against the generated build target inventory before invoking the build. CP70's unknown target demonstrates the failure mode, although its focused command itself took only 0.719 seconds, so the measurable payoff is limited. It should be considered only after the paired launcher experiment, and must not replace compilation of the focused targets.
