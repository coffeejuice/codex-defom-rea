# MDT L2: loader slots of the HeatTransfer family, HTFurnace and Pre

Packet L2 of `../loader-packets.md`, branch `claude/m2m-mdt-L2` (from `claude/m2m-mdt` 94ec44d3).
Static analysis of DEF_GUI_MO_64.exe; decompiles and disassembly in
`deform-rea-work/gui_mo/s1/mdt/L2/` (`d0a.c` .. `d5a.c` annotated with `annot.py`, `ht_post.asm`
for `0x141354ce0`), Ghidra copy `gproj_L2` (`run_L2.sh`). Code: `src/ops/family.cpp` (class
overrides), `src/engine/loader_l2.cpp` (bodies), `src/engine/props_sync.cpp` (SfOprBaseProperties
setters, shared with L3), payloads `include/m2m/data/heattransfer.hpp`. Rows: `ledger.tsv` here,
`../../owners.tsv`. Test: `tests/mdt/test_loader_l2.cpp` (corpus 08_HEAT1).

Scope: SfOprHeatTransfer, SfOprHeatTransfer2D, their base SfOprHeatTransferCommon, SfOprHTFurnace,
SfOprPre (key SfOprPre3D), SfOprPre2D. SfOprHeatTransferExpress(2D) has its own slots
(`0x1413e6360` ...) and belongs to the Express family (L3). Only the load-path slots are here; the
save-current-operation slots (vt+0x68 / +0xd0 / +0xd8 / +0xe8 / +0x80, HTFurnace's own vt+0xd8
`0x140c65940`) have no plug-in point yet (packet S).

## 1. Slots (vtables.tsv)

| Class | vt+0x58 | vt+0xb8 | vt+0x78 | vt+0xc0 | props vt+0x58 | vt+0x1f8 |
|---|---|---|---|---|---|---|
| HeatTransferCommon | base | `141354ca0` | `141355340` empty | `141354ce0` | `141357520` | true |
| HeatTransfer | base | `1413543a0` -> Common | `1413543e0` empty | `1413543c0` -> Common | same | true |
| HeatTransfer2D | base | `1413535b0` -> Common | `1413535f0` empty | `1413535d0` -> Common | same | true |
| HTFurnace | base | `140c657b0` | `140c659d0` empty | `140c65830` | `140c69480` | false |
| Pre / Pre2D | `141348e80` / `141347980` empty | base | `141348ea0` / `1413479a0` empty | base | `14134c380` | false |

vt+0x1b8 is the base `0x1405f6520` for all. Because Pre's vt+0x58 is empty, opening a Pre
operation loads nothing (no reset, no KEY read); its props slot is never reached from the load.

## 2. Bodies

* `141354ca0`: base prologue `0x1405f1bb0`, TRANS 2 = 0 (`0x1405ef6f0`), TRANS 1 = 1 (`0x1405ef750`).
* `140c657b0`: the same, then `S+0xda0` (SC TNOW 3) = 1; `0x1408221e0(S)` result unused.
* `140c65830`: SC+0x3c0 (furnace flag) = 1; when the furnace model has no curves, 0x16 curves
  (`0x1409f02e0` = DEF_PRE 140375600) and their defaults (`0x1409f0b70` = 140375e90). The props
  `dynamic_cast` result is unused. SC is not null-checked.
* `141354ce0` (asm checked): `dynamic_cast` props to SfOprHeatTransferProperties (else return);
  scratch DB_STEP_DATA(3) gets the predecessor's `SimCtrl.KEY` (`0x1408969a0`, reset 1, mode 0;
  result 2 when not read). Sub type index of OPI+0x128 (`0x1413556c0`, case-insensitive contains in
  this order: "Heat in furnace" 0, "Rest on die" 2, "Dwell on die" 3, "Transfer through air" 1,
  else -1). When OprFlag == 0 or the index is -1: v = 1; not the chain head and a predecessor:
  its index r (`0x141355500`); r == -1 -> v = 3 when the scratch KEY read returned 0 and its
  TRANS 2 (SC+0x108, `0x1408dfd80`) != 0; r == 1 -> v = 2; then OPI+0x128 = text of v
  (`0x1413558b0`). Then n = 1, or the predecessor's count of slots holding an object (min 1);
  OprFlag == 0 and n <= 1 -> n = 3, back to 1 when v is 0 or 1. With OprFlag != 0, slot 0 holding
  an object and the model holding at least one object, nothing more. Otherwise each slot i < n that
  does not hold an object with an engine object adds one (`0x140057ba0`: new project object item,
  slot code, `0x140057c60`, then `0x140060100` and props vt+0x128 `0x141358890`, the object
  temperature by material class): **refused** (M2M_E_UNSUPPORTED), the converter does not create
  project objects. After the loop, OprFlag == 0: props vt+0x120 `0x141358150` and, with a
  predecessor whose KEY was read, STNOUT/ELMNOD from the scratch SC.
* `141358150`: by sub type: "Transfer through air" 15 s / "Transfer time"; "Rest on die" 4 s /
  "Resting time"; "Dwell on die" 4 s / "Dwell time"; else 3600 s / "Heating time" with ENVTMP
  1200 (SI) / 2250. ENVTMP 20 / 68 otherwise. HeatingTime text via `0x1405fcb90`; the label
  (`0x1405fd090`, B+0x48) is the property's display label (UI, not modelled); SC TMAX = time,
  ENVTMP ndim = 0 and value.
* `141357520`: `0x1405fb720`, `0x1405fbae0`, `0x1405fbe30` (props_sync.cpp), then vt+0x68
  `141357460`: step data (+0x58) STEPDEF = STPDEF - 1, STPINC, DTMAX 1, DTPMAX 1, DTPMAX 2, 3.
* `140c69480`: ENVTMP constant = furnace curve 8 value (9, 0, 0, 1) (`0x140c69580`), props furnace
  model (+0x38) = copy of the engine's (`0x1409f0230` = DEF_PRE 140375550, exported as
  `def::cmd::furnace_model_copy`), FURNAM = its name (`0x140c67a60`), then vt+0x68 `140c69350`:
  BVIEWFACTON = (VFACTR mode == 1), STEPDEF = (STPDEF 1 or 2 ? 0 : 1), STPINC, DTMAX 1, DTPMAX 1..3.
* `14134c380`: `0x1405fb720`, `0x1405fbae0`, `0x1405fc180`.
* SfOprBaseProperties setters: text `"%1 %2".arg(v, 0, 'g', -1).arg(unitName(kind, SC unit))`;
  `0x1405fb720` NumSteps = NSTEP, StepIncr = STPINC, TimePerStep = DTMAX 1 (kind 0);
  `0x1405fbae0` EnvTemp (kind 9); `0x1405fbe30` ConvectionCoef (kind 0x20); `0x1405fc180`
  StepBy = DTTYPE mode (`0x1405fb450`, no signal) then DistPerStep (DSMAX, kind 1) or TimePerStep.

Dropped UI: MSG signals `signalEqpDataAdded`, `0x1402cea00`, props signal `0x1405f8590`, the
object-tree list `0x1405ef3a0`.

## 3. Open items

* The add-object path of `141354ce0` (and with it props vt+0x128) is refused; reaching it needs
  OprFlag 0 with a predecessor of fewer than two object slots and a sub type other than furnace or
  air transfer, or a slot with no engine object.
* `0x1405fcb90` reads the owner's model without a null check; the port refuses instead.
* `0x141359a60` (step data ctor) is P01's; L2 declares the shared payload type.
* No corpus case has an HTFurnace or Pre operation; the test runs them on 08_HEAT1's operation with
  the class key changed.
* L0's base lists in `class_slots.cpp` name `SfOprPre`, whose registry key is `SfOprPre3D`; the Pre
  classes now override the slots that matter, so the lists are not reached for Pre.

## 4. Save slots (branch claude/m2m-mdt-L2S, packet S's 0x140052ec0)

Decompiles `gui_mo/s1/mdt/L2/s0a.c` .. `s8.c`. Packet S's `save_current.cpp` dispatches by key;
L2's classes are taken off its refusal lists.

| Class | vt+0x68 | props vt+0x60 | vt+0xd8 | vt+0xe8 | vt+0x80 |
|---|---|---|---|---|---|
| HeatTransfer / 2D / Common | base | `141357590` (S) | base | base | `1413543f0` / `141353600` / `141355350` empty |
| HTFurnace | base | `140c69520` | `140c65940` = base + unused cast | base | `140c659e0` empty |
| Pre / Pre2D | `141348e90` / `141347990` empty | (not reached) | - | - | `141348eb0` / `1413479b0` empty |

* `140c69520`: `140c69920` (furnace name S+0xb90 = FURNAM by `strcpy` of its local 8-bit text: the
  port refuses a name of 0x40 bytes or more; curve 8 value (9, 0, 0, 1) = ENVTMP constant;
  signalEqpDataAdded UI), then vt+0x70 `140c691f0`: VFACTR mode = BVIEWFACTON, STPDEF = STEPDEF 0 ? 1
  : 3, DTMAX 1 = STEPDEF 0 ? DTMAX (+0x18) : DTEMPLMT0 (+0x28), DTPMAX 1..3, STPINC. No base helpers.
* Pre's save is empty, so the GUI writes no step-2 file for an active Pre operation (oracle B 86).

### Load-side finding: 0x1400547b0 is not only UI

L0 dropped `0x1400547b0` (views) as UI. With ProcType 5 it runs `0x140054950`, which on the first
open calls OB vt+0xa8 and vt+0x278 (`0x1405f15e0`); both call OB vt+0x228. SfOprHTFurnace's
vt+0x228 `140c66230` clears the thermal BCs of every slot's engine object (`140c668c0` ->
`0x140878b80` = DEF_PRE 140204d20), so the object KEYs it saves have `ECCTMP n 0 0` (oracle B 87:
corpus 57's `ECCTMP 1 0 2` becomes `1 0 0`). Ported as `OpClass::objectTreeAdd` (base: nothing,
UI only), called twice from `baseLoad`. The HeatTransfer family's vt+0x228 / vt+0xa8 change only the
tree. Nearly every other class overrides vt+0x228 and vt+0xa8; their packets must check them.

Tests: `tests/mdt/test_save_l2.cpp` rebuilds the inputs of oracle B 86_B_PRE3F and 87_B_HTFF as
`oracle_b/_tools/mk2.py` did and compares the save byte for byte: both pass (0 and 13 files);
`m2m.s.save_current` R11, R19, 59_B_HT3 still pass.
