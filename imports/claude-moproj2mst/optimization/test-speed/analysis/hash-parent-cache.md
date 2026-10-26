# Opt-in canonical-parent cache

The optional `path_engine="parent-cache"` avoids repeated canonicalization of ordinary files' parent directories while retaining complete file hashing. The default remains `path_engine="exact"`; the runner is unchanged. The parent cache is local to one snapshot and contains paths only, never file contents, metadata or hashes.

For files discovered by directory enumeration, the helper reads `lstat` metadata. Windows reparse flags, symbolic/nonregular file types, or unavailable Windows reparse attributes force the existing full per-file resolver. A failed metadata read aborts instead of returning an unchecked file. Ordinary files use a fully resolved parent plus their enumerated filename, followed by the same containment test. Explicit file roots always use full resolution, preserving canonical names when the caller supplies differently cased paths. Directory junction parents are resolved before caching; paths escaping evidence are rejected.

This opt-in requires quiescent files and stable directory/reparse topology during a snapshot. Replacing or retargeting a parent during capture is unsupported; no stronger race guarantee is claimed. Enumeration, root order, duplicate occurrences, emitted key order, stat values and complete SHA reads remain unchanged on the validated fixtures. The extra lstat can surface classification failures earlier than the exact engine; no partial snapshot is returned.

| Workers | Exact median, s | Parent-cache median, s | Reduction |
|---|---:|---:|---:|
| 1 | 12.429 | 10.395 | 16.4% |
| 2 | 7.755 | 6.808 | 12.2% |
| 4 | 7.571 | 6.134 | 19.0% |

The benchmark read the same four roots used by the retained serial manifest, covering 16,728 files / 1,867,669,827 bytes per pass. Every one of twelve snapshots matched its ordered mapping digest, covering relative keys, sizes, nanosecond mtimes and hashes. First order was exact/cache at 1, 2, 4 workers; the second reversed that order. The orchestrator reserved the heavy fixture I/O window. No tests, builds, original application or fixture writes occurred during it. OS cache state and unrelated OS activity were uncontrolled.

Four cached workers were consistently faster than two in these two repeats. This suggests an opt-in full-launcher trial at four workers; it is not yet evidence to change defaults. The cached four-worker median saves approximately 1.62 seconds against the exact two-worker median per isolated snapshot, or 3.24 seconds for two snapshots by arithmetic only. Actual launcher benefit remains unmeasured.

Validation: 22 synthetic tests ran, 20 passed and two actual file-symlink tests skipped because Windows denied creation (WinError 1314). Actual directory junction tests passed for both inside-target equivalence and outside-target rejection. Mocked reparse flags and missing reparse attributes both force full resolution. Nested/duplicate/missing roots, direct file roots, same-size/same-mtime content mutation, read/stat errors and default runner behavior remain covered. The tests are in `optimization/test-speed/tests/test_hash_parent_cache.py`, `test_hash_parent_hoist.py`, and `test_runner_hash.py`. No claim is made that actual file symlink behavior was executed on this host.

Benchmark rows (all full file reads and ordered mappings checked against retained serial digest):

```json
[
  {
    "repeat": 1,
    "engine": "exact",
    "workers": 1,
    "seconds": 12.611319200000025,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "repeat": 1,
    "engine": "parent-cache",
    "workers": 1,
    "seconds": 10.392689000000018,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "repeat": 1,
    "engine": "exact",
    "workers": 2,
    "seconds": 7.708781899999849,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "repeat": 1,
    "engine": "parent-cache",
    "workers": 2,
    "seconds": 6.695034400000168,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "repeat": 1,
    "engine": "exact",
    "workers": 4,
    "seconds": 7.508271400000012,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "repeat": 1,
    "engine": "parent-cache",
    "workers": 4,
    "seconds": 6.067725499999824,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "repeat": 2,
    "engine": "parent-cache",
    "workers": 4,
    "seconds": 6.20127320000006,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "repeat": 2,
    "engine": "exact",
    "workers": 4,
    "seconds": 7.633883999999853,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "repeat": 2,
    "engine": "parent-cache",
    "workers": 2,
    "seconds": 6.920813399999815,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "repeat": 2,
    "engine": "exact",
    "workers": 2,
    "seconds": 7.800616499999933,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "repeat": 2,
    "engine": "parent-cache",
    "workers": 1,
    "seconds": 10.397778900000048,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "repeat": 2,
    "engine": "exact",
    "workers": 1,
    "seconds": 12.246575099999973,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  }
]
```

Helper SHA-256: `fcd457a7b18dc434cdae865b5bbdc087400b733da946b5f7043c7e07a005c6bd`.
