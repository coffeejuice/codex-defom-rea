# CP71: isolated four-call Type2 PAR sequence

## Implementation

Added `saveType2SampleSequenceImmediate(const Study&, doeStudiesDir, StudyIo&)` to the DOE study helper layer. It retains one const Study reference and performs the source-qualified order:

1. Current selected mode to `Control/DEF_DOE.PAR`.
2. Explicit mode 0 to `Samples_LHS.par`.
3. Explicit mode 1 to `Samples_FF.par`.
4. Explicit mode 2 to `Samples_USR.par`.

Each attempted write calls `saveSampleTableImmediate`, which shares the same byte serializer as the planned-output helper and calls the bound `StudyIo::writeTextFile`. The returned four-entry receipt preserves mode/path and distinguishes `SkippedInvalid`, `OpenFailed`, and `Opened`. It attempts later files after an ordinary false/open failure. It does not mkdir, mutate the Study, dispatch DoeTableRefresh, enter `subProjectElement`, or alter any public Type2 guard.

For the current-mode slot, candidate code validates the mode range before indexing invalid state; original bounded native code shows a direct selected-mode invalid-byte access without a preceding range check. The safer candidate refusal for malformed current mode remains explicit and is not claimed as native equivalence. Fixed mode slots retain the native 0..2 bounds check and per-mode invalid predicate.

## Tests and static preflight

The focused tests use a single Study with selected mode 1 to verify current output precedes modes 0/1/2 and that exact bytes/path agree for all four files. A deterministic StudyIo test double reports open failure for mode-0 LHS while allowing the later mode-2 USR write; current and mode-1 FF are independently skipped because their shared selected mode is invalid. Assertions check the result-state distinctions, attempted-write order, exact successful bytes, and absence of skipped/failed files.

Static preflight confirmed the test support include resolves, all owned files have final newlines and no trailing whitespace, and existing CMake discovers `tests/doe/test_*.cpp` via its configured glob. No build, test, source caller, shared writer, CMake, conversion path, GUI, or mutable REA was changed or run. The failure test is a deterministic adapter simulation, not evidence from original runtime or an OS-level injected QFile fault.

## Owned source/test files and SHA-256

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/doe_study.hpp` | `b390efe0a2b4d004eaa2e716da817706100c4d9769be8e1cee13384d587b9955` |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `0eb68d61a8b0565e8f5ee08b5ec07c9a29425677903ea18c0df83ea62dbbbdff` |
| `source/apps/moproj2mst/tests/doe/test_doe_sample_immediate.cpp` | `6b6633f9c282a91d6caa83cbb8cec55a3c042c1da538fe995eca00b9a8af984a` |

The sequence is an isolated output primitive, not integrated public Save. Root must preserve the refresh boundary and only use this after normal delivery has been independently qualified, while keeping the four file effects ordered and visible in the public Save result.
