# CP218 staged Open directory decisions implementation

## Change

The existing pure collector is connected to the `parseOperation` Open parse path. After the `<Type>` branch applies the original zero-ID ordinal fallback and `createOperation` returns, the code calls `collectOperationProjectDirectoryDecisions(projectDir(p), id, ...)` and appends each UTF-16 path to `Ctx::stagedOperationDirectoryDecisions`.

This is staged per-call metadata only. It does not append to `FileEffects::mkdirs`, invoke a filesystem operation, alter an output/public API, or add writer integration. Decisions already appended remain in `Ctx` if a later operation parse fails.

The isolated model test opens a scratch copy derived from the B98 `Gear_blank.moproj` fixture, with a positive operation, a zero ID that falls back to its document ordinal, a repeated positive operation, a negative-ID operation, and a malformed final row. It checks the three five-path groups in order, confirms the negative row contributes none, confirms decisions survive the later parse failure, checks that no mkdir effect was added, and compares the scratch tree before and after Open.

## Source qualifications

- `source/docs/re/moproj2mst/s1-01/spec.md:315-318` records the `id >= 1` directory gate and the order `Operations`, `Objects`, `Materials`, `Equipments`, `Lubricants`.
- The retained disassembly qualification in `analysis/resume218-next-source-increment.md` identifies the original chain `0x140050820 -> 0x140051190 -> 0x14008c630`, including the ordered helper path requests. This does not establish a runtime occurrence or successful filesystem attempts.
- Candidate `parseOperation` is `0x14008ccf0`; it replaces only `id == 0` with the ordinal before `createOperation`. The staged collector call is after that factory returns, so its gate sees the effective ID and only executes for a constructed Type branch.
- `projectDir(p)` in `src/model/project.cpp` returns `cleanPath(absolutePath(p.path))`; this supplies the helper’s expected cleaned absolute root.
- CP216's bounded review (`analysis/resume216-opdir-review.md`) passed the pure collector and its tests, covering the fixed path order, normalization, zero-ID no-op, repeated-call multiplicity, and unchanged scratch-directory entries. This change integrates that helper with parsing; it does not recreate it.
- CP217's negative-ID candidate preflight reported accepted Open and an unchanged tree. That is candidate evidence only. The original runtime creator call remains unwitnessed, so no original causal or filesystem-creation claim is made.

## Validation

The new source test is intended for the existing `tests/*/test_*.cpp` CMake discovery. It was not built or run in this implementation pass; the coordinating task owns the full build and regression. No original process, GUI, or filesystem writer was invoked. No source fixture was edited; the test creates and edits its disposable copy under configured scratch.
