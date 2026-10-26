# moproj2mst corpus coverage (oracle A)

Searched on 2026-10-22, read only: `C:\PROBLEM\TEST_REA` and `C:\Program Files\SFTC\DEFORM\v11.0.2`
(including the 2D/3D Examples, LABS, and `Tutorials.zip`, whose entries were listed without
extracting).

## Result: no new cases

The search found 15 `.moproj` files, each with a saved `.MST`. All 15 are already in
`cases.tsv`:

| Source | Cases |
|---|---|
| `TEST_REA` | 27 COG_P1_B1, 28 Inc718_D420_GFM_V_DIES_Angle2_18 |
| 2D Examples | 46–51 (Boolean, Extrusion/Boolean cycles, Operation cycles; EN and SI) |
| 2D LABS | 52 BEND_2D |
| 3D Examples | 53–56 (Cogging labs 1 and 2, Multiple_Operations_Example1 EN and SI) |
| 3D LABS | 57 Gear_blank, 58 Spike_Forging |

`Tutorials.zip` (38 entries) holds no `.moproj` or `.MST`.

Fifteen `.MST` files in the Examples have no `.moproj` next to them, so they cannot be
oracle-A cases. Older GUIs wrote them:

* "TEMPLATE SERIES" header: `ROLL_EN/SI.MST` (multipass rolling) and `HAMMER_EN/SI.MST` (3 ×
  `*!SIMTYP Hammer`).
* `*!SIMTYP Deformation`/`Heat-*`/`Boolean` names: the old `MO2_*` folders, the old
  `Multiple_Operations_Example1.MST` and `Cup_Flowform-ALE.MST` (`Flow-forming`).
* "MASTER KEYWORD FILE" header: `Cogging_Example1.MST` and `COGGING.MST`.

At most they hint at keyword sequences. They are not byte references for DEF_GUI_MO_64.

## Coverage per operation class (oracle A, unique cases)

Counts are from `cases.tsv`: oracle `A`, copied, not a duplicate (43 cases). Legacy types are
mapped as in s1-01 6.1. The two projects over 300 MB (oracle A, not copied, run read-only by G1)
add 2 HeatTransfer cases with 422 ops and 2 Cogging cases with 310 ops.

| Class key | Cases | Ops | Writer packet |
|---|---|---|---|
| SfOprHeatTransfer | 27 | 86 | P10 |
| SfOprCogging | 14 | 21 | P11 |
| SfOprForming | 11 | 14 | P10 |
| SfOprForming2D | 7 | 19 | P10 |
| SfOprBoolean2D | 4 | 14 | P12 |
| SfOprHeatTransferExpress2D (AirConvect2D, HeatResting2D) | 3 | 5 | P10 |
| SfOprCycle | 2 | 2 | P13 |
| SfOprDoeOutput (not walked, no output) | 1 | 1 | P13 |

## Excluded cases

`cases.tsv` has an `exclusion` column. An excluded case has `oracle = A-excluded`, so tier 3
(`--oracle-a`) and tier 2 leave it out; the reason is in the column.

| Case | Reason |
|---|---|
| 56_Multiple_Operations_Example1 (SI) | Non-v11 MST (2013): `*!TITLE` comes from the project `Name` attribute where v11 uses the `<Title>` text, and `OPRNAM` keeps duplicate operation names that v11 renames on open (s1-01 Q9). |
| 55_Multiple_Operations_Example1 (EN) | Pre-v11 MST: saved 2013-03-22, while the exe's PE timestamp is 2014-12-04. It has 5 `ThermalStop` KEY lines (KFREAD), and the exe has no `ThermalStop…KEY` file name literal, so no v11 emitter writes them (P17). |
| 79_B_MB3X (oracle B, `B-excluded`) | Session-seeded: saved in the same GUI process after 74_B_MB2, which set the process-wide MultiBlow root template (0x143d9ef10) to `MultiBlowForging2D`, so the GUI took this file's 2D-named 2x2 tree. A fresh GUI ignores that tree and saves 3D defaults: 79F_B_MB3X is the oracle (P14). 75_B_MB3 stays: its MST equals 75F's. |
| 53_Cogging_Multiple_Ops_Lab1 | Pre-v11 MST: moproj and MST dated 2013-09-27, while the exe's PE timestamp is 2014-12-04. It has 3 `ThermalStop` KEY lines that no v11 emitter writes (no `ThermalStop…KEY` literal in the exe; P17). It was the only runnable case for SfOprHeatTransferExpress (3D). |

## Waived lines (known pre-v11 differences)

`cases.tsv` has a `waiver` column (comma-separated ids). `run_corpus.py` rewrites the matching lines
of the expected MST to the v11 form before the byte compare, and lists every rewritten line in the
result note (`waived L<n> old->new`), so the rest of the file is still verified byte for byte.

| Waiver | Cases | Rewrite |
|---|---|---|
| `legacy-express-simtyp` | 46 and 49 MO2_Boolean_Oprations (EN, SI), 52 BEND_2D | `*!SIMTYP` of a legacy express type (`SfOprAirConvect`, `SfOprHeatResting`, `SfOprHeatExpress`, `SfOprHeatDwelling`, with or without `2D`) becomes `SfOprHeatTransferExpress2D` if the key contains `2D`, otherwise `SfOprHeatTransferExpress`. This is the mapping of v11's factory 0x140046cc0 (s1-01 5.2), and P01 confirmed that v11 writes the mapped key; the three MSTs are 2013 saves without ThermalStop lines. Waived lines: 46/49 L27, L57; 52 L27. |

## Oracle B (GUI-saved, separate corpus `gui_mo/s1/corpus_b`, `cases_b.tsv`)

Saved by the original DEF_GUI_MO_64 on 2026-10-22 (`oracle_b/README.md`). One case per class:

| Class key | Case |
|---|---|
| SfOprHeatTreat / SfOprHeatTreat2D | 59_B_HT3 / 60_B_HT2 |
| SfOprDiestress | 61_B_DS3 |
| SfOprDiestressBM2D / BM3D | 62_B_BM2 / 63_B_BM3 |
| SfOprReport / SfOprShapeOptim | 64_B_RPT / 65_B_SOPT (write nothing) |
| SfOprDistortion3D / 2D | 66_B_DIS3 / 67_B_DIS2 |
| SfOprBoolean3D | 68_B_BOOL3 |
| SfOprConvert2Dto3D | 69_B_CNV |
| SfOprPre3D / SfOprPre2D | 70_B_PRE3 / 71_B_PRE2 |
| SfOprHTFurnace | 72_B_HTF |
| SfOprHeatTransferExpress (3D) | 73_B_HTX3 |
| SfOprMultiBlow2D / 3D | 74_B_MB2 / 75_B_MB3 |
| SfOprFormingExpress2D / FormingExpress | 76_B_FE2 / 77_B_FE3 |

## Gaps

Batch 2 (78-91) adds MultiBlow with heats, reheat and rotation, FormingExpress express mode
(Equipment 3/4, blow table), operations placed first (HeatTreat2D, Convert2Dto3D, Pre3D,
HTFurnace, FormingExpress) and English units (MultiBlow2D, FormingExpress 2D/3D).
Fresh-process reruns add 75F_B_MB3 (3D root `MultiBlowForging3D`), 79F_B_MB3X (2D-named
tree ignored) and 92_B_MB2REL (UseRelativeEnergy true); 75 and 79 were seeded by 74 in one
process (oracle_b/README.md).

CopyMirror and Group are unsupported in v11: not in the class registry 0x14006dba0 (P01), and the
GUI crashes opening them (oracle_b/README.md). Still without a case: DOE sub-projects (P17),
DiestressBM with a new DB, Diestress with a lower-case `.db`.
