# Last-operation warning: static Yes path and correction boundary

## Static call path

The retained evidence binds the following control flow to the original binary (`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`):

1. `0x1400b8fb0` starts with result `0x10000`. It walks the connector chain, obtains the operation info flag at virtual offset `+0x60`, and enters the warning path when bit 2 is set on a nonfinal item. This is a structural precondition; the retained contract does not broaden the must-be-last class/flag inventory.
2. The warning call at `0x1400b94b3` passes only `QMessageBox::Yes` (`0x4000`) as the button argument and `QMessageBox::No` (`0x10000`) as the requested default. The retained QtGui enum-overload analysis says the first value alone is wrapped as the available-button flags; the second is forwarded as the default value. It is not a Yes|No button set.
3. The static Qt helper path creates the sole Yes button. Since the requested No button is absent, no explicit default is assigned; the existing contract derives the sole button's fresh default answer as Yes (`0x4000`). `0x1400b8fb0` returns that integer.
4. `0x1400980d0` replaces its current answer with each nonnull subproject check result and immediately returns for any result other than Yes (`0x10000`). A warning result of Yes therefore stops the nested Save before current-operation Save and output writers. The normal Close caller `0x140044110` maps returned Yes to Cancel (`0x400000`), leaving the project attached.

Sources: `analysis/wave3-close-save-return-contract.json`; `analysis/wave3-closure-next-1400b8fb0.asm`; `analysis/wave3-closure-next-1400980d0.asm`; `analysis/wave3-closure-next-close-post-answer-static.json`; and `analysis/wave3-closure-next-close-lastop-warning-static.json`. These are static PE/DUMPBIN/decompiler-derived artifacts. The contract explicitly excludes original runtime, DLL loading, GUI, app control and builds. The warning's modal loop remains an event-delivery opportunity.

## Current adapter decision

`source/apps/moproj2mst/src/engine/save_project.cpp` currently recognizes three must-be-last class names, sets `lastOperationModalPending` and `pendingDefault=Yes`, then throws `M2M_E_UNSUPPORTED`. It does not return the modeled Yes answer to the Save caller. This is deliberately conservative relative to the retained original path; the pending value is not yet a delivered modal result. Existing `tests/s/test_save_answer.cpp` confirms that the earlier answer remains unchanged and no writes have started when this exception is raised.

## Can the faithful Yes branch be implemented without GUI event delivery?

Not as a complete correction under the current adapter contract. The static artifacts establish the button configuration and post-answer caller behavior, but they do not establish the modal loop's event delivery, including whether eligible timers/callbacks run before the Yes result is consumed. The saved close/session contracts explicitly preserve that uncertainty. Returning Yes immediately from the adapter would silently skip a potentially state-changing event boundary; continuing as if the modal had no effects is unsupported by the retained evidence.

The answer value and the modal delivery are separable in the original control flow, but the available adapter has no demonstrated event scheduler/result seam that can faithfully represent this dialog boundary. A pure unit test may validate the decoded button/default mapping and the already recovered post-answer Save/Close mapping; it cannot validate the complete warning branch by assigning Yes and claiming behavioral parity. Keep `M2M_E_UNSUPPORTED` at this boundary until the event-delivery semantics are recovered or the accepted adapter contract explicitly provides a faithful event model. Do not infer that `M2M_FLAG_STRICT`, a pending timer identity, or an unspecified due-event profile supplies the missing behavior.

## Bounded next evidence

To unblock implementation, bind the dialog to a fresh original warning fixture and a concrete lifecycle delivery profile. Record the dialog's sole button and selected return, eligible pending events before/during the modal loop, callback order/effects, Save return, Close answer, project attachment and writes. The capture must distinguish no eligible event from an event observed not to run. Until then, the static answer is Yes, the static caller path cancels Close before writes, and the full adapter correction remains unsupported because event delivery is unresolved.
