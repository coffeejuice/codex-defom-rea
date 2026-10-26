# CP191 R300 `storeSimNames` plan review

## Verdict

**PASS, with one fixture preflight condition.** The proposed cut is bounded to the existing `m ? m->simCtl() : nullptr` acquisition and the four field stores. The direct fixture is appropriate for checking helper mechanics and the proposed report correctly limits what it proves.

## Source and native order

Current `storeSimNames` matches the plan's stated prefix: active subproject, operation name and flow number, materialized `simEntries`, first position match, no-match return, unnamed-entry refusal, then `opModel`. The current tail then obtains `simCtl`, returns on null, and performs the stores in the stated order: OPRNAM (`0x40`), CURSIM[1], SIMNAM (`0x50`), CURSIM[0]. The callback can keep the current null check and those four writes together without moving lookup, conversion, refusal, or mutation order. It must remain a synchronous immediately invoked void callback and must not return or retain `sc`.

Packet S `spec.md` §1 supports the active-subproject/name/chain-number/list selection and the same first-entry store order and widths. It states no writes for an empty list or absent SimControl. The proposal appropriately treats the unnamed-entry refusal as reconstructed policy and makes no claim of exact native assembly or public GUI Save equivalence.

`GuiModel::simCtl()` is the existing access path and applies `requireGeometrySyncAvailable()` before its nullable step lookup. Preserving that accessor preserves its admission behavior. The callback narrows the alias's lexical use; it does not pin the step or establish broader lifetime/reentry safety, as the proposal acknowledges.

## Fixture and scope

The `save_interobject_step_scope` setup is a suitable starting point for the named project/operation and real loaded model. The proposed fixture should independently require a matching, named `simEntries` entry before checking output; that prevents a vacuous pass. Sentinel checks for all four fields plus model/step identity directly exercise the helper. The detached-step check is compatible with this helper's null behavior if it is confined to this direct call and restores the step with RAII; it must not be expanded to `syncOperationFiles`.

The proposed change is properly limited to `save_current.cpp` and `test_save_current.cpp`; it does not assert public dispatch coverage or close the larger R300 ownership/admission questions. One implementation preflight remains: verify `m2m.s.save_current` is an existing registered Testkit target and that the direct fixture's required engine/internal declarations and fixture configuration are available. The plan already assigns that confirmation to root, so this is not a design blocker.

No source, build, GUI, REA database, or test state was changed or run during this review.
