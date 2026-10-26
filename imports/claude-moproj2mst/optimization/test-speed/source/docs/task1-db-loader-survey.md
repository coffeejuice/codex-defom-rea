# Task 1: which DEFORM v11.0.2 component loads simulation results from .DB

Scope: read-only survey of `C:\Program Files\SFTC\DEFORM\v11.0.2`, the two example
databases in `C:\PROBLEM\TEST_REA`, and the HTML manuals. Tools used so far: a small
Python PE parser (headers, sections, entropy, import/export tables) and string scans.
No disassembler yet. Nothing in the install was modified, and nothing in the licensing
code was analysed beyond noticing where it sits.

Legend: **[fact]** = read directly from a file; **[inferred]** = my interpretation, to be
confirmed by disassembly in task 2.

## 1. Short answer

There is **no dedicated "DB loader" DLL** in DEFORM. Every DEFORM tool that touches a
database links the same Fortran module **statically**:

- **[fact]** CVS marker `$Source: /var/lib/cvs/pre2d3d/datarm23.f,v $ Revision 1.81, 2014-04-08`
  appears in all of: `DEF_GUI_PST_64.exe`, `DEF_GUI_MO_64.exe`, `DEF_PRE_64.exe`,
  `DEF_SIM_64.EXE`, `3D\DEF_DBM.exe`, `3D\DEF_TEXT_PST.exe` (and the 32-bit GUIs).
  `datarm23.f` = the shared 2D/3D "database read manager". This is the module to re-create.
- **[fact]** The same DB signature/version table (`SFTC-DEF-3D-DB100-00`, `SFTC-DEFORM-3D-DB230`,
  `SFTC-DEFORM-2D-DB450/DB720`, `SFTC-DEF-2D-DB100-00`, `SFTC-DEFORM-23-DB100`) plus the list of
  DEFORM versions it accepts (`3.1.1` … `20.00`) is present in 12 binaries:
  DEF_GUI_PST_64, DEF_GUI_MO_64, DEF_PRE_64, DEF_SIM_64, DEF_LOCREM3_64, DEF_GUI23,
  DEF_GUI_PST23, DEF_PST, DEF_DBM, DEF_SPLDB, DATEXT3, DEF_TEXT_PST.

On top of that Fortran layer the Qt/C++ GUIs keep their own in-memory model:

| Layer | Where | Evidence |
|---|---|---|
| File container: header, step directory, block I/O, byte swapping, version checks | `datarm23.f` (Fortran), static in every tool | strings `DB HEADER INFORMATION`, `ISTART ISMTYP NRSETS NXTREC IPTYPE`, `NUMRST NUMRMS`, `SwapFlag`, `Error in DBSIZE()` |
| Step payload: step header + "A" (REAL) and "MAR" (INTEGER) work arrays + dynamic arrays | same | `END OF READING DATABASE STEP HEADER / A ARRAY REGION / MAR ARRAY REGION / DYNAMIC ARRAY`, `READ_INTEGER_DYN_ARRAY()`, `READ_REAL_DYN_ARRAY()`, `COPYING ADDRESSES FROM A AND MAR ARRAYS` |
| Step metadata | same | `StepNo MeshNo SimNo SimMode HSize MARSize ASize DimStep VerStep` |
| Fortran → C/C++ data bridge | DDS layer | `DDS_GET_CFTN_GATE`, `DDS_PUT_CFTN_GATE`, `DDS_GET_OBJDAT`, `DDS_PUT_OBJDAT` (fields OBJNO, NMATAXI, MOBJNA, OBJTYP2, NSTNCMP, NSYMSUF, OBJNAM, USRRTNO, NENAM, NNUSR) |
| User-routine DB API | DBE layer | `DBE_USR_InitDb`, `DBE_USR_SetStep`, `DBE_USR_GetData` (loaded user DLLs `USR_DEF_PST3.DLL`) |
| GUI model (new post / new pre) | C++ classes (MSVC RTTI) | `SfOpenDBCommand`, `SfPstModel`, `SfPstModelManager`, `SfPstStepView`, `SfItemStepCtrl`, `SfStepControlCommand`, `SfStateVariableData`, `SfDataExtraction`, `SfPreGenDb` |

## 2. Recommended target for static analysis (task 2)

1. **`DEF_PRE_64.exe`** (x64, 5.3 MB code, no Qt, build 2014-09-29). Smallest 64-bit binary
   that contains `datarm23.f` together with the full keyword reader/writer
   (`ERROR : ELMCON could not be read`, keyword table `PROBID RUNTYP DBREAD …`). Best
   single target for "complete" static coverage of what a step can contain.
2. **`3D\DEF_DBM.exe`** (x86, 650 KB code). Smallest binary with `datarm23.f`; contains the
   container layer only (no keyword names). Good for pinning down header/directory/block
   logic quickly, then matching the same functions in the 64-bit build.
3. **`DEF_GUI_PST_64.exe`** (x64, 19.5 MB code). The new post-processor, for how the GUI
   walks steps and which state variables it pulls (`SfPstModel`, DDS gate). Has a
   batch CLI: `DEF_GUI_PST.EXE -m header -d a.db -o header.txt` and
   `-m steplist -d a.db -o steplist.txt` (useful as a reference oracle when testing a
   re-created loader).
4. `DEF_GUI_MO_64.exe` (new pre with step browser) contains the same code as 3 plus MO
   project logic; not needed separately.

`DEF_GUI23.exe` (the "file browser") only lists DBs and launches the pre/post programs.

## 3. Protection status (read before any disassembly)

- **[fact]** Main `.text` sections of all DEFORM binaries have entropy 5.4–6.7 and normal
  import tables. Nothing is packed or encrypted; no Sentinel/HASP envelope.
- **[fact]** The x64 builds carry two extra small sections named `text` (≈6.5 KB) and
  `data` (≈16 KB, entropy 6.6). **[inferred]** these belong to the linked licensing
  library. Licensing is API-style C++ code compiled into each EXE
  (`.\network_license\LManagerClientForProduct.cpp`, `DnpCheckPassword…`, `CHKPAS`),
  and `datarm23.f` itself calls a license check while loading a step
  (`License Error(1)`, `Internal error on loading a step data`, `ERROR: Step limit exceeded!!`).
- Consequence: the DB-reading code can be read statically without touching the
  protection. The re-created loader simply omits those license calls; it does not need
  to emulate or bypass anything. During task 2 I will skip over license routines rather
  than analyse them.

## 4. What the DB file looks like (from the two example files)

All values big-endian. 512-byte blocks, 1-based block numbers.

| Offset | Content |
|---|---|
| 0x000 | signature `SFTC-DEF-3D-DB100-00` (both files) |
| 0x200 | title, 80 chars (`SIMULATION`, `GFM_V_DIES_02`) |
| 0x250 | program version `11.00`, build date `06/30/14`, created / modified dates `MM/DD/YY` |
| 0x270 | 5 × int32. Small file: `31 1 32 18001 1`; big file: `786 1 790 9796507 1`. **[inferred]** number of directory entries, ?, next free entry, last used block (≈ file size/512), ? |
| 0x400 | directory chain: `4, b1, b2, b3` (big file `4 853281 2512714 4185218`). **[inferred]** 128-entry directory chunks; first chunk at 0x600, later chunks at the listed blocks |
| 0x600 | step directory, 13 × int32 per step |

Directory entry examples (`valid, step, ?, ?, ?, ?, ?, dim, version, 0, blkA, blkB, blkC`):

```
(1, -1,  500, 234, 506, 1, 2, 3, 11000, 0,   17,   25,  135)
(1,  1,  500, 234, 506, 1, 1, 3, 11000, 0,  216,  224,  337)
(1, 116, 2900,  0,   0, 12, 4, 3, 11000, 0, 853235, 853281, 864788)
```

- **[fact]** Negative step numbers (`-1, -2, …`) are the restart/remesh copies DEFORM writes
  next to positive steps.
- **[inferred]** field 3 ≈ HSize, fields 4–5 non-zero only when a new mesh is stored,
  field 6 = mesh/operation number, field 8 = `DimStep` (3 = 3D), field 9 = `VerStep` (11000 = v11.0),
  last three = block addresses of step header, address table and data.
- Inside a step: real data are 8-byte big-endian IEEE doubles (`0x3FF0000000000000` = 1.0);
  character data are stored in 8-byte words with reversed byte order (`RETNE   ` = `   ENTER`),
  i.e. the writer swapped whole 8-byte words. Integers are 4-byte big-endian.
- At the second block of a step there is a table: count (58) then 4-int tuples
  `(id, length, start, …)`, e.g. `(31, 7780, 497, 7782)`, `(101, 2248, 16243, 2250)`.
  **[inferred]** the A/MAR address table that `COPYING ADDRESSES FROM A AND MAR ARRAYS` restores.
- 32-bit block numbers × 512 B limit a file to 1 TiB, so 400 GB targets fit; the big example
  (5.0 GB, 786 directory entries) already uses block numbers ≈ 9.8 M.

## 5. Keyword vocabulary found in binaries

`DEF_TEXT_PST.exe` holds a 134-entry keyword name table (8-byte slots) starting
`PROBID RUNTYP DBREAD KFREAD GENDB …` and ending `… DIEGEO FLDGEO FLWNET`. The GUIs also
carry descriptive tables for per-node/per-element variables, e.g. `RZ (Nodal coordinates)`,
`ELMCON (Element connectivity)`, `URZ`, `DRZ`, `NDTMP`, `STRAIN`, `EFEPS`, `EFSTS`, `STRESS`,
`STNCMP`, `DAMAGE`, `DENSTY`, `VOLFC`, `GRAIN`, `USRNOD`, `USRELM`, `BCC*`, `EC*`.
**[inferred]** keyword ids in the step address table index this vocabulary; the complete
id → name → layout map is what the static pass in task 2 must recover.

## 6. What the manuals say (no byte-level spec exists)

Paths below are relative to `C:\Program Files\SFTC\DEFORM\v11.0.2\MANUALS\HTML\`.

- The DB is "the primary data storage structure", stored "in a compressed, machine readable
  format" and readable only through DEFORM pre/post. Each step is appended at the file end;
  an unsaved step is overwritten by the next one (`1_Introduction_to_DEFORM\1.10._Basic_File_System.htm`).
- **Steps.** Negative step = input/restart step written by the pre-processor or by remeshing;
  positive = solution step. The same absolute number can exist twice (`25` and `-25`).
  Saved steps follow STPINC/STPDEF; MESHNO counts remeshes (`11_Keyword_Documentation\...\NSTART.htm`,
  `MESHNO.htm`, `STPINC.htm`, `STPDEF.htm`).
- **Step table** in the GUI shows `Operation | Step | Mesh# | Dim | Ver` (Ver 10000 in the manual's
  example, 11000 in our files), i.e. each step carries its own dimension; one DB can mix 2D and 3D
  steps and several operations (`2_Integrated_Pre_Processor\2.14._Database_Generation.htm`,
  `CURSIM.htm`). This matches directory-entry fields 6–9 in section 4.
- **Storage regions.** `DYNARY.htm` states some data "is not saved in A array or MAR array" but in the
  "dynamic array region in the database file", with ArrayType 1 = coordinates, 2 = connectivity
  (subtype 3 tri, 4 quad, 5 tet, 8 hex), 3 = state variable (100 damage, 200 density, 300 strain,
  500 stress, 600 temperature, 700 velocity). Confirms the A / MAR / dynamic split seen in the binaries.
- **Keywords = step records.** "Most of the data fields in the DEFORM pre-processor correspond directly
  to a DEFORM keyword" (`1.8._Creating_Input_data.htm`); FORCE/STROKE "are saved in the DB step header".
  The 2D keyword folder (266 pages) and 3D folder (84 pages) document layouts, e.g.
  `RZ obj` → `node, x, y, z`; `ELMCON obj, Nelem, Nnode` (4-node tet or 8-node brick);
  `STRESS` 6 components Sx, Sy, Sz, Txy, Tyz, Txz with `ELMNOD` choosing element / element+node /
  integration points (8 per brick, 1 per tet); `STNCMP` field width set by `STNOUT`;
  `DAMAGE` with nDim models; `USRELM`/`USRNOD` with Nvar user values; `VOLFC` with Nphase.
- **Size.** v11 release note: "v11 system will not have DB split due to size limitation. The DB can
  grow more than 2GB" (`1.11._Release_Notes.htm`). `DEF_SPLDB.EXE` is the legacy splitter.
- **Running DB.** While the solver runs, the DB is renamed `FOR003` (`3.3._Simulation_Graphics.htm`).
  A re-created loader should cope with a DB being appended to.
- **Documented text extraction.** `DEF_PRE.EXE` in text mode with `DBREAD <step>` + `KFWRIT <file>`
  writes any step as a KEY file (`9_Appendices\Appendix_VI__Running_DEFORM_in_text_mode.htm`). This
  is a ready-made test oracle: KEY text from DEFORM vs. records decoded by our loader.
- **In-memory layout reference.** The FEM user-routine sources show how DEFORM holds a step in memory:
  `3D\UserRoutine\DEF_SIM\usr_msh.f` (RZ(3,NUMNP), NOD(NONP,NUMEL), STS(6,NUMEL), TSRS, DAMG,
  USRVE/USRVN, global node numbering across objects, local element numbering) and
  `usr_upd.f` COMMON blocks. The post-processor routine `UserRoutine\PostProcessor\pstusr23.f`
  (`USRVAR`/`USRVAR2`) shows the per-element/node variable set the post exposes.
- Not documented anywhere: `DEF_DBM`, `DEF_TEXT_PST`, the compression scheme, record layouts.

## 7. Extra hints from strings in the utilities (inferred)

- Integer and real arrays are compressed: `TOTAL COMPRESSED INTEGER SIZE`, `TOTAL COMPRESSED SIZE`,
  `started swapping`, `started decoding`. The decoder is in `datarm23.f`, so static analysis of that
  module covers it.
- Fortran include names visible in `DEF_TEXT_PST.exe`: `DBIS`, `DBIS_EXT`, `DINDEX`, `HEAD`,
  `ADS1/ADS2/ADS3/ADSD`, `ADS1_SVHEADER`, `ADS2_SVHEADER`, `VERSION`, `MDBCOM`, `STEP`, `ARSZ`.
- `DATEXT3.exe` object menu lists the record types a text extractor supports: RZ, DRZ, URZ, FRZ,
  FRZCAL, PRZ, PRZCAL, NDTMP, NDHEAT, NDHEATC, NDFLUX, BDYNOD, BDYMTR, BCCANG, BCCDEF, BCCDFN, BCCTMP,
  BCCTFN, ELMCON, MTLGRP, DENSTY, STRAIN, STRAIN_CMP, STRESS, STRESS_CMP, STRATE, STRATE_CMP, DAMAGE,
  USER NODE, USER ELEMENT, VOTAGE, VOLFC, VOLFS, TICF, DVOLF, HDNOBJ, DATOM, GRAIN.
- `DEF_DBM.exe`: convert / merge / purge / compact (`DBCVT`, `DBMER`, `DBPUR`, `DBCMP`).

## 8. Next steps (task 2, after review)

1. Disassemble `DEF_PRE_64.exe` (and `DEF_DBM.exe` for cross-checking) with Ghidra, locate the
   `datarm23.f` routines via the strings above, and recover: header/directory, step header, the
   compression codec, A/MAR address table, dynamic arrays, keyword id → record layout for every id.
2. Write a format spec in this repo, then a C++20 reader: memory-mapped I/O, directory indexed once,
   steps decoded in parallel, no license code.
3. Validate against `DEF_PRE` `DBREAD`+`KFWRIT` output and `DEF_GUI_PST -m header/steplist` on both
   example DBs.
