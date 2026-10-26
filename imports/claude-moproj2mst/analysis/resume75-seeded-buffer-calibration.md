# CP75 seeded-buffer physical error calibration revision

## Preserved first calibration failure

The first test revision attempted to arm the byte-range lock only after the serializer had called the private output adapter with an eligible small write. The retained full CTest log is `build/candidate/Testing/Temporary/LastTest.log`, SHA-256 `E21A8CF969F8980A4F3E5C9021D7EAF8C4C4CC0A4F0E9A799445FCAA06CEA8B2`. Its `m2m.moproj.project_output_physical_lock` entry records that the lock was never requested/acquired, no small write was seen, the actual error stayed zero, and the writer succeeded. The old test failed for exactly that reason. That failure is a useful calibration observation: the unmodified project serialization did not enter the adapter's proposed small-write timing point.

The original log remains intact in place. Its failure output includes the old case name `project_output_physical_lock_after_buffered_write_preserves_postclose_file_error` and failed expectations at the test source lines recorded in the log. It is not reinterpreted as a physical writer failure.

## Revised bounded experiment

Revised the dedicated test to seed the real FileDevice buffer explicitly in the factory-created output adapter:

1. The private factory runs at project-output open, after the input project has been parsed.
2. It constructs the actual FileDevice, writes one byte (`"X"`) through its normal `write` path, and records that the write returned 1 while the physical output length remained zero.
3. It opens a second shared-access handle and takes an exclusive one-byte lock at offset zero. The lock remains held across the unchanged project serializer, actual `aboutToClose`, FileDevice close, and the one post-close error query.
4. No scripted status override, output splitting, early flush/callback, or source-interface modification is used. The extra pending byte is an explicitly artificial initial FileDevice state; the native serializer did not produce it.

When the lock was successfully acquired, the test requires the handle to remain locked at close and error query, the real unmodified FileDevice error to be 2, and the project writer's returned Boolean to be false. It independently records serializer device-write attempts/rejections, but the private seam does not expose `mst::Stream::writeFailed()`, so no QTextStream-status claim is made. After the lock releases, the test records that the output remains a regular file and its observed byte count; it does not assert an invented partial prefix or expected residue bytes.

If the seeded write or lock setup does not qualify, the test emits a `[ diagnostic ]` line with the Win32 setup error, writer result, and observed post-close FileDevice error, then returns **without asserting** the physical failure contract. This keeps an unarmed setup from being represented as physical-error coverage. Root should classify that logged branch as a gap, even though the diagnostic-only path exits the test case normally.

## Scope and static preflight

The test remains one standalone test case in the existing `m2m.moproj.project_output_physical_lock` glob target. `git diff --check` emitted no issues; the private header resolves; source inspection found one case, one Testkit main, no raw C `assert`, and no trailing whitespace. Revised test SHA-256: `6A6ECFFDAE42DB318AE4B59718F3DC7E537B88C7391DE015C989587B756C5D28` (275 lines).

No build or test run was performed after this revision. The seeded case can establish a physical FileDevice post-close error only under its explicit injected initial-buffer state. It does not establish that unchanged public conversion or the original Save naturally reaches the same pending-buffer state, and it is not original-runtime parity. Production code, shared interfaces, CMake, GUI, installed application, and mutable REA state were untouched.

Exclusive ownership of `tests/moproj/test_project_output_physical_lock.cpp` is released to root. Root should rebuild/run this focused target and retain the diagnostic output if lock setup is not acquired.
