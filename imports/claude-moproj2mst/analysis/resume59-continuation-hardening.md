# CP59 completion-gate evidence hardening

## Changes

`admin/check_continuation.py` now requires typed, hash-bound evidence records for every gate whose `passed` value is true. Each record must contain:

- `path`: a file contained in the converter tree;
- `sha256`: matching the current file bytes;
- `checkpoint`: matching the checkpoint derived from the verified candidate receipt’s validation path;
- `source_tree_sha256`: matching the verified candidate receipt; and
- `claim`: exactly matching the gate key.

Passed gates without a valid verified-candidate receipt/source/checkpoint binding fail closed. Gates with `passed: false` keep the existing behavior, so legacy path strings or empty evidence on pending gates do not invalidate the current incomplete state.

For `fresh_independent_comparison`, the cited JSON must also explicitly say `fresh_original_comparison: true`. The checker rejects `objective_complete: false`, `historical_only: true`, or `fresh_capture_in_this_chat: false`. This prevents the existing CP57 historical-only, incomplete validation from being used as proof of a fresh original comparison, even if its file hash and checkpoint are correctly recorded.

## Tests and current state

Expanded `admin/test_check_continuation.py` from 9 to 13 cases, covering valid typed completion records, evidence hash mismatch, checkpoint/source/claim mismatch, rejection of a historical `objective_complete: false` validation as fresh-comparison evidence, and backwards compatibility for false/pending gates containing legacy evidence values. The existing path-containment and cursor-consistency cases remain.

Validation run:

- `python admin/test_check_continuation.py`: 13 tests passed.
- `python admin/check_continuation.py`: `CONTINUE`, 15 remaining queue items, all eight completion gates pending, no errors. The live state was read only by this checker invocation and was not edited by this task.

An initial unit run failed only because the newly added pending-gate test expected `CONTINUE`; the checker correctly returns `INVESTIGATE_BLOCKERS` when all queue items are verified but required gates are pending. The expectation was corrected and the full checker unit suite passed on rerun.

## File bindings

- `admin/check_continuation.py` SHA-256: `bf20db4496ded09984d635a2d272389404239e02065ed6dbdc80b7f4ebff1afa`.
- `admin/test_check_continuation.py` SHA-256: `8f7082799d9588dd1a5ba30c5268e68da7ec55c0af3a5f970943ebf78c7bcb1f`.

No source, CMake, build, application, REA, recovery queue or completion gate was changed. This strengthens the recorded-evidence invariant; it does not establish converter correctness or acceptance.
