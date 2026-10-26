# CP77: Candidate Open path increment (read-only preparation)

## Proposed public lifecycle binding

The public lifecycle entry is `m2m_convert` in `source/apps/moproj2mst/src/core/convert.cpp`. For a full Save it:

1. Treats the supplied ABI path as the already-selected dialog result (`:178-189`); it cleans relative paths against the starting current directory, while absolute paths are passed through unchanged at this point.
2. Computes `db = app::changeSuffix(file, ".DB")`, prepares the native session model if applicable, and initializes the project from `absolutePath(db)` / `completeBaseName(db)` (`:196-200`).
3. Registers classes and calls `openProject(p, file, ctx)` (`:226-227`), then keeps the accepted project and activates subproject zero (`:228-235`).

`moproj/read.cpp::openProject` (`:29-57`) is the actual selected-path decision seam. It resets `openedOk`; recognizes `.moproj`, `.DB`, or `.KEY` case-insensitively; sets `p.path` using `cleanPath` and, for DB/KEY, `changeSuffix(..., "moproj")`; and otherwise returns false. For `.moproj`, it reads the project and sets `openedOk`. For `.DB`/`.KEY`, the current candidate calls `engine::openProjectDb` and then throws `M2M_E_UNSUPPORTED` because the native helper bodies `0x14008b2d0/0x14008b380` are not recovered. This gap should remain a refusal; normalization coverage must not imply full DB/KEY Open support.

## Narrow proposed write set after CP76 freeze

If the parent confirms the CP76 artifact is frozen, the smallest useful change is limited to:

- `source/apps/moproj2mst/tests/p01/test_walk.cpp`: add a selected-path normalization case adjacent to `not_a_project_suffix`. Use a disposable temporary path and assert case-insensitive suffix acceptance, `p.path` normalization, `primaryDb` identity from the constructor, and the distinct current status for unsupported DB/KEY Open. Ensure no real corpus DB is opened; use a nonexistent temp sibling and verify test cleanup is scoped to the temp fixture.
- If a test cannot safely observe `p.path` before the intentional DB/KEY refusal without inducing database work, add a pure internal selected-path classifier/normalizer under `source/apps/moproj2mst/include/m2m/engine/` and `src/engine/`, with a unique test source under `tests/p01/`. The helper would return accepted kind plus normalized project path and DB identity, but `openProject` must remain the owner of parse and DB effects. Avoid changing public ABI or extending accepted lifecycle support.

Do **not** add a dialog implementation, infer initial filter selection, or alter the public `m2m_convert` path policy. The current full lifecycle assumes an already-selected path, and the ordinary File > Open dialog's caller/filter default remains unproved. Existing `tests/p01/test_walk.cpp` currently verifies corpus `.moproj` Open and driver DB identity construction; it does not establish selected `.DB`/`.KEY` support.

## Tests and boundaries

Focused cases should cover:

- `C:/work/./job/part.MOPROJ` → accepted MOPROJ kind; normalized project path `C:/work/job/part.MOPROJ`; DB sibling `C:/work/job/part.DB`.
- `.DB` and `.KEY` case variants → accepted suffix classification and normalized sibling `.moproj` path, while the current public Open remains explicitly unsupported for those selected formats.
- unsupported suffix → rejected before changing the project path.
- Path normalization tests use lexical Qt `cleanPath` expectations only; they do not claim filesystem canonicalization, symlink resolution, default dialog directory, or selected filter.

There is no Save-dialog branch to bind. Native File > Save uses the current `p.path`; output `.MST` is derived by the qualified path helper. No code, test, build file, or runtime behavior was changed in this preparation pass.

## Evidence

- `source/apps/moproj2mst/src/core/convert.cpp:178-200,226-235`: public already-selected-path and lifecycle order.
- `source/apps/moproj2mst/src/moproj/read.cpp:29-57`: current suffix/path/parse decision and deliberate DB/KEY refusal.
- `source/apps/moproj2mst/src/engine/db_load.cpp:206-216`: DB open side effect boundary before the current DB/KEY refusal.
- `source/apps/moproj2mst/tests/p01/test_walk.cpp`: existing corpus MOPROJ coverage and constructor/DB identity helper.
- `analysis/resume76-open-save-dialog-filter-path.md`: dialog-default qualification and native path order.

CP76 freeze was not represented in the available task context; source/test files were left untouched as instructed.
