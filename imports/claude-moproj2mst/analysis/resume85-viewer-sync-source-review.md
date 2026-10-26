# CP85 `viewerSync` count-read review

## Result

`viewerSync` performs its existing model barrier check first, preserves the null-object early return, and then reads `last_object_number` through a short `GuiModelAccess::withStep` callback (`src/engine/viewer.cpp:1021-1029`). The callback ends immediately after assigning the integer count. View allocation/resizing, object-to-view lookup, binding, settings, and refresh happen only after the borrowed step pointer is out of scope. The callback result is `void`; no pointer or reference escapes.

The value is deliberately the **tail object number**, not the counted non-kind-7 object count. The new focused test constructs object numbers 1 through 4 with holes represented as kind-7 entries at 2 and 3; it verifies `objectCount() == 4` while `step_count_objects() == 2`, then verifies `viewerSync` makes four views and leaves the native counts/kinds intact (`test_viewer_box.cpp:415-440`). That preserves sparse-list/view indexing semantics.

## Null, missing-step, and pending cases

- With a normal model and `obj == nullptr`, `viewerSync` returns before `viewerOf` can allocate a viewer or `withStep` can borrow a step. The focused test checks the viewer remains null and the step identity is unchanged.
- With a missing step and null object, the same shortcut remains safe; the test checks no step or viewer appears.
- With a parked model, the barrier check precedes the null-object shortcut. The pending regression checks both a pre-held non-null object and `nullptr` are refused, so the model cannot use a null input to evade the park. It also verifies the DB_STEP bytes and viewer count/identity remain unchanged.
- For non-null object plus missing step, the helper passes `nullptr` to the callback, which maps the tail number to zero; subsequent resize/view lookup behavior is outside the borrow. The test does not cover this combination. It should not be described as a prior behavior regression without the pre-migration body, but it is a narrow nullable-step coverage candidate if that call pattern is supported.

The surrounding `viewerOf` also independently checks availability before lazy allocation. Since the outer `viewerSync` check comes first, neither the nullable shortcut nor lazy viewer creation happens on a parked model.

## Layout and ABI boundary

This change adds no `GuiModel` members and changes no field order or type. `GuiModelAccess` is an internal helper type and the existing public `step` field remains as before. The test's ABI/layout assertions continue to check native `DB_STEP_DATA` size and API signatures; they do not measure `sizeof(GuiModel)` or inspect a built export table. The narrow migration therefore introduces no source-visible layout or export change, while not yet removing raw-field exposure.

No blocking issue found in the count-read seam. Review was read-only; no source/test edits, builds, or test runs were made.
