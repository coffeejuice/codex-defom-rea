# CP227: Save-side finite operation assessment

## Decision

No safe Save leaf is established from the available CP222/CP225 evidence. The bounded candidate `feDefaultPairs` in `source/apps/moproj2mst/src/engine/save_l3.cpp` is explicitly not a leaf: its borrowed `DB_STEP_DATA*` is both mutated (`inter_obj.clear()`) and read repeatedly inside a synchronous lambda, while the same operation traverses `GuiModel` objects and reads current operation properties. Replacing the borrow with an operation that returns copied data would require deciding the exact mutation and traversal boundary, and risks changing the order of clear, object lookups, filtering, and pair construction.

The ordinary object-key writer is also not safe for this increment. `saveObject` in `save_current.cpp` feeds the borrowed step to native file/stream writers via `writeTaskKey`, including target alias/open/truncation sequencing and per-slot flags. Moving that consumer behind a model operation would either pass file/callback effects through the new method or duplicate native serialization responsibility; neither is a small finite value operation.

## Relevant evidence

- `analysis/resume222-save-alias-ledger.md`: `withOpStep` and nested Save callbacks are synchronous, call-scoped borrows. It specifically records `saveInterObject` null-step return before contact/file work and the `saveObject` / mesh / BCC / property writer chains. It requires preserving callback timing, nullable behavior, per-slot order, and writer return handling.
- CP225 validation is frozen in `analysis/wave3-checkpoint225-validation.json`, bound to `analysis/cp225-two-owner-full-build.json` and `analysis/cp225-two-owner-full-tests.json`. It covers 367 registered tests (360 passed, 6 passed with logged gaps, 1 with logged differences; none incomplete, skipped, or failed) and explicitly scopes work to the named weak-cache owner comparison and viewer tail-number operation. Its qualifications include that historical oracles are not fresh original-session acceptance evidence and `s.save_folder` is diagnostic. The earlier `analysis/cp225-cache-owner-build.json` is interrupted preflight history and is superseded for checkpoint validation. This freeze validates CP225's scoped changes; it does not establish Save output equivalence or authorize broader Save changes.
- `save_l3.cpp` `feDefaultPairs` invokes its callback immediately with `m ? m->step.get() : nullptr`, clears native `inter_obj`, then reads geometry and `op.props` while constructing pairs. This makes a returned copied scalar insufficient.
- `save_current.cpp` `saveInterObject` invokes an IIFE synchronously, returns false on null step before contact work, then runs contact mutations and native key writing. The order is observable per the CP222 test ledger.

## Exact scope if revisited

Potential future direction: first identify a native-model operation whose contract is a single copied scalar or immutable value that Save actually consumes. It must not cross into file opening, output callbacks, mutable model state, or slot loops. Require the current Save output/effect tests in `tests/s/test_save_io.cpp` (especially native byte matching, alias rejection before truncation, denial/order tests, and full-save sequencing) plus the owning feature test. Preserve C ABI and cache1 unchanged. Current evidence does not name a qualifying call site or test that can verify such a migration, so no exact source edit/test pair is recommended.

Read-only assessment only. No source, build, executable, GUI, REA session, or shared runtime state was changed.

