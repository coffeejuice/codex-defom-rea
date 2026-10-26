# CP105 final ABI and acceptance audit

## Frozen build and export comparison

CP105 repair2 records a stable 3,419-file source tree at SHA-256
`45f48bb02d844e35b23381dfb758ac92c51d820d8abc1c6894b65e0e3d0d9575` and a
successful Release build. The candidate DLL hash is
`0a321757927724bd22992a713741898f31603db1361fc2c1f198d2d04cc418ae`.
Frozen CP104 DLL `build/wave3-checkpoint104-verified/moproj2mst.dll` is
`e0a91b439058a189d1e705abb48522e6757e6d134d1b7da1478d8a7d2eb65ee5`.

I parsed both PE export directories independently. All six export names,
hints, ordinals, and RVAs match:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

The three focused ABI tests passed in my independent run:
`m2m.g0.abi`, `m2m.g0.abi_open_failure`, and
`m2m.g0.abi_original_settings` (CTest exit 0).

## Seven CP105 focused tests

The source-bound full-run receipt is
`build/test-runs/1792906954802152100/receipt.json`: 287 registered tests,
280 ordinary passes, 6 passed with logged gaps, 1 passed with logged
differences, and zero skipped, failed, or incomplete-output tests. It binds
source and candidate DLL hashes before/after to the CP105 build. The seven
new focused targets below are all recorded as passed with no target-local gaps
or diagnostic differences:

| Target | Full-run result | Independent spot run |
|---|---|---|
| `m2m.mdt.editor_setup_stretch_route` | passed | passed |
| `m2m.mdt.open_split_db_default` | passed | denied creating its `C:\\PROBLEM` scratch leaf |
| `m2m.mdt.terminal_harness_safety` | passed | passed (11 cases) |
| `m2m.s.save_cogging_open_failure` | passed | denied creating its `C:\\PROBLEM` scratch leaf |
| `m2m.s.save_remesh_failure_matrix` | passed | passed |
| `m2m.s.session_b109_raw` | passed | denied creating its `C:\\PROBLEM` scratch leaf |
| `m2m.s.session_dbstep_boolean_raw` | passed | denied creating its `C:\\PROBLEM` scratch leaf |

The four spot-run failures were `Access is denied` while creating the tests'
explicitly bounded `C:\\PROBLEM` paths. I did not retry through a different
path or request broader permissions. This execution-environment restriction
does not supersede the root full-run receipt; it means the four new tests were
not independently reproduced under this worker's current sandbox. The tests
that did run independently passed. CP105 fixture manifests are unchanged:
before/after snapshots both hash to
`ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`, and the
receipt's `fixture_changes` array is empty.

## Historical 93/5 and raw difference records

The CP105 `m2m.s.save_folder` log retains the same 98-case result as CP104:
93 matches and 5 DIFF cases, no refusals. The fresh-resave corpus is 48/49
identical with one file difference; Oracle B is 45/49 identical, including
three older-window view profiles, and reports 29 file differences. The five
DIFF records are:

- `A:RS_55_Multiple_Operations_Example1`: one differing
  `operations/task00002/simctrl.key`.
- `B:120_B_DOE`: `cog_p1_b1.moproj` differs; seven DOE Control outputs are
  missing.
- `B:121_B_DOE_VAR`: the same project difference and seven DOE Control files
  missing, plus `temp.kwd` (eight missing).
- `B:122_B_DOE_GEO`: the same project difference and ten missing files: the
  seven DOE Control files plus lower, nominal, and upper geometry `.geo`
  outputs. The log abbreviates the last two names; `analysis/resume48-diff-b122.md`
  retains the complete path inventory.
- `B:98_B_DS3F`: one differing `gear_blank.mst`.

The three DOE fixture after-states include GUI-created study/variable/geometry
effects and are not simple Open→Save comparisons. The B98 comparator difference
is associated with an old-workspace `KFREAD` line; the sweep does not preserve
candidate bytes for a new byte-level diagnosis. The fresh-resave A55 comparator
records the path but not candidate bytes/hashes for the previously noted byte
offsets. These remain comparator diagnostics, not clean passes or fresh
original-runtime evidence.

Six suite-level logged-gap tests remain distinct from the folder-sweep DIFF:
optional PACK header input, unavailable Save L6 oracle, five class-registration
cases in `props_l2`, P07 corpus omissions, and projects without pre-GUI-edit
inputs, plus the associated logged gap events. The suite's zero skipped count
does not make these gaps ordinary coverage.

No source edits, build, GUI, or original-executable action were performed for
this audit. I ran only focused CTest targets; four were blocked by the current
worker's `C:\\PROBLEM` write boundary as stated above.
