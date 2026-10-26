# Normal project Close: nested Save answers and partial accepted-Close effects

## Scope and evidence

This is a source-bound static trace for original binary SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, using `analysis/wave3-normal-close-guard-contract.json`, `analysis/wave3-close-save-return-contract.json`, and `analysis/wave3-close-post-answer-contract.json`. None is a full original runtime capture. Default Yes is qualified to the retained shipped Windows/Common, WindowsXP, or WindowsVista style path with unchanged initial focus; actual application style startup and GUI focus were not runtime-verified.

## Selected current-operation question: exact nested Save return mapping

For a project with model present and process type 1 or 5, the current-operation Close question uses Yes `0x4000`, No `0x10000`, Cancel `0x400000`, with `NoButton` requested as explicit default. Under the qualified style/focus profile, the static default is Yes.

On Yes, `0x140044110` calls nested Save `0x1400980d0` while project/diagram remain attached and replaces the question result with Save's returned Qt integer. The source-bound result cases are:

| Nested Save result | Guard's final answer | Consequence in the static guard |
|---|---|---|
| Yes `0x4000` | Remapped to Cancel `0x400000` | Close rejected; project remains attached; accepted-Close callbacks are skipped. |
| No `0x10000` | Remains No | Not Cancel, so accepted-Close callbacks run. This includes a checked Save whose later writer boolean is false; that boolean is not the returned answer. |
| Cancel `0x400000` | Remains Cancel | Close rejected; project remains attached; accepted-Close callbacks are skipped. |
| Other integer, including zero | Preserved | The guard only treats exact Cancel as refusal; do not call zero a Save failure here. |

If a subproject's last-operation warning is reached, the warning offers only Yes; its statically derived default is Yes. Nested Save stores that result and returns immediately before current-operation Save and all project/property/MST writes. The Close guard then remaps this Yes to Cancel. This explains the selected invalid-placement default path without conflating it with a writer failure.

For an ordinary checked Save with no warning, Save's integer answer follows its validation path independently of writer success: after at least one nonnull subproject passes validation, it returns Yes even if later project/MST writing returns false; with no nonnull subproject it retains No, which the Close guard does not remap to Cancel. These unusual integer semantics are source-derived and should not be “corrected” into a conventional success/failure mapping.

## Accepted-Close callback order and partial effects

Only a final non-Cancel answer reaches the accepted tail. Static order is main-window virtual `+0x288`, followed by notifier `signalRemoveGraphItems`, then return true from the guard; actual project/diagram detach follows only after all guards accept.

For a nonnull display receiver, the `+0x288` route resolves the pointer and calls `0x140d0e540` with `(variable=0, component=0, notify=true)`. Retained static evidence supports selected displayed-model StateVar writes of zero, a `2signalChangePtrkGraph()` notification, clearing per-view plots on variable zero, additional StateVar/render calls, eligible named-model updates, and a later main-window callback. `d0e540` is not a no-op. Exact selected model depends on current main/third/named/second display links and controller state. Full DOE render/legend/notification closure and concrete fresh display state remain incomplete, so no complete accepted-Close state result is claimed.

These effects do not run in the selected last-operation warning case, because the nested Save result is remapped to Cancel. The separate `QDialog::exec` intervals (Close question and any nested warning) may dispatch events before answers, but actual eligible event IDs/order are unknown. Modal default selection does not imply no timer delivery.

## Candidate adapter status and smallest unit regression

The current converter candidate has no production Close guard wired into `ConversionSession`; `releaseProjectAfterCloseEffects()` expressly does not decide Close or call Save. Its `save_project.cpp` still represents the last-operation warning as pending-default Yes followed by `M2M_E_UNSUPPORTED`. Therefore candidate status is fail-closed at the warning, not parity with the statically recovered nested Save/Close mapping. Do not characterize it as accepted Close or as a completed failure contract.

The smallest candidate-only unit regression belongs in the isolated `close_dialog::resolve` test seam, without public Close or cleanup binding: supply a qualified Close-question schedule; invoke nested Save and return each integer in turn; assert only exact Save Yes becomes Close Cancel, while No, Cancel, and zero retain their values under the guard's `answer != Cancel` decision. For the warning case, supply and dispatch its distinct schedule before returning Yes, then assert Close Cancel and no accepted-Close callback (none exists in this isolated helper). Keep this a unit test of the integer contract, not an original-runtime test.

## Exact unresolved runtime input

Full normal-Close acceptance still requires a source-qualified ordered modal event profile for the Close question and for the nested last-operation warning when reached. It must state eligible timer IDs—including a supported explicit empty set—at each modal interval and resolve callbacks against the then-current session receiver/model attachment. For accepted-Close parity, also bind current display/model/controller state and the remaining transitive StateVar/render/notification recipients. Until those inputs are available, retain the candidate's unsupported warning boundary and do not infer a fixed timer schedule, empty scene, or harmless post-answer callback.

No source edits, builds, GUI sessions, mutable REA operations, or tests were performed.
