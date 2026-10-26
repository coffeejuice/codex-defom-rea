# CP220 conditional project-directory decision: static closure

## Finding

The retained evidence source-closes the conditional sixth project-directory decision made by `0x14008c630`. Its owner is the `Project* P` passed to that helper from `0x140051190`, reached by `0x140050820(OPI,id)` only when `id >= 1`. After the five unconditional project-level directory attempts, it resolves `activeSubProject(P)` (`0x1400a7390`). The additional decision is present only when that pointer is non-null and its `SubProject::type` is `4` (Die Stress). Its path is:

```text
cleanPath(projectDir(P) + separator + "DieStress")
```

`projectDir(P)` is `cleanPath(absolutePath(P.path))`. This predicate is about the active subproject, not the operation's class or ID; the positive-ID gate is the caller's separate condition. Thus the full static condition is: operation ID at least 1, active subproject exists, and active subproject type equals 4.

## Assembly order and return handling

At `0x14008c630`, the five fixed calls precede the active-subproject lookup, in this order: `Operations`, `Objects`, `Materials`, `Equipments`, `Lubricants`. Each returned path is passed to `0x14081afb0` with warning enabled (`DL=1`). Only after these calls and their temporary-string destruction does the helper call `0x1400a7390(P)`. Null skips the conditional block. A non-null result is tested by `0x140010390`; false also skips it. True calls `0x1400bc160(sub, &temporary)` and then `0x14081afb0(&temporary, 1)` for the DieStress path.

The caller never tests a return from `0x14081afb0`: there is no conditional branch after any of the five fixed calls or the optional sixth call. The retained lifecycle analysis says this helper returns true even when `QDir::mkpath` fails; with warning enabled it presents a single-OK information dialog on failure and continues after dismissal. Therefore the statically recoverable failure order is sequential: the task directory attempt in `0x140051190`, then the five fixed project directories in the order above, then the conditional DieStress attempt. A failed attempt can introduce its warning modal before the next attempt, but does not change this helper's caller-side control flow. This does not establish actual filesystem results or a runtime occurrence.

## Candidate implication

A pure internal staged-decision increment is justified: for each qualifying positive-ID operation, append the conditional cleaned DieStress path after that operation's five fixed project paths exactly when the candidate's active subproject exists and has type 4. Keep it as decision metadata only; do not feed it to `FileEffects.mkdirs`, native `mkpath`, or a public ABI path. This extends the existing CP218 five-path collector and should preserve per-operation order and multiplicity. The evidence does not authorize performing the write or claim that the original application actually attempted it in a particular run.

## Evidence and limits

- `analysis/wave3-earlier-save-14008c630.asm`: direct call/branch order, active-subproject lookup, predicate, path-helper call, warning flag, and lack of return checks.
- `analysis/wave3-earlier-save-140051190.asm`: caller order; it attempts `taskDirAbs(OPI)` before calling `0x14008c630(P)`.
- `source/docs/re/moproj2mst/s1-01/spec.md` §5.3: `id >= 1` gate and the five fixed directory names.
- `evidence/gui_mo/s1/s1-01/spec.md` §8.1 and `analysis/resume172-headless-oracle-next.md`: SubProject `+0x18` is Type and value 4 is Die Stress; `0x140010390` is the Type-4 predicate.
- `source/apps/moproj2mst/src/model/project.cpp`: active-subproject selection and project-directory normalization.
- `source/apps/moproj2mst/src/model/paths.cpp`: `0x1400bc160` path is project directory plus separator plus `DieStress`, cleaned.
- `source/apps/moproj2mst/src/engine/save_current.cpp`: candidate Save-side `makeFolders` already models the same active-subproject/type/path condition and records native attempts separately from staged Open metadata.
- `analysis/wave2-doe-lifecycle.md`: `0x14081afb0`'s always-true return and warning-on-failure behavior.
- `analysis/resume218-staged-open-dir-review.md`: existing five-path staged decisions are inert metadata and are not evidence of original runtime attempts or write authorization.

This is a static source/assembly conclusion for the retained original binary and current candidate source. It does not prove a same-run active-subproject value, a directory's prior existence, a native mkdir result, or warning-dialog timing.
