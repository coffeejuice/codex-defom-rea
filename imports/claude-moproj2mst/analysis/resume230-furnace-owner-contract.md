# CP230 furnace owner-command contract

## Decision

**PASS for the bounded `GuiModel` owner command**, with the null-step precondition made explicit. The geometry guard is already part of the existing `simCtl()` access path and must remain the first operation that can throw. The command must not reinterpret a missing `step` as a no-op or a new checked converter error.

## Exact command and call-site contract

Use the public model command signature `void initializeFurnacePostLoadDefaults();` (or the chosen project naming equivalent with the same no-argument/void shape). In its body, acquire `auto* sc = simCtl();` before any mutation, then preserve the current native sequence exactly:

1. unchecked write `sc->furnace_on = 1`;
2. query `sc->furnace.curve_count()`;
3. only when the count is zero, call `setup_curves(&n)` with a local `const std::int32_t n = 0x16`;
4. after setup returns, call `set_defaults()`.

In `furnacePostLoad`, preserve `GuiModel* m = opModel(p, op); if (!m) return;`, invoke the command, then retain `(void)furnaceProps(c.propsOf(nullptr));` after it. Do not cache/create the model, fetch `simCtl()` twice, move the property cast ahead of initialization, or initialize defaults on the nonzero branch.

`simCtl()` first calls `requireGeometrySyncAvailable()`, which throws `Error(M2M_E_UNSUPPORTED, ...)` when the model's geometry synchronization barrier is installed. It then returns `step ? &step->sim_ctrl : nullptr`. Thus the command inherits the barrier-first refusal and makes no mutation when that guard throws. `GuiModel` construction normally creates `step`, but `step` is public and can be reset/replaced; the command's null-step precondition is therefore `step != nullptr`. The current loader immediately dereferences the returned pointer without checking it, so adding `if (!sc) return` or converting it to `unsupported(...)` would change behavior. Missing-step execution is not a suitable test case under this unchecked contract.

## Native curve-call lifetime and order

The current `Cls_1403753f0` source declares `setup_curves(const std::int32_t*)`, `curve_count()`, and `set_defaults()` as synchronous member calls. `setup_curves` reads `*n` to determine the number of extra entries and does not save that argument. `set_defaults` keeps its curve pointer in a local variable within its loop. These method bodies do not store a `GuiModel`, sim-control, furnace, or input-argument borrow for later use. The furnace's curve list and curve objects are intentionally persistent model state, rather than retained owner-command references.

Source evidence does not prove that every opaque `ext::fun_*` helper has no internal global side effects or retention; no claim about those helper internals is needed for this command. Keeping all calls within the owner method prevents exposing their receiver or argument lifetime to the loader.

## Focused regression

The existing focused regression is `TEST_CASE(furnace_postload_step_scope)` in `source/apps/moproj2mst/tests/mdt/test_loader_l2.cpp`; CMake derives its CTest name as `m2m.mdt.loader_l2` (labels include `m2m-mdt` and `m2m-mdt-l2`). It already calls the registered `furnacePostLoad` slot directly. Its empty-curve branch checks that the flag becomes 1, setup produces 22 curves for `0x16`, and the first curve has the expected default shape. The populated branch repeats post-load after changing a curve value and confirms that the flag is set while count, curve identity, value, and other stored data remain intact. This is the right fixture/target to keep or extend; do not add a native-call spy solely to count/order these calls. Source order provides the call-order evidence, while state checks establish the branch effects.

If barrier-refusal coverage is added, it should use an existing geometry-barrier fixture only if the fixture can install a barrier through its established public test route; no furnace-specific barrier setup or private-state test hook was found in the inspected `mdt` tests. Assert that the exception occurs before `furnace_on` changes. Missing-step runtime testing remains excluded.

## Evidence and limits

Direct source evidence: `source/apps/moproj2mst/src/engine/loader_l2.cpp` (`furnacePostLoad`); `source/apps/moproj2mst/include/m2m/engine.hpp` (`GuiModel`, public `step`, `simCtl` declaration); `source/apps/moproj2mst/src/engine/gui_model.cpp` (`GuiModel` constructor, `requireGeometrySyncAvailable`, `simCtl`); `source/port/defmodel/include/defmodel/layout/cls_1403753f0.hpp` (method signatures); `source/port/defmodel/src/members/furnace.cpp` (`setup_curves`, `curve_count`, `set_defaults`); and `source/apps/moproj2mst/tests/mdt/test_loader_l2.cpp` (fixture and assertions). The CMake glob/register logic in `source/apps/moproj2mst/CMakeLists.txt` establishes `m2m.mdt.loader_l2` from the test path.

This is a source-contract review only. No source or tests were changed, no build/tests or GUI/REA activity was run, and no runtime occurrence against the original was established. Recovery status remains globally blocked on the separately recorded GUI release lifecycle dependency; that does not block this bounded source-level owner-contract conclusion.
