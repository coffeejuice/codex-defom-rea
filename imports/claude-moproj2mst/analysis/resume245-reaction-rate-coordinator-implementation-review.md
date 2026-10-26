# CP245 reaction-rate coordinator implementation review

**PASS — the private finite coordinator preserves callback admission, partial projection and ordered stores. No unresolved defect found.** This is an independent static implementation/test review; root owns grouped/full validation and freeze.

Reviewed the CP245 contract and PASS design review, implementation report, four-file diff/current bodies, existing reaction-rate cases and new parked callback fixture. Retained original qualifications remain those of CP135; this review performs no original-process comparison.

## Boundary and exact order

The enum and method are private StagedEditor300 members. The only caller is the reviewed reaction-rate event. No general GuiModel API, namespace export, test hook or C ABI is added. The existing owning pages/session and curve shared owners supply the synchronous lifetime; no derived native control/curve pointer is retained, returned, queued or passed to a callback. Existing cached source tokens/aliases elsewhere remain outside this confinement claim.

The public callback retains its exact binding, selection, lifecycle/page/source model/remesh admission. Temporary presence, page raw-address/shared-owner agreement and separation from temporaries zero through two still precede controls projection. The ACVCOF control aggregate and ControlsProjected report stage/flag are written before invoking the coordinator.

Inside the coordinator, one simCtl call performs geometry admission before nullable/current identity testing. Null current or different cached expected address returns MissingOrReplacedControl; the cached token is compared without dereference. Live collision comparisons follow, in envtmp, cnvcof, envatm, acvcof order, using raw addresses and short-circuit OR. Any collision returns LiveCurveAlias before index/curve stores.

After those checks there are no further failing operations, callbacks, allocations or model lookups: currentCurveIndex becomes 3; selection zero alone clears live ndim; live var[0] receives `[0,3,12,1][selection]`; temporary var[0] receives the same value; then Complete returns. Temporary ndim, tables/storage/payload, nonzero-selection live ndim and constant text are untouched. Source inspection establishes exact ordinary-store order; final-state assertions alone do not prove intermediate states.

Both private failure results map to the existing unsupported report at ControlsProjected, without modifying index/curve data. Only Complete sets complete/modelUpdated/completed outcome. Neither method catches the geometry exception or declares noexcept: a parked lookup leaves controls projected and suffix stores untouched. Repeated delivery still executes the suffix.

## Tests

Existing success cases cover every choice, constant-to-variable sequences, repeat delivery and preservation of table pointers/counts/payload, metadata and text. Refusal tests now use distinct index/live/temp dimension/variable sentinels and table pointer identities for missing/replaced control and live alias. Old step allocation is retained while its state is inspected; the live-alias case uses a shared_ptr aliasing the model lease rather than creating a second deleting owner. Early binding/selection/temporary/shared-owner failures remain covered separately.

The new viewer fixture constructs a real staged owner and obtains the session before parking through the existing low-version geometry reader path. It retains fixture-owned page/control/temp references, catches the specific parked geometry error from callback delivery and verifies all projected control fields plus unchanged index/live/temp dimension/variable sentinels. No parked pageState or admitted getter is used for inspection. This is meaningful callback-level exception-prefix coverage, beyond a direct simCtl refusal. Fixture model, operation/session and geometry restore lifetimes remain ordered; no control replacement occurs in that parked case.

Testkit entry points/assertions remain valid; the new owner header include resolves to the existing header. No production observer or public coordinator seam is added. The implementation owner reports focused Release build and 2/2 reaction-rate/viewer CTest targets passing. This reviewer ran no build or tests and does not independently certify those runtime receipts. Full suite, stopping-cached integration and freeze remain root gates.

## Source identity and qualifications

Reviewed full-file SHA-256 values match the implementation report:

- simcontrol300_owner.hpp: `CA2E95A060AA5EEB8CF3443A9346003C2EC973DC2D6456343A5952F93D8C6E02`
- simcontrol300_owner.cpp: `F08D32772E1578D6941ECCF6FC1832491FB347F158D4D6F4E6C0074E34DF476A`
- test_simcontrol300_reaction_rate.cpp: `5426BF10A46FF2120F557315FFDBA37CA0B1FE50D2A0177BA5AA34EAD0336790`
- test_viewer_box.cpp: `504D31F1D32E566AF3BA1B6F3D8D7CD529F810DEFC41D96ACE5E6C17E24D373C`

The coordinator name/enum are reconstruction structure; replacement identity/shared-owner/live collision guards are adapter qualifications. CP135's bounded static trace supports the retained native order but is not fresh original differential evidence. No physical Qt delivery, reentrancy, full editor/Save completion, atomicity, generation identity or ownership closure is established. No C ABI or Stopping/cache1 change belongs to this leaf; simcontrol300.cpp and stopping_cached test have no working diff. The implementation note retains these limitations accurately.

Only this report was written. No source/test/build/shared recovery state, original process, GUI or REA state was changed by this reviewer.
