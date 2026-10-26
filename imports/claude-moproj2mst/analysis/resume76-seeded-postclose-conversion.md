# CP76: seeded-buffer physical error through conversion continuation

## Added test and path

Added only `source/apps/moproj2mst/tests/s/test_physical_postclose.cpp`. The case uses `detail::convertDetailedWithServices` and its existing borrowed, per-call project-output factory. It does not alter production interfaces, public C ABI routing, CMake, or ordinary converter settings.

The paired control/injected calls each start from a fresh disposable copy of `133_B_DBSTEP_BOOL3_before` under `M2M_TEST_SCRATCH`, with a local `LOC_DEN_CTRL.DAT` selecting the already-qualified remesh output. The control delegates the real FileDevice status. In the injected case, the test-only output adapter constructs the real FileDevice after project input Open, writes one explicit byte to its internal buffer, verifies the disk file remains zero bytes, and takes a real exclusive Windows lock on byte zero before returning the device to the unchanged serializer. The lock remains held through Save's `aboutToClose`, actual device close, and actual post-close error query. This initial byte is an artificial buffer seed; it is not attributed to the native serializer.

## Assertions when the lock is acquired

The control requires actual post-close file error zero, successful project-write flags, `M2M_OK`, and the expected Boolean, MST, and remesh outputs.

The injected run requires that the lock was acquired and held through close/query, actual post-close FileDevice error `2`, a normally returned false project-writer result, and later outputs absent at the error-query boundary. It then checks the detailed-conversion result and flags: project write Attempted/Returned true and Succeeded false; MST Attempted/Returned/Succeeded true; final adapter code and `returnedCode` both `M2M_E_WRITE` with a project-write diagnostic. Boolean BWZ/INI, MST, and remesh bytes must match the control. The final project residue is measured only by retaining its bytes and size after lock cleanup; no prefix or fixed residue is predicted.

The adapter also records serializer device-write calls and rejected writes separately. It cannot read the private `mst::Stream::writeFailed()` state, so the test makes no direct QTextStream-status claim. If the seed or lock setup does not qualify, it prints a diagnostic with the Win32 setup error, returned code, and observed actual FileDevice error, then does not assert a physical-error result or downstream continuation. Root should classify that path as unqualified diagnostic evidence, regardless of CTest's process exit.

## Preflight and limits

Static preflight only: all three quoted includes resolve; one test case and one Testkit main; no raw C `assert`; no trailing whitespace; `git diff --check` is clean. Test SHA-256: `09A5645B2B065A7449ED9FAA4E938277F4D94B7F2411B2DA0F1B0DB7119C749A` (355 lines).

No build or executable test was run. The test is a candidate private-service conversion test with an artificial initial pending buffer. It is not evidence that unchanged public conversion or the original Save naturally reaches that initial state, and it makes no original-runtime parity claim. The failed earlier no-buffer calibration remains preserved at `build/candidate/Testing/Temporary/LastTest.log` and is summarized in `analysis/resume75-seeded-buffer-calibration.md`.

Exclusive ownership of the new test file is released to root. The CMake recursive `tests/s/test_*.cpp` glob should discover it on configure; root owns focused build/run validation.
