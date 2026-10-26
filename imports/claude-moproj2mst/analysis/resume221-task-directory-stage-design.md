# CP221 task-directory staging design

## Recommendation

Add one internal, no-I/O stage for the `taskDirAbs(op)` attempt that precedes the project-directory attempts. Keep `Ctx::stagedOperationDirectoryDecisions` as the existing project-only five-or-six-path contract. Append the task path to a new per-call ordered attempt record, then append the existing project paths to that record in their source order. Do not feed either vector to `FileEffects`, native `mkpath`, or an exported C API.

The smallest robust record is an internal struct with a cleaned UTF-16 path and an outcome-policy enum/value that records `ReturnIgnored`; the current scope has only that policy. A simple ordered `vector<u16string>` is even smaller if the API is explicitly documented as an attempt sequence and the ignored-return fact stays in the contract/test. Prefer the struct if later attempts can have distinct return handling. This is metadata describing the source sequence, not an assertion that a candidate filesystem attempt happened.

## Exact insertion point and path eligibility

In `parseOperation`, the `<Type>` branch first replaces ID zero with the document-order ordinal, then calls `createOperation`. The task-directory stage belongs immediately after successful `createOperation`, before collecting/appending project-directory paths. At that point the operation has its effective ID, project pointer, and initialized task directory, and the active subproject can be sampled once for the conditional project path. This models the assembly order in `0x140051190` (task path first) followed by `0x14008c630` (project paths).

The original caller gate is `effective id >= 1` (`0x140050820`); both the task attempt and project group are absent for a negative ID. Zero is not a remaining case at this point: it has already become the operation ordinal. When eligible, derive the first path as `taskDirAbs(*item)` and clean it using the same helper semantics as that function; do not rebuild its `Task%05d` spelling independently. Then record the five project paths in order (`Operations`, `Objects`, `Materials`, `Equipments`, `Lubricants`), with `DieStress` last only when an active subproject exists and its type is 4. The operation ID gate is independent of the active Type4 predicate.

The static evidence describes sequential calls whose returns are never tested. A failed mkdir can display the warning dialog, after which control continues to the next call once dismissed. Record this as an ordered, ignored-return policy; do not convert it into short-circuiting or success/failure gating. This is a recovered control-flow property, not a claim about actual directory results or GUI timing in a particular original run.

## Failure and compatibility behavior

Append only after `createOperation` returns, as the current project-path collector does. A failure before operation construction contributes no task or project paths. Once the `<Type>` branch has returned from construction and appended its group, a later parse error retains that prefix in `Ctx`, matching current partial-parse behavior. Repeated paths and operations remain repeated and ordered; do not deduplicate.

Do not prepend the task path into `stagedOperationDirectoryDecisions`: existing CP218/CP220 tests and consumers interpret that vector as project-level paths (five or conditional six). A new sequence vector avoids silently changing its shape and lets callers opt into the richer contract. Keep the public C ABI untouched. `Ctx` is source-visible C++ and adding a member changes its layout, as the current staged member already does; inspect any separately compiled C++ consumers before treating it as ABI-stable. No evidence supports a binary ABI promise for `Ctx`.

Save's `engine::makeFolders` already performs a task-directory attempt followed by the same five project directories and optional Type4 `DieStress` path, and distinguishes native attempt records from deferred mkdir effects. That is a separate Save execution path with real/recorded effects; it is corroborating structure, not a reason to call `makeFolders` from Open. Reuse only pure path derivation and the source-ordered metadata concept. CP220's five-plus-optional-six Open decisions remain frozen as project-only staged metadata; preserve that contract.

## Focused test additions

Extend the staged parser test (or add a focused companion) to assert:

1. A positive ID adds one cleaned `taskDirAbs` path before its five project paths; ID zero uses its ordinal and does the same.
2. A negative ID adds neither task nor project attempts (while the separate native-blob regression remains unchanged).
3. An active Type4 project adds `DieStress` after the five fixed paths; null and non-Type4 active subprojects omit it.
4. Multiple operations preserve task-first sequence, order, and multiplicity, while the legacy project-only vector still has exactly its former five/six entries per eligible operation.
5. A malformed later operation preserves the already-staged prefix; a failure before `createOperation` returns adds no group for that operation.
6. Every staged item carries/obeys `ReturnIgnored`; the sequence is not translated into `FileEffects.mkdirs` or native attempts, and scratch contents remain unchanged.

The frozen CP220 result (366 registered: 359 pass, six logged gaps, one logged difference; historical 93 SAME/five DIFF) establishes candidate project-path staging/tests only. It does not establish original Open completion, actual mkdir occurrence/results, or permission for public writes. CP219's original B98 comparison observed candidate artifacts matching the retained original filesystem files, but did not bind a completed original Open or the `0x140050820 -> 0x140051190 -> 0x14008c630` occurrence. Keep the write gate closed pending that direct same-occurrence witness.

## Evidence consulted

- `analysis/wave3-earlier-save-140051190.asm` and `analysis/wave3-earlier-save-14008c630.asm`: task-path-before-project-path call order and the five-plus-optional-six project sequence.
- `analysis/resume220-conditional-directory-static.md` and `analysis/resume220-conditional-directory-review.md`: effective positive-ID gate, Type4 predicate, ignored returns, and inert Open staging boundary.
- `source/apps/moproj2mst/src/model/opinfo.cpp`: `taskDirAbs` and `makeTaskDir` path derivation.
- `source/apps/moproj2mst/src/model/operation_rescue.cpp`, `operation_project_directories.hpp`, and `include/m2m/common.hpp`: current parser insertion point, collector, and `Ctx` contract.
- `source/apps/moproj2mst/src/engine/save_current.cpp`: Save-side `makeFolders` path and attempt ordering.
- `analysis/wave3-checkpoint220-validation.json` and `analysis/resume220-native-blob-test-review.md`: CP220 frozen candidate scope and CP219 evidence qualification.
