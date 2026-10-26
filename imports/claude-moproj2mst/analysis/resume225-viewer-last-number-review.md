# CP225 viewer last-number source review

## Result: PASS

The requested four-file diff implements the bounded scalar migration as specified.

- `GuiModel::lastObjectNumber()` calls `requireGeometrySyncAvailable()` before accessing the step, then returns the native `last_object_number(*step)` tail value or zero for a missing step. It exposes only an integer and does not mutate or allocate.
- `viewerSync` retains the geometry availability guard, null-object return, and viewer lookup before calling the scalar operation. Its sparse-object regression creates objects 1 and 4, checks tail number 4 while the native object count remains 2, then verifies viewer sizing; the absent-step query and null-object/no-viewer behavior are also covered.
- `viewerRebuild` retains its initial geometry guard and `withStep` missing-step return before viewer lookup and optional clear. The new tail read remains after the optional clear. Tests exercise absent-step queries and verify rebuild leaves the viewer absent; existing populated-model and clear-order coverage remains.
- The geometry-barrier test directly checks that `lastObjectNumber()` refuses a parked model. This checks the barrier-before-read behavior alongside the existing viewer refusal tests.
- No change appears in `include/moproj2mst.h`, exported C signatures/layouts, or cache1 code in the reviewed diff. The new C++ declaration and implementation are internal to `GuiModel`; no C++ binary ABI compatibility claim follows.

The review was source-only. No build or tests were run; root owns grouped validation. No unrelated source changes were included in this review report.
