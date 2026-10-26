# Save I/O ordering audit (RIO)

## Reconciliation

This note's original proposed current-operation Boolean fix is superseded by the focused integration result. The incorrect analysis assumed `saved.step2` effects were applied after the native project writer. Current `source/apps/moproj2mst/src/core/convert.cpp` applies `saved.step2` before `saveProjectFileNative()`; preserve that ordering. Do not implement the proposed stage2 reordering based on the old note.

The corrected current-operation Boolean fixture test in `source/apps/moproj2mst/tests/s/test_save_io.cpp` passes on the current production source: `analysis/resume46-saveio-b-build.json` records the focused build and `m2m.s.save_io` 66/66. It seeds the current Boolean's BWZ, denies its open, and confirms the expected INI/MST continuation and retained BWZ. This specific nonfatal BWZ-open behavior is settled for the tested path; the earlier claim that it exposed a stage2 gap was false.

## Qualified original behavior and remaining scope

The original Save entry at `0x1400980d0` calls current-operation Save (`0x140052ec0`), project writer (`0x1400983c0`), property writers (`0x1400982e0`), then MST (`0x1400a20d0`). `analysis/save-semantics.md` records bounded static evidence against a verified original binary hash; the routine proceeds from the project call to later calls without branching on its result. `analysis/wave3-boolean-save-io-contract.json` statically qualifies Boolean BWZ/INI attempt order and continuation. Neither is a fresh original fault-injection capture.

The passing test validates one reconstructed current-operation denied-BWZ case. It does not independently establish native partial-write/flush outcomes or all current-operation writer classes. Continue RIO by selecting a remaining writer-specific failure case supported by evidence, while preserving current source order and treating this passing case as closed. Do not reopen it absent contradictory evidence.
