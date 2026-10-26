# MDT L5: loader slots of SfOprBoolean, SfOprConvert2Dto3D, SfOprCopyMirror

Packet L5 of `../loader-packets.md`, branch `claude/m2m-mdt-L5` (base `claude/m2m-mdt` 94ec44d3).
Code: `apps/moproj2mst/src/engine/l5_slots.cpp` (bodies), the class overrides in
`src/ops/boolean.cpp`, `convert23.cpp`, `copymirror.cpp` (through the hook table
`include/m2m/mst/loader_l5.hpp`, so the standalone P12 writer test still links without the engine),
`include/m2m/data/convert.hpp`. Test: `tests/mdt/test_l5_loader.cpp`. Decompiles and disassembly:
`deform-rea-work/gui_mo/s1/mdt/L5/d0..d8.c`, `a1..a4.asm` (Ghidra copy `gui_mo/gproj_L5`).

## 1. Slot table (vtables.tsv)

| Slot | Boolean | Convert2Dto3D | CopyMirror |
|---|---|---|---|
| vt+0x58 load | base `0x1405ef430` | `0x1401c3a60` | `0x140e24b80` |
| vt+0xb8 prologue | `0x1411def50` | `0x1401c2970` | `0x140e22900` |
| vt+0x78 class data | `0x1411df000` (empty) | `0x1401c29b0` (empty) | `0x140e22940` (empty) |
| vt+0x1b8 KEY/DB | base `0x1405f6520` | base | base |
| vt+0xc0 post-load | `0x1411df020` (no base call) | `0x1401c29d0` (base first) | `0x140e24b50` (base first) |
| props vt+0x58 | base `0x1405fb600` (nothing) | `0x1401ce3e0` | `0x140e2d080` (same body) |
| vt+0x1f8 restart | false | true | false |
| vt+0x2a0 provide object | base `0x1405f5b70` | base | `0x140e25980` |

## 2. Boolean

* `0x1411def50`: base prologue `0x1405f1bb0`, TRANS 2 = 1 (`0x1405ef6f0`), TRANS 1 = 0
  (`0x1405ef750`), `0x1405ef870(0, 1, -1, 1.0)`: NSTEP +0x16c = 0, STPINC +0x164 = 1, NSTART +0x160 =
  -1, DTMAX 1 +0x1d0 = 1.0 (`0x1421249e0`); ITRMTH +0x2e0 = 1 (`0x1405ef900`); SOLMTD +0x2e4 = 1.
* `0x1411df020`: props (`0x140051230`) dynamic_cast to SfOprBooleanProperties, data +0x38; n = size
  of OPI+0x50 (`0x140061220`). n = 0: `0x140057ba0` adds an object (refused, as in L0) and
  `0x140058aa0(opi, 0, 5)`. Otherwise when object `workpiece` (data+4) exists with kind 7, every slot
  i < n whose object (`0x140057690`: own or inherited slot) has kind 5 sets workpiece to that
  object's number (`0x1413fd9d0`); the last one wins. `0x140cbc690` (cutter) is read and dropped.

## 3. Convert2Dto3D

Class members: +0x130 byte (1 around the base load), +0x138 the 3D work model (`SfStepData`).

* `0x1401c3a60`: +0x130 = 1, `0x1405ef430`, +0x130 = 0; with an operation, its model and the main
  window's GL scene (`0x1402c8170` = main window vt+0x778 `0x140c9bbd0` = +0x120, made at start-up by
  `0x140ca5850`, so present in the GUI): when +0x138 is set, the work model is written to
  `SfOprConvert2Dto3D_TEMP_STEP.KEY` (`0x1401c6f20`, `0x140c7d100` -> `0x140895350` = DEF_PRE
  `1401c5810`, flags 0). The rest (the scene's second model `0x1402c81b0` / `0x140cf9990`, its reset,
  database and KEY read, flag +0x1c1) is viewer state.
* `0x1401c29d0`: base `0x1405f1c00`; TEMP_STEP.KEY is removed when OprFlag = 0; work model
  `0x1401c7370` (a 2D one is deleted; a new one is `0x140c7a6b0(mem, 3)` with the operation's database
  and a copy of its sim control `0x1408df7b0` = `1401f3660`, TRANS 1/2 = 1); its viewer items are
  cleared (`0x140c82c70`). The predecessor's SimCtrl.KEY is read into a scratch DB_STEP_DATA(3)
  (rc 2 without it): GEOTYP into both models when rc = 0, PDIE into the operation's model when
  OprFlag = 0. TEMP_STEP.KEY, when present, is read into the work model (path through
  `QString::operator const char*`). Per object of the operation's model with kind != 7: GEO23 +0x80
  byte = 0, +0x10 = 0.0, +0x8 = 360.0; `MeshSettings2D%05d.KEY` (`0x1401c6110` mode 2) and
  `GeoConvt23Data%05d.KEY` (`0x1401c65f0`) imported (`1401b7c00`, `1401b7de0`); GEO23 mode =
  `1401ee4c0`(sim control); without a GeoConvt file, mode 1 and +0x2d94 = 0: `0x1401c71f0` sets GEO23
  +0x20 to 0 when `0x1401c70d0` finds a node with BC code 1 and |state variable 2| <= 1e-6
  (`0x142157420`) in that direction, else 1. The work model's object of the same number
  (`0x140c7c950(sc, no, 0, 1)`) gets 140161ac0, 140161b60, 1401619c0, the GEO23 block
  (`0x14085ceb0` = `140188c90`) and the mesh settings (`0x14085dd40` = `140189b70`, shapes 0, by
  dimension 1), dimension +0x39c0 = 3 (`0x1401cc360`), `MeshSettings3D%05d.KEY` into the work model,
  and +0x2d94 back. Slot index: the first slot whose resolved code (`0x14005fd20`) equals the object
  number, else number - 1. Kind 0 without mesh, or kind 5 whose object item record has +8 = 0 and no
  mesh (+0x19): GEO23 +0x38 byte = 0 on both objects. Last: work model +0x1b4c = 1 (`0x140c7b1f0`,
  third argument 0: no viewer call).
* Configuration editor `SfConv23Config` (factory `SfOprConvert2Dto3DFactory` vt+0x8 `0x1401cca90`):
  after the load the GUI shows the tree's first item, so the editor exists at Save (oracle B 97).
  Ctor `0x1401e1a80` (setupUi: "Z is up" checked), init `0x1401e1c10` for object 1 (item data 0 ->
  1, `0x140130d90`): no object or kind 7 -> disabled; GEO23 mode set to the sim control's geo kind;
  mode 1: revolve angle/start texts = `number(v,'g',6)` and their textChanged slots write `toDouble`
  back (+0x2bf0/+0x2bf8; the tolerance slot is empty); mode 2: extrusion texts (+0x2c10/+0x2c18);
  "Y is up" checked when +0x2c38 == 2 (its toggled slot writes axis 2); mode not 1/2 afterwards uses
  literal texts and is refused. The GEO23 block is copied to +0x50. The editor's vt+0x1d0
  `0x1401e2d70` runs only when the widget area leaves, deletes or replaces it (slotLeaveWidget
  `0x140dae3c0`, slotDeleteWidget `0x140dae450`, scroll-area show `0x140cda320` on the old
  widget): apply `0x1401e2ef0` (axis = Y-up ? 2 : 3 into WPAXIS `0x1408ea190` and +0x2c38, mode,
  values, for every object with kind != 7 and GEO23 mode > 0), then, when the object's block
  differs from +0x50 (`0x14085d170`), `0x1401c7560(-1, 1)` (work objects lose surfaces and mesh,
  TEMP_STEP.KEY removed), then props vt+0x58. Save's vt+0xd0 (index != 0) emits
  signalUpdateDataInWidgetArea -> slotUpdateData `0x140dae340` -> vt+0x1c0 `0x1401e2a10` (empty),
  then props vt+0x58. Without an apply 97 saves axis 0 (CRDSYS 0, WPAXIS vectors 0), so the
  Configuration editor was replaced during the load: the port shows it twice at the end of the
  load (init, vt+0x1d0, init). The count of two is inferred from oracle B 97 and 114 (a "Z is up"
  editor after Open), not traced through the Qt signals (tree rebuild `0x140d3d730`, SfNotifier
  signalRefreshTree `0x1402ceae0` -> SfTreeView::slotRefresh `0x140d3aa60`).
* `0x1401ce3e0` (props vt+0x58): data (`0x140e2d400`, `m2m/data/convert.hpp`) conType =
  `1401ee4c0`(sim control); the first object with kind != 7 and GEO23 mode > 0 gives upAxis
  (`14025a360`), revType (+0x2c08), revAngle (+0x2bf0), revStartAngle (+0x2bf8), revTol (+0x2c00),
  extLen (+0x2c10).

## 4. CopyMirror

**Not reachable in v11.0.2**: the registry builder `0x14006dba0` never inserts SfOprCopyMirror
(`src/model/registry_rescue.cpp`), so a project naming it fails to open, and no operation has this
class during a load. The slots are ported as written; the test inserts the class to run them.

Class members: +0x148 TOLERANCE, +0x150 the line after `KFREAD n`, +0x158 MIRROR_MERGE blocks.

* `0x140e24b80`: `0x1405ef430`, then the scene's second model (dropped when 2D, made when missing,
  flag, item list cleared): viewer state.
* `0x140e24b50`: base `0x1405f1c00`, then `0x140e23490`: `mirrormerge.ini` in the task folder
  (relative OPI+0x130 path, resolved against the current directory = the project folder) read by
  `0x140e22950`: lines starting with `!` skipped (the trimmed copy is dropped); key/value = sections
  0/1 by `,` when the line has one, else by `\s+`; `KFREAD n` keeps the next other line,
  `TOLERANCE x`, `MIRROR_MERGE <object> <planes>` then one line per plane (sections 0..9: three ints,
  six doubles kept as float, one int). Each block's object gets a viewer item (`0x140c82d50`; object
  number < 1 ends the loop) and per plane `0x140e23a00` builds viewer geometry: viewer code, not
  ported. `GuiModel::viewerMirrorPlanes` records the requested plane count per item; `reset` clears
  it with the item list.
* `0x140e25980` (vt+0x2a0, called by `0x14005b2f0` with each predecessor as the class's current
  operation): the current operation's slot no-1 loaded bare (`0x14005af80`); geometry kept, a mesh
  deleted (from a mesh alone: geometry from mesh first); then `0x140e25b20`: the viewer item no (made
  when missing) with planes and an object with geometry needs `0x140e25c00` (mirror construction on
  the viewer planes): refused. The object is copied into the inheriting operation's object
  (`0x140835430` = `1401613a0`, flags 1, 1, 1, 1, 0, 1, owner table +0x15f8). Returns true.
* `0x140e2d080`: the body of `0x1401ce3e0`.

## 5. Not reproduced (explicit M2M_E_UNSUPPORTED)

| Path | Where |
|---|---|
| Boolean post-load with no object entries (adds an object, `0x140057ba0`) | `booleanPostLoad` |
| CopyMirror mirror construction `0x140e25c00` (needs the viewer planes) | `mirrorObject` |
| CopyMirror slot number < 1 (no viewer item; the original dereferences null) | `mirrorObject` |
| KEY writer status != 0 for TEMP_STEP.KEY | `writeModelKey` |
| `mirrormerge.ini` with a Unicode byte-order mark (QTextStream codec detection) | `readTextLines` |

## 6. Open points for the MDT thread

* Oracle B: a Convert2Dto3D project opened on its Convert2Dto3D operation should leave
  `Operations/TaskNNNNN/SfOprConvert2Dto3D_TEMP_STEP.KEY` behind (written at open); the corpus has no
  such project, so the bytes are not checked against the original yet.
* The work model's database name copies of `0x140c7aa80` (step +0x340 / +0x1748) are not modelled,
  as in L0's `GuiModel` constructor; the KEY writer does not read them.
