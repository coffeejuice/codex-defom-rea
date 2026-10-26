# Resume 52 Cogging shared-lock probe

## Result

`build/cogging-lock-probe-001/probe.py` copied the existing `133_B_DBSTEP_BOOL3_before/Operations/Task00003/Cogging.MDT` into three disposable scratch files. Each run acquired a shared `LockFileEx` lock at offset 0 for 8,192 bytes through a `GENERIC_READ|GENERIC_WRITE`, read/write-shared handle. The lock allowed reads while denying writes. A second handle then used the same relevant Cogging path shape: `CreateFileA(GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, CREATE_ALWAYS)`, `_open_osfhandle(_O_WRONLY|_O_TEXT)`, `_fdopen("wt")`, and a pinned 4,096-byte fully buffered CRT stream.

All three runs had identical outcomes:

- Initial copied target: 8,523 bytes, SHA-256 `5b27e8d7b8ed8b2f51f05b60038e98ac0602465ac1929f8d6e167531c2bb078a`.
- Shared lock acquisition succeeded.
- `CREATE_ALWAYS` succeeded and truncated target size to zero while lock remained held.
- Setting the 4 KiB CRT buffer succeeded.
- The first 7,500-byte buffered `fwrite` returned zero after its first underlying flush was denied; file size stayed zero.
- `fflush` returned 0 with sticky `ferror=1` (the failure had already happened during `fwrite`). `fclose` returned 0; its handle was already closed, so `GetFileSizeEx` on it returned error 6. After unlocking, final file was empty with SHA-256 `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855`.

Raw per-run Win32/CRT values are preserved in `build/cogging-lock-probe-001/raw-results.json`; copied file inputs and the executable probe script are in that same owned scratch directory.

## Test assertion implication

A public Save test can use this existing Cogging fixture with a **shared**, not exclusive, range lock at offset 0. Open can read the input MDT. During Save, the native CRT writer's `CREATE_ALWAYS` reliably truncates it, and the first write fails with zero bytes. Therefore assert the final Cogging.MDT is empty, then assert public Save reaches MST completion and later Boolean BWZ/control INI output. This probe settles the host's Cogging open/truncate/write residue; it is candidate-platform evidence, not original application acceptance.

No converter, original application, GUI, production source, build configuration, or mutable REA database was used. Only copied scratch under the assigned `build/cogging-lock-probe-001` directory was modified.
