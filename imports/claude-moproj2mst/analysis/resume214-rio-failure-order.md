# CP214 RIO failure ordering: static reconciliation and next witness

## Finding

No new candidate implementation or denial permutation is justified. The CP164 conjunction already has retained validation evidence, and CP171/CP208/CP203 correctly conclude that its candidate behavior does not resolve the original native failure. Keep the public enabled-remesh failed-open path guarded as `M2M_E_UNSUPPORTED` until a same-run original witness binds the terminal route.

## Evidence reconciliation

- `analysis/resume164-rio-next.md` proposed the candidate-only conjunction in `tests/s/test_save_remesh_failure_matrix.cpp`. `analysis/wave3-checkpoint164-validation.json` records the grouped run (361 registered; zero failed), and the focused receipt/log cited in CP208 reports both matrix cases passed. This is historical candidate validation, not a fresh run or original parity.
- The conjunction's project-output event vector is output-factory open → `aboutToClose` → close returned → post-close error query. It does not directly observe input Open or Save-current. Its locks and sentinels establish candidate-side write-denial and retained bytes; it asserts the adapter guard, not original `errno`, Save return, process status, or normal Close.
- `analysis/resume171-rio-failure-next.md`, `analysis/resume208-rio-next.md`, and `analysis/resume203-io-failure-static.md` agree: more candidate combinations or candidate worker endpoint runs only repeat candidate precedence. The RIO row in `analysis/takeover-state.json` still names CP164 as validated candidate coverage and identifies the same-run original handler witness as the outstanding item.

## Source contract (static candidate/source fact)

In `source/apps/moproj2mst/src/core/convert.cpp`, native Save records MST attempt/return, then calls `saveRemeshAtProjectSave` at lines 434–449. It maps any non-`Completed` remesh result to `M2M_E_UNSUPPORTED` before mapping the saved MST or project false result. Thus earlier false writer results do not preempt the remesh stage in this candidate path.

In `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp`, `saveRemeshAtProjectSaveWithTerminalService` at lines 340–365 returns after ignored `DeleteFileW` when disabled. When enabled it opens `LOCAL_REMESH.DAT`; on failed open it may dispatch the explicitly supplied terminal service for an admitted profile, then records `UnsupportedSnapshot` and `invalidParameterReached`. Without that private service, the public helper preserves the unsupported boundary and avoids executing host undefined behavior. Neither candidate behavior nor the isolated endpoint establishes what the original process does.

The retained original disassembly/decompile, summarized in `analysis/save-semantics.md` and CP203, supports this conditional order: active operation save; project write; operation property saves; MST and following study output; local remesh. Project/MST false results do not skip later stages. On enabled remesh failed `fopen`, the first `fprintf` receives null, the CRT records module `errno=22` and dispatches invalid-parameter handling. If the handler slot is still decoded-null at that occurrence and the default handler/filter take the ordinary returning path, static code requests process termination with `0xC0000417`. Those final premises and the external outcome are inference until dynamically bound; startup null initialization is not proof of the slot value at Save.

## Smallest next original witness

Once the independently controlled original-process lane is available, make one fresh owned-process attempt using one disposable short-path B133-derived project. This is an evidence task, not another candidate test. In the same attempt, bind:

1. Original image/module identity and hash; successful Open; remesh enabled; Save-time CWD and resolved `LOCAL_REMESH.DAT`.
2. A parent read-compatible/write-denying handle whose denial is verified before the native open and held through it; then the actual null `fopen`, first `fprintf`, and module `errno`.
3. Decoded invalid-parameter handler slot at dispatcher entry and selected branch. If default, capture the filter/debugger outcome, whether `TerminateProcess(0xC0000417)` is reached/returns, and the externally observed child status.
4. Same-run before/after raw output manifest, including earlier project/property/MST effects and remesh sentinel. Record instrumentation's possible effect on filter/debugger behavior.

Reject a status-only match as insufficient. If denial is not proven pre-truncation, or handler/filter/termination branch is not bound, retain the outcome as unresolved; do not promote a conditional static prediction. No original run was performed for this audit.

## Scope and write record

Static/source findings above are bounded to the cited retained evidence and current source lines; they do not claim a new original runtime trace. No source, test, shared plan/state, GUI, original process, build, or mutable REA session was touched. This new analysis note is the only write.
