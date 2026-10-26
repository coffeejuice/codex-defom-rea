# MDT L6: loader slots of HeatTreat, Distortion, Diestress and DiestressBM

This packet covers the operation-loader slots (`loader-packets.md`, L6) of SfOprHeatTreat(2D),
SfOprDistortion, SfOprDiestress and SfOprDiestressBM(2D/3D). The code is in
`apps/moproj2mst/src/engine/l6_slots.cpp`. The class overrides are in `src/ops/heattreat.cpp`,
`distortion.cpp` and `diestress.cpp`, and they call through the hook table
`include/m2m/mst/loader_l6.hpp`. The tests are in `tests/mdt/test_l6_slots.cpp`. The decompiles
are in `deform-rea-work/gui_mo/s1/mdt/L6/` (`d0.c` holds the slot bodies, `d1..d11.c` their
callees, `a*.asm` the disassembly). The Ghidra copy is `gui_mo/gproj_L6` (`run_L6.sh`).

## 1. Slot table (from `gui_mo/s1/vtables.tsv`)

| Class | vt+0x58 | vt+0xb8 | vt+0x78 | vt+0x1b8 | vt+0xc0 | vt+0x1b0 | props vt+0x58 | vt+0x1f8 |
|---|---|---|---|---|---|---|---|---|
| HeatTreat / 2D | `1411b1240` / `1411aec20` | `1411b11d0` / `1411aebb0` | empty `1411b1220` / `1411aec00` | base | base | base | `1411bfd60` | true |
| Distortion | base | `1413627d0` | empty `141362890` | base | `1413628b0` | base | `1413665c0` | false |
| Diestress | base | `1402b59a0` | empty `1402b63b0` | `1402b59e0` | `1402b60f0` | `1402b5600` | base (nothing) | false |
| DiestressBM2D / 3D | base | `140299230` | `140299a90` | `14029b570` / `14029d3b0` | `140299740` | `14029b440` / `14029d280` = base | base (nothing) | false |

The packet adds `OpClass::loadData` for vt+0x1b8. Its base body is `0x1405f6520`, which is
`engine::baseLoadData`, and `engine::baseLoad` calls it. The packet uses L7's `createModel` for
vt+0x1b0.

## 2. Bodies (one-line summaries; the code follows the decompile line by line)

* Prologues. Each runs the base `0x1405f1bb0`, then calls the OB sim-control setters
  `0x1405ef6f0` (+0x108), `0x1405ef750` (+0x104), `0x1405ef7b0` (+0x110), `0x1405ef810` (+0x10c),
  `0x1405ef870` (+0x16c, +0x164, +0x160, +0x1d0) and `0x1405ef900` (+0x2e0).
  * HeatTreat sets TRANS2, +0x110 and +0x10c to 0 and TRANS1 to 1.
  * Distortion sets TRANS2 to 1 and TRANS1 to 0. It sets the steps to 2/1/-1 and DTMAX to 1.0,
    then sets ITRMTH = 1 and SOLMTD = 1.
  * Diestress and DiestressBM set TRANS2 to 1 and TRANS1 to 0.
* HeatTreat vt+0x58 runs the base load `0x1405ef430`, then the props cast, then `0x1411b4770`.
  `0x1411b4770` calls `0x1411b4870`. When schedule+0 is clear, that function first applies the
  engine defaults `0x1411b3540` (title "HEAT TREATMENT", +0x16c = 999999, ...) and then opens
  `HeatTreatWizard.HTWZ` with fopen "rt".
* HeatTreat props vt+0x58 `0x1411bfd60` runs vt+0x68 `0x1411c0730`, which copies the sim
  control into the +0x58 block. Then `0x1411c0980` clears the lists, the per-step loops run, and
  block+4 = n. The block is `data::HTPropsBlock` (`include/m2m/data/l6_blocks.hpp`). The save path
  reads it back through vt+0x70 `0x1411c05a0` and vt+0x60 `0x1411bfa20`.
* Distortion vt+0xc0 `0x1413628b0`: an operation without objects gets one object through L7's
  `0x140057ba0`, and that object gets kind 4 and +0x2154 = 1 unless its kind is 5. Otherwise, when
  the recorded workpiece object (+0xb4) is undefined (kind 7), the number of the last inherited
  slot object (kind 5) becomes the workpiece.
* Distortion props vt+0x58 `0x1413665c0` runs the base readers `0x1405fb720`, `0x1405fbae0`,
  `0x1405fc180` (which calls `0x1405fb450`) and `0x1405fbe30`. These fill the common block from the
  sim control. The value strings are `"%1 %2".arg(v, 0, 'g', -1).arg(unit name)`. The reader also
  sets props+0x38 +0x10/+0x14 to the fixture and pass counts (`data::DistortionCounts`, ctor
  `0x141369120`).
* Diestress / DiestressBM vt+0xc0 (`0x1402b60f0` / `0x140299740`, identical code): for each object
  position, an object whose kind is not 7 has its name copied to the slot through `0x1400585e0`.
  That function sets the object item's text, then the object's name (LLOBJ vt+8), then calls L7's
  `0x140060100`. After the loop, TRANS2 = 1 and TRANS1 = 0.
* Diestress vt+0x1b0 `0x1402b5600`: the model's dimension comes from the project DB header
  (`0x14081fd60`). That function's Fortran OPEN uses STATUS 'OLD', so without the file it returns 0
  and the dimension is 3. The model also needs the active sub-project's DB handle (mainWindow
  vt+0x238). There is no TNOW3 store.
* Diestress vt+0x1b8 `0x1402b59e0` takes one of three paths:
  * When the sub-project DB exists, the model loads its last step (`0x140c7b790`).
  * Otherwise, when `<db base>.KEY` exists, that KEY file is read.
  * Otherwise `0x1402b6870(step 0)` runs on a scratch model. It resets the model, sets the scratch
    model's dimension, clears the slot list (`0x140060eb0`), sets props step number = 0, copies
    the materials, PDIE and GEOTYP, sets NSTART = `0x1402b5e50` = -1, sets NSTEP/STEPINC from the
    contact pairs (`0x1402b5d50`) when unset, sets SOLMTD = 2 (3D) / 1 and DTMAX = 1.0.

  In every case `0x1402b7730` then sets every node temperature to 20 (SI) / 68.
* DiestressBM vt+0x78 `0x140299a90` reads `DieStressBmWizard.Wzd` with P06's reader. When the open
  fails, it applies the defaults `0x1402a15d0` and then `0x1402a2910`. Because vt+0x78 runs on the
  freshly reset model, the object count is 0. The DB name becomes "DieStressDb.DB".
* DiestressBM vt+0x1b8 (`0x14029b570` / `0x14029d3b0`): when flow number ≤ 1 it does nothing,
  otherwise it picks the DB step.

## 3. Refusals (M2M_E_UNSUPPORTED)

| Where | Why |
|---|---|
| `0x1411b4870` when HeatTreatWizard.HTWZ opens | `0x1411b4ab0` / `0x1411b5e50` write media, ECC and ECTMFN records into the loaded engine model. P16's reader keeps only the schedule. |
| `0x1411bfd60` with schedule steps | The per-step loop copies media record names (schedule+0x08), which P16's schedule does not keep. Not reachable after the refusal above, because steps come only from the HTWZ. |
| Diestress model with the project DB present | `0x14081fd60` reads the DB header; that read is not ported. |
| Diestress vt+0x1b8 with the sub-project DB present | `0x140c7b790` loads a DB step. |
| `0x1402b6870` with scratch objects | `0x140835430` object copy from a DB step. Not reachable without a DB. |
| DiestressBM vt+0x1b8 after the first operation | Without a DB, `0x14005f270` → `0x1400b8b80` indexes the empty list of DB operations unchecked. With a DB it loads a DB step. |
| `0x1402a2910` with more than 15 objects, a null object at a position | The original writes past the arrays or dereferences the null. |

## 4. Tests

The corpus has no operation of these classes, so `tests/mdt/test_l6_slots.cpp` re-classes the
cogging operation of 42_vdie (op 4) and gives it the new class's props. It has six cases:
* HeatTreat with the schedule flag set (block = SC).
* HeatTreat with the flag clear (defaults applied, block reads 0.001 / 5.0).
* HeatTreat with an HTWZ in the overlay (refused).
* Distortion props (common block = SC).
* Diestress without a DB (slot list cleared, NSTART -1, SOLMTD 2, DTMAX 1.0).
* DiestressBM after the first operation (wizard defaults, then refused).

## 5. Open items

* There is a second body for P16's `0x1411b4770` / `0x1411b4870` / `0x1411b3760`. P16's version
  parses the HTWZ without engine writes, while the loader needs either the engine-writing reader
  or a refusal. One owner should keep a single body that can do both.
* The ctor defaults of the HT props +0x58 block (ctor `0x1411bd570`) and of DiestressBM +0x111 are
  not decompiled. No value on the load path depends on them (section 2).
* There is no oracle B case yet. A project with a HeatTreat, Distortion or Diestress operation as
  the active item would check the whole chain.

## 6. The HeatTreat schedule (one body since MDT L6, moved from P16)

`src/ops/heattreat_htwz.cpp` (`m2m/ht_schedule.hpp`) is the only body of `0x1411b4770`,
`0x1411b4870`, `0x1411b3760`, `0x1411b3540`, the readers `0x1411b4ab0` (V3) and `0x1411b5e50`
(V1/V2), and the props refresh `0x1411bfd60` with vt+0x68 `0x1411c0730`. The open path (props
vt+0xc0 `0x1411c0940`), the loader (HeatTreat vt+0x58) and P16's MST writer all use it.
* The file is read with the CRT (`fopen "rt"` on a scratch copy of the bytes, so overlay files
  work too). The DEF_PRE KEY helpers do the parsing; they are the twins of the GUI's `0x140af04f0`,
  `0x140af20b0`, `0x140af7030` and `0x140af73f0` (`fmatch.tsv`).
* The schedule (`m2m/data/heattreat.hpp`) now keeps everything the reader builds:
  * the media (name, radiation, ACVCOF table, a CNVCOF table per zone);
  * the ECC and ECTMFN node-value records;
  * the steps with their two DYNARY tables (+0x20 / +0x98, which also hold t3 / t4 as constants)
    and their pair records.
* `0x1411b3540` writes the HEAT TREATMENT defaults into the owner's engine model the first time the
  schedule is read. It runs at open, because the model is made on demand by `0x14005f770`.
* `0x1411bfd60` resets an out-of-range step medium to 0 and fills the +0x58 block lists with the
  per-step time, medium name, t3, t4 (when block+1 is set) and medium radiation flag.
* Refusals:
  * a V1 2D zone region line (`0x140a9e8f0` = DEF_PRE `1403056a0`, not ported);
  * an ECTMFN record without an engine model (the original dereferences a null model);
  * ECCTMP / ECTMFN widths above 1 on an object with faces (`0x1409c8020` allocates one int per
    face and `0x140af7030` writes width ints per face).
* P16's tests (`tests/p16/test_htwz.cpp`) use real engine models now (a fresh DB_STEP_DATA with
  object 1), and they check the kept tables and records.

## 7. Save-current-operation slots (L6S)

Packet S's `saveCurrentOperation` (`src/engine/save_current.cpp`) dispatches these. The bodies are
in `src/engine/save_l6.cpp`; the vt+0xa8 overrides are in `l6_slots.cpp`.

| Class | vt+0x68 | props vt+0x60 | vt+0xd8 | vt+0xe8 | vt+0x80 | vt+0xa8 |
|---|---|---|---|---|---|---|
| HeatTreat / 2D | `1411b1320` / `1411aed00`: base, then `1411b71c0` and `1411b3b20` | `1411bfa20` (+ vt+0x70 `1411c05a0`) | base | base | empty | UI only |
| Distortion | base | `141366650` (packet S) | base | base | empty `1413628a0` | UI only |
| Diestress | base | `1405fb610` (nothing) | `1402b6220` = base | `1402b5be0`: model as `<DB base>.KEY` | empty `1402b63c0` | `1402b6410` rename loop |
| DiestressBM | base | `1405fb610` | `140299870`: rigid objects' movement (`14084e770`) and node values (`140840bc0`) reset, then base | base | `140299b60`: wizard block, then `DieStressBmWizard.Wzd` | `140299c50` rename loop |

* The wizard file is written by `0x1402a16c0` as a Qt 4.8 QDomDocument (`toByteArray(1)`, text
  mode). The attribute order is the QHash iteration order: qHash of Qt 4.8, 17 buckets. Oracle B
  `62_B_BM2` confirms the root element order (qsNewDbName, nObjTotalNum, bUseNewDb). Doubles are
  written with `%.16g`, using the MSVC CRT through `def::rt::crt::snprintf`.
* HeatTreat vt+0x68 after its base (L6T, `src/engine/ht_save.cpp`): `0x1411b71c0(sched, 0)` runs
  the 2D ECC/ECTMFN fill `0x1411bb040`, the HT_Proc<k>.MOKEY writer `0x1411b73c0` (ECTMFN prune
  `0x1411ba540` first; the KEY blocks go through the DEF_PRE twins `write_table_body`,
  `g2bc::write_edge_face_int_line` and `g57::write_phase_pair`) and the read-back `0x1411b9080`
  (first step's MOKEY into the model with its phase pairs kept, the user KEY into a scratch
  DB_STEP_DATA(3), volume fraction check `0x1411b9e70`; called with dialogs = 0, so every question
  takes the non-dialog branch). Then `0x1411b3b20` writes HeatTreatWizard.HTWZ (`0x1411b3c40`).
  Refused: a 2D object without its boundary-edge ECC array (`0x140891660` = DEF_PRE
  `14021d760`/`14021d7a0`, not ported), face nodes out of range, a PHASE line after an empty object
  position. No Oracle B case has HeatTreat as the active first operation yet, so this path is
  checked only by building; the reader side (`test_htwz`) is unchanged.
* Save step 3 (props vt+0xb8, after the .moproj; `engine::l6PropsSave`): DieStress.Wzd
  `0x1402b9050` (QDomDocument, `src/datafiles/qdom_write.cpp`), DieStressBmWizard.Wzd `0x1402a16c0`,
  MachDistortion.MDWZ `0x141367b80` and DEF_MACHDIST_CTRL2.INI_<i> `0x141366d60` (free-distortion
  BCC block `0x1411638a0` only in 3D, OBJPOS records `0x141367940`), HeatTreatWizard.HTWZ
  `0x1411b3b20`. Distortion3D/2D now have their class-init dimension flags (`0x141362050`: 0x1a /
  0x19), which the INI writer's 3D test (`0x14005f1c0`) reads.
* Oracle B: cases 98_B_DS3F, 99_B_BM2F, 100_B_BM3F, 101_B_DIS3F and 102_B_DIS2F have the L6
  operation active and first. `test_save_l6` checks step 2 (files in the second before the .moproj)
  and step 3 (the L6 data files at or after it): all byte-identical.

## 8. HTFurnace Load Pattern editor (L6T)

The HTFurnace object tree shows its first item, "Load Pattern" (SfPreLoadPattern), after the open.
Its ctor `14106e8b0` loads :/SfOprHTFurnaceForm/SfOprHTFurnLoadPattern.ui. When SC+0x3c0 (ENVTMP
furnace) is set, it reads table 14 into the model's block +0xf8 (`140c7d3a0`), then fills the form
from the block (`141070920`, which needs block+0 >= 0). Spin boxes are clamped to 1..1000 and
double spin boxes to 0..999999999 with 2 decimals. Save's vt+0xd0 sends
signalUpdateDataInWidgetArea to it: `141070870` applies the form (`141071060`) and rebuilds
table 14 (`140c7d8d0`), but only when the editor's modified flag (+0x35) is set. Only the form's
own signals set that flag, so a conversion never applies it, and 87/107 keep table 14 as read. The
caller then runs props vt+0x58 before vt+0x60. Oracle B `119_B_HTF_LPEDIT` (sbRows 1 -> 2 by
clicks) is reproduced in `test_save_l6` by editing the form and setting the flag. SimCtrl.KEY is
byte-identical. Refused: block+0 < 0, where the form would keep the node-box distances of
`1410703a0` (not ported).
