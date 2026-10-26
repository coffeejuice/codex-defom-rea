# CP142 public ABI Save sequence implementation receipt

## Implemented test

Added only `source/apps/moproj2mst/tests/g0/test_abi_write_save_sequence.cpp` plus this receipt. The file is discovered by the existing `tests/*/test_*.cpp` CMake glob; the `^test_abi` target branch links it to the exported `moproj2mst` DLL and `testkit`. No production source, CMake, retained fixture, build, GUI, or mutable REA state was changed.

The test copies `M2M_ORACLE_B/RS_08_HEAT1_before` into a unique path below `M2M_TEST_SCRATCH`, sets and later restores `M2M_MO_INPUT_MODE=0`, selects codepage 1252 and GUI unit 0, then calls `m2m_convert_w(project, nullptr, M2M_FLAG_WRITE_SAVE, ...)`. Null output uses the documented `<project folder>/<project base>.MST` route, matching `tests/s/test_save_folder.cpp::runCase`'s empty-output call. It requires exact `M2M_OK` and unchanged CWD.

The folder check uses the existing comparator's rules: compare case-insensitive relative paths and bytes; omit `.png`, `.oracleA`, and `view_state.json`; relocate expected-root path spellings in file contents; tolerate only DEFORM-banner-only lines and the `.moproj` cycle/group `Expanded` byte. Raw byte-different paths are printed before applying those comparator qualifications. The expected candidate outcome is zero remaining differing, missing, and extra comparable paths, matching CP140's `A:RS_08_HEAT1` row (SAME, 0/0/0, no adjustment).

After success, the same test process creates a separate disposable `B` directory with no `B.moproj` and `LOC_DEN_CTRL.DAT` bytes `FILTER_DISTANCE 19\nDONE\n`. It requires exact `M2M_E_PARSE`, the established `reading project` message, no `B.MST`, unchanged remesh bytes and CWD, and an unchanged snapshot of the first successful output tree. The missing-project/remesh assertions reuse the contract already exercised by `tests/g0/test_abi.cpp::sequential_public_calls_keep_single_project_failure_state_isolated`; the sequence adds the missing success-then-failure ordering.

## Hash binding

- New test SHA-256: `acb1ff092efc3a529cbcc6c7004e8aabd5afb1bd56119b2db6e7dfdb64b0e21c`.
- Retained input `RS_08_HEAT1_before/HEAT1.moproj`: `e758aa107b6d8c849b1417f9f12177f70b282a4ecd6fa96d1231f6f748e30093`.
- Retained output `RS_08_HEAT1/HEAT1.MST`: `4245304b4cb3941b1a8ff10d21a714d1290a696adf2834f0626ff5ae5896eec7`.
- Comparator source `tests/s/test_save_folder.cpp`: `dd44cc93dce3ebAbf12dbb9ec51ef6c54efda07b65377694c7a0861c1bd97ac0`.
- Conversion source used to confirm default output path `src/core/convert.cpp`: `71fe67b9c6be7bde83001e663aa7c50f01c4a6421f357c0db893a5df0a0bd632`.
- CP140 frozen candidate DLL SHA-256: `17ad2635f4dde3fbe0fdd56c700117a6278a93b82d5719a3062269b4f109f8f5`; that prior binary has not been rebuilt to include this test.

## Static preflight and limit

Static review confirms the test stays within configured scratch for all writes and reads retained oracle folders without mutation. It also restores the process input-mode environment variable through RAII and refuses to reuse an existing scratch leaf. No test/build was run as requested; Release-active runtime behavior and compiler validation remain for root's next build.

A future pass result is candidate exported-ABI status/effects versus a retained oracle and same-process state isolation for this pair. It is not a fresh original run, does not verify GUI or native Close behavior, and does not resolve the remaining public Forming editor300, DOE, Project, or Close guards.

