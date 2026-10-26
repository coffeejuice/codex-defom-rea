# CP123 active DOE Output: source-closed prefix and next boundary

## Result

The smallest actual-owner private I/O/state prefix in the active `SfOprDoeOutput` path is already present: operation model reset/dimension/view setup, followed by loading the Type2 Study-owned `.DS` document and importing its supported Part/Session records into that same `Study.outputDocument`. The active B120 candidate probe confirms this path reaches the retained `DBLIST`/`ReportList` refusal after reading the actual fixture `.DS` bytes. This is a candidate-only observation, not original GUI acceptance. No further output-data state prefix is reachable on that fixture before the guard, and no second helper should be used to bypass the ordered DBLIST refusal.

## Owner and ordered prefix

Native order in the retained DOE lifecycle report is `SfOprDoeOutput::loadOperation` (`0x140e2e320`) model reset/dimension/view setup, then `loadClassData` (`0x140e2ec40`), which obtains the active sub-project's session document, resets it, reads Control/`<project-base>.DS`, and dispatches `0x140644440(doc,1,0,0)`. The native effects preceding DBLIST import are already qualified as DOE output load effects; this report does not claim the selected Session/global task application completes.

The current candidate keeps the owner chain explicit:

1. `DoeOutputClass::loadClassData` requires the current Project, active Type2 SUB, owned DOE Study alias and configured Project `StudyIo`.
2. `prepareCurrentOutputSession` mutates the `Study.outputDocument` owned by that `Study`: lazily allocate it, reset it, then construct Control/`<project-base>.DS` and call `readSessionDocument` through that Project's `StudyIo`.
3. `readSessionDocument` stores the filename before open. With the preceding reset already performed, failed open or a zero-size file returns with the new empty `PartRoot`; nonzero size installs a fresh DOM before reading/parsing, so later read/CR/XML failure does not restore the old DOM.
4. `readSessionRecords` clears old part/session projections and retained-root children before rebuilding them in document order. For an existing `SessionRoot`, it appends the part and each `SessionRecord` before processing that Session's child payload. Part/Session selection and the four known empty-binding notifications happen only after part import completes.
5. At the first direct `DBLIST` or `ReportList` child of a `SessionRoot`, the candidate throws the retained unsupported error. Earlier filename/open/read, document replacement, part/record allocations, Task-list parsing and reached StepData effects remain in their source order; there is no rollback.
6. If class-data loading returns, `DoeOutputClass::loadOperation` still stops at `selected session/global task application remains unsupported`. That later refusal is after class-data effects and before a full active DoeOutput operation is accepted.

## Reachability observation and limits

The candidate-only test `active_doe_output_stops_at_retained_ds_dblist_guard_after_study_setup` uses a copied B120 DOE project, selects the Type2 sub-project's `SfOprDoeOutput`, verifies the copied `.DS` read (2013 bytes and retained hash), verifies the three prior Study sample writes, and expects the `0x140650e60 DBLIST/ReportList import remains unsupported` refusal. It also checks that the fixture trees are unchanged. This proves the candidate's current call order and exact input binding, not original runtime delivery or parity.

`finishCurrentOutputClassData` is a later private state prefix already wired after `prepareCurrentOutputSession` in `loadClassData`. For a selected existing `SfTaskDoe`, nonnull model/ROI model, empty regions and nonpositive constraint count, it copies the first task object/operation numbers into the actual Study and operation class state, resets the actual ROI model, applies the empty-region ROI/common-mesh rule, clears four flags and two lists, then returns. Its focused tests cover the ordered effects and partial state before first-region and positive-constraint refusal. However, it is **not reached** on the retained active B120 fixture because its direct `DBLIST` child is earlier in `readSessionRecords`; do not treat its unit fixture as active lifecycle reachability.

The existing `src/doe/dblist_reader.cpp` only returns direct-child tag/text values. It is not called from `readSessionRecords`, is not bound to the actual Session/Study owner, and does not model the native DBLIST side effects. Wiring it as success or skipping the guard would not be an evidence-backed next increment. The first new branch requires the DBLIST owner/effect contract at the encountered direct child, preserving all prior `.DS` and Session effects; ReportList remains a separate later payload. No active output file writer is source-closed past these guards.

## Disposition

Treat the Study-owned `.DS` read and pre-DBLIST Session prefix as the available source-closed private prefix; it is already implemented and exercised candidate-only. The next independent DOE Output gap is the actual DBLIST owner/effect branch, not ShapeOptim's directory helper, the table refresh receiver, or a detached output serializer. Keep the DBLIST/ReportList and selected-session/task guards until that owner is recovered. No source change, build, GUI/original execution or mutable REA operation was performed.

## Read-only source receipts

- `analysis/wave2-doe-lifecycle.md`: SHA-256 `16AEEB392A94BA82F1DBBBA7372B0E7678975BB6BEA41F0C0614B38A03D2474D`.
- `src/ops/noop.cpp`: SHA-256 `27542E5D9250EBA762A39E786F59F38048997B938142A2D490E8F4EA77B79FB4`.
- `src/doe/session.cpp`: SHA-256 `C57ADD172AE9B94C4AC6B731BD6E66AC7CF0886A5004F92C8886E056CDBEDEAC`.
- `src/doe/session_records.cpp`: SHA-256 `8E8410F7FB4596BC62E752B02969208E51C9CFCFAB402BA72727885BB3F50794`.
- `src/doe/output_data.cpp`: SHA-256 `3D317966C93944F5E5B88E7A54F1914E329723F69DBC35AEE63F09E207D833D8`.
- Candidate active guard test: `tests/mdt/test_doeoutput_active_guard.cpp`, SHA-256 `0EA39C3910535E4F8A72D5AB260A23D6DDB5A12ED4E6DD2AE8079BE06F6501C3`.
- Prior ShapeOptim frontier: `analysis/resume122-doe-report-owner-frontier.md`, SHA-256 `1DD47F99CAB5FD75F08D4E9B96A09C40ADC51A68341CFC90A58423A64FA30113`.
