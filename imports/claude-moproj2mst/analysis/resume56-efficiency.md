# Checkpoint 56: measured build/launcher follow-up

Checkpoint54's long integration command is measured, but its cause is not established. The concrete rework opportunity is earlier review of Release-active test assertions: checkpoint55 had to repeat a full exact freeze after checkpoint54 accepted a test whose C assertions were compiled out.

This report reads existing receipts and two archived test-source members only. It ran no executable/build/test, changed no source/shared state and used no GUI or mutable REA. Token usage, total agent-hours and model/scaling benefits remain unavailable. No 10x or other speedup is claimed.

## Comparable receipt scopes

| Checkpoint | Registered | Last accepted build receipt, s | Configure, s | cmake-build step, s | Internal launcher, s | CTest, s | Complete pre/post fixture SHA, s |
|---|---:|---:|---:|---:|---:|---:|---:|
| 53 | 173 | 14.708 | 10.484 | 1.813 | 25.515 | 12.031 | 12.641 |
| 54 | 177 | 108.351 | 10.313 | 95.406 | 26.781 | 12.828 | 13.047 |
| 55 | 177 | 14.120 | 10.281 | 1.329 | 26.313 | 12.406 | 13.031 |

Bindings: wave3-checkpoint53/54/55-validation.json identifies each accepted build/test receipt. They bind respectively resume53-b98-kfread-count-fix-build.json, resume54-combined-full-build.json and resume55-ppv-test-fix-build.json, with corresponding full-test JSON and run timing.json files. These build receipts all report stable source fingerprints and command exit 0.

The table uses internal launcher timings consistently. External process measurements were 25.701/26.959/26.503 seconds and have wider boundaries. The launcher contains CTest and both fixture snapshots; build receipts contain configure/build steps. Do not add nested values twice. Last accepted build is not total checkpoint work: checkpoint53 also has earlier combined-build failures/retries before its final B98 correction. These changes and incremental work sets differ, so the table is not a paired compiler benchmark or a causal performance comparison.

The 95.406 seconds is the elapsed complete cmake-build command, including compilation/linking/build-system work; it is not separately measured compiler CPU time. It accounts for 88.05% of checkpoint54's 108.351-second build receipt. Its build-plus-launcher components total 135.132 seconds, of which the cmake-build command is 70.60%. The 10.313-second configure and 2.632 seconds of remaining wrapper/source/artifact work explain the rest of that build receipt arithmetically. No controlled repeat with the same source, cache, process load and target work is available. Do not attribute the long step to agent concurrency, file count, shared-header churn, cache state, hardware pressure or a particular source edit from this single receipt.

The launcher range is 25.515-26.781 seconds across 173-177 tests. Complete fixture hashing remains about 12.6-13.0 seconds, and CTest about 12.0-12.8 seconds. There is no evidence here that the 95.406-second build commanded a corresponding test-runner slowdown. This is a descriptive observation, not proof of equal workloads or absence of resource effects.

All three gates retain unchanged fixture hashes and the same exact folder-log SHA-256, 93 SAME / 5 DIFF, six logged-gap passes and one diagnostic-difference test. Test inventory growth does not establish additional original parity; those gaps remain reported. Preserve all full hashes and raw qualification evidence.

## Observed integration rework

Checkpoint54 integration explicitly records the post-freeze defect: PPV RecoveryEntry tests used C assert under Release. Independently inspecting the saved source archives confirms:

| Frozen test source | Raw assert calls | Registered Testkit cases | Release-active CHECK/CHECK_EQ calls |
|---|---:|---:|---:|
| checkpoint54 test_ppv_recovery_entry.cpp | 15 | 0 | 0 |
| checkpoint55 corrected file | 0 | 3 | 15 |

The corrected source ends with main returning testkit::run_all(). Checkpoint55's validation records three Release-active PPV cases passing, followed by the full 177-test freeze. Its correction build receipt plus internal launcher components total 40.433 seconds (14.120 + 26.313), excluding diagnosis/editing/packaging and any unattributed gaps. This is the measured revalidation work following the defect. It is not a proven amount that a new review would save, because the proposed review's cost and false-positive rate have not been timed.

## One concrete change for the next integration

Before the full-freeze build, root should review only newly added/modified test sources for raw C assert, cassert, NDEBUG guards and bare successful exits without an observable active assertion. Require each behavioral oracle to remain executable in the configured Release build, normally through the existing Testkit CHECK/CHECK_EQ and registered case mechanism. Review legitimate alternative test harnesses explicitly rather than imposing a new blanket format on unchanged tests.

This can be a bounded diff review while workers finish independent evidence tasks. A text scan of the changed test paths is a triage aid; it is not proof that every assertion is active. At the focused check, retain the actual expected case count and final test summary, not just process exit 0. Checkpoint54's fifteen disabled assertions would have been visible before the long freeze; the acceptance failure was test effectiveness, not a compiler error.

Measure the next instance as review time plus focused validation, repeated full-build/launcher count and complete accepted-freeze time. Keep this prevention step only if it reduces total rework after its review cost. Do not claim a realized saving until that result exists. Continue the source-write barrier and one full exact build/launcher/freeze for the selected source; do not weaken registered coverage, hashes, ordered diagnostics, source/build binding, API compatibility or original-first acceptance in exchange for shorter timing.
