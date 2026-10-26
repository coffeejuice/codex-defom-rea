# CP262 independent-radius implementation review — PASS

The final production source and corrected tests match the independently reviewed native radius contract. The earlier slot-test discrimination hold is resolved: each slot uses a fresh store, hidden rejection precedes enabled acceptance, and the named donor independently proves invalid scene-model box versus valid cached all-view contribution. Focused3 builds the matching source and its focused runtime passes all 65 cases. No source/test fix remains within this bounded leaf.

Only this review was written. No source/tests/build/shared state, original process, GUI control or mutable REA was changed or run by this review.

## Slot-test discrimination corrected

The final `test_viewer_box.cpp::display_fit_radius_includes_hidden_views_only_inside_admitted_models` uses a new `Project`/display/store on every secondary, third and named loop iteration. It first proves hidden-model origin zero, then enables that route and requires origin approximately -0.01. Omitting the later radius contribution now rejects into zero, so the third/named assertions can no longer pass by retaining an earlier accepted origin.

For each donor, the final fixture hides its view, explicitly calls ordinary `viewerRefresh`, then requires `viewerSceneBox` invalid and `viewerAllViewBox` valid before the actual caller. This resets the named cached model box without clearing its hidden cached view box, so the donor affects independent radius without enlarging the shown scene box. The current window-union geometry is consequently the tiny translated main; fresh-origin acceptance now identifies the intended independent path.

These are inspected final edits, not approval of a proposed repair. Temporary donor/tiny owners die while the local project still exists, and their ordinary destructor/weak-guard behavior clears observations; canonical secondary and named storage remain display-owned. No guard/live-owner mismatch is fabricated.

## Production source assessment

`viewer.cpp::viewerAllViewBox` is a private engine query declared only in `engine_internal.hpp`. It enters through `viewerOf`, retaining existing geometry-currentness and pending-removal admission. It returns value outputs rather than a retained private borrow. Its 2D branch visits settings-bearing views without an outer ObjectOn/kind filter, invokes existing `recompute2D`, unions view rectangles, then applies axis geometry 1/3 extension from cached `v.modelRect` after each contribution. It does not overwrite modelRect. The 3D branch unions settings-bearing cached per-view boxes with no refresh, outer shown/kind filter or rectangle fallback. Other dimensions retain invalid default box; 2D rectangle conversion remains valid even when degenerate. Null-view skips follow existing adapter representation handling; no arbitrary malformed/native null-view parity is established.

The cached-model axis copy/min-x-zero/valid construction agrees with the decoded `model+108` argument and retained `V/v15.c::FUN_141238bf0`, which marks valid and copies endpoints without normalizing them. No fresh-view accumulator substitution appears. Missing-settings debug logging is still excluded from claimed parity.

`viewer_camera.cpp::sceneRadius` unions model contributions in main, secondary, third, then flagged named order, with fixed live-guard-before-shown checks and named flag-before-model lookup. It retains independent alias visits and existing `BoxD` union/derive arithmetic; this is not max-of-model-radii or scene-box-radius substitution. Canonical synchronous named traversal uses the existing owner order; no callbacks/reentrant collection replacement guarantee is inferred.

`fit` retains fixed-slot refreshes, then scene-presence guard, independent radius, first `sceneBox`, camera store, second `sceneBox`, window, reset and origin. Width/height are computed before invalidity. The store condition uses `valid && !(0.001 > radius) && !(0.001 > width) && !(0.001 > height)`, preserving the independently decoded COMISD/JA/JBE unordered value behavior under the configured `/fp:precise` compilation. No unsupported NaN fixture or signaling-NaN exception claim was added. No catches or rollback erase completed earlier refresh/2D rectangle effects; a later refusal propagates. These are source-order findings, not an executed native failure-prefix witness.

## Other test assessment

The translated curve-only fallback test proves the intended distinction using actual `displayRefreshScene`/`displayCamera`: all-view 3D box invalid, valid scene rectangle/window, and default origin retained; a proper populated imported surface then establishes accepted nonzero origin, and a later fallback fit preserves it while updating the window. This is a meaningful discriminator against the old substitution and an always-zero helper. `importedFitRep` reads the existing COG fixture's supported 242-node/480-element surface and uses its normal surface scale/translate/bind path; it does not fabricate optional cross-section mesh owner state.

The 2D hidden-view test uses genuine 2D object binding, verified flags, a nonzero cached rectangle before flag removal, and `viewerBox`'s non-recomputing 2D read before/after the actual caller. It avoids the rejected native-points mutation premise. The axis test covers geometry types 1 and 3 using a flagged named model, cached model rectangle `[0,14]`, empty recomputed local view rectangle, and fresh origin -7 acceptance. That distinguishes cached modelRect from an empty accumulator and establishes the previously open source-bound fixture without claiming natural native reachability.

The final hidden-object-view radius test is meaningful for main and all three added slot routes. Existing fixed/named camera-window tests retain union coverage. There is no new deterministic native exception/FPU witness; guard behavior follows the shared admission source and existing suite, not a new live ObjectKey/currentness proof. Object-kind filter absence is correctly implemented and disclosed as source-reviewed rather than separately kind-7 tested.

## Evidence and runtime status at review

Native reference remains independently verified DEF_GUI_MO_64.exe **v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, with the bounded radius/store PE selections and exact 0.001 constant verified in `resume262-scene-radius-next-leaf-review.md`. This implementation review reuses that independent static evidence; no original instruction was executed.

Read final `resume262-scene-radius-implementation.md`, `resume262-scene-radius-focused3-build.json` and `resume262-scene-radius-focused-ctest.log`. Focused3 configure/build exit codes are zero and before/after source fingerprint matches `f18953172cba096fb725d805918307eba03e8b79e6080f909d51f9c2c2f1e442`, with source stability true. Current `m2m_mdt_viewer_box.exe` SHA-256 matches its focused3 receipt: `f791eb2466cad863fda036698bf88ba9db58b06d4978745029afd78fe9dc4dd4`. The final retained focused log reports **0 of 65 cases failed**, registered CTest passed, 0.45 seconds. Focused1 compile failure and focused2's first 64/65 run remain retained failures, not successful final evidence.

Minor report-only correction: the implementation report says “Five new Testkit cases,” but the source adds four `TEST_CASE` entries; the populated rep profile shares the first case. Use “four cases covering the listed profiles” or omit the count. This wording does not change source/test acceptance.

Final independently checked source hashes: `viewer.cpp` `7884d9f7b53bdde3a9435c6b320d663e662b4121af9fe5b81c5d0f3a052d1456`; private header `6be7e1760e77c5e6c442dfdbab123775824964bb7d86f2980a638dba5c97c58c`; `viewer_camera.cpp` `78d811b075069f07e886a0731e54234d8da6ac042237a08ba620cc4f8d8f8c8e`; `test_viewer_box.cpp` `f91ccead6405dbb53a657d35b64239e334b5671c87a5f8565f51654ad73cc386`. PASS is for this source and focused evidence. Grouped/full checks and root-owned source-stable freeze remain separate requirements; natural native runtime reachability, complete floating/exception parity and unresolved broader recovery goals remain unclaimed.
