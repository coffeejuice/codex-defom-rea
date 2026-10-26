# CP141 independent review: bounded solver6 Fill

## Result

The finite solver6 Fill proposal is internally consistent with the retained original branch and current candidate implementation. I found no source-level blocker in the requested exact-int32 admission, `g6` formatting, or text retention across explicit-control visibility toggles. This is a static review only; no build, original application run, GUI action, or mutable REA work was performed.

## Evidence and review

`analysis/resume141-editor-next.md` ties the proposal to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Correction to that proposal's callsite label: the actual 3D solver6 branch is `[140ec2412,140ec26de)`, within the retained Fill body `analysis/wave2-save-diffs-original-iteration-fill.asm` (SHA-256 `7e6924b494cb7cb747d9e5325850cf4591cd3658a3593c19b0f32274537ce56d`). The earlier `[140ec1e3a,140ec20ff)` label points to an analogous earlier body. At the actual branch, the solver6 comparison is at 2430; controls are shown at 2447..24be; formatting and setters run at 2534..26d8. The instructions establish two `QString::number(double,'g',6)` conversions into b18/b28, followed by CVTTSD2SI and decimal `QString::number(int,10)` into b38/b48. The implementation preserves that order and does not write the four source parameters or parameter count.

The `exactSignedInt32` bit test admits either signed zero, integral values within the signed 32-bit range, and negative 2^31. Its exponent-31 case requires a negative sign and zero fraction; exponent 0..30 requires all fractional mantissa bits to be zero. Fractional, nonfinite, subnormal nonzero, and out-of-range values are rejected before CVTT conversion. This is a deliberately bounded candidate profile, not a claim that native Fill rejects those values: native CVTT truncates fractional values and its exceptional/out-of-range behavior remains outside the admission.

The formatting call reaches `qt::number(double,'g',6)` and `qt::number(int)`. In `src/qt4/number.cpp`, the former maps `g` to significant digits and calls `doubleToString` at precision 6; the latter uses decimal `longLongToString`. The focused test checks representative output including `1.23457`, `1.23457e+07`, and signed-int endpoints. This matches the source-qualified API and existing candidate formatter, but is not an independent runtime comparison against the original Qt binary for all floating-point edge cases.

The toggle path is also bounded. Original callback evidence in `analysis/resume140-editor-explicit-toggle-review.md` describes visibility operations on the explicit widgets and no text read or write. Candidate `StagedEditor300::explicitSolverToggled` only updates `form.explicitControlsVisible`; the new focused test stores all four optional texts and verifies they are unchanged across 6→non6→6 visibility events. This demonstrates candidate state retention, not native signal delivery or widget side effects beyond the statically recovered show/hide behavior.

## Limits

The review does not validate the original process's actual widget construction/event sequence, Qt allocation/failure timing, validators, locale variation, fractional CVTT cases, or solver6 Apply/Save. The existing solver6 Apply guard remains in place, and no public completion is inferred from the private page result.

## Reviewed file hashes

- Proposal: `analysis/resume141-editor-next.md` — `114FD3C4E7155CE088A93F1DDD03EBFA6924F70D14515DC45D2A40C3FC1DBE68`
- Candidate Fill: `source/apps/moproj2mst/src/engine/simcontrol300.cpp` — `D19C94F497EB6996CE61373FBAB20722004AA58F058584737ACD1F0E40BF049A`
- Focused Fill and toggle test: `source/apps/moproj2mst/tests/mdt/test_simcontrol300_explicit_fill.cpp` — `8534345E739B3102DC7BF37C0E4D9555FC6EAF3EA2F062BA687354F65AB09871`
- Toggle owner: `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp` — `80A0B704FE3F5610FE2DBF3CCFB84957D02E416DAD20352DC74128372F767FBA`
- Prior visibility test: `source/apps/moproj2mst/tests/mdt/test_simcontrol300_explicit_toggle.cpp` — `85FF18816F579ADC1A78110C76781F5FB7BAE9A78031320DEC511CE166D94564`

Root owns build and integration validation.
