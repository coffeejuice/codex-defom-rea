# CP246: remaining HeatTreat Mode writes through named model commands

The next ready property migration is the shared Mode word-write seam for Deformation, Diffusion and PhaseTransformation in `src/model/props_heattreat_slots.cpp`. CP244 moved the STEPDEF pair; the two remaining `withOwnerSimCtl` consumers are now `setMode` and `getMode`, which implement six reached property slots. Move their finite word writes into three named bool-input `GuiModel` commands, preserving defaults, property snapshots, lazy owner lookup, native guard and refresh/output order.

This avoids CP245's `simcontrol300_owner.hpp`, `simcontrol300_owner.cpp` and `test_simcontrol300_reaction_rate.cpp`. It requires shared model files, so root must reserve/serialize those against other model owners. Only this report is written; no source/test, build, shared state, original process, GUI control or mutable REA state changed, and no tests ran.

## Exact command surface

Add three domain-specific commands, proposed names:

```cpp
void GuiModel::setHeatTreatPropertyDeformation(bool enabled);
void GuiModel::setHeatTreatPropertyDiffusion(bool enabled);
void GuiModel::setHeatTreatPropertyPhaseTransformation(bool enabled);
```

Each body calls `simCtl()` once, conditionally writes its one integer word if nonnull, and returns void. Map fields exactly:

| Command | Existing native offset | Typed layout field | Written value |
|---|---:|---|---|
| Deformation | `SC+0x108` | `trans[1]` | `enabled ? 1 : 0` |
| Diffusion | `SC+0x110` | `trans[3]` | `enabled ? 1 : 0` |
| PhaseTransformation | `SC+0x10c` | `trans[2]` | `enabled ? 1 : 0` |

`source/port/defmodel/include/defmodel/layout/cls_1401e7b10.hpp` places `int32_t trans[5]` at `0x104`, supporting those exact indices. Preserve explicit int conversion rather than storing a byte bool. Do not add a public arbitrary-offset/index setter, raw control argument, native callback or resolver. Three short named commands avoid exposing unsupported modes or needing a public integer-selector error policy.

As in CP244, `simCtl()` retains geometry rejection before nullable step/control lookup. Missing step remains a guarded no-op. Do not make these commands `noexcept`, skip the guard, create storage, modify defaults or refresh properties. The pointer stays local to the owner body; no raw pointer is returned or delivered to a property callback.

## Property implementation and strict sequence

Introduce a private translation-unit discriminator such as `enum class HeatTreatMode { Deformation, Diffusion, PhaseTransformation };`. Replace the integer offset argument to local `setMode` / `getMode` with that discriminator. Their six `applyValue` / `getValue` calls currently pass `0x108`, `0x110` and `0x10c`; change only those constant arguments to the matching private value.

Use a private `syncOwnerMode(HeatTreatProps&, HeatTreatMode, bool)` helper. It returns immediately when owner/project is missing, otherwise calls `engine::opModel(*hp.owner->project, *hp.owner)` exactly once; if a model exists, select the matching named model command. The discriminator has only these internal constant call sites; do not turn it into a generic public property-command dispatcher or introduce an invalid-selector exception branch in the reached path. No native pointer enters this helper.

The bool parameter must be copied **before** calling `opModel`. This differs from CP244 STEPDEF: the existing Mode lambdas capture `[scOff, flag]` by value before `withOwnerSimCtl` resolves the model. The getter already receives `flag` by value. Do not pass a property flag reference and reread it after `opModel`; that provider may call class/model creation code. Preserve the old snapshot even if provider callbacks modify property state.

Keep `setMode` effects in this exact order:

1. If data exists and schedule defaults are not applied, call `ht::engineDefaults(ownerProject(hp), hp.data->schedule)`.
2. Assign the passed property flag with `pv::toBool(v)`.
3. Copy mode/flag values and synchronize that one native word through the property helper/model command.
4. Only afterwards call `ht::propsRefresh(ownerProject(hp), hp)`.

Do not combine default application or refresh into the new model command. Both have separate owner/lookup effects; `ownerProject`'s later refusal remains after the attempted word write. A geometry error in step 3 leaves any earlier default/property effects in place and prevents refresh, just as today. A missing control skips only the native word; the later property refresh still runs. An ownerless getter is a null-owner no-op followed by its return; an ownerless setter can still fail at one of its existing `ownerProject` calls. Do not make the whole Mode setter a silent ownerless no-op.

Keep `getMode` order as copied input flag → one owner-provider lookup → model geometry guard/nullable word write → `pv::vBool(flag)` result construction. The getter intentionally writes native state. Do not read the native word into the property or move result construction before the command.

After both consumers migrate, `withOwnerSimCtl`, `setScWord`, and their control-type alias become unused in this translation unit. Confirm with scoped search, then remove them and only the now-unused includes (`functional`, `type_traits`, `utility`, native control layout, and potentially `cstddef`/`cstdint` if no remaining use). Avoid speculative include pruning: compilation must resolve all surviving names through their own needed headers. The CP244 `syncOwnerStepDefinition` and all schedule/tree code stay unchanged. This batch closes the local raw-pointer callback seam rather than adding a second forwarding wrapper beside it.

## Bounded ownership and tests

One implementation owner needs these production files:

- `source/apps/moproj2mst/include/m2m/engine.hpp`: three named command declarations.
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: three finite guarded nullable writes.
- `source/apps/moproj2mst/src/model/props_heattreat_slots.cpp`: private mode discriminator, model-provider helper, two shared Mode consumers and six constant dispatch arguments; remove the unused native callback machinery.

Test ownership is the existing `tests/mdt/test_l6_slots.cpp` plus the established parked-model block in `tests/mdt/test_viewer_box.cpp`. No editor-owner, loader, native-layout or C ABI file changes are required.

Extend `heattreat_property_simcontrol_writes_and_null_step` in the L6 test. It already uses the real property owner/provider on a copied-corpus reclassified HeatTreat operation, exercises Deformation setter/getter, and checks Mode getter behavior while the actual owned step is detached. Add a small table over the three names/block flags/native offsets:

- On each live setter, establish that schedule defaults are already applied (the loaded fixture supplies this state), seed distinct native word sentinels, deliver a bool and assert the matching block/native word and unchanged other two native words. This prevents a Diffusion/PhaseTransformation index swap from passing Deformation-only tests.
- Before each getter, change the matching block flag independently of the native word; assert Bool Variant/result and the native word now equal the copied property value. Check the other native words remain unchanged.
- During the existing detached-step interval, call all three getters with known property values and verify returned values plus untouched saved native sentinels. Preserve restoration on normal and exceptional exits. Existing STEPDEF checks remain intact.

Do not claim that a test with defaults already applied proves the first-default path. Retain `heattreat_engine_defaults` and `heattreat_schedule_flag_set` as surrounding regressions; source review establishes that the preexisting defaults and refresh calls stay at their positions. If a new test sets `schedule.engineDefaults=true` merely to bypass a failure, identify that as a separate synthetic fixture rather than the loaded caller test.

Add one direct refused-call assertion per new command in `test_viewer_box.cpp::geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass`, adjacent to CP244's STEPDEF command refusal. Its subsequent byte/state checks already cover no native write. These tests qualify direct admission, not dynamic observation of every property-prefix effect. Do not invoke `pageState() noexcept` or use CP245's editor fixture for this property work.

Root's focused group is expected `m2m.mdt.l6_slots` and `m2m.mdt.viewer_box`; confirm names in configured metadata. Keep ordinary HeatTreat tree/property and Stopping regressions in integration, then run the complete source-stable launcher/freeze. Existing Testkit main, `CHECK`, `CHECK_EQ`, `REQUIRE` and CMake registration suffice; no new test file or external probe is needed. Testkit preflight must check every changed include/name and avoid C `assert`/`REQUIRE_EQ`.

## Source and original qualification

Reviewed the current property callback/template/Mode sites, CP244 implemented STEPDEF command and review, current `simCtl` guard and `opModel` provider, CP177 callback-cut contract/source review, existing L6 tests, CP222 property ledger, CP225 object-mutation audit and CP245 reaction-rate contract. CP244's implementation receipt explicitly leaves Mode callbacks unresolved; both remain present in current source. This is a new named-command migration, not a repeat of CP177's completed raw-return-to-callback change.

The property source annotates setters `0x1411c1c30`, `0x1411c1dc0`, `0x1411c1f50` and getters `0x1411c1d20`, `0x1411c1eb0`, `0x1411c2040`; `source/docs/re/moproj2mst/moproj/props_rest_ledger.tsv` records these six slots against the same implementation. The source header gives retained `hta.c`, `htla.c`, `tpla.c` decompile provenance. Those historical static/ported-source records support attribution, but this audit does not independently replay original bytes or provide fresh original runtime evidence for the six slots.

CP177's review already distinguishes reconstructed owner/provider/null-step tests from public Open or original execution. Preserve that limitation. The proposed commands are reconstructed ownership APIs, not claimed original C++ interfaces. Exact word mappings, capture points and effect order above are current-source facts; a passing candidate would still not prove complete HeatTreat lifecycle fidelity or actual Qt signal delivery/reentrancy.

Only existing control fields change. There is no object create/remove/recreate, dimension/storage replacement, reload, identity issuance or generation change. CP225's live ObjectKey blockers are therefore outside this leaf. Other HeatTreat reader/default/control aliases, public native model storage, Operation cache ownership, retained viewer/editor aliases, cache1 refusal and separate original-witness gates remain unchanged. Closing this local callback template does not establish whole-model ownership closure.
