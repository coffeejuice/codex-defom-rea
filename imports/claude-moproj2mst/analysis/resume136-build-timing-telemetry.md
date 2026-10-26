# CP136: additive build timing telemetry

`admin/build_candidate.py` now records the cost of its existing fingerprint calls and separates command time from the grouped step's post-command binding work. No fingerprint algorithm, call boundary, command, retry condition, artifact selection, validation condition or exit-code expression changed. Root owns real builds and their timing receipts. This lane ran no compiler, CMake command, GUI or original target.

## Exact change

The new private `_FingerprintTiming` callable delegates once to the existing provider with the same source argument, returns its result unchanged, and records elapsed monotonic time after successful return. It supplies additive `source_fingerprint_timing` receipt fields:

- `completed_calls`: number of completed existing fingerprint calls.
- `elapsed_seconds`: sum of their unrounded monotonic durations, rounded to six decimals for output.
- `sample_seconds`: durations in occurrence order, rounded independently to six decimals.
- `scope`: identifies hashing and file counting as the measured operation.

Both grouped and legacy modes use that wrapper. Grouped injection through `fingerprint_fn` remains supported. Provider exceptions propagate as before; an incomplete failing call is not counted as completed and does not produce a success receipt. There is no caching, extra traversal, changed ordering or separate file-count operation.

Each step also has additive `command_elapsed_seconds`, measured from the log-open boundary through synchronous runner completion, before post-command source/cache measurement. In grouped mode this ends before the existing post-command source/cache fingerprint. Existing `elapsed_seconds` fields retain their prior timing scope, including that post-command work for grouped steps. Fingerprint timing overlaps some grouped step timing; consumers must not add them as disjoint phases. Legacy command timing has almost the same scope as its existing step timing. No CMake/Ninja log parser or action-count acceptance rule was added in this timing-only increment; the complete logs remain retained.

Receipt schema/name and all existing fields remain compatible. Source fingerprints still contain the path, NUL separator, per-file SHA-256 and file count from the original single traversal. All source/cache comparisons, drift retries, full-build gate and final binding decisions remain in place.

## Measured baseline and validation

The read-only CP135 baseline used accepted CP132/133-final3/134 grouped envelopes of 31.824/33.846/38.129s, full-build steps of 23.125/23.078/29.313s, and launcher/CTest scopes of 31.235/13.734, 30.484/13.578 and 30.203/13.640s. Ninja logs showed 292/292/294 links in those full steps. Fingerprint-specific time was unavailable; the new fields address that gap. No production timing improvement is claimed, and no gate was removed to achieve a faster result.

Validation performed:

- `python -B -m unittest discover -s admin/tests -p test_build_candidate_grouped.py -q`: 13/13 passed in 0.343s. These existing tests use fake commands and exercise success, source/cache drift retries, retry exhaustion, failures and legacy/grouped CLI receipt compatibility. No test source was edited.
- Python AST parse passed for the modified script.
- An inline pure check used a deterministic clock to verify the wrapper returns the identical provider object and reports 0.125s, and verified that a provider exception propagates.
- A fake grouped happy path under `build/telemetry136-static` retained exactly 11 provider calls and one configure/focused/full command each. All steps contained nonnegative command timing, and before/after source fingerprints matched. Disposable fake artifacts were removed with the temporary directory; the compact verification receipt remains at `build/telemetry136-static/verification.json`. Fake timings are functional checks, not a build benchmark.

Only `admin/build_candidate.py` and this report were edited. Real build/freeze acceptance and interpretation of the first production telemetry values remain root-owned.

Frozen script SHA-256: `9e04249fd5778bc06192b4d0188d173cd8dce9f9260cde5d7dadb4d33fa54eec`.
