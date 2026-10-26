# CP141: populated property-table boundary

## Finding

The retained native finalizer closes the **row-to-view and update order** but does not close how a populated/checkable cell is created or what callback it drives. Keep the populated-cell refusal. There is no source-closed private transition to add without the missing same-view table-generation/callback witness.

## What the original finalizer establishes

The retained static decompile of `0x140d7c260` (inside `analysis/wave2-doe-stepdata-statevar-finalizer-decompile.json`, original binary SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`) gives this order:

1. Copy the model's View-pointer list with `0x140c82cb0`.
2. Refresh current object with `0x140c7b1a0(model, 1)`.
3. Iterate the copied Views. A row is consumed only for a View passing `0x1411f1b80` (bound object with kind other than 7) and returning nonnull Settings from `0x1411f1a10`.
4. For that row, visit columns 1 through 6. `QStandardItemModel::item(row,column)` is read-only; only a nonnull item for which `isCheckable()` is true gets a state lookup and `setCheckState()`. Row advances after the eligible View's six columns.

The adapter at `source/apps/moproj2mst/src/engine/viewer.cpp:1260-1284` follows this order, uses the same eligibility predicates/settings lookup, and increments row only for the eligible View. The nullable native-step refresh borrow ends before that walk. `PropertyTableState::itemIsAbsent` at `source/apps/moproj2mst/src/doe/property_table.cpp` records the query and answers absent only for negative indices or its constructor-empty state; an otherwise populated case refuses before fabricating an item or mutating flags.

## Missing edge

The `0x140d7c260` body consumes already-present items; it contains no item allocation, insertion, role/flag initialization, or signal connection. It also does not reveal whether programmatic `setCheckState` on these particular cells triggers a connected application callback with additional state effects. The retained DEFORM snapshot search found nearby `QStandardItem` construction/`setCheckable` code for other models, but did not bind any of it to this `+0x230 SfObjectManager` or its `item(row,1..6)` cells. Those lookalike callsites cannot establish this manager's population path.

The smallest witness needed is one source-qualified path that creates a checkable item in this exact `SfObjectManager`, binds its row to the same eligible View/settings entry, and establishes the connected `itemChanged`/model callback (including whether finalizer-driven `setCheckState` re-enters it). Without that identity chain, copying the setter alone would guess cell absence, flags/roles, and callback effects.

## Recommendation and limits

Preserve current constructor-empty/null-item handling and populated-cell refusal. The highest-value next recovery is the manager's actual item-population callsite and its signal connections, not another current-object borrow or a synthetic test item. Evidence reviewed was retained source/static decompilation only; no GUI, original execution, mutable REA state, source edit, or build was used. The retained decompile JSON SHA-256 is `3f8e923f48538cf0454b6e119572b2528bd61e1d7edb26be50102432f09a6e96`; current `viewer.cpp` SHA-256 is `45107ea75fca2394a3fb50444fd8807825d7ff75e1c8adaf4c486186774975df`.
