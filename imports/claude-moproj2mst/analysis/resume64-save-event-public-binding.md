# CP64: Save-entry event prefix and public binding boundary

## Current public Save path

The full-save C ABI routes through `m2m_convert_*` into `core/convert.cpp`. With `M2M_FLAG_WRITE_SAVE`, `convert::run` constructs one `engine::ConversionSession`, attaches it through `Ctx::session`, and uses the session to own project/model state and logical diagram timers. Before the Save stages it calls `engine::saveCurrent` (native Save profile) or `engine::saveProject` (legacy/full writer profile); both ultimately call `saveCurrent` in `src/engine/save_project.cpp`.

The current `save_project.cpp` path does not call CP63 `SaveEntryEventPrefix`, `ConversionSession::dispatchAt`, or a real Qt `QCoreApplication::processEvents`. `ConversionSession` owns `DiagramLifecycleEvents`, a logical model with `dispatcherAlive_` initially true and timer IDs, not a native Qt dispatcher or OS timer queue. The current public Save therefore has no source-qualified event-delivery binding at original `0x140098129`. This report does not propose enabling public Save or changing its current unsupported warning guard.

## No-due route assessment

The source contracts separate two propositions:

- Static original Qt evidence: `processEvents(flags=0)` calls the current-thread dispatcher once if present and can deliver timer messages already due. It does not block for a 1000 ms timer that is not yet due.
- Candidate adapter behavior: a synchronous C ABI call without a native Qt event loop could declare an empty due-ID list for its emulated schedule.

The second proposition is not an original no-event proof. The original fresh GUI has its own thread dispatcher and timers. The two selected Open attachment sites register separate `singleShot(1000)` timers. The elapsed time until Save is unknown; `processEvents(flags=0)` may dispatch any already-due timers. The nested warning's later `QDialog::exec` is a separate repeating modal event loop and can dispatch remaining events before the Yes result. No retained capture identifies which timers are consumed at either interval. Therefore no source-qualified empty due-event route currently exists for the public/original Save entry point.

The CP63 helper itself has a `dispatcherPresent=false` route that returns `DispatcherAbsent` without dispatch, and a dispatcher-present explicit-empty-observation route that records one poll with no due IDs. These are input-conditioned state transitions only. Neither proves that the original app had no dispatcher or no due timers. `SaveEntryDueObservation::qualification` and `Origin` are descriptive; the helper does not authenticate a native receipt.

## Exact owner

For the candidate, the logical owner is `ConversionSession::events_`, which is session-scoped and separate from `Project`. It owns logical timer registrations and receiver IDs. `ConversionSession::dispatchAt` consumes explicitly supplied due timer IDs before callback, then resolves the live receiver and current diagram attachment. This is the appropriate adapter-side timer owner; however, no existing public Save caller supplies a `DueObservation`, and no adapter profile is source-bound to the `0x140098129` pump.

For the original, the static Qt contract identifies the current thread's event dispatcher as owner of `QSingleShotTimer` children. It does not establish a candidate `ConversionSession` object, nor does it reveal which dispatcher queue entries are eligible at Save entry. Do not conflate the candidate logical owner with the original Qt dispatcher.

## Guarded integration direction

Before any public Save binding, the call site would need to:

1. Preserve the source order: perform the one flags-zero Save-entry pump before subproject validation and before any last-operation dialog can be constructed.
2. Obtain a validated schedule tagged specifically to `0x140098129`, containing the ordered due timer IDs, with an explicit evidence-backed empty set when appropriate. Deliver only through `session.dispatchAt` so pending ownership, consume-before-callback, receiver identity, and current attachment checks remain in force.
3. On a later must-be-last warning, require a separate modal-window schedule. Never reuse the Save-entry due list or infer its complement as the modal list without evidence covering the full interval.
4. Preserve existing Save/Close integer behavior: warning Yes returns before file writes; normal Close remaps exactly nested Save Yes to Cancel. Do not let the dispatch helper change answers or writer status.

Until the first two schedule profiles are validated, the public Save guard should stay unchanged. A candidate-only explicit-empty profile may support a deterministic regression of the helper mechanics, but its result must not be labeled original-equivalent.

## Smallest next evidence

One fresh original process reaching the invalid last-operation warning, with both Open-time timer registrations tracked, can close the missing no-due question only if it records delivery through both intervals: timer events consumed at Save-entry poll and timer events delivered inside the nested warning loop, including evidence for an empty set at either interval. Record the actual dispatcher/receiver identity, then-current model/item, callback effects, warning Yes result, Save return and Close mapping. Until a supported app-control release gate clears and this capture can be run under the controlled session rules, static source facts alone are insufficient to create a public no-due binding.

No public binding, source edit, build, GUI session, or mutable REA operation was performed.
