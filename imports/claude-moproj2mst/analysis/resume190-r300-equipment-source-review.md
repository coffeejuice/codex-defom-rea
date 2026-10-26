# CP190 R300 equipment independent source review

PASS for the bounded equipment-loop lexical confinement and accompanying fixtures. No source-review blocker found. This accepts source integration; focused/full test execution and a source-stable freeze remain root responsibilities. It does not establish original-runtime equivalence or completion of R300.

## Reviewed boundary and immutable basis

Read the successor plan, independent plan review, implementation report, current owner files, `GuiModel::object`, `captureKey`/`writeTaskKey`/`saveEquipment`, Packet S sections 1–2, and reconstructed equipment writer. Independently compared both owner files against `build/wave3-checkpoint189-verified/source.zip` (entries below). Baseline hashes agree with the plan and implementation report.

| Archive entry / live source-relative file | CP189 SHA-256 | Reviewed CP190 SHA-256 |
|---|---|---|
| `apps/moproj2mst/src/engine/save_current.cpp` | `1f7a02ece2e6f240cf5ade0339d460432e8804ba908438922a5591823b0d0298` | `b0d62452408864276dde7825265ced96c1e5fc45cc1fb10b8c047530e84704a0` |
| `apps/moproj2mst/tests/s/test_save_io.cpp` | `61405f012034b143d8d25d15ab7fb18c595bf868faa5388dcee103991f0185a9` | `346f8454c0a9e4fd1f519d3824b63de99fdb971b70a2a36442f86b3d6e1592ab` |

The save source diff only wraps the equipment iteration in an immediately invoked lambda and replaces its two `continue` exits with lambda-local `return`. The test diff only inserts the new fixture and three tests before `native_equipment_denial_and_failed_flush_are_not_replayed`. That existing test and the entire following tail compare unchanged to the archive. Materials, CP189 restart callback, NSTART handling, `saveEquipment`, `equipmentKeyTarget`, and writer helpers are unchanged.

## Admission, alias confinement and sequencing

At save_current.cpp:739–749, the nonstored no-argument lambda executes once per ascending slot. Its inferred return type is void. `m->object(s + 1)` remains the first operation inside it, preceding `objEntry(op, s)` and both admission checks. The `eo`, record pointer and manager-row pointer are declared and used entirely inside the invocation. No local alias is returned or stored outside it. This is a lexical nonescape finding from source, not a consequence of the identity tests.

The cached model and earlier slot count remain in use. `GuiModel::object` at gui_model.cpp:210–215 still checks geometry-sync availability, checks for absent step, then performs its numbered lookup. There is no added model lookup, direct object-list substitution, `withStep` admission or early null-step exit. Both absent-record and absent-object paths return from this iteration only. Equipment ID zero and negative also return locally; later slots and all following side-file work still run.

The existing late `*m->step` expression remains at the admitted writer call after manager lookup; no callback argument acquires a step before the object lookup. Positive equipment IDs still query `p.equipment.find(eq)`. A hit passes cleaned manager path, a miss passes empty `String()` to the same writer. Equipment status remains ignored. Object-save result `r` is untouched and returned after materials, optional restart and NSTART. Equipment manager IDs are not substituted for the engine object number: unchanged `saveEquipment` reads the supplied object's integer at +8 for the serializer. No equipment type, format or overwrite policy changed.

These findings preserve the retained Packet S sequence. Packet S is an annotation source naming historical decompiles; this review did not obtain new caller assembly or original execution. Its wrapper return annotation (1) remains distinct from reconstructed writer failed-open mapping (3). Cached model lifetime, destructive callback reentry and later simCtl borrowing remain open R300 work; confinement does not pin ownership.

## Fixture and effects audit

`SyncEquipmentRig` at test_save_io.cpp:1673 owns `TaskKeyRig`, whose `Rig{true}` selects configured corpus49 `49_MO2_Boolean_Oprations`, `MO2_Boolean_Oprations.moproj`, and Task00001 (Rig:68–97; TaskKeyRig:1188). This is not the neighboring corpus46 mode. It requires a model/step, positive object slot, numbered object and record; owning project lifetime encloses both sync calls and hook. It changes equipment IDs, not engine object numbers, rejects all other extant records, then seeds ID 7 with an `ItemKind::Equipment` manager row. The positive path is under the rig scratch root's Equipments directory, includes `unused/..`, omits `.KEY`, and computes expected target through the existing clean-path/extension logic.

The positive test filters the expected target, requires one nonempty recorded write and one native attempt, compares native bytes to recording bytes, checks identities after both calls and during the equipment before-write hook, checks unchanged ID/manager row/path, requires successful overlay removal, and requires empty native deferred writes. Both recorded and native ordering require actual ObjProperties writes before equipment and actual Material writes after it and before restart. `REQUIRE(materials > 0)` prevents a vacuous material-order pass. The disposable enabled restart supplies a following boundary. These assertions are present in source; their runtime outcome remains unverified here.

Fresh fixtures separately test zero and negative IDs with recording effects and require restart continuation. The missing-manager positive-ID case uses only default recording effects, requires exactly one nonempty write to `equipmentKeyTarget(String())`, retains ID/model object/step, and requires restart continuation. It does not dereference the fixture's now-stale manager-row field. The cwd-derived fallback target is never passed to native effects in this test. `captureKey` at save_current.cpp:77 writes a disposable process/thread-named temporary KEY and records the requested destination, so recording does not open cwd `.KEY`. The positive native equipment target is beneath `M2M_TEST_SCRATCH` through the existing Rig scratch helpers; no new missing-file host probes were introduced.

Missing record and missing engine object are appropriately covered by source review, without manufacturing mutations that earlier sync stages might reject. No detached-step full-sync fixture, equipment-removal helper invocation, notifier behavior or manager refresh was introduced. Existing direct denied-open/failed-flush coverage is preserved unchanged, including ignored flush behavior and empty deferred writes.

## Integration limits

New tests use existing Testkit registration and CHECK/CHECK_EQ/REQUIRE macros, with no new includes or C assert/REQUIRE_EQ usage. This read-only review did not compile or execute tests and does not substitute for root's focused `m2m.s.save_io`/`m2m.s.save_current` group, required complete regression, or freeze gate. No public GUI Save or original-byte parity is claimed.

Only this report was written. No source/test, build artifact, shared plan/state, installed application, external fixture, GUI/app-control session or REA database was changed or opened.
