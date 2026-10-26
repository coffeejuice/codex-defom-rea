# CP215 DOE I/O identity review

## Result: PASS

Reviewed the lazy-read identity change in `source/apps/moproj2mst/src/doe/{activation.cpp,input_table_reader.hpp,keyword_reader_prefix.cpp}`, the production `parseSubProject` bridge, `tests/doe/test_keyword_notifier_entry.cpp`, and `analysis/resume215-doe-io-identity.md`.

### Identity and lifetime

`parseSubProject` constructs `InputTableNotifierContext` from its live `ctx.session`, project, subproject, owned study, `p.doeIo.get()`, and shared `p.doeIo` owner, then calls `readLazyStudy` with that same `*p.doeIo`. In `activation.cpp`, only after `io.readFile(input, bytes)` succeeds, the lazy reader creates `InputTableSource{input, std::nullopt, &io}`, copies the optional notifier context, and synchronously calls `readInputTableFromSource`. Neither source nor context is retained by the study or I/O. The reader pointer is borrowed only during that synchronous path; comparing it does not dereference it. The notifier context's weak owner does not extend I/O lifetime.

### Guard ordering and effects

`readBoundedKeywordConstructorPrefix` first commits the retained constructor prefix. For the contextual notifier branch, it then rejects a null or mismatching `readerIo` before calling `enterSetOprHasDoeNotifierPrefix`. Thus the mismatch retains the earlier constructor effect but precedes all notifier state changes and the deeper live Session/Project/SUB/Study/I/O/manager/notifier checks. It creates no key, map entry, receiver, or callback. The deeper entry validates owner identities before changing suppression/flag state. The check is limited to `source && source->notifier`; the source-less/value-only routes do not acquire this requirement.

### Test validity

`keyword_notifier_rejects_io_different_from_input_table_reader` supplies a valid notifier context and a distinct, live `foreignReaderIo`, then checks the mismatch diagnostic, unchanged `flag90` and attempt count, and retained constructor prefix. Its `project.doeIo->reads().empty()` assertion correctly records that the direct-source fixture bypasses lazy file I/O; it does not claim that the production lazy read uses a different I/O object.

`keyword_notifier_wired_suppression_preserves_flag_and_stops_before_named_lookup` uses `rig.read(&rig.session)`, which reaches `parseSubProject` and the actual `readLazyStudy` wiring, and checks the existing suppressed outcome. Together these cover mismatch refusal and same-object production wiring. The focused compiled test was not rerun for this review; no additional tests were requested.

The resume note says the test path as `source/apps/...`, whereas the repository location is `source/apps/moproj2mst/tests/doe/test_keyword_notifier_entry.cpp`; this is a path shorthand, not a test-content discrepancy.

No source changes were made. The initial recovery status check reported the unrelated adaptive GUI validation blocker (`blocked_external_gui_release`).
