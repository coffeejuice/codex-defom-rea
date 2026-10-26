# CP81 final DLL ABI verification

## Build and artifact hashes

`analysis/resume81-integrated-grouped-build.json` reports `completed` with source stable at SHA-256 `519e500657f42de2336afcf6f6acbfc7795ea34fa32a9c5be6d89c7486ae32f2` before and after the build.

Frozen CP80 DLL `build/wave3-checkpoint80-verified/moproj2mst.dll`: SHA-256 `d8fb622546a60f6598f1f247ba4ec010c3f8bcae02ae4f553b2a7d9760214463`.

CP81 candidate DLL `build/candidate/bin/moproj2mst.dll`: SHA-256 `8c13bb2929f4d5c9e7a2f3a1503a95fd9e10742283aebe5a2b0ffbcca4c20cc9`, matching the build receipt.

## Export comparison

MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` produced identical names, ordinals, hints, and RVAs for both DLLs:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both have ordinal base 1, six functions/names, timestamp `FFFFFFFF`, version `0.00`, and matching section sizes (`.text=2F8000`). This is export-table equivalence, not implementation-byte equivalence.

## Focused public ABI tests

Ran:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three targets passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); CTest exit code 0. Test scratch is configured under `build/candidate/t`. No source, fixture, GUI, original executable, or mutable REA database was modified or used.

Qualification: CP81 preserves the CP80 six-symbol export table and passes focused public ABI tests. This does not establish original-runtime parity or compatibility of an independently compiled external client.
