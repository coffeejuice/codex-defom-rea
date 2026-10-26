# CP76 CTest parallelism benchmark: `-j 4` versus `-j 8`

## Purpose

Measure whether doubling CTest's worker count reduces the full verification launcher time on one immutable candidate, without changing the fixture hash mode or verification scope.

## Method

Both full launcher runs used `build/candidate`, CP76 frozen source, the same fixture roots, and `--hash-engine threaded --hash-workers 4 --hash-path-engine parent-cache`. The only command-line setting changed was CTest `--jobs`: 4 for the first run, then 8. The source tree and test binaries were hashed before and after each run. The launcher captured the full inventory, pre/post fixture snapshots, full CTest run and JUnit output.

| Run | Test run ID | Jobs | Full launcher (s) | CTest (s) | Fixture SHA before + after (s) | Binding before + after (s) |
|---|---|---:|---:|---:|---:|---:|
| A | `1792887657799670600` | 4 | 34.469 | 16.062 | 6.547 + 6.750 | 1.844 + 2.141 |
| B | `1792887698539223700` | 8 | 35.531 | 16.390 | 6.890 + 6.672 | 2.188 + 2.250 |

Run A's receipt is `build/test-runs/1792887657799670600/receipt.json`; Run B's receipt is `build/test-runs/1792887698539223700/receipt.json`. Timing details are in their adjacent `timing.json` files. The console logs are preserved at `build/logs/resume76-j4-launcher.log` and `build/logs/resume76-j8-launcher.log`.

## Verification outcome

Both runs registered the same 230 tests. Each had 223 passed, 6 passed with logged gaps, 1 passed with logged differences, 0 failed, 0 skipped, and 0 incomplete outputs. The sorted test-name digest was identical for both runs. Both CTest and launcher exit codes were zero, and both fixtures-before/after comparisons reported no changes.

Each run bound to source SHA-256 `da5b45846b1384219108894a5309238e7a6abf2c32b256b97ac42d4722710e49` across 3,327 files, unchanged through CTest. All 227 executable bindings matched before/after. The frozen CP76 archive receipt reports the same source identity; its `moproj2mst.exe`, `moproj2mst.dll`, and `deformdb.dll` SHA-256 values match both runs. Thus the compared candidate and core build artifacts were the same.

## Result and causal limits

In this ordered one-pair measurement, `-j 8` was 1.062 seconds slower for the full launcher (3.1%) and 0.328 seconds slower for CTest (2.0%). It did not show a speedup. Because this is one pair in a fixed order (`-j 4` then `-j 8`), the small difference may reflect system/cache variation or order effects. It is not enough to conclude that `-j 8` is generally slower; it does show there is no evidence here to adopt it for speed. Keep `-j 4` unless an alternating repeated comparison demonstrates a repeatable improvement with the same verification checks.

The 10x objective remains unmet. The fastest full-launcher result here is 34.469 seconds; the 10x target against the historical 25.250-second complete-launcher reference would be 2.525 seconds. The historical point is not a matched control, but neither current run is near that target. This benchmark makes no 10x claim and does not weaken any test or hash coverage.

