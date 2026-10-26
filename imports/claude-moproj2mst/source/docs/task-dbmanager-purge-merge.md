# DB Manager > Step List: Purge and Merge in DEF_GUI23.exe

Scope: where the Purge and Merge functions of **Tool > DB Manager > Step List** live in
`C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI23.exe` (x86, image base 0x400000, build 2014-09-29)
and how they use the shared database module `datarm23.f` described in
[`db-format-spec.md`](db-format-spec.md). Convert is out of scope. Found by static analysis
(Ghidra 12.1.4); nothing in the install was modified and the licence-check code was not analysed.

Legend as in the spec: **[fact]** read in code, **[tested]** also confirmed on a copy of an example
DB, **[inferred]** interpretation. Addresses are `DEF_GUI23.exe` function entries. Ghidra did not
create most functions in this binary on its own; they appear only after running
`FillGaps.java 00401000 00640000` on the project (see "Reproducing" below).

## 1. Short answer

* Purge and Merge run **inside DEF_GUI23.exe**. No external program is started **[fact]**.
  The GUI does contain `datarm23.f` (rev 1.81, statically linked, together with `DBOperations23.f`,
  `mod_DBOperations3.f` and the DDS gate) and calls it directly.
* Both work the same way: load every chosen step of the source DB completely into memory with the
  C++ class `_DB_STEP_DATA`, then write it into the new DB with the `datarm23` "write step"
  operation (op code 4 of the dispatcher). Steps are re-encoded on write, not copied as raw
  records **[fact]**.
* A step that should not keep its results is written **header only**: MARSize = ASize = 0,
  no MAR/A data, step header kept. This is the same header-only form the spec describes in §4.2
  **[fact]**.
* The standalone `3D\DEF_DBM.exe` has its own purge/merge (`DBPUR`, `DBMER` in `def_dbm.f`)
  driven by `DEF_DBM.INI`. The GUI still writes a `DEF_DBM.INI` for Merge but never runs
  `DEF_DBM.exe` and deletes the file afterwards; it is a leftover **[fact]**.

## 2. Call map

```
UI_dlgStpSlctn  (Qt3 dialog "Step List", base ui_dlgstpslctnBase, UI built in FUN_004cbf36)
 ├─ FUN_004b2ef6  slotShowDBMangeClicked  toggles "&Database Purging >> / <<" panel
 ├─ FUN_004b26a9  fills the panel: title, version, release of the open DB;
 │                default names <db>_PUR.DB, <db>_MERGE.DB, <db>_CONVRT.DB
 └─ FUN_004b47ee  slotStartDatabasePMC  (sender = Start Purge / Start Merging / Start Convert)
     ├─ Purge: FUN_004b2b8a/004b2c06 = checked list items, FUN_004b2bad/004b2bd0 = unchecked
     │         items (step number = QListViewItem +0x30, FUN_004981e5)
     │    └─ FUN_004b32ff  PurgeDB(oldDb, newDb, nPurged, purged[], nKept, kept[])
     └─ Merge: checks both inputs exist, are regular files and carry a DB signature
          └─ FUN_004b3d56  MergeDB(db1, db2, newDb)

Shared C++ DB layer (same in both)
 _DB_INFO       FUN_0050d390 Open(path)      header + directory into memory
                FUN_0050b1e0 Refresh header
 _DB_STEP_DATA  FUN_0051af80 ctor(dim=3)     FUN_005167a0 dtor
                FUN_0051b640 LoadStep(i)  -> FUN_0051b150 read header, MAR/A, sim control,
                                             materials, objects, dynamic arrays (v >= 11000)
                FUN_00514500 WriteStep(newDbPath, headerOnly)
                FUN_00515370 FreeStep

datarm23.f (Fortran)
 FUN_00559395  dispatcher, same op codes as FUN_1403b5a04 in DEF_PRE_64 (spec §5):
               0 open, 1 directory, 2 step header, 3 MAR+A, 4 write step, 5 dynamic arrays,
               <= -30 / >= 30 single MAR / A region
 FUN_0055acc3  op 4, write step
```

Read side, all through the dispatcher **[fact]**: `FUN_0051b7c0` (op 0, header), `FUN_0051c759`
(op 1, directory, 10 ints per entry), `FUN_0051cfb2` (op 2, step header), `FUN_0051e643`,
`FUN_0051ecea`, `FUN_0051f43b`, `FUN_0051fb8a` (op 3 and single-region reads),
`FUN_0051ac60` → `FUN_0055e5b8` / `FUN_0055eb98` / `FUN_0055ee38` (dynamic arrays).

### 2.1 In-memory directory (`_DB_INFO`)

`FUN_0050d390` keeps the directory as an array of 0x28-byte rows at `_DB_INFO+0x940`, count at
`+0x934` (= NRSETS), number of steps with arrays at `+0x938` (= NUMRST) **[fact]**:

| Offset | Directory word (spec §4.2) |
|---|---|
| +0x00 | 1 MeshNo |
| +0x04 | 2 StepNo |
| +0x08 | 3 HSize |
| +0x0c | 4 MARSize |
| +0x10 | 5 ASize |
| +0x14, +0x18 | 6 SimNo, 7 SimMode (3D < 3000: SimNo forced to 1) |
| +0x1c, +0x20 | 8 DimStep, 9 VerStep (taken from the file only when the DB version ≥ 10000, else the global dim/version) |
| +0x24 | 1 if MARSize ≥ 1, else 0 ("has arrays") |

Other `_DB_INFO` fields: title `+0x8bc` (80 chars), SMI2 release date `+0x90d`, SMI3/SMI4 dates
`+0x915`/`+0x91d`, version number `+0x928` (parsed from SMI1), NXTREC `+0x93c`, path `+0x4bc`.

## 3. Purge (`FUN_004b32ff`)

In the dialog the user ticks the steps to **keep**. The handler refuses to start if nothing or
everything is ticked **[fact]**. Then:

1. New name must differ from the old one and be non-empty. If the new file exists the user is asked
   to overwrite it, and it is deleted (`FUN_00506290`) **[fact]**.
2. If any unticked step is negative, the "NEGATIVE STEPS WILL BE PURGED" warning is shown
   (point tracking, flow net, user-variable tracking may stop working); "No" aborts **[fact]**.
3. The working directory is set to the folder of the old DB, a progress dialog opens, and one
   `_DB_STEP_DATA` reader is bound to the DB currently open in the dialog (`this+0x1f8`).
4. For every directory entry *i* of the old DB, in directory order **[fact]**:
   * if |kept[0]| ≤ |step_i| ≤ |kept[last]| (`FUN_005c70ca` = abs; first and last ticked item in
     list order), the step is copied: `LoadStep(i)`, `WriteStep(new, headerOnly)`, `FreeStep()`,
     with **headerOnly = (entry has no arrays) or (step_i is unticked)**;
   * otherwise the entry is dropped and does not appear in the new DB at all.
5. "Stop" in the progress dialog ends the loop (return 3, "Aborted" message); the partial new DB
   stays on disk **[fact]**.

So a purged DB keeps a header-only entry for every unticked step that lies between the first and
the last kept step, and loses everything before the first and after the last kept step. Steps keep
their original numbers, mesh numbers and simulation numbers **[fact]**.

The standalone `DEF_DBM.exe` purge (`FUN_00405b68`, mode 3 of `DEF_DBM.INI`: mode / old name /
new name / count / list) differs: it keeps **all** entries and makes only the listed steps
header-only (`FUN_0040696f`) **[fact]**.

## 4. Merge (`FUN_004b3d56`)

1. Writes `DEF_DBM.INI` (`2`, db1, db2, new) in the folder of db1. If the new DB exists the user is
   asked to overwrite; it is then deleted **[fact]**.
2. Opens both inputs as `_DB_INFO` (`FUN_0050d390`) with one `_DB_STEP_DATA` each **[fact]**.
3. Decides the order from the step numbers **[fact]**:
   * if firstStep(db2) = −(lastStep(db1) + 1): copy all entries of db1, then all entries of db2;
   * else if firstStep(db1) = −(lastStep(db2) + 1): copy db2 first, then db1;
   * else: "Error occurred during database merging. Please check DB_MERGE.OUT ..." and nothing
     is written.
   The second DB must therefore start with the restart step the pre-processor writes when a run
   is continued from the last step of the first one (spec §3: the pre-processor stores its steps as
   −(NSTART+1)) **[inferred]** for the meaning, **[fact]** for the test.
4. Every entry is copied with `LoadStep` / `WriteStep(new, headerOnly = entry has no arrays)` /
   `FreeStep`, i.e. header-only entries stay header-only and nothing is dropped **[fact]**.
5. Afterwards `DB_MERGE.OUT` (if present) and `DEF_DBM.INI` are deleted **[fact]**.

Unlike `DEF_DBM.exe` (`FUN_0040a18c`: "Merge not possible with different database versions"),
the GUI merge does not compare database versions; each step is written with its own dim/version
**[fact]**.

## 5. Writing one step (`_DB_STEP_DATA::WriteStep`, `FUN_00514500`)

1. If the target file exists, open it as `_DB_INFO` and find the slot for this step
   (`FUN_0050be60`): the first entry with |step| ≥ |s|; if that entry has the same |step|, is
   negative and s > 0, take the next one. Otherwise (new file) slot = 1 **[fact]**.
2. Open the target on Fortran unit 3, direct access, RECL 512 (`FUN_0053aea1`, status OLD for an
   existing file, NEW otherwise) **[fact]**.
3. Prepare the DB header (`FUN_0056bdd0` → `FUN_00557c45`, `FUN_00557cad`) **[fact]**:
   TITLE from the source step, SMI1 = version `%5.2f`, SMI2 = release date held by the step object,
   SMI3 kept unless empty, SMI4 = today (`%02d/%02d/%02d`), ISTART = slot being written,
   ISMTYP = 1 for 3D, NRSETS = current entry count of the target (0 for a new file), IPTYPE = 1.
4. Call the dispatcher with op 4: `(unit 3, 4, &slot, &stepNo, &HSize, header, &MARSize, MAR,
   &ASize, A, &ierr)`. For headerOnly the MAR/A sizes and pointers are passed as 0 **[fact]**.
5. Close the unit (`FUN_0053ae5c`).

### 5.1 What op 4 (`FUN_0055acc3`) writes **[fact]**

* **Placement.** slot = 1: first data record 17, first directory chunk at record 4 (index record 3,
  word 1). slot > NRSETS: append at NXTREC; every 128th entry allocates a new 13-record directory
  chunk at NXTREC, and every 127 chunks a new index record (limit 200 index records, else
  "ERROR: Step limit exceeded!!"). slot ≤ NRSETS: reuse that entry's header record and set
  NRSETS = slot, i.e. **all later entries are discarded**. This is how DEFORM overwrites steps;
  in purge and merge the steps arrive in ascending |step| order, so every write appends.
* **Signature.** On a new file record 1 gets `SFTC-DEF-3D-DB100-00` (3D step) or
  `SFTC-DEF-2D-DB100-00` (2D step). Writing into an old family-2/3 file first rewrites all its
  directory entries with dim/version and the new signature, turning it into a family-23 DB
  (`FUN_005580a6`).
* **Directory entry.** MeshNo, StepNo, HSize, MARSize, ASize, SimNo, SimMode, DimStep, VerStep and
  the header / MAR / A records (words 11–13). Word 13 is only set when ASize > 0.
* **Step header.** H6 = 2 (platform code), H7 = MARSize, H8 = ASize written into the header before
  it is stored (2D: H7/H8 = sizes, H9 = 2), so a GUI header-only step has H7 = H8 = 0
  **[fact]**, not tested (see §6). Header written as 2·HSize ints, never compressed.
* **MAR / A.** RLE-encoded when version > 999 (3D) / > 3109 (2D) (`FUN_00503c9d`, `FUN_005040a6`),
  byte-swapped to big-endian (`FUN_004fdcec` int, `FUN_004fdfa5` real; swap mode is always 2 in this
  build), block-written (`FUN_00504523`). 3D steps of version ≥ 6000 go region by region with a
  region table (`FUN_0055d103` MAR, `FUN_0055df53` A), which is the layout of spec §5.2.
* **Dynamic arrays** (`FUN_005600ec`) when the step has any. 3D steps with A data also get a
  5-word (20-character) text record at NXTREC (`FUN_00504a4c`); its content was not traced.
* **Header and index.** Record 2 (34 words) is rewritten, then every index record, then the
  directory chunk of the slot (`FUN_0055c4b0`, op 2 inside).

Licence checks inside this path (not analysed, a re-creation leaves them out): the dispatcher entry
("License Error(1)"), op 4 before the directory flush ("License Error(2)"), and `FUN_0051b150`
before a step is loaded.

## 6. Tests

* **[tested]** On a copy of `COG_P1_B1.DB` the solver's own header-only entries (slots 8–14,
  19–29) have MARSize = ASize = 0 in the directory but **non-zero** H7/H8 in the step header
  (e.g. slot 8: H7 = 142473, H8 = 529799). The solver therefore does not go through the same
  header-only path as the GUI, whose WriteStep passes zero sizes (§5.1). A reader must take
  "has arrays" from the directory (as the GUI does, `_DB_INFO+0x24`), not from H7/H8.
* **Not usable as an oracle:** `3D\DEF_DBM.exe` in mode 3 on a copy of `COG_P1_B1.DB`
  (purge list `2 12`) stopped after the first entry with
  `DBPUR??? Error in reading Real & Integer Arrays` and left a 10 KB file. That legacy tool does
  not handle these v11 steps, which fits the GUI doing the work in-process.
* **[tested]** The `deformdb` writer (§7) reproduces DEFORM's files. Rewriting every entry of a
  copy of `COG_P1_B1.DB` gives the same record layout and NXTREC; 181 of 9 216 000 bytes differ,
  all in ISTART, word 13 of header-only entries and H7/H8 of header-only step headers. On a copy
  of the 5 GB Inc718 file, 682 of 9.8 M records differ, every one of them a header-only step
  header, a directory chunk or record 2.
* **[tested]** DEF_PRE (`DBREAD` + `KFWRIT`) exports byte-identical KEY files from purged or
  merged copies and from the original, for COG steps 12, 25, −13 and Inc718 step 690. For a
  header-only step DEF_PRE falls back to the nearest earlier step with arrays, so in a purged
  file that is the nearest *kept* step.
* **[tested]** Merging the two halves of a purged COG file (steps |1..12| and |13..25|, given in
  reverse order) produces exactly the bytes of the full copy.
* **Not yet tested:** running the GUI's own Purge and Merge on copies and comparing their output
  with `deformdb`'s (needs driving DEF_GUI23 on screen). `DEF_GUI_PST_64 -m steplist` crashed on
  the original COG copy as well, so it was not usable as a second oracle.

## 7. Re-creation in `deformdb`

`deformdb/include/deformdb/writer.hpp` (`Writer`) re-creates op 4 for appending entries, and
`manager.hpp` (`purge`, `merge`) the two loops of §3 and §4; `dbmanager.exe` is the command line.
The writer follows the 64-bit copy of op 4 in `DEF_PRE_64.exe` (`FUN_1403b7729`), which matches
`FUN_0055acc3` in the GUI **[fact]**:

* **Region arrays** (`FUN_1403ba54c` MAR, `FUN_1403bb996` A): the region table is written first
  as a placeholder, then each region with length > 0 is RLE-encoded and written on its own
  records (stored size → word 4r+1, record → word 4r+2); a region of length 0 gets stored = 0
  and record = the current NXTREC. The table is then rewritten in place. DEFORM hard-codes the
  table lengths 234 (MAR) and 506 (A); `deformdb` keeps the source table's length, which is the
  same for v11 steps.
* **Dynamic arrays** (`FUN_1403be3d4`, only when ASize > 0): descriptor (20 ints), integer table,
  real table as placeholders, then every non-empty array RLE-encoded (`FUN_1403bec29`,
  `FUN_1403bed68`), then both tables and the descriptor again.
* The 5-word text record of §5.1 is only written when the swap mode is 3; this build always uses
  mode 2, so it is never written. H6 (3D) / H9 (2D) is that swap mode, 2.
* **Solver header-only steps.** With slot ≤ 0 ("append as the solver does"), op 4 overwrites the
  last entry if it has the same step number; otherwise, unless the previous step is a save step
  (`(step − 1) mod STPINC = 0`) or the pre-processor's start entry, it sets the previous entry's
  MARSize = ASize = 0 and starts the new step at that entry's MAR record. That is why the
  solver's header-only entries keep non-zero H7/H8 and stale MAR/A records (§6). The GUI path
  always passes an explicit slot and never uses this mode.

Differences from DEF_GUI23, all deliberate: header-only entries get word 13 (A record) and
word 10 = 0 instead of a stale value; merge takes the title of the earlier database (the GUI
ends with the title of the last step it copied); the API accepts a keep list that covers every
step (the GUI refuses that) and does not ask before overwriting unless `overwrite` is false.

## Reproducing

Ghidra project `C:\Users\alext\Projects\deform-rea-work\ghidra_gui` (project name `gui`).
Scripts in `deform-rea-work\scripts`: `run2.sh` (`GPROJ=ghidra_gui GNAME=gui`), `FillGaps.java`,
`FindImm.java` (finds code that pushes a string address), `FuncRefs.java`, `Listing.java`,
`Decomp.java`. Decompiled output: `deform-rea-work\out\gui_dbm*.c`, DEF_DBM side
`out\dbm_purmer.c`.
