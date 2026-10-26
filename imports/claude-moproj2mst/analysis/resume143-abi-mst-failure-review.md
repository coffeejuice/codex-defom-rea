# CP143 ABI denied-MST-open test review

## Finding: generated DEFORM paths violate the short-path rule

The test at `source/apps/moproj2mst/tests/g0/test_abi_write_save_boolean_mst_open_failure.cpp:85-92` copies `133_B_DBSTEP_BOOL3_before` beneath `m2m_test::scratchRoot()` and does not check path length. The existing `build/candidate/CMakeCache.txt` binds that root to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/build/candidate/t`. A representative resulting `.moproj` path with the test's leaf naming is 137 characters, above the project's `<100` limit, and it is not under `C:\PROBLEM`. This is a concrete setup blocker under the standing DEFORM fixture/path rule; it may also turn the intended Save failure test into a long-path test. The test should not be accepted until its copied project, MST, and generated side-file paths use a compliant short runtime root and are length-checked. Note that current `CMakeLists.txt` constrains `M2M_TEST_SCRATCH` to a child of the build root, so simply setting that cache variable to `C:\PROBLEM` may be rejected; integration needs a supported short-path arrangement rather than bypassing the containment guard.

## Failure injection and effects otherwise check out

The Windows sharing injection is correctly formed. `DenyWriteOpen` opens the already-existing disposable MST with `GENERIC_READ` and `FILE_SHARE_READ`, retaining the handle across `m2m_convert_w`. That permits a second read handle for `readBytes()` but does not permit another opener to request `GENERIC_WRITE`. `mst::FileDevice` requests `GENERIC_WRITE` with `FILE_SHARE_READ | FILE_SHARE_WRITE`, so the extant handle's missing `FILE_SHARE_WRITE` makes the target `OPEN_ALWAYS` fail before truncation. The candidate maps that failed open to `M2M_E_WRITE` and prefix `cannot open MST ` in `src/core/convert.cpp`; the ABI forwards the status/message. The test's exact assertions are consistent with that order.

All conversion writes are directed to the copied fixture: the project path and null output path are both rooted in `root`, and the null destination resolves to `<project-dir>/COG_P1_B1.MST`, the locked copy. The retained Oracle-B source is only read by `fs::copy`; the test does not acquire a handle to it for writing or launch/control the original app. The asserted Boolean side files are under copied `Operations/Task00002`; the test checks their content markers, the MST's unchanged pre-call bytes while sharing-denied, a changed copied `.moproj`, and CWD preservation. RAII releases the lock when the scope exits, and scratch is intentionally retained for diagnosis.

The fixture identity and claimed effects match the adjacent internal test plan: source `.moproj` SHA-256 `8e5e896cce67164430324cddc25324043449428b789d201bb3bef9c49cc89f79`, source MST SHA-256 `07b0a85d88c31326ecd1f069d765fdc14e8f5efdc046195ed301eb014e20eb1c`; test source SHA-256 at review time `00BBC31991AADA8BE0F488BEB24F5AAEE1BBF3BA31600CD83F18AE83A8F9FA0B`. No test/build was run.

## Claim boundary

The effect assertions can establish candidate exported-ABI ordering and error mapping for this copied project once path isolation is fixed. They do not establish original GUI result/status or a fresh original failed-open comparison. Keep `analysis/resume143-acceptance-next.md`'s original-runtime qualification unchanged.
