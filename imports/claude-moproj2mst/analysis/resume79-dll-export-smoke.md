# CP79: frozen/current DLL export and C API smoke check

## Inputs and hashes

The frozen CP78 binary is `build/wave3-checkpoint78-verified/moproj2mst.dll`, SHA-256 `1f344355a48f40947f4e88cffcf714645ffb6a5ef67483100cbaa5c3af139513`. This matches the DLL artifact hash recorded in `analysis/resume78-integrated-grouped2-build.json`.

The current candidate DLL, identified for this task as the CP79 pre-barrier-repair build, is `build/candidate/bin/moproj2mst.dll`, SHA-256 `c3c4757a84087d19628b2bc1b3618d6f1482b5c242f2d875bbe8e0833c78e0ce`. The CP78 build receipt does not identify this different hash; final CP79 output must be rehashed and checked after the barrier repair/rebuild.

Both export dumps were produced read-only with MSVC DUMPBIN 14.44.35228.0 (`/nologo /exports`). The raw export rows are identical:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

Each reports ordinal base 1, six functions, and six names. Both also report timestamp `FFFFFFFF` and version `0.00`. Section summary is identical except `.text`: CP78 `2F7000`, current candidate `2F8000`; therefore identical exports do not mean identical implementation bytes. Exact names, ordinals, and RVAs match across these two artifacts.

## Existing C API smoke binary

An existing executable was present at `build/candidate/bin/capi_smoke.exe`, SHA-256 `2e1a6dc1dc18f64af9ad8ad51166a3776ca49640210ea7e12525adb8387fdee6`. Its source `build/wave2-geometry-source/deformdb/tests/capi_smoke.cpp` opens the supplied DB and invokes only read/query/free/close API calls; it does not create, update, or delete fixture files. No build or source change was made.

Command, run against the retained fixture `evidence/test-rea/COG_P1_B1/COG_P1_B1.DB`:

```text
build/candidate/bin/capi_smoke.exe evidence/test-rea/COG_P1_B1/COG_P1_B1.DB 1
```

Exit code: 0. Output reported 32 entries; step 1 time 0.1, 492 nodes, 1945 elements, one object; Workpiece had 492 nodes, 1945 elements, 7780 connectivity values, first node `(-1000, -98.6549, -229.641)`. The program printed `C API smoke test passed`. This validates the separate deformdb C API smoke path; it is not a test of moproj2mst.dll entrypoint calls or the six DLL exports.

## Qualification / next check

The current candidate and frozen CP78 DLL expose the same six name/ordinal pairs. This is a pre-barrier-repair comparison only. After CP79 rebuild, repeat `/exports` on the exact final DLL and rehash it; rerun the focused `m2m.g0.abi`, `m2m.g0.abi_open_failure`, and `m2m.g0.abi_original_settings` executables against that build. The smoke test above cannot replace those DLL ABI tests. No source, build, GUI, original application, or mutable REA database was changed or run.
