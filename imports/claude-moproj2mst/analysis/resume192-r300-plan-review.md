# CP192 R300 plan review

**Result: PASS, with two fixture details to make explicit before implementation.** The proposed consumer boundary is source-supported and appropriately narrow. The plan accurately preserves subtype selection, the SimControl alias lifetime through the nested heating-time setter, store ordering, and the current geometry/null admission behavior. The direct corpus fixture is the right test shape for this change.

## Evidence checked

- `loader_l2.cpp` has the documented `opSimCtl` seam and `htSubTypeDefaults` body. The subtype matching is case-insensitive and ordered Transfer, Rest, Dwell; the unmatched branch alone selects furnace ambient. `propsSetHeatingTime` precedes TMAX, ENVTMP ndim, then ENVTMP value stores.
- `props_sync.cpp` confirms `propsSetHeatingTime` independently calls `withPropsStep`, reads the owner's SimControl unit, and refuses if the nested lookup has no step. Thus the planned test must distinguish a null direct `opSimCtl` result (early return) from a live control followed by the nested lookup.
- `GuiModel::simCtl` first invokes `requireGeometrySyncAvailable()` and only then returns either the embedded control or null for a missing step. The geometry barrier therefore throws before subtype and property effects; an ordinary missing step returns null before the nested setter.
- Existing `test_loader_l2.cpp` already loads corpus `08_HEAT1/HEAT1.moproj`, addresses operation 1, and establishes the SI `15 sec` heating-time spelling. Its `openCorpus` returns null when `M2M_CORPUS` is unset, so the new case should use the same explicit `REQUIRE` pattern.
- L2 `spec.md` section 2 supports the native subtype priority, constants, setter, dropped UI label, and field order. Its scope/open-items text also supports keeping reconstruction-only null/geometry conclusions separate from native claims.

## Review notes

1. The proposed null-step case is sound only if the test can leave `opModel`'s cached model intact while making that model's `step` null, and can restore the exact original step before any further operation. State the restoration guard's lifetime and ensure it runs even when a test assertion fails; otherwise the test can poison subsequent cases or destroy fixture-owned state. The plan already calls for a local RAII guard, so this is an implementation precision point, not a plan blocker.
2. For non-SI heating-time text expectations, bind literals from the retained unit table/source independently of `propsSetHeatingTime`; do not call production `unitName`/formatter in the expected-value construction. The plan says this, but does not yet list the exact English spellings for the exercised unit and values. Record those literal expectations when implementing the table-driven cases.
3. The missing-owner case should use a standalone `HeatTransferProps` with `owner == nullptr` and a valid sentinel `CommonBlock`, as planned. That directly proves no model creation/cache mutation and avoids conflating the owner admission with model or geometry guards.
4. Test the mixed overlap cases with distinct strings and independent expected rows: Transfer+Rest+Dwell selects Transfer; Rest+Dwell selects Rest. Include at least one mixed-case spelling in each relevant phrase so case-insensitivity is directly exercised.

No blockers found in the proposed boundary or behavior claims. The report is a plan review only; no source, tests, builds, GUI/app control, REA database, or shared analysis state was changed. No tests were run.
