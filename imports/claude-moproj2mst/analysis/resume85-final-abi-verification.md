# CP85 final DLL ABI verification

## Build and DLL identities

`analysis/resume85-integrated-grouped-build.json` records a completed source-stable build at SHA-256 `48d7798c0d97342a288a1c11561ac5eabdd4f3a2860a211d949ea7e8250ab567` before and after. Root reports the exact full-suite retry passed: 248 registered, 241 clean, six gaps, one diagnostic difference; the first attempt's folder IPC infrastructure failure remains separately classified under audit.

Frozen CP84 DLL `build/wave3-checkpoint84-verified/moproj2mst.dll`: SHA-256 `44bb9f6de99d23843676915a41fcf7d4be10dfd6ee94b63f3660978a1edb098e`.

CP85 candidate DLL `build/candidate/bin/moproj2mst.dll`: SHA-256 `91fac3ea59a53ed929f3a9d27f1cf3b832a3e0e71088be7d8799ea81ed279ebf`, matching the build receipt.

## Export comparison

MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` shows identical names, hints, ordinals, and RVAs for CP84 and CP85:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both have ordinal base 1, six functions/names, timestamp `FFFFFFFF`, version `0.00`, and matching section sizes including `.text=2F8000`. This establishes export-table equivalence only.

## Focused public ABI tests

Ran against `build/candidate`:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, and `m2m.g0.abi_original_settings`); CTest exit code 0. Tests used configured disposable candidate scratch. No source, fixture, GUI, original application, or mutable REA database was modified or used.

Qualification: CP85 retains CP84's six-export surface and passes the focused public ABI tests. This is not an original-runtime parity claim or an independently compiled-client compatibility test.
