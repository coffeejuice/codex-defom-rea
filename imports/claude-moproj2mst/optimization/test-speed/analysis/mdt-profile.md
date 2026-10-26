# MDT writer test profiling

`tests/mdt/test_mdt_write.cpp` now supports opt-in phase timings. Set
`M2M_MDT_TIMING=1` in the test process to emit aggregate milliseconds and call
counts to stderr after the test runner finishes. With the variable unset, the
test emits no additional timing output and keeps its assertions, fixtures,
conversion flags, and file operations unchanged.

The current timing buckets are:

- `convert`: calls to `convertToBytes` through the test helper.
- `corpus-scan-and-compare`: the corpus-reproduction case, including directory
  traversal, conversion, and assertions.
- `mdt-read-and-byte-compare`: reference-file reads and byte comparison for
  individual output MDTs.
- `oracle-b-case`: one full Oracle-B case, including scratch copy, input edits,
  conversion, comparison, and cleanup.

Buckets are nested intentionally, so their totals must not be summed to infer
test wall time. Compare the test runner's external wall time with the nested
phase totals. The timer only reports observations; no timing run has been made
for this change, and no speedup is claimed.

## Optimization candidates to evaluate

The corpus test runs a complete conversion for each qualifying project and the
converter's MDT-only route still opens the active operation and calls
`engine::saveCurrent` before writing MDT bytes. That work may dominate, but it
is fidelity-critical behavior documented in `src/core/convert.cpp`; skipping
it is not a safe test-speed optimization. First compare `convert` against
`mdt-read-and-byte-compare` and external wall time across repeated runs.

If fixture handling is material, Oracle-B currently recursively copies the
same source project once for each of three cases, then removes the copy. A
follow-up experiment could use a single immutable source plus three fresh
scratch directories, or stage the source copy once and reset only the edited
files. Preserve each case's independent starting bytes and cleanup semantics;
benchmark this separately before changing it. If byte-reference reads matter,
consider caching reference bytes within the test process by path, but only
after confirming repeated references actually occur and measuring the cost.

Do not combine any proposed test-harness optimization with converter behavior
changes. Keep the default test output and all comparisons identical.
