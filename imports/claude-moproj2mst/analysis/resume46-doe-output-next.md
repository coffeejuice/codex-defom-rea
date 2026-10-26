# Checkpoint 46: DOE Output task and StepData path

## Active path and first blockers

For a project whose active operation is `SfOprDoeOutput`, the lifecycle reaches `DoeOutputClass::loadOperation` during `openActiveOperation` before the later Save stages. That method resets the output class state, resolves the preceding/last operation dimension, resets and displays its model, and invokes `loadClassData`.

`loadClassData` then requires an active Type2 subproject, an owned DOE study, and configured Study I/O. It reads the output `.DS` document (`prepareCurrentOutputSession`), then calls `readSessionRecords` before `finishCurrentOutputClassData` can select a `SfTaskDoe` record. The first guards depend on document content:

1. An imported Session with an Image reaches `importSession`'s unsupported QImage decode immediately (after qUncompress diagnostics).
2. A document with no imported parts reaches the unsupported default-part construction guard.
3. An imported Session without Image reaches `createTask`. The native `TaskFactoryRuntime::createTaskBase` initializes the 17-prototype factory prefix before looking up the requested type. That prefix retains a Report prototype allocation and stops at the first Report `QObject(parent=null)` construction. This is the first guard on the ordinary populated-session path and is the outstanding Report QObject/TLS/shared-owner dependency.
4. Only after record import does `readSessionRecords` process child payloads. `StepData` without `LayerName` dispatches to `readStepData` when a Task exists; layered StepData refuses. A populated Task `OutputList` refuses in `readTaskLists`; `OprList` and `ObjList` have parsed paths. A `SfTaskDoe` record with no Task skips StepData import.
5. After document parsing, the class-data helper selects the first `SfTaskDoe` in the selected part, or constructs a default one if absent. The default path again dispatches the native factory and therefore hits the Report QObject guard. A matching null Task returns without class-state mutation. A non-null Task copies first object/operation selections, resolves and resets the ROI model, then handles regions and constraints. The implemented helper supports the zero-region/nonpositive-constraint branch and refuses at the first region geometry payload or positive constraint payload.
6. Even after class data returns, `loadOperation` currently stops at the explicit selected-session/global-task application guard. Save step 2 later has a separate `SfOprDoeOutput` guard: native decompile/source qualification says vt+0x68 writes the DOE sample object through the viewer model and updates the DOE output document. The normal Save driver applies step-2 file effects only if `saveCurrent` returns, so neither a queued effect nor the serializer helper proves a completed output write.

The ordering above is grounded in `ops/noop.cpp`, `doe/session.cpp`, `doe/session_records.cpp`, `doe/task_factory.cpp`, `doe/task_lists.cpp`, `doe/stepdata.cpp`, `doe/output_data.cpp`, `engine/save_current.cpp`, and the checkpoint-46 QObject prerequisite contract. The contract explicitly says no factory/OQObject effects are closed and the unsupported guard must remain before TaskInfo. No Report or QObject state should be inferred from test-only parsed projections.

## Output writer call order and side effects

The current source has a StepData **reader** (`readStepData`) and a default-session **writer** call (`writeStepData`) inside `constructDefaultDoeSession`. That writer creates a detached StepData node, writes StateVar first, then viewer Object StepData. It is reached only after native factory/task creation and the default-session record setup; the current function still stops at the default-session snapshot guard, and the detached Session is not appended before all payload work returns. It is therefore not evidence of a usable Save writer.

`saveCurrentOperation`'s DOE Output branch is earlier than generic operation property writers and the final MST write. Its qualified call-order summary names the viewer sample-object write and DOE document write, but the implementation keeps a single unsupported guard before those effects. The reviewed source does not establish their relative internal order or partial-failure boundary, so do not claim one from the summary. `saveCurrentSamples` / `saveSampleTable` in `doe_study.cpp` writes `Control/DEF_DOE.PAR` as a standalone file-effect helper; no active lifecycle call site was found in the current source search. It should not be reported as a Save effect of this DOE Output operation.

## Smallest independently implementable branch

Implement and validate a narrow `readTaskLists` branch for an **empty** `OutputList` child: preserve it as an empty list and continue to later children, while keeping the existing refusal for any populated OutputList payload. This is the smallest cleanly bounded task-parser extension, has a direct source hook in `task_lists.cpp`, and does not require changing QObject/TLS state, creating a Report prototype, or relaxing the native factory guard. The current code already implicitly skips an empty `OutputList`; the follow-on should make that behavior explicit in a contract/test only if source-backed original evidence distinguishes the empty element from absence. Do not broaden it to populated entries: `0x14140cf80` dispatches their children and their output-object grammar remains unrecovered.

This parser branch is locally implementable but not yet reachable through the current native lifecycle for a normal imported Session, because task creation initializes the Report prototype first. It is therefore an isolated source-qualification improvement, not lifecycle progress or acceptance. If the intended branch must be reachable end-to-end without Report TLS, no such branch is currently source-qualified: bypassing factory initialization for `SfTaskDoe` would contradict the existing recovered constructor order. Keep the guard until shared TLS/thread and QObject ownership prerequisites are satisfied.

## Validation limits

Checkpoint 45 lists DOE corpus cases as comparison diffs; the recovery plan says full Save remains unvalidated. The DOE output-data contract is a bounded static/helper projection, not original runtime acceptance. This audit ran no build, test, original process, GUI session, or REA mutation.
