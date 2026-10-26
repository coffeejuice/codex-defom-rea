# CP72: Boolean Save failure-order regression update

## Change

Updated only `source/apps/moproj2mst/tests/s/test_save_io.cpp`:

- Renamed the locked-MST test to `save_failed_mst_open_keeps_step4_boolean_files_and_prior_save_effects` and changed it to require recognizable Boolean.BWZ and DEF_MACHDIST_CTRL2.INI contents after the existing `M2M_E_WRITE` result.
- Renamed the missing-output-parent test to `save_missing_mst_parent_keeps_step4_boolean_files` and made the same content assertions while retaining its assertion that the missing MST parent is not created.

These are still failure-path tests. They do not assert or imply that the full Save completed, that the MST was produced, or that fresh original runtime parity was observed.

## Source qualification

The recovered Save sequence calls each operation's properties `vt+0xb8` in step 4 before beginning the MST write (mdt spec §1). The Boolean properties writer `0x1411e09a0 -> 0x1411e1820` writes Boolean.BWZ and DEF_MACHDIST_CTRL2.INI (s1-04 §1.2). The retained `133_B_DBSTEP_BOOL3_before/COG_P1_B1.moproj` fixture used by the default `Rig` contains Boolean3D operation ID 2, and `Rig` observes its Task00002 side files. Therefore, when the step-4 dispatcher is integrated, failure to open the later MST must not suppress the earlier Boolean files.

Root owns the production dispatch in `source/apps/moproj2mst/src/core/convert.cpp`. This test edit presumes that CP72 dispatch is implemented before these assertions are built or run. The candidate currently invokes the Boolean side-file helper again from the Boolean MST writer; the independent step-5 call is retained. File contents in these checks prove the outputs exist at the failed conversion's return, not that the two native invocations or all open-failure cases are fully matched.

## Validation status

Static preflight only, as requested. I inspected the two modified test bodies and their fixture setup plus the retained native call-order records. I did not build or run tests, launch the GUI, modify production code or fixture data, or access a mutable REA database. No original runtime comparison is claimed.
