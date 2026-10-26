# Cogging.MDT writer (DEF_GUI_MO_64 Save): static spec

Packet MDT of stage 1 (moproj2mst), branch `claude/m2m-mdt`. Static evidence from Ghidra
decompiles and disassembly of the exe copy (`deform-rea-work/gui_mo/s1/mdt/`: `w_*.c`, `save_d0.c`,
`glob_d0.c`, `ver_d*.c`, `a8c0.asm`, `17e0.asm`; strings resolved with `annot.py`, RIP-relative
references with `rip.py`, vtable calls with `vcall.py`). The reader side is P06
(`src/datafiles/mdt.cpp`, s1-03 section 5).

## 1. When the GUI writes it

File > Save is `0x1400980d0`:

1. per sub-project check `0x1400b8fb0` (aborts the save when it returns other than 0x10000),
2. `0x1400983c0(project, 0)` writes the `.moproj`,
3. `0x1400982e0(project)` walks the operation list (model vt+0x1b8 count, vt+0x1c0 item i, in list
   order) and calls **properties vt+0xb8** on every operation that has properties,
4. `0x1400a20d0` writes the MST, then `LOCAL_REMESH.DAT` handling.

For `SfOprCoggingProperties` slot 0xb8 is `0x140760400` → `0x1407616d0(PROPS+0x40 = CogModel)`:
when the model's owner item (+0x548) and its task folder (`0x1405f8d90`, `0x140050df0`) exist, it
calls `0x1407617e0(model, taskDir)`; otherwise it returns -1 and writes nothing.

`0x1407617e0` builds `convertSeparators(taskDir + QDir::separator() + "Cogging.MDT")`
(`0x140761580`), opens it with `fopen(path, "wt")` (text mode: every `\n` becomes CR LF) and, only
when the open succeeds, writes the file (section 3) and closes it. It returns 1 either way: a
failed open is silent in the original.

Other properties classes have their own slot 0xb8 (Boolean, Express family, Cycle, HeatTreat,
Distortion, Diestress, DiestressBM): not part of this packet.

Corpus check (oracle A): all 27 `Cogging.MDT` of `gui_mo/s1/corpus` have an mtime within 5 ms of
their `.moproj` (the multi-operation projects 07, 53 and 54 included), so every one was written by
the Save that wrote its project.

## 2. Inputs

The operation's `<Properties>` element is empty for `SfOprCogging` (`Number="0"` in every corpus
project), so no cogging setting in the MOPROJ reaches the MDT. The project's objects do, but only
for the active operation (section 6). The bytes come from:

* the operation's cogging model, which the P06 reader filled from the same `Cogging.MDT` at open
  (nothing between open and Save changes it without user input);
* process globals of the cogging module, last set by whichever `Cogging.MDT` was read last or by
  the last `SfOprCogData` ctor (`0x140767a20`, run when cogging properties are made). With several
  cogging operations, every MDT written gets the globals of the **last** read (list below);
* the program/version text of the DEFORM installation (header line 3).

Process globals the writer reads (all set by the reader or the ctor only; `rip.py` finds no other
writer; setters `0x1407991c0`, `0x140799200`, `0x14076be80`, `0x14076bfc0`, `0x14076bf80`,
`0x140796bb0`, `0x14076bd70` are called only from the reader `0x14076aca0` and the ctor):

| Global | Meaning | Initial | Ctor `0x14076b3d0` | Reader |
|---|---|---|---|---|
| `0x143db1a20 + k*0x80`, k=0..2 | Defaults HEAT slots 0..2 | ctor | `0x1407979c0` resets | Defaults section |
| `0x143da7e00` (0x108) | default PASS | ctor | `0x140791530` | Defaults Pass; later reheat allocs set its die positions, manip distances and DieMovDir (`0x140791870`) |
| `0x143da7cc0` (0x140) | default BITE | ctor | `0x1407906e0` | Defaults Bite |
| `0x143da6de8` | unit (UNIT = value-1) | .bss 0 | 2, then GUI unit | `UNIT`+1 (exit(1) unless 1..2) |
| `0x1437e4040` / `0x1437e4048` | POSERR / INTERR | .data 0.001 / 0.001 | - | float widened |
| `0x143da6dd0` / `dd8` | CVGERR1 / CVGERR2 | .bss 0 | 0.005 / 0.05 | float widened |
| `0x143da6df8` / `df0` | AVGSTR / LMTSTR | .bss 0 | 1.0 / 0.01 | float widened (ver > 0x13ec) |
| `0x1437e3a80` | CTLPCT | .data 50.0 | - | float widened |
| `0x1437e3444` | DieMov | .data 1 | - | `DieMov` |
| `0x1437e3440` | NoDies | .data 2 | - | `NoDies`; **the writer sets it** to 4 when die+0x14a8 == 1, else 2 |
| `0x143dacd70 + (k-1)*0x400` | KeyDie1..4 text | ctor `0x140794ce0`: `"DIE%d.KEY"` | - | **not** written by the reader |

KeyDie1..4 in the written file therefore always read `DIE1.KEY`..`DIE4.KEY`, whatever the read
MDT said (the reader stores its KeyDie lines in die+0xca8.., which the writer does not read).

Header line 3 is `"* %s %s"` of `0x143dbd100` ("DEFORM-2D/3D", set at start-up) and `0x143dbcbd0`
("11.0" plus a suffix that `0x1407fb240` builds from the `VERSION` line of
`%DEFORM_DIR%/PACK/pack.dat` (3D mode: `%DEFORM3_DIR%`): `VERSION 2` gives
`"11.0 (Service Pack 2)  "`; text containing Beta/RC/SP is used as is in parentheses). The
v11.0.2 install's pack.dat holds `VERSION 2`. Three corpus files (53, 54) carry
`11.0 (Beta ReleaseCandidate1)  ` from an older install.

## 3. File layout (in call order)

`B(m, text)` is `0x14076b450` (vsprintf into a 4 KB buffer, then by mode): mode 0 `"*\n* %s\n*\n"`,
mode 1 `"* %s\n* %s\n* %s\n"` with the separator, mode 2 `"* %s\n*\n* %s\n*\n* %s\n"` with the
separator (`* ` + 78 `*`) before and after, mode 3 `"*\n*\n* %s\n*\n*\n"`, mode 4 `"%s\n"`.
Values: `I` = `"%s %6d\n"`, `E` = `"%s %15.7E\n"`, `S` = `"%s %s\n"`; keys below are the exact
`.rdata` strings (trailing spaces matter).

`0x14076a8c0(cog = model+0x540)`:

```
B(2, "%s %s" prog version)  B(2, "MASTER DATA FILE %d" 11000)
S "PRBID  " cog+0     S "TITLE  " cog+0x64
B(0,"Defaults")  B(2,"Reheats")  HEAT(slot0) HEAT(slot1) HEAT(slot2)
B(2,"Pass")  PASSFIELDS(default PASS)          // 0x140792d40, ends with its HEAT
B(2,"Bite")  BITE(default BITE, n=1)           // 0x140790d10
BILLET(cog+0xd8) DIE(cog+0x1170) MANIP(cog+0x2e20) SIMPAR(cog)
B(2,"Simulation Data")  I "SIMLST " total      // 0x1407697d0
SIMLST lines (0x14076c1c0 per reheat)          // "%4d %s\n"
I "NoReheats" n
REHEAT loop (0x14076cb50)
```

then `0x1407617e0` appends `B(0,"Process Data")` and the 20 Process Data lines of the model.

HEAT `0x140798220`: `B(0,"Heat Transfer")`, I `"Active "` +0, I `"NSTEP  "` +4, I `"STPINC "` +8,
E `"TMAX   "` +0x10, E `"DTPMAX "` +0x18, `"%s\t0\t0\n"` `"ENVTMP "`, E `"IHTCOF "` +0x20,
E `"IHTCOF "` +0x28, S `"UserKey"` (+0x30, or `"---"` when its first byte is ' ').

PASSFIELDS `0x140792d40`: I `"NoBite     "` (+4, or 1 when AutoBite), E `"RotAng       "`,
E `"BiteAngle    "`, E `"MinDieDis    "`, I `"DiePosMethod "`, E `"DieStrtPos   "`,
E `"DieStopPos   "`, I `"DieStrtNod   "`, I `"DieStopNod   "`, I `"DieMovDir    "`,
I `"AutoBite     "`, E `"LeftManDis   "`, E `"RightManDis  "`, E `"HorizDieShift  "`,
I `"BitesToSkip  "`, E `"RadialFeedPerBite  "`, E `"DwellTime  "`, HEAT(+0x28).

BITE `0x140790d10(b, n)`: `B(0,"Bite %d" n)`, HEAT(+0xc0), `0x140791220`: `B(0,"Bite Deformation")`,
I `"NSTEP  "`, I `"STPINC "`, I `"PDIE   "`, E `"SMAX   "`, E `"LMAX   "`, E `"ClampForce"` +0x40,
E `"DTSUB  "` +0x20, I `"StepMod"`, E `"DSMAX  "`, E `"DTMAX  "`, I `"FricTyp"`, E `"FRCFAC "`,
E `"IHTCOF "` +0x58, E `"IHTCOF "` +0x60, S `"UserKey"` (+0x68 or `"---"`), E `"NomBite"` +0xb8.

BILLET `0x140797550`, DIE `0x140795fd0`, MANIP `0x1407946d0`, SIMPAR `0x14076aed0`: field lists in
`src/datafiles/mdt_write.cpp` with their offsets. DIE quirks: `DieMov` from the global;
`SPDDIE` is the die speed when TOTSTROKE <= 0 or CYCLES <= 0, else 0; NoDies global set as above;
KeyDie from the global table. SIMPAR: `UNIT` = unit global - 1, POSERR .. CTLPCT from the globals,
`UseManip` = cog+0xc8 (byte, zero-extended).

SIMLST labels (`0x14076b8e0`): heats of reheat r `RH%02d-HI` / `-HeatUp` / `-HO` when active;
per pass p: with AutoBite == 0, per bite b `RH%02d-PS%02d-BH%03d` (bite heat active) and
`RH%02d-PS%02d-BD%03d`; with AutoBite, bite 1 only as `-BH-Auto` / `-BD-Auto`; then the pass heat
(when active) `RH%02d-PH%02d-Fix` (AutoBite == 0) or `-Auto`. Numbered from 1 across the file.

REHEAT loop (disassembly `14076ac03..14076ac72`): the pointer is **accumulated**, not indexed:
`p = p_last_of_SIMLST_loop + i*400` added on top of the previous `p`. Reheat i (0-based) of the
second loop is element `n-1 + i(i+1)/2`, so with NoReheats >= 2 the original writes reheat 2's data
under "Reheat 1" and then reads past the reheat array. Not reproducible: the port refuses it.
`0x14076cb50`: `B(0,"Reheat %d")`, I `"NoPass"`, HEAT x3, then per pass `B(0,"Pass %d")`,
PASSFIELDS, and its bites (all, or the first with AutoBite).

Process Data (`0x1407617e0`): I Process_Type +0, I Temperaure_Type +4, I Use_Mandrel byte +8,
I Use_Adaptive_Reheat byte +0x4cc, I Checking_Method +0x4d0, `"%s %15.7E %15.7E\n"`
Temperature_Range +0x4d8/+0x4e0, I Solver_Type +0x4f0, I Use_DifferentDies byte +0x14,
E Adaptive_Heating_Time +0x4e8, E Swaging_Stroke +0x4f8, E Dwell_Time +0x510, E Cycle_Time +0x508,
E Decompression_Time +0x518, I Bottom_Fixed +0x520, I Heat_Transfer byte +0x504,
I Remaining_Cycle_Time_Dwell byte +0x505, E Clamp_Force +0x528, E Clamp_Speed +0x530,
I RotationalSymmetry byte +0x15, I Num_Steps_DSMAX +0x500. Bytes are zero-extended (MOVZX).

## 4. What the port refuses (explicit errors, no file written)

* NoReheats >= 2 (section 3, REHEAT loop).
* A unit outside 1..2 (the original exits through `0x140b2f9d0`).
* The project's active diagram item (`<Subproject Active="k">`) is a cogging operation (section 6).
* A task folder path with characters above U+00FF (the original's `fopen` of the 8-bit path fails
  silently).

## 6. The current operation is saved first

Before the `.moproj`, Save calls `0x140052ec0` on the current operation (`0x140047a40` → the active
diagram item of the active sub-project, read from `Active` at open). When its class has that
operation loaded (`0x1405ef9d0`, OB+0x20), it calls class vt+0x68 (`0x1405ef510`): vt+0xd0, vt+0xd8,
vt+0xe8, vt+0x80. For `SfOprCogging` vt+0xd8 is `0x14075c260` → `0x140762ec0(model, 1)`:

* `0x140766200(model, 1/2/4)`: billet, die and manipulator geometry from objects 1, 2 and 4 of the
  operation. An object of primitive type t >= 6 gives GeoTyp t-6 and its parameters
  (`0x140198b20(obj, k)`); otherwise GeoTyp -1 and the extents of its geometry's bounding box
  (`0x140c82d50`, `0x1411f5040`, `0x14121a1a0/1f0/240`) into Length, Width, Height and Dia.
* `0x140763e50`, `0x140762f30`: object positioning (writes object records +0x1ec8, +0x21b0..,
  +0x22c0..).

Oracle B: `42_vdie` (Active = 3, the cogging operation) re-saved by the GUI changes the billet
Length, Dia, Height and Width (corpus 1.3783558E+003 / 4.0425813E+002 / 4.0428906E+002, re-save
1.1125596E+003 / 4.0432001E+002); its MST is unchanged. The port does not reproduce this sync, so
it refuses to write MDTs when the active item is a cogging operation. In the corpus that is 42 and
54 (Active = 0 is a cogging operation there; its re-save happened to change nothing).

## 5. Open questions

* Q1: the program/version text is taken from the installation; the port defaults to the v11.0.2
  SP2 text and lets the caller give a DEFORM folder whose `PACK/pack.dat` is parsed as
  `0x1407fb240` does. Oracle B confirms which text the installed GUI writes.
