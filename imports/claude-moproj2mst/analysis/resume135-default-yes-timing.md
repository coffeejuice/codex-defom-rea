# CP135: default-Yes Save warning timing frontier

## Finding

Retained evidence still does not support a source cut that produces the must-be-last Save warning's default-Yes result. The call is source-qualified as a sole-Yes QMessageBox warning with an absent requested No default; the shipped-style focus chain selects Yes as the initial default target. `QDialog::exec` then runs a nested event loop. Its result field is initialized to zero and is returned only after an event ends the dialog. Default/focus state does not write the result or exit the loop.

If a Yes activation is delivered, retained QtGui static evidence closes the path through button click, QMessageBox button handler, `QDialog::done`, `exec` return, and standard-button value `0x4000`. Native Save then short-circuits before current-operation/project/property/MST writers. Those are conditional consequences; they do not establish that activation occurred.

## Callback timing and retained runs

The current retained index has only broad post-action dialog summaries for actual No responses to separate Close questions, such as `evidence/gui_mo/oracle_b/_tools/rs_log.tsv` rows 100 and 114. They do not name the must-be-last warning or record pre-answer input/callback delivery. `133_B_DBSTEP_BOOL3/dialog_1.png` shows focus on Yes in the outer Close question, not its eventual No action and not the Save warning. Neither source establishes timer readiness or callback execution during either nested loop.

The modal-delivery reports (`analysis/resume48-dialog-modal-delivery.md`, `analysis/resume49-dialog-timer-schedule.md`, and `analysis/resume50-dialog-registration.md`) identify a possible diagram callback family and nested dispatcher, but leave due-set and ordering at warning entry unknown. A callback can run before answer in principle; no retained occurrence proves whether it did, which receiver it used, or whether the attachment/model changed before Save resumed. Save's earlier nonblocking `processEvents(flags=0)` pump is a separate boundary and cannot stand in for the warning's nested loop.

## Narrowest closed behavior

The only safe candidate seam remains conditional answer handling: with a validated supplied modal schedule/result, map actual standard-button Yes to native integer `0x4000` and preserve the source-ordered Save early return. Integer substitution/remapping tests can exercise that mapping but cannot be described as default selection, modal delivery, or pre-click state behavior. Without an occurrence-qualified modal answer and callback schedule, no additional private decision branch is source-closed; keep the warning fail-closed before writers.

## Exact missing witness

One same-process original occurrence is needed from warning entry to return. Record the due/queued events and delivery order, including an observed empty set; capture receiver and current model/flow attachment before and after each callback; record the activating input, button clicked, `done(int)`, `exec` return and QMessageBox mapping; then record Save's return and writer reachability. If auditing normal Close, capture its question and nested warning as separate intervals. Retained static PE plus post-action `rs_log.tsv` summaries cannot supply this witness.

No GUI, mutable REA, source, build, or test changes were made.
