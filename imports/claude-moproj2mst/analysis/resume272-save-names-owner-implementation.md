# CP272 Save names owner implementation

The four-field Save commit now belongs to GuiModel::storeOperationSimulationNames. storeSimNames retains active-subproject lookup, operation-name copy, flow lookup, complete simEntries construction, first match and unnamed refusal, then model lookup. Only its final immediate control lambda was replaced.

## Contract and review

The command calls simCtl exactly once, returns for absent control, and remains potentially throwing. It preserves operation local8 conversion/strncpy(0x40), CURSIM[1], simulation local8 conversion/strncpy(0x50), CURSIM[0] in that order. No conversion is moved before admission, no references/control aliases are retained and no rollback/terminating helper is introduced. Retained static qualification evidence/gui_mo/out_scope/opsave3.c lines 45–78 SHA-256 3815a4d7c22943db590786fae008231cbe4f3ced9c299d40770965c767399361 was re-read; this supplies ordering evidence, not fresh original execution.

Direct owner tests independently assemble expected bytes for short (7/9), exactly bounded (64/80), and overlong (73/91) ASCII names, compare the entire step representation with only four expected changes, preserve sentinel bytes at 0x40/0x50 and step identity, and verify detached step no-op/no recreation. Existing helper scope test remains. The existing parked geometry invariant invokes the new command with and without its step and checks refusal plus existing surrounding snapshots.

Real MultiBlow Save tests independently find the first flow-matching named simulation entry, seed stale name fields/CURSIM values, execute saveCurrentOperation, and assert complete name arrays plus both integer values alongside the existing PDIE and KEY/deletion checks. The three fixture test cases now require Pass rather than accepting Skip. No operation names or historical expected fixtures changed.

## Focused validation

Source mutation began only after root released its CP269–271 snapshot lock. Root serialized the build slot while the separate scene worker paused stable edits.

Command: python admin/build_candidate.py --label resume272-save-names-owner --target m2m_s_save_current --target m2m_mdt_multiblow_save --target m2m_mdt_viewer_box. Configure and build exited 0; build command 22.485 s.

Exact configured CTests were run with verbose output and output-log build/logs/resume272-save-names-owner-ctest.log: m2m.s.save_current, m2m.mdt.multiblow_save, m2m.mdt.viewer_box. All 3/3 passed in 5.48 s; internal counts 15, 4 and 65 cases respectively, zero failed. Save-current corpus reports 19 oracle cases passed/0 skipped and 56 corpus saves.

All three real MultiBlow callers executed their SAVE_NAMES assertion receipt and passed: 93_B_MB3ACT (11 files, 2 deleted), 93D_B_MB3ACT_DWELL (11 files, 2 deleted), 94_B_MB2ACT (14 files, 2 deleted). Thus no skip substitutes for required caller coverage. git diff --check passed.

## SHA-256 receipts

Paths below are relative to this import checkout. Hashes describe the files at completion of this focused lane; later root edits/builds require new receipts.

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/include/m2m/engine.hpp | d605f403aaae40bc869458e2f28a8c223ce0b084c18fdc82edea00a1be6ebcab |
| source/apps/moproj2mst/src/engine/gui_model.cpp | 802a9d7e3aa67da77fd22b15ccef73e001a6c4208fe1141ccc8200d456ea76af |
| source/apps/moproj2mst/src/engine/save_current.cpp | d1b7b59c3f40ffaac8f3792c082ab98d44adfed810824e840a5c0fc6db5fbd24 |
| source/apps/moproj2mst/tests/s/test_save_current.cpp | 97790e33d7fec3b672245fc59d85c9b780c5aebf9654c756a3efa60292e37ef0 |
| source/apps/moproj2mst/tests/mdt/test_multiblow_save.cpp | 7ca01fab6900a2cfa1b1786c093dfd42e186a72683aa4739589590a0e12dd4ff |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 1c287170d852aeb1870469c250c151fef38bb619ec1942df7dd261bb7d721963 |
| build/logs/resume272-save-names-owner-configure.log | df0fc19ee2286fd32a2995579b04587724380ffd597a731d310c47c89d87e00f |
| build/logs/resume272-save-names-owner-build.log | c93736c5eea7c2a5503be0696fc3174981daa37e9587a748626b3a76ab072e00 |
| build/logs/resume272-save-names-owner-ctest.log | 08a30783b1df731c1b63126460cd65dd4a4c6a84af620db9f033143fa2076d37 |

## Limits and handoff

No full regression, candidate freeze, shared recovery-state update, GUI, original process, mutable REA operation, DMO build or reconstructed DEF_PRE executable build/deployment was performed in this lane. Focused binaries include the already-stable concurrent scene source edits; root owns aggregate full regression/freeze. Final states establish resulting fields; intermediate conversion interruptions and every native partial-store prefix remain static-source-established. RIO, RCLOSE, RACCEPT and historical DIFF qualifications are unchanged.

Build slot was released to root immediately after focused tests. Exclusive source ownership is released with this report.

## Independent review HOLD completion

The two test-only requirements in analysis/resume272-save-owner-implementation-review.md are implemented after root explicitly released its completed full-build lock. No production code changed.

A standalone viewer-box case store_operation_simulation_names_parked_owner_preserves_step now uses the established GeometryDirectoryFixture/GeometryDirectoryCandidate low-version record plus node-gate fixture. After actual viewerRefresh parks the model and its dispatch is required, the test snapshots the entire step immediately before the new command. It requires M2M_E_UNSUPPORTED with the exact existing parked-query message, unchanged step identity, and identical full snapshot after refusal. This directly exercises this command rather than relying only on shared barrier coverage.

The real MultiBlow Save assertion now requires model->simCtl() == control immediately after Save, before any post-Save dereference of the borrowed control.

Focused rebuild command used admin/build_candidate.py --label resume272-save-names-owner-review-fix with the same three explicit --target arguments. Configure/build exited 0; build command 4.859 s. The same exact three configured CTests passed 3/3 in 5.37 s; viewer-box now reports 0 of 66 cases failed and explicitly logs the new parked owner case. All 93/93D/94 SAVE_NAMES and PASS lines remain present; no skip accepted. No full suite/freeze was run in this lane.

The following hashes supersede earlier table rows for these changed tests; the earlier build receipts remain historical.

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/tests/mdt/test_multiblow_save.cpp | a1e400303f9fbc1df8e111985887c7853cf56b43b5a084e64d062f01029f67f2 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 05cb19702b817a83e318f0970a178a61a66fa5397450b62134f079a99c86e63c |
| build/logs/resume272-save-names-owner-review-fix-configure.log | 88f89ca55b19e4646b916c434bd2b341a665b5d04815051a280ce324388c52db |
| build/logs/resume272-save-names-owner-review-fix-build.log | ec93817b064f89c487ef85f8ea562d0de18257d530c627ed0b9c121184ad1ea9 |
| build/logs/resume272-save-names-owner-review-fix-ctest.log | 87f277299613cd4da7dc071fced74e0112773c63afc26fdf1577f1a2390030cc |
