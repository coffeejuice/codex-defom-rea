# Resume 48 project writer failure diagnosis

## Initial test failure: precondition was wrong

The original candidate locked byte zero before `m2m::convert()`. That lock blocked the `.moproj` input read during `openProject()`, before native Save. `readProjectFile()` set `openedOk=false`; because this is a full-Save session, conversion retained the constructed default sub-project and continued through activation. `saveCurrent()` then returned false on `!openedOk`, and `convert.cpp` threw `Error(M2M_E_PARSE, "Error on reading project file!")`. This is the exact source-predicted code/message for the reported non-success/no-BWZ/no-INI/no-MST outcome; it is not a project-writer result. The initial alternative expectation of `sub-project 0 does not exist` was incorrect because `initProject()` creates one default sub-project before XML parsing.

The EOF-offset retry still failed because the input reader's read request can extend beyond EOF, overlapping a lock beginning at the old EOF. Thus that public-conversion setup also failed before Save. The current `build/test-runs` logs available to this worker did not retain either new test case's failure output; the code/message above is derived from the call path, not represented as a preserved raw log. Keep the root's original terminal result if available.

## Corrected test scope

The test `project_native_locked_direct_write_uses_post_close_qfile_error` now parses and activates the project first, then acquires the byte-zero lock and calls `moproj::saveProjectFileNative()` directly. A successful control writer confirms output exceeds 16 KiB and enters the direct large-write path. The lock is therefore applied only after Open has completed and only blocks the native project output write.

The source and host probe predict: FileDevice opens the writer handle, truncates the target, then the direct WriteFile fails with ERROR_LOCK_VIOLATION. No buffered QFile bytes remain; close's initial buffer flush succeeds and successful native close clears QFile.error. The project writer samples that post-close QFile.error rather than sticky QTextStream status, so it returns true and the target is empty. The test asserts that writer result and empty target. It no longer asserts property/MST continuation because a direct writer harness cannot exercise the enclosing Save driver.

The existing public `save_project_open_failure_preserves_project_but_continues_boolean_and_mst` remains the public continuation guard for a failed project open. No production code changed and no build/test was run by this worker.
