# Converter release test profile

`project_files_unchanged` has a historical JUnit duration of about 3.37 seconds. That is the
reported test-level baseline; no fresh run was made for this change, so it is not a before/after
measurement.

Set `M2M_RELEASE_PROFILE=1` in the test process environment to emit additional tab-separated
`M2M_RELEASE_PROFILE` rows, one per existing corpus case and output mode. The columns after the
marker are case id, mode, fixture-copy microseconds, CLI-call microseconds, before-snapshot
microseconds, after-snapshot microseconds, snapshot-comparison microseconds, accumulated SHA-256
microseconds, and accumulated metadata-stat microseconds. The ordinary output and checks remain the
same when the variable is unset.

Snapshot timings include directory traversal and map construction. Hash and metadata timings are
subsets of those snapshot times, so do not add them to snapshot durations. Comparison timing
includes comparison and the output-folder stray-file check. The fixture-copy value
times only `fs::copy` and output-folder creation. CLI timing includes process creation, wait, and
exit-code collection. The profiling clock is steady-clock and reports microseconds; the test's
content-integrity behavior remains SHA-256 based, including the same-size/same-last-write-time
mutation self-check.

Collect a new baseline by running the focused `release.unchanged` test with the environment
variable set and recording both its JUnit duration and the emitted per-case rows. This task did not
build or run the converter test suite.
