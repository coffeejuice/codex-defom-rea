# CP244: next Editor300 model-alias leaf after CP243

## Finding

After the `dimensionOr(3)` migration, the smallest remaining live editor-event route that directly acquires a fresh `SimControl*` is `StagedEditor300::reactionRateClicked` in `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp` (around line 300; native callback `0x140eccf10`). It is reachable through the bound editor's four reaction-rate clicked wrappers and has a focused Testkit target, `m2m_mdt_simcontrol300_reaction_rate`.

This is a coupled transition, but the current types do not establish an implementation blocker. The current-curve owner type is `EnvironmentCurveTemporaries`, declared in `simcontrol300.hpp`; it stores four shared `DYNARY` owners and the optional current index. The event spans the page's private control projection, that editor-owned temporary/index state, and the model's live `sim_ctrl.acvcof`. A finite synchronous coordinator may be able to own the latter ordered suffix without exposing a raw control to the callback caller. The concrete boundary and type direction need review before implementation.

CP222's broad lifecycle aliases are also still present, but they hold the control across broad initial page assembly or Apply. They are not smaller isolated event consumers. The event callback is therefore the nearest candidate, with the explicit coordination dependency below.

## Source order and required behavior

Current `reactionRateClicked(binding, selection)` does the following:

1. Rejects invalid identity/selection and incomplete or mismatched page/model/remesh binding.
2. Checks the temporary curve is present, page-aliased by the same shared owner, and distinct from the other three temporary curves. These refusals happen before widget projection.
3. Projects `EnvironmentAcvcofControls` and records `ControlsProjected`.
4. Calls `binding.model.simCtl()`. This invokes `requireGeometrySyncAvailable()` first; only after that barrier does it perform the nullable current-control lookup. It then compares current control identity with `pages_->sourceControl`. A parked-geometry exception propagates after control projection. Missing or replaced control returns at `ControlsProjected`, retaining controls and leaving the curve index and data untouched.
5. Checks the temporary pointer against all four live Environment curves. A collision returns after controls but before later stores.
6. Stores `owners.currentCurveIndex = 3`.
7. For selection 0, writes live `acvcof.ndim = 0`; for all selections writes live `acvcof.var[0]` from `[0, 3, 12, 1]`; then writes the same variable to the temporary curve.
8. Marks the report complete.

The order is supported by the retained original callback qualification: `analysis/resume135-editor-reaction-rate.md` and `analysis/resume135-editor-reaction-rate-review.md`, original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, callback range `[0x140eccf10,0x140ecd2e5)`, range SHA-256 `9b53369cd493e1d3cad521a65293ab114f46edaa8dabc0e09a8b5c0b6834da38`. This is a bounded static native trace, not original-process differential evidence. The current private alias/collision refusal is adapter behavior, not a claim of native validation or failure handling.

## Next investigation: finite synchronous coordinator

A live-fields-only command such as `GuiModel::applyEditorReactionRateIfControlMatches(expected, selection)` cannot preserve the entire suffix by itself:

- The current-curve index and temporary `DYNARY` belong to `EnvironmentCurveTemporaries`, not `GuiModel`.
- The temporary curve's identity must be checked against the live control's four curves after the fresh control lookup and before index/data stores. A caller-side pre-check cannot safely inspect the control; a post-command check would be too late because the model write has already occurred.
- If the caller sets the index before a separate model command, a parked barrier or replaced/null control creates a new partial prefix (index changed) where the original leaves the index unchanged. If it sets the index after a separate command, the index is reordered after the live control writes. A single coordinator can avoid both splits by doing one admission/check phase followed by the existing plain stores; the current source does not show an intervening throwing operation in that suffix.
- Passing a raw `DYNARY*`, `EnvironmentCurveTemporaries&`, or a closure into a generic model seam would move rather than resolve the ownership boundary. However, this does not rule out a narrowly scoped, named synchronous operation that explicitly coordinates this finite Editor300 event and accepts the editor owner state it must update.

The next investigation should compare two finite boundary placements: (a) a private Editor300-domain coordinator that accepts `GuiModel&`, the expected control identity token, selection, and `EnvironmentCurveTemporaries&` plus the page's curve-3 owner identity; or (b) a named `GuiModel` operation whose declaration explicitly depends on the finite `EnvironmentCurveTemporaries` type. In either design, keep the caller's existing admission and control projection before invoking the coordinator. Inside it, perform exactly one barrier-first fresh control lookup and identity check; compare temporary 3 against the current live curve addresses; then store current index, live dimension if selection 0, live var, and temporary var in the recorded order. Do not return or retain `SimControl*`, `DYNARY*`, or a callback. Validate the shared-owner/page-curve relation before projection as the caller currently does; keep live-curve collision refusal after the fresh model lookup. The operation's result must distinguish missing/replaced control, live collision, and completion sufficiently to preserve `EnvironmentReactionReport` stage/outcome.

This is an implementation-ready behavioral contract, not a final API selection. Resolve which layer owns the explicit cross-domain dependency and whether a forward declaration/include arrangement avoids an unwanted cycle. A private domain coordinator can be a substantive migration only if it actually encloses the fresh model borrow and the ordered mutation suffix; a helper that merely forwards `simCtl()` or the raw pointer is not. Do not claim that the call boundary itself adds a concurrency/reentrancy hazard: the synchronous call does not schedule callbacks. The concern is preserving admission and mutation order, not hypothetical interleaving. No cache1/Stopping behavior or C ABI needs to change.

## Tests and evidence seam

Existing focused file/CTest: `source/apps/moproj2mst/tests/mdt/test_simcontrol300_reaction_rate.cpp` / `m2m.mdt.simcontrol300_reaction_rate`. It covers all four choices, constant-to-variable sequences, repeated clicks, absent/replaced control retaining control projection but not curve stores, invalid binding/choice, changed temporary/page alias, and live-curve alias refusal. These tests establish final state and refusal prefixes; they do not prove the order of distinct index/live/temp stores or exclude every intermediate state. Keep them as regression evidence for a coordinated operation, add nondefault index/live/temp sentinels on every refusal prefix, and add a parked callback case that catches the geometry exception and inspects a retained page reference to verify controls changed but index/data did not. Avoid `pageState() noexcept` after parking. Establish exact successful store order by source review; add runtime instrumentation only if an existing observation seam can observe it without changing production behavior. Preserve root's existing `simcontrol300_stopping_cached` and parked-geometry assertions. Testkit uses `CHECK_EQ`/`REQUIRE`; do not introduce C `assert` or `REQUIRE_EQ`.

## Scope

The other editor lifecycle aliases observed in this narrow scan are `openInitialEditorInto`'s current-control borrow across page fills and the Apply path's cached `session.sourceControl` borrow across multiple page handlers and Extra3D. Both are broader multi-page boundaries, not an independent small event. No smaller direct `binding.model.simCtl()` consumer was found in `simcontrol300_owner.cpp`.

This is a read-only next-investigation contract, not a final declaration that implementation is impossible. No source, tests, build artifacts, shared state, GUI, original process, or REA state was changed.

