# CP184 R300 next raw-step borrow: material KEY loop

## Decision and ownership

After CP183 closes `baseDefaultPairs`, the smallest source-ready disjoint cut found is `saveMaterials(Project&, Operation&, FileEffects&)` in `source/apps/moproj2mst/src/engine/save_current.cpp`. Its only function-scope `DB_STEP_DATA* step` alias spans a bounded material-list loop and is passed synchronously by reference to `saveMaterialKey`. Confine model lookup, null-step early return, material count/list reads, path construction and each `saveMaterialKey` call to one immediately invoked void callback. The function is void, so no borrowed pointer needs to escape. Reserve this production file and `source/apps/moproj2mst/tests/s/test_save_current.cpp`; CP183's edit is already integrated in those same files, so verify exact current hashes and coordinate ownership before editing. This proposal itself makes no edits.

## Semantics to preserve

At the current `saveMaterials` body (around lines 641–654), retain one `opModel(p, op)` lookup and the same `!m || !m->step` early return before any material or file effect. Preserve reading count from step offset `0x1320`, iterating indices `1..n`, retrieving each material in ascending index order, skipping null entries and type 2 (`+0x1cc8`), reading material number at offset 0, deriving `materialDefaultFile(p, no)`, then invoking `saveMaterialKey` with the same arguments. Keep all step/material pointers within the immediate callback; do not change `saveMaterialKey`, KEY writing policy, or Save admission. Do not add geometry/Stopping guards in this local borrow cut.

## Focused evidence and test

The existing focused owner is `m2m.s.save_current` / `tests/s/test_save_current.cpp`, whose oracle Save cases execute the Save-current packet and compare produced KEY files to copied oracle-B outputs. Before claiming reachability, establish which non-skipped case actually invokes `saveMaterials`, and whether its fixture includes at least one non-type-2 material as well as a type-2 or null exclusion. The file currently describes byte-for-byte step-2 KEY comparison, which is useful end-to-end evidence only for a dispatched, non-skipped fixture. If no such case is proven, add a narrow helper-level test using a valid loaded operation and real step/material list; assert emitted material-key paths/bytes and exclusion behavior through `FileEffects`, without saving corpus changes. Include the null-step early-return case only with RAII restoration and assert no FileEffects writes/deletes.

Static acceptance: exactly one model lookup; null return before material traversal or effects; same count, order, filters, number/path derivation and helper calls; no step/material pointer escapes the immediate callback. Run `m2m.s.save_current` and confirm the new assertion/case executes. Root retains source-stable suite and freeze responsibilities.

## Qualification and limits

This closes only the local `saveMaterials` borrow. It does not close `saveInterObject`, `saveBcc`, public `GuiModel::step`, other Save owners, aggregate parked-model admission, or the broader CP173 source-closure blocker. Existing qualification is the reconstructed Save-current source and its cited native annotation at `0x140058cc0`; this proposal adds no original-runtime observation, public null-step Save equivalence, cache1 authority, or same-occurrence reader-tail claim. No source, test, build, executable, GUI/app-control, REA database, or shared state was changed or run while preparing this note.
