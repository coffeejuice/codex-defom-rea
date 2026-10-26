# CP73: paired acceptance design for project post-close failure

## Purpose and prerequisite

The test should prove the continuation contract at the public C++ `convertDetailed` boundary when the native project writer returns false **after an opened write and Close**, not just when the project file cannot be opened. The private core service needed to inject/observe that final QFile error is not present in the inspected `convert_details.hpp`/`convert.cpp` API. This is a design for when that service exists; it adds no source or test code now.

Use two conversions from byte-identical disposable copies of one qualified Boolean Save fixture. Keep all source inputs read-only and compare manifests before and after. The control and injected runs must each create a fresh conversion/session; do not reuse a live Project or Session between calls.

## Pair and assertions

| Check | Control: actual writer success | Injected: post-close status override |
|---|---|---|
| `Result.code` | `M2M_OK`, provided all existing guards pass | `M2M_E_WRITE`, with project-writer failure selected after MST and remesh return normally |
| `details.returnedCode` | Present and exactly `Result.code` | Present and exactly `Result.code` |
| `details.saveCall` | Present because real Save was entered | Present because real Save was entered |
| Project writer flags | `projectWriteAttempted`, `projectWriteReturned`, and `projectWriteSucceeded` all true | Attempted and Returned true; Succeeded false. A normal returned false is not an exception/interruption. |
| Step 4 Boolean outputs | Task00002 Boolean.BWZ and DEF_MACHDIST_CTRL2.INI contain the fixture-qualified records | Same two outputs are present, with content equal to the control run for the selected fixture/profile |
| MST | Attempted/Returned/Succeeded true; generated MST contains the fixture-qualified completion marker | Same successful MST output as control; the injected project status must not skip MST generation |
| Remesh tail | Reached after MST and completes under the selected qualified input | Reached after MST and completes before project failure is mapped; selected remesh output/effect matches the control's qualified effect |
| Status message | No failure message | Project-write error, not an MST-write error; avoid exact localized text unless the API contract fixes it |

The fixture must be selected so its native Save does not stop earlier at a DOE or other unsupported guard and so the remesh path has a known successful result. If its remesh state is the default no-output branch, a passing call alone cannot prove the tail ran. In that case, add an observation-only counter/receipt at the private service boundary or use a source-qualified disposable fixture that emits a deterministic remesh artifact. Do not infer remesh execution merely from the final `M2M_E_WRITE`.

For both calls, compare Boolean and MST contents directly rather than only checking file existence. Compare project output only as a measured candidate effect; do not assert an invented partial prefix. If the injected service physically writes the complete project and then scripts a nonzero final QFile status, its bytes may equal control while the status differs; label that as simulated status injection, not a physical I/O failure. If the service prevents or truncates project output, assert only behavior directly guaranteed by that service contract and record the actual manifest.

## Private service contract required

The seam should be private to the core/native-project writer and must preserve public behavior when disabled. For the injected profile it should:

1. Allow the real project QFile open, serialization, and Close path to run.
2. Return a controlled nonzero post-close QFile error at the same decision point where the native writer samples `QFile.error()`.
3. Mark the test result as a scripted observation so logs cannot be mistaken for a real device fault.
4. Leave Boolean step 4, MST writing, and the remesh tail outside the injection branch and in their ordinary order.
5. Avoid a production public C ABI change or a transaction/rollback around earlier effects.

This verifies caller ordering and status mapping. It does **not** prove the OS can produce that exact physical QFile state under a file lock.

## Separate physical-fault validation

Retained `analysis/wave3-project-close-status-contract.json` binds the original project writer `0x1400983c0` and QtCore close/error sequence. It establishes that the original writer samples `QFile.error()` after Close and does not use `QTextStream.status()`. It also explicitly says concrete OS partial-byte outcomes and error resetting are not established by original runtime evidence.

The existing candidate test `project_native_locked_direct_write_uses_post_close_qfile_error` does not produce the required persistent error: its initial close-time QFile buffer flush succeeds, clearing the file error, and the writer returns true. Lower-level FileDevice tests exercise saved initial flush state but do not prove full public continuation. Therefore keep two evidence labels separate:

- **Scripted post-close status:** paired detailed-convert test above; proves candidate orchestration and `M2M_E_WRITE` mapping.
- **Physical post-close QFile failure:** requires a calibrated real device/lock condition and direct observation that `errorAfterClose != 0` after the initial close flush. It must be tested separately before claiming physical failure coverage. Do not claim original runtime parity without a fresh original comparison.

## Source and existing-test basis

`source/apps/moproj2mst/src/core/convert.cpp` currently records project Attempted before `saveProjectFileNative`, Returned after its normal return, runs the operation-property loop before MST, then runs MST and `saveRemeshAtProjectSave` before mapping a false MST or project result to `M2M_E_WRITE`. Boolean sidefiles are explicitly dispatched in the step-4 operation loop before MST. `ConversionDetails` exposes `SaveCallState` by value and the returned Result code, but no remesh effect or live writer/device object.

Existing tests prove narrower behaviors: public MST-open failure with earlier Boolean effects (`save_failed_mst_open_keeps_step4_boolean_files_and_prior_save_effects`); actual native project open failure with continuation; a direct project writer that returns true after close-time recovery; and lower-level FileDevice/QFile state distinctions. None is the proposed public paired post-close false project-writer test.

The original ordering qualification comes from `analysis/save-semantics.md` and `analysis/wave3-project-close-status-contract.json`; neither is a fresh original runtime fault injection. Root should add the private seam and then request a source-bound build/test receipt before treating the paired case as candidate coverage. No source, test, build, GUI, or mutable REA change was made in this design lane.
