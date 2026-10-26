# Last-operation warning: selected-model state and caller return

## Retained static facts

The original binary identified by SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` has this source-qualified path, from offline PE/DUMPBIN and retained decompilation only (`analysis/wave3-close-save-return-contract.json`):

- `0x1400b8fb0` detects a must-be-last operation before another connector item and calls a QMessageBox overload with only Yes available; the requested No default is absent. The retained overload analysis derives the sole Yes result `0x4000`.
- Nested Save `0x1400980d0` replaces its current answer with the per-subproject return and returns immediately when the result is not Yes `0x10000`. Thus warning result `0x4000` exits before current-operation Save and before the project/property/MST writers.
- The normal Close caller `0x140044110` maps returned Yes `0x4000` to Cancel `0x400000`. This warning therefore prevents the accepted-Close path and leaves the project attached.
- The post-answer model/display callback is reached only after the answer is not Cancel. Its recovered input, when reached, is `(0, 0, true)` at `0x140d0e540`; the retained `analysis/wave3-close-post-answer-contract.json` traces selection of displayed GuiModels, StateVar updates/notifications, and eligible named-model updates. This callback is not called on the warning's Yes→Cancel path.

The “selected model mutation” that could still occur during this warning is not the post-answer Close callback above. It would have to arise from events delivered by the warning's modal loop. The retained contracts explicitly leave that modal event delivery opportunity unresolved. They do not establish that no timer is due, what receiver is current at dispatch, or whether a callback changes the displayed selection/model state before the warning returns.

## Exact missing adapter seam

The adapter currently represents the warning in `save_project.cpp` by setting `lastOperationModalPending` and `pendingDefault=Yes`, then throwing `M2M_E_UNSUPPORTED`. `SaveCallState` has no represented modal-loop transition that can:

1. accept a source-qualified set of due events (or establish that none is eligible);
2. dispatch each event in original order against the then-current session receiver and model attachment, preserving consume/disconnect semantics; and
3. return the warning's Yes answer only after those event effects have been applied, while retaining the original Save short-circuit and Close Yes→Cancel mapping.

The existing `ConversionSession` event helper can consume an explicitly supplied timer and invoke `modelUpdated`, but its caller must identify that timer as due. The static warning evidence supplies no due-timer profile. Inventing one, skipping delivery, or calling the helper with a guessed receiver/attachment would turn unresolved state into behavior.

## Determination

No complete default-Yes correction is supported yet. The static answer and post-answer Save/Close returns are known, but modal-loop event delivery and any resulting selected-model mutation are not. Keep the warning unsupported at the adapter boundary until a fresh original capture or another source-qualified contract establishes the modal delivery profile and current attachment behavior. A unit test can continue to assert pending warning state and separately test already-recovered Save/Close mappings; it must not claim to validate the entire dialog branch or a no-mutation result.

## Evidence references and limits

- `analysis/wave3-close-save-return-contract.json`: nested Save answer, Yes-only overload, default answer, short-circuit and Close mapping; explicitly offline/static.
- `analysis/wave3-close-post-answer-contract.json`: model selection and `(0,0,true)` state/display effects after an accepted Save answer; transitive scene/notification closure remains partial and it does not apply to this canceled warning path.
- `analysis/wave3-close-session-integration-audit.json`: due timer is an explicit input; do not infer timer readiness at a modal dialog.
- `source/apps/moproj2mst/src/engine/save_project.cpp` and `source/apps/moproj2mst/src/engine/conversion_session.cpp`: current exception/pending-default adapter boundary and explicit-timer dispatch seam.

No source edits, builds, tests, app-control sessions, or REA mutations were made.
