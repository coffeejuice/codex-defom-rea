# CP95 focused DLL ABI verification

## Build identity

`analysis/resume95-integrated-grouped-build.json` reports completed and source stable before/after at SHA-256 `ec4212e45a75ad74f81f59e07f38a08811c03c1fbfc7f2b0cbe986f5b8e6dc3c`. Root reports four focused targets pass; the full suite is still running.

Frozen CP94 DLL `build/wave3-checkpoint94-verified/moproj2mst.dll`: SHA-256 `820bc05f63ec6ffc79e506a29739208def581c2ab5ad86179a420bd79f9ecdd9`.

CP95 candidate DLL `build/candidate/bin/moproj2mst.dll`: SHA-256 `3aaa38d2c20d1b821589d67edc9a8aca6d4d1d4c12bb076fdbb52fdf50436684`, matching the build receipt.

## Export comparison

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` reports identical export names, hints, ordinals, and RVAs for CP94 and CP95:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both report ordinal base 1, six functions/names, timestamp `FFFFFFFF`, version `0.00`, `.rdata=0xC1000`, and `.text=0x2FB000`. DLL hashes differ, so only export-table equivalence is established.

## Focused public ABI tests

Ran against the existing CP95 candidate without rebuilding:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); CTest exit code 0. Tests use configured disposable candidate scratch. No source or fixture was modified.

## Limits

CP95 retains CP94's six observed C DLL exports and passes the focused public ABI tests. The full-suite result is not included because it is still running. This does not establish original-runtime parity or external independently compiled-client compatibility.
