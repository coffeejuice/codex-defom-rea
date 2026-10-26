# CP120: bounded static result path for the Yes-only warning

## Closed conditional edge

A specific activation edge is now source-closed from retained QtGui bytes: when Return/Enter reaches `QDialog::keyPressEvent` (`0x6545b1d0`), and a visible enabled default `QPushButton` is present, the handler calls `QAbstractButton::click` (`0x65371ee0`). The nearby assembly at `0x6545b29d..0x6545b2cb` walks the button list, checks the button's default state, and calls `click` on the selected entry. For the retained warning setup, the sole button is Yes and the qualified fresh style/focus path makes it the default. Therefore a delivered Return/Enter on that dialog activates Yes.

This closes an **if-activated** path. It does not establish that Return was delivered, that a mouse event activated the button, or that the modal exits without activation.

## End of the statically closed producer chain

`QDialog::exec` (`0x65459420`) initializes the internal result field at `+0x22c` to zero, calls `QEventLoop::exec(0x40)`, and reads that field after loop return. The retained loop trace shows repeated current-thread dispatcher execution until exit. The Yes-only warning's `QMessageBox::warning` call at original `0x1400b94b3` supplies buttons `0x4000` (Yes) and requested default `0x10000` (No); the Qt overload wraps only the first argument as its button set, so No is absent. The QMessageBox helper creates the sole Yes button, leaves explicit default unset, and uses the recovered initial focus/default chain. When the dialog returns a button selection, the helper's retained button-map route yields Yes `0x4000`.

The missing edge is the event that makes the dialog finish and the runtime occurrence's event/callback schedule. Static evidence gives the Return/Enter-to-click route and the post-exec result conversion, but the bounded artifacts do not close the complete connection/callback from this button activation through `QDialog::done` for this particular warning, nor show any actual activation. A focused/default button is a target, not an invocation. A timer or reentrant callback can also execute during the nested loop; no retained due-set or answer-time trace binds those effects.

## Composed Close result, conditional on activation

The native warning's only button is Yes. If the modal actually returns that Yes result, `0x1400b8fb0` returns `0x4000` early before current-operation Save and output writers. If this happens within normal Close's nested Save, `0x140044110` maps exact Save Yes to Close Cancel `0x400000`, leaves the project attached, and skips accepted-Close callbacks. The outer Close question and nested warning each have their own `QDialog::exec` interval; the static integer mapping is closed after the result, but neither the activation nor due events in either interval are runtime-observed.

`src/engine/save_project.cpp` remains correctly fail-closed at the candidate boundary: it records pending default Yes then throws unsupported instead of treating focus/default state as an answer. The isolated Close answer helper can test supplied integers; it cannot produce the warning result or modal side effects.

## Capture reconciliation

The historical `evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3/dialog_1.png` is a current-operation Close question with Yes visually focused. It neither depicts the must-be-last warning nor records input/return. The older `evidence/gui_mo/s1/mdt/L0/C/inventory.md` table says the warning has Yes/No with default No; this is superseded by the later exact original call and Qt overload reconstruction in `analysis/wave3-close-save-return-contract.json`, which establishes a Yes-only mask with absent No. Neither source is a fresh runtime action capture.

## Smallest evidence needed

When the GUI release gate permits a fresh owned-original capture, instrument one disposable project whose flow contains a must-be-last operation before a later item. For the warning interval record entry focus/default/button set, each delivered event and callback in order, whether Return/Enter or mouse activation occurs, the click/button signal and `QDialog::done`/exec result, the QMessageBox mapped standard button, and the Save return/writer reachability. Preserve receiver and model attachment before/after callbacks. Then issue a separate Close request and record its own activation/result plus the nested warning interval if entered. This distinguishes the source-closed conditional Return path from actual default behavior and event delivery.

## Bounded evidence receipt

Original binary `DEF_GUI_MO_64.exe`: SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. QtGui4_64.dll: SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

- Existing QtGui capture `analysis/wave3-closure-next-6545b1d0.asm`, `0x6545b1d0..0x6545b32a`, SHA-256 `cca521cca15f7ec780df0fe2c798a0b2ca77de26d6ca81f89f54f1334aadf115`; exact call `0x6545b2cb -> 0x65371ee0`.
- Existing QtGui capture `analysis/wave3-closure-next-65459420.asm`, `0x65459420..0x65459598`, SHA-256 `4fe2a1475ade838554e36f703e41717c6b3044b88ac5a6ae7dcd7b40506122bd`.
- Existing QtGui capture `analysis/wave3-closure-next-65477580.asm`, containing the modal exec call and returned button-map lookup, SHA-256 `30d7d41b875e048d336b46fc0a47a8ef62df939e3e8bdf7f2ef57c3cb9efef74`.
- New bounded console-only DUMPBIN14.44.35228.0 selections: QtGui `0x65473bd0..0x65473c80` and `0x654777e0..0x65477820`; the first confirms `QMessageBox::buttonClicked` is a signal emitter (`QObject::activate`), not the missing dialog-finishing callback. No files were written by those selections.
- Existing contracts: `analysis/wave3-close-save-return-contract.json` (`811a759327378fcfd34e1afa2a964e14ddb9466fd15d2a222b5acdc862997871`), `analysis/wave3-normal-close-guard-contract.json` (`2c3c855180354f53810702b5e1cabfae0e9365e6b61f5f86902394e419a1bde5`), and `analysis/wave3-directory-failure-dialog-contract.json` (`4111e716cc5d5482433f33c6330a63ec4a9f618b592cd3a335b7f88887bf8ec4`). All are retained static evidence; none claims a fresh original dialog interaction.

No GUI control, original execution, source/build/test edits, or mutable REA operation was performed.
