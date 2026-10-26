# CP71–73 efficiency review

## Purpose

Assess whether recent grouped builds or launcher settings have produced a measured, causal speedup, then select one safe next experiment that retains every fixture hash, source/binary binding, registered test, and assertion.

## Measurements

| Checkpoint | Successful build receipt | Configure / focused / full (s) | Full launcher (s) | CTest (s) | Fixture hashes before + after (s) | Bindings before + after (s) | Test result |
|---|---|---:|---:|---:|---:|---:|---|
| CP71 | `resume71-integrated-grouped2-build.json` | 1.968 / 3.000 / 20.250 | 29.985 | 12.407 | 6.469 + 6.422 | 1.781 + 1.812 | 216 registered; exit 0 |
| CP72 | `resume72-integrated-grouped-build.json` | 1.922 / 11.406 / 27.547 | 30.063 | 12.250 | 6.734 + 6.359 | 1.860 + 1.812 | 217 registered; exit 0 |
| CP73 | `resume73-integrated-grouped2-build.json` | 1.938 / 4.203 / 20.766 | 29.703 | 12.219 | 6.390 + 6.375 | 1.844 + 1.781 | 219 registered; exit 0 |

All three successful test receipts report `completed-with-recorded-gaps`; their CTest runs passed. The launcher took 29.703–30.063 seconds. In CP73, full fixture hashing used the threaded engine with four workers and `parent-cache`, cache disabled; source and binary bindings bracketed the test run. CP73 bound testing to source SHA-256 `25f0af4c32432a0dc082ca1469f64ee8194fafa80a6e69b4331ec4652640ee5f` across 3,302 files, matching the successful grouped build source snapshot. It registered 219 tests and did not report fixture changes.

The repeated launcher phase costs are stable: CTest 12.219–12.407 seconds; fixture SHA 12.765–13.093 seconds total; execution bindings 3.593–3.672 seconds; inventory about one second; report parsing about 0.08 seconds. CTest and the two fixture scans each account for roughly 41–44% of the full launcher.

CP73 also has a failed grouped attempt (`resume73-integrated-grouped-build.json`): configure 1.891 seconds, focused preflight 5.922 seconds, elapsed 11.645 seconds, then a successful stable-source retry. The failed compile reports missing `String` and related parse errors in `test_b122_geometry_variable_prefix.cpp`; the build receipt marks source as unstable during that attempt. This temporal association does not establish that concurrent edits caused the compile error. The retry adds real iteration cost, but no supported measurement attributes that cost to the grouped builder itself. CP71 likewise had a failed first attempt before the successful grouped build, so using only the final attempt understates actual iteration time.

## What is and is not established

No optimization in CP71–73 has a causal end-to-end speedup demonstrated by matched receipts. The grouped builder has varying source snapshots, focused target sets, and incremental build work, so configure/full-build durations cannot establish savings against the earlier split workflow. The successful configure phases remain around 1.9 seconds, but there is no same-source, same-cache, same-target control run. Test inventories also vary (216, 217, 219); their launcher timings are descriptive, not a strict A/B comparison.

The 10x objective remains unmet. The current full launcher is about 29.7 seconds. Against the historical 25.250-second fastest complete launcher observation, the current run is about 1.18x slower, not faster; that historical run is not a matched control. A 10x reduction from that reference would be about 2.525 seconds, far below the current time. The fixture scans alone require about 12.8 seconds. No token savings are measured; test receipts state token measurement unavailable.

CP73's four-worker threaded parent-cache hash path is already active. The earlier isolated benchmark found nearly identical two-, four-, and eight-worker medians (10.831, 10.888, and 10.740 seconds respectively, with only two repetitions per engine). That evidence gives no basis for increasing hash concurrency or claiming that the existing four-worker mode is faster end to end.

## Next safe experiment

Measure CTest parallelism at `-j 4` versus `-j 8` in a paired three-run full-launcher experiment using a newly frozen identical source/binary snapshot, same build tree, same 219-test inventory, same CTest arguments except worker count, and the existing threaded four-worker parent-cache fixture hashing. Alternate the run order to reduce cache/order bias. Preserve and compare the complete before/after fixture manifests, source and binary hashes, test inventory, individual test results, and CTest exit status on every run. Report medians and per-phase timing; reject the variant if any result, output contract, or binding differs.

This targets the measured 12.2-second CTest phase (about 41% of the launcher) while leaving test membership and evidence coverage unchanged. It is a benchmark proposal, not a speedup claim. If full parallelism exposes a test-order dependency or changes a test result, retain `-j 4`; do not suppress or skip tests to improve the time.
