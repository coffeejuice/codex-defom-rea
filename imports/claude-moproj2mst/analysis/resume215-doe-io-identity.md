# CP215 DOE lazy-read I/O identity

## Result

The owned source closes the identity gap without changing the supported
source-less API or fabricating a native receiver. `InputTableSource` now carries
the exact `StudyIo*` used by `readLazyStudy` to obtain the InputTable bytes.
At the retained Type2 notifier boundary, the reader requires that identity to
match the independently supplied caller context's `io` before entering the
existing live Session/Project/SUB/Study/I/O/manager/notifier checks. Thus the
reader identity is checked separately from the weak-owner identity checked by
`enterSetOprHasDoeNotifierPrefix`.

Only the actual lazy-read source sets `readerIo`; existing value-only and
source-less callers retain a null identity and their prior boundary. The new
check runs only for the already contextual notifier branch, after the
constructor prefix and before notifier state (`flag90`) is changed. It neither
creates a key, map entry, receiver, nor callback.

## Focused coverage

Added `keyword_notifier_rejects_io_different_from_input_table_reader` to the
existing DOE notifier-entry test. It passes an internal source whose reader I/O
differs from the otherwise valid actual notifier context, expects an
unsupported refusal, and checks that `flag90` and notifier attempts are
unchanged while the already committed constructor prefix remains. The existing
`keyword_notifier_wired_suppression_preserves_flag_and_stops_before_named_lookup`
test exercises the same-object production bridge through `parseSubProject` and
`readLazyStudy`, reaching the existing suppressed notifier outcome.

`python admin/check_test_source_preflight.py
source/apps/moproj2mst/tests/doe/test_keyword_notifier_entry.cpp` passed. The
focused compiled test was not run in this worker turn; the converter build and
test run remain serialized at integration. No build, GUI, original, Frida, or
REA state was touched, and no shared plan/state file was edited.
