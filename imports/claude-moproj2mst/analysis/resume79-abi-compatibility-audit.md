# CP79: C ABI compatibility and reproducibility audit

## Result

The inspected source preserves the declared C ABI shape: `M2M_API_VERSION` remains 1, and `moproj2mst.h` declares six `extern "C"` exports: `m2m_api_version`, `m2m_set_codepage`, `m2m_set_gui_unit`, `m2m_set_deform_dir_w`, `m2m_convert_w`, and `m2m_convert_utf8`. `src/dll/abi.cpp` defines those same six with matching signatures. No `M2M_API`/`__declspec(dllexport)` annotation appears on the new internal C++ detail service. `m2m_core` is static-linked into the DLL; `test_abi*.cpp` link the DLL, while non-ABI C++ test files link `m2m_core` (CMake glob logic in `source/apps/moproj2mst/CMakeLists.txt`). This keeps `convertDetailed` out of the DLL export surface by design.

The review is source-qualified, not a binary ABI proof. Checkpoint75–78 receipts record successful configure/build steps and candidate artifact hashes, but the CP78 grouped receipt only focused-preflights `m2m_mdt_session_model_manager_read`, `m2m_s_save_current`, and `m2m_p01_walk`; it does not run or focused-preflight the ABI executables. A successful full build means they compile/link, not that their runtime assertions passed. The receipts contain DLL hashes but no `dumpbin /exports` snapshot, prior-vs-current export diff, or import-client compatibility run. Do not label the ABI as binary-verified from these green build statuses.

## Relevant behavior and coverage

- The public C ABI remains in `include/moproj2mst.h`; `include/m2m/convert_details.hpp` adds a C++ `ConversionDetails` observation surface and `convertDetailed`, implemented in core. It does not change a C declaration, export macro, flag value, or API version.
- `test_abi.cpp` covers version, invalid/null path and flag arguments, message truncation, setters, and two sequential calls on distinct disposable projects. Its sequential case is specifically two WRITE_SAVE parse failures: it checks each call returns `M2M_E_PARSE`, has a project-reading diagnostic, makes no MST, and leaves each distinct remesh input unchanged. The test explicitly declines to claim process-global reset. It is useful repeated-call failure-state isolation, but does not prove repeated successful calls or outputs do not cross-contaminate.
- `test_abi_open_failure.cpp` covers the physical project output failure seam. `test_abi_original_settings.cpp` exercises adapter settings. `test_detailed_outcome.cpp` tests the internal C++ detail reset/bind contract, not an exported ABI. CP75/76 physical FileDevice tests are calibration/private service evidence; they do not add C ABI exports or prove original GUI status.
- The header-level prose says “No global state except the code page set with `m2m_set_codepage()`,” while the same header declares `m2m_set_gui_unit()` and `m2m_set_deform_dir_w()`, and the adapter also snapshots `M2M_MO_INPUT_MODE` per call. This wording is stale/misleading documentation, not an observed signature incompatibility. It should be corrected when the public header next changes.

## Exact final ABI check

Before calling ABI compatibility verified, perform one source-stable check against the final candidate:

1. Hash/freeze the final `moproj2mst.h`, `src/dll/abi.cpp`, `src/core/convert.cpp`, and all `tests/g0/test_abi*.cpp`; retain the source-tree hash and the candidate DLL hash.
2. Run focused Release CTest targets `m2m.g0.abi`, `m2m.g0.abi_open_failure`, and `m2m.g0.abi_original_settings` (plus `m2m.g0.detailed_outcome` separately as an internal C++ contract). Preserve full test logs and exit codes; do not infer a run from a build artifact receipt.
3. Run `dumpbin /nologo /exports` on the exact DLL whose hash is recorded. Compare names, ordinals, and public signatures/undecorated names to the prior frozen DLL/API receipt. Require the same six names and no accidental C++ helper export. If a prior binary export receipt is unavailable, record a new baseline explicitly rather than claiming a historical diff.
4. Compile a minimal external C consumer against only `moproj2mst.h` and link/call `m2m_api_version` plus a conversion entrypoint; this catches accidental C++-only header or calling-convention drift that internal tests may miss.

The immediate regression gap is repeated **successful** public calls using two independently disposable, source-qualified success projects with distinguishable expected MST/output bytes and isolated side-effect manifests. Existing sequential C ABI coverage is failure-path only. Because the retained original GUI access audit found no established supported headless route, such a candidate test can qualify adapter isolation only; it cannot substitute for original-runtime parity.
