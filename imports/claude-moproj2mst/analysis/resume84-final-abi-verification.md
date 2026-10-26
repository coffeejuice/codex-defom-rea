# CP84 final DLL ABI verification

## Build and DLL identities

`analysis/resume84-integrated-grouped-build.json` reports `completed` and a source-stable build at SHA-256 `a516dd460fec17492a8a86b842b02f04c89835494a790a8b6a2c61b21f19f3a4` before and after.

Frozen CP83 DLL `build/wave3-checkpoint83-verified/moproj2mst.dll`: SHA-256 `26eff7bd003ffc29550447143f0fe9aa07240c19413e3261880fa07b4c63d776`.

CP84 candidate DLL `build/candidate/bin/moproj2mst.dll`: SHA-256 `44bb9f6de99d23843676915a41fcf7d4be10dfd6ee94b63f3660978a1edb098e`, matching the build receipt.

## Export comparison

MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` shows identical export names, hints, ordinals, and RVAs for CP83 and CP84:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both report ordinal base 1, six functions and names, timestamp `FFFFFFFF`, version `0.00`, and matching section sizes including `.text=2F8000`. This verifies export-table equivalence only.

## Focused public ABI tests

Ran against `build/candidate`:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); exit code 0. Tests use configured disposable candidate scratch. No source, fixture, GUI, original application, or mutable REA database was modified or used.

Qualification: CP84 retains CP83's six-export surface and passes focused public ABI tests. This does not establish original-runtime parity or compatibility of an independently compiled external client.
