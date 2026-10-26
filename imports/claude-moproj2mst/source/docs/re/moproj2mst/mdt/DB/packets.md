# MDT DB: implementation packets

Spec: `spec.md` (this folder). Target: `apps/moproj2mst`, on the engine port in `port/`
(`defdbio` for every engine twin; nothing on this path needs a new DEF_PRE port). Order below is
the dependency order; DB-1 is the only prerequisite of the others.

## DB-0 Ledger fold (small, first)

* Add status rows for `1401cc420`, `1401cc2a0`, `1401cd020`, `1401d5f50`
  (`port/defdbio/src/dbread/lead.cpp`, p-dbread); their ledger rows still say `todo`.
* Correct spec_C section 7 / `mdt/ledger.tsv`: `1401de240`, `1401de110`, `1401ddfe0`,
  `1401df660`, `1401ddf60`, `1401cb400`, `1401c8f70`, `1401c5ea0` are ported now.
* Done when `fold-check` is clean.

## DB-1 Project DB handle (spec 1)

* New `DbHandle` (data root DB part): path (`DBH+0x28`), path-set, loaded flag (`+0x38`),
  `def::model::Cls_1401dbfe0 reader` plus the GUI extras (cache-started byte, last-modified stamp,
  owner) kept outside the engine class (spec Q2).
* `@src 0x140cdcdc0` setPath (".DB" suffix rule, "FOR003", cleanPath; `.ObjectColor.DAT` read not
  needed); `@src 0x140cdc480` open: `def::dbio::load_db_info` (`1401dc3f0`), `has_previous_part`
  (`1401e4c10`) -> "2G Split DBs" answered Yes with a warning -> `setup_part_chain` (`1401e4e80`),
  vt+0x78 `omitted: licence`, `load_header_cache` (`1401dd250`), loaded = 1.
* Hook into `openProject` (`0x14008a9b0`) before the walk with the default path
  (`setDefaultDbPath` value, spec 1.3 / Q3); `0x1400b9cd0` returns it for sub-project types != 4
  (type 4: own handle from `0x1400bbee0`).
* `GuiModel::db` becomes `DbHandle*` (`M+0xd8`); `0x140c7ac50` = its reader or null.
* `@src 0x140d397a0` refreshIfChanged / `0x140d395c0` reopen at operation open (spec_C 1.9).
* Replace the `qt::fileExists(primaryDbPath(p))` tests in `loader.cpp` and `l6_slots.cpp` by the
  handle's loaded state where the binary reads the handle (keep the refusals until their own
  packets land).
* Tests: tier-1 on the 9 MB `COG_P1_B1.DB` copy (directory count 32, cache op numbers), a split
  chain fixture if one exists, a missing file (loaded = 0, no error).

## DB-2 Step availability and choice (spec 2, 3)

* `@src 0x14080f690` DB operation list, `@src 0x1400b8b80` prefix match (stop at the end of the
  list: documented deviation, spec Q4), `@src 0x14005f520` dbHasStepsFor with OB vt+0x1f0
  (new class slot: true for Boolean, CopyMirror, Convert2Dto3D; spec 2.2).
* DB branch of `decideAndLoad` (`loader.cpp`): Setup Type dialog default Yes (warning), step via
  `op_last_step` / `op_first_step`, `step == 0` -> KEY load, NSTART / step / TNOW patches,
  HT-express reset (`1401e8fb0`), restart mode on, log line. Remove the OprFlag-0 refusal.
* Depends on DB-1, DB-3 (load) and DB-4 (sync); can be written in parallel and switched on last.
* Tests: unit tests of `0x1400b8b80` / `0x14005f520` on synthetic op lists and cache records
  (prev class with and without the flag, n = 1 and 2, lastMatch < idx and >= idx, Convert2Dto3D).

## DB-3 Step read into the model (spec 4, 5)

* `@src 0x1400572e0` loadDbStep, `@src 0x140c7b870` (step -> index `1401ddf60`), `@src 0x140c7b900`:
  S+0x1b58 = 1, `def::dbio::dbread::fun_1401cc420(S, reader, idx)`, header cache, furnace table /
  object env (`0x140c7d3a0`, `0x140c7ebe0`, ported), `viewerRebuild`, `state100 = 1`. UI calls
  dropped as listed in spec 5.
* Decide spec Q8 (failed read: follow the binary or refuse).
* Tests: read entries of the `COG_P1_B1.DB` copy into a `GuiModel`, compare object count, SC
  NSTART/TMAX and a material against DEF_PRE's KEY export of the same step (oracle cache
  `out/dbmap/oracle_keys.tsv` has KEY exports of both test DBs' steps).

## DB-4 GUI sync from the model (spec 6, 6.1)

* `@src 0x1400538f0` syncFromModel(op, updateDiagram, writeEqp): material items
  (`@src 0x140060c40`: name, `matType`, `phases`), Material KEY writes into `FileEffects`
  (`1401c5ea0` via the existing writer), slot loop (`addObjectAt`, `newObjectItem`, slot name,
  `0x140064d40`), equipment ids (`+0x1bf`, `equipmentId`, eqp manager new id), optional equipment
  KEY (`saveEquipment` with force), `refreshObjManager`, OB vt+0xc8 (base empty; DB-5), diagram
  rows `@src 0x140087270` (needs spec Q6 first: `0x14007b630`, `0x14007bf20`).
* Also unblocks the restart path (`0x140057230` -> `0x1400538f0(op, 1, 1)`, refusal in
  `decideAndLoad`): small follow-up, `GuiModel::readKey` exists.
* Tests: synthetic model with materials of types 0/1/2 and objects of kinds 5/7/other on slots
  -1 / -2 / positive; check items, slot records and the file list.

## DB-5 Class overrides of OB vt+0xc8 (spec 7)

* Convert2Dto3D `0x1401c3570` (reuse MDT L5's work model: `clearWorkObjects`, temp step KEY; the
  viewer-model branch follows L5's decision on `0x1402c8170(APP)`).
* MultiBlow `0x140602260` (props movement copy `0x14060b6b0`, two messages as warnings).
* Express family `0x1413fbd10`: needs a short analysis first (spec Q7: `vt+0x2c0`, `vt+0x2b8`,
  `0x1413fd0f0`, props setters) and the "Keep Mesh of Dies?" default (Yes inferred).
* Only reachable through DB-4 (and the restart path); each class can land separately.

## DB-6 Oracle B cases (after DB-2..4)

1. `DB1_HT_OPRFLAG0`: copy `C:\PROBLEM\TEST_REA\COG_P1_B1` (project + 9 MB `COG_P1_B1.DB`; chain
   HT, HT, Cogging, HT, HT, all OprFlag 1) into `gui_mo/oracle_b/<n>_B_DBSTEP_before`, set
   `<OprFlag>0` of operation 2 (SfOprHeatTransfer, restart-capable, not first). Before running,
   check with `deformdb` `dbinfo` / the header cache that op 1's stored steps carry operation
   number 1 and the op name of the chain's first operation (else `0x1400b8b80` fails and the GUI
   loads from KEY: still a useful negative case). Run: open, Enter on "Setup Type", select
   operation 2 if the active item is not it, Save. Expect: `Materials/Material*.KEY` rewritten,
   op 2's task folder (`SimCtrl.KEY` with NSTART `ns`, object KEYs from the DB step,
   `RestartStep.KEY`), `.moproj` (OprFlag 1, NSTART, restart `KeyFileName`), MST
   (`KFREAD 1 RestartStep.KEY` for op 2).
2. `DB2_HT_LASTOFPREV`: same project with operation 4 (HT after Cogging) at OprFlag 0: exercises
   `lastMatch < idx` vs `>= idx` depending on how far the DB's op list matches.
3. `DB3_NODB`: case 1 without the `.DB` (spec Q4: crash or KEY fallback).
4. `DB4_NAME`: case 1 with the `.DB` renamed to differ from the `.moproj` base and `<Database Name>`
   edited (spec Q3).
5. Optional `DB5_RESTART`: op 2 with restart mode on and a `RestartStep.KEY` (restart path,
   `writeEqp = 1`).
   The corpus (`gui_mo/s1/corpus`, 56 projects) ships no `.DB`; `oracle_b/120..122_B_DOE*` carry a
   copy of `COG_P1_B1.DB`. The 5 GB `Inc718_D420_GFM_V_DIES_Angle2_18.DB` (HT, HT, HT, Cogging...)
   is a second source if a larger chain is needed.
