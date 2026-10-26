# CP141: default-dialog branch and callback frontier

## Decision

No new source-closed branch exists for producing the last-operation warning's default Yes answer. The retained original/Qt evidence closes the caller and result mapping only conditionally on modal activation. Keep the public Save warning unsupported unless it receives a validated modal schedule/result; do not infer that schedule from default focus.

## Closed static branch

Original `DEF_GUI_MO_64.exe` (`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`) calls `QMessageBox::warning` at `0x1400b94b3` with button mask `Yes=0x4000` and requested default `No=0x10000`; Qt wraps only the first argument as available buttons. Under the documented shipped-style/initial-focus qualification, the sole Yes button becomes the default activation target. If an activation reaches that button, Qt's retained signal/slot chain writes the dialog result and exits `QDialog::exec`; the QMessageBox helper maps it to `0x4000`.

That supplied/observed-result boundary is the only narrow private seam: given a qualified actual modal result, model the source consequence. Original Save replaces its answer with the warning result and returns immediately for any result other than `0x10000`, before current-operation Save and output writers. For actual Yes, the result is `0x4000`. This post-result integer/control-flow cut is already statically closed; it is not a producer for the result.

## Callback timing remains open

The warning helper enters `QDialog::exec` / `QEventLoop::exec(0x40)`, which repeatedly dispatches the current-thread dispatcher. The selected Open path statically registers two separate 1000 ms diagram `modelUpdated()` single-shots. Before the warning, Save has a distinct `processEvents(flags=0)` pump that may consume whichever timers are due then, without waiting for not-yet-due timers. Static evidence does not determine timer readiness at that pump or modal entry, delivery during the warning, order relative to activation, or other eligible callback sources.

If a diagram timer runs, its callback resolves the then-current receiver attachment/model and can affect view and selection/editor state before Save resumes. This is a statically possible effect, not an observed callback in a warning occurrence. Therefore neither an empty callback interval nor a fixed timer subset/order is a valid default profile.

Retained dialog screenshots show visual state, while `rs_log.tsv` contains actual No summaries for separate Close questions. Neither records this warning's activation or pre-answer events. Static Return/Enter and clicked-button paths prove only what occurs if the corresponding input is delivered.

## Smallest missing witness

One original occurrence of this must-be-last Save warning must record, from entry to return: the pending/due event set and its ordered deliveries (including explicit none), callback identities and current receiver/model state before and after each callback, the actual activating input and button, `QDialog::done(int)`, `exec` return and QMessageBox mapping, Save return, and whether any writer executes. This closes both the answer producer and whether pre-answer callbacks alter the caller's state. A separate Close question/nested Save needs separate modal intervals if the larger Close path is claimed.

Until that witness or an equivalent validated modal profile exists, preserve the public fail-closed warning guard. No GUI/app control, source, build, test, or mutable REA operation was used.
