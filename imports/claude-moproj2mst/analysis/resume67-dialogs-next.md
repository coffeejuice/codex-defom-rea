# CP67: Dialog-independent Save prefix and unresolved default lifecycle

## Qualified synchronous branch

The retained assembly `analysis/wave3-closure-next-1400980d0.asm` shows a narrow branch that can be modeled without any due-event schedule:

1. Save `0x1400980d0` initializes its answer to Yes (`0x4000`) and calls open check `0x1400a4090`.
2. If that check is false (`project+0x55` not opened), the conditional branch at `0x140098104` jumps directly to the function return path at `0x140098298`, which sets/returns No (`0x10000`).
3. The `processEvents(flags=0)` call at `0x140098129` is after this branch, so it is not reached. Per-subproject validation, last-operation warning, current-operation Save, and output writers are likewise skipped.

This is a source-bound failed-open Save prefix independent of timer readiness. Candidate `tests/s/test_save_answer.cpp` already checks the equivalent `saveCurrent` unopened state: answer No, no validation, and no project/MST writes. That test does not prove the outer native `0x1400980d0` branch or any preceding Close-dialog event delivery.

The branch must remain qualified to an open-check failure. It is not the successful fresh `.moproj` path (process type 5, project+0x55 true) used for the current-operation Save question and last-operation warning. That successful path reaches the dispatcher poll, so its due-event schedule remains unresolved.

## Dialog answer and cleanup limits

Retained static contracts derive the successful native Close question's Yes default only for the shipped Windows-family style and unchanged initial focus. They also bind nested Save Yes-to-Cancel behavior and accepted-Close callback order. No retained capture establishes an actual Save/Close dialog answer or a timer schedule: `analysis/reference-harness-status.json` says runtime groundwork was complete but the original lifecycle had not started; the later `analysis/progress-window-20261023-1503-20261024-0303.json` records an original GUI attempt that crashed immediately after Open, before Save observation. Thus neither constitutes a default-answer capture.

In the warning case, nested Save returns Yes and Close maps exactly Yes to Cancel; the accepted-Close callback/cleanup path is skipped and the project remains attached. For a non-Cancel final answer, static order is display callback `d0e540(0,0,true)` when its receiver is nonnull, then `signalRemoveGraphItems`, then guard success; outer Close proceeds to manager/project teardown only after guards accept. The `d0e540` state/render/notification effects remain partial and require current display bindings. Do not claim them absent or harmless.

The failed-open Save return No should not be silently composed with the successful `.moproj` current-operation Close question. The Close guard selects a different Keyword question when project+0x55 is false; its Yes target has a separate, partially unresolved contract. It still uses a modal loop and may dispatch events, so the Save subroutine's no-pump branch does not prove the entire failed-open Close action was event-free.

## Smallest deterministic scope

The only fully event-independent prefix established here is:

`Save enters → open check false → return No; no Save-entry processEvents, subproject check, warning, current-operation save, or output writer.`

An isolated test may pin this branch in the actual `saveCurrent`/outer Save seam if that outer return remains represented; retain the existing `unopened_save_returns_no_without_reaching_validation_or_writes` case. It should not assert dialog/Close answers, receiver callbacks, or teardown. No new source change is warranted by this read-only audit.

For the selected successful-open default path, no synchronous prefix after the open check is independent of timer state: the original calls the flags-zero dispatcher poll, which may deliver already-due timers. If the last-operation warning then appears, its separate `QDialog::exec` interval is another dispatcher opportunity. A future original capture needs to record both intervals, actual default answer, Save/Close return, and whether accepted cleanup ran.

## Evidence qualifications

Basis: `wave3-closure-next-1400980d0.asm`, `wave3-boolean-timer-contract.json`, `wave3-close-save-return-contract.json`, `wave3-normal-close-guard-contract.json`, `wave3-close-session-integration-audit.json`, `reference-harness-status.json`, and `progress-window-20261023-1503-20261024-0303.json`. Evidence is static except the later Open crash record, which contains no Save/Close dialog observation. No source/build/GUI change was made.
