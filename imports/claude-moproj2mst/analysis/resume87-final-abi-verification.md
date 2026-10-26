# CP87 focused DLL ABI verification

## Build identity

`analysis/resume87-integrated-grouped2-build.json` reports a completed source-stable build at SHA-256 `0571ad83376255d050f68320fb5650baaa5869d31b1809feb9141ea5312d4abd` before and after. Frozen CP86 DLL `build/wave3-checkpoint86-verified/moproj2mst.dll` has SHA-256 `8b79c7be7b782fc6afa532c496a5274030779ab78538fd01bd6594958f34b4d8`. CP87 candidate `build/candidate/bin/moproj2mst.dll` has SHA-256 `593062bf574437bfa6819e1ac2603a7fb24c6dca83c8ee44b90d999bd5accf9a`, matching the build receipt.

## Export comparison

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` output gives the same rows for CP86 and CP87:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both report ordinal base 1, six functions and names, timestamp `FFFFFFFF`, version `0.00`, and matching section sizes including `.text=2F8000`. The DLL hashes differ; only export-table equivalence is established.

## Focused public ABI tests

Ran without rebuilding:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); CTest exit code 0. Scratch uses configured disposable paths under `build/candidate/t`. No source or fixture was modified. The separately running full suite is not included in this focused receipt.

## Limits

This check shows CP87 preserves CP86's six observed C DLL exports and passes the three focused adapter tests. It does not establish original-runtime parity, an external independently compiled client's compatibility, or successful repeated-call cross-project lifecycle parity.
