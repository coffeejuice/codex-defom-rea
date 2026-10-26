# CP264 task-directory static reconciliation

## Result

Static source reconciliation matches the retained original call sequence. I found no remaining source discrepancy in the requested task-plus-project directory staging leaf, and made no source or test edits.

At the `<Type>` branch, `parseOperation` converts ID zero to the document ordinal before `createOperation`. After successful construction, it derives the task attempt with `taskDirAbs(*item)` only when the effective ID is positive, then stages the project decisions returned by `collectOperationProjectDirectoryDecisions`. That collector independently applies the same `id >= 1` gate and emits cleaned `Operations`, `Objects`, `Materials`, `Equipments`, and `Lubricants` paths in order; it appends `DieStress` only for active subproject type 4. The project-only `stagedOperationDirectoryDecisions` vector retains its prior five/six-path shape.

Each path in the full attempt vector receives `ReturnIgnored`. The collector and parser staging do not call `mkpath`, add `FileEffects.mkdirs`, or feed a writer. This is an inert record of requested order, not a filesystem result.

## Static sequence cross-check

`wave3-earlier-save-140051190.asm` shows `taskDirAbs` followed by `0x14081afb0(path, true)`, then resolves the project pointer and calls `0x14008c630`. It tests neither helper nor creator result. `wave3-earlier-save-14008c630.asm` shows the five fixed project paths and helper calls in the same order, then checks for a non-null active subproject and its type predicate before the optional `DieStress` helper call. Those helper returns are also not tested. The candidate's task-first attempt vector and project collector encode these same gates and order.

The retained helper/dialog contract qualifies a failed warning-enabled `mkpath` as presenting the one-OK information dialog, ignoring the dialog result, and returning true. The caller therefore has no helper-result branch; after modal dismissal, static control continues to the next attempt. This does not establish an actual dialog occurrence or its timing in an original run.

## Existing caller-test coverage

`tests/model/test_staged_operation_directories.cpp` checks the effective positive/zero IDs, excludes the negative-ID row, checks task path before each eligible operation's five project paths, verifies `ReturnIgnored`, and checks that staged metadata does not alter scratch contents or create `FileEffects` directories. `tests/model/test_operation_project_directories.cpp` separately checks cleaned fixed-path order, negative-ID suppression, and the conditional Type4 path. The call-site logic is directly visible in `src/model/operation_rescue.cpp`. Existing coverage is sufficient to identify the staging behavior; no new test was run or requested here.

## Exact next missing witness

The static match does not establish an original occurrence. As specified by `resume264-original-cli-next-probe.md`, the next missing witness is one qualified, same-child positive-ID B98 Open trace binding the accepted outer Open to entry and normal leaf of creator RVA `0x8c630` and the fifth `0x81afb0` helper occurrence (return RVA `0x8c77a`), with `RCX` decoding to that clone's `Lubricants` path while acceptance remains active. The task helper return RVA `0x511d0` identifies the creator caller. The trace also needs the existing qualified outer acceptance/return evidence, ownership and cleanup predicates, and independently qualified new-hook instruction/patch/nesting checks. No such observer route is currently approved; this static review does not authorize or execute one. The helper/mkdir causal witness remains an additional requirement if claiming actual directory creation rather than a helper path attempt.

## Evidence reviewed

- `analysis/resume264-original-cli-next-probe.md`
- `analysis/resume221-task-dir-static-review.md`
- `analysis/resume221-task-directory-stage-design.md`
- `analysis/wave3-earlier-save-140051190.asm`
- `analysis/wave3-earlier-save-14008c630.asm`
- `analysis/wave3-directory-failure-dialog-contract.json`
- `source/apps/moproj2mst/src/model/operation_rescue.cpp`
- `source/apps/moproj2mst/src/model/operation_project_directories.hpp`
- `source/apps/moproj2mst/src/model/opinfo.cpp`
- `source/apps/moproj2mst/include/m2m/common.hpp`
- `source/apps/moproj2mst/tests/model/test_staged_operation_directories.cpp`
- `source/apps/moproj2mst/tests/model/test_operation_project_directories.cpp`

Read-only static reconciliation only. No source, tests, shared state, build, GUI, original process, Frida, or mutable REA session was touched.
