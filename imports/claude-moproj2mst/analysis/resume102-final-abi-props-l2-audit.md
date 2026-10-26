# CP102 repaired-build ABI and props_l2 receipt audit

## Repaired build and public ABI

The initial CP102 build receipt is a failed preflight: its heat-route executable link returned `4294967295` after MSVC reported `LNK2019` for missing `main` (then `LNK1120`). The repair receipt records the corrected source tree SHA-256 `d8fe0587cd757f35a18d61ccb6ea20ee3e025bd2de431044b16af4004231fd6c` across 3,402 files, with configure, focused preflight, and full build all returning 0. The candidate DLL SHA-256 is `d332eceb9fcc48d2a13e1c3bb26d1681f322bbb8635ed38c2d9488c1f8e5bc39`, matching the repair build and CP102 full-run receipt.

MSVC DUMPBIN 14.44.35207.0 `/nologo /exports` reports the same six names, hints, ordinals, and RVAs in CP102 and frozen CP101 (`build/wave3-checkpoint101-verified/moproj2mst.dll`, SHA-256 `79697ac57a9f9c77298b4379c358d7f35a42232c9ee063519da2b14b348a14c`):

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

## Full-run and fixture receipts

The CP102 run `1792903500363807000` is bound to the repaired source hash before and after execution. It records 275 tests: 268 ordinary passes, six with logged gaps, one with logged differences, zero skipped/failed/incomplete-output, CTest exit 0. Fixture before/after snapshots are both 3,231,211 bytes and have identical SHA-256 `ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`; `fixture_changes` is empty. Historical B98 files remain unchanged: `Gear_blank.KEY` 2,291 bytes / SHA-256 `82565a02650afa84774ea45eccea1a32b64ed113b405720b43a7b34e967b5290`; `Gear_blank.MST` 2,756 bytes / SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`; before-state `Gear_blank.moproj` 10,932 bytes / SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`.

## `props_l2` five-skip correction

The CP102 `m2m.moproj.props_l2` diagnostic now records five fixture-local skips explicitly: `B11_CM`, `T_CM_DB.x`, `T_CM_ONLY.x`, `T_GRP.x`, and `T_GRP2.x`, each because its project does not open with the class unregistered. Its new summary says `95 operations, 900 values, 0 failures, 5 cases skipped`; the test still reports zero of one test cases failed and is categorized as a logged gap. The matching CP101 receipt had the five individual skip lines but omitted the aggregate summary. These are inner sweep cases, so CTest's top-level skipped count remains zero; the five should not be presented as cleanly covered fixtures.

No GUI or original executable was launched. Fixture integrity and ABI identity do not resolve the remaining logged gaps/differences or establish fresh original-runtime parity.
