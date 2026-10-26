# CP124: modal activation frontier

## Finding

Retained evidence establishes the static button target and the conditional Qt result path, but no retained run records the activation of the default Yes button or the callbacks delivered before activation in the nested modal loop. The candidate therefore has no occurrence-qualified producer for the Yes result. Keep answer production behind the existing explicit modal-schedule guard.

## Evidence inventory

The retained `evidence/gui_mo/oracle_b` tree contains dialog PNGs for prior GUI observations, including `133_B_DBSTEP_BOOL3/dialog_1.png`. That image is a state snapshot of the separate current-operation Close question with Yes focused. It is neither the must-be-last Save warning nor an input/action/result trace. Other retained `dialog_*.png` files likewise provide visual states; the inventory yielded no associated event log that records a click/keypress, callback sequence during the modal interval, `QDialog::done` argument, `QDialog::exec` return, or mapped `QMessageBox` standard button for the must-be-last warning. Historical project/output captures and `view_state.json` files record artifact state, not the modal answer producer.

Static evidence is more complete but conditional:

- `analysis/resume116-default-dialog-result-producer.md` and `analysis/wave3-close-save-return-contract.json` identify the native warning call's Yes-only button mask, the absent requested No default, and the qualified sole-Yes default/focus target. They do not show activation.
- `analysis/resume120-default-dialog-result.md` traces delivered Return/Enter to the visible enabled default button. The premise—Return/Enter delivered to this dialog—is unobserved.
- `analysis/resume121-default-dialog-done-bridge.md` traces an activated button through button-box and QMessageBox slots to `QDialog::done`, `exec` return, and standard-button mapping. This is a conditional signal path, not an observed occurrence.
- `analysis/resume48-dialog-modal-delivery.md`, `analysis/resume49-dialog-timer-schedule.md`, `analysis/resume50-dialog-registration.md`, and `analysis/resume112-default-dialog-save-failure.md` establish nested event-dispatch capability and possible state-changing callbacks, while leaving modal-entry due timers, callback delivery/order, and input activation unresolved.

The prior gate record `analysis/gui-control-lifecycle-assessment.json` states `unsupported_release_api` with no supported release or release-status method. No GUI interaction was attempted for this audit.

## Smallest missing witness

Capture one fresh original-process occurrence of the must-be-last Save warning using a disposable single-project fixture. At modal entry, record the visible button/default/focus state, receiver/model attachment, and eligible queued/timer events. Until the first activation, record each delivered input and callback in order with receiver and model state before/after; explicitly record an empty delivered-event set if none runs. Then record the activating input, clicked button, `done(int)`, `exec` result, mapped standard-button value, Save return, and whether any Save writer ran. This single interval resolves both whether Yes was activated and whether pre-click callbacks affected the caller state.

Do not substitute a focused-button screenshot, a declared default, elapsed time, or a static Return-key path for that occurrence. The later Close question and any nested Save warning are separate modal intervals and are not required to establish this Save-warning frontier; they need their own ordered witness if the composed Close lifecycle is claimed.

## Candidate consequence

No source increment is justified by this audit. The native integer consequences are already mapped conditionally: actual warning Yes returns `0x4000`; nested Save short-circuits before its writers; normal Close maps that exact nested Save result to Cancel. The missing dependency is the occurrence-level event/input schedule and actual modal result, not another detached integer-mapping test. Preserve the candidate's unsupported boundary until that witness or an explicit validated modal schedule is available.

No source, build, test, application, or mutable REA state was changed.
