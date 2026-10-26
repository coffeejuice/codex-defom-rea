# CP80 final DLL ABI verification

## Build and artifacts

`analysis/resume80-integrated-grouped2-build.json` reports `completed`, all configure/preflight/build steps exit 0, and a stable source tree SHA-256 `ec108f046e509be9dcf4181be5e1cd11303f3aa0b3a9b7906a5183dca3553c57`.

Frozen CP79 DLL: `build/wave3-checkpoint79-verified/moproj2mst.dll`, SHA-256 `8e4c6b8c26800bfdaec8a2fb7f70e7c92b7144be9c1ef066fff816c36cadac43`.

Final CP80 candidate DLL: `build/candidate/bin/moproj2mst.dll`, SHA-256 `d8fb622546a60f6598f1f247ba4ec010c3f8bcae02ae4f553b2a7d9760214463`, matching the grouped2 build receipt.

## Raw exports

MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` reports identical export rows, names, hints, ordinals, and RVAs for CP79 and CP80:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both report ordinal base 1, six functions, six names, timestamp `FFFFFFFF`, version `0.00`, and the same section sizes, including `.text=2F8000`. This establishes equality of the observed export tables, not equality of DLL implementation bytes.

## Focused public ABI tests

Ran against `build/candidate`:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

All three passed: `m2m.g0.abi`, `m2m.g0.abi_open_failure`, and `m2m.g0.abi_original_settings`; CTest exit code 0. CTest used its configured disposable candidate scratch directories. No source, fixture, GUI, original application, or mutable REA database was modified or used.

Qualification: CP80 retains CP79's six-export surface and its focused public ABI tests pass. These checks do not establish original-runtime parity or an external independently compiled client's binary compatibility.
