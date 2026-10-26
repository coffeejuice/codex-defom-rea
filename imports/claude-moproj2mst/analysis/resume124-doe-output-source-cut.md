# CP124 DOE Output/Study Save source cut

## Finding

There is no new DOE Output/optimisation branch that can be added to the **actual public Study Save** before its current guard without changing native order or crossing an earlier Open guard. The source-closed actual-Study Save prefix already present is: append the owned Type2 Study node, write immediate InputTable XML (including ordinary false return), record the normal writer return and refresh boundary, then stop at real DoeTableRefresh delivery. The PAR/current-mode/mode-0/1/2 helper is source-qualified in isolation but belongs after refresh and cannot be moved ahead of it.

The retained B120 active DOE Output probe is an Open-time `SfOprDoeOutput` path: it reads the Study-owned `.DS` and stops at the `DBLIST`/`ReportList` guard. That Open does not return, so a subsequent public Save is not reachable on this case. Keep the Open DBLIST guard and the public Save refresh guard separate.

## Actual Type2 Study Save order and owner

The source requires one live `ConversionSession`/Project/owned Type2 SUB/Study, the matching Project `StudyIo` and `ModelManager`. It appends the Study node to a pending `Sub_Project`, then:

1. For an empty variable list, `writeType2InputTableXmlPrefix` immediately opens/truncates `DoeInputTable.XML` via the actual StudyIo, serializes the XML, closes and observes the post-close file error. Failed open returns ordinary false after recording the error. Both ordinary true and false reach the caller's refresh boundary.
2. `moproj/write.cpp` verifies the latest exact XML normal-return journal entry, records the refresh boundary, and calls the actual Session's `DoeTableOwnerState::requireRefreshDelivery`. That boundary still refuses because live QObject/QTimer/table registration and normal slot return are not owned by the candidate.
3. The caller then throws the prefix's next-boundary guard. No Type2 PAR or sample writer is called in the public Save path after the refresh guard.

The first source-ready next native operation is therefore **refresh delivery**, not output serialization. Its first receiver effect is the real table timer stop; the candidate currently has no matching owner. Moving or calling a later writer before this guard would change partial-effect order. Nonempty Study variables stop even earlier at variable-dimension reconciliation and do not reach XML.

## Existing isolated helpers and why they are not the next integration

`saveType2SampleSequenceImmediate` has the recovered four-call order (current selected mode, then fixed modes 0, 1, 2) and actual StudyIo writing semantics. It is not called by `moproj/write.cpp`; the native Type2 Save calls it only after InputTable XML and DoeTableRefresh return. It cannot be wired before `requireRefreshDelivery` as a workaround.

`finishCurrentOutputClassData` is also an existing private helper. It operates on the active Type2 Study, output document, actual OpClass references and actual ROI model for its empty-region/nonpositive-constraint prefix. But it is called from `DoeOutputClass::loadClassData` only after `.DS` `readSessionRecords` completes. The retained B120 `.DS` encounters `DBLIST` first, so that helper's unit test does not prove its reachability for the active fixture. Separately, `save_current.cpp` still guards DoeOutput's `vt+0x68` before the native Regions cleanup and `.DS` writer. No actual Study Save continuation can be inferred from the isolated output-data test.

ShapeOptim's `studyFolder` work is an active-operation Open effect before its later geometry guard. It is neither a public Study Save operation nor a way around DoeOutput's earlier `.DS` DBLIST guard or Type2 Save's refresh guard.

## Exact next source boundary

For the Type2 Study Save itself, the next integration dependency is one correlated normal refresh delivery on the actual main-window table generation: dispatcher registration/weak guard selected, same receiver matched to `mainwindow+0x458`, its child timer stop and same-object slot/refresh normal return. Only after that boundary may the actual Study-owned immediate PAR sequence be connected in native order. Do not add a caller-provided success flag or call the helper directly in public Save.

For the B120 DoeOutput Open path, the exact earlier reached refusal is `readSessionRecords` at the first direct `DBLIST`/`ReportList` child; resolving it requires the actual Session/Study DBLIST effect owner and encounter order. A tag/text-only reader is not an import. Even after that Open boundary, DoeOutput selected Session/global task application remains separately guarded.

## Scope and receipts

Read-only source/retained-report review only; no source edit, build, GUI/original execution or mutable REA operation.

- Public Study Save writer path: `src/moproj/write.cpp` and `src/engine/save_current.cpp` (the DOE table refresh refusal and DoeOutput vt+0x68 guard).
- Study-owned immediate output helper: `src/doe/doe_study.cpp` (`saveType2SampleSequenceImmediate`); not called from public Save past the guard.
- Existing `.DS` output/load state prefix: `src/doe/session.cpp`, `src/doe/session_records.cpp`, `src/doe/output_data.cpp`; B120 active guard fixture verifies `.DS` read then DBLIST refusal candidate-only.
- Native order reference: `analysis/wave2-doe-lifecycle.md`, SHA-256 `16AEEB392A94BA82F1DBBBA7372B0E7678975BB6BEA41F0C0614B38A03D2474D`.
- Active DoeOutput candidate probe source: `tests/mdt/test_doeoutput_active_guard.cpp`, SHA-256 `0EA39C3910535E4F8A72D5AB260A23D6DDB5A12ED4E6DD2AE8079BE06F6501C3`.
- Prior source-cut report: `analysis/resume123-doe-output-next.md`, SHA-256 `2C7C2DF9B00C16DE227BEBA97AC192742FE5C7EB1A403A59175AE98AEF673D94`.
