# CP59: Close modal Yes-to-Cancel order

## Static result for the selected project default

For a successful native `.moproj` Open, retained source evidence selects process type 5 and project+0x55 true. Under the source-qualified shipped Windows-family style and unchanged initial-focus conditions, the normal current-operation Close question's static default is Yes (`0x4000`). Its modal `QDialog::exec` can dispatch current-thread events before returning; no historical capture supplies the timer IDs/order actually delivered for this interval.

The nested Save return is an integer independent of output success. In the must-be-last warning case, the warning offers only Yes and statically returns Yes; `0x1400980d0` exits before current-operation Save and file writers. Close replaces its current answer with nested Save's return and remaps exactly Yes to Cancel (`0x400000`), so the project remains attached and accepted-Close callbacks do not run. For an ordinary checked Save, writer failure alone does not change the integer answer; a returned No is not remapped to Cancel. Preserve those original control-flow distinctions.

For a final non-Cancel answer, accepted-Close order is main virtual `+0x288`, optional display callback `d0e540(0,0,true)` when its receiver is nonnull, then `signalRemoveGraphItems`; the guard returns true only after these calls. The selected display model and full transitive StateVar/render/notification effects remain partial. Do not project those effects as harmless or apply them on the warning-cancel path.

## Candidate-only test contract

The isolated `close_dialog::resolve` seam has a focused branch for this integer mapping. A candidate unit regression should:

1. Supply the qualified Close-question boundary and a callback that records event-dispatch order.
2. Resolve the shipped-style/initial-focus default Yes, then invoke nested Save.
3. In the warning branch, require the nested Save callback to dispatch the distinct warning boundary before it returns `DialogAnswer::Yes` with `lastOperationWarning=true`.
4. Assert the two boundaries occur in Close-question then warning order and the final outcome is `CloseCancelled`.
5. Assert no accepted-Close callback/teardown is represented or invoked by this helper.
6. Add controls showing a nested Save `No` remains `CloseAllowed` and nested Save `Cancel` remains `CloseCancelled`; neither is to be inferred from file-writer booleans.

This verifies only candidate answer ordering and fail-closed schedule plumbing. The current test uses caller-created qualification strings and a no-op dispatch spy; it does not validate a native modal schedule or original callback effects. Do not call it native-runtime acceptance.

## One actionable missing dependency

The exact due-event profile for the nested warning modal interval is missing. A static timer registration fact or a requested 1000 ms interval does not establish whether zero, one, or multiple diagram timers are eligible between modal entry and answer; the prior Save `processEvents(flags=0)` pump may also dispatch events before the warning. This is the single dependency blocking replacement of the current unsupported warning boundary with the default Yes return in an end-to-end candidate.

Next evidence must be one immutable, source-bound schedule for a fresh original fixture reaching this warning: timer IDs registered, those consumed at the intervening Save pump, ordered IDs delivered inside the warning modal interval (including evidence for an empty set), and each callback's then-current receiver/attachment and selected state effects before the warning result. Record the Yes result and its subsequent Save/Close mapping. Without this schedule, retain the candidate's unsupported warning behavior; do not force or suppress delivery.

## Evidence qualification

Basis: `analysis/wave3-normal-close-guard-contract.json`, `analysis/wave3-close-save-return-contract.json`, `analysis/wave3-close-post-answer-contract.json`, `analysis/wave3-boolean-timer-contract.json`, and `analysis/wave3-directory-failure-dialog-contract.json`. They are static PE/Qt analyses and specify no observed fresh Close dialog execution. No new runtime, GUI, build, source, or mutable REA operation was performed.
