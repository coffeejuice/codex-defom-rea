# CP244: HeatTreat STEPDEF property write through an owner command

The next bounded reachable property migration is the `STEPDEF` setter/getter pair in `src/model/props_heattreat_slots.cpp`. Both currently obtain a raw `SimControl*` through `withOwnerSimCtl` solely to perform the same one-word write. Move that write into a named `GuiModel` command and retain property conversion, lazy model lookup and returned Variant ordering in the property implementation.

This is new work after CP177's callback conversion, CP233's HeatTreat reader predicates, CP237's FormingExpress flags, CP239's cleanup cache fences and CP243's editor dimension query. Those changes do not already provide this command. It does not depend on unresolved ObjectKey issuance or the loader equipment/effect contract identified by the CP244 loader audit.

Only this report is written. No source/test, build, shared recovery state, original process, GUI control or mutable REA state changed; no tests ran.

## Exact finite operation

Add an internal C++ owner command, proposed name:

```cpp
void GuiModel::setHeatTreatPropertyStepDefinition(int selection);
```

Its complete behavior is the existing nullable guarded write, moved intact:

```cpp
void GuiModel::setHeatTreatPropertyStepDefinition(int selection)
{
    if (auto* control = simCtl())
        control->stpdef = selection == 0 ? 1 : 3;
}
```

Using the existing `simCtl()` **inside** the owner body preserves `requireGeometrySyncAvailable()` before nullable control resolution. The native pointer remains local and does not cross the new API. Return `void`; there is no new success status or admission token for callers to misuse. Do not make the method `noexcept`, normalize selection to an enum, validate its range, create a default step, add native reset/init, or synchronize any other property.

The mapping is exactly zero → 1 and every nonzero integer → 3, including negative and values outside the visible property enumeration. Missing step is a guarded no-op: geometry rejection still occurs first. This differs from CP226's unchecked dimension setter and must not inherit that method's missing-step precondition.

## Property dispatch and order

`HeatTreatProps::applyValue` around lines 221–226 currently stores `b.stpdef = pv::toInt(v)` first, then calls `withOwnerSimCtl` and writes `sc->stpdef`. `getValue` around lines 260–265 performs the same native write before `out = pv::vInt(b.stpdef)`. The getter is intentionally effectful. Do not turn it into a read-only query or make native-to-property synchronization replace this property-to-native write.

Add one local property helper that accepts the already-selected property block by reference, resolves the owner once, and then invokes the command. For example:

```cpp
void syncOwnerStepDefinition(HeatTreatProps& hp,
    const data::HTPropsBlock& block)
{
    if (!hp.owner || !hp.owner->project) return;
    auto* model = engine::opModel(*hp.owner->project, *hp.owner);
    if (model) model->setHeatTreatPropertyStepDefinition(block.stpdef);
}
```

Replace only the two STEPDEF callback invocations with `syncOwnerStepDefinition(*this, b)`. Keep the converted property assignment before it in `applyValue` and the `out` assignment after it in `getValue`.

Passing the block reference, rather than copying `b.stpdef` before owner resolution, preserves the important lookup/read order: `engine::opModel` may invoke `classModel`, install loader slots or lazily create a model; the old callback reads the selected block after that lookup. `src/engine/loader.cpp::opModel` around lines 168–179 establishes those possible effects. Do not use a noncreating cache presence check or `hp.owner->engineModel` in place of the actual provider.

After model lookup, the proposed helper copies the ordinary integer argument immediately before entering the owner's guard. The old callback copied that integer immediately after the guard. `requireGeometrySyncAvailable()` currently only inspects the barrier and may throw; it runs no user callback and mutates neither block nor model. This scalar read has no observable effect in the supported synchronous state, so the native/property effect prefix is unchanged. This reasoning does not promise behavior under concurrent property mutation, dangling blocks or undefined behavior. If the guard gains callbacks or block mutation in a future change, re-audit this boundary rather than relying on this local equivalence indefinitely.

Preserve these reached cases:

| State | Required behavior |
|---|---|
| Missing property owner/project | No model lookup or native write. Setter still retains its earlier converted block value; getter returns the block value afterwards. |
| Owner provider returns no model | Exactly one provider lookup; no native write; same property/output behavior. |
| Existing model, absent step | Geometry guard runs; skip only native word write; preserve block/output behavior. |
| Matching live step | Write only `stpdef` with the zero/nonzero mapping. |
| Geometry parked | Setter's earlier block conversion/store remains; native word is untouched and getter output is not assigned after the guard throws. No rollback. |

Keep `withOwnerSimCtl`, `setScWord`, `setMode` and `getMode` unchanged: the Mode branches still use that callback seam, include default/refresh work, and require their own migration. Do not claim this translation unit is callback-free after two sites move.

## Bounded ownership

Reserve one common source/API owner for:

- `source/apps/moproj2mst/include/m2m/engine.hpp`: declare the named value-input command beside other finite model commands.
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: define the guarded nullable single-field write.
- `source/apps/moproj2mst/src/model/props_heattreat_slots.cpp`: add the local model/provider helper and migrate exactly the two STEPDEF sites.

The header/model body are shared integration files; root must serialize their edits with any active editor or loader owner. No `loader_l3.cpp`, editor page types, `simcontrol300_owner.cpp`, Operation cache API, property layout, native layout or C ABI change is required. Existing headers already resolve `HTPropsBlock`, `GuiModel` and `engine::opModel`; the property helper requires no new include. Keep `<functional>`, type traits and native control includes because the remaining Mode helper still needs them.

## Meaningful tests and preflight

The existing caller-level test is `tests/mdt/test_l6_slots.cpp::heattreat_property_simcontrol_writes_and_null_step`, expected target/test `m2m_mdt_l6_slots` / `m2m.mdt.l6_slots`. It creates the copied-corpus reclassified HeatTreat property owner, loads the actual model, and already exercises both migrated dispatch sites:

- Setter selection 0 writes native 1; setter selection 2 writes native 3, while retaining each block value.
- Getter has its block changed back to 0 before invocation, so its native write is observable and the returned Variant kind/value is checked.
- With the actual owned step moved into `savedStep`, setter/getter retain property/output behavior and leave the saved native sentinel untouched. Both normal and exceptional paths restore the owned step.

Retain that case and all its Mode assertions. This tests reachable property dispatch, not only a new direct command. Extend it narrowly with an ordinary nonzero getter case if desired: seed the native word with a different sentinel, set `block->stpdef` to a negative value, call `getValue`, and assert native 3 and the unchanged negative returned/block selection. That covers the exact nonzero policy without adding a synthetic enumeration restriction.

The guard belongs to the new public C++ command, so add one direct refusal assertion to the existing parked-model block of `tests/mdt/test_viewer_box.cpp::geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass`, beside existing refused mutating commands. Its later byte/state comparisons already establish no native mutation. This is a one-line extension of an existing meaningful fixture; do not build another geometry fixture or use a real GUI session. It does not by itself test the property setter's pre-guard block store; source order and the caller test qualify that prefix. If root requires that exact parked property-prefix case dynamically, assign it separately rather than claiming the direct command test proves it.

Root's minimum focused group is `m2m.mdt.l6_slots` and `m2m.mdt.viewer_box`, confirming configured names. Keep `m2m.p15.heattreat_tree` / the existing property suite and `m2m.mdt.simcontrol300_stopping_cached` in normal integration regression; their passes would not establish this particular callback body. Run the full source-stable launcher and freeze at the usual integration boundary. No result is claimed here.

Use the existing Testkit main and Release-active `CHECK`, `CHECK_EQ`, `REQUIRE`; no C `assert` or `REQUIRE_EQ`. The focused files already resolve their includes and need no new registration. Re-run binding/quoted-include preflight and source ownership checks before compiling. No missing-file probe, installed program or external project mutation is needed.

## Source qualification and remaining boundaries

Reviewed current property helper/STEPDEF branches, `GuiModel::simCtl`/geometry guard, `engine::opModel`, existing L6 test, CP177 next-cut/implementation review, CP222 property alias ledger, CP225 object-mutation audit, CP239 cleanup migration and current CP244 loader assessment. CP177 already changed a raw-return helper to the current synchronous callbacks. The new work removes the native pointer from these two property callbacks through a named command; it is not reassignment of the completed callback conversion.

The property source annotates original setter/getter addresses `0x1411c2190` / `0x1411c2300`. `source/docs/re/moproj2mst/moproj/props_rest_ledger.tsv` records both as HeatTreat set/get slots implemented in this translation unit; `owners.tsv` assigns the same entries. The translation-unit header identifies retained `gui_mo/s1/moproj_rest/hta.c`, `htla.c`, `tpla.c` provenance. These are historical static/ported-source qualifications, not a fresh original execution or a complete independent verification of those bodies in this audit. The new API is a reconstructed ownership boundary, not an asserted original C++ method.

CP177's source review expressly qualifies the L6 test as an internal copied-corpus/null-step fixture, not public Open or original occurrence. Preserve that qualification. The current source establishes the mapping and effect order this proposal moves; the new tests would qualify reconstructed dispatch/guard behavior only.

This command mutates one field in an existing object graph. It performs no create/remove/recreate, dimension replacement, KEY/DB reload or identity issuance. Under CP225's contract, ordinary in-place control mutation does not retire object incarnations. Live ObjectKey/currentness, hidden native replacement routes, model/cache privacy and retained viewer/editor pointers remain unresolved independently. `withOwnerSimCtl` still exists for Mode callbacks; direct HeatTreat reader/default/control aliases remain elsewhere. No closure, native accepted Close, R300 cache1 continuation or original-witness gate is advanced by this leaf.
