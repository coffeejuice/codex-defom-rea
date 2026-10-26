# CP74 public remesh-open failure test

Added `save_project_postclose_error_then_remesh_open_failure_keeps_prior_outputs` to `tests/s/test_save_io.cpp`.

The disposable `Rig` supplies the enabled remesh state through `LOC_DEN_CTRL.DAT`. The test's project-output wrapper invokes a callback at `aboutToClose`: only after public conversion has opened and serialized the project does the callback create `LOCAL_REMESH.DAT` with sentinel bytes and acquire a Windows handle sharing reads but denying writes. This avoids letting the remesh lock interfere with project/input Open. A declared error 7 is then returned from the wrapper's post-close QFile error query; it is explicitly a scripted project-writer status, not physical project I/O failure or fresh original parity.

Assertions cover the partial-effect order: the project writer returned false, Boolean BWZ/INI and MST completed, the later remesh write-open did not alter the pre-existing sentinel file, and the adapter reports `M2M_E_UNSUPPORTED` from the remesh boundary before mapping the earlier project false to `M2M_E_WRITE`.

Static preflight only; no build or test run was performed. The existing isolated test `remesh_native_failed_write_open_preserves_file_and_marks_invalid_parameter_boundary` uses the same `GENERIC_READ` / `FILE_SHARE_READ` lock strategy and verifies that enabled remesh open failure preserves existing bytes. The public conversion callback timing and status precedence are visible in `src/core/convert.cpp` and `tests/s/test_save_io.cpp`. Root should build/run the focused Save I/O target before treating this as validated.

Owned test file SHA-256: `69383EDEA8D8E159AC0C4D8AE7FD940C29FED209A7BA8D74DBFD16252CE8AA28`.
