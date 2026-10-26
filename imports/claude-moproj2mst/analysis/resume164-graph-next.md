# CP164 graph: next source cut or missing occurrence witness

## Result

No independent source-ready implementation cut is supported by retained CP159–160 evidence. The exact missing original witness is a **same-owner, same-manager normal-return lifecycle occurrence** that binds (a) the actual population activation or mode-2 delegate and its clear/rebuild sequence, (b) each item inserted and its parent/root coordinates, and (c) the later StepData finalizer's direct top-level `item(row, 1..6)` queries on that same manager. The occurrence must also retain the row's View/Settings identity and establish whether a checkable item is returned and whether its item-change callback executes before/after the finalizer writes the setting. The current static pieces stop on either side of this join; they do not prove any accepted row effect.

Until that witness exists, retain the graph/property-table guards. Do not implement from `d78490`'s hierarchical writer alone, nor translate CP159's callback into an unconditional Settings update. CP160 establishes that those child rows are not the finalizer's queried top-level cells, and that `d78220`'s mode is distinct from the owner mode field.

## Narrow continuation

Reuse the retained CP159 writer, manager, and callback bodies and CP160 owner/View/finalizer bridge. The only useful next acquisition is a bounded static caller/occurrence trace starting at the already identified mode-1 activation site `0x140cdfbd0:0x140cdfe77..0x140cdfe9a`, following its selected owner and manager into the selected StepData dispatch/finalizer (`0x140c90bd0 -> 0x140d7c260`). Resolve the caller relationship and intervening manager mutation/clear calls for that same owner. If the path selects mode 2, follow only the concrete `d7a9f0 -> d3f510` delegate. This is a trace of a lifecycle occurrence, not another broad scan of writer/callback functions already closed in CP159.

For source admission, the decisive result must be one of:

1. a normally reached top-level checkable cell at the finalizer query, tied to an exact owner/Settings row and manager generation, with callback/finalizer order established; or
2. a normally completed finalizer occurrence where each queried item is null, proving the no-cell outcome for that generation.

If static caller context cannot establish activation-to-finalizer same-instance continuity, the missing witness becomes a retained same-instance original runtime trace of that interval. Do not substitute a fixture or isolated constructor defaults. Any future isolated adapter/test cut should wait until one of those outcomes gives it a concrete contract.

## Evidence and source qualifications

- CP159 `analysis/resume159-graph-callback.md` (SHA-256 `9F65AAF4632AE5CFC7D550D7DF5D2FB578A2E74E0F48C8067010BBB5AFD909F5`) binds the actual `SfObjectManager` callback owner, role reads/writes, selector normalization, linked-owner update, and sibling synchronization. Its report explicitly leaves the RGRAPH row mapping and refresh/finalize timing unresolved.
- CP159 writer evidence `analysis/resume159-graph-evidence.json` (SHA-256 `591F448C4959F22CD1121BA60AD1F824C24750E39FB881F48EB5716BE57FCA8E`) is the source for the item vtable and retained writer investigation; it does not establish top-level finalizer cells.
- CP160 `analysis/resume160-graph-widget-next.md` (SHA-256 `3E08E8C2A888F58CAB59BB7A4913BE03874669C0908A0C03B923112C80C601DC`) corrects the earlier QTableWidget description: native owner is `GuiModel+0x230` `SfObjectManager`, derived from `QStandardItemModel`. It binds actual View owner and Settings lookup, mode separation, hierarchical group/object insertion, and the direct top-level finalizer query. Its explicit remaining question is the manager population generation at that finalizer.
- CP160 range receipt `analysis/resume160-graphwidget-verification.json` (SHA-256 `417C85E8CE9FF100A46652037E52E39DEF005D90420386F33010D312180F4ACA`) qualifies retained assembly/machine spans. These are static ranges, not a dynamic occurrence.
- Current candidate header `source/apps/moproj2mst/include/m2m/doe_property_table.hpp` (SHA-256 `1FB071DFC80635A2F12A9400A5769183AE13AAF3FDD07930681DC914C242ABCF`) explicitly models a zero-row constructor and leaves populated item behavior unrecovered. `source/apps/moproj2mst/src/engine/viewer.cpp` (SHA-256 `3CD7BA2978CF3823C8500B289AC27C04B1548F545A3E0B00BD3ED3A4BF8BBEA4`) retains a populated-payload refusal. No source or test change is justified by the currently verified contract.

The evidence subject in CP159/160 is original `evidence/original-binaries/DEF_GUI_MO_64.exe`, v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; that is the original application's version, not a reconstructed version. No original process, GUI, live REA session, build, tests, or mutable analysis state was used here. The converter continuation checker remains `CONTINUE` with 15 remaining items; this note closes no global gate.
