# DOE study files (SfMoDoeManager), MDT L6T

Status: in progress. Decompiles are in `deform-rea-work/gui_mo/s1/mdt/DOE/d0.c` .. `d5.c`. Code is
in `apps/moproj2mst/src/doe/doe_study.cpp` and `include/m2m/doe_study.hpp`. The oracle B cases are
120_B_DOE (a study, no variable), 121_B_DOE_VAR (TLOC on op 1, 3 samples), 122_B_DOE_GEO
(Workpiece geometry), and 123_B_DOE_COR (blocked: Edit Geometry needs real mouse input).

## 1. Who calls what

| When | Caller | Calls |
|---|---|---|
| sub-project ctor (`1400b5470`, `1400b5680`) | manager ctor `1400d1180` | `name = "DoeStudy" + id`; `140909660(<proj>\DoeStudies\<name>)`: mkpath the folder, `Control`, `Solutions`, `Output` |
| open, `<SfDoeStudy>` of a DOE sub-project (`14008f530`, `1400909a0`) | | `1400d2aa0` (name), `1400d3970` (DoeInputTable.XML via `14090e080`), `1400d7a80(mgr, 0/1/2, Control\Samples_LHS/FF/USR.par)` |
| Save step 3, .moproj sub-project part `140099040` | | `1400d3200` (DoeInputTable.XML via `14090a550`), then the PAR files (`1400d75a0`, L10's port) |
| Save after the MST, `1400980d0` -> `1400a20d0` (DOE or optimisation sub-project) | | `1400ba650`: `<Control>\<proj>.MST.TEMP` (`140131530`), `DEF_DOE_PARSER.INI` (`140915eb0`); `1400ba800`: DEF_DOE.INI and the rest |
| variable edits (UI) | | `1400d69c0` (remove a variable's .GEO/.COR), `1400d6dd0` (rename them) |

From the mtimes in 120: COG_P1_B1.DS, DoeInputTable.XML, DEF_DOE.PAR and Samples_LHS.par are
written before the .moproj. Cogging.MDT and the MST follow, then COG_P1_B1.MST.TEMP,
DEF_DOE_PARSER.INI and DEF_DOE.INI. Add DOE Study already writes the moproj and the Control
files, and the geometry "+" writes the three .GEO copies, all before Save (oracle B note).

## 2. Ported

* `140819600` fileNameOf, `1400d5970` / `1400d5ee0` / `1400d6450` geoPath, `140909660`
  makeStudyDirs, `1400d69c0` removeVarFiles, `1400d6dd0` renameVarFiles (the Nominal .COR is
  renamed to itself, as in the original), `14090a550` writeInputTable, `1400d3200`
  saveInputTable, `14090e080` readInputTable (header elements), `1400d3970` loadInputTable,
  `1400d29d0` / `1400d2aa0` the SfDoeStudy element.
* DoeInputTable.XML is byte-identical with 120 (`test_doe_study`).
* Refused: variables and constraints (the writer's variable part and `1400d2b60`), and the readers'
  child elements (`14090ee20`, `140912a20`, `140912b00`, `140912d10`). They wait for 121 and 122.
  The .COR writer waits for 123.

## 3. To do

`140915eb0` DEF_DOE_PARSER.INI, the MST.TEMP through `140131530` with the DOE sub-project, and
`1400ba800` DEF_DOE.INI. COG_P1_B1.DS (`140907a10`?), `1400d7a80` (Samples files at open: it writes
with an empty list, a GUI bug L10 found), the PAR writers `140915290` .. `140915cd0` (L10's port
in gmodoe is to compare with), and the open path for a DOE sub-project, which still refuses in
`src/model/subproject.cpp`.
