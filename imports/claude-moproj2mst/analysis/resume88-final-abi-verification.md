# CP88 final-source DLL ABI verification

## Final build identity

This receipt supersedes `analysis/resume88-final-abi-verification.md`, which described the earlier candidate before the production `viewer.cpp` order repair. The final post-repair build receipt is `analysis/resume88-integrated-order-repair-build.json`: `completed`, source stable before/after at SHA-256 `eacfe403abee631861befe6f9f35b274c9be1928af0e52f95e6e1e671c54cfef`.

Frozen CP87 DLL `build/wave3-checkpoint87-verified/moproj2mst.dll`: SHA-256 `593062bf574437bfa6819e1ac2603a7fb24c6dca83c8ee44b90d999bd5accf9`.

Final-source CP88 candidate `build/candidate/bin/moproj2mst.dll`: SHA-256 `eed0f155561e8c95c46a77a046fcd3deb2ef23ac1770b767c05c3d76ab8401cd`, matching the order-repair build receipt.

## Export comparison

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` reports identical names, hints, ordinals, and RVAs for frozen CP87 and the final-source CP88 candidate:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both report ordinal base 1, six functions/names, timestamp `FFFFFFFF`, version `0.00`, and matching section sizes including `.text=2F8000`. This establishes export-table equivalence only; the DLL hashes differ.

## Focused public ABI tests

Ran against the final-source candidate, without rebuilding:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); CTest exit code 0. Configured test scratch is under `build/candidate/t`. No source or fixture was modified by this check.

## Acceptance status and limits

The exports match CP87, and focused public ABI tests pass for the final source hash. The full suite is still running, so no full-suite result is claimed here. This check does not establish original-runtime parity or external independently compiled-client compatibility.
