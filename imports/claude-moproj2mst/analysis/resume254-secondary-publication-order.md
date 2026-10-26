# CP254: secondary-model publication before backlink/control work

The canonical empty-secondary path can safely transfer the constructed model into the display **before** its backlink and control queries. The display's existing `secondOwned` unique owner can retain the new model when a later reconstructed geometry guard throws. This matches the original publication-before-query sequence more closely than the current local-owner rollback. Keep constructor failure before the transfer, keep the existing-secondary/missing-main branches, and do not claim original allocator/SEH/Qt-guard failure parity.

This is an implementation-ready bounded order repair for valid synchronous display ownership. It is not an unconditional equivalence claim for manually inconsistent public `DisplayModels` fields or reentrant constructor replacement; those qualifications are explicit below. No source/test, build, shared state, original process, GUI control or mutable REA state changed. Only this report is written; no tests ran.

## Current state and original order

CP252's default correction is implemented and independently reviewed: `displaySecond` constructs with `b->dimensionOr(3)`, preserving live dimensions and using 3 only when the main step is absent. That change deliberately left publication after the control-copy attempt. CP250's named-model leaf and the separate named-map-miss/no-step fault investigation do not authorize changes to this function's unrelated fields or the named path.

Current `source/apps/moproj2mst/src/engine/scene.cpp::displaySecond` (around lines 53–66) performs:

1. Existing secondary guard return.
2. Main lookup and null-main return.
3. Local `unique_ptr<GuiModel>` construction with default-3 dimension.
4. Local new-model display backlink assignment.
5. New control lookup, then main control lookup; conditional GEOTYP copy.
6. `displaySetSecond(d, std::move(m))`.
7. Return current secondary guard.

If the main `simCtl()` throws at step 5, the local unique owner currently destroys the new model, and nothing becomes the display's secondary. That rollback is a consequence of current owner placement, not the original publication order.

Retained original `evidence/gui_mo/s1/mdt/L4/vs/b.c` and the independently decoded complete `0x140cf9990..0x140cf9ac8` region establish:

| Native point | Operation |
|---|---|
| `0x140cf99ae..99b6` | Return an existing second before main lookup. |
| `0x140cf99c1..99c9` | If no main, take common return without construction. |
| `0x140cf99d4..99ff` | Allocate/construct the new model, using original default-3 dimension getter. |
| **`0x140cf9a2f`** | **Publish the new model into display+0x1b8 through `0x140cbc920`.** |
| `0x140cf9a40..9a4d` | Resolve current second and set its display backlink. |
| `0x140cf9a5e..9a76` | Resolve second again and get its nullable control. |
| `0x140cf9a7d..9a8d` | Get current main and its nullable control; null skips copying. |
| `0x140cf9a94..9aac` | Re-get main/control and copy GEOTYP only after nonnull check. |
| `0x140cf9abe..9ac7` | Return current secondary guard. |

The original `0x140c7b2d0` control getter is a raw nullable step/control projection; the converter adds a geometry barrier. Consequently a parked-main exception test demonstrates the converter's retained publication prefix at the corresponding query point, **not** an original geometry exception that was observed or proven to exist.

## Exact bounded source change

Keep `displaySetSecond` itself unchanged. In `displaySecond`, move its call immediately after successful construction; obtain the finite local pointer from the actual display guard after transfer, then perform backlink and control work:

```cpp
auto m = std::make_unique<GuiModel>(nullptr, b->dimensionOr(3));
displaySetSecond(d, std::move(m));
GuiModel* second = dm.second.get();
second->display = &d;
if (auto* sc = second->simCtl())
    if (const auto* bsc = b->simCtl()) sc->geotyp = bsc->geotyp;
return dm.second.get();
```

Preserve the two earlier short circuits and the exact new-control-before-main-control ordering. Do not add a second admission, precheck the main barrier, copy GEOTYP before publication, catch the later geometry error, remove the newly published model on failure, replace the main model, set shown/keep flags, or refresh geometry. The absence of an added fallback/null second branch is intentional: `make_unique` either supplies a complete model or throws, and the existing handoff retains it on the supported fresh-owner path.

The raw `GuiModel* second` is a local observation of the unique owner now stored in `dm.secondOwned`; it does not acquire a native step/object/control or outlive the synchronous operation. Current code has no callback/user dispatch between successful handoff, the backlink assignment and these control queries; `simCtl()` only checks its barrier and resolves the nullable step. Do not generalize that to future added callbacks or concurrent display mutation. Original repeatedly fetches current secondary/main guards; this repair preserves current stable-owner converter behavior and publication position, not every possible native reentrant guard replacement.

## Ownership and exception prefixes

`displaySetSecond` in the same source file currently checks whether a present secondary guard names a foreign/nonowned model, assigns the new guard, then moves its unique owner into `secondOwned`. Its details matter:

- Before the helper call, the local unique owner owns the new model.
- During guard assignment, the by-value helper parameter still owns it. `GuiModelGuard::reset` copies the model lifetime weak identity; it does not transfer allocation ownership or call a user callback.
- During the unique-owner move, the new model remains owned by the parameter or the display; successful completion leaves the display owning the exact published pointer. On the canonical empty-secondary path there is no old secondary owner to destroy.
- The next backlink assignment is a plain nonthrowing field store. Once any later `simCtl()` throws, the display owner remains in place; unwinding a local raw pointer does not delete the secondary.
- A later explicit `displayDropSecond` deletes through `secondOwned`; `GuiModel::~GuiModel` calls `displayForget` while its display backlink is live, clearing matching guards. `Display::~Display` first nulls backlinks on its live model links before owned member destruction, preventing a model from calling into a destroyed display. Retaining the model after a guard exception therefore uses existing teardown ownership rather than a leaked allocation or new lease.

Constructor/allocation exceptions still occur at the same point before publication. The current `GuiModel` constructor creates its lifetime metadata and native step, then initializes GUI controls; if construction throws, ordinary member unwinding occurs before the new call can run. Do not add a nothrow allocation, null-success return, custom allocator handler or fault-injection hook. This placement preserves the converter's existing constructor-throw-before-publication fact; it does **not** prove equality to original constructor/new-handler cleanup.

`displaySetSecond` can itself throw its existing foreign-owner refusal before changing a guard. In a canonical call, `displaySecond` has already returned for a nonnull secondary, and current successful constructor work introduces no app-level display callback, so that refusal is not a new normal branch. If it is reached through externally changed/reentrant display state, the by-value parameter destroys the still-unpublished new model on refusal; the old secondary guard/owner remain untouched. Moving the helper makes that converter-specific refusal occur before backlink/control work. Do not claim preservation of the old exceptional prefix in such interference states, nor attribute this foreign-owner guard to native `0x140cf9990`.

The public `DisplayModels` representation also permits a null secondary guard with a still-live `secondOwned`, or cross-aliasing that old owner as main. Those can be constructed by directly tampering with guards/owners; they are not the canonical original secondary-null state. The helper's unique-owner assignment can delete that old allocation. Moving it earlier could invalidate a cached main pointer in a deliberately cross-aliased fixture. Do not use those states as evidence of native ownership parity, add a new destructive cleanup to “repair” them, or claim the proposed cached pointer is safe under arbitrary mutations. A broad guarantee for them would require a separately defined display-owner invariant and controlled mutation API. This bounded repair is for the valid empty-secondary construction path; root should keep that scope in the implementation review.

## Focused implementation and tests

Production ownership is **`source/apps/moproj2mst/src/engine/scene.cpp` only**. No `engine.hpp`, `gui_model.cpp`, viewer implementation, editor or layout change is needed. Reserve test ownership for the existing `tests/mdt/test_display_selection.cpp` and `tests/mdt/test_viewer_box.cpp` parked fixture; serialize the latter with any other active owner.

1. **Ordinary real caller:** retain CP252's tests of live dimensions 2/3/0, absent-main-step default 3, owner/backlink identity, existing-secondary reuse before main lookup, main destruction, explicit drop and null-main return. Add a live GEOTYP propagation assertion in `test_display_selection.cpp` if not already present. It verifies no successful copy/backlink regression after handoff moves; no special failure fixture is needed there.
2. **Publication retained across the actual geometry guard:** extend `geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass` in `test_viewer_box.cpp`. The fixture already shows the real `model` in `outgoingDisplayProject`, retains the display owner and parks the actual model. Before calling, require secondary guard and `secondOwned` are both empty. Seed a main GEOTYP sentinel **before parking** and preserve the pre-query native bytes as the fixture already does. Call `displaySecond(*outgoingDisplay)` and verify the specific geometry error propagates from the main-control query. After catching it, require a nonnull secondary guard, matching `secondOwned`, backlink to that display and the corrected dimension. Its new native GEOTYP must remain at constructor default rather than the main sentinel; obtain that default from a fresh ordinary model or a source-qualified existing fixture, not a guessed constant.
3. **Reuse and teardown of the partial prefix:** retain a `GuiModelGuard` for the published secondary, call `displaySecond` again and require the same pointer without another main-control query/exception, then `displayDropSecond` and verify the guard expires and both secondary slots are empty. Keep the main model parked and its barrier/step/object/native byte snapshots unchanged. Explicitly dropping this owned secondary is fixture cleanup, not restoration of an installed executable or release of GUI app-control.

The second test fails under current late publication and directly verifies the repaired owner state through the production caller. The repeat-call behavior is native's existing-secondary shortcut: it does not unpark the main or claim GEOTYP repair. The drop checks safe lifetime handling of the retained partial state. Do not call `pageState() noexcept`, introduce a fake simCtl callback or add a production observer to see an intermediate field store.

This compound operation intentionally constructs a new model before its later guard fails; it is not an effect-free rejected getter. The existing viewer fixture compares timer/index-table globals around a block of pure refusals. Place the new caller case after those pure-refusal snapshot assertions, or establish separate legitimate construction baselines, so new-model native initialization is not mislabeled as part of the rejected main query. Keep assertions about the original main's state distinct from constructor/global effects and the newly retained display owner. Do not restore process-global tables inside production to force the older no-effect assertion to pass.

Constructor failure remains source-reviewed unless an existing bounded allocator fixture can reach it without altering this public operation. Do not add a global-new override or new handler solely for this small change; a test of a new fake constructor would not qualify the real route. Tests cannot establish original Qt guard-change/new-handler exceptions or original SEH disposition.

Expected focused group: `m2m.mdt.display_selection` and `m2m.mdt.viewer_box`, confirm configured names. Retain L5 regression in normal integration because its Convert2Dto3D/CopyMirror callers use `displaySecond`. Root owns grouped/full build and source-stable freeze. Use existing Testkit entry points and supported macros, resolvable includes, no C `assert`/`REQUIRE_EQ`, and no external file probe.

## Original qualification and remaining blockers

Read current scene/getter/guard/destructor/display owner code, CP252 default implementation and PASS review, CP252 original named/no-step report and its distinct fault path, and the retained `L4/vs/b.c` complete secondary creator. Original EXE v11.0.2 SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; native `140cf9990..140cf9ac8` byte SHA-256 `635617aa3ec47b312986217bfea91ee855f820285531448a005b9a1b431218ec`. CP252 independently verified that full region using DUMPBIN 14.44.35228.0 and PE section mapping. This design reuses that retained inspection rather than executing the original or opening a mutable analysis engine.

The source ordering—publish, backlink, destination control, source control, copy, return—is closed statically. A candidate parked-main test would qualify the converter's earlier-published partial owner after its own guard exception. It would not witness original geometry admission, physical Qt guard registration, constructor reentrancy, allocation failure, guard-change failure, accepted Close or public lifecycle completion. Those original exceptional cases remain separate evidence work and are not prerequisites for the bounded canonical order repair described here.

The named-display no-step issue stays guarded: it has an unchecked later step-number load and is not repaired by publishing the secondary early. No live ObjectKey, native object-storage mutation generation, geometry reader continuation or cache1 permission is introduced. If implementation cannot preserve the canonical empty-secondary/stable-main scope without claiming behavior for the publicly inconsistent states above, hold that broader claim and design the owner invariant first; do not silently add a new validation/rollback policy.
