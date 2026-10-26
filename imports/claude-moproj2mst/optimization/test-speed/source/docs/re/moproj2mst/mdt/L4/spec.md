# MDT L4: MultiBlow family load slots

Packet L4 of `../loader-packets.md`, branch `claude/m2m-mdt-L4` (from `claude/m2m-mdt` 94ec44d3).
Decompiles and assembly: `deform-rea-work/gui_mo/s1/mdt/L4/` (`d0.c` slots, `d1.c` callees, `d2.c`
props getters, `slots.asm` 0x140601f70, `post.asm` 0x140602010). Code: `src/engine/multiblow_load.cpp`,
L2's `src/engine/props_sync.cpp` for the props readers; the class overrides are in `src/ops/multiblow.cpp`.

## 1. Slots

`SfOprMultiBlow` (registry key `SfOprMultiBlow3D`), `SfOprMultiBlow2D` and `SfOprMultiBlowCommon`
have the same entries (`vtables.tsv`):

| Slot | Function | Note |
|---|---|---|
| vt+0x58 | `0x1405ef430` | SfOprBase load (L0) |
| vt+0xb8 | `0x140601f70` | prologue, section 2 |
| vt+0x78 | `0x140602760` | `ret` |
| vt+0x1b8 | `0x1405f6520` | SfOprBase (L0) |
| vt+0xc0 | `0x140602010` | post-load, section 3 |
| vt+0x1f8 | `0x1405f79a0` | false (no restart) |
| props vt+0x58 | `0x140612ba0` | section 4 |

The save-current-operation slots vt+0xd8 `0x140602780` (base `0x1405f22e0`, then SC+0x168 =
vt+0x120) and vt+0x80 `0x140602770` (`ret`) belong to packet S.

## 2. vt+0xb8 `0x140601f70`

```cpp
SfOprBase::loadPrologue(self);           // 0x1405f1bb0: TNOW 3 = 1, TNOW 2 = 0.0, TRANS 2 = 1, TRANS 1 = 1
0x1405ef6f0(self, 1);                    // TRANS 2 (SC+0x108) = 1 again
0x1405ef750(self, 1);                    // TRANS 1 (SC+0x104) = 1 again
m = 0x14005f770(0x1405ef2b0(self));
if (m && (sc = 0x140c7b2d0(m))) {
    int zero = 0; 0x1408da6f0(sc, &zero);  // SC+0x240 (DTTYPE mode) = 0
    sc->+0x16c = 10000;                    // NSTEP
}
```

## 3. vt+0xc0 `0x140602010`

```cpp
opi = 0x1405ef2b0(self); if (!opi) return;
m = 0x14005f770(opi);    if (!m) return;
prev = 0x140062960(opi);
if (0x140c7c1a0(m) < 3)                              // object count
    for (i = 0; i < 3; ++i) {
        if (0x140060640(code(opi, i)) && 0x140057690(opi, i)) continue;   // slot has its object
        o = 0x140057770(opi, i + 1);                 // add object i+1 (new id, slot code)
        if (!o) continue;
        if (prev && 0x140060640(code(prev, i))) o->+0xc = 5;
        0x140060100(opi, i + 1);                     // slot records <- object
    }
0x140c7b2d0(m)->+0x168 = self->vt[0x120]();          // PDIE = 2 (no null test)
if (slot = 0x1400612d0(opi, self->vt[0x120]())) slot->+0x1bf = 1;   // assign movement
if (opi->+0x48 != 0) opi->+0x48 = 0;                 // lubricant id (0x1400657f0 / 0x1407b5170)
```

No `0x1405ef3a0` test and no call of the base `0x1405f1c00`. `0x140057770` creates the model object
(`0x140c7c950(m, no, 1, 1)`), takes a new id from the project's object manager (`0x1400550d0`
vt+0x180 = `0x14006c520`, a new SfMoObjItem), signals each missing lower slot and sets the slot code
with `0x140057c60(opi, no - 1, id, 0, 1, 1)`. The object-manager side is not ported (L0 refuses
`0x140057ba0` and `0x140057c60` the same way), so the port raises `M2M_E_UNSUPPORTED` when an operation
with fewer than three model objects lacks one of its first three. An operation whose three slots
are loaded never reaches it.

## 4. props vt+0x58 `0x140612ba0`

Through `0x140054350` (props = OPI+0x10): `0x1405fb720(pr, 0)`, `0x1405fbae0(pr, 0)`,
`0x1405fc180(pr, 0)`, `0x1405fbe30(pr, 0)`, return true. These four are SfOprBaseProperties code, so
other classes' props vt+0x58 call them too; packet L2 owns them (`src/engine/props_sync.hpp`, with 0x1405f8df0).

Each reader takes a sim control or null; null means `0x1405f8df0(pr)` (owner `0x1405f8d90`,
`0x14005f7a0`) + 0x750. Strings are `QString("%1 %2").arg(v, 0, 'g', -1, ' ').arg(QString(
0x140917de0(kind, SC+0xfc)), 0, ' ')` (literals 0x14249d1c0 .. 0x14249d1e0, all "%1 %2").

| Reader | Fields (B = PROPS+0x30) |
|---|---|
| `0x1405fb720` | B+0 = SC+0x16c, B+4 = SC+0x164, B+0x40 = (SC+0x1d0, kind 0) |
| `0x1405fbae0` | B+0x20 = (SC+0x3d0, kind 9) |
| `0x1405fc180` | `0x1405fb450(pr, SC+0x240, 0)`; B+8 == 0: B+0x38 = (SC+0x1e0, kind 1), else B+0x40 = (SC+0x1d0, kind 0) |
| `0x1405fbe30` | B+0x30 = (SC+0x4c8, kind 0x20) |

## 5. Test

`tests/mdt/test_multiblow_load.cpp` (label `m2m-mdt-l4`). No corpus project has a MultiBlow
operation; the test gives corpus 42_vdie's operation 4 the MultiBlow class and checks the slot
constants, the values of its `SimCtrl.KEY` after a full OB vt+0x58 load, the reader strings for
set sim-control values (SI and English unit names) and the refusal of the add-object path.
An Oracle B case with a MultiBlow operation would check the whole load against the GUI.

## 6. Oracle B check (2026-10-22)

`93_B_MB3ACT` (75F, Active = 2, SfOprMultiBlow3D) and `94_B_MB2ACT` (92_B_MB2REL, Active = 3,
SfOprMultiBlow2D), opened and saved in a fresh GUI (claude/m2m-corpus b6147141). The test loads
`<case>_before` through `openActiveOperation` and checks the model and props against what the GUI
saved: NSTEP 10000, STPINC 10, PDIE 2, TRANS 1 1 0 0 0, DTMAX 0; NumSteps 10000, EnvTemp "20 C",
ConvectionCoef "0.02 N/sec/mm/C", StepBy 1 with TimePerStep "0 sec"; Lubricant Id -1 -> 0. All
match. The time-per-step mode does not come from the prologue (which sets SC+0x240 = 0): the
object KEY reads of the load (`0x1408969a0` -> `0x140831310` = DEF_PRE 1401d7f40 ->
`0x1408da660` = 1401ee510) set it to 1 because DSMAX is not positive, which the ported KEY
reader already does. The other differences in those cases (equipment dropped, INICTC 2, KEY files
written) are save-side (packet S).

## 7. Save with the MultiBlow operation active: the Process editor (SfMultiBlowProcess)

The MultiBlow object tree (vt+0xa8) holds the editors "Process" (SfMultiBlowProcess) and "Blow
Table" (SfMultiBlowTable) and the "Controls" item; after the open the GUI shows Process. At Save,
OB vt+0xd0 `0x1405f1e20` asks signalGetPropertyStackWidgetIndex; with an editor shown (index != 0)
it sends signalUpdateDataInWidgetArea instead of running props vt+0x60, then `0x140054350`. The
shared hook is OpClass::updateDataInWidgetArea (L5S, also Convert2Dto3D's SfConv23Config).

SfMultiBlowProcess fill `0x140636f10` (sections `0x140637100`, `0x140637270`, `0x1406375a0`,
`0x140637bd0`, `0x140637f00`, `0x140637ff0`) writes QString::number(v, 'g', 6) into line edits and the
check states into four checkable group boxes, two check boxes and a button group (ids 0 and 1 from
the ctor `0x140636c50`). The apply `0x1406370a0` (sections `0x1406371c0`, `0x140637410`, `0x1406378d0`,
`0x140637d70`, `0x140637f90`, `0x1406380a0`) reads them back with toDouble. Effects: every value is
rounded to 6 significant digits; the energy setter `0x14060ba70` writes hammer and screw energy;
DwellTimeStep / DwellHeatCoeff become QString::number of their unit-evaluated values (`0x14060c460`,
`0x14060c5a0`), which is the "0.1 sec" -> "0.1" of oracle B 93 / 94 / 93D. Not in the generic props
panel: its builder `0x141373e60` maps each QtProperty to its node only after setting the value, so
the value-changed slots apply nothing (95, FormingExpress active, keeps its units).

Test: tests/mdt/test_multiblow_get.cpp and test_multiblow_save.cpp (93, 93D, 94), moproj_ops /
moproj_write (93D added).

## 8. Cogging: the Process editor and the props tree

- The Process editor (SfCoggingProcess, fill `0x140788cc0`, apply `0x140789410`) writes its form
  back at Save, as MultiBlow's does. The round trip `QString::number(v, 'g', 6)` / `toDouble`
  rounds to 6 digits. Oracle B 115 (Process_Type 1, Swaging_Stroke = Cycle_Time = 1.23456789)
  saved `1.2345700E+000`. The code is in `src/engine/cogging_process.cpp`.
- The props ctor `0x14075df60` never calls the tree builder (props+0x10)->vt+0x20 = `0x14075eb30`.
  The vtable slot `0x142678b50` is that builder's only reference. So every Cogging operation is
  saved with `<Properties Number="0"/>`: all 27 in the corpus, and in 105 and 115. The get slots
  `0x14076b0a0` (getNumHeats, getNumPasses) are never reached.

## 9. ShapeOptim: props and the 3D display

- Props: vt+0x98 `0x14013be10` makes SfOprShapeOptimProperties with the class at +0x40
  (`0x140142620`) and keeps it in the QPointer OB+0xb0. Props vt+0x58 `0x140142ca0` and vt+0x60
  `0x140142d00` copy MaxSims / MinObjFuncVal between data +8 / +0x10 and OB +0xcc / +0xe0.
- The load `0x14013beb0`, after vt+0x78:
  - It shows the model in the main window's 3D display SfGLDisplay (`0x140cf9360`: main model +0x1a0,
    model+0x10 = display; viewer_camera.cpp and `m2m/engine_scene.hpp`).
  - vt+0x1b8 `0x14013cbe0` makes `DoeStudies\<study>\Control\Opt\` (`0x14013d180`, mkpath) and
    the display's "ROIs" model (`0x140cfa0e0` / `0x140d0d670`). It then runs the views
    `0x1400548c0(current, 1)`.
  - `0x14013fb00` / `0x14013c200`: without `<project>.DB`, the flow operation +0xd8 is loaded
    (base vt+0x1c0 `0x1405f65d0` -> `0x140055360`). Its objects are copied into the display's main
    model (`0x140c7c8b0`, `0x140835430`, kind 3, `0x140870590` without geometry).
- Save vt+0x68 `0x14013d6f0` runs vt+0xd0, vt+0x80 `0x14013e4f0` and vt+0xe8 `0x14013c8f0`:
  - vt+0x80 copies the props to OB and reads the StopCritGroup values.
  - vt+0xe8 makes the study folder again and writes one .GEO per ROIs object (none in 108).
- Still refused: the optimisation study (type-3 sub-project, SP+0x28), the DB step branch, and
  the .GEO reader / writer.
- Oracle B 108 (57 with ShapeOptim appended and active) passes save_current and moproj_write.
  The GUI also made `DoeStudies\Control\Opt`.

## 10. DB-5: OB vt+0xc8 of MultiBlow and the Express family

Called at the end of the DB / restart sync `0x1400538f0`. The slot is declared and called by the
MDT thread (DB-4). The overrides are wired when that lands.

- MultiBlow `0x140602230`: the base `0x1405f1e10` is empty, then `0x140602260`.
  - It reads the primary die (vt+0x120 = 2) of the operation's model.
  - Movement type +0x21c8 of 2 (hammer) or 3 (screw): `0x14060b6b0` copies the block +0x2160 into
    the MultiBlow data. That covers the name, the hammer +0x1e8..+0x200 or screw +0x280..+0x298
    fields, the energy into +0x20 and +0x40, and the direction +0x50..+0x60. The EquipmentType and
    EquipmentSpec children are made again, and UseEfficiencyFunctionData is set from the curve's
    point count.
  - Otherwise, or without the die, the GUI's log message becomes a warning.
  - Code: `engine::multiBlowSyncFromModelTail` and `multiBlowEquipmentFromMovement`.
- Express `0x1413fbd10` (`mst::expressSyncFromModelTail`, src/ops/express.cpp):
  - OprFlag 0 with step definition (+0xc0) 0 gives step definition 1 and the message (a warning).
  - When `0x1413fd0f0` is not 0 and the class is not "SfOprHeatExpress" (case-insensitive),
    vt+0x2c0 `0x1413fc840(keep = 1)` reports rigid dies with nodes while the setting (+0x04 == 0
    or +0x08 != 0) does not keep them.
  - Then "Keep Mesh of Dies?". Yes sets +0x04 0 -> 2 and +0x08 -> 0; No would delete the meshes
    through vt+0x2c0(0).
  - Last, vt+0x2b8 `0x1413fc6b0`: TRANS 2 = 1 for FormingExpress (case-sensitive), TRANS 1 = (+0x04
    != 0).
  - The engine bodies are in src/engine/express_sync.cpp, through `g_loaderSlotsL4`.
- `0x1413fd0f0` returns 4 when the props' vt+0x100 is false (FormingExpress 0x1400707e0).
  HeatTransferExpress (0x1413e5ef0) returns true, so it takes the props' heat-transfer type +0x18.
- **Documented deviation: "Keep Mesh of Dies?" is answered Yes, the dialog's default.**
  - The binary calls the obsolete `QMessageBox::question(parent, title, text, Yes (0x4000), No
    (0x10000))`, and neither button carries `QMessageBox::Default` (0x100). So Qt 4.8 sets no default
    button, and Return goes to the focused auto-default button, Yes.
  - Confirmed by oracle B. In 130_B_DBSTEP_FE (FormingExpress at OprFlag 0, meshed dies in the DB),
    Return gave Yes. 130n_B_DBSTEP_FE_no was answered No.
  - Between the two saves the MST, .moproj and InterObject.KEY are equal. They differ in
    RestartStep.KEY (1,020,311 vs 480,347 bytes, the die meshes dropped), SimCtrl.KEY (SMODE 3 / 1,
    TRANS 1 1 / 0) and Forming.FWZ (lines 5 and 19: 2 / 0 and 0 / 2).
  - The converter answers the default and records it as a warning. `mst::g_keepMeshOfDiesNo` answers
    No, for the 130n test.
  - The port matches both byte for byte in the six files Save writes: MST, Equipment00005.KEY,
    Forming.FWZ, InterObject.KEY, RestartStep.KEY and SimCtrl.KEY.

Tests: tests/mdt/test_db5_sync.cpp (93 and 95 from oracle B, with fields set by the test) and
test_db5_keepmesh.cpp (130 and 130n, whole Save).
