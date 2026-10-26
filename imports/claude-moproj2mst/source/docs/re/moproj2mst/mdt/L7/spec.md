# MDT L7: load slots of Cycle, Group, DoeOutput, Report, ShapeOptim, and "add object"

DEF_GUI_MO_64.exe (DEFORM v11.0.2), static analysis. Decompiles: `gui_mo/s1/mdt/L7/d0..d7.c`,
`a0..a6.c` (Ghidra copy `gproj_L7`, `run_L7.sh`). Code: `src/ops/cycle.cpp`, `src/ops/noop.cpp`
(Group, DoeOutput), `src/ops/report.cpp` (Report, ShapeOptim), `src/ops/l7.hpp`,
`src/engine/loader.cpp` (add object), `src/engine/class_slots.cpp` (base vt+0x1b0, post-load).

## 1. Slots

| Class | vt+0x58 | vt+0xb8 | vt+0x78 | vt+0xc0 | props vt+0x58 | other |
|---|---|---|---|---|---|---|
| Cycle | 0x14134d7b0 -> Group's 0x1413524f0 -> base 0x1405ef430 | 0x14134d750 -> 0x141352490 -> base | 0x14134d770 empty | 0x14134d790 -> 0x1413524d0 -> base | base 0x1405fb600 | vt+0xe0 0x14134daf0: model of the first group member (0x1400b7470, 0x14005f820) |
| Group | 0x1413524f0 -> base | 0x141352490 -> base | 0x1413524b0 empty | 0x1413524d0 -> base | 0x141352e70 (false) | |
| DoeOutput | 0x140e2e320 | 0x140e30ec0 -> base | 0x140e2ec40 | 0x140e30ee0 empty | 0x140e36290 | vt+0x1b0 0x140e32a30 |
| Report | 0x1412add20 | 0x1412af720 -> base | 0x1412ae070 | 0x1412af740 empty | 0x1412b1f10 | |
| ShapeOptim | 0x14013beb0 | 0x14013af70 -> base | 0x14013d740 | base 0x1405f1c00 | 0x140142ca0 | vt+0x1b0 0x14013fc20, vt+0x1b8 0x14013cbe0 |

vt+0x1f8 is false (0x1405f79a0) for all five. Engine model access `0x14005f770` -> `0x14005f820`:
class vt+0xe0 first (base 0x140100e20 = null), else OPI+0x38, created by class vt+0x1b0 when
missing. DoeOutput/ShapeOptim vt+0x1b0 are the base body 0x1405f6410 without the TNOW 3 store.

## 2. Where the converter stops (explicit M2M_E_UNSUPPORTED)

* DoeOutput vt+0x58: after the model reset and `set_dimension` (dimension of the predecessor, else
  of the last flow operation found in its project's table) it attaches the model to the 3D viewer
  (mainWindow vt+0x778, `0x140cf9360`, sets W+0x10) and works through viewer models
  (`0x140d0d670`); not reproduced (packet V). Its vt+0x78 reads the sub-project's DOE output
  document SP+0x30 (`0x1400b5ad0` creates a GUI object, `0x140643100`).
* ShapeOptim vt+0x58: OB field init `0x14013f5a0`, predecessor's flow number into OB+0xd8 (a null
  predecessor is dereferenced by the original), reset, `set_dimension` (operation OB+0xd8 of the
  flow, `0x140140140`), vt+0x78, then the same viewer attach. vt+0x78 refuses for a type-3
  sub-project (SP+0x28 study) and when the props exist (MaxSims / MinObjFuncVal into the props data
  +0x38, not modelled).
* Report vt+0x58 -> vt+0x78: the report document SP+0x30 (`<project>.DS`), a GUI object.
* Report / DoeOutput / ShapeOptim props vt+0x58: not modelled data; refuse.

None of the three is the active item in any corpus or oracle-B project (`act.py` scan: Cycle 33,
Group 1).

## 3. Add object `0x140057ba0` (base post-load 0x1405f1c00, n objects when the model has none)

```
0x140057ba0(opi): mgr = 0x1400550d0(opi); if !mgr return 0
  0x140060d60(opi)          slots resized to the model's object count (-1 appended / last dropped),
                            kind-7 objects free their slot (-1), trailing -1/-99 slots dropped
  id = mgr->vt[0x180]()     0x14006c520: smallest id >= 1 not in the manager (0x14006c230), then
                            vt+0x188: new SfMoObjItem (0x140068df0 -> 0x1400688f0), path vt+0x178
                            "<project>/Objects<sep>Object%05d.KEY", appendRow
  s = 0x140060530(opi)      first slot with code -1 / -99, else the slot count
  o = 0x140057c60(opi, s, id, 0, 1, 1)
  0x140069620(mgr)
0x140057c60(opi, s, id, fromFile, ui, sig):
  m = 0x14005f770(opi); if !m return 0
  0x14005fc80(opi, s, id)   slot code (list grown with -1)
  fromFile == 0: o = 0x140c7c950(m, s+1, 1, 1); id == -2 -> o->kind = 5; o+0x2bf0 = 360.0,
                 o+0x2bf8 = 0
  else if id > 0 || id == -2: o = 0x14005aec0(opi, m, s, 0)
  0x140060100(opi, s+1)     slot record and object-item record <- engine object (0x1400601b0)
  UI: 0x140055220 (manager dataChanged), 0x140054c20 (class vt+0x270: object tree; Pre/Pre2D
      override it and refuse here), messenger signals
  0x140069620(mgr)
0x140069620(mgr): 0x1400a5c10 clears every item's operation set (vt+0x210) and adds each operation
  1..max id to the items of its positive slot codes; 0x14006d490 removes every item with an empty set
  (vt+0x1a0: vt+0x1a8 = 0x14081a6e0 removes the file of vt+0x178 when it exists, takeRow).
```

Removed files go to `FileEffects::deletes`. Check (oracle B 60_B_HT2 / B02_HT2 / 62_B_BM2, the
GUI's own Save of corpus 48 copies): the cycle (slot item 5, no file) ends with item 6 "Workpiece",
ObjectType 1, HasGeo 0, HasMesh 0, DefineRotationMovement 1, Object_List 1 2 3 4 6. The tier-1 test
`m2m.mdt.l7_load` checks the same values.

## 4. Save slots (packet S's save current operation 0x140052ec0; branch claude/m2m-mdt-L7S)

| Class | vt+0x68 | vt+0xd0 | vt+0xd8 | vt+0xe8 | vt+0x80 | props vt+0x60 |
|---|---|---|---|---|---|---|
| Cycle | base 0x1405ef510 | base | base | 0x14134dae0 empty | 0x14134d780 empty | base 0x1405fb610 |
| Group | base | base | base | 0x141352530 empty | 0x1413524c0 empty | 0x141352e80 (true, nothing) |
| DoeOutput | 0x140e2e730 | 0x140e30ef0 empty | 0x140e30f00 empty | base | 0x140e2fcd0 | 0x140e36320 |
| Report | 0x1412adf90 (vt+0x80, then 0x1412af0c0) | 0x1412af750 | 0x1412af760 | base | 0x1412aeb30 | 0x1412b1f50 |
| ShapeOptim | 0x14013d6f0 (vt+0xd0, vt+0x80, vt+0xe8) | base | 0x14013af90 empty | 0x14013c8f0 | 0x14013e4f0 | 0x140142d00 |

A cycle or group writes no task file at Save (vt+0xe8 empty): only props vt+0x60 (nothing) and
0x140054540 (OPRNAM, CURSIM, SIMNAM into the shared member model) run. Oracle B 60_B_HT2 and
62_B_BM2 (corpus 51 with an operation appended after the active cycle) show no file written in
step 2; `m2m.s.save_current` passes both, and the converter's MST of those saved projects equals
the GUI's MST byte for byte. DoeOutput, Report and ShapeOptim vt+0x68 refuse (viewer model, report
or DOE document SP+0x30, ShapeOptim props data); their loads refuse before any save can run.
vt+0x190: ShapeOptim 0x14013fbf0 (opDim of 0x140140140) and Report 0x1412b0600 (opDim of the last
flow operation, else 3) are ported in `src/ops/report.cpp`; ShapeOptim vt+0x1b8 0x14013cbe0 refuses
(viewer model).

## 5. Open

* `0x1400601b0` (L0) still refuses a slot-code change on load; with `addObjectAt` it can be ported.
