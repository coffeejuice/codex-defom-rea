# Checkpoint 70: next Save partial-failure ordering gap

## Distinct gap after the SimCtrl sequence

CP66/68 cover failed or flushed SimCtrl.KEY followed by the PPV writer. The next independent boundary is the **native project writer's post-open persistent QFile error**, after Save step 2 has already completed.

Retained `analysis/wave3-project-close-status-contract.json` binds original writer `0x1400983c0` and QtCore `0x670b4840`/close path: the project writer closes QFile, then returns false iff `QFile.error()` is nonzero; it does not query `QTextStream.status()`. QFile close saves the initial buffered-file flush result before `aboutToClose`. If that initial flush fails, a later close/stream callback does not erase the saved file error. If the initial flush succeeds and close succeeds, it clears the error even when the about-to-close text flush failed. The retained contract explicitly does not qualify concrete original partial bytes.

Original top-level Save still proceeds to property writers and MST after the project writer returns false. Candidate `core/convert.cpp` likewise stores `saved.call.projectWriteSucceeded`, runs step 4, runs MST and remesh, then maps a false project result to `M2M_E_WRITE`. Thus the expected observable order for a persistent post-open failure is: project target was opened/truncated and attempted; later property sidecars are attempted; MST is written; then the adapter reports project-write failure. The project's final bytes remain unqualified and should not be asserted against a guessed partial prefix.

## Coverage already present

- `save_project_open_failure_preserves_project_but_continues_boolean_and_mst` covers project **open** failure, preservation of prior project bytes, later Boolean BWZ/INI, and MST.
- `project_native_locked_direct_write_uses_post_close_qfile_error` parses the project before locking and exercises a large direct-write failure. On this candidate host the initial QFile buffer flush then succeeds, clears the earlier error at close, and the direct writer returns true with an empty target. It covers the easily confused QTextStream-error-versus-QFile-error branch, not a false post-close QFile error.
- `tests/mdt/test_file_device.cpp::file_device_error_after_close_uses_saved_initial_flush_result` covers the lower-level persistent-error versus cleared-error state, but not the project writer or top-level Save continuation.

## Narrow regression proposal and dependency

First add a focused native project-writer case with an already parsed disposable Project and a serialization payload calibrated around the 16 KiB QTextStream/0x4000 FileDevice buffering boundary. Keep an exclusive byte-range lock on the output during `saveProjectFileNative`; arrange for pending QFile bytes to exist before close's *initial* flush. Assert only that the direct writer returns false from nonzero post-close `QFile.error()` and that close was attempted. Do not assert a concrete project byte prefix/size unless the selected Windows run itself proves that candidate effect; do not attribute that residue to the original.

To cover the later side effects in one public conversion test, the fault must be armed after the input `.moproj` has been read but before `saveProjectFileNative` opens the output. A lock installed before public `m2m::convert` also blocks the input read, as documented in `analysis/resume48-project-writer-failure-diagnosis.md`; the current failed-open test is not a valid substitute. No existing external callback is designed for this phase boundary. The smallest reliable integration dependency is a narrowly scoped native-project-writer fault hook/device factory in the Save effects/session path, or an equivalent test-only barrier immediately after Open and before project output. With that hook, assert later Boolean sidefiles and MST are present and the public adapter reports its existing `M2M_E_WRITE` mapping only after those effects. Keep the direct writer state assertion separate from original parity claims.

No source/test/build/GUI/mutable REA change was made for this audit.
