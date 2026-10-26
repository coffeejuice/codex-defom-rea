# Evidence-parent resolution hoist

The helper now resolves the evidence parent once per snapshot and retains per-file canonicalization, containment validation, full SHA-256 reads, original enumeration and duplicate processing. The original evidence argument still controls relative-key formatting. No persistent cache was added.

Synthetic checks: four new cases pass (nested/duplicate/missing roots, missing-only roots, parent resolution count, and mocked escaped canonical target); actual outside symlink test skipped with Windows WinError 1314. All ten existing runner/hash tests pass, including read/stat failures and same-size/same-mtime mutation detection. The OS symlink test is retained to run where link creation is permitted.

Exact four-root benchmark, uncontrolled warm cache, exclusive assigned fixture I/O window, no builds/tests/original app, all mappings and ordering equal the retained serial digest:

```json
[
  {
    "pass_number": 1,
    "workers": 1,
    "seconds": 12.334515600000032,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "pass_number": 2,
    "workers": 2,
    "seconds": 7.637329299999919,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "pass_number": 3,
    "workers": 4,
    "seconds": 7.398657500000127,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "pass_number": 4,
    "workers": 4,
    "seconds": 7.47561440000004,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "pass_number": 5,
    "workers": 2,
    "seconds": 7.571966599999996,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "pass_number": 6,
    "workers": 1,
    "seconds": 11.948650600000065,
    "entries": 16728,
    "ordered_mapping_matches_retained_serial": true
  }
]
```

Helper SHA-256: `7db091b34138a1ff1f68d66a325d8afbd73e0879cc05c3903a9a9d489733515b`.

| Worker count | First pass, s | Second pass, s | Median, s |
|---|---:|---:|---:|
| 1 | 12.335 | 11.949 | 12.142 |
| 2 | 7.637 | 7.572 | 7.605 |
| 4 | 7.399 | 7.476 | 7.437 |

The two-worker median decreased from the previous 10.831 seconds to 7.605 seconds (29.8% reduction). These measurements were collected in separate windows, not an interleaved before/after experiment; cache and background OS activity remain uncontrolled. Four workers were only 0.168 seconds faster than two here, insufficient to justify changing the conservative two-worker choice from two samples alone. Against the previously measured warm serial 14.587 seconds, two workers now take approximately half the time. Full-launcher savings require measurement.

The change assumes the evidence parent remains fixed during capture, alongside the existing quiescent-fixture requirement. Per-file targets are still resolved immediately before scheduling each hash and rejected if equal to or outside the canonical parent. No root deduplication or missing-root behavior change was introduced. Canonicalization of the parent now happens at snapshot entry after worker validation; errors in resolving the parent may therefore surface before root enumeration, including for an empty root set. The normal absolute, readable evidence root used by the launcher is unchanged.
