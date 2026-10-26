# CP127 historical DIFF status and one original-free check

## Latest acceptance evidence

CP126 (`wave3-checkpoint126-validation.json`, `resume126-fixture-repair-tests.json`) is the latest full validation in the current evidence set: 313 registered tests; 306 ordinary passes, six passed-with-logged-gaps, one passed-with-logged-differences; no incomplete, skipped, or failed tests; 93 SAME / five DIFF; no fixture changes. Its focused tests include passing `m2m.doe.constraint_xml_prefix`, `m2m.doe.refresh_notifier_prefix`, `m2m.mdt.doeoutput_active_guard`, `m2m.s.b98_kfread`, and `m2m.s.session_b109_raw`. Thus CP125's unsuppressed notifier scalar prefix now has candidate test evidence, but the historical rows still remain unchanged and the refresh receiver/public continuation is still guarded.

## Historical completion gates

| Row | Current gate after CP126 | Completion effect |
|---|---|---|
| A: `RS_55_Multiple_Operations_Example1` | The raw pair remains the two `SimCtrl.KEY` bytes (`0` candidate, `1` retained original) at offsets 1439 and 1455. The original process's effective `MoInputMode`, active operation at initial editor fill, CG state transition, and delivered `toggled(true)` callback are still not bound. The static callback is only a conditional explanation. | No mode-matched comparison or source correction is justified. A candidate mode-1 path remains guarded, and candidate mode-0 cannot answer whether the original used editor300. The fresh original process-bound mode/event witness remains missing. |
| B: `98_B_DS3F` | CP126's focused `m2m.s.b98_kfread` passes; CP124's CP123-frozen acceptance separately established a raw path-bearing MST difference and exact one-path-qualified equality. The folder sweep remains one raw `.MST` difference. | Candidate path behavior is well-qualified, but no fresh original same-root Save is captured. It is not raw parity or fresh original acceptance. No path normalization should be promoted into the folder comparator. |
| B: `120_B_DOE` | CP124 Constraint XML and CP125 notifier reset are now exercised by CP126 focused tests. They cover an owned zero-variable Study prefix through the notifier reset/refusal. The historical before tree lacks the post-action Type2 Study; the retained B120 DOE Output Open probe also stops at DBLIST/ReportList before normal Open returns. | The prefix tests do not supply the missing before-state Study or unblock B120 Open. The full row remains an action-state mismatch and public Save is not reached for the retained B120 route. |
| B: `121_B_DOE_VAR` | CP126 includes a passing bounded `b121_tloc_model_prefix` test. The historical after-state has nonempty scalar `Transfer Time`; the before-state lacks that post-action study/variable. Type2 Save still guards nonempty variable reconciliation before Constraint XML/refresh. | The source-tested prefix narrows one state transition but does not import the variable through complete public Open, deliver its later notification continuation, or reproduce the after-state from the retained before project. Full row remains unqualified. |
| B: `122_B_DOE_GEO` | CP126 includes passing geometry binding/prefix/writer-prefix candidate tests. The retained after-state has a nonempty geometry variable and three GEO files that are absent from the before state. Full geometry ownership, public Open/Save, and required file effects remain guarded. | The tests establish their bounded candidate prefixes, not the GUI-created study/geometry/action sequence or the ten-file historical outcome. Full row remains unqualified. |

The important CP126 update is that the CP125 reset prefix is no longer merely source-reported: `m2m.doe.refresh_notifier_prefix` passed. Its documented boundary still refuses after the unsuppressed `flag90=false` store and before key construction/registration/receiver dispatch. This does not reach the table timer, normal callback return or later PAR writers, and does not alter any historical before-state.

## One executable original-free acceptance check

The smallest remaining check that adds a durable artifact-specific receipt without an original GUI is to run the existing `admin/accept_b98_before_after.py` with the CP126 frozen CLI. CP126's `m2m.s.b98_kfread` already passed, but this opt-in script would retain the raw MST hash and separate exact-path-qualified hash tied to the latest frozen artifacts. It preserves, rather than masks, the raw B98 difference.

Frozen artifact binding:

- Source tree SHA-256: `bb389d13f31bc34005022f8feff0f8ac727213909c9e87d5a895360559564495`.
- CLI: `436b49d91221c4c043231b70341caa789154f2347bdb6f8e887c3a2c8305adae`.
- `moproj2mst.dll`: `7ed10fa831c3670fc03cb8705d81a7e2b2542b99587b7777e5e441e5e3d074b5`.
- `deformdb.dll`: `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`.

Exact command (not run for this read-only review):

```powershell
python admin\accept_b98_before_after.py --candidate-version CP126 --freeze-receipt build\wave3-checkpoint126-verified\receipt.json --source-tree-sha256 bb389d13f31bc34005022f8feff0f8ac727213909c9e87d5a895360559564495 --cli build\wave3-checkpoint126-verified\moproj2mst.exe --expected-cli-sha256 436b49d91221c4c043231b70341caa789154f2347bdb6f8e887c3a2c8305adae --expected-moproj2mst-dll-sha256 7ed10fa831c3670fc03cb8705d81a7e2b2542b99587b7777e5e441e5e3d074b5 --expected-deformdb-sha256 c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3
```

The script validates all frozen hashes, pins child `M2M_MO_INPUT_MODE=0`, copies retained input to a unique `C:\PROBLEM\b98r\` directory, retains raw output and one-path-qualified copy separately, and does not launch the original application or modify retained fixtures. A passing result would extend candidate-only B98 evidence for CP126; it would not remove the raw DIFF or upgrade the historical GUI action to fresh-original acceptance.

No build, test, GUI, source edit, original execution, fixture change, or mutable REA action was performed for this audit.
