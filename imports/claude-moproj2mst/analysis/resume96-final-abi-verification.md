# CP96 focused DLL ABI verification

## Build identity

`analysis/resume96-integrated-grouped-build.json` reports completed and source stable before/after at SHA-256 `399aa56774d5724e6c2ba379e561b22af2d009b8798cb0432d92dbf11bcde126`. Root reports four focused targets pass; the full suite remains in progress.

Frozen CP95 DLL `build/wave3-checkpoint95-verified/moproj2mst.dll`: SHA-256 `3aaa38d2c20d1b821589d67edc9a8aca6d4d1d4c12bb076fdbb52fdf50436684`.

CP96 candidate DLL `build/candidate/bin/moproj2mst.dll`: SHA-256 `69fef06a42ff2dc4bf81547c4a56cc9a55c3613816553c54b566cc8257baf684`, matching the build receipt.

## Export comparison

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` shows identical export names, hints, ordinals, and RVAs for CP95 and CP96:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both report ordinal base 1, six functions/names, timestamp `FFFFFFFF`, version `0.00`, `.rdata=0xC1000`, and `.text=0x2FB000`. DLL hashes differ; this establishes export-table equivalence only.

## Focused public ABI tests

Ran against the existing CP96 candidate without rebuilding:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); CTest exit code 0. Tests use configured disposable candidate scratch. No source or fixture was modified.

## Limits

CP96 retains CP95's six observed C DLL exports and passes the focused public ABI tests. The full-suite result is not included because it is still running. This does not establish original-runtime parity or independently compiled external-client compatibility.
