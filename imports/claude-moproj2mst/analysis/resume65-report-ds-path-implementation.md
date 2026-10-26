# CP65 — pure Report `.DS` path helper

## Change

Added `m2m::reportDsPath(projectPath)` as a pure path derivation helper in `include/m2m/report_ds_path.hpp` and `src/ops/report_ds_path.cpp`. It applies the retained native sequence: QFileInfo absolute parent path, native separator conversion, QFileInfo `baseName` (the portion before the first filename dot), cleanPath to that base, then sibling separator and `.DS` suffix. It owns no document, performs no file operation and has no Report Save/Open/Close dispatch.

Added Release-active cases in `tests/s/test_report_ds_path.cpp` for a normal project path, a multi-dot project filename, and a dotted parent directory. The test is registered by the existing packet test glob; no shared CMake edit is needed.

## Boundary and verification

The helper is independently source-qualified by the retained `FUN_1412aeb30` disassembly/path trace and `analysis/resume52-report-ds-owner.md`. It uses only the `Project.path`-equivalent input and Qt path functions already ported. The helper does not invoke or model `SP+0x30`, constructor `0x140643100`, viewer helper `0x140d0d6f0`, Report vt+0x68, or any destructor/Close effect. Public Report guards remain unchanged.

No build or tests were run, per assignment; root will validate. No GUI, runtime or mutable REA action was performed.
