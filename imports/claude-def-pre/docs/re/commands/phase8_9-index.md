# Phases 8–9: external-tool commands, CNVT3D and file input

Specs recovered from DEF_PRE_64.exe v11.0.2 for the commands that drive external programs or
import/convert data. Shared mechanics (dispatch, argument readers, the launcher
`FUN_140223790`, `DEFORM_PC_PATH`): [common.md](common.md). Each spec lists its argument
grammar, model fields, files written, launch, read-back, messages and helper addresses.

## Commands

| Command | Reg. id | Handler | External program(s) | Spec | Cases |
|---|---:|---|---|---|---:|
| DEFAMG | 293 | `FUN_1401a9be0` | 2D `DEF_AMG`; 3D `DEF_AMG3_GUI`, `MESH3D_GUI`, `BRKMSH`, `CHKVOL` | [DEFAMG.md](DEFAMG.md) | 8 |
| REMESH | 291 | `FUN_1401a9a10` | BRDEXT → DEFAMG → DEFINT chain | [REMESH.md](REMESH.md) | 6 |
| BRDEXT | 292 | `FUN_1401a9ac0` | `AMG_DXT` (2D) / `AMG_DXT3` (3D) | [BRDEXT.md](BRDEXT.md) | 5 |
| DEFINT | 294 | `FUN_1401aa000` | `DEF_INT` (2D) / `DEF_REM3_GUI` (3D) | [DEFINT.md](DEFINT.md) | 4 |
| DEFFRZ | 296 | `FUN_1401aa370` | `DEF_FRZ` (2D) / `DEF_FRZ3` (3D) | [DEFFRZ.md](DEFFRZ.md) | 5 |
| MORP | 297 | `FUN_1401aa750` | `DEF_MORP2.EXE` / `DEF_MORP3.EXE morphing DEF_MORP3.INI` | [MORP.md](MORP.md) | 3 |
| GFREAD | 298 | `FUN_1401aa9a0` | in-process readers; 3D `.PDA/.PAT` → `PDA2KEY3`, `.UNV` → `STB2KEY3` | [GFREAD.md](GFREAD.md), [DXF_IGES_READERS.md](DXF_IGES_READERS.md) | 10 |
| GENINT | 15 | `FUN_1401ac020` | none (KEY template) | [GENINT.md](GENINT.md) | 3 |
| IMPLUB | 16 | `FUN_1401ac310` | none (KEY file) | [IMPLUB.md](IMPLUB.md) | 4 |
| IMPEQP | 17 | `FUN_1401ac680` | none (KEY file) | [IMPEQP.md](IMPEQP.md) | 3 |
| ARMCTL | 21 | inline case 0x15 | none (writes `DEF_ARM_CTL.DAT`) | [ARMCTL.md](ARMCTL.md) | 3 |
| EXECMD | 11 | inline case 0xb | any (mode 1 `system`, else launcher) | [EXECMD.md](EXECMD.md) | 4 |
| CNVT3D | 287 | `FUN_1401467f0` | `DEF_AMG3_GUI`, `MESH3D_GUI`, `BRKMSH`, `CHKVOL`, `DEF_INT`, `DEF_AMG`, `FIXGEO.EXE` | [CNVT3D.md](CNVT3D.md) | 8 |
| Menu 9 | – | `FUN_14000afd0` | none (stub: asks y/n, does nothing) | [MENU9_2D3D.md](MENU9_2D3D.md) | 3 |
| Menu 2 file input | – | `FUN_140014ba0`, `FUN_14000b170` | `DEF_CVT3 KEY IGE/ANS/STL/PDA/STB/UG2` | [FILE_INPUT_MENU.md](FILE_INPUT_MENU.md) | 8 |

Not reachable from the batch paths (no code caller found): `DEF_COG_AMG`, `DEF_LOCREM3`,
`COATINGMESH` (behind a count nobody sets), `DEF_IGS.EXE` and two of the three FIXGEO launchers
(`FUN_1402e6c10`, `FUN_1402e7930`).

## Headless / licence verdicts (observed 2026-10-21, this PC)

All programs are run unchanged from the install, from DEF_PRE's working directory, through
`cmd /c` (waits; exit code ignored). Observed with `tools/extprobe/probe_ext.py` (empty input)
and by running the test cases through the oracle with and without capture wrappers
(`tools/extprobe/make_wrappers.py`). **No program opened a window or asked for a licence.**

| Program | Reached by | Verdict |
|---|---|---|
| `2D\DEF_AMG.exe` | DEFAMG/REMESH 2D, CNVT3D brick | headless; meshes and writes `<prob>_<obj>.KEY`. Prints a `CHKVLD` line, no prompt. On EOF of stdin it loops its menu forever (only when run without its `.ACT` file) |
| `3D\DEF_AMG3_GUI.com` (→ `DEF_AMG.exe`) | DEFAMG/REMESH 3D, CNVT3D tet | headless; writes `MESH3D.INP`, `NBCD.OUT`, `DENSTY.DST` |
| `3D\MESH3D_GUI.com` | DEFAMG/REMESH 3D, CNVT3D | headless; writes `mesh.out`, `MESH3D.MSG`, debug files (`surf.*`, `dbg_surf.*`, `*.diag`) left in the folder |
| `3D\BRKMSH.exe` | after MESH3D_GUI | headless; writes `SCRATCH.KEY`, `BRKMSH.KEY` |
| `3D\CHKVOL.com` | after BRKMSH when `CHK_VOL.INI` > 0 | not reached by the cases (no `CHK_VOL.INI` > 0); empty-input run untested |
| `2D\AMG_DXT.exe`, `3D\AMG_DXT3.com` | BRDEXT, REMESH | headless; write `<prob>.GEO`, `.RES` (+ `NBCD.DAT`, `DSTPAR.DAT`, `DRZ.DAT` in 3D) |
| `2D\DEF_INT.exe` | DEFINT/REMESH 2D, CNVT3D | headless; writes `<db>_REM.KEY` |
| `3D\DEF_REM3_GUI.com` | DEFINT/REMESH 3D | **fails on this PC**: prints "Running 32bit simulation", deletes `DEF_REM3.INI` and runs the 32-bit `3D\DEF_REM.exe`, which cannot start (0xC0000135: `mpich.dll` is not installed). DEF_PRE then prints `Failure`. `3D\DEF_REM_64.exe` run directly on the same inputs works headless and writes `NEWMESH.KEY`. Port and original fail the same way here |
| `2D\DEF_FRZ.exe`, `3D\DEF_FRZ3.com` | DEFFRZ | headless; write `DEF_FRZI.KEY`/`DEF_FRZI.TMP` (2D), `SCRATCH.KEY`/`SCRATCH.DAT` (3D) |
| `2D\DEF_MORP2.EXE`, `3D\DEF_MORP3.exe` | MORP | headless; both start and read their INI (test INIs carry dummy names, so they stop with "can't open" / Fortran EOF) |
| `3D\STB2KEY3.com` | GFREAD `.UNV` | headless; writes `SCRATCH.KEY`, `STB3_MESH.TMP` |
| `3D\PDA2KEY3.com` (→ `PDA2KEY.exe`) | GFREAD `.PDA/.PAT` | **hangs when `PDA3.INI` is missing**: waits for console input even with stdin from NUL. Untested with a real PATRAN file |
| `2D\FIXGEO.exe` | CNVT3D geometry path | headless (the CNVT3D die case succeeds; with FIXGEO absent DEF_PRE crashes 0xC0000005) |
| `3D\DEF_CVT3.com` | menu 2 options 3–8 | **never found**: launched with dirKind 0, so DEF_PRE looks for `<install>\DEF_CVT3` and `cmd` reports "not recognized". The imports through menu 2 cannot work with this install layout |
| `DEF_LOCREM3`, `COATINGMESH`, `DEF_COG_AMG`, `DEF_IGS` | unreachable | empty-input runs only: no window or prompt; `DEF_COG_AMG` (→ `DEF_AMG2D.exe`) loops its menu on EOF |

Practical rules for the port and its tests:

* DEF_PRE sets `DEFORM_PC_PATH` (and eight other variables) to its own folder at start-up, so the
  external programs come from `<DEF_PRE folder>\2D\` or `3D\`. The port takes the install folder
  from configuration and sets the same variables.
* Capture runs: `make_wrappers.py OUTDIR` builds a folder with a copy of DEF_PRE_64.exe and
  wrappers that save each program's inputs and outputs under `_ext_capture\`. Names launched
  with an extension (`FIXGEO.EXE`, `DEF_MORP2.EXE`, `DEF_MORP3.EXE`, `DEF_LOCREM3.EXE`,
  `COATINGMESH.EXE`) cannot be wrapped this way; their cases are checked in normal runs only.
* Many exchange files are deleted by DEF_PRE after the read-back; the capture runs are the only
  way to compare the port's input files with the original's.

## Follow-up specs (porting depth)

| Spec | Covers | New cases |
|---|---|---:|
| [GFREAD_STL_GEO_NAS.md](GFREAD_STL_GEO_NAS.md) | GFREAD STL (detect, readers, vertex merge, orientation fix), 3D/2D GEO, NASTRAN | 17 |
| [IGES_READER.md](IGES_READER.md) | GFREAD 2D IGES: driver, Fortran passes 1/2, storage | 16 (26 files) |
| [DXF_READER.md](DXF_READER.md) | GFREAD 2D DXF: pair loop, 10 entity handlers, segments, Fortran chain stage | 25 |
| [CNVT3D_SWEEP.md](CNVT3D_SWEEP.md) | CNVT3D brick sweep, 3D numbering, NBCD.OUT, state transfer | 9 |

Every new case ran against the original and matched the spec's prediction (reference models for
IGES and DXF in `tests/commands/GFREAD/models/`).

## Test cases

`tests/commands/<CMD>/*.json` in the oracle case format (76 cases). Run them with

```
python -I ../claude-deform-rea-oracle/tools/oracle/def_pre_oracle.py --timeout 600 -j 4 --copy-to OUT cases tests/commands/*/*.json
python -I tests/commands/summarize_runs.py OUT
```

All 76 ran on 2026-10-21 against the install (normal mode) and with capture wrappers; every case
completed (the one that hung, EXECMD mode 3 with `PDA2KEY3`, now runs `FIXGEO`). Each case's
`expect` block holds the prediction from the code; where the run differs, the spec says so.

## Open items

* 3D interpolation (DEFINT/REMESH 3D) cannot be verified on this PC until `DEF_REM3_GUI` runs a
  working build (32-bit `mpich.dll` missing).
* DEF_CVT3 imports (menu 2 options 3–8) never launch with this layout; IGE/ANS/UG2 may also be
  unsupported by DEF_CVT3 itself (strings only).
* Partly specified: GFREAD's STL/GEO/IGES/DXF/NASTRAN parsers, the CNVT3D sweep algorithms,
  DXF entity handlers, the DEFFRZ double argument, BRDEXT's qsort comparator. Each spec ends with
  its own list.
