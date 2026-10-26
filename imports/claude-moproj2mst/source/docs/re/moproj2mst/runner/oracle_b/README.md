# Oracle B: MSTs saved by the original DEF_GUI_MO_64

Approved by Aleksandr on 2026-10-22. Batch 1 was run the same day on Sim2.

## Where things are

| What | Path |
|---|---|
| Oracle-B corpus (cases, `cases.tsv`, `skipped.tsv`, `manifest.sha256`) | `deform-rea-work/gui_mo/s1/corpus_b/` |
| Repo copy of its `cases.tsv` | `runner/cases_b.tsv` |
| Scratch projects as saved (case folder = case id) | `deform-rea-work/gui_mo/oracle_b/<case_id>/` |
| Scripts (this folder) | `oracle_b/*.py`, case list `final.json`, corpus list `list1.tsv` |

The oracle-B cases are a separate corpus. The oracle-A corpus (`gui_mo/s1/corpus`) is unchanged,
because its tier-1 scans (P01 blob/walk/xml, P03 order/subproject, P08 corpus_lines) assume
oracle-A facts: fixed file counts, an OPRNAM in every simulation block, consecutive SIMULATION
numbers. HeatTreat, Diestress, Report and ShapeOptim break those, so mixing would fail them.

Run: `python -I run_corpus.py --cli <moproj2mst.exe> --corpus <gui_mo/s1/corpus_b>`.

## How a case is made

1. `gen_specs.py` writes `final.json`. `mk.py final.json` copies an oracle-A corpus case (57
   Gear_blank for 3D, 51 MO2_Operation_Cycles2 for 2D, both SI) into `oracle_b/<case_id>/`,
   renames its MST to `.MST.oracleA`, appends the new operation(s) to `<Operation_List>` with an
   empty `<Properties Number="0"/>` (the GUI fills its default property tree on open), and chains a
   new `<DiagramItem>` after the last one. Wizard side files are written into the new operation's
   task folder before opening: `HeatTreatWizard.HTWZ` (3 steps, first written step 2 in 3D and 1
   in 2D, step 1 with a user keyword file) and `MachDistortion.MDWZ` (2 passes, objects 2 and 3).
2. `launch.py` starts the installed exe with `SetErrorMode(FAILCRITICALERRORS | NOGPFAULTERRORBOX
   | NOOPENFILEERRORBOX)`, `shell=False`, explicit argv, cwd outside the case folders. The exe
   ignores a project path on its command line.
3. `run.py <case>...` starts a fresh GUI process per case (as in step 2) and closes it after the
   save; `run.py --same-process <case>...` is the old mode of batches 1 and 2, for re-saves only,
   because the GUI keeps session state between opens (see "Fresh-process reruns" below). It
   drives the GUI with posted window messages (no desktop tool):
   File > Open (menu by Down/Enter keys, path typed into the native file dialog), File > Save,
   File > Close answering "No" to "save changes". It stops on any unexpected dialog and keeps a
   screenshot. Save writes the `.moproj` and `.MST` together (mtime difference below 5 ms).
4. `snap.py` before and after (path, size, mtime; SHA-256 for DEFORM exe/dll/ini/dat): the only
   changes outside `oracle_b/` were the licence manager's own logs
   (`C:/Program Files/SFTC/License Manager/log/...`). The GUI also updates its recent-projects
   list (user settings).
5. `build_corpus.py list1.tsv <corpus_b> --append` copies each saved case and adds its row:
   `oracle` = B, `saved_dir`, `provenance` (exe path, PE timestamp 2014-12-04 09:38:12 UTC,
   SHA-256 prefix fd8ae1f8dd50, date, procedure, base case, appended types).

## Checks on the saved MSTs

* Every case's MST starts with its base case's oracle-A MST byte for byte (the appended
  operations only add lines), so a GUI re-save reproduces cases 51 and 57.
* A re-save of case 52 (BEND_2D, a 2013 save) differs from its MST in two places: L27
  `*!SIMTYP` (the `legacy-express-simtyp` waiver) and a new `GENCTC 0` line after L46 in the
  AirConvect2D block (the GUI adds `GENCTC 0` to the operation's ActionKeywords on open). The
  waiver covers only the first, so case 52 can still DIFF at L47 after a correct conversion.
* GENCTC re-saves (evidence only, not corpus cases): unchanged copies of 11, 19, 46, 49 and 52
  re-saved in `oracle_b/R11`, `R19`, `R46`, `R49`, `82_R52_BEND_2D`. On open the GUI inserts
  `GENCTC 0` after INICTC in any operation whose ActionKeywords lack it; a second open+save is
  byte-identical. Waiver `unopened-genctc` (P09) applied to the original MSTs gives these re-saves
  byte for byte (checked). Re-saves of 47 and 50 (`83_R47_...`, `84_R50_...`) equal the originals.
* Report and ShapeOptim must be the last operation (Save warns otherwise); each writes nothing.
* HeatTreat and Distortion with default data write nothing; their steps/passes come from the
  HTWZ/MDWZ files.
* CopyMirror with default properties (57 + CopyMirror + Forming) crashed the GUI on Save. No
  case yet.

## Batch 1 (19 cases, `--corpus corpus_b`)

| Case | Appended | MST lines added |
|---|---|---|
| 59_B_HT3 | HeatTreat (3 steps, first 2) | 49 |
| 60_B_HT2 | HeatTreat2D (3 steps, first 1, user KEY) | 66 |
| 61_B_DS3 | Diestress | 9 (waiver `project-dir`) |
| 62_B_BM2 / 63_B_BM3 | DiestressBM2D / 3D | 46 / 38 |
| 64_B_RPT / 65_B_SOPT | Report / ShapeOptim | 0 |
| 66_B_DIS3 / 67_B_DIS2 | Distortion3D / 2D (2 passes) | 136 / 144 |
| 68_B_BOOL3 | Boolean3D | 38 |
| 69_B_CNV | Convert2Dto3D (2D base) | 27 |
| 70_B_PRE3 / 71_B_PRE2 | Pre3D / Pre2D | 43 / 51 |
| 72_B_HTF | HTFurnace | 35 |
| 73_B_HTX3 | HeatTransferExpress (3D) | 35 |
| 74_B_MB2 / 75_B_MB3 | MultiBlow2D / 3D | 87 / 79 |
| 76_B_FE2 / 77_B_FE3 | FormingExpress2D / 3D | 51 / 43 |

First run with the converter built from `claude/m2m-corpus` 6a304a3f: 17 PASS, 2 DIFF.
74_B_MB2 L529 expects `OBJPOS \t2\t1\t0\t100`, the converter writes an extra `\t0`.
75_B_MB3 L110: the converter writes no MultiBlow3D block at all.

## Batch 2 (12 cases, non-default data)

`mk2.py` makes them: `mb`/`fe` copy a GUI-saved batch-1 case and rewrite its MultiBlow /
FormingExpress property tree (`mbprops.py`: heats, blows, flags), `first` turns the first operation
of a corpus case into another class (keeping its objects, ActionKeywords and NSTART). English-unit
cases are `mk.py final2.json` appends on corpus 52 and 58.

| Case | What | Converter (6a304a3f build) |
|---|---|---|
| 78_B_MB2X | MultiBlow2D 2x2 blows, rotation/flip on, heating with reheat, strain reset (TI/HT/TO blocks) | DIFF L512 NEXTSIMNO sim 25 vs 15 |
| 79_B_MB3X | MultiBlow3D 2x2, NDTMP, rotations, `* Rotate`, `* Error : Rotation angle is zero` | DIFF (no MultiBlow3D block) |
| 82_B_FE3X | FormingExpress, Equipment 3, blow table on (express) 2x3, gap, drop | DIFF L154 |
| 83_B_FE2X | FormingExpress2D, Equipment 4, express 2x2, flip, drop | DIFF L516 |
| 84_B_HT2F | HeatTreat2D as the first operation, 3 steps from step 1, user KEY | PASS |
| 85_B_CNVF / 86_B_PRE3F / 87_B_HTFF | Convert2Dto3D / Pre3D / HTFurnace first | PASS |
| 88_B_FE3F | FormingExpress (express off) first | DIFF L29 `DIM 3` vs `DIM 0` |
| 89_B_MB2EN | MultiBlow2D defaults, English units | DIFF L165 OBJPOS extra `\t0` |
| 90_B_FE2EN / 91_B_FE3EN | FormingExpress2D / 3D defaults, English units | PASS |

Notes: FormingExpress writes its express blocks only with Equipment 3 or 4 and `UseBlowTable` on.
MultiBlow heating needs `InitTemp` off (it blocks heating) and per-blow `BlowReheat`; rotation and
flip need `UseWorkpieceRotation`. In 78 the flip on blow (1,2) printed no `* Flip` block and in 79
only one of the two rotations printed `* Rotate`; that is what the GUI wrote.

CopyMirror and Group: unsupported in v11. The class registry 0x14006dba0 registers 28 classes and
neither of these (P01, plan section 8); opening a project with either crashes the GUI inside
ParseDiagramItem 0x140091f00 (access violation at RVA 0x92a1f for CopyMirror, 0x92c20 for Group,
Application event log, 5 runs).

## Fresh-process reruns (75F, 79F, 92)

Batches 1 and 2 ran every case in one GUI process. The MultiBlow root property template
(0x143d9ef10, P14) is process-wide, so 74_B_MB2 (opened first) seeded 75 and 79: their saved
moproj name the 3D operation's root group `MultiBlowForging2D`. `run.py` now starts one GUI per
case. All three saved, GUI exit code 0. `mbcheck.py <case dir>...` prints the facts below.

| Case | moproj root | UseRelativeEnergy | NumHeats | MST DOESCL / MOVCTL (first sim) | MST |
|---|---|---|---|---|---|
| 75_B_MB3 (same process) | MultiBlowForging2D | false | 1 | `DOESCL 100` / `MOVCTL 2 1` | 193 lines |
| 75F_B_MB3 (fresh) | MultiBlowForging3D | false | 1 | `DOESCL 100` / `MOVCTL 2 1` | = 75 byte for byte |
| 79_B_MB3X (same process) | MultiBlowForging2D | false | 2 (2+2 blows) | `DOESCL 100` / `MOVCTL 2 1` x4 | 433 lines, 4 blows |
| 79F_B_MB3X (fresh) | MultiBlowForging3D | false | 1 | `DOESCL 100` / `MOVCTL 2 1` | = 75F byte for byte |
| 74_B_MB2 (same process) | MultiBlowForging2D | false | 1 | `DOESCL 100` / `MOVCTL 2 1` | 555 lines |
| 92_B_MB2REL (fresh) | MultiBlowForging2D | true (kept) | 1 | `DOESCL 1` / `MOVCTL 2` | 74 except L504-505 |

* 75F: the fresh 3D root is `MultiBlowForging3D`; the moproj differs from 75 only in that name
  and the diagram view (CenterX/Y, one `-0`). Default is absolute energy: `DOESCL 100`.
* 79F: the input is 79's input again (75's moproj with a `MultiBlowForging2D` tree edited to
  2x2). A fresh GUI ignores the 2D-named tree and saves 3D defaults: moproj = 75F's, MST = 75F's.
  79_B_MB3X's 4-blow MST therefore depends on the earlier 74 open, not on its moproj alone.
* 92: the GUI keeps `UseRelativeEnergy` true. In the MST the first sim's two lines change from
  `DOESCL 100` + `MOVCTL 2 1` to `DOESCL 1` + `MOVCTL 2` (the second sim's `DOESCL 1` /
  `MOVCTL 2 2` is the same in both). The save also renumbered the workpiece object 6 to 5, as 78
  did (both re-open a GUI-saved 74).
* 75 and 79 stay in the corpus as saved. P14's call: 79_B_MB3X is `B-excluded` (its 4 blows are
  session state; 79F is the oracle); 75_B_MB3 stays, since its MST equals 75F's.
* Where the absolute-energy default comes from: the MultiBlow data ctor 0x140618d20 and the bool
  template (0x143d9f0f0, registered with `true`) both say relative, but ParseProperties 0x140094f70
  calls (props+0x10)->vt[+0x8] before applying the saved values, and MultiBlow's slot 0x14060b950
  clears UseRelativeEnergy (MBD+0x88) and UseAdaptiveReheat (MBD+0xe8). Every other class has the
  empty base slot 0x1403537f0 there (P14).
