# Parent-cache classification reuse: rejected experiment

The proposed engine produced only a marginal timing improvement and was removed from the production helper. Accepted parent-cache remains the available cached engine; runner defaults and CLI were never changed. The isolated experimental implementation is retained as `optimization/test-speed/tests/fixture_hash_classify_candidate.py`, with synthetic checks in `test_hash_parent_classify.py`.

Four-worker paired timings on the exact four roots were accepted parent-cache 6.204/6.144 seconds and classification reuse 6.025/5.970 seconds. Medians are 6.174 and 5.997 seconds: **0.176 seconds (2.86%) saved per snapshot**, only about 0.353 seconds for two snapshots by arithmetic. Two samples do not establish a robust small effect; added classification/error complexity is not justified by this evidence. All four complete ordered mappings matched the retained serial digest across 16,728 files / 1,867,669,827 bytes. Cache state was uncontrolled, order was A/B/B/A and the orchestrator reserved fixture I/O. No originals or CTest ran in the benchmark window.

The candidate used lstat during unchanged rglob traversal, retained ordinary-file classification through sorting, and kept direct-root full resolution and worker stat/SHA reads. Reparse/unknown entries used is_file plus exact resolution. On lstat failure, is_file's filtering determined whether the missing entry was excluded or the original failure was propagated. This requires quiescent files/stable topology; moving classification earlier changes when failure can surface. Actual Windows inside/outside junction tests passed; actual file symlink creation remained unavailable (WinError 1314). Same-size/same-mtime mutation, nested/duplicate/missing roots, reparse and uncertain metadata fallbacks, visible-file lstat denial, missing-entry filtering and is_file fallback errors passed. The isolated candidate suite passed nine tests with one link-privilege skip. A final broader hash test discovery ran 37 tests: 33 passed, four actual symlink tests skipped.

Rollback restored the exact helper text obtained by reversing this experiment's addition and dispatch changes, verified both by normalized-text equality and AST equality. The text diff excluding newline style is empty. There is no prior frozen byte copy available: rollback normalized mixed newline bytes, so the accepted freeze's byte hash needs renewal after the root's full-suite validation. Root explicitly instructed retaining these normalized bytes rather than attempting further reconstruction. The helper will not be altered further in this lane.

- Prior frozen SHA-256: `fcd457a7b18dc434cdae865b5bbdc087400b733da946b5f7043c7e07a005c6bd`
- Restored normalized SHA-256: `68d86366832c4f1c7fc8a42b962e56c565c7f9cfe5d408e93cc0423b4674eba9`

```json
[
  {
    "engine": "parent-cache",
    "seconds": 6.2036564,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "engine": "parent-cache-classify",
    "seconds": 6.024515000000065,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "engine": "parent-cache-classify",
    "seconds": 5.970393399999921,
    "ordered_mapping_matches_retained_serial": true
  },
  {
    "engine": "parent-cache",
    "seconds": 6.143779699999868,
    "ordered_mapping_matches_retained_serial": true
  }
]
```

Candidate helper SHA-256: `8f5f7ad17eaa6aee927583c02365cc5b270ee7da3d49f29289f32e6dbd5efc19`.
