# CP201 passive original Save-route probe plan (prepared; never executed)

## Purpose and present decision

Determine whether the retained B98 CLI Open produces the conditional dock signal auto-save route, using six passive observation hooks in one owned original process. This plan does not establish that Open, Save, or accepted Close occurred. No launch, attach, GUI acquisition, fixture copy, build, installed-file change, source edit, or probe execution occurred while preparing it. The only deliverable is this report. No reconstructed DEF_PRE instance is involved.

The instrumentation is implementation-ready. Execution has two unresolved preflight gates: the current Frida spawn schema cannot select cwd, and Frida session close never terminates a target. Resolve cwd and demonstrate a same-host exact-process termination/absence lane before launching. Prior receipts used SetCurrentDirectoryW inside the target and a separate termination script; those precedents do not qualify as an entirely passive observation script. Do not silently reuse them.

Read `resume173-headless-oracle-next.md`, `resume201-passive-oracle-feasibility.md`, `resume172-headless-oracle-next.md`, and the qualifications in `wave3-original-editor-probe-004-receipt.json` before interpreting observations. CP173 establishes static capability, not startup occurrence. GUI remains blocked by `unsupported_release_api`; independent `python tools/gui_validation_status.py` still reports no available GUI cases. This probe acquires no GUI control and cannot claim GUI control was released.

## Target and disposable project

- Original candidate executable: `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe`, historical version 11.0.2; reviewed retained bytes SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- Expected QtGui4_64.dll: `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
- Expected QtCore4_64.dll: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
- Source fixture: `evidence/gui_mo/oracle_b/98_B_DS3F_before`; project `Gear_blank.moproj`, SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd` verified during this preparation. Full retained tree contains 27 files.
- Reserved future disposable root: `C:\PROBLEM\p201a`; project `C:\PROBLEM\p201a\Gear_blank.moproj`. Fail if root already exists; choose a new short suffix and record it instead of overwriting. No directory was created here. For these 27 retained relative names the longest resulting path would be 60 characters. Recheck every copied/generated path is under the selected root and strictly shorter than 100 characters. This does not promise unknown outputs will respect that bound.
- Copy the entire source tree without changing its bytes, preserving relative names and case in the manifest. Hash before/after source tree independently. Do not rewrite embedded absolute references to make the run work; flag any reference outside the disposable root for review before execution.
- Do not invoke `oracle_b/run.py`, which performs GUI input. Existing `source/docs/re/moproj2mst/runner/oracle_b/launch.py` selects cwd but neither suspends before startup nor provides sufficiently strong cleanup identity; it is not a substitute for paused launch.

These are reviewed retained-file identities. Rehash current executable and all selected loaded-module paths on the execution host before making current installed-image claims. Reject mismatch, missing file, cross-host path ambiguity, or unqualified module version. Retain module paths, actual bases, image sizes, file digests and prehook memory prefixes. A file digest is not a complete loaded-memory digest.

## Current exact REA tool inputs and lifecycle limits

Tool schemas were read from the currently exposed metadata; no REA runtime tool was invoked. JSON below uses replacement tokens, never historical PIDs/session IDs.

```text
start_frida_session({mode:"spawn", program:EXE, argv:[EXE,"-m","open","-f",PROJECT], device_id:"local"})
  optional: argv:string[], device_id:string, pid:number, program:string,
            remote:{address:string,certificate?:string,keepalive_interval?:number,origin?:string,token?:string}
  required: mode:"attach"|"spawn"
  returns: device_id, mode, pid, session_id, state:"paused"|"running"|"detached", target
  NO cwd/working_directory/environment field. Spawn remains paused until resume.
load_frida_script({session_id:S,source_kind:"inline",source:SCRIPT})
  alternatively source_kind:"file", path:ABSOLUTE_SCRIPT_PATH
  returns normalized_result {script_id,source_kind,source_path,source_sha256,messages,messages_truncated}
resume_frida_session({session_id:S}) -> {state:"running"}
frida_session_status({session_id:S})
  returns normalized_result {device_id,messages,messages_truncated,mode,pid,scripts,session_id,state,target}
unload_frida_script({session_id:S,script_id:J}) -> {unloaded:true}
close_frida_session({session_id:S}) -> {target_state:"unknown"}
  Unloads/detaches; NEVER kills target. A still-paused spawn is resumed before detach.
list_frida_processes({device_id:"local"}) -> {cleanup_error,device_id,processes:[{identifier,name,pid}]}
```

`instrument_with_frida` exact input is `{mode:"attach"|"spawn",source_kind:"inline"|"file",argv?:string[],device_id?:string,duration_ms?:number,path?:string,pid?:number,program?:string,source?:string,remote?:<same remote object>}`. It detaches when its bounded observation ends; no cwd and no guaranteed target termination. Do not use it to solve ownership/cleanup by assumption.

`capture_process_scenario` supports `executable`, optional `arguments`, `working_directory`, `environment`, `timeout_ms`, `idle_timeout_ms`, `finalization_ms`, `filesystem_observation_paths`, `events`, `limits`, `normalization`, `settle_ms`, `terminal`. Its root finalization signals concern the captured command's root identity. Launching Python that spawns the original does not make the original that captured root; do not infer child termination from launcher completion.

Use EXE as argv[0] followed by the four original CLI tokens as shown, matching the prior retained startup preflight. Save exact arguments in the new receipt. Do not pass unsupported cwd fields or assume the MCP service inherits this shell's working directory.

## Cwd and ownership preflight gates

Preferred persistent lane is REA paused spawn, only if an observed execution-host cwd is already the disposable root. A read-only `GetCurrentDirectoryW` observation may verify it while paused, but must not call SetCurrentDirectoryW. If incorrect or unavailable, terminate the owned paused target through the prequalified termination lane; do not resume startup just to find out. Better establish inherited cwd before spawning. Absolute `-f` alone does not establish where relative application side effects go.

An alternative requires a separately prepared owned launcher on the same host: CreateProcessW with CREATE_SUSPENDED and explicit cwd, obtain PID + creation FILETIME + canonical image path + held process handle; attach Frida to that exact owned PID, load/verify hooks, then ResumeThread on the retained primary-thread handle once. That uses `start_frida_session({mode:"attach",pid:P,device_id:"local"})`; resume_frida_session cannot resume an attached process. This is a concrete implementation alternative, not an existing helper or an action performed here. Prove host visibility and cleanup first. Do not execute an existing original before installing hooks or attach to an enumerated user process.

The old grid receipt explicitly says shell enumeration could not see its owned PID. Therefore a local shell Stop-Process is not an established cleanup lane for a target visible only on REA's host. This is a real execution blocker until reconciled. Do not use process-name termination, broad child killing, window close, or GUI input.

## ASLR-safe addresses and six observations

Retained PE header reads verified preferred bases: EXE `0x140000000`, QtGui `0x65000000`. Resolve each actual VA as `Process.getModuleByName(name).base.add(RVA)`. Never reuse the historical `0x7ff7c94b0000` or `0x53e20000` bases. Check architecture windows/x64, expected canonical path, size, and bytes before installing any hook. Use `Process.attachModuleObserver({onAdded(module){...}})` for late QtGui loading; its callback must install the relevant hooks before that module's application use. If this API is unavailable or required readiness cannot be established, stop; polling after resume cannot certify no missed startup.

| Observation | Module RVA | Passive callback fields and meaning |
|---|---|---|
| requested dock visibility, original 0x1400154a4 | EXE +0x154a4 | Before virtual call: RCX dock, RDX low byte requested bool, TID. This is a requested state, not a Show event. |
| dock event entry, retained 0x6539ffa0 | QtGui +0x39ffa0 | RCX dock, RDX event pointer; safely read event+0x10 as uint16. Record type17 Show, type18 Hide and other types distinctly. |
| Show-branch signal call, retained 0x653a0084 | QtGui +0x3a0084 | RCX sender dock, RDX low byte bool before call. Reaching it proves the emission call-site was reached, not successful slot delivery. |
| connected slot entry, original 0x14003a990 | EXE +0x3a990 | RCX main, RDX low byte bool; read pointer at main+0x420 for dock correlation. Preserve pointer/read errors. |
| busy return from call at 0x14003aa81 | EXE +0x3aa86 | Immediately after original call returns, RAX low byte busy result. Link to active slot stack/TID; no onLeave on a call instruction. |
| auto-save call, original 0x14003aaa2 | EXE +0x3aaa2 | RCX main before call; link active slot/TID. It establishes path entry only; do not claim writer success. |

The fifth observation deliberately hooks the first instruction after the five-byte call, rather than `onLeave` at the call-site. Static bytes at EXE +0x3aa81 are `e89afe2800`, targeting EXE +0x2ca920, and the next instruction at +0x3aa86 stores AL. A six-hook implementation can add onLeave to the *function-entry* slot hook to maintain per-thread nested invocation IDs; no extra address hook is needed. Mid-function hooks use onEnter only and `this.context` register values, not Frida args interpreted as entry arguments.

Correlation is by actual pointer identity, TID, sequence and nested slot ID. Retain early dock events even when main is not yet known, then correlate retrospectively. Never discard earlier events because the first requested visibility has not yet occurred. Cross-thread asynchronous delivery remains a temporal association unless stronger evidence supports causality. A slot might be invoked by another producer; do not equate a nearby slot with this Show route merely by timing.

The event offset is independently qualified: current static DUMPBIN export output maps retained QtCore `?type@QEvent@@QEBA?AW4Type@1@XZ` to RVA0x11730; bytes at 0x67011730 are `0fb74110c3`, i.e. movzx eax,word ptr [rcx+0x10]; ret. The passive hook reads the field; it calls no Qt API. Do not substitute an assumed +8 event-type offset.

Prehook 24-byte identity prefixes (captured directly from the retained PE sections during preparation):

```json
{
  "DEF_GUI_MO_64.exe:154a4":"ff5058488d0d0a12d603e80d632b000fb6c085c0745d4533",
  "DEF_GUI_MO_64.exe:3a990":"8854241048894c240856574881ecd8010000c64424600048",
  "DEF_GUI_MO_64.exe:3aa86":"884424620fb644246285c07407c644246301eb3e488b8c24",
  "DEF_GUI_MO_64.exe:3aaa2":"e8d960fdff884424640fb644246485c0750dc78424c00100",
  "QtGui4_64.dll:39ffa0":"405355565741544881eca000000048c7442440feffffff4c",
  "QtGui4_64.dll:3a0084":"e877d8ffffe946020000488bcee88ae2ffffe93902000032"
}
```

Compare ALL six prefixes before attaching any hook in that module. Interceptor patching changes code bytes, so do not compare a previously hooked prefix against its baseline afterwards. No NativeFunction application calls, event pumping, input synthesis, data/register assignment, replacement, return-value rewriting, target cwd setting, target pause spin loops, or file-writing code belongs in the observation script. Interceptor itself perturbs timing and process code; qualify the resulting observations accordingly.

## Bounded window and stop behavior

Set the maximum startup observation window to 15,000 ms after the single resume, with an independent host cleanup deadline of 20,000 ms. Poll bounded session messages at 1-second intervals; do not wait for more data after the endpoint. Retain at most 512 observation records plus one terminal summary, monotonic sequence, script-relative milliseconds, TID, pointer strings and nested-slot ID. Limit each record to 1 KiB, with no arbitrary memory dumps. Script and provider truncation each invalidate completeness.

The script uses a 15-second timer to mark the observation endpoint and detach its own listener handles; the host deadline still applies if script timers stall. Endpoint is `startup_window_elapsed`, `target_exited`, `hook_error`, `budget_exhausted`, or `external_stop`. Do not force a Qt call or suspend the target to obtain an endpoint. Host must terminate promptly after endpoint so the application cannot keep writing unobserved. Unload/detach failures remain failures, not implied cleanup.

Stop and retain partial results on first hook/module identity mismatch, read error, callback exception, provider truncation, missing required readiness, timeout, user stop, or observed startup modal/dialog indication. With no GUI observation, absence of a recorded dialog is unknown; do not claim dialog-free startup. Never acquire GUI control to resolve one. Missing observations are inconclusive unless readiness, complete delivery, budgets, endpoint, and error-free coverage all support the bounded negative statement.

## Capture receipts and file hashes

Keep future reports and raw receipts under converter-owned `analysis/resume201-passive-oracle-run-<id>/`, not in shared parent build/source directories. Record UTC, tool metadata/schema snapshot, executable version resource, exact executable path/digest, host identity, launch arguments/cwd receipt, PID and creation time/session identity, module map/path/file hashes, all prehook prefixes, hook-readiness records, script bytes/SHA-256, each unmodified tool Evidence response, provider evidence_id and limitations. Do not print full captures to inspect a single field.

Before copying and before launch, create separate deterministic manifests for source tree and disposable tree. Each record contains relative path, size, mtime_ns and SHA-256; include empty directories separately and reject symlinks/reparse escapes. Sort paths ordinally and hash the UTF-8 canonical manifest. Preserve content-only and metadata-inclusive manifest digests separately so timestamp changes are not mistaken for content changes. Do not call either digest a native filesystem snapshot hash.

After observation endpoint, take a best-effort live snapshot marked unstable if files change during hashing; after verified target absence, take the authoritative quiescent after snapshot. Compare added/deleted/content-changed/metadata-only paths. Rehash the original source fixture to demonstrate it stayed unchanged. Store SHA-256 of every raw evidence/report/script/manifest in a final capture manifest, excluding that manifest's own hash from itself; store its digest in the summary receipt.

No changed file does not prove no Save occurred: a writer may serialize identical bytes. Changed files alone do not attribute a successful Save. Save-path entry plus stable expected outputs supports only the specific observed writing behavior; the wrapper ignores the top writer's return. This six-point trace has no writer-return hook and cannot assert successful writer return. Writes outside the disposable root, user settings/registry effects and effects after the endpoint remain outside directory-manifest coverage; record this limitation.

## Exact-process termination and session verification

Before launch, demonstrate a termination lane for the SAME host as the selected Frida device. Preferred: retain a Win32 process handle opened at creation with PROCESS_TERMINATE, SYNCHRONIZE and PROCESS_QUERY_LIMITED_INFORMATION. Record PID, creation FILETIME and QueryFullProcessImageNameW, compare canonical expected image, and retain the handle. TerminateProcess through that handle at the bounded endpoint; wait on that same handle and retrieve exit code. This prevents a later reused numeric PID from being killed. It is lifecycle cleanup, separate from the passive trace, and never accepted application Close.

If only REA-host execution is available, the existing session can load a separately hashed cleanup-only script that checks `Process.id === ownedPID` and actual module path/base against that session's ownership receipt, emits `owned_termination_scheduled`, then calls the kernel32 ExitProcess export after a short 250 ms timer. The prior editor termination receipt shows this general scheduled-termination approach, not a built-in REA kill API. This intentionally changes target lifecycle and must be named as cleanup, never included in the passive observation claims. A scheduled message alone proves no exit. Do not arm it until the execution lane accepts that exact cleanup method. Prefer a held process handle if available; ExitProcess can stall in DLL detach and therefore is not a guaranteed fallback for the deadline.

On every success/error/user-stop path: capture endpoint and errors; stop/unload observation script when possible; terminate only the owned process through the prequalified lane; obtain exit/wait result; independently call list_frida_processes on the SAME recorded device and verify owned PID absent; close the exact Frida session and persist its response. A new session-status query, if provider returns not-found/detached, may corroborate closure; it cannot prove process exit. Retain tool errors instead of claiming closure from an attempted call. Absence enumeration failure or cleanup_error is unresolved cleanup. If PID exists again, compare creation identity; do not kill the new process by reused PID.

Do not close a still-paused session as cleanup: close explicitly resumes a paused spawn before detaching. Terminate it first and verify absence. On normal cleanup record GUI acquired=false, owned_process_termination, process_absence_verified, session_close_observed separately. No app-control release claim follows from any of these fields.

## Interpretation and remaining blockers

A complete positive chain is requested dock state, actual type17 dock event, true Show-branch call, matching main/dock true slot entry, false busy return, auto-save call, and independently examined directory effects. Report every link separately. Even this chain is a Save discriminator with instrumentation/timing limitations. No accepted FileCloseAll/Close producer is supplied. Process termination, destructor execution, QApplication exit, a detached session and empty process enumeration are never accepted Close evidence. The full Open -> Save -> Close oracle remains blocked.

Current decision: prepare-only complete; runtime feasibility conditional. Resolve cwd without a target cwd mutation, same-host exact-owned-process cleanup with verifiable absence, and module/hook readiness before a separately authorized run. Do not broaden to GUI or user-process attachment if any gate fails.

## Bounded follow-up: current local-device host alignment

The parent coordinator supplied a new read-only comparison: shell `Get-Process -Name Codex` returned PIDs 22324 and 31472; a current `list_frida_processes({device_id:"local"})` result contained both exact PIDs with the Codex name, among 263 processes. This supports current host alignment between that shell and the REA local device. It narrows the historical shell-visibility concern; it does not demonstrate target launch, cwd inheritance, process-handle acquisition, termination rights, or successful exact-process cleanup. This report records the coordinator-provided comparison, not an independently repeated check by this worker.

The cwd-selection gate and held-handle/creation-identity cleanup gates remain. Prepare the explicit-cwd suspended launcher and validate the handle-based termination/absence procedure before an original run. Do not infer an original PID or use either Codex PID for cleanup. No runtime launch or attach was performed for this follow-up.
