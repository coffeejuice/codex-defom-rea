# CP147 logged-gap audit and next bounded increment

## Evidence boundary

The latest bound run is `analysis/resume147-tests.json`, paired with `analysis/wave3-checkpoint147-validation.json`. It reports 353 registered tests: 346 clean passes, six `passed-with-logged-gaps`, one `passed-with-logged-differences`, no failed/skipped/incomplete tests, stable source/binary hashes, and no fixture changes. The test runner itself says the historical oracles are not fresh-original acceptance, `m2m.s.save_folder` is diagnostic and does not fail on differences, and the comparator normalizes declared view/banner profiles. Thus these entries describe gaps in this test run, not by themselves public converter behavior or correctness.

## Six logged-gap suites

| Suite | Logged gap | Qualification / smallest interpretation |
|---|---|---|
| `m2m.mdt.mdt_write` | Optional imported `PACK/pack.dat` header check skipped because `M2M_INSTALL_FIXTURE` is unset. | An environment/fixture omission, not evidence that public writing fails. Supply the already-authorized imported installation fixture to the check if available; otherwise acquire/qualify that fixture before claiming coverage. |
| `m2m.mdt.save_l6` | `87_B_HTFF` skipped: no oracle B. | The missing comparison is specifically a reference artifact. Source packet `source/docs/re/moproj2mst/mdt/L6/spec.md` already identifies loader slots and decompiles for HeatTreat, Distortion, Diestress and DiestressBM, states there is no oracle B, and enumerates B cases 98–102 for L6 Save. This is the source-ready bounded increment: choose one existing simple L6 case (start with 98_B_DS3F / Diestress), bind its exact source/project and operation profile, then obtain/qualify the missing fresh-original B output before changing implementation. The current CTest skip is not a public refusal claim. |
| `m2m.moproj.deflate` | Optional deflate vectors skipped because `M2M_DEFLATE_VECTORS` is unset. | Missing external vectors. No behavioral conclusion about deflate vectors follows. Find/acquire the qualified vector set or retain the explicit gap. |
| `m2m.moproj.props_l2` | Five property cases (`B11_CM`, `T_CM_DB.x`, `T_CM_ONLY.x`, `T_GRP.x`, `T_GRP2.x`) skipped because their projects do not open (`class not registered`); 95 operations/900 values were examined, zero failures. | A test-level admission limitation. It does not establish those classes are absent from the public converter contract or that original DEFORM rejects them. Need registration/source qualification before interpreting skips as behavior. |
| `m2m.p07.corpus` | Two named corpus copies absent; three cases A-excluded; 45 pass/0 fail/5 skip. | Corpus/selection exclusions only. Recover the missing copies or retain the exclusion; no public behavior claim. |
| `m2m.s.save_current` | Five `.moproj` files not compared because there was no input before the GUI edit. | The missing pre-edit inputs prevent attribution. No `.moproj` parity conclusion for those cases. Obtain pre-edit inputs or leave them unscored. |

## Difference-bearing diagnostic

`m2m.s.save_folder` passed its assertions while logging five historical DIFFs: `A:RS_55_Multiple_Operations_Example1` (1 differ, 0 missing, 0 extra), `B:120_B_DOE` (1 differ, 7 missing), `B:121_B_DOE_VAR` (1 differ, 8 missing), `B:122_B_DOE_GEO` (1 differ, 10 missing), and `B:98_B_DS3F` (1 differ, 0 missing, 0 extra). The raw CP147 log says the only file difference in B98 is `gear_blank.mst`; aggregate Oracle-B result is 49 cases, 45 identical, 0 refused, 29 file diffs. These are historical comparator observations, not fresh-session public behavior or proof that CTest validates those outputs. B98 is the cleanest bounded follow-up among these differences because only one named MST differs and there are no missing/extra files; first inspect a raw file-level diff and its declared normalization before proposing any source edit. The L6 Save packet provides relevant Diestress evidence, but does not by itself establish the cause of `gear_blank.mst`.

## Recommendation

Treat `m2m.mdt.save_l6` as the source-ready *evidence* increment: source qualification already exists, while its oracle is explicitly absent. The smallest next step is a single-case evidence audit for 98_B_DS3F: identify the exact L6 slot/test profile and existing raw input/output provenance, then determine whether a fresh original result can be captured within the approved scope. Do not infer a public behavior from the `87_B_HTFF` skip, and do not patch based solely on the B98 diagnostic. Keep fresh-original Open/Save/Close acceptance separate from historical folder comparison.
