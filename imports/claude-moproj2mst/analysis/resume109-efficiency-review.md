# CP107–109 efficiency review

The frozen receipts show no sustained test-time reduction and no measured 10× improvement. CP109 CTest is 12.157 s, compared with 11.484 s at CP107 and 12.343 s at CP108. The complete launcher remains around 29–30 s because it includes full fixture hashing and source/binary bindings. Build receipts vary with source/build state and do not isolate an orchestration or configuration effect.

| Checkpoint / build | Configure | Focused preflight | Full build | Receipt elapsed | CTest | Launcher | Registered / clean |
|---|---:|---:|---:|---:|---:|---:|---:|
| CP107 v2 core | 2.078 s | 3.422 s | 25.109 s | 36.514 s | 11.484 s | 29.641 s | 290 / 283 |
| CP108 worker grouped | 2.078 s | 3.359 s | 22.938 s | 34.222 s | — | — | — |
| CP108 classifier repair 1 | 2.047 s | 1.609 s | 22.047 s | 31.499 s | 12.343 s | 29.765 s | 292 / 285 |
| CP109 worker failure | 2.313 s | 3.407 s | 2.687 s | 14.614 s | 12.157 s | 29.344 s | 292 / 285 |

All listed build receipts completed successfully. CP109’s 2.687 s full-build step is much shorter than CP107–108 and appears in a changed build state; it is not a like-for-like clean build. None of these receipts records a failed CP107–109 build attempt. The separate receipts for CP108’s worker grouped and classifier repair builds are retained above rather than conflated.

The launcher phases were:

| Phase | CP107 | CP108 classifier repair 1 | CP109 |
|---|---:|---:|---:|
| Inventory | 0.828 s | 0.985 s | 0.875 s |
| Fixture SHA before / after | 6.734 / 6.812 s | 6.171 / 5.985 s | 6.313 / 5.734 s |
| Source/binary binding before / after | 1.813 / 1.844 s | 1.829 / 2.328 s | 2.093 / 2.078 s |
| CTest | 11.484 s | 12.343 s | 12.157 s |
| Parse/report | 0.110 s | 0.109 s | 0.094 s |
| Launcher total | 29.641 s | 29.765 s | 29.344 s |

Every run hashes the same 16,728 fixture files totaling 1,867,669,827 bytes with four threaded workers, SHA-256, parent-cache path resolution, and caching disabled. CP109’s fixture passes take 6.313 s and 5.734 s. It binds 3,426 source files and 289 binaries before and after the run; source hash `2af75283ba49105d5b145609126edc8c40dcf669d3029f4a30b6fbd0636ec39a` is unchanged. All fixture files are unchanged. CP107 and CP108 likewise retain stable source hashes, complete binary bindings, and the full fixture manifest.

Each test receipt records zero failed, skipped, or incomplete-output tests, six logged-gap tests, and one logged-difference test. CP107 and CP108 validation receipts retain the historical 93 SAME / 5 DIFF and `objective_complete: false`. CP109 adds cases within the worker-failure test target, while the registered suite count remains 292; its receipt is a passing full suite, not evidence of a shorter critical path. The folder comparator remains diagnostic: CP109 records five diagnostic differences in `m2m.s.save_folder` and four gap events there. Token measurement is unavailable.

Agent orchestration overlap is not observable in these receipts. They provide sequential grouped-build phases and aggregate launcher phases, but no per-agent start/finish intervals, dependency edges, queue waits, or overlap markers. Therefore the CP109 lower incremental build time cannot be attributed to parallel agents or interpreted as a shortened critical path. A measurable next experiment is to capture task start/end and dependency timestamps alongside a frozen, unchanged full launcher run; calculate the longest dependency chain and compare it with summed task durations. Keep the current full hash, binary binding, and test inventory. Current measurements support no configuration change and no 10× claim.
