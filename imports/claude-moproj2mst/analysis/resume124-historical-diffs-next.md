# CP124 historical DIFF audit and smallest candidate-only check

## Current evidence

The newest completed full validation found here is CP123 (`wave3-checkpoint123-validation.json`, build source SHA-256 `81e35c00599eb03f0351cb452f502eb442fa48208d91cdcbf28fd40ac41c68c5`, 310 registered tests). CTest completed with 303 ordinary passes, six passed-with-logged-gaps suites and one passed-with-logged-differences suite. The historical comparison remains 93 SAME / five DIFF with no fixture changes. CP123 specifically shows `m2m.s.b98_kfread` passed and the historical folder sweep still reports the B98 raw difference. The test does not erase or rewrite the sweep's raw row.

| Historical DIFF | Latest retained result and boundary |
|---|---|
| A: `RS_55_Multiple_Operations_Example1` | One file difference. The two `SimCtrl.KEY` bytes remain source-consistent with the conditional CG-toggle explanation, but original process mode and signal delivery are still unbound. No candidate-only output mapping can establish those original runtime facts. |
| B: `120_B_DOE` | One `.moproj` difference and seven missing files. The before tree lacks the post-action study. This is not equivalent-state Open/Save evidence. |
| B: `121_B_DOE_VAR` | One `.moproj` difference and eight missing files. The before tree lacks the study/variable reflected in the after-state. Later source prefixes do not make the input states equivalent. |
| B: `122_B_DOE_GEO` | One `.moproj` difference and ten missing files. The before tree lacks the post-action study/geometry/DOE-output state. Later geometry prefixes do not close this state and file-owner gap. |
| B: `98_B_DS3F` | One raw MST difference and no missing/extra files. CP123's focused `m2m.s.b98_kfread` passes, and prior CP95/CP90 receipts show the exact single-path-qualified whole-file match. The raw path remains different because the copied project has a different absolute root. |

All five rows therefore remain historical raw differences. The folder sweep normalizes declared view/banner profiles but has no B98 path adjustment; its raw B98 result is consistent with the focused path test and retained candidate-only receipts. None of these retained comparisons is a fresh original Open/Save capture.

## Selected candidate-only action

The smallest useful new check is to run the existing opt-in `admin/accept_b98_before_after.py` against the current frozen CP123 artifacts. This adds a durable B98 raw-output hash and separate exact-path-qualified hash tied to the latest candidate, while requiring no original GUI and no new build. The CP123 freeze receipt binds:

- Source tree SHA-256: `81e35c00599eb03f0351cb452f502eb442fa48208d91cdcbf28fd40ac41c68c5`.
- `moproj2mst.exe`: `3a0528f5f2870b13c5f2795b5fa93df8ff53a11b3c894b147877ed976003aa97`.
- `moproj2mst.dll`: `012e686b9358b83dea9e159d93f61c5dd417b1a5df15e620ca214ed0b9fa8dbc`.
- `deformdb.dll`: `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`.

Proposed command (not run in this audit):

```powershell
python admin\accept_b98_before_after.py --candidate-version CP123 --freeze-receipt build\wave3-checkpoint123-verified\receipt.json --source-tree-sha256 81e35c00599eb03f0351cb452f502eb442fa48208d91cdcbf28fd40ac41c68c5 --cli build\wave3-checkpoint123-verified\moproj2mst.exe --expected-cli-sha256 3a0528f5f2870b13c5f2795b5fa93df8ff53a11b3c894b147877ed976003aa97 --expected-moproj2mst-dll-sha256 012e686b9358b83dea9e159d93f61c5dd417b1a5df15e620ca214ed0b9fa8dbc --expected-deformdb-sha256 c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3
```

The script pins child `M2M_MO_INPUT_MODE=0`, uses a unique disposable short `C:\PROBLEM\b98r` run directory, checks the frozen artifact hashes, retains the raw candidate MST and a separate exact-path-qualified MST, and compares the candidate folder with the retained after-state. A successful result would update candidate-only B98 acceptance for CP123; it would not remove the historical raw DIFF or prove fresh-original parity.

This is preferable to adding another B98 CTest (the focused test already passed CP123) or repeating B109 (the new raw B109 test is included and passed in CP123). It cannot resolve A55 or the three DOE historical action-state mismatches. No build, GUI, source edit, test run, fixture change or mutable REA operation was performed here.
