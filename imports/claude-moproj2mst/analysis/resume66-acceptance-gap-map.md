# CP66 acceptance gap map

## Current evidence boundary

The latest verified full-suite receipt is CP64, not the still-integrating CP65 successor. It reports 208 registered CTest entries: 201 clean passes, six `passed-with-logged-gaps`, one `passed-with-logged-differences`, and zero failing, skipped, incomplete, or unreported tests. This is candidate test status; it does not change the retained historical folder comparison (93 SAME / 5 DIFF) or establish fresh original runtime acceptance. The CP64 validation explicitly keeps `objective_complete=false`.

The GUI/original-capture gate is still blocked: `python tools/gui_validation_status.py` reports `release_unverified`, `release_ready=false`, and no available task. The retained lifecycle assessment says no supported release operation plus verifiable release receipt/status is exposed. It does not establish whether an overlay is currently active. No new original capture is currently executable under that gate.

## Six logged-gap tests and the diagnostic test

| Test | Logged qualification | What green does not establish |
|---|---|---|
| `m2m.mdt.mdt_write` | Refuses an active cogging operation; optional imported PACK header check skipped because `M2M_INSTALL_FIXTURE` is unset. | Full MDT write coverage for that operation or the omitted PACK fixture. |
| `m2m.mdt.save_l6` | `87_B_HTFF` skipped because no Oracle-B reference exists. | Save parity for that case. |
| `m2m.moproj.deflate` | Optional deflate vectors skipped because `M2M_DEFLATE_VECTORS` is unset. | Vector-backed deflate compatibility. |
| `m2m.moproj.props_l2` | Five projects do not open because classes are not registered. | Their properties or project-open behavior. |
| `m2m.p07.corpus` | Two cases absent from the corpus copy; three A-excluded; summary is 45 pass / 5 skipped. | Coverage of those five projects. |
| `m2m.s.save_current` | Five `.moproj` outputs not compared because no input existed before the GUI edit. | Save comparison for those post-edit states. |
| `m2m.s.save_folder` (logged differences) | Seven projects lack a finished fresh re-save; three GUI-action cases are explicitly not run. Assertions permit the diagnostic to report differences/refusals. | A clean comparator result or completed lifecycle acceptance. |

These statuses are accurately qualified in the receipt; calling all 208 “strict passes” would mislabel them. Also retain the separate `g0.abi` qualification: its real-project assertion permits either `M2M_OK` or `M2M_E_UNSUPPORTED`.

## Five retained historical DIFF rows

The CP64 comparator has five stable DIFFs with no comparator adjustments: `A:RS_55_Multiple_Operations_Example1` (one file differs); `B:120_B_DOE` (one differing file, seven missing); `B:121_B_DOE_VAR` (one differing file, eight missing); `B:122_B_DOE_GEO` (one differing file, ten missing); and `B:98_B_DS3F` (one differing file). CP65's audit confirms these historical diagnostic records and comparator logs did not change; it does not assert fresh raw-tree equality.

The three DOE rows compare a pre-GUI-action input with expected folders after study/variable/geometry actions. They do not isolate Open→Save parity. B98's retained diff is an environment-rooted `KFREAD` path; a candidate-only regression has already shown the candidate writes a local path and compares the bytes after replacing that exact value in a separate comparison copy. A55 has a raw two-byte `SimCtrl.KEY` difference (`0` candidate vs `1` retained output at offsets 1439 and 1455), but the retained process's `MoInputMode` is not session-bound. Thus A55's mismatch is unresolved, not proven defect or parity.

## Next independent comparison without app control

No fresh **original runtime** comparison is possible until the release gate changes. The smallest available headless candidate-to-retained-oracle comparison is the focused B98 `KFREAD` case (`m2m.s.b98_kfread`): copy its immutable fixture to disposable test scratch, run the candidate Save path, verify the source manifest, and compare raw MST bytes after replacing only the exact candidate-local path in a separate copy. This is specifically a candidate-versus-retained-output comparison, not a new original capture or general whole-tree parity claim. The CP53 test already exercises this profile; rerunning it against the next integrated CP65 build is useful only after root freezes/builds that successor and should be reported as a fresh candidate run, not new oracle evidence.

If the requirement is specifically a new original runtime comparison, the next target remains A55 after a supported release receipt exists. The smallest meaningful capture is a fresh pristine A55 Open→Save with actual `DEFORM-MO.ini` path/hash and `MoInputMode` recorded, preserving raw `Task00002/SimCtrl.KEY` and comparing its two CG values without normalization. Until then, do not edit production code based on the historical A55 delta.

## Candidate-only coverage that must stay labeled

CP65 adds `tests/doe/test_type2_native_open_guard.cpp`, but its report says root owns build/runtime validation and this lane did not run it. It proves no public Type2 success: it is an internal native writer prefix test expecting `.moproj` truncation before the InputTable refusal while retaining the prior Type2 DOM node. The CP64 DOE direct-dispatch helper is likewise candidate-only, explicitly chooses subproject index 1, and uses `RecordingStudyIo`; it is not public C ABI selection or native disk sample writes. Public PPV tests in the CP63 receipt are synthetic candidate contracts, not original PPV output or complete Close acceptance. Keep each statement at its actual test seam and do not promote helper coverage to lifecycle integration.

## Receipts consulted

- `analysis/resume64-integrated-full-tests.json` and `analysis/wave3-checkpoint64-validation.json` (CP64 suite and comparator).
- `analysis/resume65-cp64-audit.md` (freeze, test qualifications, unchanged five DIFF diagnostics).
- `analysis/resume60-five-diff-triage.md` (per-row classification and A55 raw byte qualification).
- `analysis/resume64-doe-public-acceptance.md` (public Type2 selection and candidate-only direct-dispatch boundary).
- `analysis/resume65-type2-native-prefix-test.md` (CP65 unbuilt internal test boundary).
- `analysis/resume65-original-capture-gate.md`, parent `analysis/gui-control-lifecycle-assessment.json`, and `python tools/gui_validation_status.py` (current original-capture gate).

This report used read-only receipts and status inspection only. No source, build, test, GUI, fixture, or mutable REA state was changed.
