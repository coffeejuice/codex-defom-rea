# CP103: remaining public remesh failure boundary

The bounded nonnumeric search found no new source-qualified ordinary-return behavior to implement. Current remesh reader/writer ordering already represents the retained native contract. The largest remaining public failure is enabled Save with failed `fopen("wt")`; its native default path is process-terminal rather than another returned writer failure. No source/test change is proposed for this checkpoint.

## Reconciliation with the current Session route

Project Open reaches the actual `ConversionSession::remesh_` through `moproj/read.cpp:59` and `ConversionSession::loadProjectRemesh()`; successful reader completion alone assigns `enabled`. CP98–102 numeric work therefore advances public Open before editor construction, but does not prove any later selected-editor palette/setup/Fill state.

Public Save reaches `saveRemeshAtProjectSave(session.remeshState(), qt::currentPath(), KnownC)` in `core/convert.cpp:428`, after current operation, project/property writes and MST. Earlier returned writer false values do not bypass this remesh call. Existing production already implements:

- disabled remesh: unconditional remove, ignored Boolean return;
- enabled successful open: 31 ordered formatted writes followed by Close, ignoring all write/Close results and reading values at their original call sites;
- reader duplicate/field ordering, shared last-integer reuse after an assigned value, real EOF destination preservation, raw uniform-field byte copy through the qualified span, and subsequent field continuation.

`test_simcontrol300_remesh.cpp::remesh_save_ignores_post_open_write_and_close_errors_and_continues` already verifies all 31 attempts and Close despite a failed write. CP74 covers enabled open denial after an earlier project post-close failure. CP99 `test_save_remesh_failure_matrix.cpp` closes the proposed CP98 matrix cell: prior project/MST failures combined with denied disabled-remesh removal. Repeating these would add coverage without recovering another production decision. CP102 heat-route regression likewise concerns an existing isolated setup projection, not public Session Fill.

## Exact first failure still refused

The retained `wave3-remesh-save-lifecycle-contract.json` binds enabled Save to `0x1400abcc0`. Its first `fprintf` at `0x1400abcf0` uses the unchecked `fopen` result. On null FILE, CRT entry `0x14146da0c` sets errno 22 and calls `0x141470568`. The dispatcher reads/decodes the actual native module global `0x1446ffb40`. A nonnull handler is called; if it returns, further print calls remain reachable. For the fresh initialized null-handler profile, startup explicitly installs encoded null before main, the debug leaf only clears its recorded flag, and `0x141470440` reaches `GetCurrentProcess`/`TerminateProcess(0xC0000417)`. Under ordinary Win32 behavior this does not reach later prints, fclose, Save status reporting or normal Close. Prior output effects survive.

This terminal edge is established statically under the fresh-default/no-subsequent-handler profile; it is not missing arithmetic or a fresh observed fault. The remaining dependency is the converter's outward contract: `RemeshSaveReport`/`ApplyResult` and the C ABI/CLI currently describe returned statuses; `convert.cpp:430` deliberately maps the refusal to `M2M_E_UNSUPPORTED`. Reporting ordinary completion or `M2M_E_WRITE`, continuing remaining writes, or invoking host null-FILE behavior would contradict the native edge. Killing an embedding caller is not authorized by this evidence task. Merely adding a default-handler symbol/profile with the same refusal would add no public progress.

The next executable design action is therefore root-owned: specify an explicit process-terminal conversion outcome, its effect on subsequent Save/Close and cleanup, its C ABI versus isolated CLI representation, and its admission of the fresh-default module handler profile. Only then reserve the report/result, conversion dispatcher and child-process test files together. Tests must retain earlier project/MST effects and sentinel bytes, prove no later remesh writes or normal Close, distinguish a custom-returning handler, and run any terminal executable case in an isolated child. This report does not propose or perform an API change.

## Other guarded candidates

A wider `UNIFORM_INTERNAL_EDGE_LENGTH` raw copy is not a safe substitute: retained `wave3-remesh-rawspill-layout.json` proves bytes through singleton offset +0x29 inclusive, not a sizeof(0x30) object or the next native owner. A >25-byte token plus terminator needs the adjacent actual module owner/lifetime and overlap effects before admission; host padding is not evidence.

First-use missing integer remains an unassigned native-local value, so it cannot be filled from a fabricated zero. Nonregular/encoding/native stream-allocation failure profiles require their specific handle/stream ownership before relaxing the current ordinary-file profile. No retained bounded evidence examined here closes those edges. The selected editor still requires the same post-Open palette getter/data/current-refcount witness; no startup count, module symbol alone, or setup projection substitutes for it.

## Evidence and scope

This report reused bounded retained static records, source and tests; no new native execution or mutable database was used. The native startup contract explicitly limits its default-handler inference and computed-alias scan. Relevant records are `wave3-remesh-invalid-parameter-startup-contract.json`, `wave3-remesh-save-lifecycle-contract.json`, `wave3-remesh-default-invalid-static.json`, `wave3-remesh-save-invalid-static.json`, `wave3-remesh-rawspill-layout.json`, `resume75-remesh-writer-failure.md`, `resume99-remesh-failure-matrix-test.md`, and `resume102-editor-settings-next.md`.

Only this analysis file was written. No source/test edits, builds, tests, original execution, GUI or REA mutation occurred. CP102 source remains frozen for root acceptance. This is a bounded no-increment decision, not completion of editor300 or the full recovery goal.
