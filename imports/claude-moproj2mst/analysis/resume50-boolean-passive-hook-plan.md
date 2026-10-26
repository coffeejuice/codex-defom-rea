# Bounded passive original hook plan

Purpose: recover the state needed to finish constructor PostPending and separately observe actual Boolean editor creation/replacement timing, using one owned reference process at a time without GUI input. This plan authorizes no execution by the worker. The public constructor remains guarded.

## Control and ownership boundary

The controlling repository gate reports `unsupported_release_api`: no supported GUI-control release/status receipt has been established. Do not acquire app control, call computer-use entry points, activate a window, inspect through an input-control surface, click, type, send keys, inject window messages, or answer dialogs. A fresh original GUI executable may create its own windows while its native command-line path executes; that alone is not a GUI-input session. “No GUI input acquired” must be established from the actual selected tool/session receipt, never inferred from a script comment.

Root must choose a supported debugger/instrumentation route whose process-spawn, memory-read, hook, continue, stop/detach and exact-owned-process cleanup operations do not implicitly acquire app control. Check actual current capability documentation rather than assuming an old session API still exists. If the route acquires app control, needs foreground input, or cannot distinguish an owned PID from a user session, this plan cannot launch through it until the supported release/status prerequisite is met. Do not use another tool or a hidden UI automation path to evade that gate.

This does not establish that any current GUI overlay is off. Debugger detach, JS reset, process termination and application closure are not GUI-control release operations. If no GUI control was acquired, report that fact and the separate debugger/process cleanup facts; do not claim release of an unrelated pre-existing session.

Use spawn, never attach to an existing DEF_GUI_MO process. Enumerate names plus full executable path/PID/start identity with a read-only process API immediately before launch. If a matching process already exists, leave it untouched and stop the capture lane; independent static work remains available. A race or ambiguous PID/start identity also stops that lane. Hold an owned-process token/handle from spawn and require the same PID, creation identity and executable hash on every dependent debugger/cleanup operation. A historical PID or pathname alone is insufficient.

## Prepare without runtime

1. Freeze a new startup script revision under the converter's build area, retaining probe004 and its Evidence unchanged. Read module PE hashes/entry bytes and assert each targeted instruction boundary against the retained source. Run only the available syntax/schema validation before launch. Root reviews the exact hook/read set and expected compact record schema.
2. Use disposable copied test paths beneath `C:\PROBLEM`, for example `C:\PROBLEM\m2mb50a` for startup and `C:\PROBLEM\m2mb50b` for Boolean. Verify every generated full path is shorter than100 characters. These names are proposals, not directories created by this plan. Diagnostic/script/manifest artifacts remain in this checkout.
3. Record complete copied-fixture manifests and original installed EXE/DLL hashes before launch; source fixtures and binaries remain unchanged. Bind exact argv/cwd, project selection, configuration/environment/codepage, fresh-state qualification and existing-process inventory. Do not silently supply different command-line switches to reach a later phase.
4. Reuse the already-qualified native CLI invocation from root's retained launch receipt only after inspecting its actual arguments and current binary match. Do not construct an unverified Open/Save CLI from function names. The startup endpoint and full Boolean lifecycle may require different invocations/profiles.

The provider may need instrumentation bookkeeping or trampolines to install hooks. “Passive” here means the observer reads native execution/state and does not call Qt/application functions, change application data/register decisions, force callbacks, deliver messages or pump events. It does not mean zero process perturbation. Hook pauses/overhead can change timer readiness; retain them as a qualification on the observed instrumented schedule and avoid latency claims.

## Capture A: smallest PostPending state closure

Reuse probe004's first actual SplitView role/context selection and its known terminal endpoint `d35a80` return. The correction is limited to dispatcher recognition and, only if needed, one source-bound wake-branch site. Do not extend this startup run to claim project Open/Boolean timing.

| Selected site | Passive read |
| --- | --- |
| Core+11ae90 post entry/return in the actual constructor context | Actual receiver, incoming event, default priority, initial affinity; maintain balanced context IDs. |
| Core+11afc6 before insertion under lock | Locked owner RDI, receiver R12, event RSI; receiver-private affinity/count; threadData+30 ordered queue/backing/refcount/begin/end/record and event identities. Entry-cached thread identity is not authoritative after native affinity recheck. |
| Core+11afe8 after insertion under lock | Same owner and identities, ordered new record, event flags before posted-bit/count/canWait stores. |
| Core+11b015 after unlock | Actual posted count, event flag, threadData+50 canWait and threadData+20 dispatcher. |
| Core+141b70 wake entry/return inside this selected post | Same dispatcher/private owner; private+98 HWND, +a8 serial, +b4 postedWake. Admit exact direct Core target or QtGui+5e0ca0 FF25 thunk only when loaded IAT QtGui+697618 resolves to Core+141b70 without this adjustment. |
| Optional Core+141b9b or +141bad | Successful-CAS/PostMessage-call branch reached, with source-defined register arguments. Add only if before/after cannot distinguish the actual branch; byte-guard each new site. Do not invoke PostMessage. |
| Existing native post return and constructor endpoint | Preserve post return, private+c8 store, stack Create teardown and final endpoint ordering without completing any reconstructed stage from observation alone. |

Accept only a complete same-process bounded queue snapshot with separately interned record, receiver and event identities, same locked owner and matched entry/return contexts. Observe serial increment before HWND/CAS branching. Startup004's19->20 and flags4->5 are training expectations to compare, not values to force. If the fresh run differs, preserve it and qualify branch selection before changing the reconstruction.

## Capture B: actual Boolean creation/event interval

This is a separate fresh owned process/profile on one disposable Boolean project. Use the native Open-to-Save invocation only if already qualified. If the native CLI cannot naturally reach the required Save/pump interval, stop with that bounded limitation; no click, key, synthetic command, direct function call, sleep-to-trigger, timer injection or explicit processEvents call is added to make it reach it.

Select one declared interval: native Open entry through return, then native Save entry through its existing flags0 processEvents return or the original's terminal failure/exit. Hooks are filtered by the selected project/diagram context and thread; no application-wide timer dump.

Record Open1400271b0 and Save1400980d0 source occurrences; diagram setModel140170430 and schedule site14017085f; QTimer singleShot67134d90/selected timer construction67134870/startTimer6712b1e0; selected QSingleShotTimer timerEvent67134ca0; diagram modelUpdated140183160; Boolean factory1411e0510, ctor1411e2480/fill1411e25b0 and apply1411e2870; and the actual Save pump140098129/Core6711d130. Reuse exact source/metadata/this-adjustment qualifications before interpreting fields. Retain raw identity plus unknown where a layout is unclosed.

The compact ledger contains monotonic observation ordinal, native thread, source site/occurrence, entry/return and enclosing action/pump/timer IDs; actual diagram/model/timer/editor identities; request interval and returned native timer ID; result/exception/endpoint. Resolve current attachment/item only at the source-defined delivery point after fit. Registration records do not store a substitute target snapshot for delivery.

No timer callback in the complete observed interval means only “not observed in this interval.” Two1000ms requests do not establish two deliveries. A positive creation claim needs actual factory/ctor and selected operation identity/context. A timer-driven replacement claim needs registration -> actual timerEvent -> modelUpdated -> old apply/new factory/fill nesting. Fit/notification descendants and any nested pumps remain explicit dependencies. Historical DB133 bytes/screenshots and editor300 observations cannot fill absent Boolean events.

## Budgets, failure and cleanup

Each run uses a fixed manifest-listed hook set, one owned process, no competing capture and no reinstrumentation after failure. Reuse probe004's existing small state/record/error bounds for A; propose at most4096 compact callback-ledger rows,1024 queue records and32 error rows for B, subject to root's script review. Any limit hit, unreadable pointer, unexpected signature, module mismatch, stale identity, unmatched frame or instrumentation error invalidates the affected positive claim; stop collection safely and retain the failed prefix. Do not silently truncate and call coverage complete.

Use a bounded root-controlled observation deadline consistent with the qualified invocation; a deadline does not authorize event delivery or process termination. Normal native exit is preferred. At a known terminal paused endpoint, or on timeout/crash, root applies only its existing authorized exact-owned-process cleanup procedure through the supported debugger/process API. If that operation is unavailable, rejected, identity-ambiguous or unauthorized, preserve `cleanup unresolved` and stop this runtime lane instead of touching other sessions or substituting app-control cleanup. Continue independent work. A timeout alone is not permission to terminate an uncertain process.

Persist Evidence/script/module/input hashes and the compact endpoint/failed-prefix receipt before cleanup. Detach/close the debugger session using its supported operation and retain status. Independently enumerate the exact owned creation identity to verify process absence, retaining separate receipts: process absent/present/unknown; debugger closed/open/unknown; GUI input acquired false/true/unknown. Never collapse these into one “control released” field. Do not close or kill any user-owned process.

Original process termination at a deliberately early capture endpoint is not normal project Close acceptance. Keep partial directory effects and the last observed phase, including any external settings effects; do not restore or alter user projects/configuration to manufacture fresh state. If the original reaches a dialog requiring UI input, preserve the blocked prefix and end this no-GUI lane without selecting even its default.

## Root handoff and acceptance

The root's executable next step is preparation/review of the new frozen passive script plus current provider capability/ownership preflight, not an app-control acquisition. Root alone launches and cleans any approved capture. This worker created no probe, process, debugger session, GUI session or test directory.

Capture A supplies source-bound same-process queue/wake inputs for a future posting continuation; the checkpoint48 removal helper is not that continuation. Capture B supplies actual delivery/action nesting for qualified DueObservation bindings and initial/replacement Boolean editor timing. Both require source integration and focused tests before a public guard can move. Neither establishes a full fresh Open/Save/Close acceptance result by itself.


UTC: 2026-10-24T18:30:05.557412+00:00. Source qualifications remain attached to the prior contract. No runtime validation was performed.

| Planning input | SHA-256 |
| --- | --- |
| `analysis/resume49-boolean-postpending-contract.md` | `dc244caeac2bc6bb35be0f58998975d94566f359cf25919c8101b69ce7384a17` |
| `analysis/wave3-original-splitter-probe-004-receipt.json` | `edadc1a656191b75f9cbc4666ee5439c973f6b3bdbf6b6a46cf5002102e1faed` |
| `analysis/wave3-close-widget-constructor-tail-probe-spec.json` | `ecf98dbde619cf3ef2d4f81f16e3d056803b7e299af02f8f2fe55d694b1c4dde` |
| `parent analysis/gui-control-lifecycle-assessment.json` | `5a1657cabd718a7674a2a70aaf350d4186e85bc19704e5a96318c55cb5d79d0b` |
| `AGENTS.md` | `ba6c03bde363022f9e10eddd1693c765aa176ffd65814855583e510d783d5c2b` |
