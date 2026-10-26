# CP75: bounded physical QFile post-Close error calibration

## Smallest probe

Use the existing private `ProjectOutputFactory` seam in `tests/moproj/test_project_output_device.cpp` and the real `mst::FileDevice`; do not add a new production fault path. Run one clean control and one lock-on-buffer probe against fresh copies of `133_B_DBSTEP_BOOL3_before` under the configured converter scratch root. The writer test already supplies the exact lifecycle: Open/parse the copied project first, then call `saveProjectFileNativeWithFactory`; the factory is invoked only when the project output is about to open. This ensures the diagnostic lock cannot block input Open, and the test never touches the installed application or a mutable REA project.

In the physical probe, the factory returns a delegating `ProjectOutputDevice` that owns a real `FileDevice`. It accepts actual writes normally. On the first successful `write` whose size is `1..0x4000` bytes, it opens a second handle to the now-created/truncated project path using read/write access and read/write sharing, and acquires an exclusive `LockFileEx` range beginning at offset zero and extending over the candidate project output. Retain that lock through the real FileDevice Close and the single post-close `error()` query. Release the byte-range lock and close the second handle immediately after the query, including on assertion/failure cleanup paths.

The first small accepted write is necessarily buffered by the candidate FileDevice (`block = 0x4000`); taking the lock at that point makes a later actual QFile buffer flush, including Close's initial flush, contend with the locked range. Do not alter or split the writer's output bytes. If the native serializer makes no accepted small write (for example, only a direct write larger than the buffer), mark this one probe as **fixture/profile not qualified for lock-on-buffer** and stop. Do not silently switch to an invented segmented-write simulation and label it physical.

This is a bounded two-run calibration, not a parameter sweep. Preserve the event trace and the requested/accepted write sizes so a reviewer can see whether the lock was armed at the intended point. The existing public detailed-convert factory/service can be used in a later integration test after this writer-level physical condition is first demonstrated; the smallest first probe should remain at the isolated native project writer to avoid conflating file-device calibration with Boolean/MST/remesh continuation.

## What qualifies as physical status failure

The locked run qualifies only if all of these are observed from the actual delegated device:

1. Factory opened exactly the normalized disposable project output path and the real FileDevice reported opened.
2. The lock was acquired after at least one positive, no-larger-than-`0x4000` write had been accepted into the FileDevice buffer and before any later write/Close could flush it.
3. The lock remained held through the actual `aboutToClose` callback, completed FileDevice close/handle release, and post-close error query.
4. At that query, `FileDevice::opened()` is false and the real, unmodified `FileDevice::error()` is nonzero.
5. The writer returns false. Record stream status independently if available; a sticky QTextStream failure alone is insufficient. Conversely, a nonzero QFile error with an ordinary serializer result is the contract signal from `saveProjectFileNativeWithFactory`.

The control must show no lock, the ordinary close callback/order, post-close real error zero, and writer true. The injected status value stays in real-status mode (`reportedError = realError`); do not use the CP73 scripted `7` override in this experiment.

## File residue is a separate observation

Capture the disposable project file's before/after byte count and SHA-256 after the writer/device has closed and the range lock is released. Record the actual bytes only as this candidate Windows run's output. Do not require an empty file, a prefix length, or control-file equality: output may be truncated, partially written, or retain buffered bytes depending on where the real failure occurred. A residue observation is not the status oracle; the qualifying status is the nonzero actual QFile error sampled after Close. Keep any QTextStream failure and the physical file hash in separate fields.

The CP73 pair intentionally allows the real file to be fully serialized while a service reports an artificial nonzero status. This new calibration must instead demonstrate a nonzero `FileDevice::error()` with no override and a byte-range lock held at the relevant actual flush.

## Evidence and boundaries

The implementation contract is explicit in `src/mst/file_device.cpp`: `FileDevice::close()` saves its first `flush()` result before `notifyAboutToClose()`, then releases its handle; the final QFile-style error is retained when that first flush failed. The original static contract in `analysis/wave3-project-close-status-contract.json` ties the original writer to `QFile::close()` followed by `QFile::error()`, and says the writer does not query QTextStream status. `analysis/wave3-file-device-error-contract.json` already records lower-level tests that distinguish sticky stream failure from the saved initial FileDevice flush result. Those tests are native candidate simulations using Windows locks, not execution of the original binary.

The existing `project_native_locked_direct_write_uses_post_close_qfile_error` is a different setup: it acquires a one-byte range lock before the native writer opens output. Its observed writer result is true because the close path recovers/clears the earlier error. It does not contradict the post-buffer lock test above, which arms the lock only after a small write is buffered. If the new probe still returns true or actual error zero, report that exact result and do not promote the scripted CP73 receipt as physical evidence.

After (and only after) this isolated probe qualifies, reuse the same lock-on-buffer decorator in a fresh `convertDetailedWithServices` run to test public continuation. Assert the actually observed project writer false, successful Boolean step 4, successful MST, reached/completed remesh, `SaveCallState` Attempted/Returned true and Succeeded false for project with MST success flags true, and final project `M2M_E_WRITE`. Measure—not predict—the final project residue. This follow-on needs its own run/source binding; it is not covered by this calibration design.

## Scope

This is a proposed headless candidate Windows-device experiment, not original-runtime acceptance. No source, test, build, GUI, installed-app, or mutable REA changes were made for this report.
