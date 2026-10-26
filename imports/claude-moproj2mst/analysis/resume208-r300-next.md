# CP208 R300 next source/evidence boundary

## Decision

No focused R300 source increment is justified by the retained evidence. Keep CP163's `CoordinateSync` stop immediately after publishing the post-query A allocation, with cache1/selected-length/arrays/reader effects unopened. The smallest valid next step is a same-occurrence original Open-to-InitialFill witness, plus source evidence that closes the `GuiModel::step` access surface before any cache1 publication.

## Reconciliation

- **CP163** is the R300 cut: after the real `step_dir_sizes` query it allocates and publishes fresh A (`u1b78`) under the retained ownership, reader, receipt, directory and version gates, then returns `CoordinateSync`. Its implementation report explicitly leaves selected length, cache1, arrays timer, DB body read and Stop untouched. It does not establish public Open-to-InitialFill execution.
- **CP195** is not R300. `resume195-r300-broader-next.md` uses “R300” to describe a broader workstream and selects a DOE `TaskFactory` caller-JE increment at QtCore `0x131438`; its scope is the connector/QObject task factory, not editor300 cache1. The associated implementation is in `source/apps/moproj2mst/src/doe/task_factory.cpp`.
- **CP199** is also unrelated to editor300. Its validation scope is the QtCore TLS-index symbolic CMP at RVA `0x29614..0x2961b` plus RS39 lubricant rebuild test-only coverage. It supplies no editor300 owner, selected-length, cache1 or Open-to-InitialFill evidence.
- **CP203** directly re-audited R300 and found no cache1 source increment. `GuiModel::step` remains a public `unique_ptr`; mutable pointer accessors and callback capture/escape paths remain. A Stopping flag in converted entry points would not close these bypasses. The broader source scan found `.step`/`->step` uses in 41 application-source files. The existing `withStep`/`withOpStep` callbacks are conventions, not enforced borrow lifetimes.

## Exact smallest evidence needed

1. For public behavior: one correlated original occurrence binding the live Project/DB reader, actual step and selected Stopping node, query/version mutation, A allocation/publication, selected zero length, cache1 store, loader normal return, arrays checkpoint, recipient assignments, and reader/outer return. The retained CP163/CP203 disassembly proves static instruction order only; it does not prove this public execution or that a later cache hit cannot observe cache1.
2. For a source-ready internal cache1 cut: a reviewed complete access-closure proof. Migrate all direct step reads/writes without changing admission/effect order; prevent public replacement/borrow; close mutable `simCtl`/object aliases and `Operation::engineModel` routes; and ensure Stopping, load/reload, reset, viewer/display and Save all consult the same model-owned parked state before access. Audit callback captures and pre-existing aliases, not just callback declarations.

These are conjunctive prerequisites to cache1. Neither a narrower callback wrapper nor the unrelated CP195/CP199 transitions meets them. The current GUI release lifecycle remains externally blocked, so do not acquire GUI control for this evidence.

## Checks and scope

`python admin/check_continuation.py` returned `CONTINUE`, with 15 remaining workstreams and all eight final gates open. The R300 next action explicitly retains CP195/199 helpers and CP163's cache1 guard while requiring alias closure and an Open-to-InitialFill witness. No source or tests were changed or run. This report is the only file written; no GUI/app-control session, original process, project, executable, build, deployment, mutable REA database, shared plan/state, or external checkout was touched.
