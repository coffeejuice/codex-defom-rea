# CP134 independent review: exact-frame geometry retirement

## Result

The proposal is source-compatible only if retirement occurs within the same synchronous `GuiModelAccess::withStep` callback that still holds the `nativeStep` argument. The current frozen viewer implementation has not yet incorporated retirement: after the node timer stop it sets `NodeSkipLoopCommitted`, exits the callback, and returns `HighVersionDbLoaderPending`. That refusal currently preserves the barrier and avoids replay. I found no evidence in the workspace yet of the forthcoming retirement source/test, so this report reviews the proposal and current boundary; it does not certify an implementation.

## Lifetime and frame identity

`GuiModelAccess::withStep` calls `model.requireGeometrySyncAvailable()` and then invokes the callback with `model.step.get()`. It does not acquire a mutex or pin the `unique_ptr`; the raw step pointer is valid only for this synchronous scope, assuming the documented no-reentrant-destruction boundary. Therefore:

- Check `nativeStep != nullptr`, `model.step.get() == nativeStep`, and `model.geometrySyncBarrier_ == frame` at the retirement point.
- Require `frame->stage == NodeSkipLoopCommitted` and the completed query/assignment/Stop facts from this invocation. Do not use a prior stage, lowered version, or global timer count alone.
- Clear only the exact installed barrier while still in that callback. Do not dereference `frame->step` after the callback or make a later refresh/poll a resume authority.
- If any guard fails before full Stop, keep the barrier and the existing pending/refusal behavior. No cleanup or replay should follow a partial loop.

A check of `model.step.get() == frame->step` outside the active borrow would only compare stale identities and would not establish dereference authority. Likewise, comparing the model's current barrier after returning from `withStep` is too late to establish that the step remained the borrowed one.

## Native return and no-replay boundary

The retained qualification in `resume133-geometry-next.md` closes the reader's normal `AL=1; RET`, the already-entered `which==0` outer tail for the low post-query version, and the coordinate getter's return of the existing optional-mesh data. The current implementation correctly avoids invoking the full reader or outer helper a second time. The integration must carry a local completion result from the initial callback, retire the exact frame there, then set the coordinate-prefix completion marker and continue once into the existing connectivity/readiness path. It must not replay query, timers, assignment, or the first coordinate getter.

The existing viewer still has the builder's separate second getter round and layout/numeric/owner guards. Those remain required; completing the first getter does not authorize skipping or weakening them. Connectivity-null Skip and populated mesh contribution are distinct observable outcomes.

## Tests needed before acceptance

Retain the existing pending/refusal cases for foreign/null owners, nonzero `+0x87c`, true assignment state, invalid MAR selectors, negative arrays-timer count, and incomplete Stop. Add/retain positive exact-frame cases for connectivity-null Skip and a populated builder path. Assert one query/assignment/timer sequence and one completion, byte-preserved current optional coordinates, and no repeated initial effects on the same invocation. A later fresh refresh is a new native invocation and should take the low-version fast return without repeating the completed initial effects.

The test must also discriminate stale/different frame and changed-current-step refusal without clearing another installed frame. It must not claim external mutation/thread exclusion; the proposal expressly supplies only the serialized initial-call argument.

## Evidence and limits

Reviewed `analysis/resume133-geometry-next.md`, `src/engine/gui_model_access.hpp`, `include/m2m/engine.hpp`, and the current geometry branch of `src/engine/viewer.cpp`. The proposal's identified native facts are bounded to the retained instruction ranges and candidate static control-flow walk described in that report. No source, test, build, GUI, original target, or mutable REA operation was run or changed. Root still needs to provide the frozen retirement implementation/test for code-level review and runtime validation.
