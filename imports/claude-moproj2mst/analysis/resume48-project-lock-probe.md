# Resume 48 Windows project-lock host probe

## Result

`admin/probe_project_file_lock.py` created three disposable files under `build/probe-project-file-lock`. For each run it used the same relevant Win32 access/share/disposition shape as `mst::FileDevice`: an external handle with `GENERIC_READ|GENERIC_WRITE` and read/write sharing acquired an exclusive byte-zero, length-one `LockFileEx` lock; a second handle opened with `GENERIC_WRITE`, read/write sharing and `OPEN_ALWAYS` called `SetEndOfFile` at offset zero, then issued one 20,000-byte `WriteFile`.

All three runs were identical:

- `LockFileEx`: succeeded.
- `SetEndOfFile`: succeeded; file size changed from 45,056 to 0 while byte zero was locked by the other handle.
- `WriteFile`: failed, error 33 (`ERROR_LOCK_VIOLATION`), with zero bytes written.
- File size stayed zero through close; final output was empty.

Raw per-run receipts, including handle-call booleans/errors, lengths and SHA-256 hashes, are in `build/probe-project-file-lock/raw-results.json`. The probe script is `admin/probe_project_file_lock.py`.

## Test assertion recommendation

For this host, the new Save I/O test can assert the `.moproj` target is empty after the locked direct write; the `{prior, empty}` envelope is unnecessarily broad here. The test still needs its successful control serialization to ensure it takes the stream's large direct-write path, and should continue asserting M2M_OK plus later Boolean and MST output. This host probe used a synthetic file/payload, not the converter or project fixture, so the integration test remains responsible for confirming the actual serialized path reaches the direct write and leaves the same state.

## Limits

This is a local Win32 API probe only. It did not run the converter or original application, and establishes no original behavioral parity. All scratch files stayed under the designated build probe directory.
