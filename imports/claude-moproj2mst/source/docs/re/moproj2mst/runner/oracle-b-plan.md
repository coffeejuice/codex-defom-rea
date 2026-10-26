# Oracle B plan: sample projects to save from DEF_GUI_MO_64

Status: **plan only. Nothing here has been run.** Oracle B means the original GUI saves reference
MSTs, and Aleksandr has not approved that yet (plan §1). This file says what to build if he
approves, and how a thread drives the GUI on Sim2 safely.

Why: `coverage.md` shows that 15 operation classes have no oracle-A case. The specs (s1-02..s1-05)
give their writers; this plan gives, per class, the smallest project whose saved MST exercises
those writers' main branches.

## 1. Rules for every oracle-B case

* Each case is a **new project created in a scratch folder** under
  `C:\Users\alext\Projects\deform-rea-work\gui_mo\oracle_b\<case>\`. Inputs (geometry, materials,
  equipment KEYs) are copied from the corpus copies (`gui_mo\s1\corpus\...`). Never open a project in
  `C:\Program Files\SFTC\...` or `C:\PROBLEM\...`, and never save there.
* Only **File > Save**. Never start a simulation, the DEF_PRE run of an operation, or a DOE run.
  Save itself writes side files into the scratch project (Boolean.BWZ, DEF_MACHDIST_CTRL2.INI,
  HT_Proc<k>.MOKEY, DOE Control files). It also deletes `DEF_CNV23.DAT`, `DEF_REM.INI` and
  `REMESH.SCR` in the current directory for Convert2Dto3D. That is expected, and the scratch
  folder contains all of it.
* After Save, the case goes into the corpus through `build_corpus.py`, as for oracle A, with
  `oracle` = `B` in `cases.tsv` and the case's build notes in `oracle_b/<case>/steps.md`.
* One case = one project = a few operations, so a DIFF points at one writer. Each case lists the
  branches it is meant to hit; the thread checks the saved MST for the listed lines before
  accepting it (e.g. a `NEXTSIMNO` line, a `* Flip` block).
* Build both unit systems only where a writer branches on the unit (MultiBlow and
  FormingExpress: `+0x70` = 100 for SI, 5 for English). Elsewhere SI only.

## 2. Cases per class

Priority: A = many writer branches or shared machinery (next simulation number, DOE), B = one
writer with a few branches, C = writes nothing or nearly nothing (cheap confirmation).

### P14 MultiBlow (s1-04 §7), priority A

| Case | Dim | Build | Branches hit |
|---|---|---|---|
| B-MB2-SI | 2D | Workpiece (obj 1), top die (2) with an equipment record, bottom die (3). MultiBlow2D with 2 passes × 2 blows. Blow (1,2) with rotate/flip on. NDTMP and STRAIN reset on. Heating on with TI, HT and TO times > 0 and OTPRNG on. Dwell on with dwell time > 0. DOESCL in percent off. Then one ordinary Forming2D operation. | BL (j=1 and j>1, `* Flip`, `* Positioning`, NDTMP/STRAIN, IMPEQP + DOESCL/MOVCTL "1"/"2"), DW (MOVCTL with ±angle, OTPRNG + NEXTSIMNO, NSTEP rounding), TI/HT/TO (PDIE, OTPRNG 0/2, HT strain), NEXTSIMNO to the next operation |
| B-MB2-EN | 2D | Same as B-MB2-SI in English units, with DOESCL in percent **on** and "skip second DOESCL" on | `+0x70 = 5`, `d1/100.0`, MOVCTL without "1", no 2nd DOESCL |
| B-MB3-SI | 3D | Same objects in 3D. Blow (1,2) rotation with self-center **off** and a non-axis direction. Blow (2,2) rotation with self-center on and an axis along -Y. One blow with rotation angle 0 (error text). | `* Rotate` (OBJPOS mode 3 and 6), `* Error : Rotation angle is zero`, 3D positioning, 3D MOVCTL in DW |

### P15 FormingExpress (s1-05 §5), priority A

| Case | Dim | Build | Branches hit |
|---|---|---|---|
| B-FE3-SI | 3D | FormingExpress with express on (data type 3 or 4). 2 blows × 3 steps. Step (1,2) gap > 0 (DW block). Step (1,3) drop on, with TO distance > 0 and strain reset on. Top die with equipment. DT mode 0. Then one Forming operation. | BL (`Put Top die on Workpiece`, `Move Top die back`, `Move Bottom die back`), IMPEQP/DOESCL/MOVCTL, DSMAX + DTTYPE, DW, TO (`b90` set and not set), SIMNAM `-BL<b>/-DW<b>/-TO<b>`, NEXTSIMNO |
| B-FE2-EN | 2D | Same in 2D, English units, DT mode 1, one step with flip on | `* Flip`, `Put the flipped Workpiece`, DTMAX `dt 0`, `+0x70 = 5`, ±angle MOVCTL |
| B-FE3-FIRST | 3D | FormingExpress as the **first** operation, with "start from KEY" (restart) on | first-block branch: `140134510`, GENDB mode 0 |
| B-FE3-OFF | 3D | FormingExpress with express **off** | the base writer (DIM/NSTART, s1-02 §5) |

### P16 HeatTreat, Diestress, DiestressBM, Report, ShapeOptim (s1-05 §1–4)

| Case | Prio | Dim | Build | Branches hit |
|---|---|---|---|---|
| B-HT3 | A | 3D | Forming, then HeatTreat with 3 steps and first step index 2. One step has a user keyword file (a copied KEY), one has "None". | skipped numbering, `* Operation No`, emitters with the loop index, `* Keyword file which ECC data`, KFREAD HT_Proc<k>.MOKEY and the user KEY |
| B-HT2-FIRST | B | 2D | HeatTreat2D as the **first** operation, 2 steps, first step 1 | no DBREAD on step 1, all emitters on step 0 |
| B-DS | B | 3D | Forming, then Diestress. Project DB named with upper-case `.DB`. | SIMULATION + `KFREAD` of the `.KEY` + GENDB on the sub-project DB path (s1-05 Q1) |
| B-DS-LC | C | 3D | As B-DS with a lower-case `.db` primary DB name | case-insensitive `.DB`→`.KEY` replace |
| B-BM2 | B | 2D | Forming2D (workpiece type 1, two dies), then DiestressBM2D. One object with interpolation on and `nRefNo != nObjNo`. | DEFFRZ, OBJTYP of die objects (types 1/5), SIMNAM `Die Stress Simulation`, GENDB on the sub-project DB |
| B-BM3-NEWDB | B | 3D | As B-BM2 in 3D with "use new DB" on, followed by one more operation | `dir + "/" + newDb`, `*sim = 1` (the next operation's numbering restarts) |
| B-RPT | C | 3D | Forming, Report, ShapeOptim, Forming | no lines, and the next operation's SIMULATION/CURSIM do not shift |

### P13 Distortion, Group, Cycle (s1-04 §2, §3, §6)

| Case | Prio | Dim | Build | Branches hit |
|---|---|---|---|---|
| B-DIS3 | A | 3D | Forming, then Distortion3D with 2 passes, first pass written 0, each pass with an object that has a KEY, then one Forming | `* Operation No ... Original workpiece / Pass No i / Unload`, NSTEP 0 on pass 0, KFREAD of the pass object, `Step<i>.KEY`, `DEF_MACHDIST_CTRL2.INI_<i>`, `DEF_MACHDIST_OUT<i>.KEY`, SIMNAM `<name>PS01`/`LOAD`/`UNLOAD`, NEXTSIMNO counting (O-8) |
| B-DIS2-SKIP | B | 2D | Distortion2D with 3 passes and first pass written = 2 | skipped passes (counter and NEXTSIMNO differ, O-8) |
| B-GRP | B | 2D | Group of two Forming2D operations, followed by one HeatTransfer2D | MST order with groups collapsed (s1-01 §8.3, O-4), Group writes nothing |
| B-CYC-NEST | B | 2D | Cycle (count 3) around a Forming2D and a Distortion2D | `CYCL<k>-` SIMNAM with two members, stale `curItem` (O-3) for Distortion's SIMNAM |

### P12 Boolean3D, Convert2Dto3D, CopyMirror (s1-04 §1, §4, §5)

| Case | Prio | Dim | Build | Branches hit |
|---|---|---|---|---|
| B-BOOL3 | B | 3D | Forming, then Boolean3D with workpiece object 2 and cutter 3 | 3D Boolean (OBJPOS 3D if any, INICTC on object 2); the INI's 3D BCC block is a side file (O-7) |
| B-CNV | B | 2D→3D | Forming2D, then Convert2Dto3D | DBREAD branch, `KFREAD` M23_INPUT.KEY / TEMP_STEP.KEY relative paths, GENDB flag |
| B-CNV-FIRST | C | 2D→3D | Convert2Dto3D as the first operation (start from KEY) | `140134bf0` SimCtrl branch |
| B-CM | B | 3D | Forming, then CopyMirror, then Forming | `140134bf0(…, 1, …)`, `EXECMD 3`, `mirrormerge.ini`, `ARMCTL\t0\t0\t<sim+1>` |

### P10 Pre and HTFurnace (s1-02 §5, base writer with +0x200 = 0), priority C

| Case | Dim | Build | Branches hit |
|---|---|---|---|
| B-PRE | 3D and 2D | Pre3D followed by Forming. Second project: Pre2D followed by Forming2D. | base writer without DIM/NSTART on the first operation |
| B-HTF | 3D | HTFurnace as the first operation, then Forming | as B-PRE for HTFurnace |

### P17 DOE (s1-05 §7, `M2M_FLAG_DOE_TEMP`), priority A if DOE is wanted

| Case | Build | Checked file |
|---|---|---|
| B-DOE2 | Forming project with one DOE study (type 2). (a) A keyword variable on two operations with 2-line before/after text. (b) A variable linked to (a). (c) A geometry variable on a die with `.GEO` files. | `DoeStudies/<study>/Control/<base>.MST.TEMP` (`*!SUBPRJ`, DOEVAR, MORP, `OutputGeoOp…GEO`, GFREAD/DEFAMG/GENBCC) |
| B-DOE3 | As B-DOE2 with an optimisation study (type 3) | same |

The main MST of a DOE project still needs to match. This also covers DOE geometry and variable
lines inside base-writer blocks (s1-02 §6.5).

Minimum useful set, if GUI time is short: B-MB2-SI, B-FE3-SI, B-HT3, B-DIS3, B-BM2, B-CM, B-CNV,
B-GRP, B-RPT (9 projects).

## 3. Driving the GUI on Sim2 (if approved)

1. **Approval and access.** Aleksandr approves oracle B in his own words. The thread then asks for
   computer-use access to DEF_GUI_MO_64 only. Licence, Sentinel or process-monitor dialogs are
   never touched: if one appears, the thread stops and asks him.
2. **Scratch set-up.** The thread creates `gui_mo\oracle_b\<case>\` and copies the input KEYs from
   the corpus copies. Before starting, it hashes `C:\Program Files\SFTC\DEFORM\v11.0.2`, the
   corpus folder and `C:\PROBLEM` (path, size, mtime, SHA-256) so it can prove after the session
   that only the scratch folder changed.
3. **Launch.** A small Python launcher (`python -I`, stdlib only):
   * calls `ctypes.windll.kernel32.SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX |
     SEM_NOOPENFILEERRORBOX)` first, which the child inherits;
   * starts the **installed** exe with `subprocess.Popen([r"C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe"], cwd=<scratch case dir>, shell=False)`,
     with explicit argv. It uses no `os.startfile`, ShellExecute, `start`, `cmd /c` or file
     association. The exe is never copied or modified (the analysis copy in `gui_mo\` is not
     run).
   * One GUI instance at a time, and no Ghidra run in parallel (shared CPU).
4. **Build.** The thread creates the project with File > New in the scratch folder, adds objects
   from the copied KEYs, and adds the operations and properties of the case through the
   operation wizards. Each step goes into `steps.md`. Screenshots stay in the scratch folder.
5. **Save and close.** File > Save, then close the GUI normally (File > Exit). It never kills the
   process while it writes.
6. **Check.** The thread re-hashes the three trees from step 2: only the scratch folder may differ.
   The GUI's own user settings (recent-file list in the registry or AppData) are the one expected
   outside change, and the thread lists them in the case notes. It then checks the MST for the
   case's branch lines (section 2), runs `build_corpus.py` on the case into the corpus, adds the
   `cases.tsv` row with `oracle = B`, and runs `run_corpus.py --self-test`.
7. **Commit** `cases.tsv` and the notes on `claude/m2m-corpus`. The project copies stay in the
   corpus folder, which is outside the repo.
