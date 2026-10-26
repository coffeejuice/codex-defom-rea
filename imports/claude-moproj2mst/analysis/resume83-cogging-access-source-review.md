# CP83 `deleteObject` access-layer review

## Result and guard order

`deleteObject` now enters `GuiModelAccess::withStep` before argument validation, object-count lookup, native deletion, PDIE checks, or repick. The callback uses its local `nativeStep`; it does not re-fetch `w.step`. A parked model therefore refuses even for no-op inputs such as `n <= 0`, missing object numbers, or a null step, before the callback can observe or mutate native state. `repickPdie` is independently guarded and also uses its local step pointer. Its calls to guarded `w.object()` and `w.setPdie()` are nested operations under the same synchronous callback; no pointer escapes through the void callback result.

This closes the prior partial-delete case where the native deletion occurred before a later `setPdie` guard could throw. It does not revoke previously returned raw object or step pointers, and those fields/accessors remain public; the migration only guarantees the named `deleteObject`/`repickPdie` operations enter through the barrier.

## Preserved deletion and PDIE behavior

The source retains the established sequence:

1. Ignore nonpositive object numbers and models with no counted objects.
2. Call `step_delete_object` on the native step, with an initialized `nullHit` flag.
3. If the low-level tail trim reports an impossible missing object slot, throw `M2M_E_UNSUPPORTED` before PDIE repair.
4. Only if the requested number was the current PDIE, clear it to zero, then call `repickPdie`.

The low-level delete preserves the original native behavior: a missing object is a no-op; a found object has references/pairs cleared, becomes kind 7, has its optional `u3bf8` payload's `vslot1_0` called when non-null, and empty kind-7 objects are trimmed from the tail. The migration does not reorder or bypass that native call. The separate `step_delete_object` unit tests cover missing, middle, and tail cases, pair cleanup, and `null_hit` when trimming encounters an absent slot (`port/defmodel/tests/test_gui_twins_delete.cpp:183-215`).

PDIE repick rules are also unchanged: fewer than two counted objects chooses the first real object; exactly two chooses the first rigid object, falling back to first real when none is found; above two retains a valid current non-kind-7 PDIE, otherwise chooses a rigid object and falls back to first real if needed. `direct_repick_pdie_preserves_count_rigid_and_fallback_rules` covers these branches, including missing/current kind-7 cases (`test_cogging_save.cpp:213-250`). `delete_object_repicks_pdie` covers no-op n=0/missing number, deleting the current PDIE with two-then-one survivors, and the single-kind-7 no-count-change case (`183-211`).

The optional `u3bf8` payload branch remains source-visible but neither the wrapper test nor low-level deletion test constructs a payload with a valid virtual dispatch target. That is an existing focused-test gap; constructing an arbitrary fake target would be unsafe. The `nullHit` refusal is tested at the low-level trim helper, while the wrapper's post-delete error translation is not separately forced through a malformed `GuiModel` fixture. Neither gap indicates a change in this migration's ordering.

## Parked and ordinary cases

The CP80/CP81 pending-model test now calls `deleteObject` for zero, missing, PDIE, and non-PDIE numbers, and separately calls `repickPdie`, expecting each to refuse. It compares full `DB_STEP_DATA` bytes, object count, PDIE, state, viewer, and associated state snapshots afterward (`test_viewer_box.cpp:1930-1950`). The ordinary cogging tests exercise both delete and direct repick on unparked models. This is sufficient source-level evidence for the earliest-effect gate and routine compatibility; no test was run as part of this review.

No blocking defect found. No source/test edits, builds, or tests were made.
