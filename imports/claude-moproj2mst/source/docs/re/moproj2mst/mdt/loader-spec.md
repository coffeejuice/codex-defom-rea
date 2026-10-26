# Operation loader (active item at open) and cogging geometry sync: analysis

Packet MDT, branch `claude/m2m-mdt`. Ownership (coordinator, 2026-10-22): this thread owns the lazy
operation loader `0x140051340` and OB vt+0x58 `0x1405ef430` with the class slots it calls, for every
operation class, and the cogging geometry step `0x140762ec0`. Status: analysis, nothing implemented.
Decompiles: `deform-rea-work/gui_mo/s1/mdt/load_d*.c`, `cur_d*.c`, `geo_d*.c`.

## 1. When it runs

After open the UI activates sub-project 0 and selects the item `Active`. `0x140047f80` finds its
operation and, when its class has it loaded (`0x1405ef9d0`), runs `0x140051c30` / `0x140051340`
("Open operation - %1"). That loads the operation into the GUI's DEFORM engine model (OPI+0x38) and
calls OB vt+0x58. At Save, `0x140052ec0` writes it back (inventory section 1, step 2).

## 2. OB vt+0x58 = `0x1405ef430` (all classes)

```
reset model (0x140c7b720)
vt+0xb8   class load prologue (Cogging 0x140750650: base 0x1405f1bb0, then the engine model's sim
          controls +0x16c = 100, +0x164 = 10, +0x160 = -1, 0x1408da6f0, +0x1e0, +0x2e4 = 1,
          +0x318 = 1, +0x2c8 = 0)
vt+0x78   (Cogging: empty)
vt+0x1b8  KEY / DB load, 0x1405f6520 by GUI mode 0x143d9da20 (.bss 0; set only by the Interactive
          Mode actions 0x140062f00 / 0x1400636d0, so 0 after a fresh start):
            0 -> 0x140055360(op, 1)    restart / setup-type decision, then one of the two below
            1 -> 0x1400572e0(op, step, 1)  load from a DB step (0x140c7b870), then 0x1400538f0
            2 -> 0x1400569d0(op, 1)    load from the operation's KEY data
vt+0xc0   class post-load (Cogging 0x1407507b0: object names and flags 0x140765dd0 for objects
          1..10, cogging model geometry from objects 0x140766740, KEY file reads, DEF_CNT.DAT
          "1\n" in the current directory when the object count < 3, equipment KEY via a temp file)
0x140054350 (props vt+0x58), 0x1400547b0(op, 0), 0x140054250 (viewport name only)
```

## 3. `0x140055360` decisions (mode 0)

* Restart: if the class supports restart (vt+0x1f8) and the operation's restart file is missing:
  `QMessageBox::critical` "Missing Restart File / Restart mode will be deactivated." and restart is
  switched off (`0x140064760(op, 0, 1)`).
* Setup type: when the operation is not the first (`OPI+0x10c == 0`) and the database has a step
  for it (`0x14005f520`), the GUI asks "Setup Type (Interactive/Batch)" with "YES - Interactive
  Setup" (load from the DB step) / "NO - Batch Setup" (KEY load), unless "Keep my choice" was ticked
  earlier in the same process (`0x143d77584/85`, not persisted). Convert2Dto3D and step-number
  rules pick the step (`0x14080e710`, `0x14080e5e0`); the DB branch then patches the start step
  (+0x160, +0x744, +0x650, +0x648) and, for the heat-transfer express family, resets their sim
  controls (`0x1408d5100`).
* Otherwise `0x1400569d0`: materials (`0x1408959e0` = DEF_PRE `0x1401c5ea0`), objects per slot
  (`0x14005aec0` -> `0x14005b030` / `0x14005b2f0`), equipment (`0x1403568a0`: reads the equipment
  KEY into a scratch model; a counter-blow hammer definition asks "Select object as the counter
  blow hammer:" in a `QInputDialog`).

## 4. Cogging geometry step at Save (`0x140762ec0`)

`0x140766200(model, 1/2/4)` (dimensions from objects 1, 2, 4; object +0x3aa4 primitive type,
+0x3ab0.. parameters; type < 6: GeoTyp -1 and the bounding box of the viewer geometry
`0x140c82d50` / `0x1411f5040`), then `0x140763e50` and `0x140762f30` (object positioning). The
positioning calls DEF_PRE engine code: `0x1408c0b50` = DEF_PRE `0x14019fa00` (OBJPOS interference
router, ported in `defcmd`), `0x140855d00` = DEF_PRE `0x140181ae0` (node bounding box, ported),
`0x140835430` = DEF_PRE `0x1401613a0` (object copy, ported).

## 5. Reuse of the DEF_PRE port

`gui_mo/s1/mdt/fmatch.py` matches functions by a masked byte signature (rel32 call targets and
RIP-relative displacements zeroed): 5,056 GUI functions have a unique DEF_PRE twin (`fmatch.tsv`);
1,249 of the twins are `ported` and 611 `verified` in `docs/re/pre64_batch_ledger.tsv`. Engine
object records in the GUI use DEF_PRE's `LLOBJ_DATA` offsets (name +0x828, node set +0x8a0,
movement +0x2160..). The GUI-side wrappers (`0x140c7xxxx`, `0x1411fxxxx`, `0x14121xxxx`) have no
twin and are ported as GUI code.

## 6. Dialogs on this path (need a policy: the converter has no UI)

| Dialog | Condition | Choices |
|---|---|---|
| Missing Restart File | restart on, file missing | OK (restart turned off) |
| Setup Type (Interactive/Batch) | not the first operation and a DB step exists | Interactive (DB) / Batch (KEY) |

Policy decided by Aleksandr (2026-10-22): no error for the dialog itself. The converter answers it with
the dialog's default button (`setDefaultButton(QMessageBox::Yes)` in `0x140055360`), as a fresh user
pressing Enter would ("Keep my choice", `0x143d77584`, is 0 in a fresh process): "YES - Interactive
Setup", the load from the DB step. This is a documented deviation (the GUI waits for a click). The
DB-step load itself (`0x1400572e0` -> `0x1400538f0`) and the step count of `0x14005f520` need the
project database, which is not read yet, so that path still stops with `M2M_E_UNSUPPORTED`. The
dialog occurs only for `OprFlag 0` operations; every corpus project has `OprFlag 1`.
| Counter-blow hammer | not on the load path: `0x1400569d0` calls `0x1403568a0` with noDialog = 1 (spec_A); only the interactive callers `0x140355ba0`, `0x14141ca90`, `0x1414364f0` show it | - |

None appeared in the 24 oracle-B MDT cases (no DB beside the copies, no restart, no counter-blow).
