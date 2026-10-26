# CP93 leased-reader identity access source review

## Result

The selected mismatch comparison now reads `nativeStep->db` inside a bool `GuiModelAccess::withStep` callback (`src/engine/viewer.cpp:604-606`). The condition retains the original `!reader ||` left operand, so a null reader short-circuits before the helper call. On the nonnull path, the callback captures only the already resolved `reader` pointer by value, compares identity once, and returns a bool. It does not dereference the companion pointer, return/capture the step pointer, or retain the callback.

Earlier owner checks establish the actual model and nonnull step: the N owner must match `model->step.get()`, and any E owner must match the same step. The code reaches this block only after the DB-presence callback proved `nativeStep->db != nullptr`. The peer `reader` was resolved from `dbOwner->reader` only after obtaining a strong `Project::dataDb` owner lease and verifying that owner matches `model->db`. Therefore both pointers in the comparison have qualified sources, and the strong DB owner remains in scope through later receipt/query/frame work.

## Preserved order and lifetime

The check order remains: model/byte/dimension/version admission; real coordinate COW and table selection; MAR/ID bounds; zero cache marker; node-reader timer start and checkpoint; DB presence; `currentProject`; strong actual DB lease/model-handle identity; actual reader resolution; null-reader shortcut and exact identity comparison; current directory receipt and physical-index bounds; record/query and optional frame operations. The short step borrow ends before receipt lookup or any DB query. No new reader lifetime mechanism or public API was added.

If `reader` is null, the legacy left operand immediately takes the pending return without evaluating the right side. The existing eight owner/provenance profiles cover missing scope, expired/foreign handle, forged model DB, forged companion DB, replaced reader and unobserved allocation cases; the actual matching owner/reader query tests exercise the valid path. As the CP93 plan notes, there is no separate runtime case with a null `DbHandle::reader`; this is statically protected by the short-circuit and is a narrow optional coverage addition, not a source blocker.

## ABI and scope

No model layout, helper policy, native field, public C signature, or export changed. The selected borrow adds no escaped pointer. Current hashes match the CP93 receipt: `viewer.cpp` `33f209956f40e5e0dfdd8c64a8182a78cb9619529d387ac05928546caae30703`; unchanged `test_viewer_box.cpp` `58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98`.

No blocker found. Review was read-only; no source/test edits, builds, or tests were made.
