# CP116–118 test-runner efficiency review

The launcher has three material measured costs: CTest at 13.25–13.50 s, the two fixture SHA passes at 11.59–12.20 s combined, and source/binary binding at 3.59–4.19 s combined. Inventory and report parsing together stay near one second. Full suite coverage and both before/after bindings remained intact in all three runs.

| Checkpoint | Inventory | Fixture SHA before / after | Source/binary binding before / after | CTest | Parse/report | Launcher |
|---|---:|---:|---:|---:|---:|---:|
| CP116 | 0.906 s | 5.734 / 5.937 s | 1.797 / 1.797 s | 13.391 s | 0.110 s | 29.672 s |
| CP117 | 0.906 s | 5.750 / 5.844 s | 1.844 / 2.343 s | 13.500 s | 0.109 s | 30.312 s |
| CP118 | 0.922 s | 5.985 / 6.219 s | 1.859 / 2.281 s | 13.250 s | 0.109 s | 30.625 s |

Each run used four fixture-hash workers, SHA-256, parent-cache path resolution, and disabled persistent caching. The before/after manifests covered the same 16,728 files and 1,867,669,827 bytes. Each receipt reports zero fixture changes and stable source/binary hashes. CP116–118 registered 306, 306, and 307 tests; clean pass counts were 299, 299, and 300. All retained six logged-gap tests, one logged-difference test, and zero failures, skips, or incomplete output. Token usage was unavailable.

## Safe improvement candidates

`m2m.s.save_folder` is the longest individual test at 11.45–11.64 s, while full CTest is 13.25–13.50 s. It runs the 98-case folder sweep and already has CTest `COST=25`, so it is prioritized by CTest’s scheduler. The next slowest tests are `m2m.s.save_io` (~4.74 s) and `m2m.s.save_current` (~4.34 s). Reducing or splitting the folder cases would alter the existing full-sweep acceptance contract; there is no evidence here to do that. All three runs used CTest `-j4`.

The highest-value low-risk next experiment is a controlled fixture-hash worker-count benchmark, comparing four versus eight threads on one frozen fixture manifest. Fixture hashing is the largest launcher phase outside CTest and currently varies by about 0.6 s between runs; changing only the bounded worker count preserves every file, size, mtime, and SHA check. Use alternating trials, retain complete ordered manifests, and compare wall and CPU time with cache-state limitations reported. Adopt a different count only if repeated measurements show a meaningful gain with byte-identical manifests.

Do not expect CTest `-j8` alone to help based on the prior CP76 controlled pair: it was 0.328 s slower in CTest and 1.062 s slower overall than `-j4`, smaller than the observed 4.5 s variation between `-j4` runs. That comparison is old and not definitive, but gives no measured reason to prioritize CTest worker count. The prior CP80 4 MiB chunk-size trial was also slower than 1 MiB, so keep the existing read chunk size.

An alternative runner optimization is parallel source/binary fingerprinting. The earlier CP63 source-only experiment measured a warm median of 0.872 s with four workers versus 1.332 s serial, but it did not include current binary snapshots or fixture-load contention. Do not claim or implement that end-to-end gain from the microbenchmark; first measure it on a frozen runner snapshot. No measured CP116–118 change produced a material launcher improvement, and none approaches 10×.
