# CP133 efficiency review (CP130–132)

## Measured bound receipts

| Checkpoint / run | Configure | Focused preflight | Full build | CTest | Full launcher | Registered test result |
|---|---:|---:|---:|---:|---:|---|
| CP130 final | 2.141 s | 0.766 s | 57.921 s | 13.422 s | 30.406 s | 316: 309 ordinary pass, 6 gap-qualified, 1 diff-qualified |
| CP131 final | 2.110 s | 1.125 s | 23.219 s | 13.422 s | 29.657 s | 316: 309 ordinary pass, 6 gap-qualified, 1 diff-qualified |
| CP132 first full run (failed) | — | — | — | 13.766 s | 30.718 s | 1 isolated-worker failure; retained as failed attempt |
| CP132 source-stable rerun (accepted) | — | — | — | 13.734 s | 31.235 s | 317: 310 ordinary pass, 6 gap-qualified, 1 diff-qualified |
| CP132 final build | 2.110 s | 0.750 s | 23.125 s | — | — | 317 registered |

CP130–132 final build source identities were respectively `1f775d1c…f5ef99` (3,472 source files), `49356dbe…10533ba` (3,472), and `06668fc4…c30e8` (3,473). Each build receipt reports a stable source hash before and after its steps and the same CMake cache hash. These are distinct source snapshots, so the 57.921-second CP130 full build cannot be interpreted as a controlled regression against CP131/132. Its configure and focused preflight stayed near 2.1 and 0.8 seconds; CP131/132 full build phases were both about 23.2 seconds. The measured difference is localized to the full build phase, but without action-count comparisons it does not identify a cause.

The CP132 first run used the same source and binaries as its retry, passed 316/317 tests and had one isolated-worker stale-nonce supervisor-stop failure (expected 66, observed `0xe1050001`). Its full launcher cost 30.718 seconds. After a focused recheck, the same source/binaries completed a 317-test source-bound rerun in 31.235 seconds. Thus CP132 consumed 61.953 seconds across two full launchers (27.500 seconds in CTest), plus the separately recorded focused recheck whose duration is not included here. The failed result remains visible in the checkpoint qualification; it is not averaged away or recast as accepted.

## Test and evidence costs

The successful CP130/131 launchers varied by 0.749 seconds (30.406 vs 29.657). CP132’s successful run was 31.235 seconds; CTest varied only 0.312 seconds across CP130–132 final passes. The launcher overhead beyond CTest is roughly 16–18 seconds. For CP132 this includes inventory 0.953s, fixture SHA snapshots 5.860 + 6.031s, execution-binding snapshots 2.250 + 2.281s, and report parsing/output 0.110s. CP130/131 show the same shape: two complete SHA-256 snapshots of the 16,728-file fixture corpus (~1.868 GB), full source/binary identity checks before and after, then full CTest. No fixture changes were reported, all bindings remained stable, and cache was disabled. Tokens were unavailable.

Coverage rose from 316 to 317 registered tests; CP132 added one ordinary clean pass. Every final validation remains `objective_complete: false`, with six logged-gap and one logged-difference tests retained, no skips/incomplete/final failures, and the same historical 93 SAME / 5 DIFF. The initial CP132 failure is an additional reliability qualification, not a reason to reduce the suite or fixture binding. The x10 test-time objective is not met: the successful full launcher remains about 30 seconds, and the two-run CP132 recovery cost is about 62 seconds before the focused recheck.

## Recommendation

Keep the prior early static test preflight plus one grouped focused build, full build, and full source/binary/fixture-bound suite. For a low-risk measurable improvement, capture per-target Ninja action counts and elapsed time in the existing build receipt at each checkpoint and compare CP130’s 57.921-second full-build phase against CP131/132’s ~23.2-second phases. This is diagnostic-only and preserves exact source fingerprints, targets, and acceptance gates; it can establish whether the outlier came from legitimate dependency invalidation or transient machine contention before any build-policy change is considered. Do not suppress necessary recompilation or skip the full build/tests.
