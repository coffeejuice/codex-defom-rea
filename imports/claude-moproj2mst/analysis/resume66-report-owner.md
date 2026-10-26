# CP66 — Report owner adoption and Open-to-Save reachability

## Finding

The CP65 `.DS` path helper is still an isolated pure function. No production call site adopts it, no current model field owns the native Report document at `SubProject+0x30`, and no Report Open/Save branch currently reaches a successful public Save. For an active `SfOprReport`, Open reaches the existing unsupported boundary in `ReportClass::loadClassData`; the later Save code is not executed. Keep both Report lifecycle guards.

## Source-qualified path and owner evidence

- `src/ops/report_ds_path.cpp:7-13` defines `reportDsPath(projectPath)`. It derives an absolute sibling path from the project filename and appends `.DS`. A repository-wide search finds references only in its declaration, definition, and focused path test; no loader, model, or Save path invokes it.
- `src/ops/report.cpp:64-73` implements `ReportClass::loadOperation`. With an active subproject and project, it calls `loadClassData` and then calls `l7::refuse` because the remaining Report load is not reproduced.
- `src/ops/report.cpp:77-84` implements `loadClassData`. When project and active subproject exist, it refuses at the document load. Its source annotation identifies the missing native owner as the Report document at `SP+0x30`, accessed through `0x1400b5ad0` and lazily constructed by `0x140643100`.
- `include/m2m/model.hpp` has no Report-document member on `SubProject` or `Project`; the `SP+0x30` native slot is not represented by an adopted source owner. CP65 evidence also leaves native pointer ownership, constructor connection lifecycle, and normal Close/destructor order unresolved (`analysis/resume65-report-ownership.md`).

Thus `Project.path` is sufficient input for the already implemented path derivation, but that does not establish or implement the separate document owner, read/reset/open sequence, or Close cleanup. The helper's correctness does not make the Report document lifecycle reachable.

## First reachable public Open → Save branch

`src/engine/loader.cpp:1058-1083` selects the active flow item, resolves its operation and class, resets the operation model, then invokes `classOf(*op)->loadOperation(...)`. For Report, this dispatch enters the two refusals above before `openActiveOperation` returns. Therefore `src/core/convert.cpp:255` cannot proceed beyond `openActiveOperation` for that active Report operation to reach its subsequent Save logic at lines 318-333. The first reachable branch terminates in Open; there is no Report public Save branch after it.

If a caller reaches Save with some other operation active, `saveCurrent` uses the current active operation (see `src/engine/save_project.cpp:43-67`). It does not dispatch Report Save merely because a Report exists elsewhere in the project. If Report is the active operation but Open were bypassed, `src/engine/save_current.cpp:761-764` still explicitly refuses Report vt+0x68 because the native document and viewer model path are not reproduced. The generic Report property mapping is not a substitute for that owner and callback sequence.

## Smallest next increment

There is no evidence-backed lifecycle increment that safely enables Report Open or Save. The smallest bounded next source-evidence task is to establish the native `SubProject+0x30` slot's pointer/ownership semantics and normal Project Close release order, then bind the Report document to a model owner. Separately, CP65 identifies unresolved viewer selector `0x140d0d6f0` in Report Save; it must be distinguished from the existing ROI selector `0x140d0d670`, and its returned object and first mutation must be qualified before Save can be integrated. Until those owner and callback edges are established, retain the Open and Save guards; the path helper remains independently testable only.

## Scope

Read-only source audit. No source edits, build/test execution, GUI control, or mutable REA interaction.
