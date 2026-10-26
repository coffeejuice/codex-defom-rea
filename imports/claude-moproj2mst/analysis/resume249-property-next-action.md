# CP249 property/inheritance next action

The native Report inheritance/Close gate remains witness-blocked, but a separate finite property alias migration is ready for design review and sequential implementation: move the unguarded owner-control unit read used by `propsSetHeatingTime` behind a value-only GuiModel query. This is new work; CP244 STEPDEF and CP246 Mode must not be reassigned.

## Queue reconciliation

`analysis/takeover-state.json` still marks RINHERIT ready with partial implementation/integration, incomplete validation and the next action requiring same-instance accepted Close ownership and actual document state. That is a ready investigation lane, not permission to admit Report cleanup. CP203 remains the controlling reconciliation: converter Project/SubProject/DOE SessionDocument ownership does not supply native SfSessionManager ownership. The existing diagnostic slot prefix is already represented and cannot be promoted to native delete/guard-clear from constructor defaults.

CP244's STEPDEF command is present, and both property setter/getter dispatch through syncOwnerStepDefinition. CP246's three Mode commands and private discriminator/provider helper are present; the six property sites use them and withOwnerSimCtl/setScWord are gone from props_heattreat_slots.cpp. Their repaired independent implementation reviews are PASS with focused reconstructed-fixture evidence. Remaining includes in that translation unit are not themselves a behavioral leaf. These completed migrations do not satisfy RINHERIT's document witness or close wider property/model aliases.

CP248 currently owns common engine.hpp/gui_model.cpp and meshing source/test files. This audit changes none of them. The proposal below has no semantic dependency on CP248, but root must wait for its common-file ownership release and freeze boundary before reserving edits. Do not run competing builds or edit during a source-stable run.

## Selected finite property leaf

Current `src/engine/props_sync.cpp:99`:

```cpp
withPropsStep(p, pr, [&](DB_STEP_DATA* s) {
    if (!s) throw Error(M2M_E_UNSUPPORTED,
        "operation load: heating time without an engine model (0x1405fcb90 reads a null model)");
    B(pr).heatingTime = valueWithUnit(t, 0, s->sim_ctrl.unit);
});
```

This single property setter has no explicit caller control parameter. It is smaller than migrating all four withReaderSimCtl consumers, which must preserve both explicit-control bypass and owner resolution. The source is reached by `src/engine/loader_l2.cpp::htSubTypeDefaults`, which calls propsSetHeatingTime before subsequent live tmax/envtmp writes. That loader retains its own separate raw control borrow; changing this leaf does not claim to close it.

Use a narrow proposed query, subject to independent design review:

```cpp
bool GuiModel::readPropertyHeatingTimeUnit(int& result) const noexcept;
```

Exact body:

```cpp
if (!step) return false;
result = step->sim_ctrl.unit;
return true;
```

It returns one scalar and never a native pointer, reference, callback or pointer-bearing snapshot. False leaves the output unchanged. **It must remain unguarded**: withPropsStep currently calls opModel and reads model->step directly, explicitly adding no geometry admission. Replacing this read with simCtl or another admitted accessor would add a new failure point and change parked behavior. The absence of a guard here is a preserved source property, not authorization to continue a parked geometry operation elsewhere.

In propsSetHeatingTime only, replace the callback with the equivalent owner/provider flow:

1. If pr.owner is absent, take the exact existing unsupported error without model lookup or CommonBlock allocation.
2. Otherwise call opModel(p, *pr.owner) once, using the explicit p exactly as today. Do not add an owner->project check, replace p, inspect a noncreating engine cache, or acquire a second model.
3. If no model or the new nullable unit query returns false, throw that exact existing code/message before property block creation/output assignment.
4. With a copied unit, retain `B(pr).heatingTime = valueWithUnit(t, 0, unit)` at the same point. Keep category zero, existing formatter/Qt conversion, and RHS formatting before CommonBlock assignment; do not move B(pr) allocation earlier.

No native model mutation is involved. The loader's following writes and callbacks stay in place. Keep withPropsStep/withReaderSimCtl and the other property readers unchanged because they still have consumers. Do not duplicate unit-name/Qt formatting inside the model or relocate general property logic into GuiModel merely to remove this one scalar read.

Production ownership is engine.hpp, gui_model.cpp and props_sync.cpp only, reserved after CP248. No loader_l2 edit, native layout, C ABI, property layout, ObjectKey issuance or report-document ownership change is required.

## Actual tests and meaningful additions

`tests/moproj/test_props_l2.cpp::props_step_null_and_explicit_control` is an existing always-running seam, distinct from its environment-dependent oracle cases. It already exercises propsSetHeatingTime refusal for no owner, owner with no model, and provider-returned model with absent step. It verifies the exact unsupported text, no CommonBlock allocation and provider counts. StepLookupProbe's classModel returns a real local GuiModel and counts calls. Retain these checks; missing-step/provider failures must still have one lookup for this operation, while no owner must have none. The four explicit-control reader calls must continue bypassing owner lookup; do not alter their tests or behavior.

Add to that target:

- Direct query live-unit tests with distinct values (for example 1, 2 and a negative sentinel) proving the scalar is neither normalized nor hardcoded and native fields stay unchanged.
- Absent-step false with a distinct initialized output sentinel unchanged; retain/restore the fixture step when later work needs it.
- Successful propsSetHeatingTime via StepLookupProbe with unit 1 and t=7.25: assert one provider lookup, allocated CommonBlock when previously absent and exact existing `7.25 sec` output; invoke again with another t to establish overwrite rather than cache-only behavior. Check the native unit/control is unchanged.
- Keep the current missing/null-path assertions and actual load/oracle tests. Do not claim a conditional oracle test executed merely because the target passes without its data environment.

A direct query in the existing parked-viewer fixture would use a distinct output sentinel and assert the live unit is returned without throwing. It is the most useful discriminator against accidental simCtl admission. Reserve that test file after CP248's ownership release, or explicitly retain the runtime coverage gap if only static no-guard inspection is performed. No new GUI fixture or production observer is needed.

CMake derives target `m2m_moproj_props_l2` / test `m2m.moproj.props_l2` from the existing path. Confirm configured metadata before running. Root's focused group should include that target and viewer_box if its parked query assertion is added, followed by relevant heating loader and stopping-cached regressions and the complete source-stable launcher/freeze. Existing Testkit main and Release-active CHECK/CHECK_EQ/REQUIRE suffice. Do not add a new target or C assert.

## Original qualification and exact RINHERIT blocker

The current property source attributes propsSetHeatingTime to `0x1405fcb90` and its owner's unit getter to `0x1408221e0`; the translation-unit provenance names retained MDT L2 d1.c/d2.c, and ledger.tsv/owners.tsv record 1405fcb90 as ported. These are historical static/ported-source qualifications, not newly verified original bytes or fresh runtime evidence. The proposed API is a reconstructed value boundary. Its unguarded nullable adapter behavior and error remain current-source facts; do not represent graceful error handling as original null-model behavior.

For RINHERIT itself, CP160/161/164/203 retain original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and bounded producer/Close qualifications. What remains missing is one joined original Open–Save–accepted-Close occurrence binding P, current SubProject S, D at S+0x30 and its accepted QObject parent; fresh current slot rebind/invalidation history; D's current +20/+30/+40/+48 payload identities; Save-time +2a/+68 and selected producer/callback outcomes; and both Close guard reads plus ordered deleting/guard/QObject effects, or the earlier death that made the slot null. Constructor flags, static reachability, standalone XML or DOE SessionDocument cannot replace that witness. Keep public Report Open/Save/Close refusal and the existing diagnostic prefix. No fresh GUI or mutable REA work was attempted by this audit.

Only this report was written. No source/test/build, shared recovery state, original process, GUI-control, mutable REA or deployment change was made. This plan advances one property alias boundary if approved; it neither resolves RINHERIT nor authorizes cache1 continuation or ownership closure.

