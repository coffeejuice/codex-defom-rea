# CP236 loader next-leaf design (revised after review)

## Candidate

Move the bounded FormingExpress dimension-flag write from `loader_l3.cpp::feDimFlags` into a named `GuiModel::setFormingExpressDimensionFlags()` command. The current helper (around lines 491–507) gets the operation model, returns if the model or step is null, determines 2D from the step dimension at `+0x1c`, then writes:

- For dimension 2: `+0xa34`, `+0xa68`, `+0xa30` = 1, in that order, then `+0x1118` = 0.
- For every non-2D dimension: the first three fields = 2, in that order, leaving `+0x1118` untouched.

Avoid labeling the second branch “3D”: the source predicate is dimension-equals-2, else. This is a distinct loader-model operation, separate from CP235 editor ownership and prior unit/furnace/DB-step commands.

## Exact order and guard contract

The owner command should be void and non-throwing, with an initial null-step no-op matching `feDimFlags`; it should not call `requireGeometrySyncAvailable()`. In the actual `feLoadPrologue` route, sequence remains: base load prologue; `setTrans2(true)`; `setTrans1(false)`; model lookup; on non-null `simCtl()`, write `+0x240 = 0`, conditionally set `dsmax = 0.01` when `fabs(dsmax) < 1e-6`, then call the dimension command; after that, set `op.lubSystem = 1` only when `oprFlag == 0`. Do not move the dimension write or add a new exception point. In this route `simCtl()` performs the geometry admission before the earlier control writes; a parked model refuses before the command is reached.

## Truthful test seam

The retained L3 corpus has no FormingExpress operation (`source/docs/re/moproj2mst/mdt/L3/spec.md` §6), and §7 lists an original FormingExpress project as an open item. Therefore do not propose a corpus caller-level test at this checkpoint. Add a focused direct `GuiModel` command test in `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp` (or a dedicated owner test in that directory): construct existing `GuiModel` fixtures at dimensions 2 and 3; seed native fields at `+0xa34`, `+0xa68`, `+0xa30`, and a nonzero sentinel at `+0x1118`; invoke the command; assert all three fields become 1 and `+0x1118` becomes 0 for dimension 2; assert all three become 2 and the sentinel remains unchanged for dimension 3. Test values only; do not retain or return native pointers from the command. The test establishes command behavior for those fixtures, not caller dispatch or original parity.

The existing `forming_active` case is ordinary Forming and does not exercise this helper. Until a FormingExpress corpus fixture is acquired, the caller's position/order remains source-reviewed only. Confirm the configured Testkit/CTest entry before implementing the test.

## Files and original qualification

Production files: `source/apps/moproj2mst/include/m2m/engine.hpp`, `source/apps/moproj2mst/src/engine/gui_model.cpp`, and `source/apps/moproj2mst/src/engine/loader_l3.cpp`. No C ABI changes.

`loader_l3.cpp` annotates original GUI address `0x141424af0`; `source/docs/re/moproj2mst/mdt/L3/ledger.tsv` records the leaf as ported, and `L3/spec.md` gives the same field effects. These are decompilation/source-backed qualifications, not fresh original dynamic execution. No test result from the proposed direct fixture should be described as original parity.

Only this report is changed. No source, build, shared state, original, GUI, or REA state was modified; no tests were run.
