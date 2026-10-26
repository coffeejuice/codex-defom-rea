# CP104 final ABI and historical acceptance audit

## Final repaired build and ABI

The final `analysis/resume104-geometry-test-repair4-build.json` receipt records a stable 3,407-file source tree at SHA-256 `0c1c106248734b80f493e8467ac4f3099135a14f77d7f0a2a011c49d2c3e5198`. Configure, focused preflight, and build returned 0. Candidate DLL SHA-256 `e0a91b439058a189d1e705abb48522e6757e6d134d1b7da1478d8a7d2eb65ee5` matches the build and final full-run receipts.

MSVC DUMPBIN 14.44.35207.0 `/nologo /exports` shows the same six names, hints, ordinals, RVAs, timestamp/version and section summary in CP104 and frozen CP103 (`build/wave3-checkpoint103-verified/moproj2mst.dll`, SHA-256 `943eee6b85235b0825ea78289744f17296116e0d9fcb79ba3a9c78924396c939`):

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

The focused public ABI command `ctest --test-dir build/candidate -R "^m2m\\.g0\\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure` passed all three tests (exit 0).

## Full-run binding and test diagnostics

The final run `build/test-runs/1792905433772104200/receipt.json` records 280 tests: 273 ordinary passes, six with logged gaps, one with logged differences, zero skipped/failed/incomplete-output, CTest exit 0. Source before/after hashes match the final build hash. Fixture snapshots are both 3,231,211 bytes and have identical SHA-256 `ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`; the receipt reports an empty `fixture_changes` list.

Key CP104 targets are executable and pass, but their boundaries remain distinct:

- `m2m.doe.variable_geometry_entry` passes eight cases that preserve a reached header/Description prefix then refuse at direct Geometry entry. This is not successful geometry import, QFileInfo/path ownership, or public Save completion.
- `m2m.s.session_cogging_save_geometry` passes its one public WRITE_SAVE case with raw equality for five retained `RS_42_vdie` geometry files. It does not test full-folder parity, native Close, or fresh original execution.
- `m2m.mdt.db_load_split_default`, `m2m.mdt.boolean_postload_creation`, and `m2m.mdt.editor_setup_heat_route` each pass as candidate tests; they do not prove original modal delivery or whole-lifecycle integration.

The six logged gaps remain explicit: missing optional PACK/deflate inputs, one unavailable Save L6 oracle, five class-registration skips in `props_l2`, five P07 corpus skips, and `.moproj` cases without a pre-GUI-edit input, among the recorded gap events. They are not clean coverage.

## Historical 93/5 and the five case-level DIFFs

The full-save sweep is one test (`m2m.s.save_folder`) with status `passed-with-logged-differences`; it is diagnostic and does not assert every folder match. Its 98 cases total 93 matches and five DIFF cases, no refusals. The fresh-resave corpus contributes 48 matches and one file DIFF in 49 cases. Oracle B contributes 45 matches (including three older-window view profiles) and four DIFF cases with 29 file differences. The retained results are:

| Case | Raw comparator record |
|---|---|
| `A:RS_55_Multiple_Operations_Example1` | One differing `operations/task00002/simctrl.key`; no missing/extra. Earlier retained byte analysis records offsets 1439 and 1455, but CP104's sweep log does not preserve candidate bytes/hashes for those offsets. |
| `B:120_B_DOE` | `cog_p1_b1.moproj` differs; seven DOE Control files missing: `cog_p1_b1.ds`, `cog_p1_b1.mst.temp`, `def_doe.ini`, `def_doe.par`, `def_doe_parser.ini`, `doeinputtable.xml`, `samples_lhs.par`. |
| `B:121_B_DOE_VAR` | Same differing project file and seven DOE Control files, plus `temp.kwd` missing (eight total). |
| `B:122_B_DOE_GEO` | Same differing project file and ten missing files: the seven DOE files above plus lower, nominal, and upper `workpiece-geometry_workpiece_1_*.geo`. The CP104 log abbreviates the last two entries as `... 2 more`; retained complete path inventory is in `analysis/resume48-diff-b122.md`. |
| `B:98_B_DS3F` | One differing `gear_blank.mst`; no missing/extra. Retained analysis attributes the `KFREAD` line to an old workspace path. The CP104 sweep does not retain candidate output bytes, so this is not a new byte-level diagnosis. |

The DOE rows compare pre-interaction projects with expected outputs containing GUI-created study/variable/geometry state; they do not isolate a simple Open–Save mismatch. `passed-with-logged-differences: 1` means one diagnostic test, not one differing case.

The retained B98 fixtures are unchanged: `Gear_blank.KEY` 2,291 bytes / SHA-256 `82565a02650afa84774ea45eccea1a32b64ed113b405720b43a7b34e967b5290`; `Gear_blank.MST` 2,756 bytes / SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`; before-state `Gear_blank.moproj` 10,932 bytes / SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`.

These checks establish ABI identity, focused candidate ABI results, source/fixture binding, and retained-oracle comparator diagnostics. They do not establish fresh original-runtime parity or normal Close completion. No source/build edit, GUI, or original executable action was used for this audit.
