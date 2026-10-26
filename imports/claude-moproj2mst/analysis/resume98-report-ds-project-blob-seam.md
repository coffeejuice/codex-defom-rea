# CP98 — Report `.DS` helper and inherited project-blob seam

## Finding

There is a narrow, source-bound project-blob seam: the active `Project` writer resolves an object-property KEY through prior Save effects, the Project overlay, then disk, and appends a compressed `<Data>` child only when bytes are available. This branch is integrated in the full candidate Save path and has a public-conversion lifecycle test. It is independent of Report `.DS`.

Report `.DS` path derivation is settled only as a pure helper. It has no production caller and cannot be safely wired into Report Open/Save without the same active SubProject's actual `+0x30` document and its lifetime. Existing Report Open and Save refusals remain correct. No additional source change is justified by the retained evidence.

## Owner-bound project-blob seam

- `src/moproj/write.cpp::readProjectFile` normalizes the requested path and resolves earlier deletes/writes in reverse order, then `Project::overlay`, then disk. The `Project&` and prior-effect list are explicit owners/inputs; unreadable and absent files return false.
- `src/moproj/write_ops.cpp::blobElement` maps empty path or failed read to null; successful reads become `Data` containing hex of `qCompress(bytes)`. The operation-list writer always creates and appends the `ObjPropertiesKey` parent, then appends the possibly-null Data element. It does not fail the surrounding project serialization on this local omission.
- `saveProjectFileBody` invokes `operationList` before active SubProject DOM replacement and project serialization. The output-device path then closes and queries the real project output through the project writer service.
- CP80/81 evidence closes the unreadable-existing-side-file distinction and the serialized-parent consequence for one retained fixture profile. `tests/s/test_project_blob_lifecycle.cpp` drives two fresh full candidate conversions, holds a strict read denial through project output close/error observation, and checks that only the selected `Data` is omitted while its parent remains and later Save effects continue. `resume84-integrated-grouped-tests.json` records the integrated suite receipt; it is candidate validation, not original-runtime parity.

This is the smallest owner-bound inherited project-blob integration point: `Project` + ordered `FileEffects` + selected operation/object KEY path, exercised by the existing full-conversion Save fixture. Do not generalize this bounded KEY branch to `withBlobs` archive behavior, other read failures, or Report document ownership.

## Report `.DS` seam and guard

The native Report Save first calls its vt+0x80 document routine, which derives the sibling `.DS` path and calls the document method with that path and mode 1. The Report operation then calls a separate viewer-model routine before its later property serialization. The native Report Open vt+0x78 likewise reads `.DS` through the active SubProject document.

Candidate `src/ops/report_ds_path.cpp::reportDsPath` covers only the path expression and has focused path tests; call-site search finds no production consumer. `src/ops/report.cpp::loadClassData` still refuses before document access/construction. `src/engine/save_current.cpp` refuses Report before either document or viewer Save callback. The path helper is therefore not the native consumer seam.

Static evidence binds lazy document reuse/publication at `SubProject+0x30`, constructor `0x140643100` with the actual main-window QObject parent, and the Report document deleting target. It does not close the constructor's transitive Qt ownership effects, same-instance SubProject adoption, accepted Close release order, or the preceding QObject parent teardown. The existing `ReportDocumentSlot` helper only models declared slot observations and deliberately stops before nonnull deletion; it does not provide a constructed Report document. The viewer callback's returned model/iteration is a second independent unmet dependency.

## Narrowest next gate

There is no source-closed Report public Open/Save seam to implement now. The smallest gate is a same-instance owner receipt binding the active Project and SubProject, actual main-window parent, constructed/reused document pointer published to that SubProject's `+0x30` guard, and its accepted-Close release relative to QObject parent teardown. That alone would permit reconsidering the document call; public Save would still require the source-bound viewer-model return owner and per-entry effects of `0x1412af0c0` in native order.

Until those edges are established, keep Report Open and Save guarded and leave `reportDsPath` as a pure helper. Do not synthesize a document, infer null from empty candidate metadata, or skip the viewer callback. The project-blob Save seam is already the concrete independent public-path integration; it is not a substitute for Report `.DS`.

## Scope

Read-only retained-evidence and current-source/test reconciliation. No source, test, build, GUI, original-execution, or mutable REA action was performed.
