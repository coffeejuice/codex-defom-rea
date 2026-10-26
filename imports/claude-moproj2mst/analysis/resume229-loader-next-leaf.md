# CP229 loader next leaf

## Candidate and decision

The smallest remaining raw-alias consumer with a plausible finite owner command is `furnacePostLoad` in `source/apps/moproj2mst/src/engine/loader_l2.cpp` (around lines 325–340): it obtains `m->simCtl()` and, through an immediately invoked lambda, sets `furnace_on`, and only when the furnace curve count is zero performs `setup_curves(0x16)` followed by `set_defaults()`.

This can become a named `GuiModel::initializeFurnacePostLoadDefaults()` command that owns the simulation-control access and the exact conditional native sequence. It avoids returning/exposing a control pointer and requires no change to `Project`, `Operation`, or lazy model creation: `opModel` remains the same lookup, `if (!m) return` stays before the command, and the following `furnaceProps(c.propsOf(nullptr))` remains after it. Do not cache or eagerly create a model as part of the command.

## Exact ordering and unresolved semantic gate

Preserve: resolve `m`; return on null; obtain the current sim control; write `furnace_on = 1`; query curve count; if zero, call `setup_curves(0x16)` then `set_defaults()`; only then execute the existing property dynamic-cast expression. Do not pre-read curve count or initialize defaults unconditionally.

A readiness dependency remains: current `m->simCtl()` calls `requireGeometrySyncAvailable()` and can throw before the lambda; after it returns, a null control is unchecked-dereferenced. A new owner command must preserve the geometry guard before mutation and cannot silently turn a missing step into a no-op or checked converter error. The constructor normally creates a step, but this is separate from claiming semantics for models with replaced/null step state. Likewise, preserve native `curve_count`, `setup_curves`, and `set_defaults` dispatch/order exactly. Since CP226's contract covers only `setDimension`, it does not establish this command's guard/null contract. Confirm the current private model access patterns and native method exception behavior before implementation; otherwise the candidate is blocked on that narrowly scoped contract, not on the whole loader family.

## Files and focused tests

Production, once the gate is settled: `source/apps/moproj2mst/include/m2m/engine.hpp`, `source/apps/moproj2mst/src/engine/gui_model.cpp`, and `source/apps/moproj2mst/src/engine/loader_l2.cpp`. Keep the `furnacePostLoad` call site and the preceding null return in place; replace only the lambda/control-pointer extraction with the named void command. Do not change the C ABI.

Targeted tests should use the existing furnace/l2 regression fixture/target rather than broadening CP226-owned tests. Locate the exact registered name from `source/apps/moproj2mst/tests/mdt/CMakeLists.txt`/configured metadata at implementation time. Cover two states: zero curves triggers `furnace_on`, one setup call with 0x16, then defaults; preexisting nonzero curves still sets `furnace_on` but leaves the curves untouched. Check state/effects and ordering with the existing test instrumentation if available; do not introduce a new test hook solely to spy on native calls. Add parked-geometry refusal coverage only through the established geometry-barrier fixture, ensuring state remains unchanged. Missing-step runtime testing is not appropriate while behavior is an unchecked null dereference; document the precondition instead.

## Evidence and limits

`analysis/resume222-loader-alias-ledger.md` records this loader_l2 family as raw sim-control callbacks and calls for named control/setup operations. Current `loader_l2.cpp` source directly establishes the sequence above. The source does not by itself prove non-retention by the native curve helpers, so the implementation must keep them entirely inside the owner method. This is a recommendation with the explicit guard/null-contract gate; it is not evidence of an original fresh runtime occurrence or complete loader closure.

Only this report is written. No source, build, shared recovery state, original, GUI, or REA state is changed; no tests are run.
