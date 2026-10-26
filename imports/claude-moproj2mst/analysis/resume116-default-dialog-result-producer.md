# CP116: actual result producer for the default-Yes Save warning

## Conclusion

Retained static PE evidence closes how the **default button is chosen** and how a clicked standard button is mapped back to a Qt result. It does not show that an unactivated default button causes `QMessageBox::warning` to return Yes. `QDialog::exec` enters a nested event loop and returns only after the dialog is ended; initial focus/default state is not itself a result-producing event. No retained original capture records the action that activated this warning or its actual return value.

Treat the previous “fresh default answer = Yes” contract as a conditional button-selection/result mapping: if the sole Yes button is activated under the recovered initial-focus/style path, its standard-button result is `0x4000`. It must not be read as evidence of automatic Yes or of the user's actual answer.

## Static producer chain

- Original DEF_GUI_MO_64 `0x1400b8fb0` calls the QMessageBox warning overload at `0x1400b94b3`. The exact call passes available-button mask `Yes=0x4000`; the separate requested default is `No=0x10000`, which is not present. This corrects the older `evidence/gui_mo/s1/mdt/L0/C/inventory.md` row that described Yes/No with default No. The inventory is contradicted by the later exact call/Qt-overload trace in `wave3-close-save-return-contract.json`.
- The retained Qt wrapper/shared helper trace shows it creates the sole Yes button. Since requested No is absent, no explicit default is assigned. Under the statically qualified shipped Windows/Common/XP/Vista style with unchanged initial focus, the layout/focus chain selects that sole auto-default Yes button. `QDialog::keyPressEvent` handles Return/Enter by clicking the currently visible enabled default button. That explains which button Enter targets; it does not prove that Enter was sent.
- The warning helper enters `QDialog::exec`. In retained QtGui `0x65459420`, it initializes the dialog result field at `+0x22c` to zero, calls `QEventLoop::exec(0x40)`, then reads the result field after the event loop returns. The Qt event-loop contract follows repeated dispatch through the current-thread dispatcher until loop exit. The dialog's default/focus setup does not write the result field or terminate the loop.
- The shared QMessageBox helper at QtGui `0x65477580` calls that dialog exec, handles the special `-1` return as Cancel, and otherwise converts the returned button ID through the box's button map. For the selected sole-Yes activation, the mapped standard-button integer is `0x4000`. The retained artifacts do not identify a runtime activation event, actual `done/accept` caller for this warning instance, or an implicit timeout/automatic acceptance.

These claims are static and scoped to original DEF_GUI_MO_64 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtGui4_64 SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

## Nested Close effect after an actual Yes result

For a nonnull opened project on the process-type-5 path, normal Close `0x140044110` asks its separate Yes/No/Cancel current-operation question. The retained style/focus analysis selects Yes as the initial default, but again the modal result requires activation. If Yes is actually returned, Close runs nested Save while the project and diagram remain attached.

If that nested Save reaches the must-be-last warning and the sole Yes is actually activated, `0x1400b8fb0` returns `0x4000`; Save short-circuits before current-operation Save and project/property/MST writers. Close compares that returned integer, maps exact Yes to Cancel `0x400000`, leaves the project attached, and skips accepted-Close callbacks/removal. This post-result integer mapping is source-closed; reaching it requires the unresolved second modal result and its event effects. The Close question and nested warning are separate modal intervals and need separate schedules/results.

## Historical capture review and candidate boundary

The retained image `evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3/dialog_1.png` shows a current-operation Close question with Yes visually focused. It is a state image, not a click/keypress or dialog-return trace, and it is not the must-be-last warning. It cannot prove that Yes was activated or that Save returned `0x4000`. No retained dialog capture found for this audit logs the actual warning activation/result or the nested Close sequence.

Candidate `source/apps/moproj2mst/src/engine/save_project.cpp` currently sets `lastOperationModalPending` and `pendingDefault=Yes`, then throws `M2M_E_UNSUPPORTED` before candidate writers. That is the appropriate guard: a default-button value is not a producer for the returned answer, and the modal's event callbacks can mutate the then-current view/model attachment before activation. Existing `close_dialog_answer` tests prove integer substitution/remapping for supplied modal answers; they do not supply or qualify the warning's answer producer.

## Exact missing witness and next action

The missing item is one runtime modal occurrence showing **what ended the dialog and what was delivered before it ended**. After the current GUI release gate allows an owned fresh-original capture, use a disposable project with a must-be-last operation before a later flow item. For the warning interval, retain the initial button/default/focus state, all delivered input and callback events in order (including explicit none), receiver/model attachment before and after each callback, the action that activates/closes the box, clicked standard button, `QDialog::exec` return, mapped QMessageBox result, Save return, and file-writer reachability. Then issue a separate Close request and collect the outer question's activation/result and, if Yes, a second independently delimited nested-warning interval. Do not infer either result from focus, requested default, wall-clock delay, or the other dialog's event schedule.

Until that observation or a source-backed explicit activation schedule exists, the candidate must retain the unsupported warning boundary. No GUI control, original execution, source/test/build change, or mutable REA operation was performed in this audit.

## Evidence references

- `analysis/wave3-close-save-return-contract.json` (SHA-256 `811a759327378fcfd34e1afa2a964e14ddb9466fd15d2a222b5acdc862997871`): original warning call arguments, Qt overload, sole-button return mapping, Save short-circuit, and Close Yes-to-Cancel mapping.
- `analysis/wave3-normal-close-guard-contract.json` (SHA-256 `2c3c855180354f53810702b5e1cabfae0e9365e6b61f5f86902394e419a1bde5`): process/project guard, qualified Close default button, and nested Save path.
- `analysis/wave3-directory-failure-dialog-contract.json` (SHA-256 `4111e716cc5d5482433f33c6330a63ec4a9f618b592cd3a335b7f88887bf8ec4`): Qt `QDialog::exec`/`QEventLoop` dispatch mechanism; it does not itself identify this warning's activation.
- Raw bounded QtGui captures: `wave3-closure-next-65459420.asm` (SHA-256 `4fe2a1475ade838554e36f703e41717c6b3044b88ac5a6ae7dcd7b40506122bd`), `wave3-closure-next-65477580.asm` (SHA-256 `30d7d41b875e048d336b46fc0a47a8ef62df939e3e8bdf7f2ef57c3cb9efef74`), and `wave3-closure-next-6545b1d0.asm` (SHA-256 `cca521cca15f7ec780df0fe2c798a0b2ca77de26d6ca81f89f54f1334aadf115`).
- `analysis/resume48-dialog-modal-delivery.md`, `analysis/resume50-dialog-registration.md`, and `analysis/resume112-default-dialog-save-failure.md`: prior queue and composed Close limitations.
