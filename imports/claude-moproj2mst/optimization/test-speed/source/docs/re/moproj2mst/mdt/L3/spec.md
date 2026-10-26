# MDT L3: loader slots of Forming, the FormingExpress family and the HeatTransferExpress family

DEF_GUI_MO_64.exe (DEFORM v11.0.2), static analysis only. Decompiles in
`deform-rea-work/gui_mo/s1/mdt/L3/` (`d0.c` slot bodies, `d1.c` HeatTransferExpress, `d2.c`..`d7.c`
callees, `facf0.asm` = `0x1413facf0`; Ghidra copy `gui_mo/gproj_L3`, `run_L3.sh`; `tools/va.py` reads
strings and doubles at a VA). Code: `apps/moproj2mst/src/engine/loader_l3.cpp` (bodies),
`include/m2m/mst/loader_l3.hpp` (hook table), class overrides in `src/ops/family.cpp` (Forming,
HeatTransferExpress) and `src/ops/express.hpp` (FormingExpress). Ledger: `ledger.tsv` here; owners in
`../../owners.tsv`. Names as in `../L0/spec_C.md` (OPI, OB, M, S, SC).

## 1. Slots

| Class | vt+0xb8 | vt+0x78 | vt+0xc0 | props vt+0x58 |
|---|---|---|---|---|
| SfOprForming / 2D | `0x140c060f0` / `0x140c04910` | empty | `0x140c06130` / `0x140c04950` | `0x140c09da0` |
| SfOprFormingExpress, 2D, Common | `0x14141bc90` | empty `0x14141d310` | `0x14141bda0` | `0x14142a6a0` |
| SfOprHeatTransferExpress / 2D | `0x1413e6360` / `0x1413e5aa0` | empty | `0x1413e63a0` / `0x1413e5ae0` | `0x1413ed890` |

vt+0x58 (`0x1405ef430`), vt+0x1b8 (`0x1405f6520`) and vt+0x1f8 (`0x1413e5ef0`, restart capable) are the
base slots for all of them. SfOprExpressBase (vt+0x78 pure) is never instantiated; its prologue
`0x1413fab90` and post-load `0x1413facf0` are called by HeatTransferExpress. The 3D/2D pairs have
identical bodies (only their string literal addresses differ).

## 2. Forming

* Prologue: base `0x1405f1bb0`, TRANS 2 = 1, TRANS 1 = 1 (the KEY load that follows overwrites both).
* Post-load `0x140c06130`: PDIE object `o` and its slot; when the operation has no equipment
  (OPI+0x4c < 1), the GUI writes `o`'s movement as `<temp>/Equipment (<count+1>).KEY` (`0x140358190`,
  arg 4 = 1 so no overwrite prompt), adds it to the equipment manager (`0x1400bf510` ->
  SfMoEqpManager vt+0x160), copies the name into the movement (`strncpy` 0x50), sets the slot's (+0x28)
  and the operation's (+0x4c) equipment id and writes the KEY again at the item's path.
  **Refused** (M2M_E_UNSUPPORTED): the equipment manager's add is not ported. Corpus: every Forming
  operation has an equipment id except 54 op 2, whose PDIE is 0 (no object), so nothing is refused.
* Props `0x140c09da0`: base setters `0x1405fb720`, `0x1405fbae0`, `0x1405fc180`, `0x1405fbe30` (owner
  L2, `props_sync.cpp`); then the PDIE movement (`0x14082f4b0` = DEF_PRE `0x1401d60e0`: S+0x8b8 ->
  object +0x2160). Without one: D+0x10 = 0; with one: D+0x10 = 1 and `0x140c07eb0`: D+0x18 = movement
  name; movement type +0x68 -> Equipment D+0x14: 1->0, 0->1, 4->2 (+0x318/+0x320/+0x328 into
  D+0x68/+0x70/+0x78, D+0x60 = 0), 2->3 (+0x1f0/+0x1e8/+0x1f8 into D+0x28/+0x30/+0x38), 3->4
  (+0x280..+0x298 into D+0x40..+0x58), 7..10->5, 5->6, 6->7; on a match both groups are re-made
  (`0x140c0a040`, `0x140c0a920`). The QtProperty enable calls (`0x140370e90`) and the messenger
  signal are display only.

## 3. HeatTransferExpress (SfOprExpressBase)

* Prologue `0x1413e6360` = `0x1413fab90` (base, TRANS 2 = 0, TRANS 1 = 1; when the props are heat
  Express props (vt+0x100 true) and OprFlag is 0 the wizard defaults are applied: **refused**; OprFlag 0
  also sets OPI+0xe0 = 1) then TRANS 2 = 0, TRANS 1 = 1.
* Post-load `0x1413e63a0`: `0x1413fa950` (OprFlag 0 only: copies the previous Express operation's
  wizard choices through `0x1413faaa0`/`0x1400b7fb0`: **refused**), `0x1413facf0`, TRANS 1 = 1,
  PDIE = 1 when the model has one object, else 2.
* `0x1413facf0`: reads the previous operation's SimCtrl.KEY into a block-scoped model that is
  destroyed at once; the model it later reads (TRANS 2 at +0x108, STNOUT/ELMNOD) is a second,
  never-filled one (`facf0.asm` 1413fae3f / 1413faeaa: two `0x140821bf0` at stack+0xc0 and +0x34e0,
  the read goes to +0x34e0). Kept as in the binary. When OprFlag is 0 or the wizard's HeatTransType
  is -1 (the ctor `0x1413f68e0` default; Forming.FWZ files of the corpus have no such key), the type
  comes from the sub type (OPI+0x128): AirConvect 1, HeatResting 2, HeatDwelling 3, HeatExpress 0,
  else from the previous operation. Forming type 0 becomes 2. Object checks: when an object slot has
  no object the GUI adds one with Express defaults (`0x140057ba0`, `0x141401bd0`): **refused**; OprFlag
  0 then applies the props defaults: **refused**. When the type is 0 and more than one object is
  defined (`0x1401cab30`) the GUI merges the objects into object 1 and deletes the rest: **refused**.
  Finally LubricantID (OPI+0x48) is set to 0 when it is not.
* Props `0x1413ed890`: setters `0x1405fb720`, `0x1405fbae0`, `0x1405fbe30`, then vt+0x68 `0x1413ed7d0`:
  step data at props+0x60 (STPDEF - 1, DTMAX, DTPMAX 1..3, STPINC).

## 4. FormingExpress (SfOprFormingExpressCommon)

* Prologue `0x14141bc90`: base, TRANS 2 = 1, TRANS 1 = 0; SC+0x240 = 0; DSMAX = 0.01 when
  |DSMAX| < 1e-6; `0x141424af0` (S+0xa30/+0xa34/+0xa68 = 1 and S+0x1118 = 0 in 2D, else 2); OprFlag 0:
  OPI+0xe0 = 1.
* Post-load `0x14141bda0`: `0x1413fa950`; PDIE = 2; `0x1413fc6b0` (TRANS 2 = 1 for a FormingExpress
  class, TRANS 1 = 0 for forming type 0, else 1); fewer than two objects: the GUI adds objects and
  equipment: **refused**; `0x14141f1f0` sets "assign movement" (slot +0x1bf) for object 2 and every
  object with a movement (`0x14084d4a0` = DEF_PRE `0x140179280`) or +0x2360 != 0. The loop over the
  object tree that creates equipment does not run on the open path (the tree OB+0x18 is cleared by
  `0x1400c3950` before vt+0x58 and filled by `0x1400547b0` after it). LubricantID = 0.
* Props `0x14142a6a0`: the four setters, then `0x14142c8b0`/`0x1414264d0` as for Forming without the
  types 5..7 and with the movement direction (+0x50..+0x60 -> D+0x88..+0x98).

## 5. Model changes made with the packet

* `data::ExpressData` member defaults = ctor `0x1413f68e0` (FormingType 0, DieTemperatureType 2,
  ShapeComplexity 50, SimulationAccuracy 50, HeatTransType -1, BuiltInFlownet -1.0, 2D geometry type 1).
* `HeatTransferExpressProps` (props.hpp) keeps the step data at +0x60 (same object as L2's
  `HeatTransferStepData`).
* `FormingProps`/`FormingData` moved from classprops.cpp to `m2m/props_forming.hpp`, with the doubles
  of the ctor `0x140c0aac0`.

## 6. Tests and corpus

`tests/mdt/test_engine_l3.cpp`: 49 op 1 (active HeatTransferExpress2D), 46 op 2 (HeatResting2D),
53 op 4 (HeatExpress), 56 op 1 (active Forming) and the refusal of a Forming operation without
equipment. Every L3 operation of the corpus (40) loads without a refusal. No corpus case has a
FormingExpress operation, so its slots are covered by the code reading only.

## 7. Open items

1. Equipment creation of `0x140c06130` (equipment manager vt+0x160 and the KEY writer path).
2. The Express wizard defaults singleton `0x141401bd0` (0x1b30 bytes) and object add/merge/delete
   (`0x140057ba0`, `0x140057770`, `0x1400581d0`) used on first set-up.
3. `0x1400b7fb0` (chain walk of `0x1413faaa0`) for OprFlag 0 FormingExpress operations.
4. Oracle B: a FormingExpress project and a Forming project without equipment would test 1-3.
