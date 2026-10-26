# Inherited object DB read

This correction replaces the blanket refusal at `0x14005cc00` with the original bounded DB-object read sequence. It preserves the existing KEY/provider fallback and the caller's object state and viewer synchronization order. It changes no public headers or `m2m_*` ABI.

## Evidence and qualification

The contract comes from retained [b1a.c](../evidence/gui_mo/s1/mdt/L0/B/b1a.c), [b2a.c](../evidence/gui_mo/s1/mdt/L0/B/b2a.c), [load_d6.c](../evidence/gui_mo/s1/mdt/load_d6.c), and the inherited [specification](../source/docs/re/moproj2mst/mdt/L0/spec_B.md#42-0x14005cc00-objectfromdbop-no-sim-bool-last-dst-and-0x14005c890). The specification alone was insufficient: `b2a.c` loses the return value and one stack argument. Offline DUMPBIN disassembly of exact `.pdata` function ranges resolves these issues. The [static receipt](wave2-db-inheritance-static-evidence.json) records original target SHA-256 and function-byte hashes; [machine-readable contract](wave2-db-inheritance-contract.json) adds hashes for evidence and implementation files.

No original process or copied original function was executed by this worker. No new full GUI equivalence is claimed. The coordinator owns builds, tests, original acquisition and corpus comparison.

## Original sequence

| Address | Established behavior | Implementation |
|---|---|---|
| `0x14005b2f0` | A non-head inherited slot with a resolved positive ancestor code requests `simNo(op)-1`, `last=true`. | Existing `loadInherited`, now calls the recovered helper. |
| `0x14005cc00` | Obtain operation model/reader; refresh before checking `sim<1`; ignore refresh result; reject step zero. | `inheritedObjectFromDb`; deliberately has no loaded-state gate. |
| `0x14080eb30` / `0x14080ea20` | Last/first saved step for simulation; existing DEF_PRE query twins. | `sim_last_step` / `sim_first_step`. |
| `0x14005c890` | Resolve **active subproject** path, check file, create independent `DB_STEP_DATA(3)` and local reader, open with local8 path and `headerOnly=false`. | `copyInheritedObjectFromDbStep`. The mutable P17 helper preserves generated type-4 `DbName`. |
| `0x140814e80` / `0x1408150f0` | Chain preceding DB parts when present; no question on this path. | Existing `has_previous_part` / `setup_part_chain`. |
| `0x140825540` | Raw step read has five arguments: temporary, reader, step, `1`, `0`. | Existing `fun_1401cc2a0`. |
| `0x140830270` / `0x140835430` | Find requested object number; copy with temporary array sizes and flags `1,1,1,1,0,1`. | Existing `object_list_find` / `llobj_copy_from`; name copies and destination number remains. |
| `0x14087e1a0` | Rebuild BCC groups only after successful copy. | Existing `bcc_groups_from_model`. |

The step query and actual object read use different readers. The query reader belongs to the operation model. The object reader opens the active subproject's resolved DB path independently. Substituting a project reader or using the query reader's path for both steps would change the original behavior.

Missing model/reader/subproject/file, DB-info failure, step-read failure or missing object returns false and continues through the existing KEY/provider path. A successful DB copy bypasses its initial manager KEY existence check. Original success is saved across cleanup at `0x14005cb92` and restored into `AL` at `0x14005cbcf`, as shown in [the exact disassembly](wave2-db-inheritance-14005c890-disassembly.txt).

The caller still marks type 5, records the previous type in `ObjEntry.f08`, applies its existing mesh/BCC/properties sequence, and finally calls `viewerSync`. The bare path retains its earlier return before those later steps.

## Focused validation

The new [test file](../source/apps/moproj2mst/tests/mdt/test_db_inheritance.cpp) uses public `loadFromKey` with two operation items. A synthetic query cache isolates step selection; actual object data comes from a scratch copy of `COG_P1_B1.DB`. Its step `-2` object name `Workpiece` is independently qualified by the existing DEF_PRE KEY-export fixture. A scratch KEY renamed `FILE_ONLY` distinguishes fallback from successful DB reading.

Four test cases cover five scenarios: last-step selection despite failed refresh and unloaded handle, active-subproject path selection, missing DB fallback, missing stored step fallback, and refresh before rejecting simulation zero. The success test also compares DB bytes and modification time. Fixtures and frozen corpus are untouched. No CMake edits are needed because the existing test glob and fixture environment apply.

The first coordinator compile exposed a missing include for the existing mutable path overload. Adding `m2m/p17.hpp` resolves the declaration without changing the API. The coordinator subsequently reported the combined DB/DOE v3 full build and all 96 CTests passed. It also added the new test file's `testkit::run_all()` main; the machine-readable source hash has been refreshed.

## Remaining limits

No separate new split-DB, missing-object or malformed-DB-info fixture is included. Those branches follow direct original guards and reuse existing primitives. The first-step mapping is static evidence; this caller always requests the last step. General DB format completeness, inherited BCC-definition refusal and external 2D-to-3D conversion remain outside this correction.
