# CP190 R300 equipment scoped-borrow implementation

The equipment pass in `syncOperationFiles` now confines each slot's engine object, OBJ record, equipment ID and manager row to an immediately invoked, nonstored void callback. The callback acquires `m->object(s + 1)` before the record lookup, preserving `GuiModel::object`'s geometry-availability check and absent-step guard. Its local returns replace only the former loop continues. The cached model, ascending slots, late `*m->step` expression at the writer slot, manager-miss empty String fallback, ignored writer status and overall object result remain unchanged. No model lookup, null policy, helper/API or serializer was introduced. Materials, CP189 restart callback and NSTART remain unchanged.

## Baseline reconciliation

Before any edit, both live files were compared byte-for-byte with `build/wave3-checkpoint189-verified/source.zip`, using entries `apps/moproj2mst/src/engine/save_current.cpp` and `apps/moproj2mst/tests/s/test_save_io.cpp`. Both matched. Baseline SHA-256 values were respectively `1f7a02ece2e6f240cf5ade0339d460432e8804ba908438922a5591823b0d0298` and `61405f012034b143d8d25d15ab7fb18c595bf868faa5388dcee103991f0185a9`. The source change is confined to the equipment-loop callback. The test change only inserts the new fixtures/tests before `native_equipment_denial_and_failed_flush_are_not_replayed`; that test and every following test remain byte-for-byte unchanged relative to the archive.

## Fixture coverage and preflight

`SyncEquipmentRig` owns a disposable `TaskKeyRig`, whose `Rig{true}` copies configured corpus49 `49_MO2_Boolean_Oprations`, loads `MO2_Boolean_Oprations.moproj` and Task00001. It requires a loaded model/step, positive object slot, numbered object and record. It leaves object numbers unchanged, rejects other records with negative equipment IDs, and seeds ID 7 plus an Equipment manager row. The row's path lies beneath the rig's scratch Equipments directory, contains `unused/..` and lacks `.KEY`; expected output is computed through `equipmentKeyTarget(cleanPath(...))`.

- Positive sync records exactly one nonempty equipment write, then executes native sync and compares bytes. The native hook checks stable model/step/object/record/manager identity and counts the equipment attempt. Both recorded and native order require ObjProperties before equipment and at least one actual Material write after equipment, followed by a disposable restart. Native success must erase the seeded overlay and leave deferred writes empty.
- Separate fresh fixtures exercise ID 0 and ID -7, requiring no selected equipment-target write while restart is still recorded.
- A recording-only positive-ID missing-manager fixture requires exactly one nonempty write to `equipmentKeyTarget(String())`, verifies sync continues to restart, and preserves ID/step/object. It never executes native effects for that cwd-derived destination. Recording uses the existing disposable capture KEY, without touching the recorded `.KEY` destination.

The missing-record/missing-engine-object admission remains a source-review case: object lookup still precedes record lookup and either absence returns only from the current callback. No fixture mutates those relationships across earlier Save stages. No detached-step full-sync fixture was added.

Preflight checked the existing Testkit `main`, existing resolved engine/internal/Testkit includes and supported CHECK/CHECK_EQ/REQUIRE macros. There are no new includes, REQUIRE_EQ calls or C assert calls. No build or test was executed; root owns focused target registration confirmation, focused `m2m.s.save_io`/`m2m.s.save_current`, complete source-stable regression and freeze.

## Evidence limits

Read Packet S sections 1–2, `GuiModel::object` and reconstructed equipment writer. Retained Packet S annotations identify caller `0x140057370`, model getter `0x14005f770`, equipment wrapper `0x140358190`, writer `0x140896840` and twin `0x1401c6d00`. They support the retained order/admission/path behavior but are not newly recovered caller assembly or original-runtime equivalence evidence. Current reconstructed writer returns 3 on file-open failure; the sync caller ignores its result. Existing direct native denied-open/failed-flush coverage remains intact.

Static source inspection establishes lexical nonescape of slot-local aliases; identity assertions alone do not. This is not ownership pinning, destructive-reentry protection or the full R300 lifetime barrier. Cached `m` and later simCtl borrowing remain open.

No build, full suite, GUI/app-control session, original process, REA database, shared plan/state, deployed binary or external fixture was changed. Exclusive source/test ownership is released to root for review/integration.

## Released source hashes

- `source/apps/moproj2mst/src/engine/save_current.cpp`: `b0d62452408864276dde7825265ced96c1e5fc45cc1fb10b8c047530e84704a0`
- `source/apps/moproj2mst/tests/s/test_save_io.cpp`: `346f8454c0a9e4fd1f519d3824b63de99fdb971b70a2a36442f86b3d6e1592ab`
