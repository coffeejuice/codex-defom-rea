# CP202 IO static successor audit

## Finding

No new source-ready RIO failure-order, encoding, or path increment is supported by retained CP164/CP196 static evidence. The candidate's enabled-remesh failed-open result remains an explicit `UnsupportedSnapshot`/`M2M_E_UNSUPPORTED` boundary. The original's possible default-handler termination is conditionally established from startup and writer disassembly, but is not a candidate return code and has no same-run original Save witness.

## Reconciled Save order

For original `DEF_GUI_MO_64.exe` v11.0.2 (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), the bounded Save and remesh captures support this conditional order: current-operation Save; project writer; property-save loop (individual results ignored); MST/study writer; local remesh handling. Project/MST failures do not statically bypass the later stages. Enabled remesh's failed `fopen` is unchecked and the first `fprintf(NULL)` sets module errno 22 and enters the invalid-parameter dispatcher. Startup statically assigns encoded-null to the handler slot before main; if still null at Save and the default handler/filter take ordinary returning path, the handler requests `TerminateProcess(..., 0xC0000417)`. This does not establish that a particular Save reached those predicates, that the slot remained null, that the filter returned, or the actual child status. Under the conditional termination route, there is no ordinary Save result or subsequent Close.

Current candidate `convert.cpp` in native Save executes the remesh tail after project/property/MST writes, even when the project or MST writer reported false; it maps remesh refusal before MST failure, then project failure. Remesh code detects failed enabled open and returns the explicit unsupported boundary instead of invoking host undefined behavior. That ordering already reflects the retained static prefix. Do not change it to manufacture a GUI status or process exit.

## CP164/CP196 candidate coverage versus original facts

CP164's failure-matrix case is a candidate-only conjunction: one conversion call reaches a scripted project post-close false, physically denied MST open, and enabled-remesh denied open. It checks candidate call flags, earlier effects, retained sentinels, and the candidate remesh guard. Its event vector observes the *project-output writer* sequence `output open → aboutToClose → close returned → error query`; it does not observe input Open or Save-current. The test is not an original execution and does not run the native invalid-parameter handler. CP171 already concluded that further candidate denial combinations or fixed worker endpoints add no original RIO parity. The later native-save increment documents the remesh writer's source-derived bytes/lifecycle and guard, not a fresh original run.

CP196 materials are unrelated to RIO: `wave3-checkpoint196-validation.json` scopes that checkpoint to a Report null-parent/QObject import boundary and explicitly says static evidence only, no original executable launch. CP196 must not be treated as corroboration for the RIO handler state or a Save-time failure.

## Encoding/path audit

No new encoding/path mismatch is proved. Remesh's retained profile is ordinary ASCII drive-absolute/MAX_PATH paths, with the native narrow `fopen`/CRT writer path and known C numeric locale assumptions; non-ASCII and namespace paths remain qualified. The CP105 HT property-file audit separately identifies a real open equivalence question: original `QString::toLocal8Bit()` versus candidate `encodeLocale(target, ctx.codepage)`. Neither original process ACP nor same-path conversion bytes are retained, so no code change is justified. This HT fact is not evidence about QFile/remesh paths and does not resolve RIO failure order.

## Smallest remaining witness

The limiting evidence is one same-run original failure trace, not another candidate test: bind original image identity, successful Open and remesh-enabled state, Save-time CWD/resolved primary path, pre-attempt write-denial proof, actual null `fopen` and first `fprintf`/errno, decoded handler slot and selected branch, filter/debugger outcome, termination API reach/return, external child status, and before/after output manifest. Preserve instrumentation's possible effect on exception/filter behavior. Until this exists, retain the explicit candidate guard and label terminal behavior conditional. The repository's current GUI-control release restriction blocks this experiment; this report performs no app-control action.

## Scope and provenance

Read-only sources reconciled: `analysis/resume164-rio-next.md`, `analysis/resume164-rio-test-review.md`, `analysis/resume171-rio-failure-next.md`, `analysis/wave3-remesh-native-save-increment.json`, `analysis/wave3-remesh-invalid-parameter-startup-contract.json`, `analysis/wave3-earlier-save-static.json`, `analysis/wave3-earlier-save-mkdir-contract-correction.json`, `analysis/resume105-encoding-path-next.md`, `analysis/wave3-checkpoint196-validation.json`, and current `source/apps/moproj2mst/src/core/convert.cpp` / `src/engine/simcontrol300_remesh.cpp`. No test, build, GUI, original-process, REA mutation, or source edit was performed. Only this report was written.
