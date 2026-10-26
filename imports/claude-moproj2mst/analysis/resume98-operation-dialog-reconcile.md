# CP98 operation and default-dialog reconciliation

## Finding

CP88 remains accurate for the **operation Save-class dispatch inventory**: the represented base/property and step-4 writers do not expose a new generic dispatch omission, while active DoeOutput and Report Save remain deliberately guarded. Later work does add source-qualified public reader behavior, so CP88 should not be read as saying that no operation-related branch has advanced since then:

- CP96 implements the DOE Variable `dimension` attribute-presence branch. The original qualified rule is absent means skip the record setter; present exact `3` stores `3`. Candidate source records `hasDimension` and writes only when present at `variable_record.cpp:60-100`.
- CP97 implements the separately qualified absent `name` path, preserving default-null QString and the create-or-existing/last-owner ordering. Present empty name and missing `numSamples` remain guarded.
- CP97/98 review and implementation close a prepared same-invocation geometry directory query scope in `viewer.cpp:639-647`, with the install-before-query order and exact callback borrow. This is a geometry refresh subpath, not a modal-dialog contract or a new operation Save-class handler.

These are concrete retained-evidence branches that advance candidate public reader/refresh behavior without a fresh GUI witness. They do not unblock active DoeOutput/Report Save or public Close.

## Remaining dispatch and modal gaps

No new source-qualified default-dialog answer branch emerged in CP96–98. The remaining high-impact operation Save branches are DoeOutput and Report:

- DoeOutput still refuses at `engine/save_current.cpp:757-760`; advancing it requires the actual `DoeTableRefresh` sender/receiver/normal return and process-global DBLIST admission owner. CP88's exact native output order does not establish those runtime inputs.
- Report still refuses at `engine/save_current.cpp:761-764`; advancing it requires the actual same-instance report document, viewer model, and ownership/destruction binding.
- Close/modal answer dispatch remains independently unresolved by the CP96 reader changes. In particular, a static Yes→Cancel mapping does not establish nested `processEvents`/dialog `exec` interval consumption or receiver callback timing.

The smallest next evidence needed for the dialog question is a retained native trace/capture of the exact Close action through warning answer, nested Save result, and accepted-Close callbacks, including event-dispatch boundaries and receiver identity. Without that, no public answer mapping or callback-order change is justified. For DoeOutput, the missing inputs are different and source-owned: bind the refresh receiver/normal return and DBLIST global owner first; GUI evidence alone would not substitute for those source bindings.

## Evidence and scope

- CP88: `analysis/resume88-operation-dispatch-inventory.md` (class/property Save inventory and guarded DoeOutput/Report boundaries).
- CP96: `analysis/resume96-doe-optional-dimension-implementation.md` and `analysis/resume96-doe-optional-dimension-source-review.md` (presence-sensitive dimension setter skip); `analysis/wave3-checkpoint96-validation.json` (grouped validation qualification, not fresh original acceptance).
- CP97: `analysis/resume97-doe-absent-name-plan.md`, `analysis/resume97-doe-absent-name-implementation.md`, and `analysis/resume97-doe-absent-name-source-review.md` (default-null name branch).
- CP96–98 geometry scope: `analysis/resume96-geometry-access-frontier.md`, `analysis/resume97-prepared-query-scope-review.md`, `analysis/resume97-geometry-preinstall-borrow-review.md`, and `analysis/resume98-geometry-scoped-borrow-source-review.md`.
- Current guards: `source/apps/moproj2mst/src/engine/save_current.cpp:755-764`; reader branch `source/apps/moproj2mst/src/doe/variable_record.cpp:60-100`; geometry scope `source/apps/moproj2mst/src/engine/viewer.cpp:639-647`.

Read-only reconciliation only. No GUI/original execution, REA mutation, source edit, build, or test was performed.
