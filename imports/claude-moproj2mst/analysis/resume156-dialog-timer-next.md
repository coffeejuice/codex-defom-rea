# CP156 RDIALOG: timer due order and conditional Yes mapping

## Evidence boundary

The retained original static evidence closes only the post-activation branch. For the must-be-last Save warning at original `0x1400b94b3`, the warning offers Yes (`0x4000`) while requesting default No (`0x10000`), which is unavailable; under the qualified shipped-style/initial-focus assumption, Yes is the sole actionable/default target. If Qt delivers activation to Yes, `QDialog::done(0x4000)` is mapped by the QMessageBox helper to `0x4000`. Save then returns immediately on any result other than `0x10000`; for Yes it skips current-operation Save and output writers. These are static conditional consequences, not evidence that a modal answer occurred.

The selected successful Open route statically requests two distinct 1000 ms diagram `modelUpdated()` single-shots. Save's earlier `processEvents(flags=0)` at `0x140098129` polls without waiting for timers that are not yet due. The warning's `QDialog::exec` enters a nested event loop that can deliver eligible events. Retained evidence does not determine which timers are due at the Save pump, which remain pending at warning entry, whether either is delivered before activation, their order, or whether another callback family is eligible. A requested 1000 ms interval is not an elapsed-time or modal-duration observation.

## Smallest missing original witness

One qualified occurrence of this exact warning, tied to the immutable original and Qt hashes, must capture: timer readiness immediately before the Save pump and at modal entry; ordered event deliveries through the interval before user/default activation (explicitly recording an empty set if none); callback identity and then-current receiver/model/item state around each delivery; actual activating input and selected button; `QDialog::done(int)`, `exec` return and QMessageBox mapping; Save return/status/messages/continuation; and whether any writer ran. The receipt must distinguish the Save poll from the warning's nested modal loop. Static focus/default inference does not substitute for the actual modal answer.

## Source-ready prefix (not an implementation authorization)

A private conditional seam may accept a validated modal result/profile and apply only the source-closed result mapping: actual Yes `0x4000` yields the early Save return before current-operation Save and writers. It must reject an absent/unqualified profile and must not synthesize Yes, timer readiness, event order, or an empty event set. Keep the public warning guard fail-closed until the original witness exists. No GUI, mutable REA, source/build edit, or test was performed.
