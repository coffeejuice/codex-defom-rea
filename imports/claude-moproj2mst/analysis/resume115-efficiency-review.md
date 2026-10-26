# CP113–115 efficiency review

CTest and full-launcher times remain close to CP110–112 despite the growing test inventory; no measured 10× test-time improvement appears. CP114–115 grouped builds took substantially longer than earlier checkpoints, while the associated repair build at CP115 was incremental and should not be compared with a full build.

| Checkpoint | Successful grouped full-build elapsed | Configure | Focused preflight | Full build phase | CTest | Full launcher | Registered / clean passes |
|---|---:|---:|---:|---:|---:|---:|---:|
| CP110 | 32.616 s | 2.172 s | 2.000 s | 22.515 s | 12.203 s | 30.859 s | 293 / 286 |
| CP111 | 35.589 s (repair 1) | 2.156 s | 2.890 s | 24.500 s | 12.969 s | 31.375 s | 298 / 291 |
| CP112 | 32.371 s | 2.125 s | 2.000 s | 22.234 s | 13.062 s | 29.704 s | 300 / 293 |
| CP113 | 32.901 s | 2.141 s | 2.406 s | 22.328 s | 13.437 s | 31.375 s | 301 / 294 |
| CP114 | 67.616 s | 2.234 s | 9.890 s | 49.344 s | 13.171 s | 29.609 s | 302 / 295 |
| CP115 | 76.578 s | 2.157 s | 12.703 s | 55.765 s | 13.078 s | 29.484 s | 305 / 298 |

CP111 had an initial full-build failure (13.233 s total; full-build phase 2.797 s, exit 2) before repair 1 produced the successful full build above. CP115’s later repair build completed in 11.460 s, but its full-build phase was only 0.812 s after configure/preflight; it is incremental, not a competing full-build result. No failed grouped build is recorded for CP113–115.

The launcher breakdown and fixture hashing are stable in shape:

| Checkpoint | Fixture SHA before / after | Source/binary binding before / after | CTest | Launcher |
|---|---:|---:|---:|---:|
| CP110 | 6.500 / 7.125 s | 1.813 / 2.250 s | 12.203 s | 30.859 s |
| CP111 | 6.687 / 6.968 s | 1.844 / 1.844 s | 12.969 s | 31.375 s |
| CP112 | 6.250 / 5.641 s | 1.891 / 1.844 s | 13.062 s | 29.704 s |
| CP113 | 6.641 / 6.531 s | 1.844 / 1.844 s | 13.437 s | 31.375 s |
| CP114 | 6.094 / 5.609 s | 1.891 / 1.829 s | 13.171 s | 29.609 s |
| CP115 | 5.844 / 5.844 s | 1.844 / 1.844 s | 13.078 s | 29.484 s |

All six runs use four threaded SHA-256 workers with parent-cache path resolution and caching disabled. Each before/after fixture manifest covers 16,728 files totaling 1,867,669,827 bytes. Source and binary bindings remained stable for the respective run; fixture changes were empty. The measured hash phases vary by checkpoint, so the modestly lower CP115 launcher total than CP110 (1.375 s, about 4.5%) cannot be attributed to a change in hashing configuration. That setting did not change.

Coverage increased from 293 registered / 286 clean passes at CP110 to 305 / 298 at CP115. Each checkpoint has six logged-gap tests, one logged-difference test, zero failures, skips, or incomplete outputs, and validation remains `objective_complete: false` with no changed diagnostics. Test elapsed time rose 0.875 s (7.2%) across 12 additional registered tests; average CTest time per registered test is approximately 41.7 ms at CP110 and 42.9 ms at CP115. These descriptive ratios do not establish a regression or a speedup because the source snapshots and test inventories differ. No token measurement is available.

Compared with CP110–112, CP113’s build and launcher are similar; CP114–115 builds are slower in the measured grouped runs, primarily in focused preflight and full-build phases. The receipts do not isolate whether that comes from changed compilation inputs, build-graph invalidation, or machine contention. Keep current fixture hashing and full-suite coverage. A useful next experiment is to capture Ninja command/action counts and per-target elapsed time for CP115’s grouped build on a frozen source/cache, then compare with the next source-stable build; do not change build or test configuration until that comparison identifies repeatable avoidable work. Token use remains unavailable.
