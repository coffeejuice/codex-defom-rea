# Bounded fixture hash engine

`admin/fixture_hash_fast.py` implements `snapshot(roots, *, evidence=EVIDENCE, workers=4)`. Import it from the launcher and pass its existing `roots` and `EVIDENCE`. The launcher itself has not been changed.

The result preserves the existing mapping and insertion order: relative path to `[size, mtime_ns, SHA256]`. Enumeration uses the same sorted roots, `Path.rglob`, `is_file`, resolved containment check, pre-read stat, and 1 MiB SHA-256 reads as the launcher. Nested roots and duplicate occurrences are intentionally retained; each occurrence is freshly read. No timestamp cache, sampling, skipped bytes, or changed fixture scope is introduced.

The executor accepts 1 through 32 workers (default 4), queues at most twice the worker count, and consumes results in traversal order. Exceptions propagate without returning partial results; queued futures are cancelled where possible and running reads finish before exit. A single-worker option bypasses the executor. Memory for hashing scales with the worker count; per-root sorted path enumeration retains the original list allocation.

## Synthetic measurement

Python 3.12 on the current Windows host, three consecutive warm-cache samples per configuration. Temporary fixtures contained 385 unique files and 103,022,592 unique bytes: 96 files of 1 MiB, 288 files of 8 KiB, and an empty file. Roots included a directory, a nested directory, an overlapping single file, and a missing root to exercise legacy coverage. The nested directory and file intentionally add repeated reads. Temporary fixtures were removed after measurement.

| Configuration | Samples (seconds) | Median (seconds) |
| --- | --- | --- |
| Existing serial snapshot | 0.561520, 0.575585, 0.563702 | 0.563702 |
| New, 1 worker | 0.731152, 0.621862, 0.567446 | 0.621862 |
| New, 2 workers | 0.388903, 0.388998, 0.429828 | 0.388998 |
| New, 4 workers | 0.409286, 0.357785, 0.338678 | 0.357785 |
| New, 8 workers | 0.350938, 0.338505, 0.414845 | 0.350938 |

Four workers reduced median elapsed time by 36.5% (1.58x) in this synthetic run. This is not a measurement of the full 16,728-file / 1.87 GB fixture roots. Samples ran sequentially while other workers could be active; no cold-cache or storage-isolation claim is made. Four workers remains a conservative candidate, requiring full-root measurement by the coordinator.

Every measured result matched both the existing mapping and its key insertion order. Direct outside-evidence file roots raised ValueError in both implementations. Invalid worker counts (0, 33, True, 1.5) were rejected. Symlink/junction behavior follows the identical traversal and containment code but was not separately exercised. Initial use of the sandbox system temp directory failed with access denied before fixture creation; successful measurement used a unique temporary directory beneath this checkout's optimization analysis directory.

## Integration limits

Both implementations require fixtures to remain quiescent while a snapshot is taken. Concurrent reads change observation timing; neither implementation creates an atomic filesystem snapshot or detects every write race. Errors can surface in a different order if multiple traversal/read failures coincide. Already running reads cannot be cancelled immediately. Windows HDDs, network filesystems, antivirus scanning, and small-file-heavy corpora may make extra workers slower; retain an explicit worker control and benchmark the real roots before selecting it.

The full before/after SHA-256 snapshots must remain enabled, including on test failures. Compare complete mappings and serialized insertion order against the serial launcher on a quiescent full fixture tree before integration. No build, full suite, evidence-root hashing, original application, GUI control, or REA session was run for this change.

## Extended worker-count benchmark (2026-10-24)

Seven rounds per distribution, with worker configurations shuffled deterministically each round (seed 20261024 + round index). Timing excludes fixture creation and comparison, includes full enumeration, resolution, stat, hashing, and executor teardown. Every one of the 196 measured snapshots matched the serial mapping and insertion order. Baseline capture warmed the cache. All fixtures were synthetic, placed beneath a unique temporary directory in this checkout and removed on completion. No full evidence roots were read. Engine code and semantics are unchanged.

Distributions spread files across 16 directories:

- Tiny: 2,048 files x 4 KiB = 8 MiB.
- Medium: 256 files x 256 KiB = 64 MiB.
- Large: 32 files x 4 MiB = 128 MiB.
- Mixed: 384 files x 4 KiB, 112 x 256 KiB, 16 x 4 MiB = 93.5 MiB.

Cells below show median / empirical p95 elapsed seconds. With only seven samples, nearest-rank p95 is the maximum observed sample; it is not a stable production tail-latency estimate.

| Workers | Tiny | Medium | Large | Mixed |
| --- | --- | --- | --- | --- |
| Legacy serial | 2.122666 / 2.341829 | 0.287523 / 0.360722 | 0.105465 / 0.115204 | 0.597319 / 0.688423 |
| 1 | 2.206281 / 2.338302 | 0.289434 / 0.365428 | 0.104836 / 0.113241 | 0.630774 / 0.700870 |
| 2 | 1.717658 / 1.884915 | 0.205669 / 0.255589 | 0.054048 / 0.056545 | 0.431570 / 0.478412 |
| 4 | 1.699272 / 2.014570 | 0.212219 / 0.231411 | 0.035850 / 0.042315 | 0.443445 / 0.507812 |
| 8 | 1.742314 / 1.857022 | 0.194462 / 0.232480 | 0.036648 / 0.041218 | 0.417494 / 0.459432 |
| 10 | 1.584717 / 1.941864 | 0.211498 / 0.232385 | 0.036711 / 0.040700 | 0.428216 / 0.468285 |
| 16 | 1.628833 / 1.965339 | 0.210558 / 0.244018 | 0.036970 / 0.041751 | 0.428199 / 0.473088 |

Median process CPU seconds, summed across this process's threads:

| Workers | Tiny | Medium | Large | Mixed |
| --- | --- | --- | --- | --- |
| Legacy serial | 2.125000 | 0.281250 | 0.109375 | 0.593750 |
| 1 | 2.203125 | 0.296875 | 0.109375 | 0.640625 |
| 2 | 2.546875 | 0.343750 | 0.125000 | 0.703125 |
| 4 | 2.500000 | 0.343750 | 0.125000 | 0.765625 |
| 8 | 2.687500 | 0.359375 | 0.125000 | 0.703125 |
| 10 | 2.484375 | 0.359375 | 0.125000 | 0.750000 |
| 16 | 2.625000 | 0.359375 | 0.156250 | 0.734375 |

### Crossover and contention interpretation

Two workers beat serial elapsed time for every tested distribution. Large files benefit through four workers (2.94x faster than legacy), after which median elapsed time plateaus; sixteen workers consume 25% more median process CPU than four without improving elapsed time. Tiny files improve much less: their best median is at ten workers (1.34x), while the best observed maximum is at eight. Medium and mixed best medians occur at eight (1.48x and 1.43x), but two workers are within 5.8% and 3.4% of those elapsed times. These differences do not establish an exact universal crossover file size: file count and total size also vary between distributions.

Threading spends more total CPU to reduce wall time. Tiny-file four-worker CPU time is 17.6% above legacy for 19.9% lower median elapsed time. Large-file four-worker process CPU / elapsed is approximately 3.49, versus approximately 1.47 for tiny files, consistent with more useful parallel work when hashing larger blocks. The serial path-resolution/enumeration work remains unchanged and small-file scheduling/open/stat costs are a plausible limiter. These measurements do not isolate GIL time, antivirus overhead, filesystem latency, disk queueing, or external CPU contention.

Other workers were active and the benchmark did not reserve CPUs, flush the page cache, or collect disk counters. Consequently this is evidence about warm-cache throughput and aggregate process CPU cost, not a measurement of cold-storage I/O contention. Do not infer that 10 or 16 threads helps the real fixture tree. Keep four as the current conservative default, retain configurable 1/2/4/8/10/16 counts, and let the coordinator choose from a quiescent full-root comparison. Two workers is a credible lower-contention option when the real corpus is dominated by small and medium files; eight is worth measuring but not yet justified as the default.
