# CP98 ABI and historical-fixture verification

## Build and exports

`analysis/resume98-integrated-grouped-build.json` reports a completed source-stable build at SHA-256 `5bf47926bd3142a4e2429131f9ffd75d5de32be599f36880deb81160135e169d`. The CP98 candidate DLL hash is `0e6c3012d80588bed9daef45ebc8e496f3e8185a98ac001022db101573ce5d5f`, matching the build receipt.

Frozen CP97 DLL `build/wave3-checkpoint97-verified/moproj2mst.dll`: SHA-256 `c3030bda4c7f6445cf68c36418f0e2ea5ef7728265b60c18668b585f4cdd7e6a`.

MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` reports identical names, hints, ordinals, and RVAs:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both have ordinal base 1, six functions/names, timestamp `FFFFFFFF`, version `0.00`, `.rdata=0xC1000`, and `.text=0x2FB000`. DLL hashes differ; this is export-table equivalence only.

## Focused public ABI tests

Ran on the existing CP98 candidate without rebuilding:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed; exit code 0. No source or fixture was modified by this focused run.

## Full-suite and historical-fixture receipts

The retained `build/logs/resume98-full-run.stdout.json` reports 263 registered tests: 256 ordinary passed, six passed with logged gaps, one passed with a logged difference, zero skipped/failed/incomplete-output, and CTest exit code 0. `fixture_changes` is empty, unreported/incomplete test lists are empty, and the source/binary execution binding is stable.

Independent fixture snapshot check: `fixtures-before.json` and `fixtures-after.json` each contain 16,728 entries, are structurally identical, and share SHA-256 `ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`. Historical B98 records match on both sides: `Gear_blank.KEY` is 2,291 bytes / SHA-256 `82565a02650afa84774ea45eccea1a32b64ed113b405720b43a7b34e967b5290`; retained `Gear_blank.MST` is 2,756 bytes / SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`; before-state `Gear_blank.moproj` is 10,932 bytes / SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`.

No GUI or protected original executable was launched. The six gaps and one difference remain recorded qualifications; fixture immutability and candidate test results do not establish fresh original-runtime parity.
