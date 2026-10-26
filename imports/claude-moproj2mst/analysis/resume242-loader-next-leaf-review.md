# CP242 loader next-reachable ownership leaf review

## Verdict: PASS

The revised fixture mapping matches the current caller tests: AirConvect 2D has one object and expects PDIE 1; HeatExpress 3D also has one and expects 1; HeatResting 2D has two and expects 2. Together they cover the `objectCount() == 1 ? 1 : 2` branch. The HeatExpress case confirms the type-0 integrated post-load route; it is not represented as the multiple-object case now.

The command target remains live: `installLoaderSlotsL3` binds the HeatTransferExpress post-load slot to `htePostLoad`, and the existing test operation dispatch reaches that registered path. Source order is described correctly: wizard copy, base Express post-load, TRANS1 update, model/control admission, unused `+0x240` read, then PDIE assignment. Preserve the command at that point, perform the guarded `simCtl()` access before control reads/writes, return without mutation for missing control, retain the `+0x240` read, and branch on exact total `objectCount()` equality to 1. This avoids exposing the native control pointer and preserves the existing no-step null result and geometry refusal.

The original qualification remains appropriately limited to the callback annotations and L3 spec/ledger as recovered source/decompilation evidence. The converter fixtures establish regression behavior for their paths, not a fresh original runtime result. No additional test seam is required for this scoped migration; configured focused target remains `m2m.mdt.engine_l3`.

No source/build/shared-state/GUI/REA changes were made; no tests were run.
