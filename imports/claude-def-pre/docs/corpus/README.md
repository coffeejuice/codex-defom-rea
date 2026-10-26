# DEF_PRE test corpus (Phase 0.3)

Test data for the DEF_PRE_64 batch port, as Aleksandr defined it on 2026-10-21. The files themselves
live outside the repository in `C:\Users\alext\Projects\DEFORM_corpus` (5.9 GB, 5.0 GB of it one DB); only
this manifest and the coverage catalogue are in git. Originals under `C:\PROBLEM\TEST_REA` and the
DEFORM install are only read; every copy is checked against its source by SHA-256.

The catalogue comes from parsing the files' text and structure only. It says which test files exercise
which keywords; it is not a source for how DEF_PRE behaves.

## Contents

| Set | Files | Source |
|---|---:|---|
| DB | 2 (3D, SI) | `C:\PROBLEM\TEST_REA\*\*.DB`: `COG_P1_B1.DB` (9 MB, 32 steps), `Inc718_D420_GFM_V_DIES_Angle2_18.DB` (5.0 GB, 790 steps) |
| KEY, SI (primary) | 432 | `2D\2D_Examples\SI`, `3D\3D_Examples\SI` |
| KEY, British units (optional) | 502 | `…\EN` folders of the same examples |
| KWD (keyword-format preview data) | 42 SI + 41 EN | same folders |
| CMD (command scripts) | 24 SI + 24 EN | `.MST`, `.d2mo`, `.d3mo` and `DEF_MULTI`-style `.INI` files in the example folders that contain command keywords |
| Materials | 4 × (`.std`, SI `_s` KEY, English `_e` KEY) | `MaterialLibrary\Standard\{Superalloy,Steel}` |

Materials, by the name shown in DEFORM's library:

| Library name | Files |
|---|---|
| IN718_6um[1650-2200F(900-1200C)] | `IN718_6um[1650-2200F(900-1200C)]*` |
| S45C-JAPAN | `JIS-S45C(HeatTreatment)*` |
| AISI-1020 (Oxley's Equation)machining | `AISI-1020(Machining)*` |
| AISI4340-JohnsonCook | `AISI4340-JohnsonCook*` |

`Steel_at_Extented_Temperatures` holds byte-identical copies of the last three; the `Steel` ones were taken.

Calls made while assembling it:

* Units come from the folder (`SI` / `EN`) and the material suffix (`_s` / `_e`). The `UNIT` keyword
  agrees for most files (`UNIT 1` in 238 of 240 SI files, `UNIT 2` in 234 of 250 EN files); the
  exceptions are listed in `key_coverage.tsv` (`UNIT_value` column). Both DBs are SI: their projects'
  `SimCtrl.KEY` files carry `UNIT 1`.
* Command keywords never occur in the example KEY files, only in the multi-operation scripts, so those
  48 script files were added as kind `CMD`. Nothing else from the example folders (DBs, geometry, logs)
  was copied.
* Only the `.DB` files were taken from `C:\PROBLEM\TEST_REA`, not their project folders.

## Files here

| File | Content |
|---|---|
| `manifest.tsv` | one row per corpus file: path, kind, dim, units, set (`primary` / `optional`), size, sha256, source |
| `key_coverage.tsv` | per KEY/KWD/CMD file: `UNIT` value, DEFORM header comment, registered keywords and command keywords found at line start |
| `db_coverage.tsv` | per DB step: objects and the node/element keywords deformdb decodes with data |
| `keyword_coverage.tsv` | per registry entry (all 304 of `docs/re/keyword_registry.tsv`): file counts by set, 2D/3D and units, DB steps, an example file |
| `coverage-summary.md` | the summary: covered and uncovered keywords and commands |
| `scripts/` | `build_corpus.py` (copy + manifest) and `catalogue.py` (coverage) |

## Result

304 registry entries: 247 have at least one primary (SI) test file, 5 more only a British-unit one
(`TCYCLE`, `GENAXS`, `DSTROKE`, `GRNMDL`, `TXTODF`), and 52 have none. Of the 33 command keywords, 10
are covered by SI files and 3 by EN only; 20 have no test file (`PROBID`, `RUNTYP`, `WRTOBJ`, `SVSCAL`,
`CPYOBJ`, `DOESCL`, `GENINT`, `IMPLUB`, `GENSUB`, `ARMCTL`, `ACTOUT`, `CNVT3D`, `REMESH`, `BRDEXT`,
`DEFAMG`, `DEFINT`, `GENBCC`, `DEFFRZ`, `MORP`, `GFREAD`). Those will need hand-written command scripts
in Phases 6–9. Full lists are in [`coverage-summary.md`](coverage-summary.md).

The DB side is a lower bound: deformdb's typed view maps 52 node/element keywords, so scalar and header
keywords in a DB step are not counted yet. The Phase 2 oracle (`DBREAD` + `KFWRIT` through the original
DEF_PRE) will list them per step.

## Regenerate

```bat
python -I docs\corpus\scripts\build_corpus.py C:\Users\alext\Projects\DEFORM_corpus
copy C:\Users\alext\Projects\DEFORM_corpus\manifest.tsv docs\corpus\
python -I docs\corpus\scripts\catalogue.py . C:\Users\alext\Projects\DEFORM_corpus deformdb\build\deformdb.dll
```
