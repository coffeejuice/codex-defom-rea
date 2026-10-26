# Resume 47 native project writer failure audit

## Result

Native MOPROJ open-failure continuation is already covered at two levels. `save_project_open_failure_preserves_project_but_continues_boolean_and_mst` denies the project open, checks the project bytes are retained, verifies Boolean BWZ/INI and MST later stages, and checks the adapter returns `M2M_E_WRITE`. `project_native_open_failure_precedes_dom_mutation` separately confirms the native writer does not mutate its XML DOM before a failed open. `save_project_direct_write_does_not_require_delete_sharing` covers successful direct output with a compatible reader. These close open-failure behavior and successful open/write behavior; they do not cover a failure after project open.

Original static contract `analysis/wave3-project-close-status-contract.json` (verified original DEF_GUI_MO_64.exe hash `fd8ae1f8...725acc`, writer `0x1400983c0`) says the project writer opens QFile, serializes through QTextStream, closes QFile, then returns false iff `QFile.error()` is nonzero. It does not query QTextStream status. The same contract records that QFile close may clear an error if the initial QFile buffer flush succeeded, even when a later `aboutToClose` text flush fails; the opposite case preserves the error. It explicitly qualifies concrete partial-byte outcomes as untested against the original. Top-level Save continues property and MST stages after project writer false.

The reconstructed `saveProjectFileNative()` has the corresponding open gate, `saveProjectFileBody(..., &file)` close, then `file.error()` query. `FileDevice` lower-level tests cover blocked writes, sticky stream failure, close retry/error reset, and error-after-close (`tests/mdt/test_file_device.cpp`, `analysis/wave3-file-device-error-contract.json`). Existing Save I/O tests do not combine a post-open MOPROJ write/flush failure with top-level continuation and observed target bytes.

## Exact missing assertion

Add a Save I/O integration case that lets the native project writer open/truncate the target, then induces a byte-range write failure during serialization. Assert together:

- project output is the exact retained prefix (or, if the selected lock point yields no successful bytes, the exact retained target state) expected from a successful disposable control serialization;
- the public adapter result follows the post-close `QFile.error()` contract;
- later Boolean sidefiles and MST still complete despite the project writer returning false.

A candidate Windows setup is an exclusive byte-range lock on the project target while sharing read/write, chosen so serialization emits at least one prefix before a subsequent write overlaps the lock. Compare the failed output to a separately generated successful control file in disposable scratch to avoid guessing XML bytes. The precise lock offset/chunk boundary must be demonstrated by the test itself; do not assume a fixed prefix from the 16 KiB QTextStream threshold alone, since FileDevice also has its own buffering and direct-write branch. If stable prefix injection is not achieved, capture and assert a deterministic post-open failure state without weakening continuation assertions.

## Evidence limits

The original project-writer failure behavior remains static-only; no original process or injected write fault was used. Candidate FileDevice tests establish emulated device behavior, not native application outcome. A successful project open/write test is not a substitute for the missing project-writer integration failure assertion.
