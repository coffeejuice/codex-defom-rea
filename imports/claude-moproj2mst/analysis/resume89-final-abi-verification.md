# CP89 focused DLL ABI verification

## Build identity

`analysis/resume89-integrated-grouped-build.json` reports `completed`, source stable before/after at SHA-256 `2133579882cf4daf3cdf350df3da834756c31b1c0798bfa779b1105975fbc201`. Root reports six focused targets pass; the full suite is still running.

Frozen CP88 DLL `build/wave3-checkpoint88-verified/moproj2mst.dll`: SHA-256 `eed0f155561e8c95c46a77a046fcd3deb2ef23ac1770b767c05c3d76ab8401cd`.

CP89 candidate DLL `build/candidate/bin/moproj2mst.dll`: SHA-256 `c5d4bd2fe85598b3770f45157e6c1602b11c0098f013f079dc662cad9319d89d`, matching the build receipt.

## Export comparison

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` output shows identical names, hints, ordinals, and RVAs:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both have ordinal base 1, six functions/names, timestamp `FFFFFFFF`, and version `0.00`. Section sizes differ: CP88 `.rdata=0xC0000`, `.text=0x2F8000`; CP89 `.rdata=0xC1000`, `.text=0x2F9000`. The DLL hashes differ, so only the export-table surface is established as unchanged.

## Focused public ABI tests

Ran without rebuilding:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); CTest exit code 0. Tests use disposable configured candidate scratch. No source or fixture was modified.

## Limits

CP89 retains CP88's six observed C DLL exports, and focused public ABI tests pass. The full suite result is not included because it is still running. This does not prove original-runtime parity or independently compiled external-client compatibility.
