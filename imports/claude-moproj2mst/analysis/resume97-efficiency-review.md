# CP95–97 efficiency review

The grouped build keeps configuration and focused preflight separate from the full Release build, while each test run binds the source tree and binary hashes before and after execution. The three checkpoints show stable test time and a modest CP97 build-time increase; they do not establish a faster build configuration.

| Checkpoint | Configure | Focused preflight | Full build | Build sum | Full test run | Registered / passed |
|---|---:|---:|---:|---:|---:|---:|
| CP95 | 2.140 s | 6.016 s | 33.578 s | 41.734 s | 13.844 s | 259 / 252 |
| CP96 | 2.094 s | 6.188 s | 33.235 s | 41.517 s | 14.062 s | 260 / 253 |
| CP97 | 2.250 s | 7.563 s | 37.078 s | 46.891 s | 13.968 s | 261 / 254 |

All build steps exited zero. Source tree hashes remained stable during each grouped build and each test run. Full tests completed with no failed, skipped, or incomplete-output tests; each recorded six logged gaps and one logged difference. Validation receipts consistently retain the historical 93 SAME / 5 DIFF classification and state that this is not fresh original-session acceptance. CP95–97 thus add one registered passing check per checkpoint without changing the reported gap/difference counts. Test runtime is effectively flat (13.84–14.06 s), while CP97’s build sum is 5.374 s (12.9%) above CP96, mostly in the full build (+3.843 s); three checkpoints do not establish whether that is source rework, cache state, or ordinary machine variation.

The evidence volume includes a 3,386–3,388-file source snapshot and SHA-256 bindings for the test executables and supporting binaries, plus fixture hashing and per-test outputs. The run receipt explicitly records `token_measurement: unavailable`; no token throughput claim is possible. Test throughput is approximately 18.7 passed tests per second at CP97 (254 / 13.968), but this is only a descriptive ratio, not a fidelity-adjusted performance measure.

There is no controlled comparison here of alternate job counts or build settings. Keep the current fidelity checks and bindings. To assess the CP97 build increase, compare a no-op full build and an equivalent grouped build on the same frozen source/cache/artifact set, collecting Ninja command counts and phase timings; change configuration only if repeated measurements show a material improvement without dropping targets, hashes, or tests. Do not describe a faster configuration as proven from these receipts.
