# CP82 final DLL ABI verification

## Build and DLL identities

`analysis/resume82-integrated-grouped-build.json` reports `completed`, all configure/preflight/build steps exit 0, and a stable source hash before/after: `ece69e47f000b2d09b5880a61b1190d81a3abfd176a32688ce1685c281ee2f67`.

Frozen CP81 DLL `build/wave3-checkpoint81-verified/moproj2mst.dll`: SHA-256 `8c13bb2929f4d5c9e7a2f3a1503a95fd9e10742283aebe5a2b0ffbcca4c20cc9`.

CP82 candidate DLL `build/candidate/bin/moproj2mst.dll`: SHA-256 `233154218e6e1dd981c9df2f5757fa50cb3ba536f09ad89e3727b32a38a96203`, matching the build receipt.

## Export comparison

MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` shows identical name/hint/ordinal/RVA tables for CP81 and CP82:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both report ordinal base 1, six functions and names, timestamp `FFFFFFFF`, version `0.00`, and matching section sizes including `.text=2F8000`. The export table is unchanged; this does not claim identical implementation bytes.

## Focused public ABI tests

Ran against `build/candidate`:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); exit code 0. The tests use configured disposable candidate scratch. No source, fixture, GUI, original application, or mutable REA database was changed or used.

Qualification: CP82 retains the CP81 six-export surface, and focused public ABI tests pass. This does not establish original-runtime parity or compatibility of an independently compiled external client.
