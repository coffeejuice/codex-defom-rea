# Multiple Operations GUI: .moproj open to Save and the .MST (preliminary)

DEFORM v11.0.2. Static evidence only: strings, the import table, and Ghidra
decompiles (no auto-analysis, functions created at the addresses below) of a
copy of `DEF_GUI_MO_64.exe`, plus copies of `C:\PROBLEM\TEST_REA\COG_P1_B1`.
Nothing was run. The start-up function `140001410` also holds the licence
check, so it was not decompiled; argv handling there is read from strings only.
Addresses are VAs in `DEF_GUI_MO_64.exe` (image base `0x140000000`).

## Short answer

```
DEF_GUI23.exe
  └─ DEF_GUI_MO_64.EXE -m open -f <file>        (or DEF_GUI_MO.EXE on 32-bit)
     DEF_GUI_MO_64.EXE -m new  -prob_dir <dir>
        Open : QDomDocument::setContent(<name>.moproj) → build model, cd to project dir
        Save : per-op checks → save current operation → <name>.moproj (QDom, UTF-8)
               → object hook → <name>.MST (QTextStream, in-process)
               → [DOE study] <name>.MST.TEMP + study folders
Run (DEF_GUI23 → DEF_SIMULATION → DEF_ARM_CTL3 → DEF_RDMS3/DEF_PRE, see gui-run-chain.md)
```

The MST is written in-process by `DEF_GUI_MO_64.exe`. No helper exe is used for
Save. DEF_PRE is started by the MO GUI only for a single-operation database
step (see "DEF_PRE from the MO GUI").

## Binaries

- `DEF_GUI_MO_64.exe` (67 MB) is a native Qt 4 app (MSVC x64), not .NET. It
  imports `QtCore4_64`, `QtGui4_64`, `QtXml4_64`, `QtNetwork4_64`, `QtSql4_64`,
  `QtOpenGL4_64`, `QtSvg4_64`, `QtCommercialChart_64`, plus OpenGL/GLU and
  Win32. All project, KEY and MST logic is linked into the exe; no DEFORM DLL is
  involved. It uses `QProcess` (and `CreateProcessA`) for children.
- `DEF_GUI23.exe` strings `%s -m new -prob_dir %s` and `%s -m open -f %s` with
  `DEF_GUI_MO.EXE` / `DEF_GUI_MO_64.EXE` (0x2b3c0c to 0x2b3c9c). It asks "A
  project file %1 exists, Do you want to open it?" and accepts a `.MOPROJ`, a DB
  or a KEY file. The MO exe has `prob_dir` and `slotActionFileNew()` next to its
  start-up strings (ref'd from `140001410`).

## .moproj format

Qt `QDomDocument` XML, `<!DOCTYPE ProjectFile>`, root `<Project>`, UTF-8,
written with `toString(1)` (one-space indent). Children in order:

| Element | Content |
|---|---|
| `Object_List Number=` | `MoItem`: `Name`, `Id`, `Path` (`Objects/ObjectNNNNN.KEY`), `ObjectInfo ObjectType MatId HasGeo HasMesh` |
| `Material_List` | `MoItem`: `Name`, `Id`, `Path` (`Materials/MaterialNNNNN.KEY`), `MatType` (`Single`; `PhaseList` for mixtures) |
| `Lubricant_List`, `Equipment_List` | `MoItem`: `Name`, `Id`, `Path` (`Lubricants/…`, `Equipments/…KEY`) |
| `Operation_List Number=` | `Operation`: `Name`, `Id`, `Type` (class name, e.g. `SfOprHeatTransfer`, `SfOprCogging`), `SubType`, `OprFlag`, `Equipment Id`, `Lubricant InterObjectMethod Id`, `Objects` → `Object` (attrs `Id Name ObjectType MatId EquipmentID HasGeo HasMesh Remesh SkipDefine MeshSetting Mesh2dAbsolute Mesh3dFinerInternal DefineBCC AssignMovement DefineRotationMovement ThermalStop`) each with `ObjPropertiesKey/Data`; `ActionKeywords/Keyword` (raw KEY lines such as `INICTC 1`, `GENCTC 0`), `NSTART/StartStepNo`, `UserDefinedKey`, `RestartKey` (`KeyFileName`), `Properties` (typed tree: `Property Name TypeName Value`, e.g. `doubleUnits` "0.1 sec") |
| `Title Name=`, `Unit Name=` (`SI`/`English`), `Primary_DB_Name Name=` | project header |
| `Sub_Project Active Number Type Id Name` | flow-chart view: `ViewTransform`, `SfDispView` (camera), `DiagramItem` per operation (`ClassName`, `ItemName`, `RectF`, `CellPos`, `GroupInfo`, `Connectors`, `Expansion_Items`); DOE studies hang here as `SfDoeStudy` with `Subproject_DbName`, `Samples_LHS/FF/USR.par` |

`ObjPropertiesKey/Data` is hex of `qCompress()` output (4-byte big-endian
length, then zlib). In COG_P1_B1 the first blob unpacks to the same text as
`Operations/Task00001/ObjProperties00001.KEY` (a 3-line KEY header plus
`OTPRNG`/`OPSTOP`), except LF instead of CRLF. The reader also accepts older
inline `SimCtrlKey` / `InterObjectKey` elements and a `.d2mo` extension.

The bulk data stays in KEY files beside the project:
`Objects/`, `Materials/`, `Lubricants/`, `Equipments/` and
`Operations/TaskNNNNN/{SimCtrl.KEY, InterObject.KEY, ObjPropertiesNNNNN.KEY,
RestartStep.KEY, *.MDT}` (names from `%1%2.KEY` with a 5-digit zero-padded id,
`14005eb10`, `140059290`).

## Open (File > Open, or `-m open -f`)

`14008a9b0` (called from start-up `140001410` and the menu path):
1. Picks the mode by suffix: `moproj` → project; `.DB` → open from database;
   `.KEY` → open from keyword file. Stores the cleaned path at project+0x18.
2. `.moproj`: `14008afa0` opens the file read-only, `QDomDocument::setContent`,
   then `14008b410` walks the DOM ("Error on reading project file!" on failure).
   The element names it looks for are the table above (strings 0x213e034 to
   0x213e8fc). Operations are built per `Type` class; KEY files are opened lazily
   per operation ("Open operation - %1", `140051340`/`140051c30`).
3. `.DB` → `14008b2d0`, `.KEY` → `14008b380` (not followed).
4. `QDir::setCurrent(<project dir>)`, refresh UI. Flags at +0x55/+0x56/+0x57
   record which of the three modes loaded.
5. On open it also offers to rename an outdated `FOR003` back to the primary DB
   name.

## Save (File > Save)

`1400980d0` (reached from the menu/toolbar handlers `140010b80`, `14001c2f0`,
and two slots) does, in this order:

1. Return early ("No") unless the project was loaded or saved as a .moproj (flag +0x55).
2. `processEvents`; for each operation `1400b8fb0` checks the object table and
   may raise a Yes/No warning; a non-"No" answer aborts the save.
3. Save the current operation: `140052ec0` emits "Save operation - %1", calls the
   operation's virtual save (vtable +0x68), then "Done - save operation".
   This is where `Operations/TaskNNNNN/*.KEY` are written (inferred from the
   message and the KEY-name helpers; the virtual was not followed).
4. Write `<project>.moproj`: `1400983c0`. Forces the `moproj` suffix, opens for
   write (message "Project file can't be saved. You don't have a writing
   permission in the project path."), adds `Title`, `Unit`, `Primary_DB_Name`,
   the object/material/lubricant/equipment lists (`14009cfd0`, `1400a1110`,
   `1400a16b0`, `1400a12f0`, `1400a14d0`), the operation list and the sub-project
   view, then writes the DOM as UTF-8.
5. `1400982e0`: calls virtual +0xb8 on every object in the object table. Most
   likely the per-object KEY write (`Objects/ObjectNNNNN.KEY`), not confirmed.
6. Write the MST: `1400a20d0` → `1401313c0`. If a DOE sub-project is active it
   also writes `<project>.MST.TEMP` through `140131530` and copies the study
   folders (`1400ba650`, `1400ba800`, `1400bb760`: `Operations`, `Objects`,
   `Materials`, `Equipments`, `Lubricants`, `DEF_DOE_PARSER.INI`).
7. Remove `LOCAL_REMESH.DAT` unless a local-remesh setting keeps it.

Save returns failure if step 4 or step 6 fails. The KEY files are therefore on
disk before the MST that references them.

## How the MST is built

All writing is `QTextStream` on a `QFile` in the project directory, path =
project path with `.MST` (`1400a50a0`). Nothing is converted by DEF_PRE at this
point; the MST only references the KEY files by path relative to the project
directory (`QDir::relativeFilePath`).

- `1401313c0`: returns false if there are no operations. Then
  - `140131680` header: banner, `*!MASTERFORMAT 4`, `*!TITLE`, `*!GEOTYP <n>`,
    `*!UNIT <n>`.
  - `1401318b0` main project block: `*!MAINPRJ`, `*!PRJNAM <name>`.
    (`140131a50` writes the `*!SUBPRJ` / `*!PRJNAM` variant for a DOE sub-run.)
  - `140131df0` operations: walks the flow-chart order; for each operation it
    calls the operation class's virtual MST writer (vtable +0x280) through
    `140132100` (plain run) or `1401321a0` (DOE sample, sets globals at
    `0x143d80258..`).
  - `140131d10` trailer: `* END OF MASTER FILE`.
- Each operation class writer (23 of them, e.g. `1401c4520`) emits one or more
  `SIMULATION n` blocks from shared emitters. Keyword → emitter:

| Emitter | Lines |
|---|---|
| `140131c10` | `SIMULATION n`, `*!SIMTYP <class>` |
| `140134250`, `140134360` | `DIM`, `NSTART` (first run only) |
| `1401343e0` | `DBREAD 0` + DB name (later runs) |
| `140134bf0` | `MDSOBJ`, `TNOW -1E21 0 1`, `KFREAD 1` + `Operations/TaskN/SimCtrl.KEY` |
| `140135220`, `140135680`, `140135b80` | `KFREAD` material, `OBJTYP`, `KFREAD` object, `MTLGRP` |
| `140135da0` | `CNTACT`, `KFREAD` InterObject.KEY |
| `140135fc0`, `1401360e0` | `TLOC`, `REMESH` |
| `140136810` | `IMPEQP <obj>` + equipment KEY |
| `1401365f0` | `KFREAD 1` + `ObjPropertiesNNNNN.KEY` (per object) |
| `140132bd0` | `GFREAD`, `DEFAMG`, `GENBCC`, `OutputGeoOp%1Obj%2.GEO` (geometry/mesh ops) |
| `140134730`, `1401347e0`, `140134900` | `OPRNAM`, `SIMNAM`, `CURSIM <sim> <op> 0` |
| `140134a10` | `GENDB <a> <b> <c>` + DB name, or `KFWRIT` |

Operation-specific lines (`OBJPOS …`, `TCYCLE`, `GENAXS`, `URZ`, `STROKE`,
`FORCE`, `TRANS`, `NSTEP`, `DTMAX`, …) come from each class writer and from
the operation's `ActionKeywords`.

## MST format (MASTERFORMAT 4)

Plain text with LF line ends. `*`-lines are comments; `*!` lines are directives read by the
batch side (`*!MASTERFORMAT`, `*!TITLE`, `*!GEOTYP`, `*!UNIT`, `*!MAINPRJ`,
`*!SUBPRJ`, `*!PRJNAM`, `*!SIMTYP`). The body is a sequence of
`SIMULATION n` blocks. Each block is a DEF_PRE batch command script (the same
commands as the `-m` CLI and the keyword port: `KFREAD`, `DBREAD`, `OBJTYP`,
`MTLGRP`, `CNTACT`, `IMPEQP`, `INICTC`, `GENCTC`, `OBJPOS`, `GENDB`, …). A
command whose argument is a file name puts the name on the next line. Each block
ends with `CURSIM <sim> <op> 0` and `GENDB … <prob>.DB`; the first block starts
from KEY files (`DIM 3`, `NSTART -1`, `GENDB 2 1 0`) and later ones continue the
database (`DBREAD 0`, `GENDB 1 1 2`). One operation can give several blocks
(COG_P1_B1: the Cogging operation is simulations 5 to 7, with `CURSIM 5 5`,
`6 5`, `7 5`). At Run, DEF_ARM_CTL3 uses `DEF_RDMS3.EXE "<prob>" "<n>"` to
extract block n and DEF_PRE to apply it (see `gui-run-chain.md`).

## DEF_PRE from the MO GUI

`1400a2200` is not part of Save. For one operation it writes that operation's
block (same `140132100` writer) to `DEF_MULTI.INI` in the project dir, deletes
`DEF_PRE.TDB`/`DEF_PRE3.TDB` and `DEF_PRE.MSG`, picks `DEF_PRE_64.EXE` when
`64bit.dat` exists (else `DEF_PRE.EXE`), and submits it through the GUI's
process runner with output redirected to `DEF_PRE.MSG` (`DEF_PRE > DEF_PRE.MSG`).
It is called from four UI paths (`14012bcc0`, `1401cae90`, `140e29270`,
`140eaf030`), which look like "generate database / preview this operation"
(`STEP.KEY`, `PREVIEW_DATA.KWD` helpers sit next to it). Not confirmed which menu.

## Open points

- Which virtual writes the object and operation KEY files (steps 3 and 5).
- `GEOTYP` and `UNIT` sources (`140c0df50`, `1400a2180`) and the meaning of the
  three `GENDB` arguments as the GUI picks them.
- The exact UI action behind `1400a2200`, and what DEF_PRE reads from
  `DEF_MULTI.INI` (probably the same block format).
- Argv parsing in `140001410` (skipped: it shares the function with the licence
  check).
