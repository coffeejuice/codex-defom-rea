# CP250: named-display model dimension through the existing owner query

A small independent source migration is ready in `src/engine/scene.cpp::displayNamedModel`: replace its raw main-step dimension read with the existing unguarded `GuiModel::dimension()` value query, while retaining the preceding explicit main-model/step refusal. This is one new named-model creation consumer; no common model/header, property, viewer implementation or CP249-owned test edit is required.

The larger geometry continuation and whole named-model initialization remain dependent on owner/effect contracts or original witnesses. This report recommends only the scalar owner observation, not a new geometry branch or full display/model encapsulation.

Only this report is written. No source/test, build, shared recovery state, original process, GUI control or mutable REA state changed, and no tests ran.

## Exact finite edit

Paths below are relative to `source/apps/moproj2mst/`.

`src/engine/scene.cpp::displayNamedModel`, currently around lines 25–34, first searches the existing named list. A matching name returns that holder's model immediately. On a miss it resolves `displayMain(d)`, then performs:

```cpp
if (!b || !b->step)
    unsupported("named model without a main model (0x140c7fee0 dereferences it)");
const int dim = b->step->dimension;
```

Keep the `if` and exact error unchanged; replace only the second line with:

```cpp
const int dim = b->dimension();
```

The source method currently returns `step ? step->dimension : 0`, unguarded and `noexcept`. The explicit preceding check establishes the present-step branch in valid synchronous state, so the new call reads precisely the same integer field. `DB_WRITE_VAR::dimension` is an `int32_t` at offset `0x1c` in `source/port/defmodel/include/defmodel/layout/db_write_var.hpp`, and the native step inherits that base at offset zero. No dimension normalization, identity issuance or native pointer is returned.

Do not remove the preceding `!b->step` check by treating dimension zero as “no step.” A present step with dimension zero or another arbitrary integer previously reached construction with that exact integer. Likewise, `dimensionOr(sentinel)` cannot prove absence when the same integer could be a present native dimension. Retaining this raw **presence** check is intentional staging; the leaf removes the actual dimension field read, not every use of public storage.

Do not use `simCtl()`, `simGeometryTypeIf2D()`, `setDimension()` or an admitted callback instead of `dimension()`: that would add an earlier geometry refusal or change the operation. The scalar read is unguarded today. Later named-model initialization already obtains guarded controls, and any existing later refusal/effect prefix must remain at that later point.

## Preserve the creation/lookup sequence

The complete reached source order must remain:

1. Obtain the display's existing `DisplayModels`; search named holders in current order.
2. On a matching name, return its stored model pointer immediately, including an existing null holder; do not resolve or validate the main model first.
3. On a miss, resolve the guarded main model and retain the existing explicit main/step error.
4. Read its dimension through the value query.
5. Construct the new `GuiModel(nullptr, dim)`.
6. Assign ROI/Coupons role (`step->u3400`), display backlink, native step number and title in their existing order.
7. Acquire new-model control and then main-model control as currently written; copy GEOTYP only when both exist.
8. Insert the owned named model in name order and return its pointer.

Only step 4 changes representation. Do not move main validation before the existing-name return, reorder display/backlink or native field writes, replace the raw returned model with a new lifetime scheme, or add geometry work. Keep `displaySecond` unchanged: it already uses `guiModelDim(*b)` and is not a second new dimension migration.

This retains the same supported synchronous assumptions as both expressions. It does not make concurrent `step` replacement, dangling model pointers or undefined layout mutation safe. The copied `dim` remains a value used immediately for construction; no address/token gains later authority.

## Exclusive files and meaningful test seam

Production ownership: **`src/engine/scene.cpp` only**. It already includes `m2m/engine.hpp`; no new quoted include, public API, layout, C ABI or registration edit is needed. Existing raw step/control initialization elsewhere in the function remains and should not be relabeled closed.

Focused test ownership: **`tests/mdt/test_display_selection.cpp`**, independent of CP249's `engine.hpp`, `gui_model.cpp`, property source and `test_viewer_box.cpp`. Existing `selection_named_then_third_then_main_then_second_preserves_other_flags` already calls `displayNamedModel("ROIs")` and exercises later display ownership/selection. Existing null-named-holder and secondary-lifetime tests cover nearby model-owner behavior, but do not explicitly check the copied dimension.

Add one narrow named-creation case or extend the existing selection fixture:

- For main models of dimension 2 and 3, call `displayNamedModel` for a new name and assert the returned model's `dimension()` equals the main model's exact value, its display backlink is correct, and repeated lookup returns the same pointer without creating another holder. Keep the returned model managed by its actual named holder.
- Verify existing-name lookup remains ahead of main validation: after establishing a named model, detach/reset the main model's actual step, then show that the same-name lookup returns the existing holder while a new-name lookup takes the exact existing unsupported error. This checks the **current converter's adapter refusal**, not original no-step parity: the retained native getter defaults to dimension 3 without a step. No dead native pointer should be dereferenced. The main model can remain step-less until ordinary fixture destruction; do not close or control an application.

Those are caller-level tests of the selected creation and short-circuit boundary, not an accessor-mirroring unit test. A no-main fixture can additionally check the same error if needed, but do not build a second broad scene suite. There is no need to manufacture a parked geometry state in a new fixture: source comparison establishes no added guard at step 4, and current tests of `dimension()` already qualify unguarded parked observation. Do not claim a plain named-model test dynamically proves the later parked-control failure prefix.

Expected target/CTest: `m2m_mdt_display_selection` / `m2m.mdt.display_selection`; confirm configured metadata. Retain its Testkit main and supported `CHECK`, `CHECK_EQ`, `REQUIRE`; no C `assert`, `REQUIRE_EQ` or missing-file probe. Root owns focused/full validation and source-stable freeze. This report provides no new candidate result.

## Original attribution and bounded equivalence

The scene source annotates `displayNamedModel` as GUI `0x140cfa0e0` and the scalar path as `0x140c7fee0 -> 0x140100980 (+0x1c)`. `include/m2m/engine_scene.hpp` records named-model construction from the main dimension, role values, step number/title and GEOTYP copies. Both source/header identify retained `gui_mo/s1/mdt/L4/vs/a.c`, `b.c` decompile provenance.

The independent CP250 review establishes a necessary distinction from the retained native decompiles: `c.c` shows `0x140c7fee0` initializing the result to **3**, then replacing it with `0x140100980(step)` only when a step exists; `g.c` shows that live-step getter reading `+0x1c`. The original getter therefore returns dimension 3 without a step. Current `displayNamedModel` instead explicitly refuses a main model with no step, and the reconstructed `GuiModel::dimension()` would return 0 if called on that state. This proposal preserves the converter's existing refusal as adapter behavior; it does not establish native no-step equivalence or repair that discrepancy. Its source-equivalence claim is confined to the present-step branch after the retained converter guard. The native existing-name-before-creation order remains separately supported by retained `a.c`. These are static decompile qualifications, not fresh original execution.

`source/docs/re/moproj2mst/mdt/L4/spec.md` section 9 ties the named ROI model to ShapeOptim's reached load/save path. It records historical Oracle-B 108 for that broader path, but that result is not a new dimension-specific original witness and does not qualify every named/failed/parked state. The new owner-method call is a reconstructed C++ boundary, not a claimed original API signature.

`analysis/resume222-viewer-alias-ledger.md` explicitly identifies named/main/second model retention and `displayNamedModel`'s returned pointer as remaining lifetime work. `resume173-r300-closure-audit.md` and the CP218 inventory list direct scene accesses. This one read is still present after CP248 in the inspected source, so it is a new consumer migration rather than repetition of CP224's viewer helper or CP233's HeatTreat predicates. The equivalence here is established by current source/layout and retained static attribution; no new original execution, native byte rehash or whole ShapeOptim parity claim is made.

## RGEOM / ROPS reconciliation and larger blockers

The active RGEOM cursor and `analysis/resume214-geometry-next.md` still require an actual optional cross-section/mesh owner writer or qualified positive null-owner path, matching getter results reaching `0x14124bec0`, plus a real project/effect occurrence. Synthetic `test_viewer_box` arrays and the main FE-object allocation path do not supply that optional owner. Keep those populated/cached/public geometry guards unchanged. The named-model dimension observation neither enters those readers nor manufactures their witness.

The active ROPS cursor still requires original directory-call/QDir/modal continuation evidence before public directory effects and retains the full temporary KEY/codec/scanner and Type3 owner boundaries. CP247's Forming success order and CP248's meshing writeback are separate implemented finite cuts. The present leaf changes no operation directory, file effect, Forming failure prefix or meshing field.

Larger independent alternatives are not ready under this task's file restrictions:

- The rest of named-model initialization needs a finite two-model owner operation for step/title/GEOTYP copying, preserving allocation and later guarded-control effects. It would require a reviewed common model API or separately owned domain coordinator; do not migrate those raw writes as an incidental follow-on.
- `geometry_temp_prefix.cpp` still retains raw step/reader identities and escapes a callback pointer during preparation. Replacing them with CP222 StepIdentity alone cannot certify reload/recreation under the public-storage bypasses identified by CP225; frame/reader/current-object and sealed-receipt ordering remain independent dimensions. Do not claim live-currentness from metadata or repeat the blocked ObjectKey proposal.
- Operation cleanup's remaining cache observation is a **weak** owner observation in current `OperationInfoCleanupProgress`, alongside an inert address token and owner-equivalence checks. Some older summary wording calls it strong; current source/header control that distinction. Returning another shared owner or collapsing identity to a bool would not close that seam. CP239's four simple presence fences are already migrated.

This is a small staged value-read migration with a defined caller test, not ownership closure. Raw named-model/control initialization, public aggregate storage, returned model pointers, geometry identities and native same-occurrence witnesses remain separate work. No cache1/Stopping or lifecycle acceptance gate is advanced.
