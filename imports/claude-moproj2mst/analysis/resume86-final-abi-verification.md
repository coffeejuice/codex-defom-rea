# CP86 final DLL ABI verification

## Build identity

The retained `analysis/resume86-integrated-grouped2-build.json` says `completed`, source-stable before/after at SHA-256 `828b26e6b5aff32844ded203ea7c38152be2bb795a252aee1aae91466a5bff36`; candidate DLL hash recorded there is `8b79c7be7b782fc6afa532c496a5274030779ab78538fd01bd6594958f34b4d8`. The full-run summary supplied with the task is 249 registered, 242 clean, six gaps, one difference, no skips.

Frozen CP85 DLL `build/wave3-checkpoint85-verified/moproj2mst.dll`: SHA-256 `91fac3ea59a53ed929f3a9d27f1cf3b832a3e0e71088be7d8799ea81ed279ebf`.

CP86 candidate `build/candidate/bin/moproj2mst.dll`: SHA-256 `8b79c7be7b782fc6afa532c496a5274030779ab78538fd01bd6594958f34b4d8`, matching the build receipt.

## Export comparison

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` shows CP85 and CP86 have identical names, ordinals, hints, and RVAs:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both report ordinal base 1, six functions and names, timestamp `FFFFFFFF`, version `0.00`, and same section sizes including `.text=2F8000`. This confirms export-table equivalence only; DLL hashes differ and implementation bytes are not claimed identical.

## Focused public ABI tests

Ran against the existing CP86 candidate build, with no rebuild:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed (`m2m.g0.abi`, `m2m.g0.abi_open_failure`, `m2m.g0.abi_original_settings`); CTest exit code 0. The tests use configured disposable candidate scratch. No source, fixture, GUI, original application, or mutable REA database was modified or used.

## Limits

The export comparison verifies the six observed C DLL export entries. Focused tests cover the public adapter cases represented by those tests; they do not establish original-runtime parity, all calling-convention/type-layout behavior for an independently compiled external client, or successful repeated-call cross-project lifecycle parity.
