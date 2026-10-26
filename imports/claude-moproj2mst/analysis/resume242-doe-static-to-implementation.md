# CP242 DOE static evidence to implementation

## Decision

No new internal DOE production increment is source-ready from CP240. Keep the ordinary Open/Save path unchanged and do not add an orphan Add Study helper to `Project`: the recovered body is a GUI transition with owner, DOM, active-selection, activation, and initial-operation effects, not a passive existing-study reader/writer prefix. The static call chain is now qualified, but it does not establish which branch or post-creator effects occurred in B120.

## Current-source reconciliation

The converter's public path in `src/core/convert.cpp` opens the project and calls `activateSubProject(p, 0, ctx)`. `Project::activeSub` and `subs` in `include/m2m/model.hpp` are the state read by existing converter paths. `src/model/subproject.cpp::makeSubProject` creates the converter's normal SUB and owned Type2 Study from an existing record; `activateSubProject` then parses an existing record. It does not append the GUI DOM node, create a GUI/shared-model SUB, select it as current, or request `SfOprDoeOutput`.

The apparent narrow implementation choices are not safe substitutes:

- Mutating `activeSub`, `subs`, and the project DOM to represent the new study would encode a GUI Add Study action into conversion without a public action/selection input. It could also alter the subsequent active-project save selection.
- Reusing `makeSubProject` would not match the original GUI constructor `0x1400B5680` (which receives the manager's first element), and the current source does not model that constructor's shared-main-model ownership.
- Calling the existing `createOperation` for `SfOprDoeOutput` would assume it is equivalent to original `0x1400A63C0`; CP240 expressly leaves that function's downstream effects/return outside the retained body. The existing DoeOutput class/model path does not establish the GUI insertion and active-operation behavior.
- Calling or approximating `0x1400A7B90` is likewise not licensed by the creator's call site alone. Its activation can enter Type2 parsing and the separate Report/QObject prerequisite recorded in `wave3-doe-next-active-study-dependency.json`.

CP215's reader-I/O identity guard is already implemented and reviewed; it is not an Add Study creation seam. The direct sample helpers and bounded DOE writers are also detached from this GUI transition and cannot be wired here to claim B120 output behavior.

## Evidence boundary and smallest next step

The retained creator decompile (`evidence/gui_mo/s1/s1-05/sub0_s101.c`, hash qualified in CP240) supports a static possible-path order: set the current index to the pre-append count, construct and append a Type2 GUI SUB, call activation, then request a DOE Output operation. The eight DUMPBIN excerpts qualify the upstream success branch. Together they establish neither an executed B120 branch nor the branch's compared-value meaning, constructor/operation return state, actual activation result, sample-row producer, or Save write order. Retained B120 before/after project hashes and the L6 case note identify an Add Study case and resulting state, but do not join those events to this native call chain.

Therefore the smallest implementation-enabling evidence is one fresh-process, disposable B120 staged witness joining the actual successful `0x14002AB20` predicate to the `0x1400A6A00` resulting SUB/index/DOM state, the `0x1400A7B90` activation result, the `0x1400A63C0` operation result, any actual sample-generation call/result, and explicit Save/file effects. If this witness cannot be acquired, retain the current guards. Once it exists, implement only the first fully witnessed prefix, with a focused Testkit case on that exact source seam before wider integration.

This is a bounded analysis of CP240's static audit/receipt, the cited active-study dependency record, and current source. No source, tests, build, shared queue/state, GUI, original process, or mutable REA state was changed. It makes no DOE-closure claim.
