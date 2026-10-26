# Checkpoint 71: Immediate Type2 sample sequence review

## Conformance summary

The new `saveType2SampleSequenceImmediate` helper in `src/doe/doe_study.cpp` matches the recovered source order and the ordinary open-failure continuation contract: current selected table first, then fixed modes 0/1/2. It does not call public Save, the XML writer, `DoeTableRefresh`, or DOM replacement. It is suitable as a detached sample-output primitive while the public Type2 arm remains stopped at the unqualified refresh receiver.

The direct mode checks and return states are mostly source-aligned. The current-mode branch has the one known adapter guard difference: it throws for an out-of-range selected mode, where the original `0x1400d75a0` indexes its manager storage without a range check. The fixed-mode calls always pass 0, 1, or 2; native `0x1400d7790` returns false for an invalid explicit mode or invalid-state byte, then the public caller ignores that Boolean and continues.

## Branch-by-branch check

| Sequence entry | Candidate behavior | Native evidence | Assessment |
|---|---|---|---|
| Current `DEF_DOE.PAR` | Reads `samples.mode`; out-of-range uses the documented unsupported adapter guard; invalid selected table yields `SkippedInvalid`; otherwise writes mode/table at the nested `Control/DEF_DOE.PAR` path | `0x1400d75a0` reads selected mode at `+0x40`, then invalid byte at `+0x70+mode`; invalid returns false before opening; valid mode uses matching table at `+0x58+8*mode`, and wrapper `0x140915290` appends `DEF_DOE.PAR` | Predicate and valid-path mapping match. Do not describe the explicit bounds refusal as native behavior. |
| LHS mode 0 | Fixed `mode=0`, invalid byte controls skip; otherwise immediate write to `Samples_LHS.par` | `0x1400d7790`, mode 0 at call `0x140099a77` | Match |
| FF mode 1 | Fixed `mode=1`, independently checks invalid byte; otherwise writes `Samples_FF.par` | `0x1400d7790`, mode 1 at `0x140099d7c` | Match |
| USR mode 2 | Fixed `mode=2`, independently checks invalid byte; otherwise writes `Samples_USR.par` | `0x1400d7790`, mode 2 at `0x14009a081` | Match |

The shared serializer emits the selected mode header even for a valid empty table, followed by one-based row IDs, each value plus a trailing tab, and CRLF. It uses retained rows; it does not regenerate or mutate samples. The immediate helper returns the `StudyIo::writeTextFile` Boolean, whose contract is open success. The `ImmediateSampleWriteState::Opened` name therefore means successful open, not completed/durable bytes. `writeTextFile` records write/close observations separately, and on successful open a later write/close problem does not change the native PAR Boolean. No post-close QFile.error or QTextStream status check should be added to PAR.

For each ordinary false result, the helper still proceeds to the next source-order entry. Invalid flags skip only that table; a failed `DEF_DOE.PAR` open must still be followed by 0, 1 and 2 attempts, and an LHS false must still be followed by FF and USR. A thrown adapter/owner guard is a separate stop and is not established as an original PAR return branch.

## Test review and remaining targeted coverage

`tests/doe/test_doe_sample_immediate.cpp` covers the successful four-file order and values, invalid flags, and fixed-mode open-failure continuation. I added `doe_sample_immediate_current_open_false_still_attempts_all_fixed_modes`: it leaves mode 1 and all tables valid, fails only the `DEF_DOE.PAR` open, then checks the ordered path/mode receipts, successful exact LHS/FF/USR bytes, missing DEF file, and unchanged sample state. This proves the current wrapper's ordinary false does not cancel its fixed-mode successors in the declared `SequenceIo` fixture. It does not establish native partial-byte behavior. The test was added with Testkit macros and has not been built or run.

The scratch `SequenceIo` writes with `std::ofstream` and models only open false/successful open. It does not prove original partial-byte residue. Assertions using its file bytes establish this fixture's simulated file contents only; a successful native PAR open may still leave bytes affected by Qt stream/write failure while returning true. Keep helper status assertions on the open observation, and rely on separate `TextWriteAttempt` diagnostics for the candidate adapter's write/close observations.

## Ownership and integration boundary

This sequence accepts a `const Study&`, a caller-supplied DoeStudies directory and a `StudyIo&`. That is appropriate for an isolated helper, but it does not itself prove that the Study is owned by the currently selected Type2 SUB, that `Study::sub` matches, that `Project::doeIo` is the actual Session sink, or that a callback retained the original manager. At eventual integration, root must supply the already-qualified cached Study/I/O identities from the public Save owner path; do not let this helper resolve `activeSubProject` or construct/rebind a StudyIo.

The public native arm order remains XML -> `DoeTableRefresh` -> current PAR -> fixed modes 0/1/2. The current callsite throws at the refresh receiver boundary before invoking this helper. Keep it detached until the actual table/timer receiver owner is recovered; source-backed PAR behavior is not authorization to deliver a simulated refresh or enable public Type2 Save. Type3's `.OPT` writer is a separate branch.

## Evidence boundary

Native predicates/order/Boolean behavior: `analysis/resume64-doe-save-effects.md`, the four full-body entries and calls in `analysis/resume64-doe-save-effects-pe-receipt.json`, and original PE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Candidate source reviewed: `source/apps/moproj2mst/include/m2m/doe_study.hpp`, `src/doe/doe_study.cpp`, `include/m2m/doe_io.hpp`, `src/doe/io.cpp`, and `tests/doe/test_doe_sample_immediate.cpp`. No source edits, build, test run, GUI, runtime, or mutable REA access occurred during this review.
