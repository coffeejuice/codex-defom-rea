# CP75 physical FileDevice error calibration test

## Added test

Added one standalone headless test at `source/apps/moproj2mst/tests/moproj/test_project_output_physical_lock.cpp`. It uses the existing private `ProjectOutputFactory`/`ProjectOutputDevice` seam and a real `mst::FileDevice`, with fresh copies of `133_B_DBSTEP_BOOL3_before` under `M2M_TEST_SCRATCH`.

The paired control delegates the actual FileDevice status without a lock. The calibration run waits until the actual output device has accepted a positive write no larger than the candidate's `0x4000` byte buffer block, then opens a second shared-access handle and takes an exclusive Windows byte-range lock over the output's initial range. The range lock remains held through the real `aboutToClose`, FileDevice handle close, and the one post-close `error()` query. No status is scripted and no bytes are split or rewritten by the adapter.

Assertions separate the observations:

- Control requires actual post-close FileDevice error zero and writer success.
- Locked run requires lock acquisition after the buffered write, lock still held at close callback and error query, actual FileDevice error nonzero, and native project writer false.
- Both runs require the ordinary open/close/callback/error-query order and capture the disposable output as a regular file plus its observed byte count after close and lock release. The test does not assume a partial prefix, empty output, or control-equal bytes.
- Device write rejection count is recorded separately. The existing private project-output surface does not expose QTextStream's sticky status, so the test does not claim to observe or equate that status with QFile.error.

## Static preflight and limits

Static preflight only: both quoted includes resolve; the new file has one `TEST_CASE`, one Testkit `main`, no raw C `assert`, no trailing whitespace, and `git diff --check` emitted no issues. New test SHA-256: `D3FE1C17A6E23E9A9DFC48C9FCB6DC1B363948CF370A1A8DA2328ACE1404DE4C` (269 lines).

No compilation, CTest, executable run, GUI, installed-app, source-production, shared interface, or mutable REA change was made. The assertion that this exact project serialization produces a persistent locked flush error remains unvalidated until root builds and runs the new target. If it fails to arm the lock or reports real error zero, retain that result as calibration evidence; do not replace it with the CP73 scripted-status case. This is candidate Windows file-device evidence only, not original runtime acceptance.

## Ownership release

Exclusive ownership of `tests/moproj/test_project_output_physical_lock.cpp` is released to root for integration/build review. No CMake change is included; the existing recursive test glob should discover the new test source after configure.
