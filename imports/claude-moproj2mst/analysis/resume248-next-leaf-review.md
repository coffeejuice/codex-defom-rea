# CP248 meshing object owner-command review

**PASS — the proposed command is a bounded relocation of the existing by-number four-field writeback.** CP165's missing broader meshing witness does not block moving this already-supported operation. Approval is for the contract; no build/test result or new original-behavior proof is asserted.

Reviewed CP248, current `copyMeshingObject` and both Apply callers, `GuiModel::object`/`objectAt`, the `MeshObjectSnapshot` definition, existing model declaration/include arrangements, the named disabled-remesh and selection test cases including floating-point cases, and CP165's explicit evidence boundary.

## Exact owner operation

`MeshObjectSnapshot` is one object number plus four doubles, with no native ownership/alias fields. A const reference in the named command declaration needs only its namespace forward declaration in the existing editor-type block. `gui_model.cpp` already includes the complete editor header and uses the local byte-field access helper; no new source file, test registration, general offset setter or C ABI is needed.

Inside the command call `object(value.number)` exactly once, rather than `objectAt`, object creation, an ordinal scan, or an ObjectKey resolver. Current `object(int)` first calls `requireGeometrySyncAvailable()`, then returns null without a step or uses `object_list_find` with the supplied native number. No extra dimension/existence/generation admission belongs here. No prior filter for zero/negative numbers is specified by this relocation.

The object-number argument is read to enter `object()`; the four double payloads are read only after successful lookup, as today. Keep the snapshot by const reference and do not eagerly unpack it into scalar call arguments. With a found object assign interference `+0x2dc8`, sizeRatio `+0x2dd0`, tolerance `+0x2de0`, then nodeCount `+0x2dd8` as doubles. Do not reorder numerically by offset or add conversions/validation. No pointer/reference to either snapshot or native object survives return.

Missing step/object is a no-op **after** geometry admission. A parked model throws even if the object would be absent. Keep void result and exception propagation; neither bool refusal policy nor `noexcept` is equivalent. There is no transaction or rollback. Existing prior-row writes remain if a later call throws, and the singleton update is not reached; this review does not assert that an inter-row parking occurrence is currently witnessed.

By-number semantics deliberately target the currently present object with that number. Replacement with the same number is not a stale-key refusal. An owned scalar number is not an ObjectKey/incarnation capability; preserving its current lookup semantics neither requires nor establishes global mutation/invalidation instrumentation.

## Both caller orders

Const `applyMeshing3D` retains absent/wrong-dimension, enabled-method and nonempty zero-index guards. Each remembered row is copied locally; only row zero receives the current form conversion/absolute-value/sign/spin projection before its command call. Other rows retain their copied raw double representations. Apply singleton controls only after every row call.

Mutable `applyMeshingSelection3D` retains its model/dimension/method guard, then completes `transitionMeshingObject3D(..., comboObjectIndex, SaveCurrentIndex)` before any native-row loop. Incomplete transition refuses at its existing prefix. On success pass each committed remembered row by reference, with no second conversion of the just-loaded target, then apply singleton controls after the loop. Do not invoke the new command during activation, Fill, or transition merely to eagerly synchronize state.

Replace the two helper calls at those exact points and remove only the unused helper. Other `objectField` uses/includes remain needed. A missing row skips its own write and must not stop later rows or the final enabled-byte store. Empty-row behavior and preflight remain unchanged.

## Tests and exact limits

The existing disabled-remesh tests meaningfully cover selected-first rounded values, unvisited NaN/signed-zero raw bits, unrelated-field retention, missing-middle-object continuation, disabled unused-method behavior, and enabled-invalid-method/wrong-dimension/missing-step caller refusal. The selection tests cover actual combo Save behavior, no double conversion, missing native number with later copies, empty rows, and masked/nonmasked nonfinite profiles. Preserve these tests and their floating-point environment handling. Final state does not independently prove the four-store order; review the moved body for that.

The planned parked viewer direct call with real object number and distinct payload is a useful new-entry guard regression. Keep the existing later state/byte comparisons so refusal proves no writes. It proves direct command admission, not a new whole-Apply exception-prefix occurrence. Do not call parked `pageState() noexcept` or introduce a GUI witness. Configured target names still need ordinary root confirmation; this review did not configure/build/run anything.

CP165 expressly forbids broadening the 0x158-byte temporary record, inferring extra controls, native row allocation/destruction parity, or public-editor parity without its missing same-owner witness. The unchanged four-double projection and named command add none of those claims. The CP128/129 qualifications cited by CP248 remain the retained original evidence; this review confirms the current source relocation and CP165 boundary, not independent rederivation/rehashing of each original helper instruction. Keep public constructor/palette admission, Stopping/cache1, and larger Fill/lifecycle alias qualifications intact.

Only this review report was written. No source, tests, builds, shared recovery state, GUI, original process, or mutable REA state was changed.
