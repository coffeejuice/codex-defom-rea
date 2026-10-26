# CP90 focused DLL ABI verification

## Build identity

`analysis/resume90-integrated-grouped-build.json` reports `completed` and source stable before/after at SHA-256 `bc2e716be52a0490d6e90cf703da2150c1a1cadac2a2c538957b4766290942f9`. Root reports five focused targets pass; the full suite is still running.

Frozen CP89 DLL `build/wave3-checkpoint89-verified/moproj2mst.dll`: SHA-256 `c5d4bd2fe85598b3770f45157e6c1602b11c0098f013f079dc662cad9319d89d`.

CP90 candidate DLL `build/candidate/bin/moproj2mst.dll`: SHA-256 `ac401824a4da8fe220029551b131fb69372da53213f338fad1c3078efa621435`, matching the build receipt.

## Export comparison

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` shows identical export names, ordinals, hints, and RVAs for CP89 and CP90:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both have ordinal base 1, six functions/names, timestamp `FFFFFFFF`, version `0.00`, and matching section sizes (`.rdata=0xC1000`, `.text=0x2F9000`). The DLL hashes differ; this verifies only the export table.

## Focused public ABI tests

Ran against the current candidate without rebuilding:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); CTest exit code 0. Tests use configured disposable candidate scratch. No source or fixture was modified.

## Limits

CP90 retains CP89's six observed C DLL exports and passes the three focused public ABI tests. The full suite result is not included because it is still running. This does not establish original-runtime parity or external independently compiled-client compatibility.
