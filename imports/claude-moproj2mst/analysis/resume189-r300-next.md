# CP189 R300 next raw-step owner closure

## Reconciliation and ownership

CP188's `contactFlagsFromEngine` closure is recorded complete in
`analysis/resume188-r300-contact-flags-implementation.md`: its single
`m.step.get()` and every pair read are inside an immediate callback, with no
escaping pointer or new null policy. The report releases the assigned source
and test files to root. The frozen/tested integrated checkpoint still needs
root's build, regression and freeze receipts; this note does not assert those
gates passed. CP188 ownership is closed for the next sequential source cut,
subject to root confirming the current hashes against its selected freeze and
assigning the shared pair. Do not edit either CP188 file while reconciling.

The smallest remaining raw-step owner in `save_current.cpp` is
`saveMaterials(Project&, Operation&, FileEffects&)` at `0x140058cc0`.
It is already the sequentially adjacent helper after equipment serialization
in `syncOperationFiles`, and is function-disjoint from CP188. Its existing
scoped callbacks acquire material count, test each material for eligibility,
then reacquire for the writer. Preserve those intentionally repeated model
accesses: the CP185 source review records that native reacquisition structure.
The remaining unscope is the `saveWholeModelKey` call at the end, which
directly evaluates `m->step` in its callback. This is smaller than broadening
the work to `syncOperationFiles` or `saveCurrentOperation`.

Proposed pair: `source/apps/moproj2mst/src/engine/save_current.cpp` and
`source/apps/moproj2mst/tests/s/test_save_current.cpp`, Testkit target
`m2m.s.save_current`. Root must assign exclusive ownership after reconciling
current source hashes and CP188's release; root retains build, integration,
regression and freeze.

## Behavior to preserve

Keep `n` acquisition through `withOpStep`, the inclusive one-based material
loop, each per-index eligibility callback and path construction between
lookups, and the final writer callback as separate accesses. The final
callback's current null guard must remain at its current point. Within it,
call `saveMaterialKey(p, *step, no, path, fx)` and discard its status, as the
caller does today. Do not hoist a step pointer across the loop, cache the
material pointer between callbacks, change the type-2 exclusion, reorder path
resolution relative to the third lookup, collapse callbacks, or add a
new helper-level null policy. The already-recovered native qualified behavior
allows a no-step callback to do no write; retain the status-ignored behavior.

After the callback closes, no step or material alias may be used. Preserve
number capture before path derivation and number-based reacquisition inside
`saveMaterialKey` (including native lookup-before-open behavior for
`nativeTaskKeys`). This owner cut concerns lexical lifetime only; it does
not close Save admission, `syncOperationFiles`' later direct equipment and
whole-model accesses, or the broader raw-borrow inventory in
`resume173-r300-closure-audit.md`.

## Fixture and guards

Extend `save_materials_step_scope` in the existing test file. It already opens
the copied R46 operation-2 project, loads the real model, preserves/restores
the material-list links around constructor-initialized synthetic entries,
checks ordered nonempty material KEY writes and type-2 exclusion, and uses a
noncopyable detached-step RAII owner with restoration checks. Keep every
temporary material/list mutation confined to that discarded project. Reuse
the fixture for path/order assertions; verify that the writer callback is the
third scoped model access per eligible material, not a cached borrow.

The existing detached-step probe expects zero effects and verifies list
counts; keep it. Add only narrow assertions needed to demonstrate the
post-loop callback boundary or repeated acquisition ordering. Do not invoke
the private helper with a detached owner or manufacture invalid material
storage. Static review must establish the pointer does not escape; runtime
tests cannot prove that. Testkit preflight must confirm entry point, resolved
quoted includes, supported macros, and no C `assert` before release.

## Evidence and limits

Use `source/docs/re/moproj2mst/s/spec.md` sections 1 and 2 for the
`saveMaterials` call position and material writer qualification, plus
`analysis/resume185-r300-source-review.md` for native repeated model/material
access ordering. The existing material fixture is reconstructed helper
mechanics evidence only: it is not a fresh original-process comparison,
public Save dispatch proof, or native-byte oracle. Retain these limits in any
implementation report. Do not use GUI/app-control, original-process capture,
REA database, build, or shared execution state for this read-only planning
cut.

No source or test file was modified. Only this successor note is written.
