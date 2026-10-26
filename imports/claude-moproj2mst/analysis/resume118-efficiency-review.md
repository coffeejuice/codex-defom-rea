# CP116–118 efficiency review

CP118’s grouped build took 163.710 s, driven by 32.031 s of focused preflight and 123.375 s of full build. Its CTest and full-launcher times remained near CP116–117 at 13.250 s and 30.625 s. This is a build-action increase, not evidence of slower tests or reduced verification.

| Checkpoint | Configure | Focused preflight | Full build | Grouped build total | CTest | Launcher | Registered / clean passes |
|---|---:|---:|---:|---:|---:|---:|---:|
| CP113 | 2.141 s | 2.406 s | 22.328 s | 32.901 s | 13.437 s | 31.375 s | 301 / 294 |
| CP114 | 2.234 s | 9.890 s | 49.344 s | 67.616 s | 13.171 s | 29.609 s | 302 / 295 |
| CP115 | 2.157 s | 12.703 s | 55.765 s | 76.578 s | 13.078 s | 29.484 s | 305 / 298 |
| CP116 final | 2.078 s | 4.172 s | 29.500 s | 41.569 s | 13.391 s | 29.672 s | 306 / 299 |
| CP117 | 2.219 s | 1.859 s | 22.828 s | 32.989 s | 13.500 s | 30.312 s | 306 / 299 |
| CP118 | 2.172 s | 32.031 s | 123.375 s | 163.710 s | 13.250 s | 30.625 s | 307 / 300 |

CP115’s associated test followed an 11.460 s incremental repair receipt, not its 76.578 s grouped full-build receipt. CP116’s final row likewise reports the test-matched final build. The build times are not repetitions on an unchanged source snapshot, so differences do not establish a speedup or regression for a configuration.

## CP118 Ninja actions

The retained logs show:

| Stage | Ninja actions (excluding glob recheck) | C++ compiles | Links |
|---|---:|---:|---:|
| Focused preflight | 161 | 157 | 4 |
| Full build | 589 | 295 | 294 |

The preflight compiles 154 `m2m_core` source objects plus three focused-test objects. The full build compiles 226 test translation units and 69 additional source translation units, then links 294 targets. CP116 final’s full log had 14 compile and 285 link actions; CP117 had zero compile and 285 link actions. CP114/115 logs had 70/83 compile and 288 link actions. CP118 therefore represents a broad rebuild across the core and test graph. The configure phase is only 2.172 s; admin/configure overhead cannot explain the roughly 155 s spent in preflight and full-build commands.

The CP118 build cache SHA is unchanged from prior receipts, and its source fingerprint changed from CP117 while the source file count rose by one (3,456 to 3,457). The build and test receipts bind the same CP118 source hash, and the test runner reports it stable before/after execution. This pattern is consistent with a widely included source/header dependency invalidating many objects, but the Ninja output does not record dependency explanations or per-action elapsed time. The one-file count increase does not reveal which existing files also changed. CPU contention or other machine load cannot be separated from compile time because no host utilization or per-action timings were recorded. So shared-header invalidation is a plausible explanation, not a demonstrated root cause.

## Test and fixture integrity

All CP116–118 test receipts report six logged-gap tests, one logged-difference test, zero failures, skips, or incomplete outputs, zero fixture changes, no changed diagnostics, and `objective_complete: false`. CP118 records 307 registered tests and 300 clean passes. Token usage is unavailable.

Each test run used four threaded SHA-256 workers, parent-cache path resolution, and caching disabled. The before/after fixture manifests consistently cover 16,728 files totaling 1,867,669,827 bytes. CP118’s fixture SHA passes took 5.985 s and 6.219 s; its before/after source and binary binding phases took 1.859 s and 2.281 s. CTest time stayed within 0.359 s across CP116–118. Compared with CP113–115, the test launcher remains around 30 seconds, with no evidence of a 10× test-time improvement or any reduction in acceptance coverage.

## Next measurement

The highest-value next experiment is to explain CP118’s dirty graph before changing build settings: on a disposable copy of the frozen CP118 source/build graph, capture `ninja -d explain -n` output and map every dirty object to its dependency reason and target. Compare those reasons with CP117’s action set and record host load during the next frozen grouped build. This can confirm whether a shared header caused the broad rebuild and separate invalidation volume from contention. Keep the candidate cache, focused preflight, full test inventory, fixture hashes, source/binary bindings, and acceptance gates unchanged.
