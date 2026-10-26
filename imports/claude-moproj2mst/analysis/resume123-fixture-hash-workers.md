# CP123 fixture-hash worker benchmark

The controlled comparison found no material end-to-end fixture-hashing gain from increasing the worker count from four to eight. All 12 full manifests exactly matched the frozen CP122 before and after manifests, including path order, size, modification time, and SHA-256. Keep four workers on this evidence.

## Frozen inputs and method

The benchmark used the four fixture roots recorded in CP122’s CTest inventory, the existing `fixture_hash_fast.snapshot` implementation, parent-cache path resolution, SHA-256, no persistent cache, and the existing one-megabyte per-file read chunk. Only worker count changed. The alternating order was 4, 8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4, forming six adjacent pairs.

The live source tree had changed after CP122, so it was not treated as the frozen source. Instead, the CP122 archive SHA-256 (`0e61b30e5a06b116a85aac036233831cf1c4b54478c2d5ec23b38e1784646e4e`) matched the CP122 freeze receipt, whose verified source-tree fingerprint is `ead1572502ec4844c977bb33ecfcfc5aa3de1ace5d8a0824f15980d6ededb913` across 3,462 files. All 307 test/public binaries matched the CP122 receipt before and after the benchmark.

Every trial covered 16,728 files and 1,867,669,827 bytes. Each ordered manifest digest was `7298eaa8e734a682c11dd6e09b97996421e8b4474246be380bbf3b3eb22cda69`, and every complete map compared equal to both CP122 fixture snapshots. The full gzip-compressed manifests and machine receipt are preserved in `build/test-runs/cp123-hash-workers-1792921047335107300/`.

## Results

| Trial | Workers | Wall seconds | Process CPU seconds |
|---:|---:|---:|---:|
| 1 | 4 | 6.634 | 12.672 |
| 2 | 8 | 6.421 | 13.172 |
| 3 | 8 | 6.430 | 13.109 |
| 4 | 4 | 6.497 | 13.313 |
| 5 | 4 | 6.461 | 12.906 |
| 6 | 8 | 6.382 | 13.313 |
| 7 | 8 | 6.750 | 15.906 |
| 8 | 4 | 6.840 | 17.844 |
| 9 | 4 | 7.300 | 16.656 |
| 10 | 8 | 6.742 | 13.922 |
| 11 | 8 | 6.676 | 14.125 |
| 12 | 4 | 6.385 | 13.031 |

The four-worker wall median was 6.566 s (range 6.385–7.300); the eight-worker median was 6.553 s (range 6.382–6.750). The unpaired median difference is 0.013 s, about 0.2%. Across adjacent pairs, eight workers were faster in five of six, with a median paired difference of 0.085 s in favor of eight; the sixth pair was 0.291 s slower with eight. The paired effect is small relative to the observed trial spread. Median process CPU was 13.172 s with four workers and 13.617 s with eight, about 3.4% more CPU at eight.

Filesystem cache state was not cleared or controlled, and host CPU/I/O load was not recorded. The six pairs are sufficient to rule out a large improvement here, but not to resolve a sub-percent difference under controlled conditions. There is no evidence for a material worker-count speedup, no fixture or binding compromise, and no 10× improvement. Retain four workers. No full launcher, CTest suite, build, or source/binary modification was performed.
