# CP117 operation/property dispatch coverage review

## Result

The highest-value source-closed property-dispatch branch in the retained evidence was Boolean-properties `vt+0xb8` during Save step 4. It is already integrated in the current converter, so it is not a new implementation task. The current source has the native row-order step-4 walk and calls Boolean side files before the later MST writer, while retaining the independent step-5 call. I found no further source-closed, reachable operation/property dispatcher branch ready to integrate from the inspected coverage.

One concrete reachable omission remains: active `SfOprDoeOutput` Save (`vt+0x68`, native `0x140e2e730`). Original dispatcher evidence identifies the sample-object and DOE document effects, and the candidate reaches an explicit guard in `saveCurrentOperation`. This is an intentional boundary rather than a missing generic dispatch arm: the Type2 callback/receiver return and process-global DBLIST `+0x1c` owner are unresolved. Keep the guard.

## Highest-value closed branch: Boolean step-4 property Save

The retained original Save contract calls `0x1400982e0` after project writing and before MST generation. It iterates operation-manager rows and dispatches properties `vt+0xb8`; the Boolean property body `0x1411e09a0 -> 0x1411e1820` writes `Boolean.BWZ` and `DEF_MACHDIST_CTRL2.INI`. The Boolean MST writer also calls the helper later, so those are distinct ordered invocations. Evidence is `analysis/resume71-property-dispatch-next.md` and `analysis/resume74-operation-dispatch-next.md`, with the underlying `source/docs/re/moproj2mst/mdt/spec.md` §1 and `s1-04/spec.md` §1.2.

Current `source/apps/moproj2mst/src/core/convert.cpp:367-390` performs step 4 in operation order. In native-save mode it attempts Cogging output at `:381`, dispatches `booleanSideFiles` for actual `BooleanProps` with data at `:384-386`, then the L6 and Express families at `:387-388`; the MST generation remains later. The separate Boolean writer remains in `source/apps/moproj2mst/src/ops/boolean.cpp` (the CP72 receipt records it as an independent step-5 call). This closes the CP71 Boolean ordering gap without collapsing the two occurrences.

The property coverage inventory at `analysis/resume74-operation-dispatch-next.md` and the later active-operation inventory at `analysis/resume88-operation-dispatch-inventory.md` report no other source-closed property writer family missing from the candidate. The inspection found no justification to add a generic dispatcher or to alter an existing class guard. Thus the Boolean step-4 branch is the best positive reference point, but it is already done.

## Reachable unresolved omission: active DoeOutput Save

The retained `source/docs/re/moproj2mst/mdt/L7/spec.md:78` binds DoeOutput's own `vt+0x68` to `0x140e2e730`. The original active Save route dispatches this slot at `0x140052ec0`; its bounded slot contract uses a viewer model to write a sample object, then the DOE output document (`SubProject+0x30`). After a normal Type2 refresh return, the outer Save continues to `Control/DEF_DOE.PAR` and mode writers. The source-level candidate summary and exact guard are in `analysis/resume74-operation-dispatch-next.md` and `analysis/resume88-operation-dispatch-inventory.md`.

The current explicit refusal is `source/apps/moproj2mst/src/engine/save_current.cpp:757-760`; it occurs before the candidate performs those effects. This branch is reachable when DoeOutput is the selected current operation and its source guards pass, but the needed state is not a class-key mapping alone. The preceding refresh requires the actual sender/receiver dispatch and normal slot return. Separately, original DBLIST admission tests process-global `0x143d766b8+0x1c` at `0x140651eba..0x140651ed1`; CP65/87 evidence does not bind its setter/reset lifecycle to the selected run or candidate Session. Neither a `.DS` path helper nor a fabricated callback-success flag closes these conditions.

**Next smallest action:** retain the current refusal and bind one actual Type2 Save occurrence to the selected DoeOutput owner, refresh receiver entry/normal return, and DBLIST `+0x1c` value/lifecycle. Without that occurrence/owner evidence, public integration would skip source-ordered effects or invent process-global state.

## Coverage limit

This is a bounded comparison against current converter sources and retained static dispatcher receipts. It does not prove every operation family or project fixture reaches its native path. No source/build/test/GUI/mutable-REA changes or actions were performed.
