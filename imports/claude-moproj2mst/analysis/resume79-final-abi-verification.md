# CP79 final DLL export and focused ABI verification

## Build qualification

Root reported the integrated grouped3 build complete. Its retained receipt `analysis/resume79-integrated-grouped3-build.json` says `status=completed`, all three steps exited 0, and the source tree remained stable at SHA-256 `22828eda881712488bad653fdcf0a2e05b2cebdfe32934974865b13922f1e14c` (3,347 files).

Frozen CP78 DLL: `build/wave3-checkpoint78-verified/moproj2mst.dll`, SHA-256 `1f344355a48f40947f4e88cffcf714645ffb6a5ef67483100cbaa5c3af139513`.

Final CP79 candidate DLL: `build/candidate/bin/moproj2mst.dll`, SHA-256 `8e4c6b8c26800bfdaec8a2fb7f70e7c92b7144be9c1ef066fff816c36cadac43`.

## Export comparison

Read-only MSVC DUMPBIN 14.44.35228.0 `/nologo /exports` showed the following raw CP79 export rows; they exactly match CP78 by name, ordinal, hint, and RVA:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Both binaries report ordinal base 1, six functions, and six names. CP79 sections include `.text=2F8000`; CP78 had `.text=2F7000`, so this is an export-surface comparison, not an implementation-binary identity claim. Both report timestamp `FFFFFFFF` and version `0.00`.

## Focused public ABI smoke

Ran existing Release CTest targets against the final candidate build:

```text
ctest --test-dir build/candidate -R "^m2m\.g0\.(abi|abi_open_failure|abi_original_settings)$" --output-on-failure
```

Result: all 3 passed, 0 failed; CTest exit code 0. Targets were `m2m.g0.abi`, `m2m.g0.abi_open_failure`, and `m2m.g0.abi_original_settings`. CTest's configured work directories and `M2M_TEST_SCRATCH` are under `build/candidate/t`; fixture inputs are read from retained corpus paths. No source, fixture, GUI, original executable, or mutable REA database was changed or used. (CTest created normal disposable scratch/test bookkeeping under the candidate build.)

## Qualification

The final CP79 DLL retains the CP78 six-symbol name/ordinal/RVA export surface, and focused public ABI behavior tests pass. This does not prove ABI compatibility of every parameter layout through an external independently compiled client, nor does it establish original-runtime parity. No further ABI change is indicated by this check.
