# CP97 ABI and historical-fixture verification

## Candidate build and export table

`analysis/resume97-integrated-grouped-build.json` records a completed source-stable build at SHA-256 `3c6a80a53f57a8287a626792845451d811d1bfd1052a371d85910214ba11392b` before and after. Its CP97 candidate DLL hash is `c3030bda4c7f6445cf68c36418f0e2ea5ef7728265b60c18668b585f4cdd7e6a`.

Frozen CP96 DLL `build/wave3-checkpoint96-verified/moproj2mst.dll`: SHA-256 `69fef06a42ff2dc4bf81547c4a56cc9a55c3613816553c54b566cc8257baf684`. CP97 candidate DLL `build/candidate/bin/moproj2mst.dll`: SHA-256 `c3030bda4c7f6445cf68c36418f0e2ea5ef7728265b60c18668b585f4cdd7e6a`, matching the build receipt.

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` reports identical names, hints, ordinals, and RVAs for CP96 and CP97:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both have ordinal base 1, six functions/names, timestamp `FFFFFFFF`, version `0.00`, `.rdata=0xC1000`, and `.text=0x2FB000`. DLL hashes differ; this establishes export-table equality only.

## Focused public ABI tests

Against the existing CP97 candidate, without rebuilding:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

`m2m.g0.abi`, `m2m.g0.abi_open_failure`, and `m2m.g0.abi_original_settings` all passed; CTest exit code 0. Test scratch is configured under `build/candidate/t`. No source or fixture was modified by this focused check.

## Historical fixture and full-suite receipt checks

The retained full-run receipt `build/logs/resume97-full-run.stdout.json` records CTest/launcher exit code 0, 261 registered tests, 254 ordinary passed, six passed with logged gaps, one passed with logged differences, zero skipped, zero failed, zero incomplete-output and zero unreported tests. This is a pass with recorded gaps/difference, not 261 clean semantic matches. Its execution binding matches the CP97 source tree before/after and reports stable binaries; `fixture_changes` is empty.

Independently compared the retained snapshots in `build/test-runs/1792899947378329400/fixtures-before.json` and `fixtures-after.json`: both contain 16,728 entries, have the same SHA-256 `ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`, and parse to identical mappings. The historical B98 after-state entry `gui_mo\\oracle_b\\98_B_DS3F\\Gear_blank.MST` remains 2,756 bytes with SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`; the before-state `.moproj` is also unchanged in both snapshots. This confirms fixture immutability for the logged run.

## Limits

No GUI or protected original executable was launched. These receipts establish source-stable candidate tests and unchanged retained fixture snapshots; they do not upgrade the six gaps/one difference to clean parity or constitute fresh original-runtime acceptance.
