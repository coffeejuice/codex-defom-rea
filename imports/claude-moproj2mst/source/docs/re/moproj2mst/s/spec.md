# Packet S: save current operation (File > Save step 2)

Scope (coordinator, 2026-10-22; Aleksandr Q5 = a): the generic "save current operation"
`0x140052ec0` with the OPI sync `0x140057370`, the properties -> engine copy (props vt+0x60) and
`0x140054540`, and the engine KEY writers OB vt+0x68 reaches. Class-specific overrides of the save
slots (vt+0x68, +0xd0, +0xd8, +0xe8, +0x80) belong to the loader packets L1..L7
(`mdt/loader-packets.md`); until one lands its class refuses. Base: `claude/m2m-mdt` 94ec44d3
(loader L0). Decompiles: `deform-rea-work/gui_mo/out_s/s1c.c` .. `s9c.c` (strings annotated);
writer twins from `s1/mdt/fmatch.tsv` and, where its signature was not unique, by equal size and
callee list in `out/pre64_funcs.tsv`.

## 1. Call tree

```
0x140052ec0(opi)                                  save current operation, before the .moproj write
  ob = 0x140054e90(opi)                           class singleton (registry by OPI+0x120)
  if (!ob || !0x1405ef9d0(ob)) return             gate: ob->current (+0x20) and its project; the
                                                  feature test 0x1400aab60 is licence (omitted)
  "Save operation - %1" / "Done - save operation" messenger signals (UI, dropped)
  ob->vt[0x68]()                                  SfOprBase 0x1405ef510; own: Express 0x1413f8f60,
                                                  Pre/Pre2D, HeatTreat(2D), Report, ShapeOptim,
                                                  DoeOutput (L packets)
0x1405ef510(ob)
  opi = ob->current; if (!opi) return
  ob->vt[0xd0]()      base 0x1405f1e20: signalGetPropertyStackWidgetIndex (int, 0 unless a
                      receiver answers); 0 -> 0x1400543a0(opi) (props vt+0x60), else
                      signalUpdateDataInWidgetArea + 0x140054350 (props vt+0x58)
  0x1400543a0(opi)    props vt+0x60: properties -> engine model (table in section 3)
  ob->vt[0xd8]()      base 0x1405f22e0: if ProcType (0x143d766b8+0xc) == 5: 0x140054540(opi)
  ob->vt[0xe8]()      base 0x1405f2350: 0x140057370(opi)
  ob->vt[0x80]()      SfOprBase 0x141470184 = _purecall; Forming/Forming2D empty; others L packets
```

ProcType is 5 after File > Open of a `.moproj` (`0x140013d40(win, 0)`: project loaded as moproj
-> `0x1402cb780(&5)`; also `0x140010a70`, `0x14003a500`). The converter has no property widget, so
the widget index is 0 (the GUI's receiver `slotGetPropertyStackWidgetIndex`, connected in
`0x1400111c0`, is UI): props vt+0x60 runs twice, which is idempotent.

`0x140054540(opi)` (ProcType 5 only): `sub = 0x140055050(opi)` (active sub-project); `name =
OPI+0x118`; `no = 0x140050b20(opi)` (chain number, `0x140085800`); `list = 0x1400b8540(sub, no)`
(the sub-project's sim list, 0x1400b8360 = class vt+0x298 per operation in MST order, filtered to
entries whose operation number is `no`); with at least one entry: `simName = entry0+0x10`,
`sim = entry0+0xc`; when the engine sim control exists: `strncpy(sc+0x51 OPRNAM,
local8(name), 0x40)`, `sc+0xe8 CURSIM[1] = no`, `strncpy(sc+0x92 SIMNAM, local8(simName), 0x50)`,
`sc+0xe4 CURSIM[0] = sim`. With an empty list nothing is written.

```
0x140057370(opi) -> bool
  m = 0x14005f770(opi); if (!m) return false
  0x140051190(opi)            mkpath(task dir) (0x14081afb0, dialog "Can not make the directory
                              %1." on failure) and 0x14008c630(project): mkpath Operations,
                              Objects, Materials, Equipments, Lubricants (+ DieStress for a type-4
                              sub-project)
  0x1400597d0(opi)            SimCtrl.KEY + SimCtrl.PPV
  0x14005a8a0(opi)            InterObject.KEY (contact rebuild first when ICB+0x70)
  for slot s: if (code(s) > 0) r = 0x14005c760(opi, m, s)      Objects/ObjectNNNNN.KEY
  0x14005d3e0(opi)            MeshSettingsNNNNN.KEY for slots with OBJ+0x1ba
  0x14005e480(opi)            RzElmConNNNNN.KEY + BccDefNNNNN.KEY for slots with OBJ+0x1be and
                              OBJ+4 == 5
  0x14005ef80(opi)            ObjPropertiesNNNNN.KEY for every slot record
  for slot s: eo = engine object s+1, rec = OBJ(s); if both and rec+0x28 (equipment id) > 0:
      0x140358190(step, eo, eqpManager.vt[0x208](id), overwrite = 1)   equipment KEY
  0x140058cc0(opi)            Materials/MaterialNNNNN.KEY for every engine material of type != 2
  if (OPI+0x150 restart): 0x140c7d100(m, 0x140064a50(opi, abs = 1))    restart KEY (KFWRIT)
  if ((first op || OPI+0x150) && sc): OPI+0x18 = 1; OPI+0x1c = sc+0x160 (NSTART)
  return r                    the last object write's result (true without objects)
```

Paths (all `cleanPath(taskDirAbs + sep + name)`, `toLocal8Bit` for the engine's `fopen`):
`SimCtrl.KEY` (`0x140059290`), `SimCtrl.PPV` (`0x1400593b0`), `InterObject.KEY` (`0x14005a690`),
`"%1%2.KEY".arg(stem).arg(s+1, 5, 10, '0')` with stems `MeshSettings` (`0x14005cf70`), `RzElmCon`
(`0x14005cce0`), `BccDef` (`0x14005da90`), `ObjProperties` (`0x14005eb10`); the object file is the
object manager's `vt+0x208(code)` path; the restart KEY `cleanPath(taskDir + sep + OPI+0x158)`.

* `0x1400597d0`: `flags = 0xf`; `r = W_SIMCTRL(step, path, &flags)`; then the PPV list `OPI+0x20`:
  when it is not empty `0x140d4afe0` writes `SimCtrl.PPV` (QTextStream, CRLF): the count, then per
  point `i \t true|false \t ...`. The loader refuses projects with a PPV, so the port refuses a
  non-empty list too. Returns `r == 0`.
* `0x14005a8a0`: `icb = OPI+0x70`; when `icb[0]` (the "System" lubricant byte, s1-01 9.3)
  `0x14005a9b0` rebuilds the engine's inter-object list from the lubricant KEY and the class's
  default pairs (vt+0x198, section 4); always `0x1400655d0(icb, step)` (the INICTC flags from the
  engine's pairs); then `W_INTEROBJ(step, path)`.
* `0x14005c760`: `flags = 0x3000`; `W_OBJECT(step, path, s+1, &flags, 0)`.
* `0x140358190(step, eo, file, overwrite)`: append `.KEY` unless the upper-cased name ends with it,
  then `file = convertSeparators(absPath + sep + baseName + "." + suffix)`; with `overwrite` the
  "Overwrite File" question is skipped; `W_EQUIP(step, file, eo+8)`; returns 1.
* `0x140058cc0`: for `i = 1 .. materialCount(step)` (`0x140c7cae0` = step+0x1320): `mat =
  0x140830460(step, i)` (= `14029c260`, by position); when `mat` and `mat+0x1cc8 (int [0x732]) != 2`:
  `W_MAT(step, local8(matManager.vt[0x178](mat+0)), mat+0, 0, 0, 0)`.

## 2. KEY writers (GUI -> DEF_PRE twin -> body in port/)

All open the file with `fopen(path, "wt")` (CRLF), return 1 when the path is null/empty or the
open fails, close the file, and return the body's status. `B` = file banner `0x140895560` =
`1401c5a20` (`def::key::write_file_banner(f, 0, product, version)`), obj = `0x14084ed10(step+0x1380,
no)` = `14017aaf0` (object by number).

| GUI | twin | file | body |
|---|---|---|---|
| `0x140895920` W_SIMCTRL | `1401c5de0` | SimCtrl.KEY | `step+0x1744 = *flags`; B; `0x1409ec5a0` = `1403d4990` `write_group1` (ported) |
| `0x140896030` W_INTEROBJ | `1401c64f0` | InterObject.KEY | B; `0x140b4ff90` = `1403c9ad0` `write_inter_objects` (verified) |
| `0x140895f30` W_OBJECT | `1401c63f0` | ObjectNNNNN.KEY | `outNo = arg5 ? arg5 : no`; `step+0x1744 = *flags`; `0x140974000` = `14029e250` `g2obj::write_object(m, f, obj, outNo, 1)` (ported); no banner of its own |
| `0x140896360` W_MESHSET | `1401c6820` | MeshSettingsNNNNN.KEY | B; obj null -> 3; `0x14097ef30` = `1402a9180` (verified) |
| `0x140896280` W_OBJPROPS | `1401c6740` | ObjPropertiesNNNNN.KEY | B; `0x140981be0` = `1402abe30`: `1402abd50` then `1402a4020` (both verified), returns 0 |
| `0x140896640` W_RZELM | `1401c6b00` | RzElmConNNNNN.KEY | B; obj kind != 7 and has mesh: `1402a2db0`, `1402a30c0` (ported), 0; else 3 |
| `0x140896750` W_BCCDEF | `1401c6c10` | BccDefNNNNN.KEY | B; obj kind != 7 and has mesh: `0x140981c40` = `1402abe90`: `1402abf20`, `1402ac560` (ported), its status; else 3 |
| `0x140896840` W_EQUIP | `1401c6d00` | equipment KEY | `def::key::write_equipment_key_file` (MDT L0) |
| `0x1408959e0` W_MAT | `1401c5ea0` | MaterialNNNNN.KEY | `def::key::write_material_key_file` (MDT L0) |
| `0x140895350` | `1401c5810` | restart KEY | `def::cmd::write_key_file` (ported) |

W_RZELM and W_BCCDEF leave the file open when they return 3 (the original does not close it; the
handle leaks until exit, the file holds only the banner). The port closes it after the same bytes.

## 3. Properties -> engine (props vt+0x60, via `0x1400543a0`)

`sc` = the engine sim control (step+0x750), `P` = PROPS+0x30 common block (`data::CommonBlock`).
The four base helpers write only when their unit text evaluates (`0x1402ede00`):
`F80 = 0x1405fb980` (sc+0x16c NSTEP = P+0, sc+0x164 STPINC = P+4, sc+0x1d0 = P+0x40 cat 0),
`F90 = 0x1405fbd10` (sc+0x3d0 = P+0x20 cat 9), `FA0 = 0x1405fc060` (sc+0x4c8 = P+0x30 cat 0x20),
`FB0 = 0x1405fc5d0` (sc+0x240 = P+8; mode 0: sc+0x1e0 = P+0x38 cat 1, sc+0x1d0 = 0; else sc+0x1d0
= P+0x40 cat 0, sc+0x1e0 = 0).

| Class | vt+0x60 | Body |
|---|---|---|
| Base, Boolean, Cycle, DiestressBM, Diestress, Express | `0x1405fb610` | nothing |
| Forming(2D) | `0x140c09ed0` | F80, F90, FB0, FA0 |
| Distortion | `0x141366650` | F80, F90, FB0, FA0 |
| FormingExpress, MultiBlow | `0x14142a6f0`, `0x140612bf0` | F80, F90, FB0, FA0 |
| Cogging | `0x14075ee60` | F80, F90, FA0 |
| Pre, Report | `0x14134c3c0`, `0x1412b1f50` | F80, F90, FB0 |
| Group | `0x141352e80` | nothing |
| HeatTransfer, HTExpress | `0x141357590`, `0x1413ed900` | F80, F90, FA0, then own vt+0x70 (`0x1413573a0` / `0x1413ed710`: sc+0x1c8 = D+0 + 1, +0x1d0, +0x270, +0x2a0, +0x2a8 doubles, +0x164) |
| HTFurnace | `0x140c69520` | `0x140c69920`, vt+0x70 `0x140c691f0` (L2) |
| HeatTreat | `0x1411bfa20` | vt+0x70 `0x1411c05a0` and the schedule sync (L6) |
| Convert2Dto3D, CopyMirror | `0x1401ce590`, `0x140e2d230` | per engine object (kind != 7): movement block +0x20a0 (`0x1408ea190`), +0x2c38, +0x2be8.. (L5) |
| DoeOutput, ShapeOptim | `0x140e36320`, `0x140142d00` | DOE / optimisation data (L7) |

S ports the rows whose bodies use only F80..FB0 and the HeatTransfer/HTExpress vt+0x70; the
others refuse until their packet lands.

## 4. Inter-object rebuild `0x14005a9b0` (ICB+0x70 set)

`local = DB_STEP_DATA(3)` (`0x140821bf0` = `1401c8950`); `0x1408e6cc0(step+0x13c8)` (clear, =
`1401fade0`); `lub = OPI+0x48`; when `lub > 0`: `file = lubManager.vt[0x208](lub)`; when
`0x140897fd0(local, file)` (= `1401b7530` KEY import) succeeds: `defPair = 0x1408e6b70(local+0x13c8,
1)` (= `1401fac90`). `pairs = ob->vt[0x198]()` (base `0x1405f5c90`; Cogging, Express,
Distortion own). For each `(master, slave, ...)`: `o = 0x140830270(step, master)`; when `o`:
`0x140875a20(o, &pair, &flag, step+0x1640)` (= `140201bc0`); then without `defPair`
`0x14035e6f0(step, &master, &slave, d1, d2, flag)` else `0x14035e540(step, &master, &slave, defPair,
flag)`: append a pair (`0x1408e6980`, ctor `1401f46e0`), kind +0x170 = 2, master +0x168, slave
+0x16c, (+0x1c0, +0x20 = d1/d2 or a copy of `defPair` `0x1408e57e0`), +0x174 = flag in 3D, then
append its mirror (`0x140360d30`: swapped, kind 2 <-> 1).

`0x1405f5c90` (base default pairs): over the engine objects of kind != 7 count `n` and the
"master candidates" (kind not 0/3, or for a type-5 slot record +8 not in {0,3}); when `n > 1` and
exactly one candidate `c`: every other object `i` (not `c` in 3D) gives `(i, c)` with
`vt[0x1a0]` (`0x1405f60c0`) friction/IHTC from the properties and +0x19 of its slot record.
Ported in S for the base classes; the three overrides belong to L1/L3/L6.

## 5. Ledger

`docs/re/moproj2mst/s/ledger.tsv`.

## 6. Open questions

* Q1 The widget index (section 1) is taken as 0. If the GUI answered non-zero after open, props
  vt+0x58 would run first; it maps engine -> properties and back, so the KEY bytes do not change,
  but the property texts in the `.moproj` would be re-printed. Oracle B: no property text changed
  in the 34 saved projects except the documented ones, consistent with 0.
* Q2 W_RZELM / W_BCCDEF return 3 with the file open (section 2): the original leaves a file of the
  banner only; the port writes the same bytes.

## 7. Status (claude/m2m-s, based on claude/m2m-mdt 2f86ce0e)

* Ported: section 1 (base slots), section 2 (all nine writers, `port/defkey/src/item_writers/
  task_key_writers.cpp`), section 3 rows for Forming(2D), Distortion, FormingExpress, MultiBlow,
  Cogging, Pre, Report, HeatTransfer(2D) and the empty ones; the sim entry name with its cycle
  prefix (`0x1400682a0`, P14's `simcount.cpp`); `setEngineVersion` puts the start-up version text
  (`0x143dbcbd0`, the same text as the MDT header) into the engine banner global.
* Refused with M2M_E_UNSUPPORTED: SimCtrl.PPV (`0x140d4afe0`, the loader refuses PPV too), the
  system-lubricant contact rebuild `0x14005a9b0` (section 4), and every class whose own vt+0x68 /
  +0xd8 / +0xe8 / +0x80 or props vt+0x60 is not ported (list in `save_current.cpp`; owners L1..L7).
* Test `m2m.s.save_current` (oracle B): R11 and R19 (HeatTransfer, fresh copies of corpus 11 and
  19) PASS: the six files the GUI wrote in step 2 (SimCtrl, InterObject, Object00001,
  ObjProperties00001, Equipment00001, Material00001) byte for byte, and no other file. R46, R47
  (Boolean2D, loader L5), R52 (HeatTransferExpress2D, L3) and 59 (Forming, L3) are skipped until
  their loader slots land.

## 8. Save effects outside this packet (L4 and coordinator, oracle B 93_B_MB3ACT / 94_B_MB2ACT)

With a MultiBlow operation active the Save also drops equipment the active operation does not use
from `Equipment_List`, deletes its KEY files and sets those objects' EquipmentID to -1 (the MST's
first Forming block loses its IMPEQP lines), and the `.moproj` gets INICTC 2. The INICTC flags come
from `0x1400655d0` here; the equipment drop and delete are not on the `0x140052ec0` path traced
above and are open (the `.moproj` writer `0x1400983c0` builds Equipment_List with `0x1400a12f0`).
The DTTYPE switch to time per step in those saves comes from the KEY load (`0x1408969a0` ->
`1401d7f40` -> `1401ee510`), not from this packet. MultiBlow's own vt+0xd8 `0x140602780` is L4's.

## 9. Views step after the load: equipment sync (`engine::postLoadViews`)

Owner: packet S (coordinator, 2026-10-22). The loader `0x140051340` calls `0x1400547b0(op, 0)` after
the load; MDT L0 had dropped it as UI. Chain: `0x1400547b0` -> `0x1400548c0` (ProcType 5) ->
`0x140054950` -> OB vt+0xa0 (`0x1405f1b60`, true) -> OB vt+0xa8 (property panel model, per class:
Forming `0x140c06780`, Forming2D `0x140c05000`, MultiBlow2D `0x140600fc0`, MultiBlow3D
`0x1406019f0`) and OB vt+0x278 (`0x1405f15e0`, object tree model). Both call OB vt+0x228 (Forming
`0x140c07130`, Forming2D `0x140c04fa0`, MultiBlow2D `0x140600f30`, MultiBlow3D `0x140601960`):
tree states `0x1405eea60`, `0x1405eeab0`, `0x1405eebb0`, `0x1400c4670` (UI, `0x1400c3f50` item roles),
then OB vt+0x230, the only model effect:

* base `0x1401b20e0`: nothing.
* Forming / Forming2D `0x140c068c0` / `0x140c05200`: for each tree node (one per slot, node i ->
  slot i-1, engine object `0x140057690`) whose slot record has equipment id < 1: name
  `"Equipment (" + (rows + 1) + ")"`, the object's equipment KEY written to
  `QDir::tempPath()/<name>.KEY` (`0x140358190`, overwrite: `mov r9b, 1` at `0x140c06d6e`), added
  with `0x1400bf510` = SfMoEqpManager vt+0x160 `0x14006c610` (first free id `0x14006c230`, copy to
  vt+0x178 `0x1400bef30` = `Equipments\EquipmentNNNNN.KEY` through vt+0x238 `0x1400bf310`, row
  vt+0x240 `0x1400bfdf0`: name, +0x20 source, +0x28 path), temp file removed, engine object
  +0x2160 = name (strncpy 0x50), the KEY written again at the new path, slot record +0x28 = id.
  Then for the primary die (SC+0x168) the operation's Equipment Id (OPI+0x4c) = that id.
* MultiBlow `0x140607d10`: the primary die as above; every other object whose slot record has an
  equipment id > 0: SfMoEqpManager vt+0x1a0 `0x1400bff90` -> `0x14006ca50`: vt+0x1a8 `0x14006ccf0`
  deletes `vt+0x178(id)` (`0x14081a6e0`, QFile::remove), takeRow; slot record id = -1.

The temp file leaves no trace (removed before and after). Oracle B checked: 93_B_MB3ACT and
94_B_MB2ACT (before vs saved folders): Equipment00002/3 and 00001/3 deleted, Equipment_List
{1,4,5,6} / {2,4,5,6}, Equipment Id 1 / 2, slot EquipmentID -1, kept, -1 (test
`m2m.s.postload_views`, passes with L4 merged locally; skips until L4 lands). Creation has no
oracle case yet.

### 9.1 Dispatch and call site (after L2 and L4 review, 2026-10-22)

* Call site: SfOprBase vt+0x58 `0x1405ef430` calls `0x1400547b0(op, 0)` after props vt+0x58
  (`0x140054350`); the loader's baseLoad calls `engine::postLoadViews` there (one call site).
  Classes with their own vt+0x58 (Pre, Pre2D, Cycle, Group, DoeOutput, ShapeOptim, Report, ...)
  reach it only if their body calls it.
* `postLoadViews`: OB vt+0xa0 (false for DoeOutput `0x140e30e50` and Report `0x1412af6b0`), then
  `OpClass::objectTreeFill` (vt+0xa8, base `0x1405f1b70` empty) and, unless vt+0xa0 was false,
  `OpClass::objectTreeAdd` for vt+0x278 (base `0x1405f15e0` calls vt+0x228 first). Class packets
  own their vt+0xa8 and vt+0x228 bodies; a vt+0xa8 override that calls vt+0x228 calls
  `objectTreeAdd` itself.
* vt+0x230 (`engine::equipmentSync`, also `mst::g_equipmentSync` for src/ops) runs only from the
  vt+0x228 overrides: Forming / Forming2D here (`FormingClass` in family.cpp: vt+0xa8 `0x140c06780`
  / `0x140c05000` -> vt+0x228 `0x140c07130` / `0x140c04fa0` -> vt+0x230); MultiBlow is L4's.
* Forming vt+0xc0 `0x140c06130` (L3's slot) creates the primary die's equipment when the operation
  has none, through the same `createEquipment`.
* Oracle B 96_B_FNOEQ (corpus 57, active Forming with Equipment Id -1): Equipment00007.KEY byte for
  byte, ids 1..7, Equipment Id 7, slots 2, 7, 3 (`m2m.s.postload_views`); 59_B_HT3 (Forming
  active) now passes `m2m.s.save_current` (13 files). 93/94 wait for L4's overrides.
