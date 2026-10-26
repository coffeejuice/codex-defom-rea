# CP195 R300 broader alias owner after L2 exhaustion

## Selection and freeze comparison

Retain `dieStressFromDbStep` in `source/apps/moproj2mst/src/engine/l6_slots.cpp` paired with `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp` as the next source-ready candidate. Both exact current files match the requested CP194 frozen source archive byte-for-byte. Archive: `build/wave3-checkpoint194-verified/source.zip`, SHA-256 `e837f823a179e5039ea70a05bc2820c0b16c081ea8d28e9dbed9ca39435de2b2`. Archive paths omit leading `source/`.

| File | Frozen/current SHA-256 | Match |
|---|---|---|
| `apps/moproj2mst/src/engine/l6_slots.cpp` | `aea0e5363c5680fdeb645089b6fe33c1969f6c6d0a36494bd2aab903b0e1d9be` | yes |
| `apps/moproj2mst/tests/mdt/test_l6_slots.cpp` | `a9d53b00ec11a9d17a883cded8f541df9d373c41f6924496a0136af514ee262f` | yes |

The owner, loader route, fixture and `GuiModelAccess::withStep` contract are unchanged from CP194. Its private helper calls `requireGeometrySyncAvailable()` then invokes the callback with `model.step.get()`, including null; it does not allocate a default or make a null borrow safe. The source/test pair is distinct from the connector pair.

## Exact separate borrow scopes

This function has two different model owners (`m`, the actual operation model, and `s`, the scratch model). One callback cannot scope both step pointers. Any implementation should use immediate `GuiModelAccess::withStep` calls separately for each model, never capture/store/return either raw step pointer, and preserve this sequence:

1. Keep operation/model admission and `dieStressScratch(c,p)` where they are. Keep `m->reset()` before any destination-step borrow.
2. Read scratch dimension in a scratch-only `withStep(*s, ...)` callback into a scalar `d`; end that callback. Then set destination dimension in an actual-only `withStep(*m, ...)` callback. A null pointer remains passed to the same unchecked dereference, preserving the current null fault behavior.
3. Keep `opi->objIds.clear()`, the empty-scratch-object check/refusal, property/data validation and `stepNumber` assignment in their current order.
4. For the copy block, use an actual-step callback around a nested scratch-step callback. The actual callback uses its pointer for `n_materials`, material destination and phase-pair destination; the nested scratch callback uses its pointer for material/phase-pair sources. End both borrows after these copies. Preserve the existing unsupported branch before property validation and do not move it into a changed order.
5. Use a fresh actual-step callback for the existing `m->simCtl()` acquisition and all remaining `sc` work. Retain the current nullable `sc` early return. Within the `is2D(*m)` branch only, obtain scratch control through a scratch-only callback and copy `geotyp`; then obtain scratch control again through a separate scratch-only callback for the unconditional `pdie1` read, preserving current call order and unchecked dereference. End each scratch callback before continuing. Call `s.reset()` at its current point, after PDIE and before NSTART. Keep NSTART/contact-pair calls, conditional control stores, and final SOLMTD/DTMAX stores in their existing order and inside the actual callback so the borrowed actual step does not outlive those uses of `sc`.

`withStep` adds geometry-sync admission before each borrow. This is a migration precondition, not a recovered original geometry policy: each check must remain at its corresponding access point after the preexisting admission/scratch work, rather than being hoisted to function entry. The existing `m->simCtl()` itself performs geometry admission before returning nullable control; keep that call and its null early return. `s->simCtl()` currently may return null and its dereference is unchecked; preserve that rather than introducing a fallback. Keep geometry decisions on destination `m` (`is2D(*m)` and `is3D(*m)`), not scratch `s`. This is lexical confinement with ordered admission checks, not model pinning, reentry safety, or global closure of public `GuiModel::step`.

## Fixture and observable boundary

`test_l6_slots.cpp::diestress_without_db` reaches this owner through the public L6 loader route: it opens corpus `42_vdie/vdie.moproj`, reclasses operation 4, and invokes `loadCode`; `diestressLoadData` routes to `dieStressFromDbStep` when the project DB/KEY is absent. The test header explicitly says the corpus has no native Diestress operation. The fixture supplies an empty scratch model and existing independent literal output checks for empty object IDs, step number, NSTART, SOLMTD/DTMAX control values, and post-load TRANS flags. Keep those checks as regression coverage; do not claim they establish native-class behavior, populated material/phase-pair copying, or foreign-owner rejection.

Remove any proposed same-owner positive/negative identity assertion. This fixture has a single public route and does not supply independent expected values for a foreign-owner comparison; self-comparison would be tautological. Do not add a synthetic model or test seam. The existing test can exercise the refactored route and its current output assertions, but it cannot be presented as direct borrow-identity coverage. The unsupported populated-object branch remains untested by this fixture.

The test is registered as `m2m.mdt.l6_slots` (`build/candidate/CTestTestfile.cmake:315-316`). No source/test/build was run or changed during this selection audit. A later implementation may use this existing test as its route regression while preserving its stated coverage limits.

## Inventory reconciliation and evidence limits

The CP173 direct-`.step` inventory is historical. Subsequent scoped migrations closed many ordinary save, property, equipment, restart, material, interobject, contact, BCC, and L3/L6 save owners. CP192–CP194 closed the cited L2 owners; CP195's preceding L2 audit found no worthwhile additional callback. L5 still has dispersed scratch/actual-step process/apply and writer flows with distinct effect order; this audit found no smaller single owner with a matching existing public fixture and disjoint source/test pair. `dieStressFromDbStep` has a bounded model-copy sequence and an existing route regression.

The L6 test header and retained `source/docs/re/moproj2mst/mdt/L6/spec.md` qualify expected behavior from historical decompiles/disassembly, not fresh original runtime evidence. The no-DB fixture uses class substitution. Claims here are limited to current source ordering, route reachability, and existing reconstructed outputs. Native null/geometry policy, native-class coverage, nonempty-copy behavior, and complete public equivalence remain unproven.

This pass wrote only this report. No source/test/build, GUI/app-control, live or mutable REA database, shared plan/state, parent/DMO/Claude/original files, or deployment was changed or used.
