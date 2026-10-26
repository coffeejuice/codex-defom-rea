# Four-worker parent-cache phase profile

Filesystem metadata and path preparation still dominate the producer. The single instrumented full-root pass took **6.073 seconds** and matched the retained serial ordered snapshot exactly. Producer phase spans totaled 5.113 seconds (84.2% of wall time). SHA-256 computation is not the limiting cost demonstrated by this profile.

| Producer phase | Seconds |
|---|---:|
| Enumeration and is_file classification | 1.126 |
| Sort paths | 0.029 |
| Per-file lstat and reparse classification | 1.803 |
| Parent resolve/cache insertion, 3,245 directories | 1.510 |
| Parent cache lookup | 0.132 |
| Path join and containment | 0.513 |
| Full-resolution file fallback, one file | 0.0002 |

Workers collectively spent 7.733 seconds in reads, 3.075 in open, 2.160 in stat, 1.178 in close, 0.988 in SHA updates and 0.975 formatting keys/digests. These are sums of overlapping intervals across four threads, not percentages of wall time or additional elapsed cost. Reads include OS/cache latency and scheduling delays; they do not measure physical disk throughput. Worker spans and producer spans cannot be added. The unassigned ~0.960 seconds includes scheduling/wait/insertion, setup and timer overhead; it is not an isolated overhead measurement.

**One next candidate:** within the opt-in parent-cache engine, reuse each ordinary file's classification metadata collected during the existing rglob enumeration. Replace the ordinary-file is_file-followed-by-lstat pair with a single lstat classification that is retained until sorted scheduling. Preserve the current Path.rglob traversal and sort; use the existing is_file/full-resolve fallback for symbolic links, reparse points and uncertain metadata. Keep the worker stat and every file read unchanged. The cache must be snapshot-local and is valid only with quiescent fixtures and stable topology. This avoids a redundant ordinary-file metadata syscall without removing the authoritative worker metadata or hash. It is a proposal, not an implemented or validated change.

The optimistic ceiling for this candidate is **less than 1.126 seconds per snapshot (18.5%)**, because that entire measured phase also includes directory traversal and object creation, which would remain. Even eliminating all of it would reduce 6.073 seconds only to 4.947 seconds, approximately 1.23×; overlap and shifting worker bottlenecks should reduce the real benefit. The profile does not isolate the is_file syscall fraction, so a more precise predicted gain would be unsupported. Moving classification earlier also moves its timer attribution; evaluate total wall time rather than congratulating a disappearing phase.

Required equivalence checks before acceptance: identical ordered key/size/mtime/SHA mappings for all four roots; repeated nested/duplicate/missing/direct-root cases; file symlinks and inside/outside directory junctions; regular directories excluded; unknown reparse classification uses the exact fallback; case-preserving canonical keys; same-size/same-mtime edits still detected; disappearing/unreadable entries reproduce existing is_file filtering or abort behavior as appropriate. Preserve exception semantics explicitly: lstat failures must not silently hide entries that the current filter would include. Actual file-symlink tests remain unavailable on this host without link privilege, so retain that qualification. Run paired full-root timing and full-launcher equivalence before changing any default.

The profile read all 16,728 files / 1,867,669,827 bytes and made 17,816 nonempty block reads. The orchestrator reserved the fixture I/O window. No helper/runner edits, builds, tests, original app runs or fixture writes occurred. The read-only profiler is `optimization/test-speed/tools/profile_parent_cache.py`, using the earlier worker-span instrumentation from `profile_fixture_hash.py`.

Single instrumented pass; worker sums overlap producer time and each other. Timers introduce overhead. Same bounded scheduling and full reads as helper. OS caches were not cleared.

```json
{
  "workers": 4,
  "wall_seconds": 6.073085499999706,
  "producer_phase_seconds": {
    "enumeration_and_is_file": 1.1261006999998244,
    "sort": 0.0288087999997515,
    "lstat_and_reparse_classification": 1.8031353000119452,
    "full_resolution_fallback": 0.00020300000005590846,
    "parent_cache_lookup": 0.13179290000061883,
    "parent_resolve_and_cache_insert": 1.5095603999902778,
    "path_join_and_containment": 0.512936299987814
  },
  "worker_phase_summed_seconds": {
    "stat": 2.160190800026612,
    "open": 3.074605600020277,
    "close": 1.1780494000158797,
    "read": 7.733203100033734,
    "sha256_update": 0.9879973999959475,
    "key_and_digest_format": 0.9745547999850714
  },
  "entries": 16728,
  "bytes": 1867669827,
  "read_blocks": 17816,
  "files_under_64KiB": 14868,
  "bytes_under_64KiB": 57479314,
  "snapshot_ordered_sha256": "7298eaa8e734a682c11dd6e09b97996421e8b4474246be380bbf3b3eb22cda69",
  "counts": {
    "fallback_files": 1,
    "parent_resolves": 3245
  },
  "matches_retained_serial_snapshot": true,
  "helper_sha256": "fcd457a7b18dc434cdae865b5bbdc087400b733da946b5f7043c7e07a005c6bd"
}
```
