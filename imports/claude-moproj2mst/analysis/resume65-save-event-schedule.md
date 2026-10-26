# CP65: Candidate Save-entry zero-delivery profile

## Owner distinction

On the original path, the `processEvents(flags=0)` call at `0x140098129` uses the current thread's Qt event dispatcher. Its single zero-timeout poll may deliver already-due timer messages; it does not wait for not-yet-due 1000 ms timers. The original dispatcher can own the diagram `QSingleShotTimer` registrations made earlier during Open.

On the converter path, `ConversionSession` owns `events_` (`DiagramLifecycleEvents`) and the receiver/timer IDs. This is a logical, clockless queue: `modelAttached` records requested 1000 ms intervals, `deliver` consumes only an explicitly named ID, and no wall clock or native Qt dispatcher is consulted. The session source explicitly disclaims a full Qt startup claim. Public Save currently does not bind CP63's prefix helper or call `dispatchAt` at the native Save-entry point.

## Can an empty list bind to the actual candidate session?

Yes, as a declared adapter policy only. The retained `wave3-boolean-timer-contract.json` says the immediate adapter schedule supplies zero explicitly delivered timer events, and the `ConversionSession::dispatchAt` API treats an explicit empty due list as zero deliveries in that declared profile while `nullopt` means schedule unavailable. Thus a session-bound `DueObservation{origin=DeclaredAdapterSchedule, point=(0x140098129, occurrence), qualification=<adapter profile identity>, dueInOrder={}}` can represent the current synchronous candidate's no-delivery Save prefix without deleting, coalescing, or otherwise changing any pending logical timer records.

The zero list means only “this candidate adapter schedule dispatches no diagram timers at this prefix.” It is not “there are no pending timers,” and it is not an original-observed empty due set. The candidate queue may still contain the two earlier timer registrations on the selected normal Open path, and those remain pending when this prefix dispatches nothing. Keep the modal warning interval as a separate schedule input; do not reuse this Save-entry empty list for the nested `QDialog::exec` event loop.

## Required adapter binding shape

If root chooses to integrate CP63 as a candidate policy, construct the observation at the actual Save invocation from the live `ConversionSession`:

1. Bind exact source site `0x140098129` and the Save-call occurrence; avoid a generic `"Save"` point.
2. Mark `Origin::DeclaredAdapterSchedule` and use a stable profile identity naming the synchronous no-delivery policy. The profile must say this is candidate behavior, not an original due-set observation.
3. Supply an explicit empty due list and pass it through `ConversionSession::dispatchAt` so source-point matching and the session's event ownership remain the path used by Save. Do not clear `events_.pending()` or fabricate timer IDs.
4. Keep the current warning fail-closed behavior. If/when a warning modal schedule is introduced, it requires its own profile and ordered event set.

`SaveEntryEventPrefix` itself verifies exact site, flags zero, one attempt, explicit observation, and dispatch callback order. It does not authenticate the profile string or verify timer IDs against `events_`; an integration wrapper should create the `DueObservation` from a typed, immutable candidate schedule and use the actual `session.dispatchAt` callback.

## Original-parity limit and next acceptance evidence

No source-qualified empty *original* due-event profile exists. The original process has a real dispatcher; elapsed time between Open registration and Save is unobserved, and the single Save poll can deliver timers already due. If original default parity is claimed, obtain a fresh original trace recording timer registration, due IDs at the Save-entry poll, callback effects against the then-current attachment, and a separate modal interval schedule if the warning opens. Until then, root may test the declared candidate no-delivery behavior, but must not present it as native event-schedule parity.

## Evidence

- `analysis/wave3-boolean-timer-contract.json`: original Save-entry poll mechanics, timer schedule/unknowns, and explicit candidate adapter boundary (`Immediate adapter schedule supplies zero explicitly delivered timer events`).
- `source/apps/moproj2mst/src/engine/conversion_session.cpp`: actual session owner, explicit `dispatchAt`, and no implicit clock readiness.
- `source/apps/moproj2mst/src/engine/lifecycle_events.cpp`: `events_` pending IDs are recorded and consumed only through explicit delivery.
- `analysis/resume63-save-event-prefix.md`: CP63 helper/test scope and qualification limitations.

No source edits, builds, GUI sessions, or mutable REA operations were performed.
