# CP87: first-Open timing remains runtime-dependent; minimal discriminating test

No independent static or original-capture-free fact currently resolves **actual** first-Open callback/addObject scheduling. The source does resolve conditional intra-call order: a successful addObject nested in an admitted synchronous Boolean Apply returns before that Apply/UpdateData invocation returns. What remains unavailable is whether that invocation occurs inside the correct enclosing Open interval. This report identifies the runtime-only predicate and one actionable passive instrumentation test; it enables no behavior.

Only this report was written. No source/build/tests/GUI/control/native execution or mutable REA. The12 retained capture/status files bound by CP76's survey were rehashed: all unchanged; the set of12 `build/original-*` probe directories is unchanged. No new retained runtime witness appeared in that bounded set. The current takeover state still distinguishes final Boolean-file equality from unreconstructed Open effect timing.

## Static proof and its limit

CP82's verified DUMPBIN binds selected UpdateData emission5f4524 ->custom notifier explicit type1 ->actual impl current/cast140dae340 ->Boolean virtual+1c0=1411e2870. If the current text selects `Add New Object`, Workpiece uses addObject140057ba0 at1411e2955 (return1411e295a); Cutter uses1411e2bc6 (return1411e2bcb), with Workpiece handling earlier. These normal call returns precede their caller's return. This is a conditional source theorem, not an observed invocation or object-creation count.

Source alone does not select current property index/editor, valid live receiver/method, notifier+141, combo text, callback occurrence, native readiness or nested event-loop branch.1000ms is a requested interval, not an Open duration or delivery deadline observation. The Save-entry flags-zero poll neither waits that interval nor proves a due set. A complete static proof of no indirect/reentrant event dispatch before Open completion is absent. Conversely, an unclosed indirect route is not proof that it actually ran.

Declared schedules can demonstrate different conditional candidate outcomes, but neither a deterministic candidate clock, empty due list, forced callback, source callgraph exploration nor successful final Save can identify the original schedule. Replaying/emulating the original with a fabricated OS clock/event order would supply a new schedule, not recover the missing occurrence. Native constructor/primary-address lineage likewise does not populate current field/editor state.

The current raw0x4a constructor guard remains at `widget_events.cpp:393`. Raw0x4a posted delivery, the1000ms diagram timer and synchronous UpdateData are distinct paths; closing one identity projection does not close the others.

## Exact runtime-only predicate

For the narrow Boolean Apply creation question, let M be an actual normally returned140057ba0 call whose native continuation is1411e295a or1411e2bcb, nested in the actual selected Apply/editor/model occurrence. The needed predicate is:

`selected enclosing Open entry < M successful return < selected enclosing Open completion`, with owner generations and action profile equal throughout.

An Apply entry alone cannot supply M because existing-object text may bypass both add sites. An addObject entry proves an attempt; successful object return and later rename/Apply completion differ. “First M” additionally needs gap-free coverage of the selected interval and qualified earlier state. This two-site predicate is Boolean Apply creation, not every engine object loaded/created by other paths.

The enclosing boundary must be explicit. CLI loader01ca30 returns at1c94, then postOpen02a0e0 is called at1ca9 and returns1cae, and the selected startup branch joins1e4e. For GUI Open, handler0271b0 returns at028514, after loader028383, postOpen02839a and busyfalse0283ba. A creation after1c94 can still be within Open; no generic “OpenReturned” event may substitute that loader boundary.

## One actionable passive instrumentation test

Prepare one fresh owned reference run, one copied Boolean fixture and one explicitly selected profile. This is a future test design, not execution/permission to acquire app control. Existing release/receipt gates and user-owned session rules still govern any run. Use native hooks/read-only argument/result observation, no Qt getter invocation, injected event pumps, target state assignments or sleeps.

Use one event schema: `{capture, module hashes/base, selected window generation, tid, ordinal, invocation, parentInvocation, kind, sourceSite, editor generation, operation generation, model identity, result identity/number, status}`. Ordinals order observations on the selected thread; cross-thread relevant mutation, read errors, missing frames, truncation or observer errors mark the timing result unavailable rather than inventing total order. Raw pointer equality alone is never a generation binding.

Hooks and selectors:

1. **Boundary selection:** retain actual main-window constructor owner10490. For CLI, select01ca30 only with that owner and caller continuation1c94; separately record loader return, selected02a0e0 entry/return with that same Project/owner, and selected branch join1e4e. For GUI, bracket actual0271b0 entry/onLeave for that owner and keep its loader/postOpen subspans distinct. Reject unsupported nested Open rather than flattening it.
2. **Boolean lifetime/invocation:** observe11e2480 constructor entry to assign editor-generation identity and retain its actual Operation argument; record Apply11e2870 entry/return with actual RCX and a per-thread invocation stack. Constructor entry is allocation-generation evidence, not completed QWidget construction or native liveness. An Apply without the selected owner/generation chain stays unclassified.
3. **Mutation attempt/return:** hook140057ba0; retain only calls whose actual return address rebases to11e295a or11e2bcb and which nest in that Apply frame. Capture actual model RCX on entry, RAX on normal return and, if nonnull/readable, the source-qualified object-number field+8. A failed read/nonreturn/null result is not successful creation. Do not invoke helper functions to obtain these fields. Emit Workpiece/Cutter role from the exact continuation, not from guessed object number/name.
4. **Optional causal stamp in the same test:** bracket183160 modelUpdated and140dae340 current-update calls; connect an Apply only when its actual stack nests in that selected frame. This distinguishes a witnessed callback cause from other direct Apply calls. A callback lacking this chain remains a separate observation. Do not infer its timer ID/readiness or due-list membership from the nesting stamp.

Keep logging compact and buffered; emit only selected boundaries/attempts/returns and bounded unmatched/error counters. Do not pause at constructor/Apply/timer entry or while Open is in flight. Such pauses can make the1000ms timer ready and change the very phase being measured. Hook instrumentation also has overhead: record hook set, observer timing/record counts, run environment, settings/fixture hashes and capture duration; report the result as an instrumented reference trace. Repeated lighter/heavier observations may expose sensitivity, but cannot certify uninstrumented scheduling by assertion. Never present an observer-induced forced callback as normal baseline timing.

Acceptance is observational: output a table of each qualified successful mutation versus LoaderReturned, PostOpenReturned and the selected enclosing completion, plus its observed causal parent if available. Label first only with complete coverage. If none appears in complete selected two-site coverage, state no **Boolean Apply** creation in that interval, without broadening to all object-creation paths. If capture ends before the enclosing completion, preserve unknown even if an early add was logged; if a positive completed add precedes an observed completion it can establish that occurrence without requiring timer registration reconstruction.

## Immediate work and remaining guard

The test design is actionable once the original run/release prerequisites are available; this lane did not author a hook script or run it. A candidate-only event-schema/phase-classification regression can independently reject loader-versus-Open boundary confusion, missing add return, foreign generations and unsupported nesting, but cannot close the original runtime predicate. Existing conditional Add New/order/refill tests already cover the mutation leaf and should not be re-counted as timing evidence.

Evidence: CP82 PE report and its exact return boundaries; CP80 narrow mutation witness; CP72 timer/notifier/receiver gates; CP75/76 historical owner/current-field distinction; CP77 primary Qt identity; retained probe004's qualified eb26f0 editor endpoint; CP76 survey hashes rechecked here. Original EXE identity remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No fresh callback/addObject/Open-boundary witness is available. Public first-Open/Save due-state/PostPending admission remains guarded.
