# CP83 final DLL ABI verification

## Build and DLL identities

`analysis/resume83-integrated-grouped2-build.json` reports `completed` and a source-stable build at SHA-256 `0a6221a8ce86c84ec95346ca92ec878d540cf204efbe7664b4b04b8364a1f713` before and after.

Frozen CP82 DLL `build/wave3-checkpoint82-verified/moproj2mst.dll`: SHA-256 `233154218e6e1dd981c9df2f5757fa50cb3ba536f09ad89e3727b32a38a96203`.

CP83 candidate DLL `build/candidate/bin/moproj2mst.dll`: SHA-256 `26eff7bd003ffc29550447143f0fe9aa07240c19413e3261880fa07b4c63d776`, matching the build receipt.

## Export comparison

MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` shows identical names, hints, ordinals, and RVAs for CP82 and CP83:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both report ordinal base 1, six functions and names, timestamp `FFFFFFFF`, version `0.00`, and matching section sizes including `.text=2F8000`. This is export-table equivalence, not identical implementation bytes.

## Focused public ABI tests

Ran against `build/candidate`:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed: `m2m.g0.abi`, `m2m.g0.abi_open_failure`, and `m2m.g0.abi_original_settings`; CTest exit code 0. Tests used configured disposable candidate scratch. No source, fixture, GUI, original application, or mutable REA database was changed or used.

Qualification: CP83 retains CP82's six-export surface and passes the focused public ABI tests. This does not establish original-runtime parity or an external independently compiled client's compatibility.
