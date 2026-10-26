# CP103 final ABI, raw-difference, and RS_42 acceptance audit

## Final build and ABI

The final CP103 build is `analysis/resume103-integrated-grouped-repair4-build.json`: source SHA-256 `b321107fecaa4a56c81eeaee2b814559b82f7c18ca44ace885731d96142583bc` across 3,406 files; configure, focused preflight, and full build all returned 0. Candidate DLL SHA-256 `943eee6b85235b0825ea78289744f17296116e0d9fcb79ba3a9c78924396c939` matches repair3/repair4 build receipts and the final run binding.

MSVC DUMPBIN 14.44.35207.0 `/nologo /exports` shows identical exports, hints, ordinals, RVAs, timestamp/version, and section summary for CP103 and frozen CP102 (`build/wave3-checkpoint102-verified/moproj2mst.dll`, SHA-256 `d332eceb9fcc48d2a13e1c3bb26d1681f322bbb8635ed38c2d9488c1f8e5bc39`):

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

The public ABI focused command (`ctest --test-dir build/candidate -R "^m2m\\.g0\\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure`) passed all three tests, exit 0.

## Separate failed attempts and final run

Do not collapse earlier build failures into the final success:

| Attempt | Receipt result | Retained reason |
|---|---|---|
| Initial `resume103-integrated-grouped-build.json` | Focused preflight exit 2 | `test_db_load_split_default.cpp` compile errors (`engine`, `DbHandle`, and `Ctx` undeclared). |
| `repair1` | Focused preflight exit `4294967295` | Link failure for `m2m_mdt_db_load_split_default.exe`: unresolved `main` (`LNK2019`, then `LNK1120`). |
| `repair2` | Full build exit 2 | DOE variable tests failed to compile against `Var` (`dimension` missing; cascading assertion errors). |
| `repair3` | Configure, preflight, and build exit 0 | Candidate DLL hash `943eee…`. |
| Final `repair4` | Configure, preflight, and build exit 0 | Same DLL hash as repair3; final test run uses the final repaired source hash. |

Final full-run receipt `build/test-runs/1792904675837235500/receipt.json` records 279 tests: 272 ordinary passes, six with logged gaps, one with logged differences, zero skipped/failed/incomplete-output, CTest exit 0. Source hashes before and after match repair4. Fixture snapshots are both 3,231,211 bytes with SHA-256 `ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`; `fixture_changes` is empty. Historical B98 records remain: `Gear_blank.KEY` 2,291 bytes / `82565a02650afa84774ea45eccea1a32b64ed113b405720b43a7b34e967b5290`; `Gear_blank.MST` 2,756 bytes / `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`; before-state `Gear_blank.moproj` 10,932 bytes / `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`.

The final `m2m.s.session_cogging_save_geometry` test passed with no gaps/differences. Its log records one case passed. It compares raw bytes for `Task00004/Cogging.MDT` and `Objects/Object00010..13.KEY` against retained `RS_42_vdie` output after calling the public `convert(..., M2M_FLAG_WRITE_SAVE, 1252)`, and requires `M2M_OK` plus an MST. It verifies five selected files only, not full-folder byte parity, native Close, or a fresh original run. The broad `m2m.s.save_folder` entry separately says `SAME A:RS_42_vdie`; that comparator result is not the strict test.

| Selected retained output | Before SHA-256 | Expected after SHA-256 | Result in strict test |
|---|---|---|---|
| `Operations/Task00004/Cogging.MDT` | `b2b8e57be83ff27def086b0b95ce0ee53b68c2262d6b1c583ffd47a9507b786e` | `714b238dd66674e0927d8b8dc8125aba3b66f21dee6871f5e3cb1592795cfd77` | raw bytes equal after |
| `Objects/Object00010.KEY` | `4d2c7b3dd72886940f69b246ac9b59f312dd40bea4e1c3176679deecc5f44085` | same | raw bytes equal after |
| `Objects/Object00011.KEY` | `666d5fa54af58f9b1b8a42b4d46acc359f649c0063853a5566ae0b89abfa9791` | same | raw bytes equal after |
| `Objects/Object00012.KEY` | `8beeb38fcbc3d6656323fb6d4c16c74195b12ba84933a94fa7bf53d2bf415fdc` | `f333ed0ed0bc6ac9db29dac98016f4bd2d537ead74ef23700fe562b136fffa08` | raw bytes equal after |
| `Objects/Object00013.KEY` | `01244cf1c6124179ebb3da42f9460950f5d5b523b2f6b86820d6e96026d82171` | `4cdb1a2ca9def0f924d531a1fc3046fa154acd9d52abc129a94a2d803d194833` | raw bytes equal after |

The test uses a generated owned leaf below `C:/PROBLEM`, asserts path lengths under 100 characters, and removes only its own leaf. The full-run log shows success and cleanup was reached. The parent separately notes an earlier sandbox denial for this path; I found no retained standalone denial receipt in the CP103 files. Keep that denial as a separate blocked attempt, not as a test result or as evidence against the final passing run. The test and run do not claim any original GUI/modal execution.

## Folder-sweep raw differences

`passed-with-logged-differences: 1` is the `m2m.s.save_folder` test. Its current CP103 log records five differing cases:

| Case | File-level differences |
|---|---|
| `A:RS_55_Multiple_Operations_Example1` | One differing file: `operations/task00002/simctrl.key`; no missing/extra. An earlier retained byte audit of this same historical pair records offsets 1439 and 1455 (candidate `0`, retained Save `1`), but the CP103 sweep removes work folders and does not retain fresh candidate bytes/hashes; do not call those offsets a CP103 raw byte capture. |
| `B:120_B_DOE` | `cog_p1_b1.moproj` differs; seven DOE Control paths missing: `cog_p1_b1.ds`, `cog_p1_b1.mst.temp`, `def_doe.ini`, `def_doe.par`, `def_doe_parser.ini`, `doeinputtable.xml`, `samples_lhs.par`. |
| `B:121_B_DOE_VAR` | Same differing project file and seven missing DOE paths, plus `temp.kwd`. |
| `B:122_B_DOE_GEO` | Same differing project file and ten missing paths: the seven DOE files plus `workpiece-geometry_workpiece_1_lower.geo`, `workpiece-geometry_workpiece_1_nominal.geo`, `workpiece-geometry_workpiece_1_upper.geo`. The CP103 per-test log elides its last two names as `... 2 more`; the full names are in retained `analysis/resume48-diff-b122.md`. |
| `B:98_B_DS3F` | One differing file: `gear_blank.mst`; no missing/extra. Retained analysis attributes the differing `KFREAD` line to an old workspace path. CP103 does not keep the generated candidate MST, so this remains a path-qualified historical discrepancy, not a new raw byte diagnosis. |

The Oracle-B tally is 29 file differences (8 + 9 + 11 + 1); the fresh corpus tally is one difference. Three older-window view profiles are separately accepted by that comparator. The DOE inputs are pre-interaction projects while retained outputs include GUI-created study/variable/geometry state, so those rows do not isolate a simple no-op Open–Save defect. The sweep remains diagnostic, not a strict acceptance gate.

No source/build change, GUI, or original executable was used for this audit. The CP103 run records candidate execution against retained fixtures; it does not establish fresh original-runtime parity.
