# CP184 R300 saveMaterials implementation

The local material-step borrow now lives entirely inside an immediately invoked void callback in `source/apps/moproj2mst/src/engine/save_current.cpp`. The callback retains exactly one `opModel` lookup, the existing missing-model/step return before traversal/effects, the count read at `0x1320`, ascending `1..n` traversal, null/type-2 exclusions, number/path derivation and synchronous `saveMaterialKey` call. No step/material pointer leaves the callback. No admission guard, writer or Save-dispatch behavior changed.

## Frozen CP183 inputs

SHA-256 recorded before editing:

- `source/apps/moproj2mst/src/engine/save_current.cpp`: `EB702BD2E07FFACFDD7E49AB73488FCD1E17B77821AD5F4516DC40FCF20BEEDF`
- `source/apps/moproj2mst/tests/s/test_save_current.cpp`: `4F8342C0B14812CA908A843E24652C67E3D348659F37C411FC7C9674E627D5E5`

## Focused fixture

The new `save_materials_step_scope` Testkit case directly loads copied corpus operation 2 from `46_MO2_Boolean_Oprations/MO2_Boolean_Oprations.moproj`, asserting its `SfOprHeatTransferExpress2D` class. Before the helper call, it prints and inspects the live material count, every material number/type and null entry, and checks list/count consistency.

It preserves the loaded list and appends one ordinarily constructed, uniquely numbered type-2 material to guarantee exclusion coverage. Only when no loaded eligible material exists does it also append one ordinarily constructed eligible material. This is explicit synthetic helper mechanics. The noncopyable fixture guard restores the original head, tail, tail link, list count and step material count before destroying temporary nodes; original material contents and index arrays are untouched. Both temporary nodes receive the real loaded step as their owner.

The test computes eligible paths from the seeded list and asserts the exact number/order of writes, exact `materialDefaultFile` paths, nonempty bytes containing the material KEY header, excluded paths absent, and no deletes. `FileEffects::writes` is a vector and `captureKey` appends, so ordered path assertions are supported (the fixture-resolution note's map characterization was inaccurate).

A separate `DetachedStep` noncopyable guard tests the null-step call on the seeded model, asserts no writes/deletes and unchanged saved list counts, then verifies model/step identity and counts after normal and exceptional detachment scope exit. Final assertions verify restoration of the original loaded list after synthetic fixture teardown.

## Verification and limits

Static preflight confirmed the existing `testkit::run_all` entry point, supported `REQUIRE`/`CHECK`/`CHECK_EQ` macros, resolved includes, the Material constructor/destructor and list ownership layout, and the append order of `captureKey`. No C `assert` was added. No build, executable, focused/full test, GUI/app-control, REA database or shared checkpoint state operation was run by this owner. Root must build and execute `m2m.s.save_current`, inspect the printed live material inventory and new case result, and perform source-stable validation/freeze.

This change closes only the local source borrow. The fixture does not establish public Save admission/dispatch, native/oracle byte equivalence, other Save owners, or aggregate parked-model closure. The existing source qualification remains the reconstructed Save-current source and its native annotation at `0x140058cc0` cited by the CP184 plan; no new original-runtime observation was acquired.

Released source SHA-256:

- `source/apps/moproj2mst/src/engine/save_current.cpp`: `36B3B12693FD5C2B3D4F4BDE9EC486A9B673F7575DA4B6D94903DE19CEB0CF8B`
- `source/apps/moproj2mst/tests/s/test_save_current.cpp`: `AEC6E922352E31123545270266576F7C9C1E9CD16E9CDF7AE30290B4FB530C37`
