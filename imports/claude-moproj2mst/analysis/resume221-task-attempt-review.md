# CP221 task-attempt source review

## Verdict: PASS, with one explicit coverage limit

The implementation matches the retained static contract for task-first directory-attempt metadata. The task path is staged only for effective IDs at least one, after `createOperation` has initialized the operation and before project paths are appended. The old `stagedOperationDirectoryDecisions` vector remains project-only and preserves its five/six-path semantics.

No exact fix is required for the reviewed scope. The root's configured MSVC build/test remains the required compiled validation; this review does not count the earlier environment-failed build as a pass.

## Findings

- **Effective-ID gate and ordering are correct.** `parseOperation` converts zero to the document ordinal before `createOperation`. `createOperation` calls `newOprItem`/initialization with that ID, and the parser appends `taskDirAbs(*item)` before iterating the existing project decisions. Both task and project decisions are omitted for negative IDs because the task is guarded by `id >= 1` and the collector itself returns for `operationId < 1`. This matches the retained `0x140050820 -> 0x140051190 -> 0x14008c630` sequence.

- **The path source and project sequence are sound.** `taskDirAbs` uses the operation's attached project, relative task path, cleaned project directory and separator normalization. It does not perform filesystem work. The project helper returns cleaned `Operations`, `Objects`, `Materials`, `Equipments`, `Lubricants` paths, then conditionally `DieStress` for active subproject type 4. The attempt vector mirrors that order and retains duplicates.

- **Return policy is modeled, not executed.** Each attempt stores `ReturnIgnored`, consistent with the disassembly's unchecked helper returns and the qualified helper/dialog contract. No `mkpath`, `FileEffects`, writer, or filesystem API is called from the added code. The existing project-only vector is appended separately and unchanged. No exported C declaration or C ABI behavior is modified. `Ctx` is a source-visible C++ struct whose layout grows; the inspected C API is separate, and no binary ABI guarantee for `Ctx` is established by the reviewed evidence.

- **Partial parse behavior and multiplicity are preserved.** Staging occurs only after `createOperation` returns, so a construction failure cannot append that operation's attempts. Later parser failure leaves earlier operations' staged prefix in the context, as before. The test covers malformed later input and repeated positive-ID paths, and checks scratch-tree preservation.

## Test limits

The staged parser test exercises effective positive and ordinal-derived IDs, a negative ID, repeated operation paths, task-first order, `ReturnIgnored`, project-only vector shape, a malformed trailing operation, and no filesystem change. It does **not** demonstrate an active Type4 project variation or assert the optional `DieStress` attempt; its expected path set is exactly five project paths per eligible operation. Therefore the Type4 branch is supported here by source review of the existing collector and retained static evidence, not by this new test. Add an explicit active-Type4 fixture/assertion if test coverage for that conditional is required by the merge bar.

The test's `FileEffects` assertion checks a newly default-constructed empty object, so that assertion alone does not prove the attempt metadata was never translated into effects. Source search confirms the new vector has only the parser append sites and the test reads it; no production consumer or effect translation is present in this subtree.

## Evidence inspected

- `source/apps/moproj2mst/include/m2m/common.hpp`
- `source/apps/moproj2mst/src/model/operation_rescue.cpp`
- `source/apps/moproj2mst/src/model/opinfo.cpp` (`makeTaskDir`, `taskDirAbs`)
- `source/apps/moproj2mst/src/model/operation_project_directories.hpp`
- `source/apps/moproj2mst/tests/model/test_staged_operation_directories.cpp`
- `analysis/resume221-task-dir-static-review.md`
- `analysis/resume221-task-directory-stage-design.md`
- `analysis/resume221-task-attempt-implementation.md`
- `analysis/wave3-earlier-save-140051190.asm`
- `analysis/wave3-earlier-save-14008c630.asm`

