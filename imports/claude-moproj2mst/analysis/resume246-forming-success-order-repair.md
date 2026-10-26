# CP246: bounded Forming success-order repair

## Purpose and boundary

Repair the ordering of the already-enabled `formingPostLoad` equipment creation on its **recorded-effects, successful-add path**. This is an internal source correction, not a claim that native add/copy failure or filesystem effects are fully recovered. Keep the existing class hook, creation gate, model/slot lookups, and `equipmentSync` behavior. Do not enable an operation or branch that currently refuses or skips.

## Current versus retained original order

`loader_l3.cpp:126-143` calls `opModel`, guarded nullable `simCtl`, reads PDIE and operation equipment ID, then calls `m->object(pdie)` and `objEntryN(op,pdie)`. The latter may append missing in-range records even when the later creation gate fails (`model/objects.cpp:93-107`). When `equipmentId < 1` and both object and slot exist, the caller invokes `createEquipment(p,*m->step,o,fx)`, assigns `slot->equipmentId`, then assigns `op.equipmentId`.

`postload_views.cpp:165-174` currently performs name creation, first `equipmentBytes` serialization, `addEquipment` (which appends mkdir/write effects and the Project row), native movement-name copy, manager path lookup, **second** `equipmentBytes` serialization/write append, then returns the ID. Thus a failure in the final serialization leaves the slot and operation IDs at their old values despite earlier row/effects/name changes.

The retained original `d0.c:84-161` (`0x140c06130`, Forming) and `:254-327` (`0x140c04950`, Forming2D) instead serialize a temp KEY, call manager add, remove the temp source, copy the name to movement bytes, query the manager path, store slot ID, store operation ID, reacquire the step, query the path again and attempt the final KEY writer. The second writer can therefore occur only after both ID stores. The decompile has no positive-ID or nonempty-path gate after add; even `-1` continues through naming/path/ID steps. This report does not implement that failure branch.

## Narrow internal API split

Split only the existing engine-local helper at the final KEY step in `postload_views.cpp`, with declarations in `engine_internal.hpp` if needed across translation units:

```cpp
struct EquipmentCreationPrefix { int id; String pathBeforeIdStores; };
EquipmentCreationPrefix beginEquipmentCreation(Project&, DB_STEP_DATA&, LLOBJ_DATA*, FileEffects&);
void appendFinalEquipmentKey(DB_STEP_DATA&, LLOBJ_DATA*, const String& path, FileEffects&);
```

`beginEquipmentCreation` does exactly the current `createEquipment` work through the first `managerFile` call: name, first serialization, add, movement-name copy, path lookup. It returns only value data, never a step/object/slot alias. `appendFinalEquipmentKey` does exactly today's `if (!file.isEmpty()) fx.writes.push_back({equipmentKeyTarget(file), equipmentBytes(step,obj)})`, with the same condition and expression evaluation. The path condition is inherited converter behavior; it is **not** evidence of original `-1` fidelity.

Preserve the existing `createEquipment` symbol for `equipmentSync`: implement it as `prefix = begin...; appendFinalEquipmentKey(step,obj,prefix.pathBeforeIdStores,fx); return prefix.id;`. This retains its current final-write-before-caller-ID sequence and avoids changing a separate route under this repair. In `formingPostLoad`, replace its `createEquipment` call with the prefix, store `slot->equipmentId = prefix.id`, then `op.equipmentId = prefix.id`, then perform the final append. On this route, reacquire `m->step` at the point corresponding to the original's second step lookup; if absent, skip only the final append. Query `managerFile(p.equipment,prefix.id)` again after the two stores for the final path, matching the original's second manager query on the supported positive-ID path. Do not call `simCtl()` or `object()` again, add a geometry admission, move `objEntryN`, or precompute second KEY bytes before the ID stores.

This is intentionally an engine-local helper split, not a new `GuiModel` owner command. `loader_l3.cpp` continues to use its existing native aliases, so no broader alias-closure claim follows. A later finite `GuiModel` coordinator can subsume this sequence once its full failure/effect contract is established. If a shared split changes `equipmentSync` beyond the exact wrapper sequence above, prefer a forming-specific private prefix/final pair rather than altering that caller.

## Observable prefixes and tests

For the currently representable positive-ID path, preserve these source-visible prefixes on a C++ exception without catch/rollback:

* Before or during first serialization: no new Project row, equipment `FileEffects` entry, movement rename or ID store. The disposable temp file may have its own native prefix; see the evidence gap below.
* After `addEquipment` and before either ID store: the recorded mkdir and first write and the Project row remain. Once movement copy has run, the native name remains. A throw during the first manager path construction still leaves IDs unchanged.
* After slot ID but before operation ID: only a nonstandard C++ interruption between two scalar stores could expose this prefix; do not insert a new throw or callback there.
* During final path lookup, serialization, or `fx.writes.push_back`: the row, first recorded write, movement name and **both IDs remain**. The second write is present only if its append completed. This is the corrected reachable prefix. No rollback or transactional effect staging is authorized.

Extend the existing copied-corpus `test_engine_l3.cpp::forming_without_equipment_creates_one` beyond its `op->equipmentId > 0` assertion. Verify the selected PDIE `ObjEntry` ID matches, exactly one row was added with that ID and expected `Equipment (n)` name/path, the two equipment writes are ordered first-copy then final KEY in `fx.writes`, and the bytes reflect the movement name change. Keep test effects under `M2M_TEST_SCRATCH`; no host missing-file probe. The historical Oracle-B 96 case in `test_postload_views.cpp:9-11,141-151` is a useful final-byte/ID comparison, with historical provenance only. Existing equipment, missing engine object and missing operation slot are regression branches: no new row/KEY effect/ID change; preserve `objEntryN`'s possible lazy append on an in-range lookup. A direct branch test must isolate object absence from slot absence by fixture construction.

No current `FileEffects` API offers an interruption hook between the ID stores and final serialization, and `equipmentBytes` has no named injection seam. Final-state tests cannot establish this ordering on their own. Review the changed call sequence directly. Add an exception-prefix test only if an existing, real writer/effect seam can interrupt at that point; do not introduce a production observer or a synthetic throw that bypasses the actual serializer merely to make the test possible. Do not claim native failure parity from a passing recorded-effects test.

## Existing evidence and the unresolved failure contract

The independent CP246 review verified the retained decompile hashes `d0.c` SHA-256 `6D3703619843FB9AE1760F91E6633FB982C35CDC39D9DD2D89BEE7435477FBFF` and `d2.c` SHA-256 `CF3F99AF220CB92EC9CE8A19328640113333DF4C0194FAAC2B20B475BA2C6F57` against the import manifest. The original GUI binary retained for this static evidence has SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`; the historical product label is DEFORM v11.0.2. These are static original-source qualifications, not a fresh original execution trace.

`analysis/wave3-equipment-copy-investigation.json` and `analysis/wave3-equipment-fallback-contract.json` already establish the manager/copy outline. Manager vt+0x160 resolves and checks the source, selects an ID, calls vt+0x238, appends a row only on successful copy and returns `-1` otherwise. The copy helper attempts parent creation, handles an existing destination, and calls `QFile::copy`; the retained Qt static path includes `CopyFileW` followed on failure by source open, a temporary destination, streaming/length check, rename and cleanup. These receipts remain static-only. Do not repeat that manager inspection as the next task.

The current converter cannot express that failure: with non-null bytes `addEquipment` records mkdir/write, appends a row and returns a positive ID (`postload_views.cpp:91-104`). `equipmentBytes` writes a process-named scratch KEY, reads bytes and removes it (`:108-119`) rather than passing the original temporary source file to the manager. `FileEffects` separates mkdir and write vectors, and `applyEffects` replays them later by category. The next bounded investigation must reconcile the **original first KEY writer's status and temporary-source lifetime**, QTemporaryFile name/open/collision/auto-remove behavior, QFile rename fallback and Windows stream backend, and the continuation after original add returns `-1`: removal of source, movement copy, both path queries, both ID stores and final writer attempt. Bind conclusions to the retained original identity and keep physical filesystem outcomes unclaimed until static evidence or an appropriately gated fresh owned-original observation settles them. None of those gaps is repaired by this success-order extraction.

This report is read-only. No source, tests, build, shared state, original, GUI-control session or mutable REA state was changed or run.
