# CP168 connector independent plan review

## Decision

**Conditionally source-ready after CP167 is implemented and its acceptance gates pass.** The selected QtCore cut `[0x1311bb, 0x1311bf)` is the four-byte store `mov [rbx+0x48], rdi`. The preceding CP167 cut establishes `RDI = 0` and stores zero to private `+0x40`; the displayed instruction sequence has no intervening write to `RDI` or `RBX`. Stopping at `0x1311bf` excludes the separate `+0x50` store. This supports one new known 64-bit zero observation on the same private object.

The requested checkout's current source already contains CP167's `private40` optional and `BeforePrivateZero48` phase, contrary to the stale source snapshot described by the CP168 note. That establishes a source seam, not CP167 acceptance: I did not run or verify its focused tests, checkpoint regression, or original comparison. The next phase should be a distinct `BeforePrivateZero50` state at RVA `0x1311bf`.

## Instruction boundary and qualification

The cut and byte interpretation are consistent with the cited PE mapping and CP167 predecessor: `[0x1311bb,0x1311bf)` is exactly `48 89 7b 48`. The receiver is `RBX`, retained from the incoming QObjectPrivate pointer, and the source note qualifies it to the same `0x88`-byte private allocation and QtCore module hash. The static/unwind-region evidence supports this selected normal-path effect only. It does not establish a live constructor occurrence, exceptional paths, later stores, or native release behavior.

The note appropriately treats CP167's `+0x40` zero as a required predecessor observation. Do not add `+0x50`, `+0x58`, later container bindings, or inferred register state to this cut.

## Owner, guard, and field representation

Represent native private `+0x48` as a disengaged `std::optional<std::uint64_t>` on `PendingTaskPrototype::SparseQObjectPrivate`. The existing source uses this pattern for `private40`; absence means unknown bytes, and assignment of `uint64_t{0}` records the selected store. Keep the observation on the pending Report's retained private object.

Admission should require the exact `BeforePrivateZero48` phase, unchanged pending Report/private identity and native sizes/address points, written address-point markers, prior `private40 == 0`, and incomplete QObject/base state with backlinks unavailable. Revalidate the existing same-root invariant: the live factory `WidgetRuntimeState` must be the allocation store's associated pointee, and the private `+0x18` QList and `+0x30` QString bindings must match that root's qualified objects, module hash, and symbol RVAs `0x2f0a50` and `0x2f0b30`; both commits must already be marked. Preserve committed deltas without replaying them. The stated `ownsWidgetRuntime` control-block-plus-pointee check is important against aliasing shared pointers.

On success, write only `private48 = 0` and advance once to `BeforePrivateZero50`. On malformed phase, owner, identity, binding, marker, predecessor-field, or completion state, refuse before mutation. Do not change root deltas, mappings, counts, identities, or completion state.

## Focused acceptance

The proposed focused checks cover the central contract. Ensure the positive case begins from an accepted CP167 frame, captures pending/private/root identities and both symbolic deltas, and proves that only `+0x48` becomes known zero while `+0x40` remains zero and `+0x50/+0x58` remain unknown. Verify the new phase and unchanged identities/deltas/counts.

Same-key and different-key reentry should refuse without replaying either field or prior delta and without task/cache/prototype growth. Refusal injections should include missing/expired/foreign and aliasing-foreign roots; wrong or uncommitted bindings/symbols; wrong phase/address point; missing or nonzero `+0x40`; prematurely observed `+0x48`; and completed QObject/backlink state. Inspect retained state after each refusal, and include the real `ConversionSession` ownership pairing. Then run the focused factory/root-owner tests and the established source-stable checkpoint build/regression and original-comparison gates.

No additional live witness is necessary for this one statically qualified store if CP167's source and gates are accepted. This review ran no tests, build, or comparison. Its material prerequisite remains acceptance of CP167, not a new live observation for `+0x48`.
