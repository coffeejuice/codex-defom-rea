# CP143 next public ABI acceptance test

## Priority

The next high-value increment should bridge an existing candidate failure-order test to the exported C ABI: run Full Save on a copied `133_B_DBSTEP_BOOL3_before` project while denying write-open to its pre-existing default MST. The internal `m2m::convert` test already proves the candidate order on this fixture, but no existing G0 test requires the DLL entry to return the exact failure while preserving earlier Save effects. CP142 adds a public clean-save path and success-then-failure isolation; this complements it with a public Save failure at the later MST open.

## Proposed new test and fixture

Create `source/apps/moproj2mst/tests/g0/test_abi_write_save_boolean_mst_open_failure.cpp` only. Existing CMake glob discovers `tests/g0/test_*.cpp`, and `^test_abi` links the test to `moproj2mst` plus `testkit`. No production or CMake change is needed.

Use `M2M_ORACLE_B/133_B_DBSTEP_BOOL3_before` as immutable source, copied recursively into a unique directory returned by `m2m_test::scratchOutput(m2m_test::scratchRoot() / ...)`. The fixture contains `COG_P1_B1.moproj`, `COG_P1_B1.MST`, and the `Task00002` SimCtrl/InterObject/ObjProperties inputs. Its `Task00002/Boolean.BWZ` and `DEF_MACHDIST_CTRL2.INI` outputs are absent, as required by the existing `Rig` test. Before conversion, save the copied `.moproj` bytes and `COG_P1_B1.MST` bytes. Open the copied MST with `CreateFileW(..., GENERIC_READ, FILE_SHARE_READ, ..., OPEN_EXISTING, ...)` and keep that handle alive across `m2m_convert_w`; this allows reading but denies the converter's later write-open. Verify handle acquisition itself before interpreting any result.

Pin `M2M_MO_INPUT_MODE=0` with RAII restoration, set codepage 1252 and GUI unit 0, then call:

`m2m_convert_w(project.c_str(), nullptr, M2M_FLAG_WRITE_SAVE, message, messageChars)`.

The null output path is essential: the public API contract selects `<project folder>/<project base>.MST`, the same pre-existing file being locked. The test should require exact `M2M_E_WRITE` and a message beginning `cannot open MST `. While the handle is still open, assert that the copied MST retains exact pre-call bytes and that Task00002 `Boolean.BWZ` includes `WZD_BOOLEAN_CUTTER_OBJECT_NO` and `DEF_MACHDIST_CTRL2.INI` includes `WZD_MACHDIST_OPERATION_BOOLEAN`. Assert the `.moproj` bytes changed, matching the already tested earlier project Save effect. Check current directory before and after conversion. Release the lock by RAII before any later file inspection. Keep all writes under configured test scratch and do not remove evidence on a failed assertion.

## Basis and source binding

- CP142 test `source/apps/moproj2mst/tests/g0/test_abi_write_save_sequence.cpp` covers exact ABI `M2M_OK` on the retained RS_08_HEAT1 project then an exact parse failure in the same process; it does not test failure after earlier Save writers.
- `tests/s/test_save_io.cpp::save_failed_mst_open_keeps_step4_boolean_files_and_prior_save_effects` is the closest candidate oracle: it holds the MST with `FILE_SHARE_READ`, requires `M2M_E_WRITE`, checks both Boolean outputs, verifies the locked MST remains `retained MST`, and checks prior `.moproj` mutation. It exercises internal `m2m::convert`, not the exported C ABI.
- `tests/s/test_save_io.cpp::Rig` chooses `133_B_DBSTEP_BOOL3_before`, observes Boolean outputs in Task00002, requires both absent initially, and targets `native-output.MST` to avoid altering the fixture's own MST. The public test should copy the folder and lock its existing `COG_P1_B1.MST` instead.
- `analysis/resume72-boolean-step4-acceptance.md` and `analysis/resume72-boolean-step4-test.md` bind the original static Save ordering: project Save (step 3), operation property dispatch (step 4, including Boolean.BWZ and DEF_MACHDIST_CTRL2.INI), then main MST writer (step 5). The original writer returns false on MST open failure, but no external original GUI/runtime status is established.
- `src/core/convert.cpp` records candidate Full Save dispatch and maps main-MST open failure to adapter `M2M_E_WRITE`; `src/dll/abi.cpp::m2m_convert_w` forwards the `Result` code and message. The exported path therefore has a concrete expected candidate result, distinct from any native GUI return.

Fixture hashes: source `.moproj` SHA-256 `8e5e896cce67164430324cddc25324043449428b789d201bb3bef9c49cc89f79`; source `COG_P1_B1.MST` SHA-256 `07b0a85d88c31326ecd1f069d765fdc14e8f5efdc046195ed301eb014e20eb1c`. The current inspected candidate files were `test_save_io.cpp` SHA-256 `83fec33ee033409b1c0faf90aa9e15d8d82200800a0c2b3c50e55367f745cafb`, `convert.cpp` SHA-256 `71fe67b9c6be7bde83001e663aa7c50f01c4a6421f357c0db893a5df0a0bd632`, and `abi.cpp` SHA-256 `6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848`. The retained evidence does not prove a fresh original failed-open run.

## Claim boundary and queue status

A passing test would establish candidate exported-ABI return/message and selected earlier/later file effects under a deterministic denied MST open. It would not establish original GUI status, modal feedback, native Close parity, rollback semantics outside these asserted files, or complete DOE/Boolean/public editor coverage. It does not resolve any of CP140's five historical raw DIFFs: A55 and B98 have one differing file each; B120/B121/B122 each have one differing file and respectively 7/8/10 missing files. CP140's 335 registered result (328 clean, six logged gaps, one logged difference) is not objective completion. Current queue still holds public Forming editor300, DOE Save, Boolean owner/occurrence, and remesh handler/process boundaries; no public Close admission is established.

This is a proposed next task only. No source, test, build, GUI, or mutable REA action was taken for this audit.
