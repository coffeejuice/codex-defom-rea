# MDT L3S: save slots of Forming, FormingExpress and HeatTransferExpress (File > Save step 2)

Packet S (`../../s/spec.md`) ports the generic "save current operation" `0x140052ec0` and refuses
class overrides. This note covers the overrides of the L3 classes and, by the coordinator's
assignment (2026-10-22), the generic system-lubricant contact rebuild `0x14005a9b0` with the base
default pairs. Decompiles: `deform-rea-work/gui_mo/s1/mdt/L3/s0.c` .. `s2.c`. Code:
`apps/moproj2mst/src/engine/save_l3.cpp` (class bodies) and `save_current.cpp` (dispatch by class
key, the rebuild). Ledger: `ledger.tsv` here (packet `MDT-L3S`).

## 1. Slots

| Class | vt+0x68 | vt+0xd0 | vt+0xd8 | vt+0xe8 | vt+0x80 | props vt+0x60 | vt+0x198 | vt+0x1a0 |
|---|---|---|---|---|---|---|---|---|
| Forming / 2D | base | base | base | base | empty `0x140c06770` / `0x140c04f90` | `0x140c09ed0` (S) | base | base |
| FormingExpress, 2D, Common | `0x1413f8f60` | base | `0x14141cf90` | `0x1413f91f0` | empty `0x14141d320` | `0x14142a6f0` (S) | `0x14141ef60` | `0x1413fd4c0` |
| HeatTransferExpress / 2D | `0x1413f8f60` | base | `0x1413e6460` / `0x1413e5ba0` | `0x1413f91f0` | empty `0x1413e64d0` / `0x1413e5c10` | `0x1413ed900` | base | `0x1413fd4c0` |

Forming needs nothing beyond S and the L3 loader: oracle B 59 is byte-identical.

## 2. Express bodies

* vt+0x68 `0x1413f8f60`: when the heat-transfer type (`0x1413fd0f0`) is 0, or the class name contains
  "SfOprHeatExpress" (case-insensitive), every engine object whose kind is not 0 or 3 gets
  `+0x1f00 = ENVTMP + d`, `+0x1ef8 = ENVTMP - d` with `d = |+0x1f00 - +0x1ef8| / 2`; then the base
  `0x1405ef510` (vt+0xd0, props vt+0x60, vt+0xd8, vt+0xe8, vt+0x80).
* HeatTransferExpress vt+0xd8 `0x1413e6460`: `TMAX = (TNOW 3 == 0 ? TNOW 1 : TNOW 2) + HeatingTime`
  (`0x1405fcad0`: B+0x18 evaluated in the props' unit, category 0; `0x1408cb0c0`). The base
  `0x140054540` (OPRNAM/SIMNAM/CURSIM) is not called.
* vt+0xe8 `0x1413f91f0`: the base `0x140057370` first, then in the same heat-transfer condition the
  contact block's "generate" byte (ICB+0x65) is cleared and INICTC set for the objects of kind != 7.
  The loop passes positions 0 .. n-1 to `0x140c7aed0`, whose position 0 is the first object (the last
  object is not visited), as in the binary.
* HTExpress props vt+0x60 `0x1413ed900` = F80, F90, FA0 (S) and vt+0x70 `0x1413ed710` (step data
  +0x60 -> SC +0x1c8 (+1), +0x1d0, +0x270, +0x2a0, +0x2a8, +0x164).
* vt+0x1a0 `0x1413fd4c0`: friction and IHTC from the props (`0x1405fc890`, `0x1405fc8e0`); when
  |IHTC| < 1e-6 the IHTC comes from the project unit and the sub type (OPI+0x128 compared with
  "Rest on die" / "Dwell on die"; values in `save_l3.cpp`).
* FormingExpress vt+0x198 `0x14141ef60`: clears the engine's pairs; every object of kind != 7 (not
  object 1 in 3D) gets a pair with object 1 and the props' friction and IHTC.

## 3. Generic rebuild `0x14005a9b0` (system lubricant, ICB+0x70 = OPI+0xe0)

`../../s/spec.md` section 4, ported as described there: local `DB_STEP_DATA(3)`, the engine's pair
list cleared, the lubricant KEY (lubricant manager vt+0x208) read into the local model and its first
pair taken as the template; class vt+0x198 (base `0x1405f5c90`, FormingExpress `0x14141ef60`;
Cogging and Distortion refuse until L1/L6); per default pair `0x140875a20` (= DEF_PRE `140201bc0`,
exported from `port/defcmd/src/p8_01/commands.cpp` as `def::cmd::bccdef_lubricant`), then
`0x14035e6f0` (values) or `0x14035e540` (copy of the template), each followed by the mirror
`0x140360d30`. The pair element is 0x20 bytes (`0x1400657a0`: master 1, slave 1, friction, IHTC,
+0x18 lubricant flag 0, +0x19 = 1 which goes to pair+0x174 in 3D). Base vt+0x1a0 `0x1405f60c0` sets
only the IHTC (unit and sub type).

## 4. Tests

`m2m.s.save_current` (oracle B): R11, R19 (S), 82_R52_BEND_2D (HeatTransferExpress2D with the system
lubricant: InterObject.KEY through the rebuild) and 59_B_HT3 (Forming) pass byte-identical with the
exact file set; R46/R47 skip (Boolean2D loader, L5).

## 5. FormingExpress vt+0xd8 `0x14141cf90` (oracle B 95_B_FE3ACT)

Slot 1's state variables get byte +0x40 = 1 (URZ given: the MST's `URZ 1 0 0 0 0`), then the base
`0x140054540`, PDIE = 2, and with the wizard's step definition "System" (data+0xc0 == 0)
`0x1413f6b60`: DSMAX = die stroke (x1.2 unless exact) / NSTEP, 0.001 when that is 0
(`0x1413f7110`, `0x1416f0cb0`). A mechanical-press PDIE movement (`0x140355ab0`) and a 2D model
(`0x141424be0`) refuse. The first workpiece-like object of the current model (main window vt+0x268,
taken as the saved operation's model: inferred) that is 3D, not kind 5 and meshed gets its target
volume updated (`0x141424e60` -> DEF_PRE `14021d630`) when its volume (`140163540`) differs by
more than 0.6 %.

The GUI engine's step-control init `0x1408d45f0` leaves NSTEP 100 where DEF_PRE's `1401e8490`
writes 100 and then 1; `GuiModel` re-applies the GUI values after construction and after a clear
(a new operation's SimCtrl.KEY in 95 has NSTEP 100).

## 6. Object-tree add vt+0x228 (load views, `OpClass::objectTreeAdd`)

HeatTransferExpress `0x1413e6520` / 2D `0x1413e5c60`: in heat-transfer type 0 with more than one
object slot the GUI deletes objects (`0x1400581d0`): refused; the rest (`0x1413f9e00`,
`0x1413fcf70`) is the object tree only. Forming `0x140c07130` / 2D `0x140c04fa0` call vt+0x230
(`0x140c068c0` / `0x140c05200`, packet S's equipment sync) after tree-only code. FormingExpress
`0x1414134e0` / 2D `0x141412bd0` and every vt+0xa8 (`0x1413f93c0`, `0x140c06780`, `0x140c05000`,
`0x14141dc10`) change only the tree and its item model (besides calling vt+0x228).

## 7. Open

* The deletion paths above and the FormingExpress press/2D branches of vt+0xd8.
