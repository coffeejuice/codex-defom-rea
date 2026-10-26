# CP91 final-source ABI verification after test-only repair

## Source and binary identity

This updates the earlier `analysis/resume91-final-abi-verification.md` for the final CP91 source. `analysis/resume91-integrated-repair-build.json` reports completed and source-stable before/after at SHA-256 `ee3a6df1b5503dfee1839f843f031135a2f8d5a9df1ce5b6f18707d5065929e9`.

The repair build receipt records candidate DLL SHA-256 `81d7d3a3f0d4c02d53cb9b279f016e7f354b459886c60d07e73c3050de9380c4`, identical to the pre-repair CP91 build receipt. The previous report recorded the same binary hash. The CP90 frozen DLL `build/wave3-checkpoint90-verified/moproj2mst.dll` remains SHA-256 `ac401824a4da8fe220029551b131fb69372da53213f338fad1c3078efa621435`. No separate `build/wave3-checkpoint91-verified/moproj2mst.dll` copy was present, so CP91 pre-/post-repair binary identity is established from the two build receipts' identical artifact hash, not a second-file comparison.

## Export table

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` on final `build/candidate/bin/moproj2mst.dll` and frozen CP90 reports identical six rows:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both report ordinal base 1, six functions/names, timestamp `FFFFFFFF`, version `0.00`, and `.rdata=0xC1000`. `.text` differs (`0x2F9000` CP90 and `0x2FA000` CP91). Identical export names/ordinals/hints/RVAs are established; implementation bytes are not equal.

## Focused public ABI tests

Ran against the final-source candidate without rebuilding:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); CTest exit code 0. Configured scratch is under `build/candidate/t`. No source or fixture was modified by this ABI check.

Qualification: the test-only repair changed the final source hash but not the candidate DLL hash; the six-export surface is unchanged, and focused public ABI tests pass. Full-suite status is separate. This does not establish original-runtime parity or external independently compiled-client compatibility.
