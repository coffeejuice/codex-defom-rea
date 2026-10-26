# CP60: Save pump versus nested warning modal schedule

## Determination

Retained native evidence defines two distinct dispatch intervals in the last-operation Close path:

1. At Save entry, `0x140098129` calls Qt `processEvents(flags=0)` before per-subproject validation at `0x1400b8fb0`. The timer contract maps this overload to one current-thread dispatcher poll; it can deliver timer messages already due but does not wait for a not-yet-due 1000 ms timer.
2. If validation finds a must-be-last operation before another connector item, `0x1400b8fb0` constructs the warning at `0x1400b94b3`. The QMessageBox warning enters a separate `QDialog::exec` / `QEventLoop::exec(0x40)` nested loop, which repeatedly dispatches until the dialog exits. The sole available button is Yes; its result is returned only when this modal invocation ends.

The intervals are structurally distinct and sequential. The initial Save pump occurs before warning construction, so it cannot dispatch the warning's eventual button answer. It may consume zero, one, or more previously registered due diagram timers. The warning loop may then consume remaining timers that become eligible before the Yes answer. Neither receipt identifies the set of consumed timers at either point. The answer itself is not a diagram timer; it is the modal dialog result. Its selection and timer dispatch can occur within the same nested loop interval, but they are separate effects/events.

## Consequence for candidate schedules

The candidate must not collapse this into one `dispatchAt` point or interpret Save's flags=0 pump as the warning modal loop. Model two separate schedule points:

- `0x140098129` Save-entry poll, before last-operation validation;
- `0x1400b94b3` warning modal interval, after the warning condition is reached and before its Yes result returns.

At each point, require a separate evidence/profile input with the ordered due IDs for that interval, including an evidenced empty list. Timer deliveries at the first point are consumed and cannot be delivered again inside the warning. A modal result remains Yes under the selected static button contract, but do not return it to Save/Close until the warning interval's event schedule has been applied. The subsequent control path is Save short-circuit before file writes, followed by Close Yes-to-Cancel and project retention.

## Exact remaining uncertainty

The static records establish the pump order and capabilities, not elapsed time, OS readiness, exact timer IDs delivered at either point, or whether other event callback families run inside the modal loop. Specifically unknown is how the two Open-registered 1000 ms timers partition between the Save-entry poll and the warning interval. A due timer callback resolves the then-current receiver/model attachment and can fit the view and potentially update current selection/editor state. No original runtime trace proves that callback occurs here. Do not infer the partition from requested interval, default Yes, or elapsed wall-clock assumptions.

## Smallest controlled capture after release gate clears

Use one owned fresh original process and a disposable successful `.moproj` fixture with a must-be-last item preceding another connector item. Establish and record the two source-predicted diagram timer registrations. Instrument/observe event delivery without substituting answers: capture the exact ordered diagram timer callbacks before the validation boundary, during the warning modal loop, and after the warning returns; record receiver identity and then-current model/item for each. Record the warning's sole Yes button/result, nested Save return, Close answer mapping, project attachment, and attempted/written files. Include a truly observed empty phase if none of the registered timers is delivered there. Preserve the pre-dialog Save pump and warning modal as separate timeline intervals.

This is blocked until the required supported app-control release operation and verifiable release receipt/status are established. No new GUI interaction should be acquired before then. If release support remains unavailable, retain this as the next controlled capture; the static contracts already permit implementation of the two-boundary schedule API, but not an invented due-event profile.

## Evidence and qualification

Sources: `analysis/wave3-boolean-timer-contract.json` (`pump_boundaries.save_entry`, timer registration and unknowns); `analysis/wave3-normal-close-guard-contract.json` (modal event delivery); `analysis/wave3-directory-failure-dialog-contract.json` (`QDialog::exec` nested dispatcher); and `analysis/wave3-close-save-return-contract.json` (warning return and caller mapping). These are retained static analyses, not runtime observations. No source/build/GUI/mutable-REA operation was performed.
