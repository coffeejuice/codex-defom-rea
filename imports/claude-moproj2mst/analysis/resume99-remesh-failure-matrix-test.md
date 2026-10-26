# CP99: disabled-remesh cleanup failure with earlier Save failures

Added one focused public lifecycle test in `source/apps/moproj2mst/tests/s/test_save_remesh_failure_matrix.cpp`. It uses the existing private `ConversionCallServices` project-output factory and real disposable `FileDevice` delegation, so no shared production or test-support edit was needed.

The test copies `133_B_DBSTEP_BOOL3_before` beneath `M2M_TEST_SCRATCH`, seeds an unmarked `LOCAL_REMESH.DAT` (`DONE\n`) and holds a handle that allows reads/writes but denies delete sharing. That preserves the disabled-state input while making the eventual `DeleteFileW` fail. The project's `aboutToClose` callback runs only after converter Open and Save-current; it seeds the MST sentinel and arms a write-open denial. The project output adapter separately declares error 7 after its real close.

Assertions cover the observed project open/close/error-query sequence; SaveCallState records both project and MST failures; the Boolean BWZ/INI effects still exist; the MST sentinel and remesh sentinel remain; and the returned adapter status is `M2M_E_WRITE` for the MST path. This distinguishes the scripted project error, actual MST denial, ignored disabled-remesh removal failure, and candidate adapter result. It does not claim `M2M_E_WRITE` as the original GUI Save status: the original receipt establishes source order and ignored `QFile::remove` result, while its earlier check/status local remains separate. The test does not exercise editor300 decimal overflow.

Preflight: standalone Testkit entry point present; includes resolve relative to the test/source/target include dirs; only supported `REQUIRE`, `CHECK`, and `CHECK_EQ` macros are used; CMake's existing `tests/s/test_*.cpp` glob will discover it after configure. No build or test run was performed; root owns validation. No GUI, original execution, or mutable REA action occurred.

Test source SHA-256: `C45745562A68E98A61F08B0FB9E727C393738EF644FAC5A6C375564095C11878`.
