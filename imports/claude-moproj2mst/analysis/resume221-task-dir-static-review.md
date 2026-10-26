# CP221 task-directory attempt: static review

## Finding

The retained original evidence establishes the task-directory attempt as the
first directory effect in the operation-ID initialization path. The operation
ID setter `0x140050820(OPI, id)` stores the ID and enters its directory block
only for `id >= 1`. In that block it first stores
`Operations/Task` plus the decimal ID zero-padded to five digits at `OPI+0x130`,
then calls `0x140051190(OPI)`. That helper derives
`taskDirAbs(OPI)` and calls `0x14081afb0(path, true)`; after it returns, it
resolves the project pointer and calls `0x14008c630(P)`. Thus the project-folder
attempts follow the task attempt, not the reverse.

For an operation with an attached project, the task path formula is
`convertSeparators(cleanPath(projectDir(P) + separator + taskDirRel(OPI)))`.
`projectDir(P)` is the cleaned absolute project path. The path helper returns a
null string if the operation has no project pointer; the ordinary Project Open
path carries `P` into operation creation. This is static path construction,
not proof that the native attempt succeeds or that a particular Open reached
it.

## Call and failure behavior

The retained `0x140051190` assembly has a straight-line sequence: call
`0x140050df0` for the absolute task path, call `0x14081afb0` with `DL=1`,
resolve the project from the operation, call `0x14008c630`, then destroy the
temporary string and return. It does not branch on the path helper's return.
The `0x14008c630` body similarly makes five ordered calls for `Operations`,
`Objects`, `Materials`, `Equipments`, and `Lubricants`, then optionally attempts
`DieStress` when an active subproject exists and its type predicate is true.
None of these call sites checks the `0x14081afb0` return.

The retained helper qualification says `0x14081afb0` invokes `QDir::mkpath`;
on failure with warning enabled it presents the one-OK Information dialog,
ignores the dialog result, and returns true. Therefore a false filesystem result
does not select a caller-side error branch: after the modal returns, execution
continues to the next directory attempt. The task call precedes the fixed
project paths and the optional DieStress path. The task helper excerpt itself
does not establish the precise dialog dispatch/answer timing for any actual
run. No explicit exception handler or recoverable exception branch is visible
in the retained `0x140051190` instruction range; behavior under allocation,
Qt, or other exceptional failures is not established by this evidence and
should not be described as ordinary mkdir failure behavior.

## Candidate staging and public-write boundary

The candidate's internal `collectOperationProjectDirectoryDecisions` is
source-closed for its represented project-directory decisions: after the
effective ID is chosen (zero becomes the document ordinal), it records the
five cleaned paths in the retained order for IDs at least one, and appends
`DieStress` when the active subproject type is 4. `parseOperation` stores those
paths in per-conversion `Ctx` metadata. The helper performs no I/O, does not
append to `FileEffects::mkdirs`, and is not consumed by `applyEffects` or a
writer. This is an internal decision record, not evidence of the original
runtime attempt.

That staging is not a complete representation of the full original directory
attempt sequence: it currently omits the first `Operations/TaskNNNNN` path from
`0x140051190`. The candidate does set the relative task path in
`setOperationId`, but the attached comment explicitly says the `0x140051190`
mkdir side effect is not done. Any claim that all task-and-project directory
decisions are already staged would therefore be too broad.

Public writes remain unproven and gated. Before applying the decisions as
filesystem writes, evidence still needs to establish a comparable original
Open occurrence binding the effective ID and project pointer to the full
ordered call chain, including the task path, all `0x14081afb0` attempts and
results, and failure/modal continuation. Static source and disassembly do not
prove an occurrence, prior directory state, successful `mkpath`, actual modal
delivery, or parity of candidate public writes. Save and accepted Close remain
separate behavior gates.

## Evidence reviewed

- `analysis/wave3-earlier-save-140051190.asm`: task-path helper call, warning
  flag, subsequent project-directory helper call, and absence of return tests.
- `analysis/wave3-earlier-save-14008c630.asm`: fixed project path order and
  conditional `DieStress` decision.
- `source/docs/re/moproj2mst/s1-01/spec.md` §5.3: `id >= 1` gate, task
  relative-path format, and `taskDirAbs` formula.
- `analysis/wave2-doe-lifecycle.md` and
  `analysis/wave3-directory-failure-dialog-contract.json`: shared helper's
  always-true return and warning/modal qualification.
- `source/apps/moproj2mst/src/model/operation_rescue.cpp`, `opinfo.cpp`,
  `project.cpp`, and `operation_project_directories.hpp`: candidate relative
  task path, absolute task path, project-root normalization, and inert staged
  project-directory decisions.
- `analysis/resume218-staged-open-dir-implementation.md` and
  `analysis/resume220-conditional-directory-static.md`: existing staging and
  project-path branch qualifications.

This review uses retained static evidence only. It makes no claim that the
original GUI or filesystem was exercised in CP221.
