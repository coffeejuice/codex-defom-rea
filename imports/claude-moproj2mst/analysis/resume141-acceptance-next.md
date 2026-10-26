# CP141 public converter lifecycle acceptance next task

## Finding

CP140 is source-stable and the historical suite remains 93 SAME / 5 DIFF (335 registered: 328 clean, six logged gaps, one logged difference). The remaining public lifecycle boundary is still materially under-tested: `tests/g0/test_abi.cpp::corpus_project_result` accepts either `M2M_OK` or `M2M_E_UNSUPPORTED` and uses flags 0, while its sequential public-call test covers only failure then failure. The full `m2m.s.save_folder` sweep reports its results diagnostically and does not assert the exported C ABI status. The public `M2M_FLAG_WRITE_SAVE` API is the closer user-facing path and already returns only after candidate Session scope unwinds.

The mode-nonzero Forming editor guard at `loader_l3.cpp`/`conversion_session.cpp` remains a genuine unresolved refusal (`0x6504abb0`); DOE_TEMP is explicitly unsupported. This next case avoids those gates by pinning `M2M_MO_INPUT_MODE=0` and selecting a retained, mode-0 heat project. It adds positive exported-API coverage without implying those unsupported branches are complete.

## Recommended executable task

Add one G0 Release-active test, `source/apps/moproj2mst/tests/g0/test_abi_write_save_sequence.cpp`, linked through the existing `^test_abi` CMake rule. It should make two sequential exported `m2m_convert_w` calls in one process:

1. Copy only `evidence/gui_mo/oracle_b/RS_08_HEAT1_before` to a unique short directory below `M2M_TEST_SCRATCH` (the configured scratch is the CTest-owned disposable area), pin `M2M_MO_INPUT_MODE=0`, set code page 1252 and GUI unit 0, and invoke the real linked DLL with `M2M_FLAG_WRITE_SAVE` and default `HEAT1.MST` output. Require exactly `M2M_OK`; retain the message buffer and complete generated tree. Compare all comparable generated files to `evidence/gui_mo/oracle_b/RS_08_HEAT1` using the existing folder-comparator exclusions and rules. This case is already `SAME` with 0 differing, missing, and extra paths in the CP140 `m2m.s.save_folder` receipt, with no comparator adjustment. The C ABI test should preserve raw hashes before any diagnostic view and report the full raw comparator result.
2. Without restarting the process, issue an existing evidence-backed `WRITE_SAVE` parse-failure call using the established disposable B/B.moproj-missing plus valid `LOC_DEN_CTRL.DAT` fixture from `tests/g0/test_abi.cpp`. Require exact `M2M_E_PARSE`, the existing `reading project` diagnostic, no MST, and byte-identical remesh input. Confirm the first call's output tree remains intact and that the CWD is restored after both calls.

The first call directly tests exported API success/status and full Save file effects against retained original bytes. The second checks that a completed successful candidate Session does not contaminate the next public conversion. It extends the existing failed-then-failed isolation case with the missing success-then-failure lifetime edge.

## Binding and limits

CP140 freeze: source tree SHA-256 `9a9eb34f0a934ffd5fde3e02b5bb6b48152121ab91f608b215cdeb820b9bf981`; `moproj2mst.dll` SHA-256 `17ad2635f4dde3fbe0fdd56c700117a6278a93b82d5719a3062269b4f109f8f5`; `deformdb.dll` SHA-256 `c89256fd4a9b226adc9e1eeeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`. Fixture input `RS_08_HEAT1_before/HEAT1.moproj` SHA-256 `e758aa107b6d8c849b1417f9f12177f70b282a4ecd6fa96d1231f6f748e30093`; retained output MST SHA-256 `4245304b4cb3941b1a8ff10d21a714d1290a696adf2834f0626ff5ae5896eec7`.

This remains candidate-versus-retained-output acceptance. It does not constitute a fresh original run, prove native GUI modal behavior, authorize public editor300/DOE/Project/Close guards, or establish original Close parity. No source/build/GUI/mutable REA action was taken in this review.
