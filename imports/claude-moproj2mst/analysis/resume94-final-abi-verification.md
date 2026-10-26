# CP94 focused DLL ABI verification

## Build identity

`analysis/resume94-integrated-grouped-build.json` reports completed and source stable before/after at SHA-256 `fecc641f67098680de243c4b78d675a54d323f70221c3a07909707cada576070`. Root reports seven focused targets pass; the full suite remains in progress.

Frozen CP93 DLL `build/wave3-checkpoint93-verified/moproj2mst.dll`: SHA-256 `bbb74a2a04c0c2eb133e3fa79afc5ce4b91fb4ce7ba17deb107c30d038a615b6`.

CP94 candidate DLL `build/candidate/bin/moproj2mst.dll`: SHA-256 `820bc05f63ec6ffc79e506a29739208def581c2ab5ad86179a420bd79f9ecdd9`, matching the build receipt.

## Export comparison

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` reports identical export names, hints, ordinals, and RVAs for CP93 and CP94:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both report ordinal base 1, six functions/names, timestamp `FFFFFFFF`, version `0.00`, `.rdata=0xC1000`, and `.text=0x2FB000`. DLL hashes are different; only export-table equivalence is established.

## Focused public ABI tests

Ran against the existing CP94 candidate without rebuilding:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); CTest exit code 0. Tests use configured disposable candidate scratch. No source or fixture was modified.

## Limits

CP94 retains CP93's six observed C DLL exports and passes the focused public ABI tests. The full-suite result is not included because it is still running. This does not establish original-runtime parity or independently compiled external-client compatibility.
