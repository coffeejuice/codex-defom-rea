# CP193 R300 furnacePostLoad plan review

## Result: PASS, with two precision notes

The proposed CP193 owner is a valid small lexical-scope cut against the CP192 frozen source. In `loader_l2.cpp`, `furnacePostLoad` obtains `m` through `opModel`, returns on null model, obtains `sc` by direct `m->simCtl()`, stores `furnace_on = 1`, conditionally sets up curves only at count zero, applies defaults after setup, then performs the unused `furnaceProps(c.propsOf(nullptr))` cast. The reviewed annotated L2 spec section 2 and ledger entry for `0x140c65830` agree with this order and unchecked SC dereference. The spec qualifies the underlying original evidence as static analysis; its named historical decompile/assembly files are absent here, so this review relies on the retained annotations rather than a new native trace.

The immediate void lambda taking the original `m->simCtl()` result preserves the access path and all effects in the stated order. Keeping the model-null return before it and the props cast after it preserves the current boundaries. `GuiModel::simCtl()` calls `requireGeometrySyncAvailable()` before checking the nullable step, so geometry refusal still precedes the furnace mutation and trailing cast; a null step yields null and the owner still dereferences it without a guard. Retaining that policy is consistent with this scope-only change. These are reconstructed-source facts, not additional native null-policy evidence.

The exact empty-branch test expectations are supported: `setup_curves(&22)` creates 21 curve entries for types `0x8e..0xa2` and one extra `0xa3`; `set_defaults()` then gives the first curve shape `ndim == 1`, `n[0] == 2`, `ncomp == 6` from the retained shape table. The populated-branch identity and sentinel checks are appropriate for proving the nonempty branch does not rebuild/reset the curve. The stated warning not to retain a curve pointer across `clear_curves()` is necessary.

The proposed fixture is reachable without a new seam: `heat1_as_furnace` already demonstrates opening `08_HEAT1/HEAT1.moproj`, mutating operation 1 to `SfOprHTFurnace`, replacing its properties and binding `owner`, then loading. `furnacePostLoad` is registered in `g_loaderSlotsL2`, and `test_loader_l2.cpp` already includes its public slot declaration. Calling the slot directly with `cls->current = op` exercises the same-owner postload branch while avoiding later load/property-copy effects. The corpus caveat in the L2 spec is accurately represented: this is class-substitution route coverage, not an original HTFurnace corpus operation.

The suggested invariants are grounded in available APIs: retain the loaded model, its step/control, props owner, and FileEffects vector sizes; check `NSTEP` independently; snapshot effects after load. The trailing cast's result is unused, and this function does not alter the furnace properties' copied curve list, so asserting its count/name remain unchanged during direct slot calls is coherent.

## Precision notes

- The expected count 22 is a test literal justified by the independently inspected setup implementation (21 fixed entries plus one extra for `n=22`). It should not be described as a value established solely by native evidence; the annotated native spec establishes the request of `0x16`, while reconstructed member source establishes the resulting count.
- The plan's “same-owner” claim is accurately about operation/model/property ownership and the callback's argument. Avoid implying this proves immunity to arbitrary reentry, operation replacement, or validity of aliases held elsewhere; the plan already disclaims those properties.

No blockers found. This is a static plan review only; no source or test edits, builds, tests, GUI/app-control, or REA database operations were performed. The project-level GUI lifecycle status script was not run because it is absent from this converter checkout (`tools/gui_validation_status.py` not found); no GUI session was acquired.
