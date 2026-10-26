# CP185 R300 next raw-step borrow: InterObject KEY writer

## Decision and candidate

The next disjoint, bounded source cut is `saveInterObject(Project&, Operation&, FileEffects&)` in `source/apps/moproj2mst/src/engine/save_current.cpp`. It obtains `DB_STEP_DATA* step` at function scope, then uses it only in the synchronous native/file writer callbacks passed to `writeTaskKey`. Confine the existing non-null path from model lookup through optional contact rebuild, contact-flag synchronization, path derivation, and the `writeTaskKey` call to one immediately invoked callback taking `DB_STEP_DATA*`; return the existing bool from that callback. Null must still return false before these operations. Keep pointer and callback synchronous and uncaptured beyond this invocation.

Candidate pair: `save_current.cpp` plus `tests/s/test_save_current.cpp`. Both have subsequent integrated changes, so recheck ownership and hashes immediately before editing. Current SHA-256: production `36b3b12693fd5c2b3d4f4bde9ec486a9b673f7575da4b6d94903de19ceb0cf8b`; test `aec6e922352e31123545270266576f7c9c1e9cd16e9cdf7ae30290b4fb530c37`. CP184's material test is present in the current test file and must be preserved.

## Order and guard contract

Preserve exactly one `opModel(p, op)` lookup, then null-model/null-step refusal before `lubSystem`, `rebuildSystemContacts`, `contactFlagsFromEngine`, path derivation, or effects. For a present step retain the order: optional `rebuildSystemContacts` when `op.lubSystem`; `contactFlagsFromEngine(op, *m)`; `interObjectPath(op)`; `writeTaskKey` with overwrite flag 1 and the same file/stream serializers; return `r == 0`. The step borrow stays valid through both synchronous writer callbacks. Do not add a geometry or Stopping guard, alter `writeTaskKey`, or claim aggregate admission closure.

## Focused fixture and assertions

Add a `save_interobject_step_scope` direct helper mechanics case in `m2m.s.save_current`, using the existing copied-corpus setup pattern in `base_default_pairs_step_scope` / `save_materials_step_scope`: open and activate `46_MO2_Boolean_Oprations/MO2_Boolean_Oprations.moproj`, select operation 2, install loader slots, load its operation, and require a real step. Declare the internal helper in the test's existing `m2m::engine` forward-declaration block if no suitable internal declaration exists. Ensure the input inter-object list is nonempty (seed a real pair if necessary), call `saveInterObject`, and assert success, the exact `interObjectPath(op)`, nonempty captured KEY bytes, and no deletes. Treat any synthetic pair mutation as discarded fixture state, not corpus output.

For the null-step branch, use a noncopyable RAII detached-step owner. Call only `saveInterObject` while detached; require false, no writes/deletes, and unchanged detached inter-object count. On normal and exceptional exit verify model and step identity and list count are restored. Keep the assertions explicitly helper-level: they do not prove public Save dispatch or native equivalence.

## Static acceptance and evidence limits

Verify a single model lookup, null refusal before all side effects, original ordering, unchanged serializers/overwrite policy, and no step pointer escape beyond the immediately invoked callback. Run `m2m.s.save_current` and confirm both present-step assertions and null-step refusal execute. Root retains grouped integration, source-stable suite, and freeze responsibilities.

The implementation annotation identifies the reconstructed function at `0x14005a8a0`; the existing native writer references are `0x140896030`. This local closure adds no runtime observation, cache1 authority, same-occurrence reader-tail claim, or model-wide parked barrier. No source, test, build, GUI/app-control session, REA database, or shared state was changed while preparing this proposal.
