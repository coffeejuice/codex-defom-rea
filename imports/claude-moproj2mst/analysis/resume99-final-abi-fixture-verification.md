# CP99 ABI and fixture verification

CP99 rebuilt from a stable source tree: SHA-256 `69eac4686a3c0b8d0f2347408bc8f65bab147bef42502e8c895ce9b06c2b1c3e` across 3,393 files before and after configure, build, and test. All three build phases returned 0. Candidate `moproj2mst.dll` SHA-256: `121951a6a657fad6fb23c96634f745e0802a540dbaa5384138ba668a9915d39a`, matching the CP99 build and full-run receipts.

MSVC DUMPBIN 14.44.35207.0 `/nologo /exports` shows CP99 and frozen CP98 (`build/wave3-checkpoint98-verified/moproj2mst.dll`, SHA-256 `0e6c3012d80588bed9daef45ebc8e496f3e8185a98ac001022db101573ce5d5f`) have identical six exports, hints, ordinals, RVAs, ordinal base, timestamp, version, and section summary:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

The focused public ABI command `ctest --test-dir build/candidate -R "^m2m\\.g0\\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure` passed all three tests, exit 0. No rebuild was performed for this independent check.

The CP99 full-run receipt records 266 registered tests: 259 ordinary passes, six with logged gaps, one with a logged difference, zero skipped/failed/incomplete-output, and CTest exit 0. Build and run receipts bind to the same source hash. The before/after fixture snapshots for run `1792901141642323300` are each 3,231,211 bytes and have identical SHA-256 `ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`; the receipt also has an empty `fixture_changes` list. Historical B98 manifest entries remain unchanged: `Gear_blank.KEY` 2,291 bytes, SHA-256 `82565a02650afa84774ea45eccea1a32b64ed113b405720b43a7b34e967b5290`; `Gear_blank.MST` 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`; before-state `Gear_blank.moproj` 10,932 bytes, SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`.

These checks establish exported ABI identity, focused candidate ABI test success, and fixture immutability. The logged gaps and difference remain; the historical fixtures are not fresh original-runtime acceptance evidence. No GUI or original executable was launched.
