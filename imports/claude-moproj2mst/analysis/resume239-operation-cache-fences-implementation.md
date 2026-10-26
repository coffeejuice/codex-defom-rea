# CP239 Operation cache-presence implementation

## Change

Replaced the four staged cleanup reappearance checks in `source/apps/moproj2mst/src/model/operation_info_cleanup.cpp` with `operation.hasEngineModel()` at their original positions. Existing refusal messages and stage ordering are unchanged. No assertions were missing, so `tests/mdt/test_operation_info_cleanup.cpp` was not modified.

The earlier engine-cache observation/weak-owner state, free express helper, model headers, and all other source were left unchanged.

## Validation

- Focused candidate build: `python admin/build_candidate.py --label cp239-operation-cache-fences --target m2m_mdt_operation_info_cleanup` — passed, exit code 0. No `--then-full` option was used. Receipt: `analysis/cp239-operation-cache-fences-build.json`.
- Registered CTest: `ctest --test-dir build/candidate --output-on-failure -R '^m2m\\.mdt\\.operation_info_cleanup$'` — passed, 1/1 (`m2m.mdt.operation_info_cleanup`).

## Source hashes

- `source/apps/moproj2mst/src/model/operation_info_cleanup.cpp`: SHA-256 `32F02591C4045D23FA6EC245ADFA7DBF5FA3C2E67C0274BC090E446A7FDFEA8C`
- `source/apps/moproj2mst/tests/mdt/test_operation_info_cleanup.cpp` (unchanged): SHA-256 `EABC92972CA92B10E1126E8FEAA5CB609613AFE189CC280CC0ABDFE1F95B5D46`

## Limits

This verifies only the focused adapter target and its registered test. It is not a source-stable grouped regression, candidate freeze, original-application witness, native cleanup completion, or public Close acceptance. The member query preserves the existing stored-pointer presence semantics; this change makes no claim about original shared-pointer representation or a native member API.
