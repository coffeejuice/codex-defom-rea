# Fixture hash phase profile

Path canonicalization is the largest measured cost and largely controls the two-worker schedule. In the instrumented single-worker pass, resolution and containment took 6.171 seconds of 14.818 seconds, versus 4.298 seconds reading bytes and 0.705 seconds updating SHA-256. With two workers, the producer spent 8.330 seconds resolving paths and 1.102 seconds enumerating them within 10.081 seconds of elapsed wall time. Worker phase sums overlap that producer work. Increasing hash worker count cannot remove this serial producer cost.

| Phase | One worker, s | Two workers, s |
|---|---:|---:|
| Enumerate and file classification | 1.144 | 1.102 |
| Sort paths | 0.029 | 0.034 |
| Resolve and check containment | 6.171 | 8.330 |
| Stat, worker sum | 0.614 | 1.136 |
| Open, worker sum | 0.723 | 0.949 |
| Read, worker sum | 4.298 | 4.559 |
| SHA-256 update, worker sum | 0.705 | 0.752 |
| Close, worker sum | 0.122 | 0.505 |
| Relative key and digest formatting, worker sum | 0.905 | 0.981 |
| Total elapsed | 14.818 | 10.081 |

The roots contain 14,868 files smaller than 64 KiB: 88.9% of all 16,728 files, but only 57,479,314 bytes (3.1% of 1,867,669,827 bytes). Per-file filesystem and path work is substantial. Raw SHA computation is a small fraction of elapsed time. The read measurements include Python/OS/cache overhead and do not establish physical disk throughput. No physical cold-cache experiment was performed.

The next bounded experiment should measure canonicalizing the evidence parent once per snapshot while retaining per-file canonicalization, relative-path checks, complete reads and ordering. The existing `_child` resolves the identical parent for every occurrence. This is a candidate, not a demonstrated saving: parent resolution was not separately timed, and Windows path checks must still reject escaped symlink targets. Before adoption, compare complete mappings and order, test failure paths, and state the existing quiescent-filesystem assumption. Do not remove file reads or use size/mtime as a substitute for SHA-256. Moving per-file resolution into workers is another measurable candidate, but it changes scheduling/error order and needs explicit compatibility review.

Both passes matched the retained serial ordered snapshot digest, including all paths, sizes, nanosecond mtimes and file hashes. No fixtures were modified; no tests, builds or original application were executed. The orchestrator reserved the I/O window. The profiler is `optimization/test-speed/tools/profile_fixture_hash.py`.

Measured read-only using the exact helper schedule and retained four roots. All ordered mappings match the retained serial snapshot digest. Timers add instrumentation overhead; worker phase sums overlap each other and producer time under two workers, so they must not be added to explain wall time. Enumeration includes directory traversal and is_file checks; ordering measures path sorting only. Future waits/result insertion remain in unassigned wall time. OS cache was not cleared.

```json
[
  {
    "workers": 1,
    "wall_seconds": 14.817566299999953,
    "producer_phase_seconds": {
      "enumeration_and_is_file": 1.1437759999998889,
      "path_sort": 0.029016000000410713,
      "path_resolution_and_containment": 6.171403499983398
    },
    "worker_phase_summed_seconds": {
      "stat": 0.6136383000014121,
      "open": 0.7234155000080591,
      "close": 0.12174679999270666,
      "read": 4.297710200015445,
      "sha256_update": 0.7046388000176194,
      "key_and_digest_format": 0.90527909998832
    },
    "entries": 16728,
    "bytes": 1867669827,
    "read_blocks": 17816,
    "files_under_64KiB": 14868,
    "bytes_under_64KiB": 57479314,
    "snapshot_ordered_sha256": "7298eaa8e734a682c11dd6e09b97996421e8b4474246be380bbf3b3eb22cda69",
    "matches_retained_serial_snapshot": true
  },
  {
    "workers": 2,
    "wall_seconds": 10.081206800000018,
    "producer_phase_seconds": {
      "enumeration_and_is_file": 1.1021444000000429,
      "path_sort": 0.03384800000003452,
      "path_resolution_and_containment": 8.329858399984687
    },
    "worker_phase_summed_seconds": {
      "stat": 1.1357897000041248,
      "open": 0.9485293999982787,
      "close": 0.5047356000081891,
      "read": 4.558678400004283,
      "sha256_update": 0.7515517999959229,
      "key_and_digest_format": 0.9813069999975141
    },
    "entries": 16728,
    "bytes": 1867669827,
    "read_blocks": 17816,
    "files_under_64KiB": 14868,
    "bytes_under_64KiB": 57479314,
    "snapshot_ordered_sha256": "7298eaa8e734a682c11dd6e09b97996421e8b4474246be380bbf3b3eb22cda69",
    "matches_retained_serial_snapshot": true
  }
]
```
