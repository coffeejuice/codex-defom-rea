# CP100 ABI and fixture verification

The CP100 build receipt records a stable 3,395-file source tree at SHA-256 `16aa2f83306fd0692fabe254be58ee4c37313a4f12993e38d9d5651cfc98ea62`; configure, focused preflight, and build all returned 0. Candidate `moproj2mst.dll` SHA-256 is `3cf71daaca2aa4c1ae5dfbe8be0aa5895452ce691ae912ef16b6f56ae356de99`, matching the build and full-run receipts.

MSVC DUMPBIN 14.44.35207.0 `/nologo /exports` shows identical six exports, hints, ordinals, RVAs, ordinal base, timestamp, version, and section summary for CP100 and frozen CP99 (`build/wave3-checkpoint99-verified/moproj2mst.dll`, SHA-256 `121951a6a657fad6fb23c96634f745e0802a540dbaa5384138ba668a9915d39a`):

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

The CP100 full-run receipt records 268 tests: 261 ordinary passes, six with logged gaps, one with a logged difference, and no skipped, failed, or incomplete-output tests (CTest exit 0). Source tree before/after hashes in the execution binding match the build hash. Fixture snapshots `build/test-runs/1792901518628521400/fixtures-before.json` and `fixtures-after.json` are both 3,231,211 bytes and have identical SHA-256 `ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`; the receipt reports no fixture changes. Retained B98 entries are unchanged: `Gear_blank.KEY` 2,291 bytes, SHA-256 `82565a02650afa84774ea45eccea1a32b64ed113b405720b43a7b34e967b5290`; `Gear_blank.MST` 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`; before-state `Gear_blank.moproj` 10,932 bytes, SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`.

This establishes export-table identity, focused candidate ABI results, and fixture immutability. The six gaps and one difference remain qualifications; historical fixture checks do not establish fresh original-runtime parity. No GUI or original executable was launched.
