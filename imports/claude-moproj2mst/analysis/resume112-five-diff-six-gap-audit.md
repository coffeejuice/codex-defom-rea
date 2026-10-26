# CP112: five historical differences and six logged gaps

## Baseline and conclusion

The latest completed full-suite evidence is CP110, `wave3-checkpoint110-validation.json` and `resume110-handler-model-repair1-tests.json`. It binds 293 registered tests to source SHA-256 `300cf2f784365a4e858b2782a84a274777b12d37f4591041dbfc46ed4adc09d1`; CTest exited 0 with 286 ordinary passes, six suites marked passed-with-logged-gaps, and one suite marked passed-with-logged-differences. The historical folder sweep remains 93 SAME and five DIFF. The receipt says there were no fixture changes.

These counts describe the retained comparisons and test run. `s.save_folder` logs rather than fails on folder differences, and its folder comparator normalizes declared view/banner profiles. Therefore the five row results must remain labeled as raw historical DIFFs; they are not proof of raw byte identity or fresh original Open-to-Save acceptance. CP110 also records that the original executable was not launched.

## Historical DIFF reconciliation

| Case | CP110 retained result | Current qualification |
|---|---|---|
| A: `RS_55_Multiple_Operations_Example1` | One differing file, no missing/extra files. | Retained narrow `SimCtrl.KEY` evidence identifies two bytes (offsets 1439 and 1455) and line 52 values `0,0,0,0` candidate versus `0,1,0,1` original. A CG-toggle callback can explain those values, but fresh Open delivery is unproven. Keep as a real retained raw difference with a runtime trigger unresolved. |
| B: `120_B_DOE` | One differing `.moproj`, seven missing DOE outputs. | Before tree lacks the study that appears in the post-GUI output. This is an action-state mismatch, not an equivalent-state Save comparison. CP110's newer DOE prefixes do not create or prove the missing full state. |
| B: `121_B_DOE_VAR` | One differing `.moproj`, eight missing outputs, including `temp.KWD`. | Before tree lacks the study/variable created before the retained expected output. CP75-qualified scalar prefix work narrows the native sequence, but delivery/normal continuation and a complete public read remain unqualified. Preserve the difference and its action-state qualification. |
| B: `122_B_DOE_GEO` | One differing `.moproj`, ten missing files, including seven DOE outputs and three GEO files. | Before tree lacks the study, geometry variable, and DOE Output operation present after GUI actions. CP104 geometry work is only a prefix. This remains a post-action state mismatch and is not evidence for inventing those objects during ordinary Open. |
| B: `98_B_DS3F` | One differing MST file, no missing/extra files. | The raw line embeds an absolute `KFREAD` path rooted in the historical workspace. A separate focused candidate test checks an exact path substitution while requiring all other bytes to match. This does not erase the raw difference; retain both raw output and the separate transformed comparison. The extra B98 regression is stale as a proposed next test because that focused test already exists. |

The earlier A55 callback explanation is a hypothesis, not a settled cause. Likewise, CP110's improved DOE prefixes change the available candidate behavior but do not make the three before/after trees equivalent. No DIFF should be removed or normalized based on those developments.

## Six logged-gap reconciliation

CP110's test JSON still counts these six suites as passed-with-logged-gaps. The exact logs under `build/test-runs/1792909588199089500/` confirm the following:

| Suite | CP110 evidence and disposition |
|---|---|
| `m2m.mdt.mdt_write` | Log retains an active-cogging refusal and skips the optional imported PACK header check because `M2M_INSTALL_FIXTURE` is unset. The installed `PACK/pack.dat` was independently verified in CP102 at 11 bytes, text `VERSION 2`, SHA-256 `f82967bf8117671aad02e8621549adbb6caf1e5265c55818ec806d9d6b250c10`, with identical root/2D/3D copies. The environmental fixture binding remains open; the refusal is a distinct expected branch. |
| `m2m.mdt.save_l6` | Still logs `SKIP 87_B_HTFF: no oracle B`. The retained exact pre-save state is absent. A related corpus or output tree cannot substitute for the required input/action state. |
| `m2m.moproj.deflate` | Still skips optional `M2M_DEFLATE_VECTORS`. Recompressing retained project blobs is useful Qt compatibility evidence, but it is not an independent input/output vector corpus. |
| `m2m.moproj.props_l2` | Five unsupported class fixtures remain skipped and their schemas are still unported. However, the old CP102 note about the summary counter is stale: current source increments `skipped` on open failure, and CP110 log correctly says `95 operations, 900 values, 0 failures, 5 cases skipped`. Keep the class-coverage gap; drop the accounting-defect claim. |
| `m2m.p07.corpus` | Still logs five skips: two retained Oracle A pairs absent from the corpus copy and three cases explicitly A-excluded. CP102 identifies exact retained sample pairs for the two omissions; a two-case targeted qualification remains possible, but would not close the full corpus gap or cover the three excluded cases. |
| `m2m.s.save_current` | Still omits `.moproj` comparison for R46, 83_R47, 59_B_HT3, 60_B_HT2, and 62_B_BM2 because there is no input snapshot before the GUI edit/appended operation. Side-file comparisons still run. The missing state snapshots remain a real qualification. |

Thus the stale claims are limited: `props_l2` skip accounting is fixed in CP110, while the five unsupported classes remain; the recommendation to create another B98 path test is superseded by its existing focused test. The other five gap categories remain in the current CP110 receipt, although some have bounded candidate-side ways to reduce a subset of the gap.

## Selected next raw-output test

Use the already-authored, exclusive `source/apps/moproj2mst/tests/s/test_session_b109_raw.cpp` as the next focused raw comparison. It calls public `convert(..., M2M_FLAG_WRITE_SAVE, 1252)` on an isolated copy and compares `Operations/Task00003/Forming.FWZ` byte-for-byte against the retained B109 output. The bound oracle files are 1,071 bytes (`6ff1aa1952526905b741dda3e970f46543b1bd5234250b079fd9084d76929c44`) and 1,098 bytes (`35ae7b62904990d976f815235f7e057cfd26dbc258d1d86700175ad4568976c9`). It does not normalize numbers, paths, views, banners, or line endings, and it requires the output to change, so a no-op cannot pass.

This test is not part of the CP110 293-test run: its source hash is `65924fe049e89f1d90de18d19e3e48b79cc0f5a6b33502325c8960c9d5999dad`, while CP110 binds a source tree of 3,429 files at the earlier source hash above. The test file matches the existing `tests/*/test_*.cpp` CMake discovery pattern. The concrete next action is to include this one new test in the next frozen source-stable build and run it, then record its raw result separately from any normalized folder sweep. Its fixture provenance is a retained GUI-saved B109 output, not a fresh original-session reproduction; it does not claim normal Close.

No source, build, fixture, GUI, original executable, or mutable REA state was changed or run for this audit.
