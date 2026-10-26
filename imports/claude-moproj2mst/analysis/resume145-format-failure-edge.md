# CP145 format failure-order edge

## Selected source-closed next case

The smallest useful unimplemented reachable case is count-two `SPRING2.INI` bytes exactly `1X\r\n`, paired with the already supported complete two-row ASCII `SPRING.INI` fixture. Expected flags are `[1, 0]`; both input files remain unchanged. This is a narrow uppercase-suffix sibling of the already modeled lowercase `1x\r\n` case, but it exercises the same important status/cursor order while producing a nonzero first row flag.

The original Qt path is source-qualified from the retained `getInteger` and `operator>>(int&)` captures cited by `analysis/resume120-spring2-malformed-flags.md` and `analysis/resume83-sw2sp-parser-independent-review.md` (QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`). Decimal extraction accumulates the digit `1`; the first non-digit `X` is restored unread and the getter succeeds with 1. Since the companion helper's loop is not yet at end and count is two, it attempts a second extraction at `X`. That extraction has no digits, leaves the same character unread, writes zero to the destination, and sets fresh stream status to the non-EOF error state. The loop index increments to two, so the count condition ends the helper. `FUN_1410da690` ignores that status and returns true after successful open. `analysis/resume120-spring2-malformed-flags.md` directly qualifies the corresponding `1x\r\n` cursor/status mechanism; the uppercase `X` result follows the same ASCII non-digit branch and does not depend on case-folding or locale.

The candidate already reproduces lowercase `1x\r\n` as `[1,0]`, but its suffix handling admits only lowercase `x` in that precise branch. For exact `1X\r\n`, the current `readSpringFlagInt` falls through to the unsupported suffix guard, so this is a concrete candidate gap. A minimal next implementation can admit only the full-byte companion `1X\r\n` with primary count exactly two, returning `[1,0]`; preserve all other uppercase/malformed suffixes and existing filename/count/ASCII gates. Test exact values, byte identity, and refusal of nearby `1X 2`, `1XX\r\n`, one-row, and count-three profiles. Internal sticky status need not become public state; model the returned values and cursor outcome required by this bounded caller.

## Why the remesh failure branch is not the selected cut

`analysis/resume124-remesh-handler-frontier.md` and `analysis/resume116-remesh-invalid-parameter-occurrence.md` source-qualify a null `fopen("wt")` result reaching the first `fprintf`, `EINVAL` assignment, and the conditional CRT invalid-parameter dispatcher path. They do not establish the actual handler slot at that Save, handler/filter replacements, debugger/error-mode state, whether the process filter returns, or the observed process termination status. The report requires a same-run original-process witness over the locked disposable B133 overlay to bind those outcomes. Reusing the candidate's predicted `0xC0000417` or adding another pure projection would not close that runtime-dependent terminal edge. Keep failed-open remesh behavior guarded until that witness is available.

## Scope and qualification

The `1X` case is an ASCII-only static derivation within the original default C-locale QTextStream branch. No ACP or BOM question is involved. No original-process differential was run. The proposed test is a private native reader test, not public Open/Save parity. No source, build, GUI, or mutable REA state changed for this investigation.

Reviewed candidate bindings:

- Reader: `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` — `AD1633CD791DCB541F33BB070741F72B4DF73B54B2BBD706323264832C37BB22`
- Focused reader test: `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` — `EE84FBF08D6BF6BD9E9718434E593E00543A5C696AC7BCF08D6B434122C7F444`
- Exact two-row primary fixture currently in that test uses count 2 and complete distinct rows `(10, 0.5, 1.5, 20, 0, 1, 0)` and `(30, 2.5, 3.5, 40, 1, 0, 1)`.

No source/build action is proposed on the remesh path; root owns any later original-process evidence capture.
