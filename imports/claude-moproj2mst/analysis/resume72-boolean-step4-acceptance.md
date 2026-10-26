# CP72: Boolean step-4 effects when MST open fails

## Expected lifecycle result

For a `WRITE_SAVE` conversion that passes the pre-write checks, reaches the Boolean operation property writer, and then fails to open its MST target for writing, the qualified candidate regression contract is:

| Observable | Expected result |
|---|---|
| `Operations/Task00002/Boolean.BWZ` | Generated Boolean output exists and contains the fixture-qualified `WZD_BOOLEAN_CUTTER_OBJECT_NO` record. |
| `Operations/Task00002/DEF_MACHDIST_CTRL2.INI` | Generated Boolean control output exists and contains `WZD_MACHDIST_OPERATION_BOOLEAN`. |
| Pre-existing MST target denied for write-open | Its bytes remain exactly `retained MST`; an open failure occurs before successful WriteOnly truncation. |
| Earlier `.moproj` Save | Its changed bytes remain; there is no project-wide rollback. |
| Public converter result | Candidate adapter returns `M2M_E_WRITE` for the MST-open failure (`cannot open MST ...`). |

These expectations are about the reconstructed converter's existing adapter contract and the retained original order. They do not claim an original DLL `M2M_E_WRITE` status: the imported original Save returns its earlier check/status local, and the mapping from that internal return to any external UI status is not qualified here.

## Original dispatch evidence

The source-qualified static sequence is in `analysis/save-semantics.md` for original `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The recovered Save entry at `0x1400980d0` calls project writer `0x1400983c0` at `0x1400981f0`, then operation property-save dispatch `0x1400982e0` at `0x140098201`, then MST writer `0x1400a20d0` at `0x14009820e`. There is no conditional branch on project-writer result between these calls. The property loop invokes vtable `+0xb8` and ignores individual return values. The Boolean property path is qualified in `analysis/resume72-boolean-step4-test.md` as `0x1411e09a0 -> 0x1411e1820`, writing Boolean.BWZ and DEF_MACHDIST_CTRL2.INI. The MST routine `0x1401313c0` opens the target with write mode and returns false on open failure.

This establishes order and writer-local false behavior, not an original runtime fault-injection result. In particular, the native Save's integer status must not be replaced with the adapter status.

## Existing public candidate tests

`source/apps/moproj2mst/tests/s/test_save_io.cpp` has the intended end-to-end regression:

- `save_failed_mst_open_keeps_step4_boolean_files_and_prior_save_effects` copies the disposable `133_B_DBSTEP_BOOL3_before` fixture, verifies the two sidefiles start absent, writes `retained MST`, then holds the MST with `FILE_SHARE_READ` so the converter cannot open it for write. It expects `M2M_E_WRITE`, both generated Boolean markers, unchanged retained MST bytes after releasing the handle, and changed `.moproj` bytes.
- `save_missing_mst_parent_keeps_step4_boolean_files` independently exercises a nonexistent output parent. It expects `M2M_E_WRITE`, no creation of the missing parent, and both Boolean markers.
- Adjacent tests cover a post-open locked-stream failure and successful MST generation; these are distinct from an MST open failure.

The fixture's `Rig` selects `133_B_DBSTEP_BOOL3_before`, with Boolean3D operation in `Task00002`; the test reads the generated outputs under that task directory. These are candidate public `m2m::convert` tests against disposable copies, not fresh-original parity tests. `analysis/resume72-boolean-step4-test.md` records the static preflight and explicitly says no build or run was performed in that test-only lane. Therefore the test source establishes the desired regression contract, but its execution must be proven by a later source-bound CTest receipt.

## Misleading status to avoid

A successful Boolean sidefile test alone does not prove the whole Save succeeded: for the denied MST case the correct candidate result is `M2M_E_WRITE`, with earlier side effects retained. Conversely, checking only `M2M_E_WRITE` is insufficient because it would pass if the step-4 Boolean writes were incorrectly skipped. The regression needs all four categories together: status, both file contents, retained MST bytes, and earlier project mutation.

The missing-parent case tests an alternate open-failure setup, but it does not prove exact Windows sharing behavior or replace the pre-existing-file preservation assertion. Neither test establishes original UI messaging, process-level failure status, or all Boolean branch variants.

## Next gate

Run the focused `m2m.s.save_io` test on the integrated source tree and retain its exact log and source/build binding. If it passes, report candidate coverage of this selected MST-open-failure ordering only; keep original-runtime and full Boolean lifecycle claims qualified. No new source, test, build, GUI, or mutable REA change was made in this audit.
