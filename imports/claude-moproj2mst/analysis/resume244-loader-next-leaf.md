# CP244 loader next-leaf assessment

## Result: no independent small L3 leaf is ready

The remaining reachable loader/model aliases are either coupled to multi-stage external effects or lack a caller-level fixture; turning one into a “small owner command” would hide unresolved ordering rather than reduce alias scope. The next candidate requiring a separate contract is `formingPostLoad` in `source/apps/moproj2mst/src/engine/loader_l3.cpp` around lines 123–141.

That live path resolves `m`, obtains guarded nullable `simCtl()`, reads `pdie1`, then looks up the object by number and its `ObjEntry`. Only when `op.equipmentId < 1` and both exist does it call `createEquipment(p, *m->step, o, fx)`, then set slot equipment ID, then operation equipment ID. The called writer adds project equipment and appends file effects, writes movement-name bytes into the native object, and serializes a second KEY. Moving this to `GuiModel` requires designing an owner/effect command spanning `Project`, `Operation`, `ObjEntry`, and `FileEffects`; it is not an isolated native alias replacement. Preserve order and return/effect prefixes before proposing that API.

The nearby `fePostLoad` and `expressBasePostLoad` are also poor next leaves: FormingExpress lacks a current corpus operation per L3 spec §6, and its missing caller-level fixture leaves the path source-reviewed only. Remaining `postload_views::equipmentSync` and `loader.cpp` inherited/object-copy sites likewise combine object/control aliases with FileEffects, native serialization, or ordering-sensitive partial mutation. Do not schedule any of these as a one-field accessor without a dedicated contract.

## Evidence and work queue

The source census after CP241/242 shows live accesses in `loader_l3.cpp` (`formingPostLoad`, property loaders, and Express post-load), `postload_views.cpp::equipmentSync`, and DB/object-loading helpers. CP242 already covers HeatTransferExpress PDIE with real L3 tests; the CP238 raw dimension helper was dead after CP237 and is excluded. CP239's Operation cache-presence leaf is an independent ready task, but it is outside the loader/model alias family.

For `formingPostLoad`, the existing `test_engine_l3.cpp::forming_without_equipment_creates_one` is a caller-level fixture for successful equipment creation. Before migration, a source review must identify fixtures for the no-equipment conditional's other branches (existing equipment, missing object, missing slot), and tests must assert FileEffects and slot/operation ID update order. Original qualification is decompilation/source-backed at GUI `0x140c06130` / `0x140c04950` and L3 ledger entries; this is not fresh dynamic original execution.

No new production ownership assignment is made in this report. No source, test, build, shared state, original, GUI, or REA files were changed; no tests were run.
