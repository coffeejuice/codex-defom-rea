# CP92 focused DLL ABI verification

## Build identity

`analysis/resume92-integrated-grouped-build.json` reports a completed source-stable build at SHA-256 `d4b22dc3d2ac458d84c7ae230516467c3712302979395ec26c37ca969ab2eff0` before and after. Root reports five focused targets pass; the full suite is still running.

Frozen CP91 DLL `build/wave3-checkpoint91-verified/moproj2mst.dll`: SHA-256 `81d7d3a3f0d4c02d53cb9b279f016e7f354b459886c60d07e73c3050de9380c4` (also recorded by `analysis/resume91-integrated-repair-build.json`). CP92 candidate `build/candidate/bin/moproj2mst.dll`: SHA-256 `c7a875bbf3a42c97f5c98955d4545a250fe7033bc4885c992371e55d51fbacf7`, matching the CP92 build receipt.

## Export comparison

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` on CP91 and CP92 shows the same names, hints, ordinals, and RVAs:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both have ordinal base 1, six functions/names, timestamp `FFFFFFFF`, version `0.00`, and `.rdata=0xC1000`. `.text` differs (`0x2FA000` CP91; `0x2FB000` CP92), and the binary hashes differ; only the export table is unchanged.

## Focused public ABI tests

Ran against the existing CP92 candidate without rebuilding:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); CTest exit code 0. Tests use configured disposable candidate scratch. No source or fixture was modified.

## Limits

CP92 preserves CP91's six observed C DLL exports and passes the focused public ABI tests. The full-suite result is not included because it is still running. This does not establish original-runtime parity or external independently compiled-client compatibility.
