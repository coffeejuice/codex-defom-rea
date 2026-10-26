# CP66: Save-entry due-event source qualification

## Static native order

For the selected fresh single-project lifecycle, native Open attaches the nonnull active diagram model through `0x140170430`; on the qualified active-subproject route, the model is attached twice and each attachment requests a separate `QTimer::singleShot(1000, receiver, modelUpdated())`. The timers belong to the current thread's Qt event dispatcher and are not coalesced or canceled by same-model attachment.

During Save, `0x1400980d0` calls `processEvents(flags=0)` at `0x140098129` before per-subproject validation at `0x1400b8fb0`. The selected Qt implementation performs one current-thread dispatcher poll with no blocking wait. It may deliver an already-due timer message; it does not establish that any timer is due. If validation later opens the last-operation warning, that warning enters a separate repeating `QDialog::exec` event loop. The warning's only button/result is Yes; Save returns that result before writers and normal Close maps it to Cancel.

## Timer slot effect statically bound

If a registered single-shot is delivered, Qt consumes/deletes the timer before invoking the captured diagram-view receiver's `modelUpdated()` slot. The slot first performs the view fit action, then reads the receiver's current model and current item at delivery time. A null current model/item skips diagram selection after fit. Otherwise it emits the diagram-item-click/update route, which uses current project/loading state and current item ID. For a non-process item with an existing property editor, the retained route applies the old editor, leaves it, creates/refills the selected item's editor, and shows it. Thus the possible slot effect is source-traced and can mutate view/property-editor state; it is not a stale registration-time item callback and not a no-op.

This qualifies the callback effects **conditional on a timer delivery**. It does not qualify whether one is due at Save entry, which of the two registrations is due, callback order for multiple due IDs, the current attachment, or whether the current item reaches the existing-editor branch.

## Due-set qualification result

No due-set or no-due route can be recovered from static call order alone. The flags-zero pump proves only one nonblocking dispatcher poll before validation. Requested interval 1000 ms plus deterministic registration count does not establish elapsed time from the later Open calls to Save. Dialog default Yes does not establish timer delivery timing. The saved timer contract explicitly lists actual elapsed time and OS timer readiness as unknown, and says historical screenshots do not prove timer count or exact dispatch phase.

Therefore a declared empty due list is usable as an explicit **candidate adapter schedule** (the current synchronous adapter profile), but it cannot be labeled an original source-qualified no-due observation. A source-backed original default lifecycle needs a fresh trace of the registrations, Save-entry poll, actual timer callbacks/current attachment, and, if the warning is shown, the separate modal interval. Until then keep due IDs external inputs and preserve the unsupported warning modal boundary.

## Candidate adapter separation

The converter's `ConversionSession::events_` is a logical clockless timer owner. It stores separate IDs and only delivers IDs explicitly named by a profile. It does not bind a real Qt dispatcher or infer OS readiness. Public Save does not currently call the CP63 prefix helper or the generic `dispatchAt` seam at the recovered Save poll. The adapter can implement the source-proven control point and callback semantics behind an explicit declared schedule; it cannot claim that the same list was due in the original.

The modal warning has a separate event schedule. Save-entry due IDs that were delivered are consumed and cannot be delivered again inside the modal loop. Any remaining pending timers are not automatically due merely because the dialog runs; the warning loop's current-thread dispatcher repeatedly waits/dispatches, but elapsed-to-answer and callback ordering remain unobserved.

## Qualification

Basis: `analysis/wave3-boolean-timer-contract.json`, `analysis/wave3-close-save-return-contract.json`, `analysis/wave3-normal-close-guard-contract.json`, and `analysis/wave3-directory-failure-dialog-contract.json`. These are static PE/Qt analyses; they contain no fresh GUI timer trace. No source, build, GUI, or mutable REA operation was performed.
