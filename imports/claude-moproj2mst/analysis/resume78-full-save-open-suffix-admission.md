# CP78: Full-Save Open suffix pre-admission

## Finding

In the current full-Save adapter, `core/convert.cpp` creates the `ConversionSession` and calls `prepareProjectModel(db)` / `commitProjectModelConstruction()` before `openProject(p, file, ctx)` checks the selected suffix. `prepareProjectModel()` records `SessionModelManagerReadReceipt`, creates/registers a model child and binds `p.dataDb`; only later does `moproj/read.cpp::openProject` return false for an unsupported suffix. The public error code eventually remains `M2M_E_PARSE`, but an unsupported path has already crossed the candidate model-manager read/construction boundary.

The retained dispatcher ordering places its Open suffix admission before the native manager `+0x50` read. Candidate `m2m_convert` currently crosses the `prepareProjectModel()` manager-read/child-registration boundary before `openProject()` performs its selected-path suffix check. The correction below moves only the candidate admission point; it does not port file-dialog UI or the legacy import actions. Its accepted set matches the current candidate `openProject` decision so DB/KEY remain accepted-but-guarded.

## Smallest correction

Add a pure selected-project suffix classifier using the existing case-insensitive `app::endsWithSuffix` helper. Use precisely the `openProject` accepted set:

| Classification | Predicate | Full-Save pre-admission action |
|---|---|---|
| Project | `.moproj` | Continue unchanged. |
| Database | `.DB` | Continue unchanged into existing DB open prefix and intentional `M2M_E_UNSUPPORTED` guard. |
| Keyword | `.KEY` | Continue unchanged into existing DB open prefix and intentional `M2M_E_UNSUPPORTED` guard. |
| Rejected | none of the above | Throw `Error(M2M_E_PARSE, "Error on reading project file!")` before Session construction/model receipt. |

Run this admission only for `M2M_FLAG_WRITE_SAVE`, after the existing input path has been made absolute/clean when relative (`convert.cpp:185-189`) and before `DiagramEffectDriver`, `ConversionSession`, `prepareProjectModel`, and `initProject` (`:190-200`). This preserves the current adapter's non-full-save `checkReadable()` precedence and its error mapping. Do not reject DB/KEY in the admission step: that would replace their current `M2M_E_UNSUPPORTED` outcome with `M2M_E_PARSE` and skip a currently retained candidate DB-open prefix.

The guard's unsupported-suffix code/message should match the existing later failure at `convert.cpp:227-233` exactly. Do not claim native legacy import support, alter the C ABI, or alter the ordinary MOPROJ/DB/KEY parse/guard implementation.

## Focused internal checkpoint

The smallest testable internal checkpoint is the pure classifier/admission helper, unit-tested independently and called once at the pre-Session location:

1. Case-insensitive `.moproj` is admitted.
2. Case-insensitive `.DB` and `.KEY` are admitted as distinct kinds (test confirms they remain eligible for downstream guards, not successful Open).
3. An arbitrary suffix is rejected with exactly `M2M_E_PARSE` and the existing message.
4. A focused call-order test should prove the rejected classification occurs before an injected/observable session-creation continuation. Keep this as an internal helper seam; do not add a public C ABI diagnostic field solely to expose the Session receipt.

The existing `g0` public API error test can be extended with a full-Save unsupported-suffix input to lock the public result/message. It cannot alone prove absence of the candidate Session receipt; the helper's call placement before `ConversionSession` and an internal continuation/order test should cover that.

Avoid testing DB/KEY by running the complete public lifecycle against an existing database: their current path calls `engine::openProjectDb` before the explicit refusal. The required preservation assertion is classification/admission plus the unchanged guard location/result, not a DB/KEY file open.

## Evidence

- `source/apps/moproj2mst/src/core/convert.cpp:178-200,226-233`: selected-path normalization, current Session/model construction order, Open invocation, and existing parse-status mapping.
- `source/apps/moproj2mst/src/engine/conversion_session.cpp:266-285`: `prepareProjectModel()` creates the existing-manager read receipt, registers the child and binds it to Project storage.
- `source/apps/moproj2mst/src/moproj/read.cpp:29-57`: accepted suffix set, normalization and DB/KEY unsupported boundary.
- `evidence/gui_mo/s1/s1-01/dc/sub0.c`, `FUN_14001ca30`: native legacy-import routing, DB identity/model manager construction, then call to `0x14008a9b0`.
- `evidence/gui_mo/s1/s1-01/dc/open0.c`, `FUN_14008a9b0`: native accepted suffix check and path normalization.
- `analysis/resume76-open-save-dialog-filter-path.md`, `analysis/resume77-open-path-increment.md`: dialog/path qualification and selected-path test contract.

Static design only. No source, tests, build, GUI, or mutable REA state was changed or run.
