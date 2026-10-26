# CP121: Yes-button click to modal result bridge

## Static chain closed

The retained QtGui PE closes the synchronous bridge from an activated QMessageBox button to the dialog result. The conditional chain is:

```text
Return/Enter on the focused default button
  -> QDialog::keyPressEvent 0x6545b1d0 calls QAbstractButton::click 0x65371ee0
  -> QAbstractButton clicked signal reaches QDialogButtonBox::_q_handleButtonClicked
  -> QDialogButtonBox emits clicked(QAbstractButton*)
  -> QMessageBoxPrivate::_q_buttonClicked(QAbstractButton*)
  -> QMessageBox object's vtable slot +0x190 resolves to QDialog::done(int)
  -> QDialog::done stores the result and exits the nested event loop
  -> QDialog::exec returns it; QMessageBox maps it to the standard button
```

The activation is conditional. This chain proves what happens **if** Return/Enter or another button activation is delivered. It does not prove that the original user activated Yes, or that a warning auto-returns merely because Yes is default/focused.

## Connection evidence

The QMessageBox setup routine at QtGui `0x65475760` connects its internal QDialogButtonBox (`d+0x258`) to the QMessageBox receiver. At `0x65475a20..0x65475a38` it calls imported `QObject::connect` with these exact retained Qt string operands:

- signal: `2clicked(QAbstractButton*)`
- slot: `1_q_buttonClicked(QAbstractButton*)`

The `QMessageBox::qt_static_metacall` body at `0x654755b0` dispatches method index 1 to `0x65474b70`. That handler records the clicked button, maps it to its standard button ID, and calls the receiver's virtual slot at `+0x190` with that ID. In the primary QMessageBox QObject vtable at `0x65720eb8`, entry `+0x190` is exactly `QDialog::done(int)` at `0x654595a0`.

The input-side button-box connection is also retained. `QDialogButtonBox::addButton`'s private helper at `0x6539cb90` connects each added button's `2clicked()` signal to `1_q_handleButtonClicked()` on the button box. Its meta-call table maps that slot to `0x6539b910`. That handler obtains the sender button, emits the button-box `clicked(QAbstractButton*)` signal through `QMetaObject::activate`, then performs role-specific accepted/rejected/help notification handling. Thus the message-box connection above is reached by the button's clicked signal path, not by focus alone.

## Result propagation

`QDialog::done(int)` at `0x654595a0` writes the supplied integer to dialog private `+0x22c`, calls the event-loop exit path, and emits the dialog completion signals. `QDialog::exec()` at `0x65459420` initializes that result field to zero, enters `QEventLoop::exec(0x40)`, then reads and returns the field after the loop exits. The shared QMessageBox helper at `0x65477580` calls `QDialog::exec`; it handles `-1` as Cancel and otherwise looks up the returned button ID. For the warning's sole Yes button, an actual Yes activation therefore returns standard-button value `0x4000` through the warning call.

For the native must-be-last warning, original `0x1400b8fb0` supplies button mask `0x4000` and requested default `0x10000` (No, absent). The style/focus analysis selects the sole Yes button as the default activation target. On an actual Yes result, nested Save returns Yes before current-operation Save and file writers; Close then remaps exactly that Save Yes to Cancel. The existing integer mapping remains unchanged.

## Still unresolved

The missing evidence is now narrower: whether and when the warning receives an activating input, and which unrelated eligible events/callbacks the nested event loop dispatches before that activation. The Return/Enter action is a source-closed possible producer; there is no retained runtime event proving it occurred. Historical screenshots show focus only, not activation/result. Actual click-to-Yes, timer readiness/order, callback effects on current receiver/model attachment, and fresh-session behavior remain runtime unknowns.

Accordingly the candidate should keep the warning branch fail-closed unless it is given an explicit modal event/input schedule. A default-button value is not an event. Any capture after the GUI-release gate clears should preserve the warning interval and the separate outer Close interval, logging input/activation, callback order and receiver/model state, `done` argument, `exec` result, QMessageBox mapped button, Save return and Close mapping. No GUI control or original execution was used here.

## Evidence receipt

Original DEF_GUI_MO_64 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64 SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

Bounded DUMPBIN14.44.35228.0 selections were made directly against the retained QtGui PE, without writing capture files:

- `0x6539cb90..0x6539cc30`: per-button signal-to-QDialogButtonBox slot connection.
- `0x6539b910..0x6539b9c6`: QDialogButtonBox sender lookup and clicked/role signals.
- `0x65475760..0x65475c90`: QMessageBox private setup; exact clicked-to-slot QObject connection at `0x65475a38`.
- `0x654755b0..0x65475616`: QMessageBox meta-call dispatch to `_q_buttonClicked`.
- `0x65474b70..0x65474cde`: clicked-button storage, standard-button conversion and virtual `done` call.
- `0x654595a0..0x65459641`: QDialog result-field write and event-loop exit.
- Existing `analysis/wave3-closure-next-65459420.asm` and `analysis/wave3-closure-next-65477580.asm`: QDialog exec and QMessageBox helper result mapping.
- `analysis/wave3-close-save-return-contract.json`, `analysis/wave3-normal-close-guard-contract.json`, and `analysis/wave3-directory-failure-dialog-contract.json`: original caller arguments, Save/Close integer consequences, and nested event-loop mechanics.

No source/build/test changes, GUI control, original execution, or mutable REA operation occurred.
