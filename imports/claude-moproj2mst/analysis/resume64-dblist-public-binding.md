# Checkpoint 64: DBLIST facts and safe binding point

## Call path and existing owners

The full Save driver creates one `engine::ConversionSession` in
`src/core/convert.cpp`. It sets `ctx.session` only for `M2M_FLAG_WRITE_SAVE`,
constructs `StudyIo`, and binds `session.modelManagerOwner()` to that I/O only
when `ctx.session` is present. It then reads the project, commits the initial
project-model selection, activates Sub_Project 0, and dispatches the active
operation. The public entry does not expose an arbitrary sub-project selector.

For a deliberately activated Type 2 DoeOutput operation,
`DoeOutputClass::loadClassData` reaches the registered
`prepareCurrentOutputSession(Project&, Operation&)` callback through
`g_loaderSlots.readDoeDocument`. `prepareCurrentOutputSession` currently
builds a `SessionImportContext` with clear-named-model, warning-observation,
StepData, and `StudyIo*` inputs, then calls `readSessionRecords`. In the
current source, `importPart` still throws at either a direct DBLIST or
ReportList child before any such child is parsed or dispatched. The CP63
warning receipt and `DbListManagerPrefix` are detached helpers; neither is
called by this path.

## Facts that are already bindable

The existing full Save path has one useful actual owner chain: when the
session's `StudyIo` has a bound `ModelManager`, `StudyIo::modelManager()`
returns that manager, and `ModelManager::ownerSession()` returns its owning
`ConversionSession`. This is the candidate's already-owned manager/session,
not a manager to allocate from the SessionPart. The manager-prefix helper
requires that actual Session/Project/I/O/manager identity and accepts only a
resolved path already known to exist. It stops before native child
construction on a miss.

The warning receipt can consume caller-supplied facts without performing
effects: `hasMissingDatabase`, the observed global `+0x0c` warning-state
DWORD, the ordered raw missing DBNAME strings, and an optional opaque parent
token. It returns only the qualified title/prefix/name receipt when a missing
path exists and the state is not 5. It does not determine those facts. The
test's fabricated parent token is receipt-only and is not a native QWidget.

## First unavailable integration facts

There is no safe production binding point yet because the operation that
would call the detached DBLIST helpers does not have the required live
original-state facts. `SessionImportContext` contains no `ConversionSession*`
or DBLIST callback today; its `StudyIo*` can recover the actual manager owner
only for the full Save profile. More critically, no candidate owner supplies
the original caller's sixth-argument DBLIST byte, the original global
`+0x1c` admission value, or the global `+0x0c` warning-state value. Neither
field is represented by `SessionNotificationState::suppressed`, and the
existing Project/Session model does not provide an authenticated mapping for
either one. The actual QWidget pointer for the main-window warning parent is
also not available as a `QWidget*`; `ModelManager` ownership is not a widget
pointer. Do not synthesize these values from flags, active sub-project, or
model-manager presence.

Further, normal DBLIST execution needs the original `QFileInfo::exists`
decision and path spelling, the existing-child lookup, and the missing-child
constructor/register route. The current prefix covers only existing paths and
stops before the native virtual `+0x68` construction path. Thus even with a
real session manager, it cannot yet execute the full DBNAME loop without
dropping original manager effects.

## Smallest safe next source step

Do not change `importPart`'s public DBLIST refusal and do not attach
`SessionPart` data or invoke the receipt from `prepareCurrentOutputSession`.
The smallest source plumbing step, after root assigns implementation, is to
add an **optional non-owning actual-session handle** to the private
`SessionImportContext`, populated at `prepareCurrentOutputSession` only by
following an already-bound `StudyIo::modelManager()->ownerSession()` chain.
It must remain null when no actual manager/session owner is bound; Recording
mode by itself does not imply null because a Save-flag conversion can have a
real session and manager while using recording I/O. The binding must validate
that the session's live Project is the same `Project&`, and must not
allocate/rebind an owner. This gives future isolated manager work a trustworthy identity while
leaving the DBLIST/ReportList dispatch guard unchanged. It still must not be
used to manufacture the missing global gates or QWidget pointer.

Before any production DBLIST callback is wired, the next required evidence is
an actual ConversionSession representation/binding for the distinct global
admission and warning-state fields (or a defensible reason they are supplied
by a different owner), plus the exact sixth-argument source and main-window
QWidget identity/lifetime. Separately recover the missing-child manager
construction prefix and path-existence adapter. Only then can an explicitly
guarded DBLIST callback preserve the corrected tag assignment and the
already-qualified “earlier registrations survive later missing names” order.
ReportList remains its separate handler, and the existing public B127 guard
stays in place.

## Evidence limits

This is a read-only source and receipt audit. `convert.cpp`,
`SessionImportContext`, `prepareCurrentOutputSession`,
`session_records.cpp`, `ModelManager`, the CP62 DBLIST manager report, and the
CP63 warning contract establish the call/owner relationships above. No source,
build, tests, GUI, project, or mutable REA database was changed or executed.
