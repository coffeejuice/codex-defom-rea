# CP117 Close busy phase source review

## Review result

No blocker found in the frozen phase-aware decision extension. `CloseMainBusyPhase::Unknown` and invalid enum values return `NeedsMainPhase` before examining supplied values (`src/engine/close_main_busy_decision.cpp:19-32`). The base-construction case returns `NotBusy` without consuming either optional input, matching the retained constant-false `ca44f0` leaf and its lack of widget/mode reads. The derived case delegates directly to the prior overload (`:28-30`), preserving its index-first read boundary: absent current index returns `NeedsCurrentIndex`, index 2 short-circuits to Busy, and only other indexes require mode+270.

The overload preserves the old API. The original two-argument function and its default mode argument are unchanged; the new phase overload requires all three arguments, so legacy `evaluateCloseMainBusyDecision({})` remains unambiguous. `NeedsMainPhase` is appended after prior result values. Tests exercise Unknown with and without conflicting values, invalid phase, base with absent/conflicting widget values, derived order after a base projection, and legacy empty-brace compatibility (`tests/mdt/test_close_main_busy_decision.cpp:12-15,77-116`).

There is no Close/public lifecycle widening: `rg` found call sites only in the decision implementation and its pure unit test, not in Close dispatch/session code. The helper stores no phase or identity; a caller can supply a phase as an explicit mathematical input, but it is not a live construction witness. The implementation receipt states the same limitation. No constructor, `c99530`, application current-main publication, manager, stack producer, Close callsite, or accepted-close effects were added.

## Evidence and validation limits

`analysis/resume117-close-canonical-main-construction.md` source-qualifies the base/derived vtable split and explains why `c99530` alone cannot identify the base phase. `analysis/resume117-close-busy-phase-implementation.md` binds the change to exact header/source/test hashes and reports a static-only preflight. Root reports the CP117 grouped full suite passed 306/306 with stable source; I did not independently build or run tests. The remaining original event-registration/selector witness, real QApplication/main constructor producer, and actual fresh W/stack lifecycle remain open, so this helper does not establish an observed Close path or public Close acceptance.

Read-only review only: no source edits, build/test execution, GUI control, original launch, or mutable REA operation.
