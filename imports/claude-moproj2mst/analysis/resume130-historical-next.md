# CP130 historical DIFF reconciliation and headless comparison

## Latest accepted baseline

CP129 is the latest completed full validation available: 315 registered tests, 308 ordinary passes, six passed-with-logged-gaps, one passed-with-logged-differences, no incomplete/skipped/failed tests, and 93 SAME / five DIFF (`wave3-checkpoint129-validation.json`, `resume129-final-tests.json`). No fixtures changed. The five historical rows remain exactly A:RS_55, B:120, B:121, B:122, and B:98 with the same difference/missing counts and no per-case comparator adjustments. The folder sweep still logs differences rather than failing assertions and applies only its declared view/banner normalizations.

## Case reconciliation against CP129

| Historical row | CP129 evidence | Completion gate |
|---|---|---|
| A: `RS_55_Multiple_Operations_Example1` | Still one raw `SimCtrl.KEY` difference. Retained original/candidate files differ only at two bytes following `SOLMTD 2 4`; candidate values are `[0,0,0,0]`, retained original `[0,1,0,1]`. The static CG callback remains a conditional explanation. | No process-bound original `MoInputMode`, active Task00002 at first fill, unchecked-to-checked CG transition, or callback-delivery receipt has appeared. The historical comparison is not mode-matched and cannot justify a default writer fix. |
| B: `120_B_DOE` | Still one `.moproj` difference and seven missing files. CP129 has passing candidate tests for DOE prefixes, but these are not a completed B120 public Open/Save reproduction. | Before input lacks the Type2 Study present in the retained after-state. B120's active DOE Output Open probe reaches the `.DS` `DBLIST`/`ReportList` guard before normal Open returns. The full comparison remains an action-state mismatch with an earlier Open blocker. |
| B: `121_B_DOE_VAR` | Still one `.moproj` difference and eight missing files. `m2m.doe.b121_tloc_model_prefix` passes, establishing only its bounded variable prefix. | Before input lacks the post-action Type2 variable. Nonempty scalar-variable reconciliation, notifier completion and public Save continuation remain unqualified. The prefix test does not make the historical before/after pair equivalent. |
| B: `122_B_DOE_GEO` | Still one `.moproj` difference and ten missing files. CP129 geometry binding, variable-prefix, and writer-prefix tests pass. | Before input lacks the post-action geometry variable/DOE operation; full geometry ownership/Open and all three GEO file writes remain unproved. The focused tests do not close the historical action-state gap. |
| B: `98_B_DS3F` | Still one raw `Gear_blank.MST` content difference. `m2m.s.b98_kfread` passed; prior CP124 candidate-only acceptance established raw path difference and whole-file equality after replacing only the exact project path in a separate copy. | Candidate path production is qualified, but the raw difference remains and no fresh original same-root Save was captured. Preserve raw and path-qualified results separately. |

CP129's passing `m2m.doe.constraint_xml_prefix` and `m2m.doe.refresh_notifier_prefix` cover the actual-owner zero-variable XML prefix and unsuppressed notifier scalar store before the refresh key/receiver guard. They do not create historical Type2 study state, pass B120's earlier Open guard, reconcile B121's variable, import B122 geometry, or reach the real table receiver/public continuation. They change no full-case gate. The B98 and B109 tests also pass CP129; that adds candidate-side evidence only.

## Selected original-free headless check

Run the existing opt-in `admin/accept_b98_before_after.py` against CP129's frozen artifacts. This is a candidate-only raw comparison plus the existing narrowly scoped one-path diagnostic view. It produces a durable raw `Gear_blank.MST` hash and a separate exact-path-qualified hash, directly bound to the latest candidate. It launches no original GUI and does not modify the retained fixtures.

CP129 frozen binding:

- Source SHA-256: `eb54421d31c4fb42e23c1339459ce6b7cfb706ebd2967f870da7a0b5157b3490`.
- CLI SHA-256: `b4ca7375f3d5a50614dc932770aa68d7001eb491bb9a45cdd73090d1c33de46d`.
- `moproj2mst.dll` SHA-256: `c7dd503e96e630797508b5e9e085f45ee1fc4d5123ce670ddd14d48921251f12`.
- `deformdb.dll` SHA-256: `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`.

Exact command (not run for this review):

```powershell
python admin\accept_b98_before_after.py --candidate-version CP129 --freeze-receipt build\wave3-checkpoint129-verified\receipt.json --source-tree-sha256 eb54421d31c4fb42e23c1339459ce6b7cfb706ebd2967f870da7a0b5157b3490 --cli build\wave3-checkpoint129-verified\moproj2mst.exe --expected-cli-sha256 b4ca7375f3d5a50614dc932770aa68d7001eb491bb9a45cdd73090d1c33de46d --expected-moproj2mst-dll-sha256 c7dd503e96e630797508b5e9e085f45ee1fc4d5123ce670ddd14d48921251f12 --expected-deformdb-sha256 c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3
```

The script pins candidate child `M2M_MO_INPUT_MODE=0`, verifies the frozen receipt and adjacent binaries, copies the retained B98 before tree into a unique short `C:\PROBLEM\b98r\` run directory, compares 29 paths, and retains raw candidate output plus a separate one-path-qualified copy. Expected contract: at most the single raw MST path-value difference, no missing/extra paths, and full byte equality only after that exact path substitution. A successful result extends candidate-only CP129 evidence; it does not erase B98's raw DIFF or establish fresh-original parity.

No build, test, GUI, source edit, candidate execution, original execution, fixture mutation or mutable REA operation was performed for this read-only audit.
