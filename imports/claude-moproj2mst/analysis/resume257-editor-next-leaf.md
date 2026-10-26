# CP257 next finite Editor300 owner leaf

The next source-ready leaf after the active SPRING label migration is the **final Advanced Apply state remap**. Move the existing fixed-false native call from `applyAdvanced3D` into one named GuiModel command. This removes one direct native-step argument from the editor Apply caller and preserves the already-implemented remap behavior; it does not require cache1 admission, broader meshing or a GUI witness.

Only this report is written. No source/test/build, shared recovery state, original process, GUI or mutable REA state changed.

## Current-source reconciliation

CP253's initial admission now uses model.dimension(). CP255 owns the SPRING producer migration; its in-progress source must not be reassigned or treated as absent merely because an intermediate read still contains the old helper. Title/frequency/StepCount commands, current-control identity queries, reaction coordinator, meshing writeback, heating-unit and MultiBlow prologue cuts remain distinct completed or reserved work.

Current `src/engine/simcontrol300.cpp::applyAdvanced3D` still ends with:

```cpp
for (int i = 0; i < 3; ++i) sc.elmnod[i] = form.elementNodeOptions[i];
sc.pdie2 = form.secondaryDieNumber;
def::model::step_remap_material_state(*model.step, false);
return ApplyResult::Completed;
```

The existing editor alias ledger calls for finite model-owned operations instead of public native-step access. This is a single existing native operation with a fixed argument and no returned borrow. It is smaller than migrating the whole Advanced page, its secondary-die object scan, recovery resizing or the lifecycle backup owner/copy sequence.

## Exact command contract

Proposed internal C++ API:

```cpp
void GuiModel::remapEditorAdvancedState();
```

Exact body:

```cpp
def::model::step_remap_material_state(*step, false);
```

Declare beside other named model commands; implement in gui_model.cpp with the existing native step declaration/header available. Reserve the shared files after CP255 ownership release and its source-stable integration/freeze. No generic bool selector, arbitrary native callback, raw input, return alias, new value snapshot or C ABI is needed.

The method requires the existing live step and must remain **unguarded and not noexcept**. The old final call dereferences model.step directly; adding simCtl, matchesCurrentSimControl or requireGeometrySyncAvailable here would create a new refusal after earlier page stores. Adding missing-step no-op or a success/failure result would likewise redefine its unchecked live-step contract. Do not exercise an absent step as a graceful-failure test. Native remap may allocate/rebuild arrays; do not add noexcept or catch/rollback allocation failures.

The preceding existing advancedApplyProfile still admits the current live 3D control before page mutation. This leaf does not change that preflight or establish safety under concurrent/reentrant replacement. There is no new callback introduced between admission and the final command.

Replace only the final native call with `model.remapEditorAdvancedState()`. Keep it after all three ELMNOD stores and PDIE2, after earlier time/geometry/string/output-flag effects, immediately before Completed. Every existing early ValidationStopped/UnsupportedSnapshot return must continue skipping remap. Do not hoist it into preflight, call it after each store, merge it into fillAdvanced3D or move it into the separate recovery-width routine.

Inside the existing native false-remap body, state widths are rebuilt and each eligible current object array is remapped using old/new widths. That behavior is already implemented, not a request to rewrite or optimize it. Keep false fixed; true additionally snapshots/remaps material-number state and is outside this event's selected operation. Existing native backlink/array ownership remains in its native implementation. No native object, array or step pointer crosses the new public method boundary, but storage reallocation can still invalidate prior interior aliases elsewhere; this is not model ownership closure.

## Actual caller tests

The strongest existing tests are unconditional cases in `tests/mdt/test_simcontrol300_snapshot.cpp`:

- `advanced_invalid_returns_keep_exact_earlier_stores_and_skip_final_remap` seeds state width 1234, supplies invalid TNOW2 or later GEOERR, and verifies the exact earlier field prefix plus unchanged width. This meaningfully catches accidental eager remap.
- `advanced_final_remap_rebuilds_widths_and_allocates_live_object_state` allocates two node records with distinctive payload, fills remembered ELMNOD options, changes the live option/width, calls the actual applyAdvanced3D, then requires rebuilt widths 3 and 1, retained node count/payload and newly allocated zeroed state-array storage. This catches an omitted command or a no-op wrapper. The fixture does not necessarily distinguish true from false when material-number remapping prerequisites are absent; the fixed false argument must be checked in source.
- Existing Advanced source-field/flag/secondary-die tests and full lifecycle/recovery-tail tests retain surrounding effect and repeated-Apply regression. They do not prove native Qt delivery or every intermediate ordinary store.

Do not replace these caller tests with only a direct empty-model command test. A new direct test is optional because existing actual-caller tests already observe the intended remap and skip boundaries. If extending the success case, seed any additional checked field with a value differing from its expected result; avoid default-state assertions that pass when the call is omitted. Keep object/node fixture allocations alive while inspecting their state and reacquire reallocated array pointers after Apply as the existing test does.

Do not add a viewer parked-refusal assertion for this command: it has no new geometry guard by contract. A direct parked call could mutate array state and would test a different operation from admitted Advanced Apply. Source inspection establishes the preserved unguarded final call; the existing Advanced preflight and viewer admission tests remain separate. No new production observer should be added to prove order among ordinary stores.

Ownership is engine.hpp, gui_model.cpp and simcontrol300.cpp; optional test edits only to test_simcontrol300_snapshot.cpp. These common files must not overlap CP255. No lifecycle/Spring producer, reaction owner, meshing writeback representation, native layout, CMake registration or public ABI change is required. Existing includes should resolve the native remap declaration; confirm actual compile include paths before changing headers.

Root focused group should include `m2m.mdt.simcontrol300_snapshot` and the lifecycle/recovery-tail regressions that invoke Advanced, with normal stopping_cached integration and complete source-stable launcher/freeze. Confirm configured target names; expected main target is `m2m_mdt_simcontrol300_snapshot`. Use existing Testkit assertions/entry points; no C assert, REQUIRE_EQ or external probe. No build/test result is claimed here.

## Original evidence and qualification

Read retained `analysis/wave2-save-diffs-300-advanced-contract.json` and `analysis/wave2-save-diffs-300-advanced-twin-comparison.json`. The contract attributes Advanced Apply to original `0x140ec7500`, and its final operation to GUI `0x14082a360(step,0)`, corresponding to native `0x1401d0fc0(step,false)` after the prior page stores. The comparison records 268 normalized instruction shapes for GUI range `[0x14082a360,0x14082a9f9)` and native `[0x1401d0fc0,0x1401d1659)`, with paired external targets explicitly not proved transitively equivalent.

Original GUI identity is DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; the retained comparison identifies original DEF_PRE_64 bytes by SHA-256 `f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794`. This is bounded static instruction-shape/source evidence, not a new original execution, full native call-graph proof, or fresh differential acceptance.

The new method name is reconstructed API structure. The extraction preserves the existing adapter operation and its live-step/preflight qualifications. It does not enable cache1 publication, establish interior-pointer lifetime safety, recover physical Qt emissions/reentrancy, close full editor/Save lifecycle, resolve Report accepted Close or alter CP165 broader meshing. No additional original witness is required for this finite current-source migration; those larger witness gates remain intact.

