# CP253 dimension preflight implementation review

**PASS after the focused correction and rerun — prior HOLD resolved.** Production migration remains correct, and the repaired test now explicitly removes the step and checks absence before and after the actual caller. No remaining required correction was found in this bounded re-review.

## Correction re-review

The absent branch now starts with `GuiModel(nullptr, 3)`, executes `model.step.reset()`, requires `!model.step` before `openInitialEditorInto`, and checks `!model.step` afterward. The live-dimension branches separately require a present step with the expected scalar. Thus the test now distinguishes genuine missing storage from live dimension zero while retaining invalid-locale precedence, default report/page/source metadata, construction state and zero-read/effect assertions.

The implementation receipt appends an explicit correction/rerun section and identifies `cp253-editor-dimension-preflight-focused-r1-build.json`, rather than overwriting the original evidence. That receipt records the same focused targets with matching before/after source-tree hashes. Current focused CTest log records both targets passing at 15:14. The current test source hash is `21c189d5183cd479a08be74958c31d8f128fd07e94b7f9b6d2474eee298127ee`; the rebuilt lifecycle executable hash is `89754eb6b771070b3f26179120aa2d19dc7ef533c8437c20159ba4eb73007d95`, matching the correction receipt. No reviewer build/test execution occurred.

The initial review below is retained as the historical reason for the resolved HOLD. Its old source/test hashes and absent-step defect describe the superseded first attempt, not current coverage. Other optional test-strength/provenance limitations remain applicable; none requires further change for this one-expression migration.

Reviewed current lifecycle source/test, the implementation receipt, CP252 contract and CP253 PASS design review, model constructor/getter, existing build receipt and focused CTest log. Current source/test hashes match the implementation receipt (`56506efe…baffd` and `da4d0266…f7fb0`). No reviewer build or test was run.

## Required correction

In `initial_editor_dimension_preflight_preserves_admission_and_locale_order`, `checkRefusal(0, true, ...)` constructs `GuiModel(nullptr, 0)` and never clears `model.step`. `GuiModel` unconditionally initializes `step` with `make_unique<DB_STEP_DATA>(dim)` even for dim zero. Thus this is another live-zero fixture, not missing storage. Root flagged this during its inspection; this independent review verified it directly against the constructor and test.

The assertion `CHECK(!model.step || model.dimension() == dimension)` does not detect the error: it accepts a present zero-dimensional step. The receipt's statement that the absent-step case asserts the step remains absent is therefore unsupported by this source and must be corrected.

Construct a valid ordinary model, explicitly clear/move out its step for the absent branch, and `REQUIRE(!model.step)` before calling the actual `openInitialEditorInto`. Assert `!model.step` again afterward rather than the current disjunction. Do not retain/dereference a control after its allocation is destroyed. Preserve the invalid locale and all report/no-I/O assertions so this case exercises `!control` refusal before dimension/locale/page work. Rerun the two focused targets and refresh hashes/receipt, since current passing binaries never tested this path.

## Production source passes

The sole selected conditional is exactly `if (!control || model.dimension() != 3)`. Report reset, AlreadyStarted branch, constructionStarted assignment and single simCtl lookup remain in order. Getter is the existing unguarded nullable scalar, so no second geometry admission was introduced; null current control short-circuits it. Live dimension three alone passes this predicate.

Locale validation remains afterward, then control-reference formation/source model/control/remesh publication and page fills. Parked simCtl exceptions still occur after constructionStarted commits and before dimensional/locale issue assignment or metadata. No control lookup was hoisted, no fallback-three policy introduced, and no exception policy changed. Larger native/cached aliases remain.

## Other test strength and limits

The live-two/live-zero invalid-locale cases meaningfully distinguish InvalidModel-before-locale. The live-three invalid-locale case distinguishes acceptance of actual three from model refusal. The retry repairs dimension two to three and still expects AlreadyStarted, which would catch accidental replay. Its source order establishes report reset; seeding nondefault report fields before retry would be a stronger direct reset assertion, but is not the blocking issue.

`Io` now increments readCalls at entry to all three read methods. Existing remove/write/warning entry paths append to effects, so zero reads plus an empty effects vector is meaningful no-I/O coverage for these fresh preflights. No claimed read is hidden behind an early return in those counters. Existing successful lifecycle tests provide downstream 3D regression coverage.

The matrix checks page arrays, reached-page absence, source identities and construction/completeness on invalid-model branches. It does not explicitly check every backup/refill bool, and the live-three/retry branches have fewer metadata assertions; avoid describing those as exhaustive byte/state comparisons. Strengthening them is optional beyond fixing the absent-step case.

`cp253-editor-dimension-preflight-focused-build.json` confirms successful configure/build for lifecycle and model-dimension targets. Current focused CTest log records both passing. Those results are real for the tested fixtures, but they cannot repair the absent-step coverage error. The receipt appropriately makes no full build/freeze claim.

Retained getter attribution and adapter admission qualifications remain valid: this source refactor does not establish original graceful handling of null/zero models, actual Qt construction, concurrent mutation safety, public Save or ownership closure.

Only this review report was written. No source, tests, builds, shared state, GUI, original process, or mutable REA state was changed.
