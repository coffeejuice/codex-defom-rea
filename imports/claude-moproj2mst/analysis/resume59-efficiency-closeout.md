# CP59 safe-boundary efficiency closeout

The CP59 grouped preflight caught integration errors before the broad gate, and the checkpoint finished with one accepted full launcher. Total retained build occupancy fell relative to CP58, but the source/workloads differ and no paired experiment establishes a causal speedup. CP60 should keep grouped preflight and add one bounded include/API dependency review across all changed translation units before retrying it.

This review reads existing receipts/logs only and writes this report. No source/shared-state change, build/executable, GUI or REA action occurred. Tokens, cached/uncached/generated usage, total agent-hours and model/scaling savings are unavailable. No 10x claim is supported.

## Retained cycle measurements

| Checkpoint | Build attempts | Targeted preflight/repair occupancy, s | Broad build occupancy, s | All build receipt interval union, s | Configure steps, s | Failed compile/link attempt receipts, s | Accepted internal launcher, s |
|---|---:|---:|---:|---:|---:|---:|---:|
| 57 | 1 | 0 separately recorded | 107.428 | 107.428 | 10.531 | 0 | 25.859 |
| 58 | 6 | 29.563 | 167.005 | 196.568 | 63.687 | 149.296 | 27.312 |
| 59 | 5 | 65.496 | 27.790 | 93.286 | 54.360 | 34.658 | 26.282 |

Build intervals within each checkpoint do not overlap; interval unions equal their sums. Focused CTest names are recorded by the integration receipts, but separate focused-command wall times are not retained in these selected build/test JSON records. Do not invent a complete focused edit-loop duration from CTest body time or build elapsed time.

All selected builds report stable source. Configure/compiler steps are nested inside build receipts; CTest/hash phases are nested inside launcher timing. Their nested values must not be added again. Build-plus-accepted-launcher measured components are 133.287/223.880/119.568 seconds respectively, excluding diagnosis/editing, separately unmeasured focused execution, coordination and freeze packaging. They are not whole-checkpoint or agent wall time.

CP57's cmake-build step took 94.422 seconds; CP58 had three failed broad attempts before its stable accepted final build. CP59 had two failed targeted attempts, then one passing targeted group, one focused test repair build and a passing broad build. Different incremental work, caches, source sets and target graphs prevent a causal comparison. The evidence establishes stages/durations and concrete compiler diagnostics, not why a successful broad command is long or short.

## CP59 rework anatomy

| Receipt | Configure, s | cmake-build, s | Whole receipt, s | Outcome |
|---|---:|---:|---:|---|
| resume59-focused-preflight-build.json | 11.765 | 2.500 | 16.891 | Missing m2m/engine/engine_internal.hpp include in new PPV Save adapter |
| resume59-focused-preflight2-build.json | 10.610 | 4.578 | 17.767 | New PPV Open-sequence test lacked qt::cleanPath declaration |
| resume59-focused-preflight3-build.json | 10.625 | 2.031 | 15.233 | Eleven-target group compiled/linked |
| resume59-focused-repair-build.json | 10.688 | 2.343 | 15.605 | Two focused fixture assumptions repaired and rebuilt |
| resume59-integrated-full-build.json | 10.672 | 14.563 | 27.790 | Broad build passed |

The two include/declaration failures cost 34.658 seconds of retained build-driver occupancy. Configuration consumed 54.360 seconds, 58.27% of the 93.286-second build union. This identifies repeated setup as material but does not prove it can be eliminated safely or that all failed-attempt work was wasted; objects and feedback can remain useful.

Root's validation qualifications identify the two separate focused assumptions: Session startup already owns CreateTaskBase registrations; active DoeOutput reaches the DBLIST guard after its .DS read and three buffered study writes. The fixture expectations were corrected to match those existing behaviors. These were caught before the final full launcher, so CP59 has no failed full-run repeat. Their focused runtime/review cost is not measured separately. Worker source-freeze hashes precede these integration corrections and must not be presented as the final candidate hashes.

The bounded new/changed preflight covered eleven targets: eight new test files and three modified regressions. The final suite registered 194 tests versus 186 at CP58. This is useful test/source integration, not eight newly completed public features. The public PPV, DOE, editor and Close paths remain guarded at unresolved effects; administrative/global acceptance gates remain pending.

## Actual accepted launcher speed

| Gate | Internal launcher, s | CTest, s | Complete pre/post fixture SHA, s | Registered tests |
|---|---:|---:|---:|---:|
| CP57 | 25.859 | 12.250 | 12.719 | 181 |
| CP58 | 27.312 | 12.984 | 13.406 | 186 |
| CP59 | 26.282 | 12.563 | 12.749 | 194 |

CP59 is 1.030 seconds, 3.77%, lower than CP58's internal launcher; the descriptive ratio is 1.039x. It is 0.423 seconds above CP57. These are single runs on changing suites, not a controlled claim that the preflight made the launcher faster. CP57 external launcher is 26.038 seconds. CP58/59 integration fields named launcher_seconds match their internal timing.json values and must not be relabeled independent external measurements.

Against the retained test-speed baseline from optimization/test-speed/analysis/results-2026-10-24.md and accepted-candidate-freeze.json:

- Historical accepted optimized checkpoint45 launcher 25.390 seconds: CP59 is 0.892 seconds, 3.51%, higher. The optimized baseline covered 150 tests; CP59 covers 194. The 25.250-second best observed historical run remains a separate best-run statistic.
- Historical separate-state serial launcher 58.598 seconds: 58.598/26.282 = 2.23x descriptively.
- Historical warm serial median 47.371 seconds: 47.371/26.282 = 1.80x descriptively.

Those historical comparisons differ in time/cache/workload and cannot be promoted to paired current speedups. The 10x full-launcher objective is not achieved or demonstrated by these receipts. Source/hash/fixture/behavior qualifications remain attached.

All three accepted current gates preserve complete fixture hashes, exact folder log SHA-256 58dd03cef8bfc3a4f22ce0ca8b8296fd63fa5bcda50aa8811de986c58002f77b, 93 SAME/5 DIFF, six logged-gap passes and one diagnostic-difference test. Token usage is unavailable; no log-size reduction is converted into token savings.

## One CP60 adjustment

Before the next grouped preflight retry, root performs one bounded review of the new include/API lines across every handed-off changed .cpp, rather than fixing only the first compiler diagnostic and immediately rerunning. Require handoffs to identify the declaring header for newly used nonstandard helpers and the actual source-private/public header path. Check the siblings in the same batch: CP59's missing internal include and missing qt::cleanPath declaration were both visible dependency problems across the new PPV translation units.

This is a small source-handoff review, not a new build system or permission to invent declarations, weaken asserts or change behavior. Root still owns all builds. Keep the eleven-target/grouped selection rule, source-write barrier, focused active-case summaries, normal broad build, full optimized launcher, complete fixture hashes, ordered diagnostics/raw differences, source/archive/build binding and original-first acceptance. Expand the standalone ownership-consumer closure when a real lifecycle change reaches it.

Measure the review cost and compile-attempt count at CP60. The observed 34.658 seconds of failed attempts is exposure, not guaranteed savings; review and preflight have overhead. A corrected first-pass group followed by the same exact full freeze is the desired outcome. Do not skip the full freeze or infer a speed benefit before the next measured result exists.
