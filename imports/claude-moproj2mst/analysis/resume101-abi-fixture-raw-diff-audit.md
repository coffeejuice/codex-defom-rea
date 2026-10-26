# CP101 ABI, fixture, and raw-difference audit

## ABI and build binding

CP101 build/source receipt: 3,397 files, SHA-256 `7bdb6beb6b7dc8768c8fc23f7b0203e7756e40a509f2d17bc4a6bb6aa5da3239`, unchanged through configure, focused preflight, and build; all three commands returned 0. Candidate DLL SHA-256 `79697ac57a9f9c77298b4379c358d7f35a42232c9ee063519da2b14b348a14c8` matches the build and full-run receipts.

MSVC DUMPBIN 14.44.35207.0 `/nologo /exports` reports six identical exports, hints, ordinals, and RVAs for CP101 and frozen CP100 (`build/wave3-checkpoint100-verified/moproj2mst.dll`, SHA-256 `3cf71daaca2aa4c1ae5dfbe8be0aa5895452ce691ae912ef16b6f56ae356de99`):

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

The focused command `ctest --test-dir build/candidate -R "^m2m\\.g0\\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure` passed all three public ABI tests (exit 0).

## CP101 full-run and retained fixtures

The run receipt binds to the same source hash and reports 270 tests: 263 ordinary passes, six with logged gaps, one with logged differences, zero skipped/failed/incomplete-output, CTest exit 0. Fixture before/after files for run `1792901954688081700` are both 3,231,211 bytes and have identical SHA-256 `ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`; `fixture_changes` is empty. B98 historical entries remain as before: `Gear_blank.KEY` 2,291 bytes / SHA-256 `82565a02650afa84774ea45eccea1a32b64ed113b405720b43a7b34e967b5290`; `Gear_blank.MST` 2,756 bytes / SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`; before-state `Gear_blank.moproj` 10,932 bytes / SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`.

## Raw folder-sweep differences

The single suite count `passed-with-logged-differences: 1` is the diagnostic `m2m.s.save_folder` test, not one differing case. Its receipt/log report five DIFF cases and no extras:

| Case | Raw comparator record |
|---|---|
| `A:RS_55_Multiple_Operations_Example1` | One differing file: `operations/task00002/simctrl.key`; no missing/extra. Earlier retained byte-level audit of this same historical pair records 2,146 bytes on both sides and offsets 1,439 and 1,455; line 52 is candidate `SOLMTD ... 0 0 0 0` vs retained original `0 1 0 1`. The CP101 run log confirms the file-level difference but does not retain CP101 output bytes/hashes, so those earlier offsets are not asserted as a fresh CP101 byte inspection. |
| `B:120_B_DOE` | One differing file `cog_p1_b1.moproj`; seven missing: `doestudies/doestudy1/control/cog_p1_b1.ds`, `cog_p1_b1.mst.temp`, `def_doe.ini`, `def_doe.par`, `def_doe_parser.ini`, `doeinputtable.xml`, `samples_lhs.par`. |
| `B:121_B_DOE_VAR` | One differing `cog_p1_b1.moproj`; the same seven DOE Control files plus `temp.kwd` missing. |
| `B:122_B_DOE_GEO` | One differing `cog_p1_b1.moproj`; ten missing: the seven DOE Control files above plus `workpiece-geometry_workpiece_1_lower.geo`, `workpiece-geometry_workpiece_1_nominal.geo`, and `workpiece-geometry_workpiece_1_upper.geo`. The CP101 per-test log prints the first eight missing entries and `... 2 more`; the complete names are corroborated by the retained checkpoint47 diff inventory in `analysis/resume48-diff-b122.md`. |
| `B:98_B_DS3F` | One differing file `gear_blank.mst`; no missing/extra. The retained original’s line 30 contains a `KFREAD` path rooted in an older workspace. Earlier candidate-only path qualification normalizes that one path value; CP101’s folder-sweep output files are removed after comparison, so this run does not add fresh candidate bytes or a raw line-level comparison. |

The case ledger sums to 29 Oracle-B file discrepancies (8 + 9 + 11 + 1) and one corpus discrepancy. The sweep reports three expected older-window `.moproj` view profiles (`B:R11`, `B:R19`, `B:82_R52_BEND_2D`) separately from DIFFs. It also notes seven corpus projects without finished fresh re-saves, and skips GUI-preconditioned B114, B119, and B130n. The sweep is diagnostic and its passing CTest status does not mean these outputs match.

The DOE rows compare a pre-interaction input against an output containing GUI-created DOE state; they are not isolated no-op Open–Save comparisons. This report uses retained evidence for the two omitted B122 path names and earlier byte offsets, and does not claim CP101 candidate output byte hashes for the folder sweep. No GUI or original executable was launched.
