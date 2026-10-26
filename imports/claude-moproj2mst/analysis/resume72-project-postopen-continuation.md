# Checkpoint 72: project post-open error continuation sequence

## Failure and observable ordering

For a native `M2M_FLAG_WRITE_SAVE` conversion, the candidate enters `saveCurrent`, completes step 2, then calls `moproj::saveProjectFileNative` before step 4. That writer opens/truncates the `.moproj`, mutates/serializes the DOM, closes the file, and returns `file.error() == 0`. A post-open persistent QFile error therefore sets `saved.call.projectWriteSucceeded=false`, while `projectWriteAttempted` and `projectWriteReturned` are true.

The converter does not return at that point. It iterates property writers in operation order, applying native effects immediately; then writes MST. It invokes `saveRemeshAtProjectSave` after MST handling even when the project or MST writer returned false. Error priority is: unrecovered remesh outcome → `M2M_E_UNSUPPORTED`; then MST failure → `M2M_E_WRITE` for MST; then project writer false → `M2M_E_WRITE` for project. A focused test must keep the remesh path at `Completed` and MST writable to reach the project-error report. The exact project bytes after failure are not qualified.

Native evidence (`analysis/wave3-project-close-status-contract.json`, original `0x1400983c0` and QtCore close state) supports project error detection after close and the source-qualified top-level Save continuation to property/MST stages. The candidate's `convert.cpp` order above is concrete. `M2M_E_WRITE` is this adapter's diagnostic code; it must not be presented as the original GUI Save return. `ConversionDetails.saveCall` provides independent reached-state flags for project/MST, and `returnedCode` captures the adapter result.

## Exact private-seam regression

No current public input can arm the needed post-open file fault at a deterministic point: a byte lock taken before `convert` also blocks Open's read, while the existing direct project-writer test locks only after parsing and hits the large direct-write/error-cleared branch. Add a private test hook at the native project-output boundary, after the output has successfully opened/truncated and before DOM serialization. Prefer a project-specific output-device factory/interface exposing `opened`, `write`, `flush`, `close`, and post-close `error`, with the normal implementation still wrapping `FileDevice`. The fault device should record calls, accept/open/truncate a disposable target, inject a persistent QFile-style error on close, and never escape the test harness.

Run a full native Save conversion on an existing disposable Boolean fixture with a writable, distinct MST target and no remesh action. Assert in order:

1. injected project target open succeeds and post-close `error != 0`; project writer returns false;
2. `saveCall.projectWriteAttempted`, `projectWriteReturned`, and `!projectWriteSucceeded`;
3. Boolean step-4 sidefiles appear with their normal expected signatures;
4. `saveCall.mstWriteAttempted`, `mstWriteReturned`, and `mstWriteSucceeded`, and the MST terminator is present;
5. remesh reaches Completed (if observable through the private receipt) before error reporting;
6. final `returnedCode == M2M_E_WRITE` and message identifies the project path, not MST.

Keep the project-target byte assertion limited to the fault device's owned trace. Do not assert a native `.moproj` prefix/empty envelope or claim the injected device reproduces a specific original Windows residue. A separate direct writer test can assert the DOM mutation/open-before-serialization behavior; the integrated test's decisive result is the later effects and final status.

## Error masking and limits

A failed MST open/write masks the project error in this adapter because `mstFailure` is raised first. An unrecovered remesh branch also masks it with `M2M_E_UNSUPPORTED`. Either condition would invalidate the intended test. Sidefile effects before the project writer are retained, and no rollback is introduced. Source `convert.cpp` and static evidence govern ordering; no fresh original execution was performed.

No code, test, build, GUI, mutable REA, or original artifact was changed for this analysis.
