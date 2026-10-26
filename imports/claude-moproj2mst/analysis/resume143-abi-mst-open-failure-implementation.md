# CP143 public ABI denied-MST-open test implementation receipt

## Test behavior

Added only `source/apps/moproj2mst/tests/g0/test_abi_write_save_boolean_mst_open_failure.cpp` and this receipt. No production source, public header, CMake, retained fixture, build, GUI, or mutable REA state was changed.

The test copies immutable `M2M_ORACLE_B/133_B_DBSTEP_BOOL3_before` to a unique leaf `C:\PROBLEM\m2mA143-<pid>-<tick>-<sequence>`. Before any copy or directory creation, it requires `C:\PROBLEM` to exist, refuses an existing leaf, and checks every projected copied fixture path plus explicit generated outputs are shorter than 100 characters. No ACL changes or deletion are used; scratch evidence remains for diagnosis. It sets `M2M_MO_INPUT_MODE=0` with environment restoration, pins codepage 1252, GUI unit 0, and empty deform directory. It retains the fixture's pre-existing `COG_P1_B1.MST` bytes and `.moproj` bytes, then opens only the copied MST for read with `FILE_SHARE_READ`. The held handle allows another read and denies a conversion write-open without ACL changes.

It calls the exported `m2m_convert_w(project, nullptr, M2M_FLAG_WRITE_SAVE, ...)`, so the API's documented default MST path is the locked copied `COG_P1_B1.MST`. Assertions require exact `M2M_E_WRITE`, message prefix `cannot open MST `, byte-identical retained MST, Boolean.BWZ marker `WZD_BOOLEAN_CUTTER_OBJECT_NO`, Boolean control marker `WZD_MACHDIST_OPERATION_BOOLEAN`, changed `.moproj` bytes, and restored CWD. The sharing-denial handle is released by RAII; scratch artifacts remain available for diagnosis.

## Evidence binding

- New test SHA-256: `1f6ec07f587487168b93d3efda8ab3f5af7823b1f49a23b40b3311920de94084`.
- Retained input project `COG_P1_B1.moproj`: `8e5e896cce67164430324cddc25324043449428b789d201bb3bef9c49cc89f79`.
- Retained input MST `COG_P1_B1.MST`: `07b0a85d88c31326ecd1f069d765fdc14e8f5efdc046195ed301eb014e20eb1c`.
- Current candidate comparator `tests/s/test_save_io.cpp`: `83fec33ee033409b1c0faf90aa9e15d8d82200800a0c2b3c50e55367f745cafb`; its `save_failed_mst_open_keeps_step4_boolean_files_and_prior_save_effects` exercises the same failure ordering through internal C++ `m2m::convert`, not exported ABI.
- `analysis/resume72-boolean-step4-acceptance.md`: `146873184d89a22a69adf516cafd3d699f312633a2cafbe51ed76ecc6c2890b9`; it records the original static order of project save, Boolean property step 4, then main MST writer step 5, and distinguishes candidate adapter status from original GUI status.
- Current candidate `src/core/convert.cpp`: `71fe67b9c6be7bde83001e663aa7c50f01c4a6421f357c0db893a5df0a0bd632`; `src/dll/abi.cpp`: `6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848`.

## Static preflight and limits

The fixture is copied before modification, the held file handle is limited to that copy, and all project/effect paths are checked below 100 characters under the required `C:\PROBLEM` root. `FILE_SHARE_READ` makes the denied write-open condition deterministic under Windows sharing rules, provided handle acquisition succeeds. The test refuses an existing unique leaf and does not delete files or change ACLs. No runtime test or build was run in this assignment.

This test can establish candidate DLL status/message and selected earlier/later effects for this one denied-open case after it is built and run. It cannot establish original GUI return status, modal messaging, Close parity, or general rollback semantics. CP140's five historical folder differences and unresolved Forming editor300, DOE, Boolean occurrence/owner, and remesh handler/process gates remain separate.

