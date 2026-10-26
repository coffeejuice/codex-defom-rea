# MOPROJ-to-MST converter takeover

Purpose: finish the headless converter begun in Claude, with its original behavior established from DEFORM v11.0.2 and validated on copied projects. The full GUI reconstruction is outside this takeover.

The import is explicitly authorized. The user selected full headless Open-to-Save scope and then clarified that this is scientific research into REA/Ghidra plus Codex recovery into modern C++. Internal architecture, functions, decisions, behavior, interfaces and external results must follow the original within that scope. See RESEARCH_CHARTER.md. The interview settled one project per fresh reference session and preservation of the m2m compatibility ABI; baseline build and relocation audit have started.

## Baseline and retained work

The starting source commit is `bb869387a85a89193f3fdd78b69442a6ea567866` (`claude/m2m-l2-folder`), found in the `jolly-pare-170b29` worktree. Git ancestry confirms it includes both `9f1283895be488a1e31ebd47637c06b06ad80f6d` (recorded remote MDT integration) and `996e98d8992103b72966531b26a9e4ac67f8283e` (older local MDT integration).

This corrects the earlier audit's baseline choice. Its last two commits attribute the 175-pixel diagram view to project database opening, not the Setup Type answer. The first local build and bounded checks are recorded in analysis/baseline-build.json, analysis/baseline-checks.json and analysis/baseline-audit.md.

- `source/`: source extracted from the selected Git commit, retaining the converter and shared library layout.
- `working-snapshots/`: filesystem contents of converter worktrees, including uncommitted alternatives; generated build directories are excluded.
- `history/`: locally created converter Git bundle and exact baseline archive. No fetch or push occurred.
- `historical-builds/`: saved test/build logs and existing converter binaries; these are not the new baseline's build.
- `evidence/`: Ghidra projects, REA analysis projects and target copies, decompiles, specifications, scripts, original binaries, corpus and oracle recordings.
- `context/`: converter-related Claude memory and original guidance, retained as historical context.
- `admin/`: acquisition script, checksums, original paths, reference identities, exclusions and completion receipt.

The final import receipt and manifests are authoritative for what was successfully acquired. Database files are copied without opening their analysis engines. A stable, hash-verified filesystem copy does not establish transaction-level database consistency or successful reopen.

## What is already implemented

The C++20 converter exposes a Windows x64 DLL with a C ABI and a CLI. It reads `.moproj`, reconstructs project/operation state and writes the MST text using recovered Qt 4 formatting and path behavior. Its dependencies include the recovered DEF_PRE runtime, model, KEY reader/writer, database and command layers; copying only the converter folder would be insufficient.

The selected source's converter ledger has 1,765 entries: 1,566 marked ported, 116 library, 54 out-of-scope, and 29 spec. The 88.7% raw ported fraction describes this inventoried closure, not end-to-end completion. Source recovery has continued beyond the older 1,762-entry audit.

The public ABI is still version 1. It has UTF-16 and UTF-8 path entry points; code page and startup units are configurable. Conversions serialize around the embedded engine's process-wide state. The older plan's promise of unrestricted concurrent conversions is superseded by the current header and implementation.

Default conversion writes only MST. Optional flags add Cogging.MDT rewriting and full Save effects, including `.moproj` rewriting and referenced file creation/deletion. Even default conversion executes active-operation load/save logic in memory. Its geometry and engine dependencies cannot be removed merely by disabling disk writes.

## Established validation and limits

The older narrow MST gate records 63/63 tier-1 tests, 11/11 tier-2 cases and 45 passing tier-3 cases; two size-skipped projects were tested separately. These historical results do not validate the latest expanded source.

The older MDT integration's latest saved compiled test run passed 74/92 tests. Its expanded Save sweep reports 16/49 Oracle-B cases identical and 33 refused. A separate fresh-corpus resave sweep reports 0/49 identical, 47 refused and 2 differences. These records belong to their historical binaries, not automatically to the imported commit.

The release smoke examples in C++, C# and Python check the missing-input error path. They do not prove successful real-project conversion through every binding.

Some reference files reflect state accumulated across multiple projects in one original GUI process, while the intended converter semantics in the notes are one fresh GUI process per conversion. MultiBlow template behavior exposes this difference. References must be qualified before treating a mismatch as a port defect.

## Remaining work and unresolved contract

- Recover the remaining 3D geometry-list bounds/scene-box paths used by active-operation loading and saving. Current source explicitly returns unsupported for some of these paths.
- Finish DOE `.MST.TEMP` and remaining DOE variable/constraint behavior if included in the agreed release scope.
- Reconcile unmerged connector validation from `claude/m2m-mdt-L5S`; do not overlay its older full file because that would lose newer diagram-placement code.
- Compare preserved working copies even when HEADs match: two `9f128389` worktrees differ in converter and shared-library files.
- Complete remaining operation-specific Save behavior, including cogging cases, multiple reheats, property files and view-dependent state, according to the agreed scope.
- Relocate absolute test/oracle paths to imported evidence and create an isolated build. Do not reuse upstream CMake caches.
- Run fresh successful-path ABI tests and original-vs-converter comparisons, including input immutability for MST-only mode.
- Restore original failure semantics where the port differs. The user explicitly revoked the proposed all-or-nothing Save requirement. Static analysis confirms that the original calls subsequent Save stages after .moproj failure, uses direct file writes and has a Cogging writer that returns success after a failed open. Claude's atomic-replacement helper is therefore a fidelity issue requiring review, not an accepted modernization. See analysis/save-semantics.md.

The deliverable boundary is settled: full headless Open-to-Save behavior, not the complete GUI. Necessary engine, geometry, state and file-processing dependencies remain in scope. The original determines behavior; remaining interview questions should concern genuinely undecided scope or interfaces, one at a time. Ledger percentages alone do not demonstrate completion of the research objective.

## Key source references

- `source/apps/moproj2mst/include/moproj2mst.h`: current public contract and output flags.
- `source/apps/moproj2mst/src/core/convert.cpp`: serialization, active-operation processing and write order.
- `source/apps/moproj2mst/src/engine/viewer.cpp`: explicit remaining geometry refusals.
- `source/docs/re/moproj2mst/ledger.tsv`: inventory status.
- `source/docs/plan-gui-mo-stage1-moproj2mst.md`: original plan and appended decisions; early paragraphs can be stale.
- `source/docs/re/moproj2mst/runner/cases.tsv`: corpus provenance.

No original executable, build, oracle or test was run during acquisition. Claude's files were read only. After the scope discussion, the imported converter was built under build/baseline/bin and bounded baseline checks were run on copied projects. This is a research baseline, not a completed release. No DMO binary has been built or deployed by this takeover.
