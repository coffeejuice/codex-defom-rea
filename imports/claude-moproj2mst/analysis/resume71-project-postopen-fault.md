# Checkpoint 71: project writer persistent-error test seam

## Native and candidate order

The SHA-256-qualified original `0x1400983c0` / QtCore close contract in `analysis/wave3-project-close-status-contract.json` is specific: after QFile open and QTextStream serialization, the original closes QFile, then queries `QFile.error()`; it does not query `QTextStream.status()`. The QFile close path snapshots the *initial* QFile buffer flush result before emitting `aboutToClose`. An initial flush failure preserves the file error through a later close; an initially successful flush plus successful engine close clears a later stream-callback error. Thus a post-open stream error alone is insufficient to expect a false project-writer result.

Candidate `moproj::saveProjectFileNative` mirrors this: open/truncate, call `saveProjectFileBody`, close inside the body, then return `file.error() == 0`. In `core/convert.cpp`, false is stored in `saved.call.projectWriteSucceeded`; step 4 property writers and MST still run. After remesh and MST handling, candidate maps the project failure to `M2M_E_WRITE`. `ConversionDetails::saveCall` exposes project/MST attempted/returned/succeeded flags, and `returnedCode` exposes the final adapter result, so an integrated assertion can observe this order without inventing another public API.

## Existing coverage and why it does not hit the branch

- The public project-open-failure case proves project open failure preserves prior project bytes while Boolean sidefiles and MST complete.
- `project_native_locked_direct_write_uses_post_close_qfile_error` parses before locking and uses a large serialization/direct write. It asserts the post-close QFile error was cleared and the target became empty; the writer returned true. This is the important control branch, not persistent error.
- `test_file_device.cpp::file_device_error_after_close_uses_saved_initial_flush_result` exercises both persistent error and cleared error directly on FileDevice, but does not invoke the project serializer or outer Save coordinator.

The project writer currently feeds one serialized `QString` through one `Stream << text`; its actual size determines whether QTextStream flushes before close. A lock before public conversion blocks the input project read. The prior byte-zero lock must therefore be armed after Open if the public Save path is to be tested. A public filesystem-only lock is not currently synchronized to this phase, and the direct-writer test's large-payload failure lands on the error-cleared branch.

## Exact test seam and acceptance assertions

The narrow deterministic seam is an internal **project output device factory** accepted by `saveProjectFileNative` (or a single internal `ProjectOutputDevice` parameter), with the production path still constructing the real `FileDevice`. The abstraction must expose `opened()`, `write()`, `flush()`, `close()`, and post-close `error()` so the serializer keeps its existing call order and status query. A test device can:

1. open successfully and model the normal truncating target;
2. accept/record the project payload prefix or delegate to an actual disposable file;
3. inject a pending-buffer write/flush failure that remains in `error()` after close;
4. record `open → serialize/write → initial flush → aboutToClose/stream flush → close → error query` ordering.

For an integrated `convertDetailed` case, pass that same internal factory through the Save session only in tests. Keep Open's input reader on the real file; substitute only the later project **output** device. Use a known Boolean fixture whose later property phase writes BWZ/INI. Assert `saveCall.projectWriteAttempted && projectWriteReturned && !projectWriteSucceeded`, `saveCall.mstWriteAttempted && saveCall.mstWriteReturned && saveCall.mstWriteSucceeded`, existence/content of later sidefiles and MST, and final `M2M_E_WRITE` mapping. Do not assert exact `.moproj` partial bytes; they depend on the injected device contract and are not known for the original Windows fault.

If introducing a factory is too broad, keep two narrow tests: a device-level project writer test proving false iff post-close error is nonzero, and rely on the existing public open-failure case plus `convert.cpp` control-flow for continuation. Do not claim that those separate tests empirically prove post-open-failure integration.

## Evidence boundary

The native writer false result and later property/MST order are statically qualified, not exercised against the original application. The candidate's large direct-write locked case is evidence for the close-clears-error branch only. No source, test, build, GUI, mutable REA, or original binary was changed for this audit.
