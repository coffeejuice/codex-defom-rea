# CP123 ShapeOptim Type2 study-name integration review

## Owner identity

The new nonempty-name branch is narrowly tied to the actual Type2 owner shape. `makeSubProject(p, 2, id)` retains a concrete `doe::Study` in `ownedDoeStudy` and points the base `doe` alias at that same object; the study's `sub` backpointer points to the owning `SubProject`. Type2 XML loading updates that same study's `name`. `ShapeOptimClass::studyFolder` checks the caller Project against the class Project, `sp->project`, Type2 discriminator, nonnull owner, base-alias pointer identity, Study-to-SubProject backpointer, and nonempty imported name before consuming it. These checks avoid accidentally using a detached manager or Type3's separate optimization-study object. Empty imported names and aliases outside this precise shape are still refused; no claim is made for their original string/owner behavior.

This makes the candidate's name value traceable through the retained owner. It does not independently prove native QObject/QString ownership or cover the native `sp+0x20 -> +0x98` lookup for every malformed or empty study record.

## Path construction

The candidate concatenates `projectDir`, `DoeStudies`, the study name, `Control`, and `Opt`, with the platform separator between components and a trailing separator after `Opt`. That matches the bounded native `studyFolder` construction and the Type2 nonempty-name case. If no DOE alias exists, the candidate leaves the name empty and retains the existing empty-name path behavior; the nonempty Type2 increment does not widen that branch. Qt separator normalization and a real project-root spelling remain dependent on the existing project path owner.

The report's focused test checks a generated `DoeStudy0` directory below the disposable project root, under 100 path characters. It validates the candidate owner shape and path assembly; it does not establish behavior with arbitrary imported names or at a user project path.

## Attempt ordering and earlier guards

Native `baseLoad` invokes `loadClassData` before `loadData`. The ShapeOptim `loadClassData` slot (`0x14013d740`, in `source/apps/moproj2mst/src/ops/report.cpp`) returns for missing Project/SUB and refuses Type3 before the later `loadOperation` tail reaches `loadData`. Thus the Type2 name increment does not bypass the earlier Type3 guard. In `loadData`, the native `studyFolder`/`QDir::mkpath` attempt occurs before the later nonempty `listF0_` GEO-file guard; a later GEO refusal does not undo the earlier directory attempt.

The candidate `loadData` consumes the queued mkdir and calls `nativeMkpathAbsolute` immediately only when `ctx.nativeOperationBlobs || fx.nativeDirectories` is true. On successful creation this preserves the path's position before the GEO guard for that profile. The native helper is called with dialogs enabled; on failed `mkpath` it displays a modal warning, ignores the dialog result, returns true, and continues. The candidate instead records failure and throws `M2M_E_UNSUPPORTED` at the unrecovered modal-delivery boundary. That is a deliberate limit, not parity for failure continuation.

For contexts where both immediacy flags are false, the candidate leaves the directory in `FileEffects::mkdirs` for later application. If a subsequent GEO guard exits before those effects are applied, this would lose an attempt that native `loadData` already made. The current CP122 report qualifies the immediate path as the full `WRITE_SAVE` Open callback and separately records deferred behavior. Keep any resulting claim limited to the immediate profile; verify the caller's mode before asserting that a public Open-only or other mode preserves native ordering.

## Review result and bounds

Within the source-qualified nonempty Type2 owner and immediate-success profile, identity and path construction are coherent, the earlier Type3 guard remains before the directory attempt, and the GEO guard remains after it. Remaining gaps are the failed-mkpath modal continuation and deferred-mode ordering if a later guard prevents queued effects. Empty study names, unknown owners, Type3 optimization-study state, populated native GEO loading, arbitrary imported-name encoding, and GUI modal delivery are outside this review's supported path.

Read-only source and retained-evidence review only. No source edits, build, GUI/original execution, or mutable REA operation were performed.
